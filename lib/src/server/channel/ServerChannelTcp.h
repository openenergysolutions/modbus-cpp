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
#ifndef MODBUS_SERVERCHANNELTCP_H
#define MODBUS_SERVERCHANNELTCP_H

#include <memory>
#include <unordered_map>

#include "exe4cpp/asio/StrandExecutor.h"

#include "logging/Logger.h"
#include "server/channel/IServer.h"
#include "server/channel/IServerChannelImpl.h"

namespace modbus
{

class ServerChannelTcp final : public IServerChannelImpl
{
public:
    ServerChannelTcp(std::shared_ptr<Logger> logger,
                     std::shared_ptr<exe4cpp::IExecutor> executor,
                     std::shared_ptr<IServer> server);
    ~ServerChannelTcp();

    void start() final;
    void shutdown() final;
    void add_session(const UnitIdentifier& unit_identifier, std::shared_ptr<IServerSession> session) final;
    void on_mbap(const MbapMessage& message, ITcpConnection& connection) final;

private:
    template<typename TRequest, typename TResponse>
    void process_message(std::shared_ptr<IServerSession> session,
                         const MbapMessage& message,
                         uint8_t function_code,
                         ITcpConnection& connection);
    void send_message(ITcpConnection& connection,
                      const UnitIdentifier& unit_id,
                      const TransactionIdentifier& transaction_id,
                      const IMessage& message);

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<exe4cpp::IExecutor> m_executor;
    std::shared_ptr<IServer> m_server;

    bool m_is_started;
    std::unordered_map<UnitIdentifier, std::shared_ptr<IServerSession>> m_sessions;
};

} // namespace modbus

#endif //MODBUS_SERVERCHANNELTCP_H
