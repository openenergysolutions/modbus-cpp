#ifndef MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
#define MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H

#include "modbus/exceptions/IException.h"

namespace modbus
{

class MalformedModbusResponseException : public IException
{
public:
    explicit MalformedModbusResponseException(const std::string& message)
        : IException{message}
    {

    }
};

} // namespace modbus

#endif //MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
