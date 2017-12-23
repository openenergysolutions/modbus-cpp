#include "catch.hpp"

#include "PrivateHeader.h"

using namespace modbus;

TEST_CASE("The test always succeeds")
{
    SECTION("Simple assert")
    {
        REQUIRE(1 == 1);
    }

    SECTION("Another assert")
    {
        REQUIRE(2 == 2);
    }
}

TEST_CASE("The test now succeeds")
{
    REQUIRE(0 == 0);
}

TEST_CASE("Private headers")
{
    PrivateClass test;
    REQUIRE(test.test() == 0);
}
