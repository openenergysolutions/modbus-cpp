#ifndef MODBUS_IEXCEPTION_H
#define MODBUS_IEXCEPTION_H

#include <string>

namespace modbus
{

class IException
{
public:
    virtual std::string get_message() const = 0;
};

} // namespace modbus

#endif //MODBUS_IEXCEPTION_H
