#ifndef MODBUS_EXCEPTION_H
#define MODBUS_EXCEPTION_H

#include <utility>

#include "modbus/messages/ExceptionType.h"

namespace modbus
{

class Exception
{
public:
    Exception();
    explicit Exception(ExceptionType type);

    ExceptionType get_type() const;

    explicit operator bool() const;
    bool operator==(const ExceptionType& other) const;

private:
    std::pair<ExceptionType, bool> m_exception_type;
};

} // namespace modbus

#endif //MODBUS_EXCEPTION_H
