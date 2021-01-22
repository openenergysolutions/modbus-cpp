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
#include <algorithm>
#include "messages/mbap/MbapParser.h"
#include "mocks/MbapSinkMock.h"

using namespace modbus;

void check_complete_message(const MbapMessage& msg)
{
    REQUIRE(msg.unit_id == 0x42);
    REQUIRE(msg.transaction_id == TransactionIdentifier{0x1234});
    REQUIRE(msg.data.length() == 2);
    REQUIRE(msg.data[0] == 0x98);
    REQUIRE(msg.data[1] == 0x76);
}

TEST_CASE("MbapParser")
{
    MbapSinkMock sink;
    MbapParser parser{&sink};

    std::array<uint8_t, 9> complete_message{{
            0x12, 0x34, // Transaction Identifier
            0x00, 0x00, // Protocol Identifier
            0x00, 0x03, // Length
            0x42,       // Unit identifier
            0x98, 0x76  // Data
    }};

    SECTION("When receive complete message, then report received message")
    {
        ser4cpp::rseq_t buffer{complete_message.data(), static_cast<uint32_t>(complete_message.size())};

        parser.parse(buffer);

        REQUIRE(sink.get_num_messages() == 1);
        check_complete_message(sink.get_messages()[0]);
    }

    SECTION("When receive message one byte at a time, then report received message")
    {
        for(auto b : complete_message)
        {
            ser4cpp::rseq_t buffer{&b, 1};
            parser.parse(buffer);
        }

        REQUIRE(sink.get_num_messages() == 1);
        check_complete_message(sink.get_messages()[0]);
    }

    SECTION("When receive two complete messages in one call, then report both messages")
    {
        std::array<uint8_t, 18> two_messages;
        auto end = std::copy(complete_message.begin(), complete_message.end(), two_messages.begin());
        std::copy(complete_message.begin(), complete_message.end(), end);
        ser4cpp::rseq_t buffer{two_messages.data(), static_cast<uint32_t>(two_messages.size())};

        parser.parse(buffer);

        REQUIRE(sink.get_num_messages() == 2);
        check_complete_message(sink.get_messages()[0]);
        check_complete_message(sink.get_messages()[1]);
    }

    SECTION("When receive incomplete message, then don't report received message")
    {
        ser4cpp::rseq_t buffer{complete_message.data(), static_cast<uint32_t>(complete_message.size() - 1)};

        parser.parse(buffer);

        REQUIRE(sink.get_num_messages() == 0);

        SECTION("When receiving the rest, then report message")
        {
            ser4cpp::rseq_t buffer{complete_message.data() + complete_message.size() - 1, 1};

            parser.parse(buffer);

            REQUIRE(sink.get_num_messages() == 1);
            check_complete_message(sink.get_messages()[0]);
        }
    }

    SECTION("When reset, then stop parsing and reset internal state")
    {
        std::array<uint8_t, 6> incomplete_message{{
                0x45, 0x67, // Transaction Identifier
                0x00, 0x00, // Protocol Identifier
                0x00, 0x03, // Length
        }};
        ser4cpp::rseq_t buffer{incomplete_message.data(), static_cast<uint32_t>(incomplete_message.size())};
        parser.parse(buffer);

        parser.reset();

        buffer = ser4cpp::rseq_t{complete_message.data(), static_cast<uint32_t>(complete_message.size())};
        parser.parse(buffer);
        REQUIRE(sink.get_num_messages() == 1);
        check_complete_message(sink.get_messages()[0]);
    }

    SECTION("When receive wrong protocol identifier, then reject message")
    {
        std::array<uint8_t, 9> wrong_protocol_id_message{{
                0x12, 0x34, // Transaction Identifier
                0x42, 0x42, // Protocol Identifier
                0x00, 0x03, // Length
                0x42,       // Unit identifier
                0x98, 0x76  // Data
        }};
        ser4cpp::rseq_t buffer{wrong_protocol_id_message.data(), static_cast<uint32_t>(wrong_protocol_id_message.size())};

        parser.parse(buffer);

        REQUIRE(sink.get_num_messages() == 0);

        SECTION("Then keep parsing as usual")
        {
            buffer = ser4cpp::rseq_t{complete_message.data(), static_cast<uint32_t>(complete_message.size())};

            parser.parse(buffer);

            REQUIRE(sink.get_num_messages() == 1);
            check_complete_message(sink.get_messages()[0]);
        }
    }

    SECTION("When length is 0, then reject message")
    {
        std::array<uint8_t, 7> length_zero_message{{
                0x12, 0x34, // Transaction Identifier
                0x42, 0x42, // Protocol Identifier
                0x00, 0x00, // Length
                0x42,       // Unit identifier
        }};
        ser4cpp::rseq_t buffer{length_zero_message.data(), static_cast<uint32_t>(length_zero_message.size())};

        parser.parse(buffer);

        REQUIRE(sink.get_num_messages() == 0);

        SECTION("Then keep parsing as usual")
        {
            buffer = ser4cpp::rseq_t{complete_message.data(), static_cast<uint32_t>(complete_message.size())};

            parser.parse(buffer);

            REQUIRE(sink.get_num_messages() == 1);
            check_complete_message(sink.get_messages()[0]);
        }
    }

    SECTION("When length is greater than the maximum, then go to end of stream, but only report maximum size")
    {
        std::array<uint8_t, 65541> length_max_message{{
                0x12, 0x34, // Transaction Identifier
                0x00, 0x00, // Protocol Identifier
                0xFF, 0xFF, // Length
                0x42,       // Unit identifier
                0x76        // Data (length = 65535 bytes)
        }};
        ser4cpp::rseq_t buffer{length_max_message.data(), static_cast<uint32_t>(length_max_message.size())};

        parser.parse(buffer);

        REQUIRE(sink.get_num_messages() == 1);
        auto msg = sink.get_messages()[0];
        REQUIRE(msg.data.length() == 253);

        SECTION("Then keep parsing as usual")
        {
            buffer = ser4cpp::rseq_t{complete_message.data(), static_cast<uint32_t>(complete_message.size())};

            parser.parse(buffer);

            REQUIRE(sink.get_num_messages() == 2);
            check_complete_message(sink.get_messages()[1]);
        }
    }
}
