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
#ifndef MODBUS_EXCEPTIONRESPONSE_H
#define MODBUS_EXCEPTIONRESPONSE_H

#include "modbus/messages/ExceptionType.h"
#include "messages/IMessage.h"

namespace modbus
{

class ExceptionResponse : public IMessage
{
public:
    ExceptionResponse(uint8_t function_code, const ExceptionType& exception);

    std::unique_ptr<IMessage> clone() const override;

    bool is_valid() const override;
    size_t get_message_length() const override;
    void build_message(ser4cpp::wseq_t& buffer) const override;

private:
    uint8_t m_function_code;
    ExceptionType m_exception;
};

} // namespace modbus

#endif //MODBUS_EXCEPTIONRESPONSE_H
