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
#ifndef MODBUS_ISERVER_H
#define MODBUS_ISERVER_H

#include <memory>

#include "server/channel/IServerConnectionListenerBuilder.h"

namespace modbus
{

class IServer : public std::enable_shared_from_this<IServer>
{
public:
    virtual ~IServer() = default;

    virtual void start(std::shared_ptr<IServerConnectionListenerBuilder> channel) = 0;
    virtual void shutdown() = 0;
};

} // namespace modbus

#endif //MODBUS_ISERVER_H
