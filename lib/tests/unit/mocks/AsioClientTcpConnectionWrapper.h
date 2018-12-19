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
#ifndef MODBUS_ASIOCLIENTTCPCONNECTIONWRAPPER_H
#define MODBUS_ASIOCLIENTTCPCONNECTIONWRAPPER_H

#include <memory>

#include "client/channel/AsioClientTcpConnection.h"

class AsioClientTcpConnectionWrapper
{
public:
    AsioClientTcpConnectionWrapper(std::shared_ptr<modbus::AsioClientTcpConnection> connection);
    ~AsioClientTcpConnectionWrapper();

    std::shared_ptr<modbus::AsioClientTcpConnection> get();
    modbus::AsioClientTcpConnection* operator->();

private:
    std::shared_ptr<modbus::AsioClientTcpConnection> m_connection;
};

#endif //MODBUS_ASIOCLIENTTCPCONNECTIONWRAPPER_H
