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
#ifndef MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H
#define MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H

/** @file
 * @brief Struct @ref modbus::WriteMultipleRegistersRequest
 */

#include <vector>
#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Write multiple registers request message
 *
 * Write a block of contiguous registers.
 *
 * This is the function code @cpp 0x10 @ce of the Modbus protocol.
 *
 * @see @ref WriteMultipleRegistersResponse, @ref WriteSingleRegisterRequest
 */
struct WriteMultipleRegistersRequest
{
    /**
     * @brief Starting address to write to
     *
     * The starting address must be in a valid range of the device.
     */
    Address starting_address;

    /**
     * @brief Values to write to the registers
     *
     * Note that the Modbus protocol limits the number of registers you can write in a
     * single request to 123. Therefore, the vector should have a length of 1 to 123 inclusively.
     */
    std::vector<uint16_t> values;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H
