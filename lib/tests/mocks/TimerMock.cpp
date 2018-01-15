#include "mocks/TimerMock.h"

TimerMock::TimerMock(const openpal::steady_time_t& expiration, const openpal::action_t& action)
    : m_expiration{expiration},
      m_action{action}
{

}

void TimerMock::cancel()
{

}

openpal::steady_time_t TimerMock::expires_at()
{
    return m_expiration;
}

void TimerMock::execute()
{
    m_action();
}
