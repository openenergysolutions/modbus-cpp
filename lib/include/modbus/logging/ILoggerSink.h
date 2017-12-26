#ifndef MODBUS_ILOGGERSINK_H
#define MODBUS_ILOGGERSINK_H

#include <string>

#include "modbus/logging/LogLevel.h"

namespace modbus
{
namespace logging
{

class ILoggerSink
{
public:
    ILoggerSink() = default;
    virtual ~ILoggerSink() = default;

    virtual void log(const std::string& loggerName, LogLevel logLevel, const std::string& msg) = 0;
};

} // namespace logging
} // namespace modbus

#endif // define MODBUS_ILOGGERSINK_H