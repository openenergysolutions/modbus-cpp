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
#ifndef MODBUS_WRITEMULTIPLECOILSRESPONSE_H
#define MODBUS_WRITEMULTIPLECOILSRESPONSE_H

/** @file
 * @brief Struct @ref modbus::WriteMultipleCoilsResponse
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Write multiple coils response message
 *
 * Response to @ref WriteMultipleCoilsRequest.
 *
 * This is the response for function code @cpp 0x0F @ce of the Modbus protocol.
 *
 * @see @ref WriteMultipleCoilsRequest
 */
struct WriteMultipleCoilsResponse
{
    /**
     * @brief Starting address written to
     */
    Address starting_address;

    /**
     * @brief Quantity of coils forced
     */
    uint16_t qty_of_coils;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLECOILSRESPONSE_H
