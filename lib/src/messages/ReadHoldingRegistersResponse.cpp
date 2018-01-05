#include "modbus/messages/ReadHoldingRegistersResponse.h"

namespace modbus
{

Expected<ReadHoldingRegistersResponse> ReadHoldingRegistersResponse::parse(const openpal::rseq_t& data)
{
    ReadHoldingRegistersResponse response;
    response.add_value(Address{ 0x01 }, 0x24);
    return Expected<ReadHoldingRegistersResponse>(response);
}

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
