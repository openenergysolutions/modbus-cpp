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
#include "TransactionIdentifier.h"

#include <iomanip>

namespace modbus
{

TransactionIdentifier::TransactionIdentifier(uint16_t value)
        : m_value(value)
{

}

uint16_t TransactionIdentifier::get_value() const
{
    return m_value;
}

bool TransactionIdentifier::operator==(const TransactionIdentifier& other) const
{
    return m_value == other.m_value;
}

bool TransactionIdentifier::operator!=(const TransactionIdentifier& other) const
{
    return m_value != other.m_value;
}

TransactionIdentifier& TransactionIdentifier::operator++()
{
    ++m_value;
    return *this;
}

TransactionIdentifier TransactionIdentifier::operator++(int)
{
    auto temp = (*this);
    ++m_value;
    return temp;
}

std::ostream& operator<<(std::ostream& stream, const TransactionIdentifier& transaction_id)
{
    return stream << "0x"
                  << std::uppercase
                  << std::setw(4) << std::setfill('0')
                  << std::hex << (unsigned int)transaction_id.get_value();
}

} // namespace modbus
