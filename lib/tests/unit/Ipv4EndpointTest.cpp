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

#include "modbus/Ipv4Endpoint.h"

using namespace modbus;

TEST_CASE("Ipv4Endpoint")
{
    SECTION("Constructor initialize with the right values")
    {
        Ipv4Endpoint endpoint{"hostname", 502};

        REQUIRE(endpoint.get_hostname() == "hostname");
        REQUIRE(endpoint.get_port() == 502);
    }

    SECTION("Equality operator checks if all values are equal")
    {
        Ipv4Endpoint default_endpoint{"hostname", 502};
        Ipv4Endpoint identical_endpoint{"hostname", 502};
        Ipv4Endpoint endpoint_with_different_hostname{"asdf", 502};
        Ipv4Endpoint endpoint_with_different_port{"hostname", 500};

        REQUIRE(default_endpoint == identical_endpoint);
        REQUIRE(default_endpoint != endpoint_with_different_hostname);
        REQUIRE(default_endpoint != endpoint_with_different_port);
    }
}