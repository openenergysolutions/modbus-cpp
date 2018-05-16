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

#include <memory>
#include "ser4cpp/container/Buffer.h"
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
            auto length = request_impl.get_request_length();

            REQUIRE(length == 6 + 6);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_request_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_request(slice);

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
}
