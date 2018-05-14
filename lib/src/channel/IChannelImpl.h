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
#ifndef MODBUS_ICHANNELIMPL_H
#define MODBUS_ICHANNELIMPL_H

#include "ser4cpp/container/SequenceTypes.h"

#include "modbus/channel/IChannel.h"
#include "modbus/ResponseHandler.h"

namespace modbus
{

class IRequest;
class ISession;

class IChannelImpl : public IChannel
{
public:
    virtual ~IChannelImpl() = default;

    virtual void send_request(const UnitIdentifier& unit_identifier,
                              const IRequest& request,
                              const exe4cpp::duration_t& timeout,
                              ResponseHandler<ser4cpp::rseq_t> response_handler) = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNELIMPL_H
