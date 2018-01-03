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
    static std::shared_ptr<Logger> create_console_logger(const std::string& name);
    static std::shared_ptr<Logger> create_custom_logger(const std::string& name,
                                                        std::shared_ptr<spdlog::logger> custom_logger);
};

} // namespace modbus

#endif // define MODBUS_LOGGERFACTORY_H