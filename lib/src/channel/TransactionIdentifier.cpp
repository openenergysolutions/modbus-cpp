#include "channel/TransactionIdentifier.h"

namespace modbus
{

TransactionIdentifier::TransactionIdentifier(uint16_t value)
        : m_value(value)
{

}

uint16_t TransactionIdentifier::get_value() const
{
    return m_value;
}

bool TransactionIdentifier::operator==(const TransactionIdentifier& other) const
{
    return m_value == other.m_value;
}

bool TransactionIdentifier::operator!=(const TransactionIdentifier& other) const
{
    return m_value != other.m_value;
}


} // namespace modbus