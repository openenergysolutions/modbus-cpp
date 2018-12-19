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
#ifndef MODBUS_CLIENTCHANNELTCP_H
#define MODBUS_CLIENTCHANNELTCP_H

#include <deque>
#include <memory>
#include "ser4cpp/container/Buffer.h"
#include "exe4cpp/IExecutor.h"

#include "IConnectionListener.h"
#include "client/channel/IClientChannelImpl.h"
#include "client/channel/PendingRequest.h"
#include "messages/mbap/IMbapSink.h"
#include "messages/mbap/MbapParser.h"
#include "logging/Logger.h"
#include "messages/IRequest.h"

namespace modbus
{

class ITcpConnection;

class ClientChannelTcp : public IClientChannelImpl, public IConnectionListener, public IMbapSink
{
public:
    ClientChannelTcp(std::shared_ptr<exe4cpp::IExecutor> executor,
                     std::shared_ptr<Logger> logger,
                     std::shared_ptr<ITcpConnection> tcp_connection);
    ~ClientChannelTcp() = default;

    std::shared_ptr<IClientSession> create_session(const UnitIdentifier& unit_identifier,
                                                   const exe4cpp::duration_t& default_timeout) override;

    void send_request(const UnitIdentifier& unit_identifier,
                      const IRequest& request,
                      const exe4cpp::duration_t& timeout,
                      ResponseHandler<ser4cpp::rseq_t> response_handler) override;

    void shutdown() override;

    // Connection listener
    void on_receive(const ser4cpp::rseq_t& data) override;
    void on_error(const std::string& message) override;

    // MBAP sink
    void on_mbap_message(const MbapMessage& message) override;

private:
    void check_pending_requests();
    void cancel_current_request();
    void cancel_all_pending_requests(const std::string& message);

    std::shared_ptr<exe4cpp::IExecutor> m_executor;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<ITcpConnection> m_tcp_connection;

    bool m_is_shutdown;
    std::vector<std::shared_ptr<IClientSession>> m_sessions;
    MbapParser m_parser;
    std::deque<std::unique_ptr<PendingRequest>> m_pending_requests;
    std::unique_ptr<PendingRequest> m_current_request;
    exe4cpp::Timer m_current_timer;
    TransactionIdentifier m_next_transaction_id;
};

} // namespace modbus

#endif //MODBUS_CLIENTCHANNELTCP_H
