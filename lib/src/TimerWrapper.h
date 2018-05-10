#ifndef MODBUS_TIMERWRAPPER_H
#define MODBUS_TIMERWRAPPER_H

#include "exe4cpp/Timer.h"
#include "modbus/ITimer.h"

namespace modbus
{

class TimerWrapper : public ITimer
{
public:
    TimerWrapper(exe4cpp::Timer timer_impl);

    void cancel() override;
    steady_time_t expires_at() override;

private:
    exe4cpp::Timer m_timer_impl;
};

} // namespace modbus

#endif //MODBUS_TIMERWRAPPER_H
