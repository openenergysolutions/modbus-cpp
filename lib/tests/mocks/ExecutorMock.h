#ifndef MODBUS_EXECUTORMOCK_H
#define MODBUS_EXECUTORMOCK_H

#include <memory>
#include <queue>
#include "openpal/executor/IExecutor.h"
#include "mocks/TimerMock.h"

class TimerMockComparator
{
public:
    bool operator()(std::shared_ptr<TimerMock> lhs, std::shared_ptr<TimerMock> rhs)
    {
        return (lhs->expires_at() > rhs->expires_at());
    }
};

class ExecutorMock : public openpal::IExecutor
{
public:
    ExecutorMock();

    openpal::Timer start(const openpal::duration_t& duration, const openpal::action_t& action) override;
    openpal::Timer start(const openpal::steady_time_t& expiration, const openpal::action_t& action) override;
    void post(const openpal::action_t& action) override;

    openpal::steady_time_t get_time() override;

    void advance_time(const openpal::duration_t& duration);
    void set_time(const openpal::steady_time_t& time);

private:
    void check_expired_timers();

    openpal::steady_time_t m_current_time;
    std::priority_queue<std::shared_ptr<TimerMock>,
                        std::vector<std::shared_ptr<TimerMock>>,
                        TimerMockComparator> m_active_timers;
};

#endif //MODBUS_EXECUTORMOCK_H
