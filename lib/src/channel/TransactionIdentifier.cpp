#include "channel/TransactionIdentifier.h"

#include <iomanip>

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

TransactionIdentifier& TransactionIdentifier::operator++()
{
    ++m_value;
    return *this;
}

TransactionIdentifier TransactionIdentifier::operator++(int)
{
    auto temp = (*this);
    ++m_value;
    return temp;
}

std::ostream& operator<<(std::ostream& stream, const TransactionIdentifier& transaction_id)
{
    stream << "0x"
           << std::uppercase
           << std::setw(4) << std::setfill('0')
           << std::hex << (unsigned int)transaction_id.get_value();
}

} // namespace modbus