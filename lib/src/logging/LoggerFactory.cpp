#include "modbus/logging/LoggerFactory.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/null_sink.h"

#include "logging/Logger.h"

namespace modbus
{

std::shared_ptr<Logger> LoggerFactory::create_console_logger(const std::string& name)
{
    auto spd_logger = spdlog::stdout_color_mt(name);
    return std::make_shared<Logger>(spd_logger);
}

std::shared_ptr<Logger> LoggerFactory::create_null_logger(const std::string& name)
{
    auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
    auto spd_logger = std::make_shared<spdlog::logger>(name, null_sink);
    return std::make_shared<Logger>(spd_logger);
}

std::shared_ptr<Logger> LoggerFactory::create_custom_logger(std::shared_ptr<spdlog::logger> custom_logger)
{
    return std::make_shared<Logger>(custom_logger);
}

} // namespace modbus
