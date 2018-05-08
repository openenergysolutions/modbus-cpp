#ifndef MODBUS_ISESSIONRESPONSEHANDLER_H
#define MODBUS_ISESSIONRESPONSEHANDLER_H

#include "modbus/exceptions/IException.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersResponse.h"

namespace modbus
{

class ISessionResponseHandler
{
public:
    virtual ~ISessionResponseHandler() = default;

    virtual void on_response(const ReadInputRegistersResponse& response) {};
    virtual void on_response(const ReadHoldingRegistersResponse& response) {};

    virtual void on_exception(const IException& exception) {};
    virtual void on_timeout() {};
};

} // namespace modbus

#endif //MODBUS_ISESSIONRESPONSEHANDLER_H
