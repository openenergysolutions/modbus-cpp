#include "modbus/messages/Exception.h"

namespace modbus
{

Exception::Exception()
:m_exception_type(ExceptionType::IllegalFunction, false)
{

}

Exception::Exception(ExceptionType type)
:m_exception_type(type, true)
{

}

ExceptionType Exception::get_type() const
{
    if(m_exception_type.second)
    {
        return m_exception_type.first;
    }

    throw std::logic_error("Exception type was not set.");
}

Exception::operator bool() const
{
    return m_exception_type.second;
}

bool Exception::operator==(const ExceptionType& other) const
{
    if(m_exception_type.second)
    {
        return m_exception_type.first == other;
    }

    return false;
}

} // namespace modbus
