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
#include "modbus/server/db/InMemoryDatabase.h"

#include "modbus/exceptions/ModbusException.h"

namespace modbus
{

bool InMemoryDatabase::add_coil(Address address, bool value)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_coil_db.add_value(address, value);
}

bool InMemoryDatabase::add_discrete_input(Address address, bool value)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_discrete_input_db.add_value(address, value);
}

bool InMemoryDatabase::add_holding_register(Address address, uint16_t value)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_holding_register_db.add_value(address, value);
}

bool InMemoryDatabase::add_input_register(Address address, uint16_t value)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_input_register_db.add_value(address, value);
}

void InMemoryDatabase::execute_transaction(std::function<void(IDatabase& db)> transaction)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    transaction(*this);
}

Expected<ReadCoilsResponse> InMemoryDatabase::on_request(const ReadCoilsRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    std::vector<bool> values(request.qty_of_bits);
    auto result = m_coil_db.get_values(request.starting_address, values);
    if(!result)
    {
        return Expected<ReadCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    ReadCoilsResponse response{};
    for(size_t i = 0; i < request.qty_of_bits; ++i)
    {
        response.values.push_back(SingleBitValue{static_cast<Address>(request.starting_address + i), values[i]});
    }
    return Expected<ReadCoilsResponse>{response};
}

Expected<ReadDiscreteInputsResponse> InMemoryDatabase::on_request(const ReadDiscreteInputsRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    std::vector<bool> values(request.qty_of_bits);
    auto result = m_discrete_input_db.get_values(request.starting_address, values);
    if(!result)
    {
        return Expected<ReadDiscreteInputsResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    ReadDiscreteInputsResponse response{};
    for(size_t i = 0; i < request.qty_of_bits; ++i)
    {
        response.values.push_back(SingleBitValue{static_cast<Address>(request.starting_address + i), values[i]});
    }
    return Expected<ReadDiscreteInputsResponse>{response};
}

Expected<ReadHoldingRegistersResponse> InMemoryDatabase::on_request(const ReadHoldingRegistersRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    std::vector<uint16_t> values(request.qty_of_registers);
    auto result = m_holding_register_db.get_values(request.starting_address, values);
    if(!result)
    {
        return Expected<ReadHoldingRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    ReadHoldingRegistersResponse response{};
    for(size_t i = 0; i < request.qty_of_registers; ++i)
    {
        response.values.push_back(RegisterValue{static_cast<Address>(request.starting_address + i), values[i]});
    }
    return Expected<ReadHoldingRegistersResponse>{response};
}

Expected<ReadInputRegistersResponse> InMemoryDatabase::on_request(const ReadInputRegistersRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    std::vector<uint16_t> values(request.qty_of_registers);
    auto result = m_input_register_db.get_values(request.starting_address, values);
    if(!result)
    {
        return Expected<ReadInputRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    ReadInputRegistersResponse response{};
    for(size_t i = 0; i < request.qty_of_registers; ++i)
    {
        response.values.push_back(RegisterValue{static_cast<Address>(request.starting_address + i), values[i]});
    }
    return Expected<ReadInputRegistersResponse>{response};
}

Expected<WriteSingleCoilResponse> InMemoryDatabase::on_request(const WriteSingleCoilRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    auto result = m_coil_db.set_value(request.value.address, request.value.value);
    if(!result)
    {
        return Expected<WriteSingleCoilResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    WriteSingleCoilResponse response{SingleBitValue{request.value.address, request.value.value}};
    return Expected<WriteSingleCoilResponse>{response};
}

Expected<WriteSingleRegisterResponse> InMemoryDatabase::on_request(const WriteSingleRegisterRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    auto result = m_holding_register_db.set_value(request.value.address, request.value.value);
    if(!result)
    {
        return Expected<WriteSingleRegisterResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    WriteSingleRegisterResponse response{RegisterValue{request.value.address, request.value.value}};
    return Expected<WriteSingleRegisterResponse>{response};
}

Expected<WriteMultipleCoilsResponse> InMemoryDatabase::on_request(const WriteMultipleCoilsRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    auto result = m_coil_db.set_values(request.starting_address, request.values);
    if(!result)
    {
        return Expected<WriteMultipleCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    WriteMultipleCoilsResponse response{request.starting_address, static_cast<uint16_t>(request.values.size())};
    return Expected<WriteMultipleCoilsResponse>{response};
}

Expected<WriteMultipleRegistersResponse> InMemoryDatabase::on_request(const WriteMultipleRegistersRequest& request)
{
    std::lock_guard<std::mutex> lock{m_mutex};

    auto result = m_holding_register_db.set_values(request.starting_address, request.values);
    if(!result)
    {
        return Expected<WriteMultipleRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress});
    }

    WriteMultipleRegistersResponse response{request.starting_address, static_cast<uint16_t>(request.values.size())};
    return Expected<WriteMultipleRegistersResponse>{response};
}

bool InMemoryDatabase::get_coil(Address address, bool& output) const
{
    return m_coil_db.get_value(address, output);
}

bool InMemoryDatabase::set_coil(Address address, bool value)
{
    return m_coil_db.set_value(address, value);
}

bool InMemoryDatabase::get_discrete_input(Address address, bool& output) const
{
    return m_discrete_input_db.get_value(address, output);
}

bool InMemoryDatabase::set_discrete_input(Address address, bool value)
{
    return m_discrete_input_db.set_value(address, value);
}

bool InMemoryDatabase::get_holding_register(Address address, uint16_t& output) const
{
    return m_holding_register_db.get_value(address, output);
}

bool InMemoryDatabase::set_holding_register(Address address, uint16_t value)
{
    return m_holding_register_db.set_value(address, value);
}

bool InMemoryDatabase::get_input_register(Address address, uint16_t& output) const
{
    return m_input_register_db.get_value(address, output);
}

bool InMemoryDatabase::set_input_register(Address address, uint16_t value)
{
    return m_input_register_db.set_value(address, value);
}

} // namespace modbus
