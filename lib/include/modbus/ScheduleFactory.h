#ifndef MODBUS_SCHEDULEFACTORY_H
#define MODBUS_SCHEDULEFACTORY_H

#include <memory>

#include "openpal/executor/TimeDuration.h"

namespace modbus
{

class ISchedule;

class ScheduleFactory
{
public:
    static std::unique_ptr<ISchedule> CreatePeriodicSchedule(const openpal::TimeDuration& delay);
};

} // namespace modbus

#endif //MODBUS_SCHEDULEFACTORY_H
