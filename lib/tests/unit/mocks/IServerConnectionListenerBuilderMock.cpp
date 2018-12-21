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
#include "mocks/IServerConnectionListenerBuilderMock.h"

#include "mocks/ConnectionListenerMock.h"

using namespace modbus;

IServerConnectionListenerBuilderMock::IServerConnectionListenerBuilderMock(exe4cpp::duration_t timeout)
    : m_timeout{timeout},
      m_pending_connection{false},
      m_num_connections{0}
{

}

std::unique_ptr<IConnectionListener> IServerConnectionListenerBuilderMock::build(std::shared_ptr<ITcpConnection> connection)
{
    std::lock_guard<std::mutex> lock(m_connection_lock);
    ++m_num_connections;
    m_pending_connection = true;
    m_connection_cv.notify_all();

    return std::make_unique<ConnectionListenerMock>();
}

unsigned int IServerConnectionListenerBuilderMock::get_num_connections() const
{
    return m_num_connections;
}

bool  IServerConnectionListenerBuilderMock::wait_for_connection()
{
    std::unique_lock<std::mutex> lock(m_connection_lock);
    auto result = m_connection_cv.wait_for(lock, m_timeout, [=]() { return m_pending_connection; });
    m_pending_connection = false;

    return result;
}
