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