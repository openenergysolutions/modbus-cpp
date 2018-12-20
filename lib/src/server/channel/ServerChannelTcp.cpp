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

#include "messages/ReadCoilsRequestImpl.h"
#include "messages/ReadCoilsResponseImpl.h"
#include "server/channel/ServerConnectionListenerBuilder.h"

namespace modbus
{

ServerChannelTcp::ServerChannelTcp(std::shared_ptr<Logger> logger,
                                   std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                   std::shared_ptr<IServer> server)
    : m_logger{std::move(logger)},
      m_executor{std::move(executor)},
      m_server{std::move(server)}
{

}

ServerChannelTcp::~ServerChannelTcp()
{
    shutdown();
}

void ServerChannelTcp::start()
{
    m_server->start(std::make_shared<ServerConnectionListenerBuilder>(std::dynamic_pointer_cast<ServerChannelTcp>(shared_from_this())));
}

void ServerChannelTcp::shutdown()
{
    m_server->shutdown();
    for(auto session : m_sessions)
    {
        session.second->shutdown();
    }
    m_sessions.clear();
}

void ServerChannelTcp::add_session(const UnitIdentifier& unit_identifier, std::shared_ptr<IServerSession> session)
{
    m_executor->post([=]() {
        m_sessions.insert({unit_identifier, session});
    });
}

void ServerChannelTcp::on_mbap(const MbapMessage& message, ITcpConnection& connection)
{
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
            process_message<ReadCoilsRequestImpl, ReadCoilsResponseImpl>(it->second, message, connection);
            break;
        default:
            m_logger->warn("Received unsupported function code: {}", function_code);
            // TODO: return appropriate response
        }
    }
    else
    {
        m_logger->warn("Received message for unknown unit: {}", message.unit_id);
    }
}

template<typename TRequest, typename TResponse>
void ServerChannelTcp::process_message(std::shared_ptr<IServerSession> session, const MbapMessage& message, ITcpConnection& connection)
{
    // Parse the request
    auto parse_result = TRequest::parse(message.data);
    if(!parse_result.is_valid())
    {
        m_logger->warn("Received invalid message.");
        return;
    }
    auto request = parse_result.get();

    // Ask the session what to return
    auto response_result = session->on_request(request);
    if(!response_result.is_valid())
    {
        // TODO: do something
        return;
    }
    auto response = response_result.get();

    // Build the response and send it to the connection
    TResponse formatted_response{response};
    ser4cpp::StaticBuffer<uint32_t, 260> buffer;
    auto response_view = buffer.as_wseq();
    auto serialized_request = MbapMessage::build_message(message.unit_id,
                                                         message.transaction_id,
                                                         formatted_response,
                                                         response_view);
    connection.send(serialized_request);
}

} // namespace modbus
