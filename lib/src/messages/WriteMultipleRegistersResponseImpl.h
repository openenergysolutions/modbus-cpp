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
#ifndef MODBUS_WRITEMULTIPLEREGISTERSRESPONSEIMPL_H
#define MODBUS_WRITEMULTIPLEREGISTERSRESPONSEIMPL_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"
#include "messages/IMessage.h"
#include "messages/WriteMultipleRegistersRequestImpl.h"

namespace modbus
{

class WriteMultipleRegistersResponseImpl : public IMessage
{
public:
    static Expected<WriteMultipleRegistersResponse> parse(const WriteMultipleRegistersRequestImpl& req,
                                                          const ser4cpp::rseq_t& data);
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSRESPONSEIMPL_H
