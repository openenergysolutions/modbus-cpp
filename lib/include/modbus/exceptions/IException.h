#ifndef MODBUS_IEXCEPTION_H
#define MODBUS_IEXCEPTION_H

#include <string>

namespace modbus
{

class IException
{
public:
    explicit IException(const std::string& message)
        : m_message{message}
    {}

    const std::string& get_message() const
    {
        return m_message;
    }

private:
    std::string m_message;
};

} // namespace modbus

#endif //MODBUS_IEXCEPTION_H
