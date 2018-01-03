#include "modbus/UnitIdentifier.h"

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


} // namespace modbus