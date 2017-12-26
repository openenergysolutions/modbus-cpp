#include "logging/CustomLoggerSink.h"

namespace modbus
{
namespace logging
{

CustomLoggerSink::CustomLoggerSink(std::shared_ptr<ILoggerSink> loggerSink)
    :m_loggerSink(loggerSink)
{

}

void CustomLoggerSink::log(const spdlog::details::log_msg& msg)
{
    m_loggerSink->log(*msg.logger_name, convertSpdLogLevel(msg.level), msg.formatted.str());
}

void CustomLoggerSink::flush()
{

}

LogLevel CustomLoggerSink::convertSpdLogLevel(spdlog::level::level_enum spdLogLevel)
{
    switch (spdLogLevel)
    {
    case spdlog::level::trace:
        return LogLevel::trace;
    case spdlog::level::debug:
        return LogLevel::debug;
    case spdlog::level::info:
        return LogLevel::info;
    case spdlog::level::warn:
        return LogLevel::warn;
    case spdlog::level::err:
        return LogLevel::err;
    case spdlog::level::critical:
        return LogLevel::critical;
    default:
        return LogLevel::debug;
    }
}

} // namespace logging
} // namespace modbus