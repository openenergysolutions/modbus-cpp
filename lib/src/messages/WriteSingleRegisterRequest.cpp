#include "modbus/messages/WriteSingleRegisterRequest.h"

#include "openpal/serialization/BigEndian.h"

namespace modbus
{

WriteSingleRegisterRequest::WriteSingleRegisterRequest(Address address, uint16_t value)
        : m_address{address},
          m_value{value}
{

}

std::unique_ptr<IRequest> WriteSingleRegisterRequest::clone() const
{
    return std::make_unique<WriteSingleRegisterRequest>(m_address, m_value);
}

size_t WriteSingleRegisterRequest::get_request_length() const
{
    return 5;
}

void WriteSingleRegisterRequest::build_request(openpal::wseq_t& buffer) const
{
    openpal::UInt8::write_to(buffer, 0x06); // Function code
    openpal::UInt16::write_to(buffer, m_address); //Address
    openpal::UInt16::write_to(buffer, m_value); // Value
}

const Address& WriteSingleRegisterRequest::get_address() const
{
    return m_address;
}

uint16_t WriteSingleRegisterRequest::get_value() const
{
    return m_value;
}

} // namespace modbus
