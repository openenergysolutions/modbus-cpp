#ifndef MODBUS_LOGGERFACTORY_H
#define MODBUS_LOGGERFACTORY_H

#include <memory>
#include <string>

namespace spdlog
{
    class logger;
}

namespace modbus
{
namespace logging
{

class Logger;

class LoggerFactory
{
public:
    std::shared_ptr<Logger> CreateConsoleLogger(const std::string& name);
    std::shared_ptr<Logger> CreateCustomLogger(const std::string& name, std::shared_ptr<spdlog::logger> customLogger);
};

} // namespace logging
} // namespace modbus

#endif // define MODBUS_LOGGERFACTORY_H