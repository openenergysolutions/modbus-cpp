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
#ifndef MODBUS_TIMETYPEDEFS_H
#define MODBUS_TIMETYPEDEFS_H

/** @file
* @brief Typedef @ref modbus::action_t and @ref modbus::duration_t
*/

#include <chrono>
#include <functional>

namespace modbus
{

/**
 * @brief A copyable void function pointer taking no argument
 */
using action_t = std::function<void()>;

/**
 * @brief Represents relative time
 */
using duration_t = std::chrono::steady_clock::duration;

/**
 * @brief Represents an absolute timepoint based on steady_clock
 */
using steady_time_t = std::chrono::steady_clock::time_point;

} // namespace modbus

#endif //MODBUS_TIMETYPEDEFS_H
