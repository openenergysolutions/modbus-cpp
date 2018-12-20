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
#ifndef MODBUS_READREGISTERSRESPONSEIMPL_H
#define MODBUS_READREGISTERSRESPONSEIMPL_H

#include <vector>
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"
#include "messages/IMessage.h"
#include "messages/ReadRegistersRequestImpl.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
class ReadRegistersResponseImpl : public IMessage
{
public:
    static Expected<response_t> parse(const ReadRegistersRequestImpl<function_code, request_t>& req,
                                      const ser4cpp::rseq_t& data);
};

extern template class ReadRegistersResponseImpl<0x03, ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>;
extern template class ReadRegistersResponseImpl<0x04, ReadInputRegistersRequest, ReadInputRegistersResponse>;

} // namespace modbus

#endif //MODBUS_READREGISTERSRESPONSEIMPL_H
