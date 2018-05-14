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
#ifndef MODBUS_WRITESINGLEREGISTERREQUEST_H
#define MODBUS_WRITESINGLEREGISTERREQUEST_H

/** @file
 * @brief Struct @ref modbus::WriteSingleRegisterRequest
 */

#include "modbus/messages/RegisterValue.h"

namespace modbus
{

/**
 * @brief Write Single Register request message
 *
 * Write a single holding register.
 *
 * This is the function code @cpp 0x06 @ce of the Modbus protocol.
 *
 * @see @ref WriteSingleRegisterResponse, @ref WriteMultipleRegistersRequest
 */
struct WriteSingleRegisterRequest
{
    /**
     * @brief Address of the register to write and its new value
     *
     * The address must be in a valid range of the device.
     */
    RegisterValue value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERREQUEST_H
