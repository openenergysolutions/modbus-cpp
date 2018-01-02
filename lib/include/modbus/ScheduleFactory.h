#ifndef MODBUS_SCHEDULEFACTORY_H
#define MODBUS_SCHEDULEFACTORY_H

#include <memory>

#include "openpal/executor/Typedefs.h"

namespace modbus
{

class ISchedule;

class ScheduleFactory
{
public:
    static std::unique_ptr<ISchedule> CreatePeriodicSchedule(const openpal::duration_t& delay);
};

} // namespace modbus

#endif //MODBUS_SCHEDULEFACTORY_H
