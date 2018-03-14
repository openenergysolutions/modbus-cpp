#include "mocks/ExecutorMock.h"

#include <chrono>
#include "mocks/TimerMock.h"

ExecutorMock::ExecutorMock()
    : m_current_time{std::chrono::steady_clock::now()},
      m_num_post_calls{0}
{

}

loopser::Timer ExecutorMock::start(const loopser::duration_t& duration, const loopser::action_t& action)
{
    return start(m_current_time + duration, action);
}

loopser::Timer ExecutorMock::start(const loopser::steady_time_t& expiration, const loopser::action_t& action)
{
    auto timer = std::make_shared<TimerMock>(expiration, action);
    m_active_timers.push(timer);

    return loopser::Timer{timer};
}

void ExecutorMock::post(const loopser::action_t& action)
{
    ++m_num_post_calls;
    action();
}

loopser::steady_time_t ExecutorMock::get_time()
{
    return m_current_time;
}

void ExecutorMock::advance_time(const loopser::duration_t& duration)
{
    set_time(m_current_time + duration);
}

void ExecutorMock::set_time(const loopser::steady_time_t& time)
{
    m_current_time = time;

    check_expired_timers();
}

void ExecutorMock::check_expired_timers()
{
    while(!m_active_timers.empty() && m_active_timers.top()->expires_at() <= m_current_time)
    {
        m_active_timers.top()->execute();
        m_active_timers.pop();
    }
}

unsigned int ExecutorMock::get_num_post_calls() const
{
    return m_num_post_calls;
}
