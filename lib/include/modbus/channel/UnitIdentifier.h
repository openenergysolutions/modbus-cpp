#ifndef MODBUS_UNITIDENTIFIER_H
#define MODBUS_UNITIDENTIFIER_H

#include <cstdint>

namespace modbus
{

class UnitIdentifier
{
    constexpr static uint8_t DefaultUnitIdentifier = 0xFF;

public:
    static UnitIdentifier default_unit_identifier()
    {
        return UnitIdentifier(DefaultUnitIdentifier);
    }

public:
    UnitIdentifier(uint8_t value);

    uint8_t get_value() const;
    bool operator==(const UnitIdentifier& other) const;
    bool operator!=(const UnitIdentifier& other) const;

private:
    uint8_t m_value;
};

} // namespace modbus

#endif //MODBUS_UNITIDENTIFIER_H
