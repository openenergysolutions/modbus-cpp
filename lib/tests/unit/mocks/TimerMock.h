#ifndef MODBUS_TIMERMOCK_H
#define MODBUS_TIMERMOCK_H

#include "exe4cpp/ITimer.h"

class TimerMock : public exe4cpp::ITimer
{
public:
    TimerMock(const exe4cpp::steady_time_t& expiration, const exe4cpp::action_t& action);

    void cancel() override;
    exe4cpp::steady_time_t expires_at() override;

    void execute();

private:
    exe4cpp::steady_time_t m_expiration;
    exe4cpp::action_t m_action;
};

#endif //MODBUS_TIMERMOCK_H
