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
#include "messages/WriteMultipleCoilsRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"
#include "modbus/exceptions/ModbusException.h"

namespace modbus
{

WriteMultipleCoilsRequestImpl::WriteMultipleCoilsRequestImpl(const WriteMultipleCoilsRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IMessage> WriteMultipleCoilsRequestImpl::clone() const
{
    return std::make_unique<WriteMultipleCoilsRequestImpl>(*this);
}

bool WriteMultipleCoilsRequestImpl::is_valid() const
{
    return m_request.values.size() > 0 &&
           m_request.values.size() <= m_request.max_coils;
}

size_t WriteMultipleCoilsRequestImpl::get_message_length() const
{
    return 6 + get_byte_count_from_qty_of_outputs(m_request.values.size());
}

void WriteMultipleCoilsRequestImpl::build_message(ser4cpp::wseq_t& buffer) const
{
    auto num_bytes = get_byte_count_from_qty_of_outputs(m_request.values.size());

    ser4cpp::UInt8::write_to(buffer, 0x0F); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, static_cast<uint16_t>(m_request.values.size())); // Qty of coils
    ser4cpp::UInt8::write_to(buffer, static_cast<uint8_t>(num_bytes)); // Byte count

    // Coil values
    unsigned int current_bit = 0;
    for(unsigned int i = 0; i < num_bytes; ++i)
    {
        uint8_t value = 0x00;
        for(unsigned int j = 0; j < 8; ++j)
        {
            if(current_bit >= m_request.values.size())
            {
                break;
            }

            value |= static_cast<uint8_t>(m_request.values[current_bit]) << j;
            ++current_bit;
        }

        ser4cpp::UInt8::write_to(buffer, value);
    }
}

const WriteMultipleCoilsRequest& WriteMultipleCoilsRequestImpl::get_request() const
{
    return m_request;
}

Expected<WriteMultipleCoilsRequest> WriteMultipleCoilsRequestImpl::parse(const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check number of bytes
    if(view.length() < 6)
    {
        return Expected<WriteMultipleCoilsRequest>::from_exception(MalformedModbusRequestException{"Request is too short."});
    }

    // Check function code
    uint8_t function_code;
    ser4cpp::UInt8::read_from(view, function_code);
    if(function_code != 0x0F)
    {
        return Expected<WriteMultipleCoilsRequest>::from_exception(MalformedModbusRequestException{"Invalid function code."});
    }

    // Read starting address
    uint16_t starting_address;
    ser4cpp::UInt16::read_from(view, starting_address);

    // Read quantity of outputs
    uint16_t qty_of_outputs;
    ser4cpp::UInt16::read_from(view, qty_of_outputs);
    if(qty_of_outputs < 0x0001 || qty_of_outputs > WriteMultipleCoilsRequest::max_coils)
    {
        return Expected<WriteMultipleCoilsRequest>::from_exception(ModbusException{ExceptionType::IllegalDataValue});
    }

    // Read byte count
    uint8_t byte_count;
    ser4cpp::UInt8::read_from(view, byte_count);
    if(get_byte_count_from_qty_of_outputs(qty_of_outputs) != byte_count)
    {
        return Expected<WriteMultipleCoilsRequest>::from_exception(ModbusException{ExceptionType::IllegalDataValue});
    }
    if(byte_count != view.length())
    {
        return Expected<WriteMultipleCoilsRequest>::from_exception(MalformedModbusRequestException{"Request does not have the expected length."});
    }

    // Extract the values
    WriteMultipleCoilsRequest request;
    request.starting_address = starting_address;
    uint8_t current_value;
    while(ser4cpp::UInt8::read_from(view, current_value))
    {
        auto num_bits_to_extract = qty_of_outputs - request.values.size();
        if(num_bits_to_extract > 8)
        {
            num_bits_to_extract = 8;
        }

        for(unsigned int i = 0; i < num_bits_to_extract; ++i)
        {
            auto bit_value = static_cast<bool>((current_value >> i) & 0x01);
            request.values.emplace_back(bit_value);
        }
    }

    return Expected<WriteMultipleCoilsRequest>(request);
}

size_t WriteMultipleCoilsRequestImpl::get_byte_count_from_qty_of_outputs(size_t qty_of_outputs)
{
    auto num_bytes = qty_of_outputs / 8;
    if (qty_of_outputs % 8 != 0)
    {
        ++num_bytes;
    }

    return num_bytes;
}

} // namespace modbus
