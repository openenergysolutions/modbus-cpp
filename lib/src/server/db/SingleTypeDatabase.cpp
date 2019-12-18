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
#include "modbus/server/db/SingleTypeDatabase.h"

namespace modbus
{

template<typename T>
bool SingleTypeDatabase<T>::add_value(Address address, T value)
{
    return m_values.insert({address, value}).second;
}

template<typename T>
bool SingleTypeDatabase<T>::set_value(Address address, T value)
{
    auto it = m_values.find(address);
    if(it != m_values.end())
    {
        it->second = value;
        return true;
    }

    return false;
}

template<typename T>
bool SingleTypeDatabase<T>::set_values(Address starting_address, const std::vector<T>& values)
{
    // Validate the range
    if(!is_valid_range(starting_address, values.size()))
    {
        return false;
    }

    // Update the values
    auto it = m_values.find(starting_address);
    for(size_t i = 0; i < values.size(); ++i)
    {
        it->second = values[i];
        ++it;
    }

    return true;
}

template<typename T>
bool SingleTypeDatabase<T>::get_value(Address address, T& output) const
{
    auto it = m_values.find(address);
    if(it != m_values.end())
    {
        output = it->second;
        return true;
    }

    return false;
}

template<typename T>
bool SingleTypeDatabase<T>::get_values(Address starting_address, std::vector<T>& output) const
{
    // Validate the range
    if(!is_valid_range(starting_address, output.size()))
    {
        return false;
    }

    // Retrieve the values
    auto it = m_values.find(starting_address);
    for(size_t i = 0; i < output.size(); ++i)
    {
        output[i] = it->second;
        ++it;
    }

    return true;
}

template<typename T>
bool SingleTypeDatabase<T>::is_valid(Address address) const
{
    auto it = m_values.find(address);
    if(it != m_values.end())
    {
        return true;
    }

    return false;
}

template<typename T>
bool SingleTypeDatabase<T>::is_valid_range(Address starting_address, size_t count) const
{
    // Find the start value
    auto it = m_values.find(starting_address);
    if(it == m_values.end())
    {
        return false;
    }
    ++it;

    // Check that the range does not contain any hole
    Address current_address = starting_address;
    while(static_cast<size_t>(current_address - starting_address + 1) < count && it != m_values.end())
    {
        if(it->first != current_address + 1)
        {
            return false;
        }

        current_address = it->first;
        ++it;
    }

    // Check that we have found the required number of values
    return static_cast<size_t>(current_address - starting_address + 1) == count;
}

template class SingleTypeDatabase<bool>;
template class SingleTypeDatabase<uint16_t>;

} // namespace modbus
