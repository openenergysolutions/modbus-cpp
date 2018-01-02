#ifndef MODBUS_ISCHEDULE_H
#define MODBUS_ISCHEDULE_H

#include <memory>

#include "openpal/executor/Typedefs.h"

namespace modbus
{

class ISchedule
{
public:
    virtual ~ISchedule() = default;

    virtual void Reset(const openpal::steady_time_t& now) = 0;
    virtual void OnSuccess(const openpal::steady_time_t& now) = 0;
    virtual void OnTimeout(const openpal::steady_time_t& now) = 0;
    virtual void OnFailure(const openpal::steady_time_t& now) = 0;
    virtual openpal::steady_time_t GetNextExecution() const = 0;
};

} // namespace modbus

#endif //MODBUS_ISCHEDULE_H
