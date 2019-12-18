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

#include <sstream>
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

    SECTION("std::hash returns same value when unit identifiers are equal")
    {
        UnitIdentifier first_unit_identifier{0x42};
        UnitIdentifier second_unit_identifier{0x42};

        REQUIRE(std::hash<UnitIdentifier>()(first_unit_identifier) == std::hash<UnitIdentifier>()(second_unit_identifier));
    }

    SECTION("std::hash returns different values when unit identifiers are different")
    {
        UnitIdentifier first_unit_identifier{0x22};
        UnitIdentifier second_unit_identifier{0x42};

        REQUIRE(std::hash<UnitIdentifier>()(first_unit_identifier) != std::hash<UnitIdentifier>()(second_unit_identifier));
    }
}
