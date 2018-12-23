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
#include "messages/WriteSingleCoilRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"

namespace modbus
{

WriteSingleCoilRequestImpl::WriteSingleCoilRequestImpl(const WriteSingleCoilRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IMessage> WriteSingleCoilRequestImpl::clone() const
{
    return std::make_unique<WriteSingleCoilRequestImpl>(m_request);
}

bool WriteSingleCoilRequestImpl::is_valid() const
{
    return true;
}

size_t WriteSingleCoilRequestImpl::get_message_length() const
{
    return 5;
}

void WriteSingleCoilRequestImpl::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x05); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.value.address); // Address

    auto value = WriteSingleCoilRequestImpl::OFF;
    if(m_request.value.value == true)
    {
        value = WriteSingleCoilRequestImpl::ON;
    }
    ser4cpp::UInt16::write_to(buffer, value); // Value
}

const WriteSingleCoilRequest& WriteSingleCoilRequestImpl::get_request() const
{
    return m_request;
}

Expected<WriteSingleCoilRequest> WriteSingleCoilRequestImpl::parse(const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check length
    if(view.length() != 5)
    {
        return Expected<WriteSingleCoilRequest>::from_exception(MalformedModbusRequestException{"Invalid request size."});
    }

    // Check function code and Modbus exceptions
    uint8_t function_code;
    ser4cpp::UInt8::read_from(view, function_code);
    if(function_code != 0x05)
    {
        return Expected<WriteSingleCoilRequest>::from_exception(MalformedModbusRequestException{"Invalid function code size."});
    }    

    // Read address
    uint16_t address;
    ser4cpp::UInt16::read_from(view, address);

    // Read value
    uint16_t value;
    ser4cpp::UInt16::read_from(view, value);

    // Convert to boolean value
    bool bool_value;
    switch(value)
    {
        case WriteSingleCoilRequestImpl::ON:
            bool_value = true;
            break;

        case WriteSingleCoilRequestImpl::OFF:
            bool_value = false;
            break;

        default:
            return Expected<WriteSingleCoilRequest>::from_exception(MalformedModbusRequestException{"Invalid coil state."});
    }

    // Return result
    WriteSingleCoilRequest response{SingleBitValue{address, bool_value}};
    return Expected<WriteSingleCoilRequest>(response);
}

} // namespace modbus
