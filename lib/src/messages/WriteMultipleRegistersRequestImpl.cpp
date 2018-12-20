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

} // namespace modbus
