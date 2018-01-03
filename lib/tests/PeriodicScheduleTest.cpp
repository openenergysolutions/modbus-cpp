#include "catch.hpp"

#include "PeriodicSchedule.h"

using namespace modbus;

TEST_CASE("PeriodicSchedule")
{
    constexpr auto Now = openpal::steady_time_t{std::chrono::milliseconds(500)};
    constexpr auto Delay = std::chrono::milliseconds(100);

    PeriodicSchedule schedule{Delay};

    SECTION("On reset, then next execution is scheduled after delay")
    {
        schedule.reset(Now);

        auto test = std::chrono::steady_clock::time_point{Now + Delay};
        REQUIRE(schedule.get_next_execution() == openpal::steady_time_t{Now + Delay});
    }

    SECTION("On success, then next execution is scheduled after delay")
    {
        schedule.on_success(Now);

        REQUIRE(schedule.get_next_execution() == openpal::steady_time_t{Now + Delay});
    }

    SECTION("On timeout, then next execution is scheduled after delay")
    {
        schedule.on_timeout(Now);

        REQUIRE(schedule.get_next_execution() == openpal::steady_time_t{Now + Delay});
    }

    SECTION("On failure, then next execution is scheduled after delay")
    {
        schedule.on_failure(Now);

        REQUIRE(schedule.get_next_execution() == openpal::steady_time_t{Now + Delay});
    }
}
