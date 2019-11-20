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
#include "ModbusManagerImpl.h"

#include "exe4cpp/asio/StrandExecutor.h"
#include "modbus/channel/Ipv4Endpoint.h"
#include "channel/ChannelTcp.h"
#include "channel/AsioTcpConnection.h"
#include "logging/Logger.h"

namespace modbus
{

ModbusManagerImpl::ModbusManagerImpl(std::shared_ptr<Logger> logger, unsigned int number_of_threads)
    : m_io_service{std::make_shared<asio::io_service>()},
      m_logger{std::move(logger)},
      m_thread_pool{m_io_service, number_of_threads}
{

}

ModbusManagerImpl::~ModbusManagerImpl()
{
    shutdown();
}

std::shared_ptr<IChannel> ModbusManagerImpl::create_tcp_channel(const std::string& name,
                                                                const Ipv4Endpoint& endpoint,
                                                                const std::string& adapter,
                                                                const LoggingLevel level)
{
    auto executor = std::make_shared<exe4cpp::StrandExecutor>(m_io_service);

    auto connection_logger = m_logger->clone(name + " - Connection", level);
    auto tcp_connection = std::make_shared<AsioTcpConnection>(connection_logger,
                                                              executor,
                                                              endpoint,
                                                              adapter);

    auto channel = std::make_shared<ChannelTcp>(executor, m_logger->clone(name, level), tcp_connection);

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_created_channels.emplace_back(channel);
    }

    return channel;
}

void ModbusManagerImpl::shutdown()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for(auto channel : m_created_channels)
    {
        channel->shutdown();
    }

    m_created_channels.clear();
}

} // namespace modbus
