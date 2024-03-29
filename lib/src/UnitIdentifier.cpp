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
#include "modbus/UnitIdentifier.h"

#include <ios>
#include <iomanip>

namespace modbus
{

std::ostream& operator<<(std::ostream& stream, const UnitIdentifier& unit_id)
{
    return stream << "0x"
                  << std::uppercase
                  << std::setw(2) << std::setfill('0')
                  << std::hex << (unsigned int)unit_id.get_value();
}

} // namespace modbus

namespace std
{
    size_t hash<modbus::UnitIdentifier>::operator()(const modbus::UnitIdentifier& unit_identifier) const
    {
        return hash<unsigned int>()(unit_identifier.get_value());
    }
} // namespace std
