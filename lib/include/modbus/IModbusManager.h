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
#ifndef MODBUS_IMODBUSMANAGER_H
#define MODBUS_IMODBUSMANAGER_H

/** @file
* @brief Interface @ref modbus::IModbusManager
*/

#include <string>
#include <memory>
#include "modbus/Ipv4Endpoint.h"
#include "modbus/client/IClientChannel.h"
#include "modbus/logging/LoggingLevel.h"

namespace modbus
{

class Logger;

/**
 * @brief Main entry point of modbus-cpp
 *
 * Use @ref create(std::shared_ptr<Logger> logger) or
 * @ref create(std::shared_ptr<Logger> logger, unsigned int number_of_threads) to effectively create an instance
 * of this class.
 *
 * Once you get an instance of this class, one or multiple background threads will be running to execute all
 * the Modbus I/O.
 *
 * From this class, you can create a channel using
 * @ref create_tcp_channel(const std::string& name, const Ipv4Endpoint& endpoint) from which you will be
 * able to create sessions.
 */
class IModbusManager
{
public:
    /**
     * @brief Creates an instance of @ref IModbusManager
     * @param logger    Logger to use for all the operations
     * @return Unique pointer to an instance of @ref IModbusManager
     *
     * This function will create the number of threads returned by @cpp std::thread::hardware_concurrency @ce.
     * It is usually a good default value.
     *
     * The returned instance can safely be put in a @cpp std::shared_ptr @ce if necessary.
     *
     * @see @ref LoggerFactory
     */
    static std::unique_ptr<IModbusManager> create(std::shared_ptr<Logger> logger);
    /**
     * @brief Creates an instance of @ref IModbusManager
     * @param logger            Logger to use for all the operations
     * @param number_of_threads Number of threads to use by the library
     * @return Unique pointer to an instance of @ref IModbusManager
     *
     * The number of threads to use is often found by trial and error. It depends on the machine
     * used, as well as the number of active channels and their polling frenquency. A good start
     * is usually @cpp std::thread::hardware_concurrency @ce.
     *
     * The returned instance can safely be put in a @cpp std::shared_ptr @ce if necessary.
     *
     * @see @ref LoggerFactory
     */
    static std::unique_ptr<IModbusManager> create(std::shared_ptr<Logger> logger, unsigned int number_of_threads);

public:
    /**
     * @brief Destructor
     *
     * When destroyed, it will effectively call @ref shutdown().
     *
     * @warning When this destructor is called, it will effectively close all the channels
     * and their associated sessions. However, since operations may still be queued in other
     * threads and that all the threads are gracefully ended, it may take some time to completely
     * join all the threads.
     *
     * @see @ref shutdown()
     */
    virtual ~IModbusManager() = default;

    /**
     * @brief Create a TCP channel.
     * @param name      Name associated with the channel. This name will appear in the logs.
     * @param endpoint  IPv4 endpoint to which the channel will be connected.
     * @param level     Logging level of the channel.
     * @returns Shared pointer of a @ref IChannel instance.
     *
     * @note The returned channel instance is shared with the internal of the library. If you
     * release the shared pointer, it will be kept alive by the internal of the library. The channel
     * will be effectively destroyed when @ref shutdown is called.
     */
    virtual std::shared_ptr<IClientChannel> create_tcp_channel(const std::string& name,
                                                               const Ipv4Endpoint& endpoint,
                                                               const LoggingLevel level = LoggingLevel::Info) = 0;

    /**
     * @brief Closes all the associated channels and sessions and join all the background threads.
     *
     * All the associated channels and sessions are gracefully shutdown and the threads are joined.
     *
     * @warning This method effectively blocks until all the background threads are joined. It can be
     * a source of deadlocks if the client code is blocked in a callback.
     */
    virtual void shutdown() = 0;

protected:
    IModbusManager() = default;
};

} // namespace modbus

#endif // MODBUS_IMODBUSMANAGER_H
