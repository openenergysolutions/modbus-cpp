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
#include "messages/ReadCoilsResponseImpl.h"
#include "messages/ReadCoilsRequestImpl.h"

using namespace modbus;

ReadCoilsResponse generate_response(size_t num_bits)
{
    ReadCoilsResponse response{};
    for(size_t i = 0; i < num_bits; ++i)
    {
        SingleBitValue value{};
        value.address = static_cast<Address>(i);
        value.value = i % 2 == 0;
        response.values.push_back(value);
    }
    return response;
}

TEST_CASE("ReadCoilsResponseImpl")
{
    SECTION("Request with number of bits not a multiple of 8")
    {
        const size_t num_bits = 17;
        auto response = generate_response(num_bits);
        ReadCoilsResponseImpl response_impl{response};

        REQUIRE(response_impl.is_valid() == true);
        REQUIRE(response_impl.get_message_length() == 5);

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) response_impl.get_message_length()};
            auto slice = buffer.as_wslice();

            response_impl.build_message(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x01); // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x03); // Byte count
            REQUIRE(buffer.as_wslice()[2] == 0b01010101); // Output status 7-0
            REQUIRE(buffer.as_wslice()[3] == 0b01010101); // Output status 15-8
            REQUIRE(buffer.as_wslice()[4] == 0b00000001); // Output status 16
        }

        SECTION("When clone, then effectively creates a copy of the request")
        {
            auto copy = response_impl.clone();
            auto other_response = static_cast<ReadCoilsResponseImpl*>(copy.get());

            REQUIRE(&response_impl != other_response);
        }
    }

    SECTION("Request with number of bits multiple of 8")
    {
        const size_t num_bits = 24;
        auto response = generate_response(num_bits);
        ReadCoilsResponseImpl response_impl{response};

        REQUIRE(response_impl.is_valid() == true);
        REQUIRE(response_impl.get_message_length() == 5);

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) response_impl.get_message_length()};
            auto slice = buffer.as_wslice();

            response_impl.build_message(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x01); // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x03); // Byte count
            REQUIRE(buffer.as_wslice()[2] == 0b01010101); // Output status 7-0
            REQUIRE(buffer.as_wslice()[3] == 0b01010101); // Output status 15-8
            REQUIRE(buffer.as_wslice()[4] == 0b01010101); // Output status 23-16
        }
    }

    SECTION("Parse")
    {
        const uint16_t starting_address = 0x0016;
        const uint16_t qty_of_bits = 16;
        ReadCoilsRequest request{starting_address, qty_of_bits};
        ReadCoilsRequestImpl request_impl{request};

        SECTION("When proper response, then parse it properly") {
            std::array<uint8_t, 4> proper_response{{
                0x01,       // Function code
                0x02,       // Number of bytes
                0b01010101, 0b11110000, // Coil values
            }};
            ser4cpp::rseq_t buffer{proper_response.data(), static_cast<uint32_t>(proper_response.size())};

            auto result = ReadCoilsResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.is_valid() == true);
            auto response = result.get();
            auto values = response.values;
            REQUIRE(values.size() == 16);
            REQUIRE(values[0].address == 0x0016);
            REQUIRE(values[0].value == true);
            REQUIRE(values[1].address == 0x0017);
            REQUIRE(values[1].value == false);
            REQUIRE(values[2].address == 0x0018);
            REQUIRE(values[2].value == true);
            REQUIRE(values[3].address == 0x0019);
            REQUIRE(values[3].value == false);

            REQUIRE(values[8].address == 0x001E);
            REQUIRE(values[8].value == false);
            REQUIRE(values[15].address == 0x0025);
            REQUIRE(values[15].value == true);
        }

        SECTION("When proper response for request with number of bits not a multiple of 8, then parse it properly")
        {
            const uint16_t qty_of_bits_not_multiple_of_8 = 17;

            ReadCoilsRequest request_not_multiple_of_8{starting_address, qty_of_bits_not_multiple_of_8};
            ReadCoilsRequestImpl request_impl_not_multiple_of_8{request_not_multiple_of_8};
            std::array<uint8_t, 5> proper_response{{
                0x01,       // Function code
                0x03,       // Number of bytes
                0b01010101, 0b11110000, 0b00000001 // Coil values
            }};
            ser4cpp::rseq_t buffer{proper_response.data(), static_cast<uint32_t>(proper_response.size())};

            auto result = ReadCoilsResponseImpl::parse(request_impl_not_multiple_of_8, buffer);

            REQUIRE(result.is_valid() == true);
            auto response = result.get();
            auto values = response.values;
            REQUIRE(values.size() == 17);
            REQUIRE(values[0].address == 0x0016);
            REQUIRE(values[0].value == true);
            REQUIRE(values[1].address == 0x0017);
            REQUIRE(values[1].value == false);
            REQUIRE(values[2].address == 0x0018);
            REQUIRE(values[2].value == true);
            REQUIRE(values[3].address == 0x0019);
            REQUIRE(values[3].value == false);

            REQUIRE(values[8].address == 0x001E);
            REQUIRE(values[8].value == false);
            REQUIRE(values[15].address == 0x0025);
            REQUIRE(values[15].value == true);

            REQUIRE(values[16].address == 0x0026);
            REQUIRE(values[16].value == true);
        }

        SECTION("When exception response, then parse report exception")
        {
            std::array<uint8_t, 2> exception_response{{
                0x81, // Exception function code
                0x02  // Illegal data address
            }};
            ser4cpp::rseq_t buffer{exception_response.data(), static_cast<uint32_t>(exception_response.size())};

            auto result = ReadCoilsResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.has_exception<ModbusException>() == true);
            REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
        }

        SECTION("When size not present, then return malformed exception")
        {
            std::array<uint8_t, 1> wrong_size_response{ {
                0x01 // Function code
            }};
            ser4cpp::rseq_t buffer{wrong_size_response.data(), static_cast<uint32_t>(wrong_size_response.size())};

            auto result = ReadCoilsResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
        }

        SECTION("When wrong size response, then return malformed exception")
        {
            std::array<uint8_t, 3> wrong_size_response{{
                0x01, // Function code
                0x02, // Length should be 2
                0x42  // Only 1 byte is received
            }};
            ser4cpp::rseq_t buffer{wrong_size_response.data(), static_cast<uint32_t>(wrong_size_response.size())};

            auto result = ReadCoilsResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
        }

        SECTION("When size does not fit with the expectation, then return malformed exception")
        {
            std::array<uint8_t, 5> wrong_size_response{ {
                0x01,            // Function code
                0x03,            // Size should be 2, not 3
                0x42, 0x43, 0x44 // Data
            }};
            ser4cpp::rseq_t buffer{wrong_size_response.data(), static_cast<uint32_t>(wrong_size_response.size())};

            auto result = ReadCoilsResponseImpl::parse(request_impl, buffer);

            REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
        }
    }
}
