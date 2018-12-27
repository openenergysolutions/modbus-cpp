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
#include "messages/WriteMultipleRegistersRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"

namespace modbus
{

WriteMultipleRegistersRequestImpl::WriteMultipleRegistersRequestImpl(const WriteMultipleRegistersRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IMessage> WriteMultipleRegistersRequestImpl::clone() const
{
    return std::make_unique<WriteMultipleRegistersRequestImpl>(*this);
}

bool WriteMultipleRegistersRequestImpl::is_valid() const
{
    return m_request.values.size() > 0 &&
           m_request.values.size() <= m_request.max_registers;
}

size_t WriteMultipleRegistersRequestImpl::get_message_length() const
{
    return 6 + 2 * m_request.values.size();
}

void WriteMultipleRegistersRequestImpl::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x10); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, static_cast<uint16_t>(m_request.values.size())); // Qty of registers
    ser4cpp::UInt8::write_to(buffer, static_cast<uint8_t>(2 * m_request.values.size())); // Byte count

    // Register values
    for(auto value : m_request.values)
    {
        ser4cpp::UInt16::write_to(buffer, value);
    }
}

const WriteMultipleRegistersRequest& WriteMultipleRegistersRequestImpl::get_request() const
{
    return m_request;
}

Expected<WriteMultipleRegistersRequest> WriteMultipleRegistersRequestImpl::parse(const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check number of bytes
    if(view.length() < 6)
    {
        return Expected<WriteMultipleRegistersRequest>::from_exception(MalformedModbusRequestException{"Request is too short."});
    }

    // Check function code
    uint8_t function_code;
    ser4cpp::UInt8::read_from(view, function_code);
    if(function_code != 0x10)
    {
        return Expected<WriteMultipleRegistersRequest>::from_exception(MalformedModbusRequestException{"Invalid function code."});
    }

    // Read starting address
    uint16_t starting_address;
    ser4cpp::UInt16::read_from(view, starting_address);

    // Read quantity of outputs
    uint16_t qty_of_registers;
    ser4cpp::UInt16::read_from(view, qty_of_registers);
    if(qty_of_registers < 0x0001 || qty_of_registers > WriteMultipleRegistersRequest::max_registers)
    {
        return Expected<WriteMultipleRegistersRequest>::from_exception(MalformedModbusRequestException{"Invalid quantity of outputs."});
    }

    // Read byte count
    uint8_t byte_count;
    ser4cpp::UInt8::read_from(view, byte_count);
    if(2 * qty_of_registers != byte_count)
    {
        return Expected<WriteMultipleRegistersRequest>::from_exception(MalformedModbusRequestException{"Request byte count does not fit with the quantity of outputs."});
    }
    if(byte_count != view.length())
    {
        return Expected<WriteMultipleRegistersRequest>::from_exception(MalformedModbusRequestException{"Request does not have the expected length."});
    }

    // Extract the values
    WriteMultipleRegistersRequest request;
    request.starting_address = starting_address;
    uint16_t current_value;
    while(ser4cpp::UInt16::read_from(view, current_value))
    {
        request.values.emplace_back(current_value);
    }

    return Expected<WriteMultipleRegistersRequest>(request);
}

} // namespace modbus
