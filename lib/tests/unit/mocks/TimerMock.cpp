#include "mocks/TimerMock.h"

TimerMock::TimerMock(const loopser::steady_time_t& expiration, const loopser::action_t& action)
    : m_expiration{expiration},
      m_action{action}
{

}

void TimerMock::cancel()
{

}

loopser::steady_time_t TimerMock::expires_at()
{
    return m_expiration;
}

void TimerMock::execute()
{
    m_action();
}
