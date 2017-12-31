#include "modbus/messages/ReadHoldingRegistersResponse.h"

namespace modbus
{

void ReadHoldingRegistersResponse::AddValue(Address address, uint16_t value)
{
    m_values.push_back(RegisterValue{address = address, value = value});
}

const std::vector<RegisterValue>& ReadHoldingRegistersResponse::GetValues() const
{
    return m_values;
}

} // namespace modbus
