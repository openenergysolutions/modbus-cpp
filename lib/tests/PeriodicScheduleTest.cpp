#include "catch.hpp"

#include "PeriodicSchedule.h"

using namespace modbus;

TEST_CASE("PeriodicSchedule")
{
    constexpr uint64_t NOW = 1000;
    constexpr uint64_t DELAY = 100;

    PeriodicSchedule schedule{openpal::TimeDuration::milliseconds(DELAY)};

    SECTION("On reset, then next execution is scheduled after delay")
    {
        schedule.Reset(openpal::Timestamp{NOW});

        REQUIRE(schedule.GetNextExecution() == openpal::Timestamp{NOW + DELAY});
    }

    SECTION("On success, then next execution is scheduled after delay")
    {
        schedule.OnSuccess(openpal::Timestamp{NOW});

        REQUIRE(schedule.GetNextExecution() == openpal::Timestamp{NOW + DELAY});
    }

    SECTION("On timeout, then next execution is scheduled after delay")
    {
        schedule.OnTimeout(openpal::Timestamp{NOW});

        REQUIRE(schedule.GetNextExecution() == openpal::Timestamp{NOW + DELAY});
    }

    SECTION("On failure, then next execution is scheduled after delay")
    {
        schedule.OnFailure(openpal::Timestamp{NOW});

        REQUIRE(schedule.GetNextExecution() == openpal::Timestamp{NOW + DELAY});
    }
}
