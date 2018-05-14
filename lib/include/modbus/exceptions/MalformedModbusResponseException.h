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
#ifndef MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
#define MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H

/** @file
 * @brief Class @ref modbus::MalformedModbusResponseException
 */

#include "modbus/exceptions/IException.h"

namespace modbus
{

/**
 * @brief Malformed Modbus response error
 *
 * This error is generated when a malformed response is received. Check the associated message with
 * @ref IException::get_message() for more details about the error. The vendor of the faulty device should
 * be notified of the faulty response.
 */
class MalformedModbusResponseException : public IException
{
public:
    /**
     * @brief Constructor
     * @param message Message describing the exception
     */
    explicit MalformedModbusResponseException(const std::string& message)
        : IException{message}
    {

    }
};

} // namespace modbus

#endif //MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
