#ifndef MODBUS_ISCHEDULE_H
#define MODBUS_ISCHEDULE_H

#include <memory>

#include "openpal/executor/TimeDuration.h"
#include "openpal/executor/Timestamp.h"

namespace modbus
{

class ISchedule
{
public:
    virtual ~ISchedule() = default;

    virtual void Reset(const openpal::Timestamp& now) = 0;
    virtual void OnSuccess(const openpal::Timestamp& now) = 0;
    virtual void OnTimeout(const openpal::Timestamp& now) = 0;
    virtual void OnFailure(const openpal::Timestamp& now) = 0;
    virtual openpal::Timestamp GetNextExecution() const = 0;
};

} // namespace modbus

#endif //MODBUS_ISCHEDULE_H
