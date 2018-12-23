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
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "messages/WriteSingleRegisterResponseImpl.h"

using namespace modbus;

TEST_CASE("WriteSingleRegisterResponse")
{
    const uint16_t address = 0x1234;
    const uint16_t value = 0x6789;
    WriteSingleRegisterResponse response{{address, value}};
    WriteSingleRegisterResponseImpl response_impl{response};

    REQUIRE(response_impl.is_valid() == true);
    REQUIRE(response_impl.get_message_length() == 5);

    SECTION("When build request, then write appropriate values to the buffer")
    {
        ser4cpp::Buffer buffer{(uint32_t)response_impl.get_message_length()};
        auto slice = buffer.as_wslice();

        response_impl.build_message(slice);

        REQUIRE(buffer.as_wslice()[0] == 0x06); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Address LSB
        REQUIRE(buffer.as_wslice()[3] == 0x67); // Register value MSB
        REQUIRE(buffer.as_wslice()[4] == 0x89); // Register value LSB
    }

    SECTION("When clone, then effectively creates a copy of the response")
    {
        auto copy = response_impl.clone();

        REQUIRE(&response_impl != copy.get());
    }

    SECTION("Parse")
    {
        const uint16_t address = 0x1234;
        const uint16_t value = 0x6789;
        WriteSingleRegisterRequest request{{address, value}};
        WriteSingleRegisterRequestImpl request_impl{request};

        SECTION("When proper response, then parse it properly")
        {
            std::array<uint8_t, 5> proper_response{{
                0x06,       // Function code
                0x12, 0x34, // Address
                0x67, 0x89  // Value
            }};
            ser4cpp::rseq_t buffer{proper_response.data(), proper_response.size()};

            auto result = WriteSingleRegisterResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.is_valid() == true);
            auto response = result.get();
            REQUIRE(response.value.address == address);
            REQUIRE(response.value.value == value);
        }

        SECTION("When exception response, then parse report exception")
        {
            std::array<uint8_t, 2> exception_response{{
                0x86,       // Exception function code
                0x02        // Illegal data address
            }};
            ser4cpp::rseq_t buffer{exception_response.data(), exception_response.size()};

            auto result = WriteSingleRegisterResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
        }

        SECTION("When too small, then return malformed exception")
        {
            std::array<uint8_t, 1> too_small_response{ {
                0x06 // Function code
            }};
            ser4cpp::rseq_t buffer{ too_small_response.data(), too_small_response.size() };

            auto result = WriteSingleRegisterResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
        }

        SECTION("When too big, then return malformed exception")
        {
            std::array<uint8_t, 7> too_big_response{{
                0x06,       // Function code
                0x12, 0x34, // Address
                0x67, 0x89, // Value
                0x42, 0x42  // Junk
            }};
            ser4cpp::rseq_t buffer{ too_big_response.data(), too_big_response.size() };

            auto result = WriteSingleRegisterResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
        }
    }
}
