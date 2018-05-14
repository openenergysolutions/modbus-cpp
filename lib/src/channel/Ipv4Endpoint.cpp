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
#include "modbus/channel/Ipv4Endpoint.h"

namespace modbus
{

Ipv4Endpoint::Ipv4Endpoint(const std::string& hostname, uint32_t port)
        : m_hostname{hostname},
          m_port{port}
{

}

const std::string& Ipv4Endpoint::get_hostname() const
{
    return m_hostname;
}

uint32_t Ipv4Endpoint::get_port() const
{
    return m_port;
}

bool Ipv4Endpoint::operator==(const Ipv4Endpoint& other) const
{
    return m_hostname == other.m_hostname &&
           m_port == other.m_port;
}

bool Ipv4Endpoint::operator!=(const Ipv4Endpoint& other) const
{
    return !(*this == other);
}

} // namespace modbus