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
#ifndef MODBUS_MALFORMEDMODBUSREQUESTEXCEPTION_H
#define MODBUS_MALFORMEDMODBUSREQUESTEXCEPTION_H

/** @file
 * @brief Class @ref modbus::MalformedModbusRequestException
 */

#include "modbus/exceptions/IException.h"

namespace modbus
{

/**
 * @brief Malformed Modbus request error
 *
 * This error is generated when the user tries to send a malformed request. Refer to the documentation of the
 * message you are sending for further details.
 */
    class MalformedModbusRequestException : public IException
    {
    public:
        /**
         * @brief Constructor
         * @param message Message describing the exception
         */
        explicit MalformedModbusRequestException(const std::string& message)
                : IException{message}
        {

        }
    };

} // namespace modbus

#endif //MODBUS_MALFORMEDMODBUSREQUESTEXCEPTION_H
