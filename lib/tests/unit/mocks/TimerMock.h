#ifndef MODBUS_TIMERMOCK_H
#define MODBUS_TIMERMOCK_H

#include "loopser/executor/ITimer.h"

class TimerMock : public loopser::ITimer
{
public:
    TimerMock(const loopser::steady_time_t& expiration, const loopser::action_t& action);

    void cancel() override;
    loopser::steady_time_t expires_at() override;

    void execute();

private:
    loopser::steady_time_t m_expiration;
    loopser::action_t m_action;
};

#endif //MODBUS_TIMERMOCK_H
