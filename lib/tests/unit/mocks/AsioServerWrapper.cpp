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
#include "mocks/AsioServerWrapper.h"

AsioServerWrapper::AsioServerWrapper(std::shared_ptr<modbus::AsioServer> server)
        : m_server{std::move(server)}
{

}

AsioServerWrapper::~AsioServerWrapper()
{
    m_server->shutdown();
}

std::shared_ptr<modbus::AsioServer> AsioServerWrapper::get()
{
    return m_server;
}

modbus::AsioServer* AsioServerWrapper::operator->()
{
    return m_server.get();
}
