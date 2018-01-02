#include "modbus/ScheduleFactory.h"

#include "PeriodicSchedule.h"

namespace modbus
{

std::unique_ptr<ISchedule> ScheduleFactory::CreatePeriodicSchedule(const openpal::duration_t& delay)
{
    return std::make_unique<PeriodicSchedule>(delay);
}

} // namespace modbus
