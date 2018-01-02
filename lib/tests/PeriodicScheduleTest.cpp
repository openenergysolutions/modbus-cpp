#include "catch.hpp"

#include "PeriodicSchedule.h"

using namespace modbus;

TEST_CASE("PeriodicSchedule")
{
    constexpr auto NOW = openpal::steady_time_t{std::chrono::milliseconds(500)};
    constexpr auto DELAY = std::chrono::milliseconds(100);

    PeriodicSchedule schedule{DELAY};

    SECTION("On reset, then next execution is scheduled after delay")
    {
        schedule.Reset(NOW);

        auto test = std::chrono::steady_clock::time_point{NOW + DELAY};
        REQUIRE(schedule.GetNextExecution() == openpal::steady_time_t{NOW + DELAY});
    }

    SECTION("On success, then next execution is scheduled after delay")
    {
        schedule.OnSuccess(NOW);

        REQUIRE(schedule.GetNextExecution() == openpal::steady_time_t{NOW + DELAY});
    }

    SECTION("On timeout, then next execution is scheduled after delay")
    {
        schedule.OnTimeout(NOW);

        REQUIRE(schedule.GetNextExecution() == openpal::steady_time_t{NOW + DELAY});
    }

    SECTION("On failure, then next execution is scheduled after delay")
    {
        schedule.OnFailure(NOW);

        REQUIRE(schedule.GetNextExecution() == openpal::steady_time_t{NOW + DELAY});
    }
}
