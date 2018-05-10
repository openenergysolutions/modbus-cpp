#ifndef MODBUS_IREQUEST_H
#define MODBUS_IREQUEST_H

#include <memory>
#include "ser4cpp/container/SequenceTypes.h"

namespace modbus
{

class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual std::unique_ptr<IRequest> clone() const = 0;

    virtual size_t get_request_length() const = 0;
    virtual void build_request(ser4cpp::wseq_t& buffer) const = 0;
};

} // namespace modbus

#endif //MODBUS_IREQUEST_H