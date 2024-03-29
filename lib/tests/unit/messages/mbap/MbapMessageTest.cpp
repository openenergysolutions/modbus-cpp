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
#include "messages/mbap/MbapMessage.h"
#include "mocks/IMessageMock.h"

using namespace modbus;

TEST_CASE("MbapMessage")
{
    UnitIdentifier unit_id{0x42};
    TransactionIdentifier transaction_id{0x1234};
    IMessageMock request{2, 0x76};
    std::array<uint8_t, 9> raw_request_data{{
            0x12, 0x34, // Transaction Identifier
            0x00, 0x00, // Protocol Identifier
            0x00, 0x03, // Length
            0x42,       // Unit identifier
            0x76, 0x76  // Data
    }};
    ser4cpp::rseq_t raw_request{raw_request_data.data(), static_cast<uint32_t>(raw_request_data.size())};

    SECTION("When constructor, then set the values appropriately")
    {
        MbapMessage message{unit_id, transaction_id, raw_request};

        REQUIRE(message.unit_id == unit_id);
        REQUIRE(message.transaction_id == transaction_id);
        REQUIRE(message.data.equals(raw_request));
    }

    SECTION("When build message, then appropriately build the message")
    {
        ser4cpp::Buffer buffer{260};
        auto write_buffer = buffer.as_wslice();
        auto result = MbapMessage::build_message(unit_id, transaction_id, request, write_buffer);

        REQUIRE(result.equals(raw_request));
        REQUIRE(buffer.as_rslice().take(result.length()).equals(raw_request));
    }

    SECTION("When build message of size 0, then appropriately build the message")
    {
        ser4cpp::Buffer buffer{260};
        IMessageMock empty_request{0, 0x42};
        auto write_buffer = buffer.as_wslice();
        auto result = MbapMessage::build_message(unit_id, transaction_id, empty_request, write_buffer);

        std::array<uint8_t, 7> expected_message_data{{
                0x12, 0x34, // Transaction Identifier
                0x00, 0x00, // Protocol Identifier
                0x00, 0x01, // Length
                0x42,       // Unit identifier
        }};
        ser4cpp::rseq_t expected_message{expected_message_data.data(), static_cast<uint32_t>(expected_message_data.size())};
        REQUIRE(result.equals(expected_message));
    }

    SECTION("When build message of size greater than the limit, then stop writing after the limit")
    {
        ser4cpp::Buffer buffer{260};
        IMessageMock big_request{400, 0x42};
        auto write_buffer = buffer.as_wslice();
        auto result = MbapMessage::build_message(unit_id, transaction_id, big_request, write_buffer);

        REQUIRE(result.length() == 260);
        REQUIRE(result[4] == 0x00);
        REQUIRE(result[5] == 0xFE);
    }
}