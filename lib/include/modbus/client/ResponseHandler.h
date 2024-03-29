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
#ifndef MODBUS_RESPONSEHANDLER_H
#define MODBUS_RESPONSEHANDLER_H

/** @file
* @brief Typedef @ref modbus::ResponseHandler
*/

#include <functional>

#include "modbus/Expected.h"

namespace modbus
{

/**
 * @brief Response handler used in callbacks
 *
 * The response handler will be called with the expected response. Be sure to
 * check that the expectation is valid with @ref Expected::is_valid() before using it.
 *
 * @warning All the response handlers are called from background threads created by
 * the @ref IModbusManager. Therefore, all response handlers *should not block* in any
 * way. Otherwise, you risk having other channels not performing their tasks because
 * the thread is blocked or even deadlocks. Also, be sure to use the appropriate mechanisms
 * to eliminate the risks of race conditions in the user code.
 *
 * @see @ref IClientSession
 */
template<typename TResponse>
using ResponseHandler = std::function<void(const Expected<TResponse>&)>;

} // namespace modbus

#endif //MODBUS_RESPONSEHANDLER_H
