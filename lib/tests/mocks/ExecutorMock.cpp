#include "mocks/ExecutorMock.h"

#include <chrono>
#include "mocks/TimerMock.h"

ExecutorMock::ExecutorMock()
    : m_current_time{std::chrono::steady_clock::now()},
      m_num_post_calls{0}
{

}

openpal::Timer ExecutorMock::start(const openpal::duration_t& duration, const openpal::action_t& action)
{
    return start(m_current_time + duration, action);
}

openpal::Timer ExecutorMock::start(const openpal::steady_time_t& expiration, const openpal::action_t& action)
{
    auto timer = std::make_shared<TimerMock>(expiration, action);
    m_active_timers.push(timer);

    return openpal::Timer{timer};
}

void ExecutorMock::post(const openpal::action_t& action)
{
    ++m_num_post_calls;
    action();
}

openpal::steady_time_t ExecutorMock::get_time()
{
    return m_current_time;
}

void ExecutorMock::advance_time(const openpal::duration_t& duration)
{
    set_time(m_current_time + duration);
}

void ExecutorMock::set_time(const openpal::steady_time_t& time)
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
