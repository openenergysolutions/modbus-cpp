#ifndef MODBUS_TIMEOUTEXCEPTION_H
#define MODBUS_TIMEOUTEXCEPTION_H

#include "modbus/exceptions/IException.h"

namespace modbus
{

class TimeoutException : public IException
{
public:
    std::string get_message() const override
    {
        return "Timeout reached";
    }
};

} // namespace modbus

#endif //MODBUS_TIMEOUTEXCEPTION_H
