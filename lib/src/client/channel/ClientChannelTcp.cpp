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
#include "client/channel/ClientChannelTcp.h"

#include "ser4cpp/container/StaticBuffer.h"
#include "modbus/exceptions/ConnectionException.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"
#include "modbus/exceptions/TimeoutException.h"
#include "client/channel/IClientTcpConnection.h"
#include "client/session/ClientSessionImpl.h"

namespace modbus
{

ClientChannelTcp::ClientChannelTcp(std::shared_ptr<exe4cpp::IExecutor> executor,
                                   std::shared_ptr<Logger> logger,
                                   std::shared_ptr<IClientTcpConnection> tcp_connection)
    : m_executor{std::move(executor)},
      m_logger{std::move(logger)},
      m_tcp_connection{tcp_connection},
      m_is_shutdown{false},
      m_parser{this},
      m_current_timer{nullptr},
      m_next_transaction_id{0x0000}
{
    
}

std::shared_ptr<IClientSession> ClientChannelTcp::create_session(const UnitIdentifier& unit_identifier,
                                                                 const duration_t& default_timeout)
{
    std::shared_ptr<IClientSession> session;

    if(!m_is_shutdown)
    {
        session = std::make_shared<ClientSessionImpl>(m_executor,
                                                      m_logger->clone("session"),
                                                      std::dynamic_pointer_cast<IClientChannelImpl>(shared_from_this()),
                                                      unit_identifier,
                                                      default_timeout);
        m_sessions.push_back(session);
    }

    return session;
}

void ClientChannelTcp::send_request(const UnitIdentifier& unit_identifier,
                                    const IRequest& request,
                                    const duration_t& timeout,
                                    ResponseHandler<ser4cpp::rseq_t> response_handler)
{

    if(request.is_valid())
    {
        std::shared_ptr<IRequest> req{request.clone()};
        m_executor->post([=, self = shared_from_this()]() {
            if (!m_is_shutdown)
            {
                auto pending_request = std::make_unique<PendingRequest>(unit_identifier,
                                                                        m_next_transaction_id,
                                                                        *req,
                                                                        timeout,
                                                                        response_handler);
                m_pending_requests.push_back(std::move(pending_request));

                ++m_next_transaction_id;

                check_pending_requests();
            }
        });
    }
    else
    {
        m_executor->post([=, self = shared_from_this()]() {
            auto exception = Expected<ser4cpp::rseq_t>::from_exception(MalformedModbusRequestException("Modbus request is not valid."));
            response_handler(exception);
        });
    }
}

void ClientChannelTcp::shutdown()
{
    m_logger->info("Shutting down.");
    m_executor->post([=, self = shared_from_this()] {
        m_is_shutdown = true;

        for(auto& session : m_sessions)
        {
            session->shutdown();
        }
        m_sessions.clear();

        m_pending_requests.clear();
        m_current_request.reset(nullptr);
        m_current_timer.cancel();

        m_tcp_connection->close();
    });
}

void ClientChannelTcp::on_receive(const ser4cpp::rseq_t& data)
{
    m_logger->debug("Received {} bytes of data.", data.length());

    m_parser.parse(data);
}

void ClientChannelTcp::on_error(const std::string& message)
{
    m_logger->error("Error from TCP connection, cancelling all pending requests.");

    m_parser.reset();

    cancel_all_pending_requests(message);

    if(m_current_request)
    {
        m_current_request->response_handler(Expected<ser4cpp::rseq_t>::from_exception(ConnectionException{message}));
        cancel_current_request();
    }
}

void ClientChannelTcp::on_mbap_message(const MbapMessage& message)
{
    if(m_current_request)
    {
        if(message.transaction_id == m_current_request->transaction_id &&
           message.unit_id == m_current_request->unit_id)
        {
            m_logger->info("Received response for pending request. UnitId: {}, TransactionId: {}.",
                           message.unit_id, message.transaction_id);

            m_current_timer.cancel();
            m_current_request->response_handler(Expected<ser4cpp::rseq_t>{message.data});
            m_current_request.reset(nullptr);

            check_pending_requests();
        }
        else
        {
            m_logger->warn("Received unexpected response. UnitId: {}, TransactionId: {}.",
                           message.unit_id, message.transaction_id);
        }
    }
    else
    {
        m_logger->warn("Received message while not waiting for any response. Ignoring it.");
    }
}

void ClientChannelTcp::check_pending_requests()
{
    if(!m_current_request && !m_pending_requests.empty())
    {
        m_current_request = std::move(m_pending_requests.front());
        m_pending_requests.pop_front();

        m_logger->info("Sending request. UnitId: {}, TransactionId: {}, Timeout: {} ms, Length: {} bytes.",
                       m_current_request->unit_id,
                       m_current_request->transaction_id,
                       std::chrono::duration_cast<std::chrono::milliseconds>(m_current_request->timeout).count(),
                       m_current_request->request->get_request_length());

        ser4cpp::StaticBuffer<uint32_t, 260> buffer;
        auto view = buffer.as_wseq();
        auto serialized_request = MbapMessage::build_message(m_current_request->unit_id,
                                                             m_current_request->transaction_id,
                                                             *m_current_request->request,
                                                             view);

        m_tcp_connection->set_listener(std::dynamic_pointer_cast<ClientChannelTcp>(shared_from_this()));
        m_tcp_connection->send(serialized_request);

        m_current_timer = m_executor->start(m_current_request->timeout, [=, self = shared_from_this()]() {
            if (m_current_request)
            {
                m_logger->warn("Timeout reached. UnitId: {}, TransactionId: {}.",
                    m_current_request->unit_id, m_current_request->transaction_id);

                m_current_request->response_handler(Expected<ser4cpp::rseq_t>::from_exception(TimeoutException{}));
                cancel_current_request();
            }
        });
    }
}

void ClientChannelTcp::cancel_current_request()
{
    if(m_current_request)
    {
        m_current_request.reset(nullptr);
        m_current_timer.cancel();

        check_pending_requests();
    }
}

void ClientChannelTcp::cancel_all_pending_requests(const std::string& message)
{
    for(auto& req : m_pending_requests)
    {
        req->response_handler(Expected<ser4cpp::rseq_t>::from_exception(ConnectionException{message}));
    }

    m_pending_requests.clear();
}

} // namespace modbus
