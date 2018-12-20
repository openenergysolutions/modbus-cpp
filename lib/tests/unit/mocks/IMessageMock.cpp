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
#include "IMessageMock.h"

IMessageMock::IMessageMock(std::size_t length, uint8_t value, bool valid)
    : m_length{length},
      m_value{value},
      m_valid{valid}
{

}

std::unique_ptr<modbus::IMessage> IMessageMock::clone() const
{
    return std::make_unique<IMessageMock>(*this);
}

bool IMessageMock::is_valid() const
{
    return m_valid;
}

size_t IMessageMock::get_message_length() const
{
    return m_length;
}

void IMessageMock::build_message(ser4cpp::wseq_t& buffer) const
{
    for(std::size_t i = 0; i < m_length; ++i)
    {
        buffer.put(m_value);
    }
}

modbus::Expected<bool> IMessageMock::parse_function_code(uint8_t function_code, ser4cpp::rseq_t& data)
{
    return IMessage::parse_function_code(function_code, data);
}
