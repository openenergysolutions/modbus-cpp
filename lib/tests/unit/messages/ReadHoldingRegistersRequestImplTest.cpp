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

#include "ser4cpp/container/Buffer.h"
#include "messages/ReadHoldingRegistersRequestImpl.h"

using namespace modbus;

TEST_CASE("ReadHoldingRegistersRequestImpl")
{
    SECTION("Valid request")
    {
        const uint16_t starting_address = 0x1234;
        const uint16_t qty_of_registers = 4;
        ReadHoldingRegistersRequest request{starting_address, qty_of_registers};
        ReadHoldingRegistersRequestImpl request_impl(request);

        REQUIRE(request_impl.is_valid() == true);
        REQUIRE(request_impl.get_request_length() == 5);

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_request_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_request(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x03); // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12); // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34); // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00); // Qty of registers MSB
            REQUIRE(buffer.as_wslice()[4] == 0x04); // Qty of registers LSB
        }

        SECTION("When clone, then effectively creates a copy of the request")
        {
            auto copy = request_impl.clone();
            auto other_request = static_cast<ReadHoldingRegistersRequestImpl*>(copy.get());

            REQUIRE(other_request->get_request().starting_address == starting_address);
            REQUIRE(other_request->get_request().qty_of_registers == qty_of_registers);
            REQUIRE(&other_request->get_request() != &request);
        }
    }

    SECTION("When quantity is 0, then request is not valid")
    {
        const uint16_t starting_address = 0x1234;
        ReadHoldingRegistersRequest request{starting_address, 0};
        ReadHoldingRegistersRequestImpl request_impl(request);

        REQUIRE(request_impl.is_valid() == false);
    }

    SECTION("When quantity is higher than 125, then request is not valid")
    {
        const uint16_t starting_address = 0x1234;
        ReadHoldingRegistersRequest request{starting_address, 126};
        ReadHoldingRegistersRequestImpl request_impl(request);

        REQUIRE(request_impl.is_valid() == false);
    }
}
