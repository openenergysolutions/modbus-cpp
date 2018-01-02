#ifndef MODBUS_PERIODICTIMER_H
#define MODBUS_PERIODICTIMER_H

#include "modbus/ISchedule.h"

namespace modbus
{

class PeriodicSchedule : public ISchedule
{
public:
    explicit PeriodicSchedule(const openpal::duration_t& delay);

    void Reset(const openpal::steady_time_t& now) override;
    void OnSuccess(const openpal::steady_time_t& now) override;
    void OnTimeout(const openpal::steady_time_t& now) override;
    void OnFailure(const openpal::steady_time_t& now) override;
    openpal::steady_time_t GetNextExecution() const override;

private:
    openpal::steady_time_t m_nextExecution;
    openpal::duration_t m_delay;
};

} // namespace modbus

#endif //MODBUS_PERIODICTIMER_H
