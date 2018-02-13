#ifndef MODBUS_ISCHEDULEDREQUEST_H
#define MODBUS_ISCHEDULEDREQUEST_H

#include <memory>

namespace modbus
{

class IScheduledRequest : public std::enable_shared_from_this<IScheduledRequest>
{
public:
    virtual ~IScheduledRequest() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool is_running() const = 0;

    virtual void set_frequency(const openpal::duration_t& frequency) = 0;
    virtual openpal::duration_t get_frequency() const = 0;
};

} // namespace modbus

#endif //MODBUS_ISCHEDULEDREQUEST_H
