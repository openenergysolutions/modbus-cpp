#include "PeriodicSchedule.h"

namespace modbus
{

PeriodicSchedule::PeriodicSchedule(const openpal::duration_t& delay)
    : m_delay{delay}
{

}

std::unique_ptr<ISchedule> PeriodicSchedule::clone()
{
    return std::make_unique<PeriodicSchedule>(*this);
}

void PeriodicSchedule::reset(const openpal::steady_time_t& now)
{
    m_next_execution = now + m_delay;
}

void PeriodicSchedule::on_success(const openpal::steady_time_t& now)
{
    m_next_execution = now + m_delay;
}

void PeriodicSchedule::on_timeout(const openpal::steady_time_t& now)
{
    m_next_execution = now + m_delay;
}

void PeriodicSchedule::on_failure(const openpal::steady_time_t& now)
{
    m_next_execution = now + m_delay;
}

openpal::steady_time_t PeriodicSchedule::get_next_execution() const
{
    return m_next_execution;
}

} // namespace modbus
