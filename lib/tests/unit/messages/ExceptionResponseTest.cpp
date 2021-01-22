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

#include "ser4cpp/container/Buffer.h"
#include "messages/ExceptionResponse.h"

using namespace modbus;

TEST_CASE("ExceptionResponse")
{
    const uint8_t function_code = 0x34;
    const ExceptionType exception_type = ExceptionType::IllegalDataAddress;
    ExceptionResponse response{function_code, exception_type};

    REQUIRE(response.is_valid() == true);
    REQUIRE(response.get_message_length() == 2);

    SECTION("When build request, then write appropriate values to the buffer")
    {
        ser4cpp::Buffer buffer{(uint32_t) response.get_message_length()};
        auto slice = buffer.as_wslice();

        response.build_message(slice);

        REQUIRE(buffer.as_wslice()[0] == 0xB4); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x02); // Exception code
    }

    SECTION("When clone, then effectively creates a copy of the request")
    {
        auto copy = response.clone();
        auto other_response = static_cast<ExceptionResponse*>(copy.get());

        REQUIRE(&response != other_response);
    }
}
