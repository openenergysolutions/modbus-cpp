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
    virtual void cancel() = 0;
};

} // namespace modbus

#endif //MODBUS_ISCHEDULEDREQUEST_H
