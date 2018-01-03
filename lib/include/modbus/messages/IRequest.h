#ifndef MODBUS_IREQUEST_H
#define MODBUS_IREQUEST_H

#include "openpal/container/SequenceTypes.h"

namespace modbus
{

class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual void build_request(openpal::wseq_t& buffer) = 0;
};

} // namespace modbus

#endif //MODBUS_IREQUEST_H
