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

#include "server/channel/ServerConnectionListener.h"
#include "mocks/IServerChannelImplMock.h"
#include "mocks/TcpConnectionMock.h"

using namespace modbus;

TEST_CASE("ServerConnectionListener")
{
    auto channel = std::make_shared<IServerChannelImplMock>();
    auto connection = std::make_shared<TcpConnectionMock>();
    ServerConnectionListener connection_listener{channel, connection};

    SECTION("On receive, then parse the MBAP and sends the message to the channel")
    {
        std::array<uint8_t, 9> complete_mbap_message{{
            0x12, 0x34, // Transaction Identifier
            0x00, 0x00, // Protocol Identifier
            0x00, 0x03, // Length
            0x42,       // Unit identifier
            0x98, 0x76  // Data
        }};
        ser4cpp::rseq_t buffer{complete_mbap_message.data(), complete_mbap_message.size()};

        connection_listener.on_receive(buffer);
        
        REQUIRE(channel->get_num_messages_received() == 1);
        REQUIRE(channel->get_last_message_received().transaction_id == TransactionIdentifier{0x1234});
    }
}
