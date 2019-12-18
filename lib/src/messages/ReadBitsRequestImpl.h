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
#ifndef MODBUS_READBITSREQUESTIMPL_H
#define MODBUS_READBITSREQUESTIMPL_H

#include "modbus/Expected.h"
#include "modbus/messages/ReadCoilsRequest.h"
#include "modbus/messages/ReadDiscreteInputsRequest.h"
#include "messages/IMessage.h"

namespace modbus
{

template <uint8_t function_code, typename request_t>
class ReadBitsRequestImpl : public IMessage
{
public:
    explicit ReadBitsRequestImpl(const request_t& request);

    std::unique_ptr<IMessage> clone() const override;

    bool is_valid() const override;
    size_t get_message_length() const override;
    void build_message(ser4cpp::wseq_t& buffer) const override;

    const request_t& get_request() const;

public:
    constexpr static uint16_t max_bits = 2000;
    static Expected<request_t> parse(const ser4cpp::rseq_t& data);

private:
    request_t m_request;
};

extern template class ReadBitsRequestImpl<0x01, ReadCoilsRequest>;
extern template class ReadBitsRequestImpl<0x02, ReadDiscreteInputsRequest>;

} // namespace modbus

#endif //MODBUS_READBITSREQUESTIMPL_H
