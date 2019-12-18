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
#ifndef MODBUS_ISERVERCHANNEL_H
#define MODBUS_ISERVERCHANNEL_H

/** @file
 * @brief Interface @ref modbus::IServerChannel
 */

#include <memory>
#include "modbus/UnitIdentifier.h"
#include "modbus/server/IServerSession.h"

namespace modbus
{

/**
 * @brief Server communication channel for a Modbus slave
 *
 * This class represents a server accepting connections from masters. It is responsible for
 * holding acception the connections, deserializing the PDU in ADU and sending the requests to the sessions.
 *
 * A @ref IServerSession must be created and added with @ref add_session() in order to actually handle the requests.
 * The server can then be started with @ref start().
 *
 * An instance of this class can be obtained via @ref IModbusManager::create_server_tcp_channel().
 * 
 * The server can only maintain a limited amount of connections opened at a time. If a new connection
 * is established to the server while the maximum number of connections is already reached, then the oldest
 * connection is closed and the newest connection is accepted. The number of connections can be specified
 * in @ref IModbusManager::create_server_tcp_channel().
 *
 * @see @ref IModbusManager, @ref IServerSession
 */
class IServerChannel : public std::enable_shared_from_this<IServerChannel>
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
    virtual ~IServerChannel() = default;

    /**
     * @brief Start listening to incoming connections.
     */
    virtual void start() = 0;

    /**
     * @brief Closes all the associated sessions.
     *
     * All the active connections will be gracefully closed.
     *
     * @note This method is not usually called by user code, but by @ref IModbusManager::shutdown(). It is
     * still possible to shutdown a channel manually using this method, however please note that the
     * @ref IModbusManager will keep holding a shared pointer to it.
     *
     * @warning Channels cannot be restarted, but you can safely create a new one via @ref IModbusManager.
     *
     * @see @ref IModbusManager::shutdown()
     */
    virtual void shutdown() = 0;

    /**
     * @brief Attach a new session to the channel.
     * @param unit_identifier Unit identifier associated with the session.
     * @param session Shared pointer to the session handler
     * 
     * @note If a session is added with an already used unit_identifier, than the old session is
     * replaced with the new one.
     */
    virtual void add_session(const UnitIdentifier& unit_identifier, std::shared_ptr<IServerSession> session) = 0;
};

} // namespace modbus

#endif //MODBUS_ISERVERCHANNEL_H
