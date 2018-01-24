#ifndef MODBUS_CONNECTIONEXCEPTION_H
#define MODBUS_CONNECTIONEXCEPTION_H

#include "modbus/exceptions/IException.h"

namespace modbus
{

class ConnectionException : public IException
{
public:
    std::string get_message() const override
    {
        return "Connection error";
    }
};

} // namespace modbus

#endif //MODBUS_CONNECTIONEXCEPTION_H
