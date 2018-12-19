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
#ifndef MODBUS_ISERVERSESSION_H
#define MODBUS_ISERVERSESSION_H

/** @file
 * @brief Interface @ref modbus::ISession
 */

#include <memory>

#include "modbus/Expected.h"
#include "modbus/messages/ReadCoilsRequest.h"
#include "modbus/messages/ReadCoilsResponse.h"
#include "modbus/messages/ReadDiscreteInputsRequest.h"
#include "modbus/messages/ReadDiscreteInputsResponse.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"
#include "modbus/messages/WriteMultipleCoilsRequest.h"
#include "modbus/messages/WriteMultipleCoilsResponse.h"
#include "modbus/messages/WriteMultipleRegistersRequest.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"
#include "modbus/messages/WriteSingleCoilRequest.h"
#include "modbus/messages/WriteSingleCoilResponse.h"
#include "modbus/messages/WriteSingleRegisterRequest.h"
#include "modbus/messages/WriteSingleRegisterResponse.h"

namespace modbus
{

class IServerSession : public std::enable_shared_from_this<IServerSession>
{
public:
    virtual ~IServerSession() = default;

    virtual void shutdown() = 0;

    virtual Expected<ReadCoilsResponse> on_request(const ReadCoilsRequest& request) = 0;
    virtual Expected<ReadDiscreteInputsResponse> on_request(const ReadDiscreteInputsRequest& request) = 0;
    virtual Expected<ReadHoldingRegistersResponse> on_request(const ReadHoldingRegistersRequest& request) = 0;
    virtual Expected<ReadInputRegistersResponse> on_request(const ReadInputRegistersRequest& request) = 0;

    virtual Expected<WriteSingleCoilResponse> on_request(const WriteSingleCoilRequest& request) = 0;
    virtual Expected<WriteSingleRegisterResponse> on_request(const WriteSingleRegisterRequest& request) = 0;
    virtual Expected<WriteMultipleCoilsResponse> on_request(const WriteMultipleCoilsRequest& request) = 0;
    virtual Expected<WriteMultipleRegistersResponse> on_request(const WriteMultipleRegistersRequest& request) = 0;
};

} // namespace modbus

#endif //MODBUS_ISERVERSESSION_H
