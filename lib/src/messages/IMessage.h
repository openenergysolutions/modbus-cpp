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
#ifndef MODBUS_IMESSAGE_H
#define MODBUS_IMESSAGE_H

#include <memory>
#include "ser4cpp/container/SequenceTypes.h"
#include "modbus/Expected.h"

namespace modbus
{

class IMessage
{
public:
    virtual ~IMessage() = default;

    virtual std::unique_ptr<IMessage> clone() const = 0;

    virtual bool is_valid() const = 0;
    virtual size_t get_message_length() const = 0;
    virtual void build_message(ser4cpp::wseq_t& buffer) const = 0;

protected:
    static Expected<bool> parse_function_code(uint8_t function_code, ser4cpp::rseq_t& data);
};

} // namespace modbus

#endif //MODBUS_IMESSAGE_H
