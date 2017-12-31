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

class Logger;

class LoggerFactory
{
public:
    static std::shared_ptr<Logger> CreateConsoleLogger(const std::string& name);
    static std::shared_ptr<Logger> CreateCustomLogger(const std::string& name, std::shared_ptr<spdlog::logger> customLogger);
};

} // namespace modbus

#endif // define MODBUS_LOGGERFACTORY_H