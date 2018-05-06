#include "modbus/messages/WriteSingleRegisterRequest.h"

#include "ser4cpp/serialization/BigEndian.h"

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

void WriteSingleRegisterRequest::build_request(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x06); // Function code
    ser4cpp::UInt16::write_to(buffer, m_address); // Address
    ser4cpp::UInt16::write_to(buffer, m_value); // Value
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
