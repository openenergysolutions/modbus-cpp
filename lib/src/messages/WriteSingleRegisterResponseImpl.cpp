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
#include "messages/WriteSingleRegisterResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

WriteSingleRegisterResponseImpl::WriteSingleRegisterResponseImpl(const WriteSingleRegisterResponse& response)
    : m_response{response}
{

}

std::unique_ptr<IMessage> WriteSingleRegisterResponseImpl::clone() const
{
    return std::make_unique<WriteSingleRegisterResponseImpl>(m_response);
}

bool WriteSingleRegisterResponseImpl::is_valid() const
{
    return true;
}

size_t WriteSingleRegisterResponseImpl::get_message_length() const
{
    return 5;
}

void WriteSingleRegisterResponseImpl::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x06); // Function code
    ser4cpp::UInt16::write_to(buffer, m_response.value.address); // Address
    ser4cpp::UInt16::write_to(buffer, m_response.value.value); // Value
}

Expected<WriteSingleRegisterResponse> WriteSingleRegisterResponseImpl::parse(const WriteSingleRegisterRequestImpl& req,
                                                                             const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(0x06, view);
    if(!function_code_result.is_valid())
    {
        return Expected<WriteSingleRegisterResponse>::from_exception(function_code_result.get_exception());
    }

    // Check length
    if(view.length() != 4)
    {
        return Expected<WriteSingleRegisterResponse>::from_exception(MalformedModbusResponseException{"Invalid response size."});
    }

    // Read address
    uint16_t address;
    ser4cpp::UInt16::read_from(view, address);

    // Read value
    uint16_t value;
    ser4cpp::UInt16::read_from(view, value);

    // Return result
    WriteSingleRegisterResponse response{RegisterValue{address, value}};
    return Expected<WriteSingleRegisterResponse>(response);
}

} // namespace modbus
