#ifndef MODBUS_UNITIDENTIFIER_H
#define MODBUS_UNITIDENTIFIER_H

#include <cstdint>

namespace modbus
{

class UnitIdentifier
{
    static constexpr uint8_t DEFAULT_UNIT_IDENTIFIER = 0xFF;

public:
    static UnitIdentifier Default()
    {
        return UnitIdentifier(DEFAULT_UNIT_IDENTIFIER);
    }

public:
    UnitIdentifier(uint8_t value);

    uint8_t GetValue() const;
    bool operator==(const UnitIdentifier& other) const;
    bool operator!=(const UnitIdentifier& other) const;

private:
    uint8_t m_value;
};

} // namespace modbus

#endif //MODBUS_UNITIDENTIFIER_H
