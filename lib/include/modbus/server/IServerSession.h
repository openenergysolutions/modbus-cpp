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
 * @brief Interface @ref modbus::IServerSession
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

/**
 * @brief Server communication session with a Modbus device
 *
 * This class represents a session for communicatiing with a Modbus device. This is the class you'll be using
 * to send requests to the device and schedule them for periodic polling.
 *
 * An instance of this class can be registered to a channel with @ref IServerChannel::add_session().
 *
 * It is possible to have multiple sessions on a single channel. For example, if the server you are implementing
 * is a gateway, you can have a single channel to the gateway and multiple sessions for each slave devices
 * connected to the gateway. The unit identifier for each device is set in @ref IServerChannel::add_session().
 * 
 * Each @cpp on_request(...) @ce methods should return either the appropriate response,
 * or a @ref ModbusException with the appropriate @ref ExceptionType. By default, all the
 * request handlers return a @ref ExceptionType::IllegalFunction to represent that the particular
 * function code is not supported. The library already validates that the data is valid. However,
 * the library cannot validate the address range and it's therefore the responsibility of the session
 * implementation to return a @ref ExceptionType::IllegalDataAddress when necessary.
 * 
 * @ref InMemoryDatabase is an already made server session with basic functionality that
 * should fit the majority of basic use cases. It can also be used as a template for
 * custom session implementations.
 *
 * @warning All the request handlers are called from background threads created by
 * the @ref IModbusManager. Therefore, all request handlers *should not block* in any
 * way. Otherwise, you risk having other channels not performing their tasks because
 * the thread is blocked or even deadlocks. Also, be sure to use the appropriate mechanisms
 * to eliminate the risks of race conditions in the user code. See @ref IModbusManager for
 * more details.
 *
 * @see @ref IModbusManager, @ref IServerChannel
 */
class IServerSession : public std::enable_shared_from_this<IServerSession>
{
public:
     /**
     * @brief Destructor
     *
     * @warning This destructor should never be called from user code. @ref IServerChannel
     * will hold a shared pointer to every sessions it has created and will release them in
     * @ref IServerChannel::shutdown(), which is probably called by @ref IModbusManager::shutdown().
     *
     * @see @ref IModbusManager::shutdown(), @ref IServerChannel::shutdown(), @ref shutdown()
     */
    virtual ~IServerSession() = default;

    /**
     * @brief Release all resources associated with the session.
     *
     * @note This method is usually **not** called by user code, but by @ref IServerChannel::shutdown(), which is normally
     * called by @ref IModbusManager::shutdown(). It is still possible to shutdown a session manually using this method,
     * however please note that the @ref IServerChannel will keep holding a shared pointer to it.
     *
     * @warning Sessions cannot be restarted, but you can safely create and attach a new one via @ref IServerChannel.
     *
     * @see @ref IModbusManager::shutdown(), @ref IServerChannel::shutdown()
     */
    virtual void shutdown() {};

    /**
    * @brief Callback upon reception of a @ref ReadCoilsRequest.
    * @param request Received request
    * @return @ref ReadCoilsResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<ReadCoilsResponse> on_request(const ReadCoilsRequest& request)
    {
        return Expected<ReadCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    /**
    * @brief Callback upon reception of a @ref ReadDiscreteInputsRequest.
    * @param request Received request
    * @return @ref ReadDiscreteInputsResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<ReadDiscreteInputsResponse> on_request(const ReadDiscreteInputsRequest& request)
    {
        return Expected<ReadDiscreteInputsResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    /**
    * @brief Callback upon reception of a @ref ReadHoldingRegistersRequest.
    * @param request Received request
    * @return @ref ReadHoldingRegistersResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<ReadHoldingRegistersResponse> on_request(const ReadHoldingRegistersRequest& request)
    {
        return Expected<ReadHoldingRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    /**
    * @brief Callback upon reception of a @ref ReadInputRegistersRequest.
    * @param request Received request
    * @return @ref ReadInputRegistersResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<ReadInputRegistersResponse> on_request(const ReadInputRegistersRequest& request)
    {
        return Expected<ReadInputRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    /**
    * @brief Callback upon reception of a @ref WriteSingleCoilRequest.
    * @param request Received request
    * @return @ref WriteSingleCoilResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<WriteSingleCoilResponse> on_request(const WriteSingleCoilRequest& request)
    {
        return Expected<WriteSingleCoilResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    /**
    * @brief Callback upon reception of a @ref WriteSingleRegisterRequest.
    * @param request Received request
    * @return @ref WriteSingleRegisterResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<WriteSingleRegisterResponse> on_request(const WriteSingleRegisterRequest& request)
    {
        return Expected<WriteSingleRegisterResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    /**
    * @brief Callback upon reception of a @ref WriteMultipleCoilsRequest.
    * @param request Received request
    * @return @ref WriteMultipleCoilsResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<WriteMultipleCoilsResponse> on_request(const WriteMultipleCoilsRequest& request)
    {
        return Expected<WriteMultipleCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }

    /**
    * @brief Callback upon reception of a @ref WriteMultipleRegistersRequest.
    * @param request Received request
    * @return @ref WriteMultipleRegistersResponse or a @ref ModbusException wrapped in @ref Expected
    *
    * @note By default, this method returns a @ref ExceptionType::IllegalFunction.
    */
    virtual Expected<WriteMultipleRegistersResponse> on_request(const WriteMultipleRegistersRequest& request)
    {
        return Expected<WriteMultipleRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalFunction});
    }
};

} // namespace modbus

#endif //MODBUS_ISERVERSESSION_H
