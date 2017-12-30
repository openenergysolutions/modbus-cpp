#ifndef MODBUS_ISESSION_H
#define MODBUS_ISESSION_H

#include <memory>

#include "modbus/ResponseHandler.h"

namespace modbus
{

class ISchedule;
class ReadHoldingRegistersRequest;
class ReadHoldingRegistersResponse;
class ReadInputRegistersRequest;
class ReadInputRegistersResponse;

class ISession
{
public:
    virtual ~ISession() = default;

    // One-time requests
    virtual void SendRequest(const ReadHoldingRegistersRequest& request, ResponseHandler<ReadHoldingRegistersResponse> handler) = 0;
    virtual void SendRequest(const ReadInputRegistersRequest& request, ResponseHandler<ReadInputRegistersResponse> handler) = 0;

    // Periodic requests
    virtual void ScheduleRequest(const ReadHoldingRegistersRequest& request, std::unique_ptr<ISchedule> schedule) = 0;
    virtual void ScheduleRequest(const ReadInputRegistersRequest& request, std::unique_ptr<ISchedule> schedule) = 0;
};

} // namespace modbus

#endif //MODBUS_ISESSION_H
