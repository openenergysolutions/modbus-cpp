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
#ifndef MODBUS_IDATABASE_H
#define MODBUS_IDATABASE_H

#include <memory>
#include <mutex>

#include "modbus/server/IServerSession.h"
#include "modbus/server/db/BitDatabase.h"
#include "modbus/server/db/RegisterDatabase.h"

namespace modbus
{

class IDatabase
{
public:
    virtual ~IDatabase() = default;

    virtual bool get_coil(Address address, bool& output) const = 0;
    virtual bool set_coil(Address address, bool value) = 0;

    virtual bool get_discrete_input(Address address, bool& output) const = 0;
    virtual bool set_discrete_input(Address address, bool value) = 0;

    virtual bool get_holding_register(Address address, uint16_t& output) const = 0;
    virtual bool set_holding_register(Address address, uint16_t value) = 0;

    virtual bool get_input_register(Address address, uint16_t& output) const = 0;
    virtual bool set_input_register(Address address, uint16_t value) = 0;
};

} // namespace modbus

#endif //MODBUS_IDATABASE_H
