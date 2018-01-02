#include "PeriodicSchedule.h"

namespace modbus
{

PeriodicSchedule::PeriodicSchedule(const openpal::duration_t& delay)
    : m_delay{delay}
{

}

void PeriodicSchedule::Reset(const openpal::steady_time_t& now)
{
    m_nextExecution = now + m_delay;
}

void PeriodicSchedule::OnSuccess(const openpal::steady_time_t& now)
{
    m_nextExecution = now + m_delay;
}

void PeriodicSchedule::OnTimeout(const openpal::steady_time_t& now)
{
    m_nextExecution = now + m_delay;
}

void PeriodicSchedule::OnFailure(const openpal::steady_time_t& now)
{
    m_nextExecution = now + m_delay;
}

openpal::steady_time_t PeriodicSchedule::GetNextExecution() const
{
    return m_nextExecution;
}

} // namespace modbus
