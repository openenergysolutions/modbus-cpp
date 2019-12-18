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

#include "modbus/server/db/SingleTypeDatabase.h"

using namespace modbus;

TEST_CASE("SingleTypeDatabase")
{
    SingleTypeDatabase<uint16_t> database{};
    const Address address{0x1234};
    const Address other_address{0x2345};
    const uint16_t value{0x4567};
    const uint16_t other_value{0x9876};

    SECTION("add_value")
    {
        auto add_result = database.add_value(address, value);
        REQUIRE(add_result == true);
        
        uint16_t get_value;
        auto get_result = database.get_value(address, get_value);
        REQUIRE(get_result == true);
        REQUIRE(get_value == value);

        auto add_same_result = database.add_value(address, other_value);
        REQUIRE(add_same_result == false);

        uint16_t same_get_value;
        auto same_get_result = database.get_value(address, same_get_value);
        REQUIRE(same_get_result == true);
        REQUIRE(same_get_value == value);
    }

    SECTION("get_value")
    {
        database.add_value(address, value);
        
        uint16_t get_value;
        auto get_result = database.get_value(address, get_value);
        REQUIRE(get_result == true);
        REQUIRE(get_value == value);

        auto other_get_result = database.get_value(other_address, get_value);
        REQUIRE(other_get_result == false);
    }

    SECTION("get_values")
    {
        const size_t num_values = 10;
        const Address starting_address{0x1234};
        for(size_t i = 0; i < num_values; ++i)
        {
            database.add_value(static_cast<uint16_t>(starting_address + i), static_cast<uint16_t>(i));
        }
        
        std::vector<uint16_t> values(num_values);
        auto get_result = database.get_values(starting_address, values);
        REQUIRE(get_result == true);
        for(size_t i = 0; i < num_values; ++i)
        {
            REQUIRE(values[i] == i);
        }

        get_result = database.get_values(starting_address + 1, values);
        REQUIRE(get_result == false);

        get_result = database.get_values(starting_address - 1, values);
        REQUIRE(get_result == false);

        database.add_value(starting_address + num_values + 1, 0x4242);
        get_result = database.get_values(starting_address + 2, values);
        REQUIRE(get_result == false);
    }

    SECTION("set_value")
    {
        database.add_value(address, value);
        
        auto set_result = database.set_value(address, other_value);
        uint16_t get_value;
        auto get_result = database.get_value(address, get_value);
        REQUIRE(set_result == true);
        REQUIRE(get_result == true);
        REQUIRE(get_value == other_value);

        auto other_set_result = database.set_value(other_address, 0x4242);
        REQUIRE(other_set_result == false);
    }

    SECTION("set_values")
    {
        const size_t num_values = 10;
        const size_t offset = 20;
        const Address starting_address{0x1234};
        for(size_t i = 0; i < num_values; ++i)
        {
            database.add_value(static_cast<uint16_t>(starting_address + i), static_cast<uint16_t>(i));
        }
        
        std::vector<uint16_t> new_values(num_values);
        for(size_t i = 0; i < num_values; ++i)
        {
            new_values[i] = static_cast<uint16_t>(i + offset);
        }
        auto set_result = database.set_values(starting_address, new_values);
        std::vector<uint16_t> get_values(num_values);
        database.get_values(starting_address, get_values);
        REQUIRE(set_result == true);
        for(size_t i = 0; i < num_values; ++i)
        {
            REQUIRE(get_values[i] == new_values[i]);
        }

        set_result = database.set_values(starting_address + 1, new_values);
        REQUIRE(set_result == false);

        set_result = database.get_values(starting_address - 1, new_values);
        REQUIRE(set_result == false);

        database.add_value(starting_address + num_values + 1, 0x4242);
        set_result = database.get_values(starting_address + 2, new_values);
        REQUIRE(set_result == false);
    }

    SECTION("is_valid")
    {
        database.add_value(address, value);
        
        auto result = database.is_valid(address);
        REQUIRE(result == true);

        result = database.is_valid(other_address);
        REQUIRE(result == false);
    }

    SECTION("is_valid_range")
    {
        const size_t num_values = 10;
        const Address starting_address{0x1234};
        for(size_t i = 0; i < num_values; ++i)
        {
            database.add_value(static_cast<uint16_t>(starting_address + i), static_cast<uint16_t>(i));
        }
        
        auto result = database.is_valid_range(starting_address, num_values);
        REQUIRE(result == true);

        result = database.is_valid_range(starting_address + 1, num_values);
        REQUIRE(result == false);

        result = database.is_valid_range(starting_address - 1, num_values);
        REQUIRE(result == false);

        database.add_value(starting_address + num_values + 1, 0x4242);
        result = database.is_valid_range(starting_address + 2, num_values);
        REQUIRE(result == false);
    }
}
