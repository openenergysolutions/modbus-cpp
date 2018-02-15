#ifndef MODBUS_TIMERMOCK_H
#define MODBUS_TIMERMOCK_H

#include "openpal/executor/ITimer.h"

class TimerMock : public openpal::ITimer
{
public:
    TimerMock(const openpal::steady_time_t& expiration, const openpal::action_t& action);

    void cancel() override;
    openpal::steady_time_t expires_at() override;

    void execute();

private:
    openpal::steady_time_t m_expiration;
    openpal::action_t m_action;
};

#endif //MODBUS_TIMERMOCK_H
