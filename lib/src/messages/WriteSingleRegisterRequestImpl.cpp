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
#include "messages/WriteSingleRegisterRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"

namespace modbus
{

WriteSingleRegisterRequestImpl::WriteSingleRegisterRequestImpl(const WriteSingleRegisterRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IMessage> WriteSingleRegisterRequestImpl::clone() const
{
    return std::make_unique<WriteSingleRegisterRequestImpl>(m_request);
}

bool WriteSingleRegisterRequestImpl::is_valid() const
{
    return true;
}

size_t WriteSingleRegisterRequestImpl::get_message_length() const
{
    return 5;
}

void WriteSingleRegisterRequestImpl::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x06); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.value.address); // Address
    ser4cpp::UInt16::write_to(buffer, m_request.value.value); // Value
}

const WriteSingleRegisterRequest& WriteSingleRegisterRequestImpl::get_request() const
{
    return m_request;
}

Expected<WriteSingleRegisterRequest> WriteSingleRegisterRequestImpl::parse(const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check length
    if(view.length() != 5)
    {
        return Expected<WriteSingleRegisterRequest>::from_exception(MalformedModbusRequestException{"Invalid request size."});
    }

    // Check function code and Modbus exceptions
    uint8_t function_code;
    ser4cpp::UInt8::read_from(view, function_code);
    if(function_code != 0x06)
    {
        return Expected<WriteSingleRegisterRequest>::from_exception(MalformedModbusRequestException{"Invalid function code."});
    }    

    // Read address
    uint16_t address;
    ser4cpp::UInt16::read_from(view, address);

    // Read value
    uint16_t value;
    ser4cpp::UInt16::read_from(view, value);

    // Return result
    WriteSingleRegisterRequest response{RegisterValue{address, value}};
    return Expected<WriteSingleRegisterRequest>(response);
}

} // namespace modbus
