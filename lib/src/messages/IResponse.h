#ifndef MODBUS_IRESPONSE_H
#define MODBUS_IRESPONSE_H

#include "ser4cpp/container/SequenceTypes.h"
#include "modbus/Expected.h"

namespace modbus
{

class IResponse
{
public:
    virtual ~IResponse() = default;

protected:
    static Expected<bool> parse_function_code(uint8_t function_code, ser4cpp::rseq_t& data);
};

} // namespace modbus

#endif //MODBUS_IRESPONSE_H
