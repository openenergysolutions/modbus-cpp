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
#include "server/channel/ServerConnectionListenerBuilder.h"

#include "server/channel/ServerConnectionListener.h"

namespace modbus
{

ServerConnectionListenerBuilder::ServerConnectionListenerBuilder(std::weak_ptr<IServerChannelImpl> channel)
    : m_channel{channel}
{

}

std::unique_ptr<IConnectionListener> ServerConnectionListenerBuilder::build(std::weak_ptr<ITcpConnection> connection)
{
    return std::make_unique<ServerConnectionListener>(m_channel, connection);
}

} // namespace modbus
