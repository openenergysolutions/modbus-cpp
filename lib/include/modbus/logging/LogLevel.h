#ifndef MODBUS_LOGLEVEL_H
#define MODBUS_LOGLEVEL_H

namespace modbus
{
namespace logging
{

enum class LogLevel
{
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    err = 4,
    critical = 5
};

} // namespace logging
} // namespace modbus

#endif // define MODBUS_LOGLEVEL_H