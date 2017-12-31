#include "PeriodicSchedule.h"

namespace modbus
{

PeriodicSchedule::PeriodicSchedule(const openpal::TimeDuration& delay)
    : m_delay{delay}
{

}

void PeriodicSchedule::Reset(const openpal::Timestamp& now)
{
    m_nextExecution = now.add(m_delay);
}

void PeriodicSchedule::OnSuccess(const openpal::Timestamp& now)
{
    m_nextExecution = now.add(m_delay);
}

void PeriodicSchedule::OnTimeout(const openpal::Timestamp& now)
{
    m_nextExecution = now.add(m_delay);
}

void PeriodicSchedule::OnFailure(const openpal::Timestamp& now)
{
    m_nextExecution = now.add(m_delay);
}

openpal::Timestamp PeriodicSchedule::GetNextExecution() const
{
    return m_nextExecution;
}

} // namespace modbus
