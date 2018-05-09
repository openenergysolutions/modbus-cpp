#ifndef MODBUS_CONNECTIONEXCEPTION_H
#define MODBUS_CONNECTIONEXCEPTION_H

/** @file
 * @brief Class @ref modbus::ConnectionException
 */

#include "modbus/exceptions/IException.h"

namespace modbus
{

/**
 * @brief Connection error
 *
 * Represents a connection error. It can happen because an host is unreachable, the network
 * adapter is faulty, the other end closed the connection, etc.
 */
class ConnectionException : public IException
{
public:
    /**
     * @brief Constructor
     */
    ConnectionException() : IException{"Connection error"} {}
};

} // namespace modbus

#endif //MODBUS_CONNECTIONEXCEPTION_H
