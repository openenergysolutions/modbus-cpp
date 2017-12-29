#ifndef MODBUS_ISCHEDULE_H
#define MODBUS_ISCHEDULE_H

#include <memory>

#include "openpal/executor/TimeDuration.h"
#include "openpal/executor/Timestamp.h"

namespace modbus
{

class ISchedule
{
public:
    virtual ~ISchedule() = default;

    virtual void Reset(const openpal::Timestamp& now) = 0;
    virtual void OnSuccess(const openpal::Timestamp& now) = 0;
    virtual void OnTimeout(const openpal::Timestamp& now) = 0;
    virtual openpal::Timestamp GetNextExecution() const = 0;
};

class PeriodicSchedule : public ISchedule
{
public:
    PeriodicSchedule(const openpal::TimeDuration& delay)
            :m_delay(delay)
    {

    }

    void Reset(const openpal::Timestamp& now) override
    {
        m_nextExecution = now.add(m_delay);
    }

    void OnSuccess(const openpal::Timestamp& now) override
    {
        m_nextExecution = now.add(m_delay);
    };

    void OnTimeout(const openpal::Timestamp& now) override
    {
        m_nextExecution = now.add(m_delay);
    };

    openpal::Timestamp GetNextExecution() const override
    {
        return m_nextExecution;
    }

private:
    openpal::Timestamp m_nextExecution;
    openpal::TimeDuration m_delay;
};

} // namespace modbus

#endif //MODBUS_ISCHEDULE_H
