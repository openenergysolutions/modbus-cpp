#include "modbus/messages/Exception.h"

namespace modbus
{

Exception::Exception()
:m_exceptionType(ExceptionType::IllegalFunction, false)
{

}

Exception::Exception(ExceptionType type)
:m_exceptionType(type, true)
{

}

ExceptionType Exception::GetExceptionType() const
{
    if(m_exceptionType.second)
    {
        return m_exceptionType.first;
    }

    throw std::logic_error("Exception type was not set.");
}

Exception::operator bool() const
{
    return m_exceptionType.second;
}

bool Exception::operator==(const ExceptionType& other) const
{
    if(m_exceptionType.second)
    {
        return m_exceptionType.first == other;
    }

    return false;
}

} // namespace modbus
