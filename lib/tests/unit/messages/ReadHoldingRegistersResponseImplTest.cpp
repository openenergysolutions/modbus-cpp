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
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "messages/ReadHoldingRegistersResponseImpl.h"
#include "messages/ReadHoldingRegistersRequestImpl.h"

using namespace modbus;

TEST_CASE("ReadHoldingRegistersResponseImpl")
{
    const uint16_t starting_address = 0x0016;
    const uint16_t qty_of_registers = 4;
    ReadHoldingRegistersRequest request{starting_address, qty_of_registers};
    ReadHoldingRegistersRequestImpl request_impl{request};

    SECTION("When proper response, then parse it properly")
    {
        std::array<uint8_t, 10> proper_response{{
            0x03,       // Function code
            0x08,       // Number of bytes
            0x00, 0x01, // Register 0x0016
            0x00, 0x02, // Register 0x0017
            0x00, 0x03, // Register 0x0018
            0x00, 0x04  // Register 0x0019
        }};
        ser4cpp::rseq_t buffer{proper_response.data(), proper_response.size()};

        auto result = ReadHoldingRegistersResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.is_valid() == true);
        auto response = result.get();
        auto values = response.values;
        REQUIRE(values.size() == 4);
        REQUIRE(values[0].address == 0x0016);
        REQUIRE(values[0].value == 0x0001);
        REQUIRE(values[1].address == 0x0017);
        REQUIRE(values[1].value == 0x0002);
        REQUIRE(values[2].address == 0x0018);
        REQUIRE(values[2].value == 0x0003);
        REQUIRE(values[3].address == 0x0019);
        REQUIRE(values[3].value == 0x0004);
    }

    SECTION("When exception response, then parse report exception")
    {
        std::array<uint8_t, 2> exception_response{{
            0x83, // Exception function code
            0x02  // Illegal data address
        }};
        ser4cpp::rseq_t buffer{exception_response.data(), exception_response.size()};

        auto result = ReadHoldingRegistersResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<ModbusException>() == true);
        REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
    }

    SECTION("When size not present, then return malformed exception")
    {
        std::array<uint8_t, 1> wrong_size_response{ {
            0x03 // Function code
        }};
        ser4cpp::rseq_t buffer{ wrong_size_response.data(), wrong_size_response.size() };

        auto result = ReadHoldingRegistersResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When size is not even, then return malformed exception")
    {
        std::array<uint8_t, 5> wrong_size_response{ {
            0x03,            // Function code
            0x03,            // Size not even
            0x42, 0x43, 0x44 // Appropriate data
        }};
        ser4cpp::rseq_t buffer{ wrong_size_response.data(), wrong_size_response.size() };

        auto result = ReadHoldingRegistersResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When wrong size response, then return malformed exception")
    {
        std::array<uint8_t, 3> wrong_size_response{{
            0x03, // Function code
            0x02, // Length should be 2
            0x42  // Only 1 byte is received
        }};
        ser4cpp::rseq_t buffer{wrong_size_response.data(), wrong_size_response.size()};

        auto result = ReadHoldingRegistersResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }
}
