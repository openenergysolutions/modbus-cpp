#ifndef MODBUS_IREQUEST_H
#define MODBUS_IREQUEST_H

#include "openpal/container/Buffer.h"

namespace modbus
{
namespace messages
{

class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual void BuildRequest(openpal::Buffer &buffer) = 0;
};

} // namespace messages
} // namespace modbus

#endif //MODBUS_IREQUEST_H
