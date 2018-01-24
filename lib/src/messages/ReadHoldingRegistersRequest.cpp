#include "modbus/messages/ReadHoldingRegistersRequest.h"

#include "openpal/serialization/BigEndian.h"

namespace modbus
{

ReadHoldingRegistersRequest::ReadHoldingRegistersRequest(Address starting_address, uint16_t qty_of_registers)
        : m_starting_address{starting_address},
          m_qty_of_registers{qty_of_registers}
{

}

std::unique_ptr<IRequest> ReadHoldingRegistersRequest::clone() const
{
    return std::make_unique<ReadHoldingRegistersRequest>(m_starting_address, m_qty_of_registers);
}

size_t ReadHoldingRegistersRequest::get_request_length() const
{
    return 5;
}

void ReadHoldingRegistersRequest::build_request(openpal::wseq_t& buffer) const
{
    openpal::UInt8::write_to(buffer, 0x03); // Function code
    openpal::UInt16::write_to(buffer, m_starting_address); // Starting address
    openpal::UInt16::write_to(buffer, m_qty_of_registers); // Qty of registers
}

const Address& ReadHoldingRegistersRequest::get_starting_address() const
{
    return m_starting_address;
}

uint16_t ReadHoldingRegistersRequest::get_qty_of_registers() const
{
    return m_qty_of_registers;
}

} // namespace modbus
