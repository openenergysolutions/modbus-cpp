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

    virtual void on_response(const ReadInputRegistersResponse& response) {};
    virtual void on_response(const ReadHoldingRegistersResponse& response) {};

    virtual void on_exception(const Exception& exception) {};
    virtual void on_timeout() {};
};

} // namespace modbus

#endif //MODBUS_ISESSIONRESPONSEHANDLER_H
