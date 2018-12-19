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

#include "exe4cpp/asio/ThreadPool.h"
#include "exe4cpp/asio/StrandExecutor.h"

#include "modbus/logging/LoggerFactory.h"
#include "mocks/IServerConnectionListenerBuilderMock.h"
#include "server/channel/AsioServer.h"

using namespace modbus;

TEST_CASE("AsioServer")
{
    constexpr unsigned short test_port = 8888;
    constexpr unsigned int max_connections = 10;

    auto connection_listener_builder = std::make_shared<IServerConnectionListenerBuilderMock>();

    auto io_service = std::make_shared<asio::io_service>();
    exe4cpp::ThreadPool thread_pool{io_service, 1};
    auto executor = std::make_shared<exe4cpp::StrandExecutor>(io_service);

    auto logger = LoggerFactory::create_null_logger("test");
    const Ipv4Endpoint endpoint{"0.0.0.0", test_port};

    auto server = std::make_shared<AsioServer>(logger, executor, endpoint, max_connections);
    server->start(connection_listener_builder);
    server->shutdown();
}
