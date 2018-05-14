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
#include "RequestMock.h"

RequestMock::RequestMock(std::size_t length, uint8_t value)
    : m_length{length},
      m_value{value}
{

}

std::unique_ptr<modbus::IRequest> RequestMock::clone() const
{
    return std::make_unique<RequestMock>(*this);
}

size_t RequestMock::get_request_length() const
{
    return m_length;
}

void RequestMock::build_request(ser4cpp::wseq_t& buffer) const
{
    for(std::size_t i = 0; i < m_length; ++i)
    {
        buffer.put(m_value);
    }
}
