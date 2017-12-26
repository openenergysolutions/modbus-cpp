#ifndef MODBUS_CUSTOMLOGGERSINK_H
#define MODBUS_CUSTOMLOGGERSINK_H

#include <memory>

#include "spdlog/spdlog.h"

#include "modbus/logging/ILoggerSink.h"

namespace modbus
{
namespace logging
{

class CustomLoggerSink : public spdlog::sinks::sink
{
public:
    CustomLoggerSink(std::shared_ptr<ILoggerSink> loggerSink);

    void log(const spdlog::details::log_msg& msg) override;
    void flush() override;

private:
    LogLevel convertSpdLogLevel(spdlog::level::level_enum spdLogLevel);

    std::shared_ptr<ILoggerSink> m_loggerSink;
};

} // namespace logging
} // namespace modbus

#endif // MODBUS_CUSTOMLOGGERSINK_H