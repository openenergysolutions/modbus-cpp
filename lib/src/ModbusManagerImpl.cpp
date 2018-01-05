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

std::shared_ptr<IChannel> ModbusManagerImpl::create_tcp_channel(const Ipv4Endpoint& endpoint,
                                                                std::unique_ptr<ISchedule> channel_retry_schedule)
{
    auto executor = std::make_shared<openpal::StrandExecutor>(m_io_service);
    auto tcp_connection = std::make_shared<AsioTcpConnection>(m_io_service, endpoint);
    return std::make_shared<ChannelTcp>(executor, m_logger->clone("channel"), tcp_connection);
}

void ModbusManagerImpl::run()
{
    asio::io_service io;

    asio::steady_timer timer1(io);
    timer1.expires_from_now(std::chrono::seconds(5));
    timer1.async_wait([=] (const std::error_code& err) {
        m_logger->info("The end of timer 1");
    });

    asio::steady_timer timer2(io);
    timer2.expires_from_now(std::chrono::seconds(2));
    timer2.async_wait([=] (const std::error_code& err) {
        m_logger->info("The end of timer 2");
    });

    io.run();
}

} // namespace modbus
