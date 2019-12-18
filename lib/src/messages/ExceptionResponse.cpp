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
#include "messages/ExceptionResponse.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

ExceptionResponse::ExceptionResponse(uint8_t function_code, const ExceptionType& exception)
    : m_function_code{function_code},
      m_exception{exception}
{

}

std::unique_ptr<IMessage> ExceptionResponse::clone() const
{
    return std::make_unique<ExceptionResponse>(m_function_code, m_exception);
}

bool ExceptionResponse::is_valid() const
{
    return true;
}

size_t ExceptionResponse::get_message_length() const
{
    return 2;
}

void ExceptionResponse::build_message(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, m_function_code | 0x80); // Function code
    ser4cpp::UInt8::write_to(buffer, static_cast<uint8_t>(m_exception)); // Exception code
}

} // namespace modbus
