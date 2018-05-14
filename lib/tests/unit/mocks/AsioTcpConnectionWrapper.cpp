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
#include "mocks/AsioTcpConnectionWrapper.h"

AsioTcpConnectionWrapper::AsioTcpConnectionWrapper(std::shared_ptr<modbus::AsioTcpConnection> connection)
        : m_connection{std::move(connection)}
{

}

AsioTcpConnectionWrapper::~AsioTcpConnectionWrapper()
{
    m_connection->close();
}

std::shared_ptr<modbus::AsioTcpConnection> AsioTcpConnectionWrapper::get()
{
    return m_connection;
}

modbus::AsioTcpConnection* AsioTcpConnectionWrapper::operator->()
{
    return m_connection.get();
}
