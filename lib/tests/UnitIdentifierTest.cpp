#include "catch.hpp"

#include "modbus/UnitIdentifier.h"

using namespace modbus;

TEST_CASE("UnitIdentifier")
{
    SECTION("Constructor initialize with the right value")
    {
        UnitIdentifier unitIdentifier{0x24};

        REQUIRE(unitIdentifier.GetValue() == 0x24);
    }

    SECTION("Equality operator checks if value is equal")
    {
        UnitIdentifier firstUnitIdentifier{0x24};
        UnitIdentifier secondUnitIdentifier{0x42};

        REQUIRE(firstUnitIdentifier == firstUnitIdentifier);
        REQUIRE(firstUnitIdentifier != secondUnitIdentifier);
    }

    SECTION("Default unit identifier is 0xFF")
    {
        auto defaultUnitIdentifier = UnitIdentifier::Default();
        REQUIRE(defaultUnitIdentifier == UnitIdentifier(0xFF));
    }
}
