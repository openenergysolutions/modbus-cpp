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
#ifndef MODBUS_LOGGERFACTORY_H
#define MODBUS_LOGGERFACTORY_H

/** @file
 * @brief Class @ref modbus::LoggerFactory
 */

#include <memory>
#include <string>
#include "modbus/logging/LoggingLevel.h"

namespace spdlog
{
    class logger;
}

namespace modbus
{

/**
 * @brief Pointer type for a logger
 *
 * This type is only used internally. It is made available here in order to pass
 * it to the @ref IModbusManager.
 *
 * @see @ref LoggerFactory, @ref IModbusManager
 */
class Logger;

/**
 * @brief Factory of loggers
 *
 * Creates loggers using <a href="https://github.com/gabime/spdlog">spdlog</a>. Note that
 * the spdlog is completely private dependency and you don't need it, except if you compile from the sources.
 *
 * If you are already using spdlog in your application, you can use
 * @ref create_custom_logger(std::shared_ptr<spdlog::logger> custom_logger) to use an existing
 * logger.
 *
 * @see @ref IModbusManager
 */
class LoggerFactory
{
public:
    /**
     * @brief Creates a console logger
     * @param name  Name associated with the logger
     * @param level Logging level of this logger
     * @return Shared pointer of the logger
     *
     * The console logger will be colored and is completely thread-safe. It uses
     * @cpp spdlog::stdout_color_mt @ce under the hood.
     */
    static std::shared_ptr<Logger> create_console_logger(const std::string& name, LoggingLevel level = LoggingLevel::Info);

    /**
     * @brief Creates a logger that simply drops all the messages
     * @param name  name associated with the logger
     * @return Shared pointer of the logger
     *
     * This logger is useful if you don't want to provide logging (e.g. in tests). It simply ignores all the messages
     * passed to it. It uses a @cpp spdlog::sinks::null_sink_st @ce under the hood.
     */
    static std::shared_ptr<Logger> create_null_logger(const std::string& name);

    /**
     * @brief Creates a logger from a custom logger
     * @param custom_logger Shared pointer to a @cpp spdlog::logger @ce logger
     * @param level Logging level of this logger
     * @return Shared pointer of the logger
     *
     * Use this method if you already use spdlog and want to use a custom logger.
     */
    static std::shared_ptr<Logger> create_custom_logger(std::shared_ptr<spdlog::logger> custom_logger, const LoggingLevel level = LoggingLevel::Info);
};

} // namespace modbus

#endif // define MODBUS_LOGGERFACTORY_H