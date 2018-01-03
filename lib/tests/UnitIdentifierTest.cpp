#include "catch.hpp"

#include "modbus/UnitIdentifier.h"

using namespace modbus;

TEST_CASE("UnitIdentifier")
{
    SECTION("Constructor initialize with the right value")
    {
        UnitIdentifier unit_identifier{0x24};

        REQUIRE(unit_identifier.get_value() == 0x24);
    }

    SECTION("Equality operator checks if value is equal")
    {
        UnitIdentifier first_unit_identifier{0x24};
        UnitIdentifier second_unit_identifier{0x42};

        REQUIRE(first_unit_identifier == first_unit_identifier);
        REQUIRE(first_unit_identifier != second_unit_identifier);
    }

    SECTION("Default unit identifier is 0xFF")
    {
        auto default_unit_identifier = UnitIdentifier::default_unit_identifier();
        REQUIRE(default_unit_identifier == UnitIdentifier(0xFF));
    }
}
