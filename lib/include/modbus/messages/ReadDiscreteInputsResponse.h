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
#ifndef MODBUS_READDISCRETEINPUTSRESPONSE_H
#define MODBUS_READDISCRETEINPUTSRESPONSE_H

/** @file
 * @brief Struct @ref modbus::ReadCoilsResponse
 */

#include <vector>
#include "modbus/messages/SingleBitValue.h"

namespace modbus
{

/**
 * @brief Read Discrete Inputs response message
 *
 * Response to @ref ReadDiscreteInputsRequest.
 *
 * This is the response for function code @cpp 0x02 @ce of the Modbus protocol.
 *
 * @see @ref ReadDiscreteInputsRequest
 */
struct ReadDiscreteInputsResponse
{
    /**
     * @brief Discrete input values read on the device
     *
     * Each value contains both the address and the value of the coil.
     */
    std::vector<SingleBitValue> values;
};

} // namespace modbus

#endif //MODBUS_READDISCRETEINPUTSRESPONSE_H
