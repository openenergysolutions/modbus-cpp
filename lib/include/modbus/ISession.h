#ifndef MODBUS_ISESSION_H
#define MODBUS_ISESSION_H

#include "modbus/ResponseHandler.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"

#include "ISchedule.h"

namespace modbus
{

class ISession
{
public:
    virtual ~ISession() = default;

    // One-time requests
    virtual void SendRequest(const messages::ReadHoldingRegistersRequest& request, ResponseHandler<messages::ReadHoldingRegistersResponse> handler) = 0;
    virtual void SendRequest(const messages::ReadInputRegistersRequest& request, ResponseHandler<messages::ReadInputRegistersResponse> handler) = 0;

    // Periodic requests
    virtual void ScheduleRequest(const messages::ReadHoldingRegistersRequest& request, std::unique_ptr<ISchedule> schedule) = 0;
    virtual void ScheduleRequest(const messages::ReadInputRegistersRequest& request, std::unique_ptr<ISchedule> schedule) = 0;
};

} // namespace modbus

#endif //MODBUS_ISESSION_H
