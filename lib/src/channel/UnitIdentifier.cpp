#include "modbus/channel/UnitIdentifier.h"

#include <ios>
#include <iomanip>

namespace modbus
{

UnitIdentifier::UnitIdentifier(uint8_t value)
        : m_value(value)
{

}

uint8_t UnitIdentifier::get_value() const
{
    return m_value;
}

bool UnitIdentifier::operator==(const UnitIdentifier& other) const
{
    return m_value == other.m_value;
}

bool UnitIdentifier::operator!=(const UnitIdentifier& other) const
{
    return m_value != other.m_value;
}

std::ostream& operator<<(std::ostream& stream, const UnitIdentifier& unit_id)
{
    stream << "0x"
           << std::uppercase
           << std::setw(2) << std::setfill('0')
           << std::hex << (unsigned int)unit_id.get_value();
}

} // namespace modbus