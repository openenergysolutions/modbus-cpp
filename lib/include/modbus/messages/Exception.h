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
    Exception(ExceptionType type);

    ExceptionType GetExceptionType() const;

    operator bool() const;
    bool operator==(const ExceptionType& other) const;

private:
    std::pair<ExceptionType, bool> m_exceptionType;
};

} // namespace modbus

#endif //MODBUS_EXCEPTION_H
