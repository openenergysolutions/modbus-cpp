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
#ifndef MODBUS_ISERVERCONNECTIONLISTENERBUILDERMOCK_H
#define MODBUS_ISERVERCONNECTIONLISTENERBUILDERMOCK_H

#include <condition_variable>
#include <mutex>
#include "exe4cpp/Typedefs.h"

#include "server/channel/IServerConnectionListenerBuilder.h"

class IServerConnectionListenerBuilderMock : public modbus::IServerConnectionListenerBuilder
{
public:
    IServerConnectionListenerBuilderMock(exe4cpp::duration_t timeout);

    std::unique_ptr<modbus::IConnectionListener> build(std::shared_ptr<modbus::ITcpConnection> connection) override;

    unsigned int get_num_connections() const;
    bool wait_for_connection();

private:
    exe4cpp::duration_t m_timeout;
    std::mutex m_connection_lock;
    std::condition_variable m_connection_cv;
    bool m_pending_connection;
    unsigned int m_num_connections;
};

#endif //MODBUS_ISERVERCONNECTIONLISTENERBUILDERMOCK_H
