#ifndef MODBUS_IREQUEST_H
#define MODBUS_IREQUEST_H

#include "openpal/container/SequenceTypes.h"

namespace modbus
{

class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual size_t get_request_length() const = 0;
    virtual void build_request(openpal::wseq_t& buffer) const = 0;
};

} // namespace modbus

#endif //MODBUS_IREQUEST_H
