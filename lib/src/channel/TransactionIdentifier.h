#ifndef MODBUS_TRANSACTIONIDENTIFIER_H
#define MODBUS_TRANSACTIONIDENTIFIER_H

#include <cstdint>
#include <ostream>

namespace modbus
{

class TransactionIdentifier
{
public:
    TransactionIdentifier(uint16_t value);

    uint16_t get_value() const;
    bool operator==(const TransactionIdentifier& other) const;
    bool operator!=(const TransactionIdentifier& other) const;
    TransactionIdentifier& operator++();
    TransactionIdentifier operator++(int);

private:
    uint16_t m_value;
};

std::ostream& operator<<(std::ostream& stream, const TransactionIdentifier& transaction_id);

} // namespace modbus

#endif //MODBUS_TRANSACTIONIDENTIFIER_H
