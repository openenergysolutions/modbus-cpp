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

#include <array>
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "mocks/IMessageMock.h"

using namespace modbus;

TEST_CASE("IMessage")
{
    uint8_t function_code = 0x03;

    SECTION("When corresponding function code, then return true")
    {
        std::array<uint8_t, 3> good_function_code{{
            0x03,       // Good function code
            0x42, 0x43 // Data
        }};
        ser4cpp::rseq_t buffer{good_function_code.data(), static_cast<uint32_t>(good_function_code.size())};

        auto result = IMessageMock::parse_function_code(function_code, buffer);

        REQUIRE(result.is_valid() == true);
        REQUIRE(buffer[0] == 0x42);
    }

    SECTION("When buffer is empty, then return malformed exception")
    {
        auto data = ser4cpp::rseq_t::empty();

        auto result = IMessageMock::parse_function_code(function_code, data);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When wrong function code, then return malformed exception")
    {
        std::array<uint8_t, 3> wrong_function_code{{
            0x01,       // Wrong function code
            0x00, 0x00 // Data
        }};
        ser4cpp::rseq_t buffer{wrong_function_code.data(), static_cast<uint32_t>(wrong_function_code.size())};

        auto result = IMessageMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When wrong exception function code, then return malformed exception")
    {
        std::array<uint8_t, 3> wrong_exception_function_code{{
            0x81,      // Wrong exception function code
            0x00, 0x00 // Data
        }};
        ser4cpp::rseq_t buffer{wrong_exception_function_code.data(), static_cast<uint32_t>(wrong_exception_function_code.size())};

        auto result = IMessageMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When exception function code with invalid length, then return malformed exception")
    {
        std::array<uint8_t, 3> exception_function_code_invalid_length{{
            0x83,      // Exception function code
            0x02, 0x03 // Data of invalid length
        }};
        ser4cpp::rseq_t buffer{exception_function_code_invalid_length.data(), static_cast<uint32_t>(exception_function_code_invalid_length.size())};

        auto result = IMessageMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When exception function code without exception code, then return malformed exception")
    {
        std::array<uint8_t, 1> exception_function_without_code{{
            0x83,      // Exception function code
        }};
        ser4cpp::rseq_t buffer{exception_function_without_code.data(), static_cast<uint32_t>(exception_function_without_code.size())};

        auto result = IMessageMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When exception function code, then return modbus exception")
    {
        std::array<uint8_t, 2> exception_function_code{{
            0x83,      // Exception function code
            0x02       // Illegal data address
        }};
        ser4cpp::rseq_t buffer{exception_function_code.data(), static_cast<uint32_t>(exception_function_code.size())};

        auto result = IMessageMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<ModbusException>() == true);
        REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
    }
}
