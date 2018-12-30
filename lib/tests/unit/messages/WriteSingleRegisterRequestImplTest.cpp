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
#include <memory>
#include "ser4cpp/container/Buffer.h"
#include "modbus/exceptions/MalformedModbusRequestException.h"
#include "messages/WriteSingleRegisterRequestImpl.h"

using namespace modbus;

TEST_CASE("WriteSingleRegisterRequestImpl")
{
    const uint16_t address = 0x1234;
    const uint16_t value = 0x6789;
    WriteSingleRegisterRequest request{{address, value}};
    WriteSingleRegisterRequestImpl request_impl{request};

    REQUIRE(request_impl.is_valid() == true);
    REQUIRE(request_impl.get_message_length() == 5);

    SECTION("When build request, then write appropriate values to the buffer")
    {
        ser4cpp::Buffer buffer{(uint32_t)request_impl.get_message_length()};
        auto slice = buffer.as_wslice();

        request_impl.build_message(slice);

        REQUIRE(buffer.as_wslice()[0] == 0x06); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Address LSB
        REQUIRE(buffer.as_wslice()[3] == 0x67); // Register value MSB
        REQUIRE(buffer.as_wslice()[4] == 0x89); // Register value LSB
    }

    SECTION("When clone, then effectively creates a copy of the request")
    {
        auto copy = request_impl.clone();
        auto other_request = static_cast<WriteSingleRegisterRequestImpl*>(copy.get());

        REQUIRE(other_request->get_request().value.address == address);
        REQUIRE(other_request->get_request().value.value == value);
        REQUIRE(&other_request->get_request() != &request);
    }

    SECTION("Parse")
    {
        const uint16_t address = 0x1234;
        const uint16_t value = 0x6789;

        SECTION("When proper request, then parse it properly")
        {
            std::array<uint8_t, 5> proper_request{{
                0x06,       // Function code
                0x12, 0x34, // Address
                0x67, 0x89  // Value
            }};
            ser4cpp::rseq_t buffer{proper_request.data(), static_cast<uint32_t>(proper_request.size())};

            auto result = WriteSingleRegisterRequestImpl::parse(buffer);

            REQUIRE(result.is_valid() == true);
            auto request = result.get();
            REQUIRE(request.value.address == address);
            REQUIRE(request.value.value == value);
        }

        SECTION("When wrong function code, then parse report exception")
        {
            std::array<uint8_t, 5> proper_request{{
                0x42,       // Wrong function code
                0x12, 0x34, // Address
                0x67, 0x89  // Value
            }};
            ser4cpp::rseq_t buffer{proper_request.data(), static_cast<uint32_t>(proper_request.size())};

            auto result = WriteSingleRegisterRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When too small, then return malformed exception")
        {
            std::array<uint8_t, 1> too_small_request{ {
                0x06 // Function code
            }};
            ser4cpp::rseq_t buffer{too_small_request.data(), static_cast<uint32_t>(too_small_request.size())};

            auto result = WriteSingleRegisterRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When too big, then return malformed exception")
        {
            std::array<uint8_t, 7> too_big_request{{
                0x06,       // Function code
                0x12, 0x34, // Address
                0x67, 0x89, // Value
                0x42, 0x42  // Junk
            }};
            ser4cpp::rseq_t buffer{too_big_request.data(), static_cast<uint32_t>(too_big_request.size())};

            auto result = WriteSingleRegisterRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }
    }
}
