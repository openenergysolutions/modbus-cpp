#ifndef MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
#define MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H

/** @file
 * @brief Class @ref modbus::MalformedModbusResponseException
 */

#include "modbus/exceptions/IException.h"

namespace modbus
{

/**
 * @brief Malformed Modbus response error
 *
 * This error is generated when a malformed response is received. Check the associated message with
 * @ref IException::get_message() for more details about the error. The vendor of the faulty device should
 * be notified of the faulty response.
 */
class MalformedModbusResponseException : public IException
{
public:
    /**
     * @brief Constructor
     * @param message Message describing the exception
     */
    explicit MalformedModbusResponseException(const std::string& message)
        : IException{message}
    {

    }
};

} // namespace modbus

#endif //MODBUS_MALFORMEDMODBUSRESPONSEEXCEPTION_H
