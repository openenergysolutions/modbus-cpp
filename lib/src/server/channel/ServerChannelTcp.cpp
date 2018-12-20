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
        // TODO: Parse the message and send it to the session
    }
    else
    {
        m_logger->warn("Received message for unknow unit: {}", message.unit_id);
    }
}

} // namespace modbus
