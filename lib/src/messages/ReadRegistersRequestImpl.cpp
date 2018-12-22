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
#include "messages/ReadRegistersRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"

namespace modbus
{

template <uint8_t function_code, typename request_t>
ReadRegistersRequestImpl<function_code, request_t>::ReadRegistersRequestImpl(const request_t& request)
        : m_request{request}
{

}

template <uint8_t function_code, typename request_t>
bool ReadRegistersRequestImpl<function_code, request_t>::is_valid() const
{
    return m_request.qty_of_registers > 0 &&
           m_request.qty_of_registers <= m_request.max_registers;
}

template <uint8_t function_code, typename request_t>
std::unique_ptr<IMessage> ReadRegistersRequestImpl<function_code, request_t>::clone() const
{
    return std::make_unique<ReadRegistersRequestImpl>(m_request);
}

template <uint8_t function_code, typename request_t>
size_t ReadRegistersRequestImpl<function_code, request_t>::get_message_length() const
{
    return 5;
}

template <uint8_t function_code, typename request_t>
void ReadRegistersRequestImpl<function_code, request_t>::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, function_code); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, m_request.qty_of_registers); // Qty of registers
}

template <uint8_t function_code, typename request_t>
const request_t& ReadRegistersRequestImpl<function_code, request_t>::get_request() const
{
    return m_request;
}

template <uint8_t function_code, typename request_t>
Expected<request_t> ReadRegistersRequestImpl<function_code, request_t>::parse(const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check length
    if(view.length() != 5)
    {
        return Expected<request_t>::from_exception(MalformedModbusRequestException{"Request does not have the expected length."});
    }

    // Check the function code
    uint8_t read_function_code;
    ser4cpp::UInt8::read_from(view, read_function_code);
    if(read_function_code != function_code)
    {
        return Expected<request_t>::from_exception(MalformedModbusRequestException{"Unexpected function code."});
    }

    // Read starting address
    uint16_t starting_address;
    ser4cpp::UInt16::read_from(view, starting_address);

    // Read quantity of registers
    uint16_t qty_of_registers;
    ser4cpp::UInt16::read_from(view, qty_of_registers);
    
    request_t request;
    request.starting_address = starting_address;
    request.qty_of_registers = qty_of_registers;
    return Expected<request_t>(request);
}

template class ReadRegistersRequestImpl<0x03, ReadHoldingRegistersRequest>;
template class ReadRegistersRequestImpl<0x04, ReadInputRegistersRequest>;

} // namespace modbus
