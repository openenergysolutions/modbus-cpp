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
#include <catch2/catch.hpp>

#include "modbus/server/db/InMemoryDatabase.h"

using namespace modbus;

TEST_CASE("InMemoryDatabase")
{
    InMemoryDatabase db{};

    SECTION("Coils")
    {
        const Address address = 0x1234;
        auto result = db.add_coil(address, true);
        REQUIRE(result == true);

        SECTION("get/set in transaction")
        {
            db.execute_transaction([=](IDatabase& database) {
                bool value;
                auto get_result = database.get_coil(address, value);
                REQUIRE(get_result == true);
                REQUIRE(value == true);

                database.set_coil(address, false);
                database.get_coil(address, value);
                REQUIRE(value == false);

                auto invalid_get_result = database.get_coil(0xFFFF, value);
                REQUIRE(invalid_get_result == false);
            });
        }

        SECTION("Read request")
        {
            auto session = static_cast<IServerSession*>(&db);

            SECTION("Valid address")
            {
                auto result = session->on_request(ReadCoilsRequest{address, 1});
                REQUIRE(result.is_valid() == true);
                auto values = result.get().values;
                REQUIRE(values.size() == 1);
                REQUIRE(values[0].address == address);
                REQUIRE(values[0].value == true);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(ReadCoilsRequest{0xFFFF, 1});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }

        SECTION("Write single request")
        {
            auto session = static_cast<IServerSession*>(&db);

            SECTION("Valid address")
            {
                auto result = session->on_request(WriteSingleCoilRequest{address, false});
                REQUIRE(result.is_valid() == true);
                auto value = result.get().value;
                REQUIRE(value.address == address);
                REQUIRE(value.value == false);

                bool get_value;
                db.execute_transaction([&](IDatabase& database) {
                    database.get_coil(address, get_value);
                });
                REQUIRE(get_value == false);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(WriteSingleCoilRequest{0xFFFF, true});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }

        SECTION("Write multiple request")
        {
            auto session = static_cast<IServerSession*>(&db);
            db.add_coil(address + 1, false);

            SECTION("Valid address")
            {
                auto result = session->on_request(WriteMultipleCoilsRequest{address, {false, true}});
                REQUIRE(result.is_valid() == true);
                auto value = result.get();
                REQUIRE(value.starting_address == address);
                REQUIRE(value.qty_of_coils == 2);

                bool first_value;
                bool second_value;
                db.execute_transaction([&](IDatabase& database) {
                    database.get_coil(address, first_value);
                    database.get_coil(address + 1, second_value);
                });
                REQUIRE(first_value == false);
                REQUIRE(second_value == true);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(WriteMultipleCoilsRequest{0xEEEE, {true, false}});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }
    }

    SECTION("Discrete input")
    {
        const Address address = 0x1234;
        auto result = db.add_discrete_input(address, true);
        REQUIRE(result == true);

        SECTION("get/set in transaction")
        {
            db.execute_transaction([=](IDatabase& database) {
                bool value;
                auto get_result = database.get_discrete_input(address, value);
                REQUIRE(get_result == true);
                REQUIRE(value == true);

                database.set_discrete_input(address, false);
                database.get_discrete_input(address, value);
                REQUIRE(value == false);

                auto invalid_get_result = database.get_discrete_input(0xFFFF, value);
                REQUIRE(invalid_get_result == false);
            });
        }

        SECTION("Read request")
        {
            auto session = static_cast<IServerSession*>(&db);

            SECTION("Valid address")
            {
                auto result = session->on_request(ReadDiscreteInputsRequest{address, 1});
                REQUIRE(result.is_valid() == true);
                auto values = result.get().values;
                REQUIRE(values.size() == 1);
                REQUIRE(values[0].address == address);
                REQUIRE(values[0].value == true);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(ReadDiscreteInputsRequest{0xFFFF, 1});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }
    }

    SECTION("Holding register")
    {
        const Address address = 0x1234;
        const uint16_t start_value = 0x4242;
        auto result = db.add_holding_register(address, start_value);
        REQUIRE(result == true);

        SECTION("get/set in transaction")
        {
            db.execute_transaction([=](IDatabase& database) {
                uint16_t value;
                auto get_result = database.get_holding_register(address, value);
                REQUIRE(get_result == true);
                REQUIRE(value == start_value);

                database.set_holding_register(address, 0xFFFF);
                database.get_holding_register(address, value);
                REQUIRE(value == 0xFFFF);

                auto invalid_get_result = database.get_holding_register(0xFFFF, value);
                REQUIRE(invalid_get_result == false);
            });
        }

        SECTION("Read request")
        {
            auto session = static_cast<IServerSession*>(&db);

            SECTION("Valid address")
            {
                auto result = session->on_request(ReadHoldingRegistersRequest{address, 1});
                REQUIRE(result.is_valid() == true);
                auto values = result.get().values;
                REQUIRE(values.size() == 1);
                REQUIRE(values[0].address == address);
                REQUIRE(values[0].value == start_value);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(ReadHoldingRegistersRequest{0xFFFF, 1});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }

        SECTION("Write single request")
        {
            auto session = static_cast<IServerSession*>(&db);

            SECTION("Valid address")
            {
                auto result = session->on_request(WriteSingleRegisterRequest{address, 0x7676});
                REQUIRE(result.is_valid() == true);
                auto value = result.get().value;
                REQUIRE(value.address == address);
                REQUIRE(value.value == 0x7676);

                uint16_t get_value;
                db.execute_transaction([&](IDatabase& database) {
                    database.get_holding_register(address, get_value);
                });
                REQUIRE(get_value == 0x7676);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(WriteSingleRegisterRequest{0xFFFF, 0x4242});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }

        SECTION("Write multiple request")
        {
            auto session = static_cast<IServerSession*>(&db);
            db.add_holding_register(address + 1, 0xAAAA);

            SECTION("Valid address")
            {
                auto result = session->on_request(WriteMultipleRegistersRequest{address, {0x2222, 0x3333}});
                REQUIRE(result.is_valid() == true);
                auto value = result.get();
                REQUIRE(value.starting_address == address);
                REQUIRE(value.qty_of_registers == 2);

                uint16_t first_value;
                uint16_t second_value;
                db.execute_transaction([&](IDatabase& database) {
                    database.get_holding_register(address, first_value);
                    database.get_holding_register(address + 1, second_value);
                });
                REQUIRE(first_value == 0x2222);
                REQUIRE(second_value == 0x3333);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(WriteMultipleRegistersRequest{0xEEEE, {0x2222, 0x3333}});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }
    }

    SECTION("Input register")
    {
        const Address address = 0x1234;
        const uint16_t start_value = 0x4242;
        auto result = db.add_input_register(address, start_value);
        REQUIRE(result == true);

        SECTION("get/set in transaction")
        {
            db.execute_transaction([=](IDatabase& database) {
                uint16_t value;
                auto get_result = database.get_input_register(address, value);
                REQUIRE(get_result == true);
                REQUIRE(value == start_value);

                database.set_input_register(address, 0xFFFF);
                database.get_input_register(address, value);
                REQUIRE(value == 0xFFFF);

                auto invalid_get_result = database.get_input_register(0xEEEE, value);
                REQUIRE(invalid_get_result == false);
            });
        }

        SECTION("Read request")
        {
            auto session = static_cast<IServerSession*>(&db);

            SECTION("Valid address")
            {
                auto result = session->on_request(ReadInputRegistersRequest{address, 1});
                REQUIRE(result.is_valid() == true);
                auto values = result.get().values;
                REQUIRE(values.size() == 1);
                REQUIRE(values[0].address == address);
                REQUIRE(values[0].value == start_value);
            }

            SECTION("Invalid address")
            {
                auto result = session->on_request(ReadInputRegistersRequest{0xEEEE, 1});
                REQUIRE(result.has_exception<ModbusException>() == true);
                REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
            }
        }
    }
}
