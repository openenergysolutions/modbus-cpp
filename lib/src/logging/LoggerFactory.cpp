/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "modbus/logging/LoggerFactory.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "logging/Logger.h"
#include "logging/LoggingLevelConversion.h"

namespace modbus
{

std::shared_ptr<Logger> LoggerFactory::create_console_logger(const std::string& name, const LoggingLevel level)
{
    auto spd_logger = spdlog::stdout_color_mt(name);
    spd_logger->set_level(to_spdlog_level(level));
    return std::make_shared<Logger>(spd_logger);
}

std::shared_ptr<Logger> LoggerFactory::create_null_logger(const std::string& name)
{
    auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
    auto spd_logger = std::make_shared<spdlog::logger>(name, null_sink);
    return std::make_shared<Logger>(spd_logger);
}

std::shared_ptr<Logger> LoggerFactory::create_custom_logger(std::shared_ptr<spdlog::logger> custom_logger, LoggingLevel level)
{
    custom_logger->set_level(to_spdlog_level(level));
    return std::make_shared<Logger>(custom_logger);
}

} // namespace modbus
