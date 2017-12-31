#ifndef MODBUS_ISESSIONRESPONSEHANDLER_H
#define MODBUS_ISESSIONRESPONSEHANDLER_H

namespace modbus
{

class Exception;
class ReadInputRegistersResponse;
class ReadHoldingRegistersResponse;

class ISessionResponseHandler
{
public:
    virtual ~ISessionResponseHandler() = default;

    virtual void OnResponse(const ReadInputRegistersResponse& response) {};
    virtual void OnResponse(const ReadHoldingRegistersResponse& response) {};

    virtual void OnException(const Exception& exception) {};
    virtual void OnTimeout() {};
};

} // namespace modbus

#endif //MODBUS_ISESSIONRESPONSEHANDLER_H
