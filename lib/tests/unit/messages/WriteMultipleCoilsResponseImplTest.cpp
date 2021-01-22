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
#include "ser4cpp/container/Buffer.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "messages/WriteMultipleCoilsResponseImpl.h"

using namespace modbus;

TEST_CASE("WriteMultpileCoilsResponseImpl")
{
    const uint16_t starting_address = 0x1234;
    const uint16_t qty_of_outputs = 42;
    WriteMultipleCoilsResponse request{starting_address, qty_of_outputs};
    WriteMultipleCoilsResponseImpl request_impl{request};

    REQUIRE(request_impl.is_valid() == true);
    REQUIRE(request_impl.get_message_length() == 5);

    SECTION("When build request, then write appropriate values to the buffer")
    {
        ser4cpp::Buffer buffer{(uint32_t) request_impl.get_message_length()};
        auto slice = buffer.as_wslice();

        request_impl.build_message(slice);

        REQUIRE(buffer.as_wslice()[0] == 0x0F); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Starting address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Starting address LSB
        REQUIRE(buffer.as_wslice()[3] == 0x00); // Quantity of outputs MSB
        REQUIRE(buffer.as_wslice()[4] == 0x2A); // Quantity of outputs LSB
    }

    SECTION("When starting address is 0, then request is not valid")
    {
        WriteMultipleCoilsResponse invalid_request{starting_address, 0};
        WriteMultipleCoilsResponseImpl invalid_request_impl{invalid_request};

        REQUIRE(invalid_request_impl.is_valid() == false);
    }

    SECTION("When starting address is greater than maximum, then request is not valid")
    {
        WriteMultipleCoilsResponse invalid_request{starting_address, 2001};
        WriteMultipleCoilsResponseImpl invalid_request_impl{invalid_request};

        REQUIRE(invalid_request_impl.is_valid() == false);
    }

    SECTION("When clone, then effectively creates a copy of the request")
    {
        auto copy = request_impl.clone();

        REQUIRE(&request_impl != copy.get());
    }

    SECTION("Parse")
    {
        const uint16_t starting_address = 0x1234;
        const uint16_t qty_of_coils = 3;
        WriteMultipleCoilsRequest request{starting_address, {}};
        WriteMultipleCoilsRequestImpl request_impl{request};

        SECTION("When proper response, then parse it properly")
        {
            std::array<uint8_t, 5> proper_response{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x03  // Qty of registers
            }};
            ser4cpp::rseq_t buffer{proper_response.data(), static_cast<uint32_t>(proper_response.size())};

            auto result = WriteMultipleCoilsResponseImpl::parse(request, buffer);

            REQUIRE(result.is_valid() == true);
            auto response = result.get();
            REQUIRE(response.starting_address == starting_address);
            REQUIRE(response.qty_of_coils == qty_of_coils);
        }

        SECTION("When exception response, then parse report exception")
        {
            std::array<uint8_t, 2> exception_response{{
                0x8F,       // Exception function code
                0x02        // Illegal data address
            }};
            ser4cpp::rseq_t buffer{exception_response.data(), static_cast<uint32_t>(exception_response.size())};

            auto result = WriteMultipleCoilsResponseImpl::parse(request, buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
        }

        SECTION("When too small, then return malformed exception")
        {
            std::array<uint8_t, 1> too_small_response{{
                0x0F // Function code
            }};
            ser4cpp::rseq_t buffer{ too_small_response.data(), static_cast<uint32_t>(too_small_response.size())};

            auto result = WriteMultipleCoilsResponseImpl::parse(request, buffer);

            REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
        }

        SECTION("When too big, then return malformed exception")
        {
            std::array<uint8_t, 7> too_big_response{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x03, // Qty of coils
                0x42, 0x42  // Junk
            }};
            ser4cpp::rseq_t buffer{ too_big_response.data(), static_cast<uint32_t>(too_big_response.size())};

            auto result = WriteMultipleCoilsResponseImpl::parse(request, buffer);

            REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
        }
    }
}
