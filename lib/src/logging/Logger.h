#ifndef MODBUS_LOGGER_H
#define MODBUS_LOGGER_H

#include <memory>
#include <string>

#include "spdlog/spdlog.h"

namespace modbus
{

class Logger
{
public:
    explicit Logger(std::shared_ptr<spdlog::logger> logger_impl) : m_logger_impl(std::move(logger_impl)) {}
    virtual ~Logger() = default;

    template <typename Arg1, typename... Args> void trace(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_logger_impl->trace(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void debug(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_logger_impl->debug(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void info(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_logger_impl->info(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void warn(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_logger_impl->warn(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void error(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_logger_impl->error(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void critical(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_logger_impl->critical(fmt, arg1, args...);
    }

    template <typename T> void trace(const T& msg) { m_logger_impl->trace(msg); }
    template <typename T> void debug(const T& msg) { m_logger_impl->debug(msg); }
    template <typename T> void info(const T& msg) { m_logger_impl->info(msg); }
    template <typename T> void warn(const T& msg) { m_logger_impl->warn(msg); }
    template <typename T> void error(const T& msg) { m_logger_impl->error(msg); }
    template <typename T> void critical(const T& msg) { m_logger_impl->critical(msg); }

    std::shared_ptr<Logger> clone(const std::string& name)
    {
        auto spd_logger = std::make_shared<spdlog::logger>(name, m_logger_impl->sinks().begin(), m_logger_impl->sinks().end());
        return std::make_shared<Logger>(spd_logger);
    }

private:
    std::shared_ptr<spdlog::logger> m_logger_impl;
};

} // namespace modbus

#endif // MODBUS_LOGGER_H