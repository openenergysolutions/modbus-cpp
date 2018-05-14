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
#ifndef MODBUS_IREQUEST_H
#define MODBUS_IREQUEST_H

#include <memory>
#include "ser4cpp/container/SequenceTypes.h"

namespace modbus
{

class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual std::unique_ptr<IRequest> clone() const = 0;

    virtual size_t get_request_length() const = 0;
    virtual void build_request(ser4cpp::wseq_t& buffer) const = 0;
};

} // namespace modbus

#endif //MODBUS_IREQUEST_H
