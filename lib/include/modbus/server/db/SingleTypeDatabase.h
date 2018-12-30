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

#include <map>
#include <vector>
#include "modbus/messages/Address.h"

namespace modbus
{

template<typename T>
class SingleTypeDatabase
{
public:
    virtual ~SingleTypeDatabase() = default;

    bool add_value(Address address, T value);

    bool set_value(Address address, T value);
    bool set_values(Address starting_address, const std::vector<T>& values);

    bool get_value(Address address, T& output) const;
    bool get_values(Address starting_address, std::vector<T>& output) const;

    bool is_valid(Address address) const;
    bool is_valid_range(Address starting_address, size_t count) const;

private:
    std::map<Address, T> m_values;
};

extern template class SingleTypeDatabase<bool>;
extern template class SingleTypeDatabase<uint16_t>;

} // namespace modbus

#endif //MODBUS_SINGLETYPEDATABASE_H
