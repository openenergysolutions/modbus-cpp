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

#include "server/channel/ServerConnectionListener.h"
#include "server/channel/ServerConnectionListenerBuilder.h"
#include "mocks/IServerChannelImplMock.h"
#include "mocks/TcpConnectionMock.h"

using namespace modbus;

TEST_CASE("ServerConnectionListenerBuilder")
{
    auto channel = std::make_shared<IServerChannelImplMock>();
    auto connection = std::make_shared<TcpConnectionMock>();
    ServerConnectionListenerBuilder builder{channel};

    SECTION("When build, then return an instance of ServerConnectionListenerBuilder")
    {
        auto result = builder.build(connection);

        REQUIRE(dynamic_cast<ServerConnectionListener*>(result.get()) != nullptr);
    }
}
