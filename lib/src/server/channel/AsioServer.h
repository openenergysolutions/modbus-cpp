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
#ifndef MODBUS_ASIOSERVER_H
#define MODBUS_ASIOSERVER_H

#include <list>
#include <memory>
#include "exe4cpp/asio/StrandExecutor.h"

#include "modbus/Ipv4Endpoint.h"
#include "ITcpConnection.h"
#include "logging/Logger.h"
#include "server/channel/AsioServerTcpConnection.h"
#include "server/channel/IServer.h"

namespace modbus
{

class AsioServer : public IServer
{
public:
    AsioServer(std::shared_ptr<Logger> logger,
               std::shared_ptr<exe4cpp::StrandExecutor> executor,
               const Ipv4Endpoint& endpoint,
               const unsigned int max_connections);

    void start(std::shared_ptr<IServerConnectionListenerBuilder> connection_listener_builder) override;
    void shutdown();

private:
    void start_accept();
    void handle_accept(std::shared_ptr<AsioServerTcpConnection> connection, const asio::error_code& ec);

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<exe4cpp::StrandExecutor> m_executor;
    unsigned int m_max_connections;
    asio::ip::tcp::acceptor m_tcp_acceptor;
    std::shared_ptr<IServerConnectionListenerBuilder> m_connection_listener_builder;
    std::list<std::weak_ptr<AsioServerTcpConnection>> m_connections;
};

} // namespace modbus

#endif //MODBUS_ASIOSERVER_H
