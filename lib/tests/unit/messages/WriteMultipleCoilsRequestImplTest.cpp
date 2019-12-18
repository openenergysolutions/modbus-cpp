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
#include "modbus/exceptions/ModbusException.h"
#include "messages/WriteMultipleCoilsRequestImpl.h"

using namespace modbus;

TEST_CASE("WriteMultipleCoilsRequestImpl")
{
    const uint16_t starting_address = 0x1234;
    WriteMultipleCoilsRequest request{starting_address, {false, true, false, true,
                                                         false, false, true, true}};

    SECTION("Normal request")
    {
        WriteMultipleCoilsRequestImpl request_impl{request};

        SECTION("When get length, then return actual length.")
        {
            auto length = request_impl.get_message_length();

            REQUIRE(length == 6 + 1);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_message_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_message(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x0F);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of coils value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x08);  // Qty of coils value LSB
            REQUIRE(buffer.as_wslice()[5] == 0x01);     // Byte count
            REQUIRE(buffer.as_wslice()[6] == 0b11001010);  // 8 coils
        }

        SECTION("When clone, then effectively creates a copy of the request")
        {
            auto copy = request_impl.clone();
            auto other_request = static_cast<WriteMultipleCoilsRequestImpl*>(copy.get());

            REQUIRE(other_request->get_request().starting_address == starting_address);
            REQUIRE(other_request->get_request().values.size() == 8);
            REQUIRE(&other_request->get_request() != &request);
        }
    }

    SECTION("Request with number of coils not a multiple of 8")
    {
        request.values.emplace_back(true);
        request.values.emplace_back(false);
        request.values.emplace_back(true);
        WriteMultipleCoilsRequestImpl request_impl{request};

        SECTION("When get length, then return actual length.")
        {
            auto length = request_impl.get_message_length();

            REQUIRE(length == 6 + 2);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_message_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_message(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x0F);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of coils value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x0B);  // Qty of coils value LSB
            REQUIRE(buffer.as_wslice()[5] == 0x02);     // Byte count
            REQUIRE(buffer.as_wslice()[6] == 0b11001010);  // First 8 coils
            REQUIRE(buffer.as_wslice()[7] == 0b00000101);  // Last 3 coils
        }
    }

    SECTION("When empty request, then is invalid")
    {
        WriteMultipleCoilsRequest empty_request{starting_address, {}};
        WriteMultipleCoilsRequestImpl emtpy_request_impl{empty_request};

        REQUIRE(emtpy_request_impl.is_valid() == false);
    }

    SECTION("When request too large, then is invalid")
    {
        std::vector<bool> values;
        for(unsigned int i = 0; i < 1969; ++i)
        {
            values.emplace_back(i % 2 != 0);
        }
        WriteMultipleCoilsRequest large_request{starting_address, values};
        WriteMultipleCoilsRequestImpl large_request_impl{large_request};

        REQUIRE(large_request_impl.is_valid() == false);
    }

    SECTION("Parse")
    {
        SECTION("When proper request, then parse it properly") {
            std::array<uint8_t, 8> proper_request{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x0C, // Quantity of outputs (12)
                0x02,       // Byte count
                0b01010101, 0b11110101, // Outputs value
            }};
            ser4cpp::rseq_t buffer{proper_request.data(), static_cast<uint32_t>(proper_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.is_valid() == true);

            auto response = result.get();
            REQUIRE(response.starting_address == 0x1234);

            auto values = response.values;
            REQUIRE(values.size() == 12);
            REQUIRE(values[0] == true);
            REQUIRE(values[1] == false);
            REQUIRE(values[2] == true);
            REQUIRE(values[3] == false);
            REQUIRE(values[4] == true);
            REQUIRE(values[5] == false);
            REQUIRE(values[6] == true);
            REQUIRE(values[7] == false);
            REQUIRE(values[8] == true);
            REQUIRE(values[9] == false);
            REQUIRE(values[10] == true);
            REQUIRE(values[11] == false);
        }

        SECTION("When proper request with quantity of outputs multiple of 8, then parse it properly") {
            std::array<uint8_t, 8> proper_request{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x10, // Quantity of outputs (16)
                0x02,       // Byte count
                0b01010101, 0b11110101, // Outputs value
            }};
            ser4cpp::rseq_t buffer{proper_request.data(), static_cast<uint32_t>(proper_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.is_valid() == true);

            auto response = result.get();
            REQUIRE(response.starting_address == 0x1234);

            auto values = response.values;
            REQUIRE(values.size() == 16);
            REQUIRE(values[0] == true);
            REQUIRE(values[1] == false);
            REQUIRE(values[2] == true);
            REQUIRE(values[3] == false);
            REQUIRE(values[4] == true);
            REQUIRE(values[5] == false);
            REQUIRE(values[6] == true);
            REQUIRE(values[7] == false);
            REQUIRE(values[8] == true);
            REQUIRE(values[9] == false);
            REQUIRE(values[10] == true);
            REQUIRE(values[11] == false);
            REQUIRE(values[12] == true);
            REQUIRE(values[13] == true);
            REQUIRE(values[14] == true);
            REQUIRE(values[15] == true);
        }

        SECTION("When mismatch between quantity of outputs and byte count, then parse return Modbus exception 0x03") {
            // Note: the spec is not really clear if the exception 0x03 should be thrown if the byte count
            // **value** does not match the quantity of outputs, or if it should be thrown when the actual number
            // of bytes doesn't fit.

            std::array<uint8_t, 10> mismatch_request{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x1F, // Quantity of outputs (31)
                0x02,       // Byte count (2, should be 4)
                0x42, 0x42,
                0x42, 0x42  // Output values
            }};
            ser4cpp::rseq_t buffer{mismatch_request.data(), static_cast<uint32_t>(mismatch_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataValue);
        }

        SECTION("When mismatch between byte count and actual byte count, then parse report exception") {
            std::array<uint8_t, 10> mismatch_request{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x0C, // Quantity of outputs (12)
                0x02,       // Byte count
                0x42, 0x42,
                0x42, 0x42  // Output values (should only be 2 bytes)
            }};
            ser4cpp::rseq_t buffer{mismatch_request.data(), static_cast<uint32_t>(mismatch_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When quantity of outputs is 0, then parse return Modbus exception 0x03") {
            std::array<uint8_t, 6> quantity_of_outputs_0_request{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x00, // Invalid quantity of outputs (0)
                0x00,       // Byte count (0)
            }};
            ser4cpp::rseq_t buffer{quantity_of_outputs_0_request.data(), static_cast<uint32_t>(quantity_of_outputs_0_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataValue);
        }

        SECTION("When quantity of outputs is greater than maximum, then parse return Modbus exception 0x03") {
            std::array<uint8_t, 253> quantity_of_output_max_plus_one_request{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x07, 0xB1, // Invalid quantity of outputs (1969)
                0xF7,       // Byte count (247)
            }};
            ser4cpp::rseq_t buffer{quantity_of_output_max_plus_one_request.data(), static_cast<uint32_t>(quantity_of_output_max_plus_one_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataValue);
        }

        SECTION("When invalid function code, then parse report exception") {
            std::array<uint8_t, 8> invalid_function_code_request{{
                0x42,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x0C, // Quantity of outputs (12)
                0x02,       // Byte count
                0b01010101, 0b11110101, // Outputs value
            }};
            ser4cpp::rseq_t buffer{invalid_function_code_request.data(), static_cast<uint32_t>(invalid_function_code_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }

        SECTION("When request is too short, then parse report exception") {
            std::array<uint8_t, 5> too_short_request{{
                0x0F,       // Function code
                0x12, 0x34, // Starting address
                0x00, 0x0C, // Quantity of outputs (12)
            }};
            ser4cpp::rseq_t buffer{too_short_request.data(), static_cast<uint32_t>(too_short_request.size())};

            auto result = WriteMultipleCoilsRequestImpl::parse(buffer);

            REQUIRE(result.has_exception<MalformedModbusRequestException>() == true);
        }
    }
}
