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
#ifndef MODBUS_READCOILSREQUEST_H
#define MODBUS_READCOILSREQUEST_H

/** @file
 * @brief Struct @ref modbus::ReadCoilsRequest
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Read Coils request message
 *
 * Read the content of contiguous status of coils.
 *
 * This is the function code @cpp 0x01 @ce of the Modbus protocol.
 *
 * @see @ref ReadCoilsResponse, @ref ReadDiscreteInputsRequest
 */
struct ReadCoilsRequest
{
    /**
     * @brief Maximum number of coils that can be requested
     *
     * Its value is 2000.
     */
    constexpr static uint16_t max_bits = 2000;

    /**
     * @brief Starting address to read from
     *
     * The starting address must be in a valid range of the device.
     */
    Address starting_address;

    /**
     * @brief Quantity of coils to read
     *
     * The quantity must be between 1 and 2000 inclusively and must be in a valid range of the device.
     */
    uint16_t qty_of_bits;
};

} // namespace modbus

#endif //MODBUS_READCOILSREQUEST_H
