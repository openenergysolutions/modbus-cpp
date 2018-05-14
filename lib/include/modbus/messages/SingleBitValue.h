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
#ifndef MODBUS_SINGLEBITVALUE_H
#define MODBUS_SINGLEBITVALUE_H

/** @file
 * @brief Struct @ref modbus::SingleBitValue
 */

#include "Address.h"

namespace modbus
{

/**
 * @brief Represent a coil or a discrete input address and its value
 *
 * @see @ref ReadCoilsResponse, @ref ReadDiscreteInputsResponse,
 *      @ref WriteSingleCoilRequest, @ref WriteSingleCoilResponse,
 */
struct SingleBitValue
{
    Address address;
    bool value;
};

} // modbus

#endif //MODBUS_SINGLEBITVALUE_H
