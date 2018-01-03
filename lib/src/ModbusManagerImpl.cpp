#include "ModbusManagerImpl.h"

#include "asio.hpp"

#include "modbus/IChannel.h"
#include "modbus/Ipv4Endpoint.h"
#include "modbus/ISchedule.h"
#include "logging/Logger.h"

namespace modbus
{

ModbusManagerImpl::ModbusManagerImpl(std::shared_ptr<Logger> logger, unsigned int number_of_threads)
    :m_logger{std::move(logger)}
{

}

std::unique_ptr<IChannel> ModbusManagerImpl::create_tcp_channel(const Ipv4Endpoint& endpoint,
                                                                std::unique_ptr<ISchedule> channel_retry_schedule)
{
    return nullptr;
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
