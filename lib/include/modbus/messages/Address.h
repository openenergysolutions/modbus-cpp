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
#ifndef MODBUS_ADDRESS_H
#define MODBUS_ADDRESS_H

/** @file
 * @brief Typedef @ref modbus::Address
 */

#include <cstdint>

namespace modbus
{

/**
 * @brief Address of a register
 *
 * This is a 16-bit value.
 *
 * See section *4.4: MODBUS Addressing model* of the Modbus documentation for more details
 * about the addressing model of Modbus.
 *
 * @see @ref RegisterValue, @ref SingleBitValue,
 *      @ref ReadCoilsRequest, @ref ReadDiscreteInputsRequest,
 *      @ref ReadHoldingRegistersRequest, @ref ReadInputRegistersRequest,
 *      @ref WriteMultipleCoilsRequest, @ref WriteMultipleCoilsResponse
 *      @ref WriteMultipleRegistersRequest, @ref WriteMultipleRegistersResponse
 */
using Address = uint16_t;

} // namespace modbus

#endif //MODBUS_ADDRESS_H
