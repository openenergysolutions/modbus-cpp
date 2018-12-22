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
#include "messages/ReadRegistersResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
ReadRegistersResponseImpl<function_code, request_t, response_t>::ReadRegistersResponseImpl(const response_t& request)
    : m_response{request}
{

}

template <uint8_t function_code, typename request_t, typename response_t>
std::unique_ptr<IMessage> ReadRegistersResponseImpl<function_code, request_t, response_t>::clone() const
{
    return std::make_unique<ReadRegistersResponseImpl>(m_response);
}

template <uint8_t function_code, typename request_t, typename response_t>
bool ReadRegistersResponseImpl<function_code, request_t, response_t>::is_valid() const
{
    return true;
}

template <uint8_t function_code, typename request_t, typename response_t>
size_t ReadRegistersResponseImpl<function_code, request_t, response_t>::get_message_length() const
{
    return 2 + m_response.values.size() * 2;
}

template <uint8_t function_code, typename request_t, typename response_t>
void ReadRegistersResponseImpl<function_code, request_t, response_t>::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, function_code); // Function code
    ser4cpp::UInt8::write_to(buffer, static_cast<uint8_t>(get_message_length() - 2)); // Byte count

    // Write the bytes of data
    for(auto& value : m_response.values)
    {
        ser4cpp::UInt16::write_to(buffer, value.value);
    }
}

template <uint8_t function_code, typename request_t, typename response_t>
Expected<response_t>
ReadRegistersResponseImpl<function_code, request_t, response_t>::parse(const ReadRegistersRequestImpl<function_code, request_t>& req,
                                                                       const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(function_code, view);
    if(!function_code_result.is_valid())
    {
        return Expected<response_t>::from_exception(function_code_result.get_exception());
    }

    // Read number of bytes
    if(view.length() < 1)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response is too short."});
    }
    uint8_t length;
    ser4cpp::UInt8::read_from(view, length);
    if(length % 2 != 0)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{ "Response should contain an even number of register bytes." });
    }

    // Check that each number of registers match
    if(view.length() != 2 * req.get_request().qty_of_registers)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response does not contain the expected number of registers."});
    }

    // Extract the values
    response_t response;
    auto current_address = req.get_request().starting_address;
    uint16_t current_value;
    while(ser4cpp::UInt16::read_from(view, current_value))
    {
        response.values.emplace_back(RegisterValue{current_address, current_value});
        ++current_address;
    }

    return Expected<response_t>(response);
}

template class ReadRegistersResponseImpl<0x03, ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>;
template class ReadRegistersResponseImpl<0x04, ReadInputRegistersRequest, ReadInputRegistersResponse>;

} // namespace modbus
