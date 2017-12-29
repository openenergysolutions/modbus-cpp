#ifndef MODBUS_LOGGER_H
#define MODBUS_LOGGER_H

#include <memory>
#include <string>

#include "spdlog/spdlog.h"

namespace modbus
{
namespace logging
{

class Logger
{
public:
    Logger(std::shared_ptr<spdlog::logger> loggerImpl) : m_loggerImpl(loggerImpl) {}
    virtual ~Logger() = default;

    template <typename Arg1, typename... Args> void trace(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_loggerImpl->trace(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void debug(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_loggerImpl->debug(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void info(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_loggerImpl->info(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void warn(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_loggerImpl->warn(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void error(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_loggerImpl->error(fmt, arg1, args...);
    }

    template <typename Arg1, typename... Args> void critical(const char* fmt, const Arg1& arg1, const Args&... args)
    {
        m_loggerImpl->critical(fmt, arg1, args...);
    }

    template <typename T> void trace(const T& msg) { m_loggerImpl->trace(msg); }
    template <typename T> void debug(const T& msg) { m_loggerImpl->debug(msg); }
    template <typename T> void info(const T& msg) { m_loggerImpl->info(msg); }
    template <typename T> void warn(const T& msg) { m_loggerImpl->warn(msg); }
    template <typename T> void error(const T& msg) { m_loggerImpl->error(msg); }
    template <typename T> void critical(const T& msg) { m_loggerImpl->critical(msg); }

    std::shared_ptr<Logger> clone(const std::string& name)
    {
        auto spdLogger = std::make_shared<spdlog::logger>(name, m_loggerImpl->sinks().begin(), m_loggerImpl->sinks().end());
        return std::make_shared<Logger>(spdLogger);
    }

private:
    std::shared_ptr<spdlog::logger> m_loggerImpl;
};

} // namespace logging
} // namespace modbus

#endif // MODBUS_LOGGER_H