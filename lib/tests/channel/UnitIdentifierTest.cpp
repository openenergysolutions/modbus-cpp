#include "catch.hpp"

#include <sstream>
#include "modbus/channel/UnitIdentifier.h"

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

    SECTION("Printing writes proper hexadecimal format")
    {
        SECTION("Leading zeros are added")
        {
            UnitIdentifier unit_id_zero{0x00};
            std::ostringstream oss;
            oss << unit_id_zero;

            REQUIRE(oss.str() == "0x00");
        }


        SECTION("Letters are capitalized")
        {
            UnitIdentifier unit_id_four_c{0x4C};
            std::ostringstream oss;
            oss << unit_id_four_c;

            REQUIRE(oss.str() == "0x4C");
        }
    }
}
