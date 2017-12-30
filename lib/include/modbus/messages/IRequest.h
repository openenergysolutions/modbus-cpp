#ifndef MODBUS_IREQUEST_H
#define MODBUS_IREQUEST_H

namespace openpal
{
    class Buffer;
}

namespace modbus
{

class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual void BuildRequest(openpal::Buffer &buffer) = 0;
};

} // namespace modbus

#endif //MODBUS_IREQUEST_H
