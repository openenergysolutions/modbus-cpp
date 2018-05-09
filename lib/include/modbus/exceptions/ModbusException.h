#ifndef MODBUS_MODBUSEXCEPTION_H
#define MODBUS_MODBUSEXCEPTION_H

#include <sstream>
#include "modbus/exceptions/IException.h"
#include "modbus/messages/ExceptionType.h"

namespace modbus
{

/**
 * @brief Modbus exception error
 *
 * This error is generated whenever a Modbus exception is received from the wire. Modbus exception
 * errors are characterized by having the most significant bit of the function code set to 1. Refer to
 * the Modbus documentation as well as the device documentation for further details.
 *
 * @see @ref ExceptionType
 */
class ModbusException : public IException
{
public:
    /**
     * @brief Retrieve the human-readable message from an @ref ExceptionType
     * @param exception_type Modbus exception type
     * @return Human-readable error message
     */
    static std::string get_message(const ExceptionType& exception_type)
    {
        std::ostringstream oss;
        oss << exception_type;
        return oss.str();
    }

public:
    /**
     * @brief Constructor
     * @param exception_type Modbus exception type
     */
    explicit ModbusException(const ExceptionType& exception_type)
        : IException{get_message(exception_type)},
          m_exception_type{exception_type}
    {

    }

    /**
     * @brief Get the Modbus exception type associated with the error
     * @return Modbus exception type
     */
    const ExceptionType& get_exception_type() const
    {
        return m_exception_type;
    }

private:
    ExceptionType m_exception_type;
};

} // namespace modbus

#endif //MODBUS_MODBUSEXCEPTION_H
