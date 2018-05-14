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
#ifndef MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
#define MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H

/** @file
 * @brief Struct @ref modbus::WriteMultipleRegistersResponse
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Write multiple registers response message
 *
 * Response to @ref WriteMultipleRegistersRequest.
 *
 * This is the response for function code @cpp 0x10 @ce of the Modbus protocol.
 *
 * @see @ref WriteMultipleRegistersRequest
 */
struct WriteMultipleRegistersResponse
{
    /**
     * @brief Starting address written to
     */
    Address starting_address;

    /**
     * @brief Quantity of registers written to
     */
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
