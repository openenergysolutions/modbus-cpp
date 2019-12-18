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
#ifndef MODBUS_INMEMORYDATABASE_H
#define MODBUS_INMEMORYDATABASE_H

/** @file
 * @brief Class @ref modbus::InMemoryDatabase
 */

#include <functional>
#include <memory>
#include <mutex>

#include "modbus/server/IServerSession.h"
#include "modbus/server/db/BitDatabase.h"
#include "modbus/server/db/IDatabase.h"
#include "modbus/server/db/RegisterDatabase.h"

namespace modbus
{

/**
 * @brief In-memory database session
 * 
 * This basic implementation of @ref IServerSession holds an in-memory database of
 * the values and offers methods to update them. It handles all the Modbus requests
 * appropriately.
 * 
 * This class is synchronized by a mutex so it can be used by many channels. The values
 * can also be updated from any thread and the transaction mechanism offers a way of
 * doing complex operations completely thread-safe.
 */
class InMemoryDatabase : public IServerSession, public IDatabase
{
public:
    /**
     * @brief Destructor
     *
     * @warning This destructor should never be called from user code. @ref IServerChannel
     * will hold a shared pointer to it and will release it in @ref IServerChannel::shutdown().
     *
     * @see @ref IServerChannel::shutdown()
     */
    virtual ~InMemoryDatabase() = default;

    /**
     * @brief Add a coil to the database.
     * @param address Address of the coil
     * @param value Initial value of the coil
     * @return @cpp true @ce if no coil with the specified address existed, @cpp false @ce otherwise.
     */
    bool add_coil(Address address, bool value);

    /**
     * @brief Add a discrete input to the database.
     * @param address Address of the discrete input
     * @param value Initial value of the discrete input
     * @return @cpp true @ce if no discrete input with the specified address existed, @cpp false @ce otherwise.
     */
    bool add_discrete_input(Address address, bool value);

    /**
     * @brief Add a holding register to the database.
     * @param address Address of the holding register
     * @param value Initial value of the holding register
     * @return @cpp true @ce if no holding register with the specified address existed, @cpp false @ce otherwise.
     */
    bool add_holding_register(Address address, uint16_t value);

    /**
     * @brief Add a input register to the database.
     * @param address Address of the input register
     * @param value Initial value of the input register
     * @return @cpp true @ce if no input register with the specified address existed, @cpp false @ce otherwise.
     */
    bool add_input_register(Address address, uint16_t value);

    /**
     * @brief Execute a transaction on the database.
     * @param transaction Function to execute as the transaction
     * 
     * The @ref IDatabase passed in argument can be used to manipulate the database.
     * 
     * The transaction will be executed with an internal mutex acquired, so
     * this method is completely thread-safe.
     */
    void execute_transaction(std::function<void(IDatabase& db)> transaction);

protected:
    // IServerSession
    Expected<ReadCoilsResponse> on_request(const ReadCoilsRequest& request) override;
    Expected<ReadDiscreteInputsResponse> on_request(const ReadDiscreteInputsRequest& request) override;
    Expected<ReadHoldingRegistersResponse> on_request(const ReadHoldingRegistersRequest& request) override;
    Expected<ReadInputRegistersResponse> on_request(const ReadInputRegistersRequest& request) override;
    Expected<WriteSingleCoilResponse> on_request(const WriteSingleCoilRequest& request) override;
    Expected<WriteSingleRegisterResponse> on_request(const WriteSingleRegisterRequest& request) override;
    Expected<WriteMultipleCoilsResponse> on_request(const WriteMultipleCoilsRequest& request) override;
    Expected<WriteMultipleRegistersResponse> on_request(const WriteMultipleRegistersRequest& request) override;

    // IDatabase
    bool get_coil(Address address, bool& output) const override;
    bool set_coil(Address address, bool value) override;

    bool get_discrete_input(Address address, bool& output) const override;
    bool set_discrete_input(Address address, bool value) override;

    bool get_holding_register(Address address, uint16_t& output) const override;
    bool set_holding_register(Address address, uint16_t value) override;

    bool get_input_register(Address address, uint16_t& output) const override;
    bool set_input_register(Address address, uint16_t value) override;

private:
    std::mutex m_mutex;
    BitDatabase m_coil_db;
    BitDatabase m_discrete_input_db;
    RegisterDatabase m_holding_register_db;
    RegisterDatabase m_input_register_db;
};

} // namespace modbus

#endif //MODBUS_INMEMORYDATABASE_H
