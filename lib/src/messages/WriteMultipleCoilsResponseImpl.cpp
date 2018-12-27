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
#include "messages/WriteMultipleCoilsResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

WriteMultipleCoilsResponseImpl::WriteMultipleCoilsResponseImpl(const WriteMultipleCoilsResponse& response)
    : m_response{response}
{

}

std::unique_ptr<IMessage> WriteMultipleCoilsResponseImpl::clone() const
{
    return std::make_unique<WriteMultipleCoilsResponseImpl>(m_response);
}

bool WriteMultipleCoilsResponseImpl::is_valid() const
{
    return m_response.qty_of_coils >= 1 && m_response.qty_of_coils <= WriteMultipleCoilsRequest::max_coils;
}

size_t WriteMultipleCoilsResponseImpl::get_message_length() const
{
    return 5;
}

void WriteMultipleCoilsResponseImpl::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x0F); // Function code
    ser4cpp::UInt16::write_to(buffer, m_response.starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, m_response.qty_of_coils); // Quantity of coils
}

Expected<WriteMultipleCoilsResponse> WriteMultipleCoilsResponseImpl::parse(const WriteMultipleCoilsRequestImpl& req,
                                                                           const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(0x0F, view);
    if(!function_code_result.is_valid())
    {
        return Expected<WriteMultipleCoilsResponse>::from_exception(function_code_result.get_exception());
    }

    // Check length
    if(view.length() != 4)
    {
        return Expected<WriteMultipleCoilsResponse>::from_exception(MalformedModbusResponseException{"Invalid response size."});
    }

    // Read address
    uint16_t starting_address;
    ser4cpp::UInt16::read_from(view, starting_address);

    // Read value
    uint16_t qty_of_coils;
    ser4cpp::UInt16::read_from(view, qty_of_coils);

    // Return result
    WriteMultipleCoilsResponse response{starting_address, qty_of_coils};
    return Expected<WriteMultipleCoilsResponse>(response);
}

} // namespace modbus
