#ifndef MODBUS_MODBUSEXCEPTION_H
#define MODBUS_MODBUSEXCEPTION_H

#include <sstream>
#include "modbus/exceptions/IException.h"
#include "modbus/messages/ExceptionType.h"

namespace modbus
{

class ModbusException : public IException
{
public:
    static std::string get_message(const ExceptionType& exception_type)
    {
        std::ostringstream oss;
        oss << exception_type;
        return oss.str();
    }

public:
    explicit ModbusException(const ExceptionType& exception_type)
        : IException{get_message(exception_type)},
          m_exception_type{exception_type}
    {

    }

    const ExceptionType& get_exception_type() const
    {
        return m_exception_type;
    }

private:
    ExceptionType m_exception_type;
};

} // namespace modbus

#endif //MODBUS_MODBUSEXCEPTION_H
