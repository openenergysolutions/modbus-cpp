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
#include "modbus/exceptions/ModbusException.h"
#include "modbus/messages/ExceptionType.h"
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

    virtual void shutdown() {};

    virtual Expected<ReadCoilsResponse> on_request(const ReadCoilsRequest& request)
    {
        return Expected<ReadCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    virtual Expected<ReadDiscreteInputsResponse> on_request(const ReadDiscreteInputsRequest& request)
    {
        return Expected<ReadDiscreteInputsResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    virtual Expected<ReadHoldingRegistersResponse> on_request(const ReadHoldingRegistersRequest& request)
    {
        return Expected<ReadHoldingRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    virtual Expected<ReadInputRegistersResponse> on_request(const ReadInputRegistersRequest& request)
    {
        return Expected<ReadInputRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    virtual Expected<WriteSingleCoilResponse> on_request(const WriteSingleCoilRequest& request)
    {
        return Expected<WriteSingleCoilResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    virtual Expected<WriteSingleRegisterResponse> on_request(const WriteSingleRegisterRequest& request)
    {
        return Expected<WriteSingleRegisterResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    virtual Expected<WriteMultipleCoilsResponse> on_request(const WriteMultipleCoilsRequest& request)
    {
        return Expected<WriteMultipleCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    virtual Expected<WriteMultipleRegistersResponse> on_request(const WriteMultipleRegistersRequest& request)
    {
        return Expected<WriteMultipleRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }
};

} // namespace modbus

#endif //MODBUS_ISERVERSESSION_H
