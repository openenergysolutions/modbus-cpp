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
    :m_io_service{std::make_shared<asio::io_service>()},
     m_logger{std::move(logger)},
     m_thread_pool{m_io_service, number_of_threads}
{

}

ModbusManagerImpl::~ModbusManagerImpl()
{
    shutdown();
}

std::shared_ptr<IChannel> ModbusManagerImpl::create_tcp_channel(const Ipv4Endpoint& endpoint,
                                                                std::unique_ptr<ISchedule> channel_retry_schedule)
{
    auto executor = std::make_shared<openpal::StrandExecutor>(m_io_service);
    auto tcp_connection = std::make_shared<AsioTcpConnection>(m_io_service, executor->strand, endpoint);
    auto channel = std::make_shared<ChannelTcp>(executor, m_logger->clone("channel"), tcp_connection);

    m_created_channels.emplace_back(channel);

    return channel;
}

void ModbusManagerImpl::shutdown()
{
    for(auto channel_ptr : m_created_channels)
    {
        auto channel = channel_ptr.lock();
        if(channel)
        {
            channel->shutdown();
        }
    }
}

} // namespace modbus
