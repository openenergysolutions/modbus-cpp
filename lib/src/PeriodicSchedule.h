#ifndef MODBUS_PERIODICTIMER_H
#define MODBUS_PERIODICTIMER_H

#include "modbus/ISchedule.h"

namespace modbus
{

class PeriodicSchedule : public ISchedule
{
public:
    explicit PeriodicSchedule(const openpal::TimeDuration& delay);

    void Reset(const openpal::Timestamp& now) override;
    void OnSuccess(const openpal::Timestamp& now) override;
    void OnTimeout(const openpal::Timestamp& now) override;
    void OnFailure(const openpal::Timestamp& now) override;
    openpal::Timestamp GetNextExecution() const override;

private:
    openpal::Timestamp m_nextExecution;
    openpal::TimeDuration m_delay;
};

} // namespace modbus

#endif //MODBUS_PERIODICTIMER_H
