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
#ifndef MODBUS_READBITSRESPONSEIMPL_H
#define MODBUS_READBITSRESPONSEIMPL_H

#include <vector>
#include "modbus/messages/ReadCoilsRequest.h"
#include "modbus/messages/ReadCoilsResponse.h"
#include "modbus/messages/ReadDiscreteInputsRequest.h"
#include "modbus/messages/ReadDiscreteInputsResponse.h"
#include "messages/IResponse.h"
#include "messages/ReadBitsRequestImpl.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
class ReadBitsResponseImpl : public IResponse
{
public:
    static Expected<response_t> parse(const ReadBitsRequestImpl<function_code, request_t>& req,
                                      const ser4cpp::rseq_t& data);
};

extern template class ReadBitsResponseImpl<0x01, ReadCoilsRequest, ReadCoilsResponse>;
extern template class ReadBitsResponseImpl<0x02, ReadDiscreteInputsRequest, ReadDiscreteInputsResponse>;

} // namespace modbus

#endif //MODBUS_READBITSRESPONSEIMPL_H
