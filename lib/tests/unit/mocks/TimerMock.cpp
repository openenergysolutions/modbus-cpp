#include "mocks/TimerMock.h"

TimerMock::TimerMock(const exe4cpp::steady_time_t& expiration, const exe4cpp::action_t& action)
    : m_expiration{expiration},
      m_action{action}
{

}

void TimerMock::cancel()
{

}

exe4cpp::steady_time_t TimerMock::expires_at()
{
    return m_expiration;
}

void TimerMock::execute()
{
    m_action();
}
