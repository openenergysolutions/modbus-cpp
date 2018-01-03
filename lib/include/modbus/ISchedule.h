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

    virtual void reset(const openpal::steady_time_t& now) = 0;
    virtual void on_success(const openpal::steady_time_t& now) = 0;
    virtual void on_timeout(const openpal::steady_time_t& now) = 0;
    virtual void on_failure(const openpal::steady_time_t& now) = 0;
    virtual openpal::steady_time_t get_next_execution() const = 0;
};

} // namespace modbus

#endif //MODBUS_ISCHEDULE_H
