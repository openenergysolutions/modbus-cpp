#ifndef MODBUS_EXECUTORMOCK_H
#define MODBUS_EXECUTORMOCK_H

#include <memory>
#include <queue>
#include "loopser/executor/IExecutor.h"
#include "mocks/TimerMock.h"

class TimerMockComparator
{
public:
    bool operator()(std::shared_ptr<TimerMock> lhs, std::shared_ptr<TimerMock> rhs)
    {
        return (lhs->expires_at() > rhs->expires_at());
    }
};

class ExecutorMock : public loopser::IExecutor
{
public:
    ExecutorMock();

    loopser::Timer start(const loopser::duration_t& duration, const loopser::action_t& action) override;
    loopser::Timer start(const loopser::steady_time_t& expiration, const loopser::action_t& action) override;
    void post(const loopser::action_t& action) override;

    loopser::steady_time_t get_time() override;

    void advance_time(const loopser::duration_t& duration);
    void set_time(const loopser::steady_time_t& time);

    unsigned int get_num_post_calls() const;

private:
    void check_expired_timers();

    loopser::steady_time_t m_current_time;
    std::priority_queue<std::shared_ptr<TimerMock>,
                        std::vector<std::shared_ptr<TimerMock>>,
                        TimerMockComparator> m_active_timers;
    unsigned int m_num_post_calls;
};

#endif //MODBUS_EXECUTORMOCK_H
