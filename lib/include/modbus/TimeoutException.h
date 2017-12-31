#ifndef MODBUS_TIMEOUTEXCEPTION_H
#define MODBUS_TIMEOUTEXCEPTION_H

#include <stdexcept>

namespace modbus
{

class TimeoutException : public std::domain_error
{
public:
    TimeoutException() : std::domain_error("Timeout reached") {}
};

} // namespace modbus

#endif //MODBUS_TIMEOUTEXCEPTION_H
