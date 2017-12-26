#include "ModbusManagerImpl.h"

#include <chrono>

#include "asio.hpp"

#include "logging/CustomLoggerSink.h"

namespace modbus
{

using namespace logging;

ModbusManagerImpl::ModbusManagerImpl()
    :m_logger(spdlog::stdout_color_mt("ModbusManagerImpl"))
{

}

ModbusManagerImpl::ModbusManagerImpl(std::shared_ptr<logging::ILoggerSink> loggerSink)
    :m_customLoggerSink(std::make_shared<CustomLoggerSink>(loggerSink)),
    m_logger(std::make_shared<spdlog::logger>("ModbusManagerImpl", m_customLoggerSink))
{

}

void ModbusManagerImpl::run()
{
    asio::io_service io;

    asio::steady_timer timer1(io);
    timer1.expires_from_now(std::chrono::seconds(5));
    timer1.async_wait([=] (const std::error_code& err) {
        m_logger.info("The end of timer 1");
    });

    asio::steady_timer timer2(io);
    timer2.expires_from_now(std::chrono::seconds(2));
    timer2.async_wait([=] (const std::error_code& err) {
        m_logger.info("The end of timer 2");
    });

    io.run();
}

} // namespace modbus
