#include "modbus/messages/ReadHoldingRegistersResponse.h"

namespace modbus
{
namespace messages
{

ReadHoldingRegistersResponse::ReadHoldingRegistersResponse()
{

}

void ReadHoldingRegistersResponse::AddValue(Address address, uint16_t value)
{
    m_values.push_back(RegisterValue{address = address, value = value});
}

const std::vector<RegisterValue>& ReadHoldingRegistersResponse::GetValues() const
{
    return m_values;
}

} // namespace messages
} // namespace modbus
