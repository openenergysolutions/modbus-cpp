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
#include "messages/IResponse.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"

namespace modbus
{

Expected<bool> IResponse::parse_function_code(uint8_t function_code, ser4cpp::rseq_t& data)
{
    // Check length for at least a function code
    if(data.length() < 1)
    {
        return Expected<bool>::from_exception(MalformedModbusResponseException{"Response is empty."});
    }

    uint8_t read_function_code;
    ser4cpp::UInt8::read_from(data, read_function_code);

    // Check if the function code matches the expected one (ignoring the MSB)
    if((read_function_code & 0x7F) != function_code)
    {
        return Expected<bool>::from_exception(MalformedModbusResponseException{"Wrong function code."});
    }

    // If we have a Modbus exception
    if((read_function_code & 0x80) != 0)
    {
        if(data.length() != 1)
        {
            return Expected<bool>::from_exception(MalformedModbusResponseException{"Modbus exception should have a single byte of data."});
        }

        uint8_t exception_code;
        ser4cpp::UInt8::read_from(data, exception_code);
        auto exception_type = parse_exception_type(exception_code);
        return Expected<bool>::from_exception(ModbusException{exception_type});
    }

    return Expected<bool>{true};
}

} // namespace modbus
