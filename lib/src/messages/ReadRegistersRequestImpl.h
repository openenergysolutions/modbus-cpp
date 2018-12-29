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
#ifndef MODBUS_READREGISTERSREQUESTIMPL_H
#define MODBUS_READREGISTERSREQUESTIMPL_H

#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "messages/IMessage.h"

namespace modbus
{

template <uint8_t function_code, typename request_t>
class ReadRegistersRequestImpl : public IMessage
{
public:
    explicit ReadRegistersRequestImpl(const request_t& request);

    std::unique_ptr<IMessage> clone() const override;

    bool is_valid() const override;
    size_t get_message_length() const override;
    void build_message(ser4cpp::wseq_t& buffer) const override;

    const request_t& get_request() const;

public:
    constexpr static uint16_t max_registers = 125;
    static Expected<request_t> parse(const ser4cpp::rseq_t& data);

private:
    request_t m_request;
};

extern template class ReadRegistersRequestImpl<0x03, ReadHoldingRegistersRequest>;
extern template class ReadRegistersRequestImpl<0x04, ReadInputRegistersRequest>;

} // namespace modbus

#endif //MODBUS_READREGISTERSREQUESTIMPL_H
