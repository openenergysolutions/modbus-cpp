#include "catch.hpp"

#include "modbus/Ipv4Endpoint.h"

using namespace modbus;

TEST_CASE("Ipv4Endpoint")
{
    SECTION("Constructor initialize with the right values")
    {
        Ipv4Endpoint endpoint{"hostname", 502};

        REQUIRE(endpoint.GetHostname() == "hostname");
        REQUIRE(endpoint.GetPort() == 502);
    }

    SECTION("Equality operator checks if all values are equal")
    {
        Ipv4Endpoint defaultEndpoint{"hostname", 502};
        Ipv4Endpoint identicalEndpoint{"hostname", 502};
        Ipv4Endpoint endpointWithDifferentHostname{"asdf", 502};
        Ipv4Endpoint endpointWithDifferentPort{"hostname", 500};

        REQUIRE(defaultEndpoint == identicalEndpoint);
        REQUIRE(defaultEndpoint != endpointWithDifferentHostname);
        REQUIRE(defaultEndpoint != endpointWithDifferentPort);
    }
}