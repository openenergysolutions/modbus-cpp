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
#ifndef MODBUS_ISERVERCHANNEL_H
#define MODBUS_ISERVERCHANNEL_H

#include <memory>
#include "modbus/UnitIdentifier.h"
#include "modbus/server/IServerSession.h"

namespace modbus
{

class IServerChannel : public std::enable_shared_from_this<IServerChannel>
{
public:
    virtual ~IServerChannel() = default;

    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual void add_session(const UnitIdentifier& unit_identifier, std::shared_ptr<IServerSession> session) = 0;
};

} // namespace modbus

#endif //MODBUS_ISERVERCHANNEL_H
