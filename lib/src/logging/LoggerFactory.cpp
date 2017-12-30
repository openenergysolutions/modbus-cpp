#include "modbus/logging/LoggerFactory.h"

#include "spdlog/spdlog.h"

#include "logging/Logger.h"

namespace modbus
{

std::shared_ptr<Logger> LoggerFactory::CreateConsoleLogger(const std::string& name)
{
    auto spdLogger = spdlog::stdout_color_mt(name);
    return std::make_shared<Logger>(spdLogger);
}

std::shared_ptr<Logger> LoggerFactory::CreateCustomLogger(const std::string& name, std::shared_ptr<spdlog::logger> customLogger)
{
    return std::make_shared<Logger>(customLogger);
}

} // namespace modbus
