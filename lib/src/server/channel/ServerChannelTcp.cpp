/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "server/channel/ServerChannelTcp.h"

#include "ser4cpp/container/SequenceTypes.h"
#include "ser4cpp/container/StaticBuffer.h"
#include "ser4cpp/serialization/BigEndian.h"

#include "messages/ExceptionResponse.h"
#include "messages/ReadCoilsRequestImpl.h"
#include "messages/ReadCoilsResponseImpl.h"
#include "messages/ReadDiscreteInputsRequestImpl.h"
#include "messages/ReadDiscreteInputsResponseImpl.h"
#include "messages/ReadHoldingRegistersRequestImpl.h"
#include "messages/ReadHoldingRegistersResponseImpl.h"
#include "messages/ReadInputRegistersRequestImpl.h"
#include "messages/ReadInputRegistersResponseImpl.h"
#include "messages/WriteMultipleCoilsRequestImpl.h"
#include "messages/WriteMultipleCoilsResponseImpl.h"
#include "messages/WriteMultipleRegistersRequestImpl.h"
#include "messages/WriteMultipleRegistersResponseImpl.h"
#include "messages/WriteSingleCoilRequestImpl.h"
#include "messages/WriteSingleCoilResponseImpl.h"
#include "messages/WriteSingleRegisterRequestImpl.h"
#include "messages/WriteSingleRegisterResponseImpl.h"
#include "server/channel/ServerConnectionListenerBuilder.h"

namespace modbus
{

ServerChannelTcp::ServerChannelTcp(std::shared_ptr<Logger> logger,
                                   std::shared_ptr<exe4cpp::IExecutor> executor,
                                   std::shared_ptr<IServer> server)
    : m_logger{std::move(logger)},
      m_executor{std::move(executor)},
      m_server{std::move(server)},
      m_is_shutdown{false},
      m_is_started{false}
{

}

void ServerChannelTcp::start()
{
    if(m_is_shutdown) return;

    m_executor->post([=, self=shared_from_this()] {
        if(m_is_shutdown) return;

        m_server->start(std::make_shared<ServerConnectionListenerBuilder>(std::dynamic_pointer_cast<ServerChannelTcp>(shared_from_this())));
        m_is_started = true;
    });
}

void ServerChannelTcp::shutdown()
{
    m_executor->post([this, self=shared_from_this()] {
        m_logger->info("Shutting down.");
        m_is_shutdown = true;

        if(m_is_started)
        {
            m_server->shutdown();
            m_is_started = false;
        }
        
        for(auto session : m_sessions)
        {
            session.second->shutdown();
        }
        m_sessions.clear();
    });
}

void ServerChannelTcp::add_session(const UnitIdentifier& unit_identifier, std::shared_ptr<IServerSession> session)
{
    if(m_is_shutdown) return;

    m_executor->post([=, self=shared_from_this()]() {
        if(m_is_shutdown) return;

        m_sessions.insert({unit_identifier, session});
    });
}

void ServerChannelTcp::on_mbap(const MbapMessage& message, ITcpConnection& connection)
{
    if(m_is_shutdown) return;
    
    auto it = m_sessions.find(message.unit_id);
    if(it != m_sessions.end())
    {
        auto view = message.data;
        if(view.length() < 1)
        {
            m_logger->warn("Received invalid message.");
            return;
        }

        uint8_t function_code;
        ser4cpp::UInt8::read_from(view, function_code);
        switch(function_code)
        {
        case 0x01:
            process_message<ReadCoilsRequestImpl, ReadCoilsResponseImpl>(it->second, message, function_code, connection);
            break;
        case 0x02:
            process_message<ReadDiscreteInputsRequestImpl, ReadDiscreteInputsResponseImpl>(it->second, message, function_code, connection);
            break;
        case 0x03:
            process_message<ReadHoldingRegistersRequestImpl, ReadHoldingRegistersResponseImpl>(it->second, message, function_code, connection);
            break;
        case 0x04:
            process_message<ReadInputRegistersRequestImpl, ReadInputRegistersResponseImpl>(it->second, message, function_code, connection);
            break;
        case 0x05:
            process_message<WriteSingleCoilRequestImpl, WriteSingleCoilResponseImpl>(it->second, message, function_code, connection);
            break;
        case 0x06:
            process_message<WriteSingleRegisterRequestImpl, WriteSingleRegisterResponseImpl>(it->second, message, function_code, connection);
            break;
        case 0x0F:
            process_message<WriteMultipleCoilsRequestImpl, WriteMultipleCoilsResponseImpl>(it->second, message, function_code, connection);
            break;
        case 0x10:
            process_message<WriteMultipleRegistersRequestImpl, WriteMultipleRegistersResponseImpl>(it->second, message, function_code, connection);
            break;
        default:
            m_logger->warn("Received unsupported function code: {}", function_code);
            send_message(connection, message.unit_id, message.transaction_id, ExceptionResponse{function_code, ExceptionType::IllegalFunction});
        }
    }
    else
    {
        m_logger->warn("Received message for unknown unit: {}", message.unit_id);
    }
}

template<typename TRequest, typename TResponse>
void ServerChannelTcp::process_message(std::shared_ptr<IServerSession> session,
                                       const MbapMessage& message,
                                       uint8_t function_code,
                                       ITcpConnection& connection)
{
    // Parse the request
    auto parse_result = TRequest::parse(message.data);
    if(!parse_result.is_valid())
    {
        m_logger->warn("Received invalid message.");

        // If the parser returned a Modbus exception, we send it to the client.
        if(parse_result.template has_exception<ModbusException>())
        {
            auto exception = parse_result.template get_exception<ModbusException>();
            ExceptionResponse response{function_code, exception.get_exception_type()};
            send_message(connection, message.unit_id, message.transaction_id, response);
        }

        // Otherwise, we send nothing
        return;
    }
    auto request = parse_result.get();

    // Ask the session what to return
    auto response_result = session->on_request(request);
    if(!response_result.is_valid())
    {
        // If the user returned a Modbus exception, then send the exception
        if(response_result.template has_exception<ModbusException>())
        {
            auto exception = response_result.template get_exception<ModbusException>();
            ExceptionResponse response{function_code, exception.get_exception_type()};
            send_message(connection, message.unit_id, message.transaction_id, response);
        }

        // Otherwise, we send nothing
        return;
    }

    // Build the response
    auto response = response_result.get();
    TResponse formatted_response{response};

    // Send the response to the connection
    send_message(connection, message.unit_id, message.transaction_id, formatted_response);
}

void ServerChannelTcp::send_message(ITcpConnection& connection,
                                    const UnitIdentifier& unit_id,
                                    const TransactionIdentifier& transaction_id,
                                    const IMessage& message)
{
    ser4cpp::StaticBuffer<uint32_t, 260> buffer;
    auto response_view = buffer.as_wseq();
    auto serialized_request = MbapMessage::build_message(unit_id,
                                                         transaction_id,
                                                         message,
                                                         response_view);
    connection.send(serialized_request);
}

} // namespace modbus
