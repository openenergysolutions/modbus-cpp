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
#ifndef MODBUS_MODBUSMANAGERIMPL_H
#define MODBUS_MODBUSMANAGERIMPL_H

#include <mutex>
#include "exe4cpp/asio/ThreadPool.h"
#include "modbus/IModbusManager.h"

namespace modbus
{

class ModbusManagerImpl : public IModbusManager
{
public:
    explicit ModbusManagerImpl(std::shared_ptr<Logger> logger, unsigned int number_of_threads);
    ~ModbusManagerImpl();

    std::shared_ptr<IChannel> create_tcp_channel(const std::string& name,
                                                 const Ipv4Endpoint& endpoint,
                                                 const std::string& adapter,
                                                 const LoggingLevel level) override;
    void shutdown() override;

private:
    std::shared_ptr<asio::io_service> m_io_service;
    std::shared_ptr<Logger> m_logger;
    exe4cpp::ThreadPool m_thread_pool;

    std::mutex m_mutex;
    std::vector<std::shared_ptr<IChannel>> m_created_channels;
};

} // namespace modbus

#endif // MODBUS_MODBUSMANAGERIMPL_H
