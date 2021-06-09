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
#ifndef MODBUS_SINGLETYPEDATABASE_H
#define MODBUS_SINGLETYPEDATABASE_H

/** @file
 * @brief Class @ref modbus::SingleTypeDatabase
 */

#include <cstddef>
#include <map>
#include <vector>
#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief In-memory database containing a single type of data
 * 
 * This class is used by @ref InMemoryDatabase but it can be particularly
 * useful when implementing a custom @ref IServerSession. It provides methods
 * to add, get and set values individually or in sequence. It also provides
 * methods to validate ranges of addresses.
 * 
 * @see @ref BitDatabase, @ref RegisterDatabase
 */
template<typename T>
class SingleTypeDatabase
{
public:
    /**
     * @brief Destructor
     */
    virtual ~SingleTypeDatabase() = default;

    /**
     * @brief Add a value to the database.
     * @param address Address of the value
     * @param value Initial value
     * @return @cpp true @ce if no value with the specified address existed, @cpp false @ce otherwise.
     */
    bool add_value(Address address, T value);

    /**
     * @brief Set a single value
     * @param address Address of the value
     * @param value Value to set
     * @return @cpp true @ce if the value was successfully set, @cpp false @ce if the address is invalid.
     */
    bool set_value(Address address, T value);

    /**
     * @brief Set multiple adjacent values
     * @param starting_address Starting address
     * @param values Values to set
     * @return @cpp true @ce if the values were successfully set, @cpp false @ce if the address range is invalid.
     */
    bool set_values(Address starting_address, const std::vector<T>& values);

    /**
     * @brief Get a single value
     * @param address Address of the value
     * @param output Reference to write the value to
     * @return @cpp true @ce if the value was successfully retrieved, @cpp false @ce if the address is invalid.
     */
    bool get_value(Address address, T& output) const;

    /**
     * @brief Get a multiple adjacent values
     * @param starting_address Starting address
     * @param output Reference to a vector to write the values to
     * @return @cpp true @ce if the values were successfully retrieved, @cpp false @ce if the address range is invalid.
     * 
     * The @p output vector should already be sized appropriately to contain all the desired values.
     */
    bool get_values(Address starting_address, std::vector<T>& output) const;

    /**
     * @brief Validate if an address is assigned to a value.
     * @param address Address to check
     * @return @cpp true @ce if the address is valid, @cpp false @ce otherwise.
     */
    bool is_valid(Address address) const;

    /**
     * @brief Validate if an address range is assigned to values.
     * @param starting_address Starting address
     * @param count Length of the range to validate
     * @return @cpp true @ce if the address range is valid, @cpp false @ce otherwise.
     */
    bool is_valid_range(Address starting_address, size_t count) const;

private:
    std::map<Address, T> m_values;
};

extern template class SingleTypeDatabase<bool>;
extern template class SingleTypeDatabase<uint16_t>;

} // namespace modbus

#endif //MODBUS_SINGLETYPEDATABASE_H
