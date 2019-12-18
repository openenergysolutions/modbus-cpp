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
#ifndef MODBUS_ITIMER_H
#define MODBUS_ITIMER_H

/** @file
* @brief Interface @ref modbus::ITimer
*/

#include "modbus/Typedefs.h"

namespace modbus
{

/**
 * @brief Timer interface for custom polling
 *
 * This class is returned by @ref IClientSession::start() method in order to offer the user
 * the ability to implement custom polling behaviors.
 */
class ITimer
{
public:
    virtual ~ITimer() = default;

    /**
     * @brief Cancel the execution of the timer
     *
     * The action won't be called except if it was already
     * queued in a thread.
     */
    virtual void cancel() = 0;

    /**
     * @brief Get the time at which the timer will expire
     * @return Time at which the timer will expire
     */
    virtual steady_time_t expires_at() = 0;
};

} // namespace modbus

#endif //MODBUS_ITIMER_H
