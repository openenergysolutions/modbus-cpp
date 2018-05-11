#include "TimerWrapper.h"

namespace modbus
{

TimerWrapper::TimerWrapper(exe4cpp::Timer timer_impl)
    : m_timer_impl{timer_impl}
{

}

void TimerWrapper::cancel()
{
    m_timer_impl.cancel();
}

steady_time_t TimerWrapper::expires_at()
{
    return m_timer_impl.expires_at();
}

} // namespace modbus
