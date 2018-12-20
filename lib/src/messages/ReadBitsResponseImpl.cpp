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
#include "messages/ReadBitsResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
ReadBitsResponseImpl<function_code, request_t, response_t>::ReadBitsResponseImpl(const response_t& request)
    : m_response{request}
{

}

template <uint8_t function_code, typename request_t, typename response_t>
std::unique_ptr<IMessage> ReadBitsResponseImpl<function_code, request_t, response_t>::clone() const
{
    return std::make_unique<ReadBitsResponseImpl>(m_response);
}

template <uint8_t function_code, typename request_t, typename response_t>
bool ReadBitsResponseImpl<function_code, request_t, response_t>::is_valid() const
{
    // TODO: implement this
    return true;
}

template <uint8_t function_code, typename request_t, typename response_t>
size_t ReadBitsResponseImpl<function_code, request_t, response_t>::get_message_length() const
{
    auto num_bytes = m_response.values.size() / 8;
    if(m_response.values.size() % 8 != 0)
    {
        ++num_bytes;
    }
    return 2 + num_bytes;
}

template <uint8_t function_code, typename request_t, typename response_t>
void ReadBitsResponseImpl<function_code, request_t, response_t>::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, function_code); // Function code
    ser4cpp::UInt8::write_to(buffer, static_cast<uint8_t>(get_message_length() - 2)); // Byte count

    // Write the bytes of data
    uint8_t current_byte = 0x00;
    uint8_t current_offset = 0;
    for(auto& value : m_response.values)
    {
        current_byte |= static_cast<uint8_t>(1) << current_offset;
        ++current_offset;

        if(current_offset == 8)
        {
            ser4cpp::UInt8::write_to(buffer, current_byte);
            current_byte = 0x00;
            current_offset = 0;
        }
    }
    // Write final byte (if necessary)
    if(current_offset != 0)
    {
        ser4cpp::UInt8::write_to(buffer, current_byte);
    }
}

template <uint8_t function_code, typename request_t, typename response_t>
Expected<response_t>
ReadBitsResponseImpl<function_code, request_t, response_t>::parse(const ReadBitsRequestImpl<function_code, request_t>& req,
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

    // Check that the number of bytes fits
    if(view.length() != length)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response length does not fit with the value in the header."});
    }

    // Check that the response has the expected number of bits
    auto expected_num_bytes = req.get_request().qty_of_bits / 8;
    if(req.get_request().qty_of_bits % 8 != 0)
    {
        ++expected_num_bytes;
    }
    if(length != expected_num_bytes)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response does not have the expected length."});
    }

    // Extract the values
    response_t response;
    auto current_address = req.get_request().starting_address;
    uint8_t current_value;
    while(ser4cpp::UInt8::read_from(view, current_value))
    {
        auto num_bits_to_extract = req.get_request().qty_of_bits - response.values.size();
        if(num_bits_to_extract > 8)
        {
            num_bits_to_extract = 8;
        }

        for(unsigned int i = 0; i < num_bits_to_extract; ++i)
        {
            auto bit_value = static_cast<bool>((current_value >> i) & 0x01);
            response.values.emplace_back(SingleBitValue{current_address, bit_value});
            ++current_address;
        }
    }

    return Expected<response_t>(response);
}

template class ReadBitsResponseImpl<0x01, ReadCoilsRequest, ReadCoilsResponse>;
template class ReadBitsResponseImpl<0x02, ReadDiscreteInputsRequest, ReadDiscreteInputsResponse>;

} // namespace modbus
