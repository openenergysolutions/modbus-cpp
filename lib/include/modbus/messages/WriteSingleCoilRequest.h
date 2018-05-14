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
#ifndef MODBUS_WRITESINGLECOILREQUEST_H
#define MODBUS_WRITESINGLECOILREQUEST_H

/** @file
 * @brief Struct @ref modbus::WriteSingleCoilRequest
 */

#include "modbus/messages/SingleBitValue.h"

namespace modbus
{

/**
 * @brief Write Single Coil request message
 *
 * Write a single coil.
 *
 * This is the function code @cpp 0x05 @ce of the Modbus protocol.
 *
 * @see @ref WriteSingleCoilResponse, @ref WriteMultipleCoilsRequest
 */
    struct WriteSingleCoilRequest
    {
        /**
         * @brief Address of the coil to write and its new value
         *
         * The address must be in a valid range of the device.
         */
        SingleBitValue value;
    };

} // namespace modbus

#endif //MODBUS_WRITESINGLECOILREQUEST_H
