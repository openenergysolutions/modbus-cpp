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
#include "messages/WriteSingleCoilRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

WriteSingleCoilRequestImpl::WriteSingleCoilRequestImpl(const WriteSingleCoilRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IRequest> WriteSingleCoilRequestImpl::clone() const
{
    return std::make_unique<WriteSingleCoilRequestImpl>(m_request);
}

size_t WriteSingleCoilRequestImpl::get_request_length() const
{
    return 5;
}

void WriteSingleCoilRequestImpl::build_request(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x05); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.value.address); // Address

    auto value = WriteSingleCoilRequestImpl::OFF;
    if(m_request.value.value == true)
    {
        value = WriteSingleCoilRequestImpl::ON;
    }
    ser4cpp::UInt16::write_to(buffer, value); // Value
}

const WriteSingleCoilRequest& WriteSingleCoilRequestImpl::get_request() const
{
    return m_request;
}

} // namespace modbus
