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
#ifndef MODBUS_ICLIENTCHANNEL_H
#define MODBUS_ICLIENTCHANNEL_H

/** @file
 * @brief Interface @ref modbus::IClientChannel
 */

#include <memory>
#include "modbus/Typedefs.h"
#include "modbus/UnitIdentifier.h"
#include "modbus/client/IClientSession.h"

namespace modbus
{

/**
 * @brief Client communication channel to a Modbus device
 *
 * This class represents a channel of communication with a Modbus device. It is responsible for
 * holding the physical connection, serializing the PDU in ADU and sequencing the outstanding requests.
 *
 * You cannot send requests directly from this class. You must create a session with the @ref create_session()
 * methods and send the requests from the returned @ref IClientSession.
 *
 * An instance of this class can be obtained via @ref IModbusManager::create_client_tcp_channel().
 *
 * @note Currently, the TCP channel supports only one outstanding request, meaning that the requests
 * are always sent sequentially. Therefore, before sending a new request, the channel waits for the response
 * of the previous request or its timeout.
 *
 * @see @ref IModbusManager, @ref IClientSession
 */
class IClientChannel : public std::enable_shared_from_this<IClientChannel>
{
public:
    /**
     * @brief Destructor
     *
     * @warning This destructor should never be called from user code. @ref IModbusManager
     * will hold a shared pointer to every channels it has created and will release them in
     * @ref IModbusManager::shutdown().
     *
     * @see @ref IModbusManager::shutdown(), @ref shutdown()
     */
    virtual ~IClientChannel() = default;

    /**
     * @brief Create a session using the channel
     * @param unit_identifier           Modbus unit identifier of the device. In TCP, this identifier is useless,
     *                                  but it must fit the slave unit identifier.
     * @param default_timeout           Default timeout of requests sent on the wire
     * @return Shared pointer to an instance of @ref IClientSession
     */
    virtual std::shared_ptr<IClientSession> create_session(const UnitIdentifier& unit_identifier,
                                                           const duration_t& default_timeout) = 0;

    /**
     * @brief Closes all the associated sessions.
     *
     * All the associated sessions are gracefully shutdown.
     *
     * @note This method is not usually called by user code, but by @ref IModbusManager::shutdown(). It is
     * still possible to shutdown a channel manually using this method, however please note that the
     * @ref IModbusManager will keep holding a shared pointer to it.
     *
     * @warning Once a channel is shutdown, sending requests through it is not permitted. Channels cannot
     * be restarted, but you can safely create a new one via @ref IModbusManager.
     *
     * @see @ref IModbusManager::shutdown()
     */
    virtual void shutdown() = 0;
};

} // namespace modbus

#endif //MODBUS_ICLIENTCHANNEL_H
