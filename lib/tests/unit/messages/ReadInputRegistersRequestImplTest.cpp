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
#include "catch.hpp"

#include <array>
#include "ser4cpp/container/Buffer.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"
#include "modbus/exceptions/ModbusException.h"
#include "messages/ReadInputRegistersRequestImpl.h"

using namespace modbus;

TEST_CASE("ReadInputRegistersRequestImpl")
{
    SECTION("Valid request")
    {
        const uint16_t starting_address = 0x1234;
        const uint16_t qty_of_registers = 4;
        ReadInputRegistersRequest request{starting_address, qty_of_registers};
        ReadInputRegistersRequestImpl request_impl{request};

        REQUIRE(request_impl.is_valid() == true);
        REQUIRE(request_impl.get_message_length() == 5);

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_message_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_message(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x04); // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12); // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34); // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00); // Qty of registers MSB
            REQUIRE(buffer.as_wslice()[4] == 0x04); // Qty of registers LSB
        }

        SECTION("When clone, then effectively creates a copy of the request")
        {
            auto copy = request_impl.clone();
            auto other_request = static_cast<ReadInputRegistersRequestImpl*>(copy.get());

            REQUIRE(other_request->get_request().starting_address == starting_address);
            REQUIRE(other_request->get_request().qty_of_registers == qty_of_registers);
            REQUIRE(&other_request->get_request() != &request);
        }

        SECTION("When quantity is 0, then request is not valid")
        {
            const uint16_t starting_address = 0x1234;
            ReadInputRegistersRequest request{starting_address, 0};
            ReadInputRegistersRequestImpl request_impl(request);

            REQUIRE(request_impl.is_valid() == false);
        }
    }

    SECTION("When quantity is higher than 125, then request is not valid")
    {
        const uint16_t starting_address = 0x1234;
        ReadInputRegistersRequest request{starting_address, 126};
        ReadInputRegistersRequestImpl request_impl(request);

        REQUIRE(request_impl.is_valid() == false);
    }

    SECTION("Parse")
    {
        SECTION("When proper request, then parse it properly") {
            std::array<uint8_t, 5> proper_request{{
                0x04,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x23, // Quantity of registers
            }};
            ser4cpp::rseq_t buffer{proper_request.data(), proper_request.size()};

            auto result = ReadInputRegistersRequestImpl::parse(buffer);

            REQUIRE(result.is_valid() == true);
            auto request = result.get();
            REQUIRE(request.starting_address == 0x1234);
            REQUIRE(request.qty_of_registers == 0x0023);
        }

        SECTION("When quantity of registers is 0, then parse return ModbusException 0x03") {
            std::array<uint8_t, 5> qty_of_registers_0_request{{
                0x04,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x00, // Quantity of registers (0)
            }};
            ser4cpp::rseq_t buffer{qty_of_registers_0_request.data(), qty_of_registers_0_request.size()};

            auto result = ReadInputRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataValue);
        }

        SECTION("When quantity of registers is greater than maximum, then parse return ModbusException 0x03") {
            std::array<uint8_t, 5> qty_of_registers_max_plus_one_request{{
                0x04,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x7E, // Quantity of registers (126)
            }};
            ser4cpp::rseq_t buffer{qty_of_registers_max_plus_one_request.data(), qty_of_registers_max_plus_one_request.size()};

            auto result = ReadInputRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataValue);
        }

        SECTION("When wrong size request, then return malformed exception")
        {
            std::array<uint8_t, 3> wrong_size_request{{
                0x04, // Function code
                0x42, 0x42 // Random data
            }};
            ser4cpp::rseq_t buffer{wrong_size_request.data(), wrong_size_request.size()};

            auto result = ReadInputRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When wrong function code, then return malformed exception")
        {
            std::array<uint8_t, 5> wrong_function_code_request{{
                0x42,       // Wrong function code
                0x12, 0x34, // Starting address
                0x01, 0x23, // Quantity of outputs
            }};
            ser4cpp::rseq_t buffer{wrong_function_code_request.data(), wrong_function_code_request.size()};

            auto result = ReadInputRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }
    }
}
