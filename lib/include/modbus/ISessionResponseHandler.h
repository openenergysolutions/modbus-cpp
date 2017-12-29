#ifndef MODBUS_ISESSIONRESPONSEHANDLER_H
#define MODBUS_ISESSIONRESPONSEHANDLER_H

#include "modbus/messages/ReadInputRegistersResponse.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/Exception.h"

namespace modbus
{

class ISessionResponseHandler
{
public:
    virtual ~ISessionResponseHandler() = default;

    virtual void OnResponse(const messages::ReadInputRegistersResponse& response) {};
    virtual void OnResponse(const messages::ReadHoldingRegistersResponse& response) {};

    virtual void OnException(const messages::Exception& exception) {};
};

} // namespace modbus

#endif //MODBUS_ISESSIONRESPONSEHANDLER_H
