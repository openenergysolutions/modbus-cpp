#include "modbus/messages/ReadHoldingRegistersResponse.h"

namespace modbus
{

void ReadHoldingRegistersResponse::add_value(Address address, uint16_t value)
{
    m_values.push_back(RegisterValue {
        address,
        value
    });
}

const std::vector<RegisterValue>& ReadHoldingRegistersResponse::get_values() const
{
    return m_values;
}

} // namespace modbus
