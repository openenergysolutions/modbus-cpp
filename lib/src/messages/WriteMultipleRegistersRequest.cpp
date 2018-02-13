#include "modbus/messages/WriteMultipleRegistersRequest.h"

#include "openpal/serialization/BigEndian.h"

namespace modbus
{

WriteMultipleRegistersRequest::WriteMultipleRegistersRequest(Address starting_address)
        : m_starting_address{starting_address}
{

}

std::unique_ptr<IRequest> WriteMultipleRegistersRequest::clone() const
{
    return std::make_unique<WriteMultipleRegistersRequest>(*this);
}

size_t WriteMultipleRegistersRequest::get_request_length() const
{
    return 6 + 2 * m_values.size();
}

void WriteMultipleRegistersRequest::build_request(openpal::wseq_t& buffer) const
{
    openpal::UInt8::write_to(buffer, 0x10); // Function code
    openpal::UInt16::write_to(buffer, m_starting_address); // Starting address
    openpal::UInt16::write_to(buffer, m_values.size()); // Qty of registers
    openpal::UInt8::write_to(buffer, 2 * m_values.size()); // Byte count

    // Register values
    for(auto value : m_values)
    {
        openpal::UInt16::write_to(buffer, value);
    }
}

void WriteMultipleRegistersRequest::add_register(uint16_t value)
{
    m_values.push_back(value);
}

const Address& WriteMultipleRegistersRequest::get_starting_address() const
{
    return m_starting_address;
}

} // namespace modbus