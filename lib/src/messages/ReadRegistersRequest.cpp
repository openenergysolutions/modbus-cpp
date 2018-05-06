#include "modbus/messages/ReadRegistersRequest.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

template <uint8_t function_code>
ReadRegistersRequest<function_code>::ReadRegistersRequest(Address starting_address, uint16_t qty_of_registers)
        : m_starting_address{starting_address},
          m_qty_of_registers{qty_of_registers}
{

}

template <uint8_t function_code>
std::unique_ptr<IRequest> ReadRegistersRequest<function_code>::clone() const
{
    return std::make_unique<ReadRegistersRequest>(m_starting_address, m_qty_of_registers);
}

template <uint8_t function_code>
size_t ReadRegistersRequest<function_code>::get_request_length() const
{
    return 5;
}

template <uint8_t function_code>
void ReadRegistersRequest<function_code>::build_request(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, function_code); // Function code
    ser4cpp::UInt16::write_to(buffer, m_starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, m_qty_of_registers); // Qty of registers
}

template <uint8_t function_code>
const Address& ReadRegistersRequest<function_code>::get_starting_address() const
{
    return m_starting_address;
}

template <uint8_t function_code>
uint16_t ReadRegistersRequest<function_code>::get_qty_of_registers() const
{
    return m_qty_of_registers;
}

template class ReadRegistersRequest<0x03>;
template class ReadRegistersRequest<0x04>;

} // namespace modbus
