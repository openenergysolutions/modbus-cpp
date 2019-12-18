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
#ifndef MODBUS_REQUESTMOCK_H
#define MODBUS_REQUESTMOCK_H

#include "messages/IMessage.h"

class IMessageMock : public modbus::IMessage
{
public:
    IMessageMock(std::size_t length, uint8_t value, bool valid = true);

    std::unique_ptr<modbus::IMessage> clone() const override;

    bool is_valid() const override;
    size_t get_message_length() const override;
    void build_message(ser4cpp::wseq_t& buffer) const override;

public:
    static modbus::Expected<bool> parse_function_code(uint8_t function_code, ser4cpp::rseq_t& data);

private:
    std::size_t m_length;
    uint8_t m_value;
    bool m_valid;
};

#endif //MODBUS_REQUESTMOCK_H
