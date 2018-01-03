#ifndef MODBUS_PERIODICTIMER_H
#define MODBUS_PERIODICTIMER_H

#include "modbus/ISchedule.h"

namespace modbus
{

class PeriodicSchedule : public ISchedule
{
public:
    explicit PeriodicSchedule(const openpal::duration_t& delay);

    void reset(const openpal::steady_time_t& now) override;
    void on_success(const openpal::steady_time_t& now) override;
    void on_timeout(const openpal::steady_time_t& now) override;
    void on_failure(const openpal::steady_time_t& now) override;
    openpal::steady_time_t get_next_execution() const override;

private:
    openpal::steady_time_t m_next_execution;
    openpal::duration_t m_delay;
};

} // namespace modbus

#endif //MODBUS_PERIODICTIMER_H
