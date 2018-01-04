#ifndef MODBUS_TRANSACTIONIDENTIFIER_H
#define MODBUS_TRANSACTIONIDENTIFIER_H

#include <cstdint>

namespace modbus
{

class TransactionIdentifier
{
public:
    TransactionIdentifier(uint16_t value);

    uint16_t get_value() const;
    bool operator==(const TransactionIdentifier& other) const;
    bool operator!=(const TransactionIdentifier& other) const;

private:
    uint16_t m_value;
};

} // namespace modbus

#endif //MODBUS_TRANSACTIONIDENTIFIER_H
