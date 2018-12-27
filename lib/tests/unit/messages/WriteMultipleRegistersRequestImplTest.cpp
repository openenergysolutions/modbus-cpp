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
#include "messages/WriteMultipleRegistersRequestImpl.h"

using namespace modbus;

TEST_CASE("WriteMultipleRegistersRequestImpl")
{
    const uint16_t starting_address = 0x1234;
    WriteMultipleRegistersRequest request{starting_address};

    SECTION("Normal request")
    {
        request.values.emplace_back(0x1111);
        request.values.emplace_back(0x1122);
        request.values.emplace_back(0x1133);

        WriteMultipleRegistersRequestImpl request_impl{request};

        SECTION("When get length, then return actual length.")
        {
            auto length = request_impl.get_message_length();

            REQUIRE(length == 6 + 6);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_message_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_message(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x10);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of registers value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x03);  // Qty of registers value LSB
            REQUIRE(buffer.as_wslice()[5] == 6);     // Byte count
            REQUIRE(buffer.as_wslice()[6] == 0x11);  // First register MSB
            REQUIRE(buffer.as_wslice()[7] == 0x11);  // First register LSB
            REQUIRE(buffer.as_wslice()[8] == 0x11);  // Second register MSB
            REQUIRE(buffer.as_wslice()[9] == 0x22);  // Second register LSB
            REQUIRE(buffer.as_wslice()[10] == 0x11); // Third register MSB
            REQUIRE(buffer.as_wslice()[11] == 0x33); // Third register LSB
        }

        SECTION("When clone, then effectively creates a copy of the request")
        {
            auto copy = request_impl.clone();
            auto other_request = static_cast<WriteMultipleRegistersRequestImpl*>(copy.get());

            REQUIRE(other_request->get_request().starting_address == starting_address);
            REQUIRE(other_request->get_request().values.size() == 3);
            REQUIRE(&other_request->get_request() != &request);
        }
    }

    SECTION("When empty request, then is invalid")
    {
        WriteMultipleRegistersRequest empty_request{starting_address, {}};
        WriteMultipleRegistersRequestImpl emtpy_request_impl{empty_request};

        REQUIRE(emtpy_request_impl.is_valid() == false);
    }

    SECTION("When request too large, then is invalid")
    {
        std::vector<uint16_t> values;
        for(unsigned int i = 0; i < 124; ++i)
        {
            values.emplace_back(0x42);
        }
        WriteMultipleRegistersRequest large_request{starting_address, values};
        WriteMultipleRegistersRequestImpl large_request_impl{large_request};

        REQUIRE(large_request_impl.is_valid() == false);
    }

    SECTION("Parse")
    {
        SECTION("When proper request, then parse it properly") {
            std::array<uint8_t, 10> proper_request{{
                0x10,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x02, // Quantity of registers (2)
                0x04,       // Byte count (4)
                0x12, 0x34,
                0x56, 0x78  // Register values
            }};
            ser4cpp::rseq_t buffer{proper_request.data(), proper_request.size()};

            auto result = WriteMultipleRegistersRequestImpl::parse(buffer);

            REQUIRE(result.is_valid() == true);

            auto response = result.get();
            REQUIRE(response.starting_address == 0x1234);

            auto values = response.values;
            REQUIRE(values.size() == 2);
            REQUIRE(values[0] == 0x1234);
            REQUIRE(values[1] == 0x5678);
        }

        SECTION("When mismatch between quantity of registers and byte count, then parse report exception") {
            std::array<uint8_t, 10> mismatch_request{{
                0x10,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x02, // Quantity of registers (2)
                0x02,       // Byte count (2, should be 4)
                0x42, 0x42,
                0x42, 0x42  // Register values
            }};
            ser4cpp::rseq_t buffer{mismatch_request.data(), mismatch_request.size()};

            auto result = WriteMultipleRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When mismatch between byte count and actual byte count, then parse report exception") {
            std::array<uint8_t, 11> mismatch_request{{
                0x10,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x02, // Quantity of registers (2)
                0x04,       // Byte count (4)
                0x42, 0x42,
                0x42, 0x42,
                0x42         // Extra register values
            }};
            ser4cpp::rseq_t buffer{mismatch_request.data(), mismatch_request.size()};

            auto result = WriteMultipleRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When invalid quantity of registers, then parse report exception") {
            std::array<uint8_t, 254> invalid_quantity_of_registers_request{{
                0x10,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x7C, // Invalid quantity of registers (124)
                0xF8,       // Byte count (248)
            }};
            ser4cpp::rseq_t buffer{invalid_quantity_of_registers_request.data(), invalid_quantity_of_registers_request.size()};

            auto result = WriteMultipleRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When invalid function code, then parse report exception") {
            std::array<uint8_t, 10> invalid_function_code_request{{
                0x42,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x02, // Quantity of registers (12)
                0x04,       // Byte count (4)
                0x42, 0x42,
                0x42, 0x42  // Register values
            }};
            ser4cpp::rseq_t buffer{invalid_function_code_request.data(), invalid_function_code_request.size()};

            auto result = WriteMultipleRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When request is too short, then parse report exception") {
            std::array<uint8_t, 5> too_short_request{{
                0x10,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x02, // Quantity of registers (2)
            }};
            ser4cpp::rseq_t buffer{too_short_request.data(), too_short_request.size()};

            auto result = WriteMultipleRegistersRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }
    }
}
