#ifndef MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
#define MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H

#include "modbus/exceptions/IException.h"

namespace modbus
{

class MalformedModbusResponseException : public IException
{
public:
    explicit MalformedModbusResponseException(const std::string& message)
        : m_message{message}
    {

    }

    std::string get_message() const override
    {
        return m_message;
    }

private:
    std::string m_message;
};

} // namespace modbus

#endif //MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
