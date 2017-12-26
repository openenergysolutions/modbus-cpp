#include "ModbusManagerImpl.h"

#include "asio.hpp"

namespace modbus
{

using namespace logging;

ModbusManagerImpl::ModbusManagerImpl(std::shared_ptr<logging::Logger> logger)
    :m_logger(logger)
{

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
