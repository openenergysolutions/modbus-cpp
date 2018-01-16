#ifndef MODBUS_CONNECTIONEXCEPTION_H
#define MODBUS_CONNECTIONEXCEPTION_H

#include <stdexcept>

namespace modbus
{

class ConnectionException : public std::domain_error
{
public:
    ConnectionException() : std::domain_error("Connection error") {}
};

} // namespace modbus

#endif //MODBUS_CONNECTIONEXCEPTION_H
