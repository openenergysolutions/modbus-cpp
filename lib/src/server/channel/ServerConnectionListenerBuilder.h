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
#ifndef MODBUS_SERVERCONNECTIONLISTENERBUILDER_H
#define MODBUS_SERVERCONNECTIONLISTENERBUILDER_H

#include "server/channel/IServerChannelImpl.h"
#include "server/channel/IServerConnectionListenerBuilder.h"

namespace modbus
{

class ServerConnectionListenerBuilder : public IServerConnectionListenerBuilder
{
public:
    ServerConnectionListenerBuilder(std::weak_ptr<IServerChannelImpl> channel);

    std::unique_ptr<IConnectionListener> build(std::weak_ptr<ITcpConnection> connection) override;

private:
    std::weak_ptr<IServerChannelImpl> m_channel;
};

} // namespace modbus

#endif //MODBUS_SERVERCONNECTIONLISTENERBUILDER_H
