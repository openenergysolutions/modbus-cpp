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

#include <memory>
#include <vector>

#include "exe4cpp/asio/ThreadPool.h"
#include "exe4cpp/asio/StrandExecutor.h"

#include "modbus/logging/LoggerFactory.h"
#include "server/channel/AsioServer.h"
#include "mocks/AsioServerWrapper.h"
#include "mocks/IServerConnectionListenerBuilderMock.h"

using namespace modbus;

TEST_CASE("AsioServer")
{
    constexpr unsigned short test_port = 8888;
    constexpr unsigned int max_connections = 10;

    auto connection_listener_builder = std::make_shared<IServerConnectionListenerBuilderMock>(std::chrono::seconds{5});

    auto io_service = std::make_shared<asio::io_service>();
    exe4cpp::ThreadPool thread_pool{io_service, 1};
    auto executor = std::make_shared<exe4cpp::StrandExecutor>(io_service);

    auto logger = LoggerFactory::create_null_logger("test");
    const Ipv4Endpoint endpoint{"0.0.0.0", test_port};

    SECTION("When server is started")
    {
        AsioServerWrapper server{std::make_shared<AsioServer>(logger, executor, endpoint, max_connections)};
        server->start(connection_listener_builder);

        SECTION("When client establishes connection, then server accepts the connection")
        {
            asio::ip::tcp::socket client{*io_service};
            asio::error_code ec;
            client.connect(asio::ip::tcp::endpoint{asio::ip::address::from_string("127.0.0.1"), test_port}, ec);
            connection_listener_builder->wait_for_connection();

            REQUIRE(!ec);
            REQUIRE(server->get_num_connections() == 1);

            SECTION("When client closes connection, then server reports the lost connection")
            {
                client.close();
                std::this_thread::sleep_for(std::chrono::seconds{1});
                REQUIRE(server->get_num_connections() == 0);
            }
        }

        SECTION("When server has more connections than the limit, then old connections are closed")
        {
            std::vector<asio::ip::tcp::socket> sockets;
            for(size_t i = 0; i < max_connections; ++i)
            {
                sockets.emplace_back(*io_service);
                asio::error_code ec;
                sockets.back().connect(asio::ip::tcp::endpoint{asio::ip::address::from_string("127.0.0.1"), test_port}, ec);
                connection_listener_builder->wait_for_connection();
                REQUIRE(!ec);
            }

            REQUIRE(server->get_num_connections() == max_connections);

            asio::ip::tcp::socket client{*io_service};
            asio::error_code ec;
            client.connect(asio::ip::tcp::endpoint{asio::ip::address::from_string("127.0.0.1"), test_port}, ec);
            connection_listener_builder->wait_for_connection();

            REQUIRE(server->get_num_connections() == max_connections);
        }
    }
}
