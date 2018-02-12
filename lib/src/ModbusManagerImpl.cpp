#include "ModbusManagerImpl.h"

#include "asiopal/StrandExecutor.h"
#include "modbus/Ipv4Endpoint.h"
#include "modbus/ISchedule.h"
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
                                                                std::unique_ptr<ISchedule> channel_retry_schedule)
{
    auto executor = std::make_shared<openpal::StrandExecutor>(m_io_service);

    auto connection_logger = m_logger->clone(name + " - Connection");
    auto tcp_connection = std::make_shared<AsioTcpConnection>(connection_logger,
                                                              m_io_service,
                                                              executor->strand,
                                                              endpoint);

    auto channel = std::make_shared<ChannelTcp>(executor, m_logger->clone(name), tcp_connection);

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
