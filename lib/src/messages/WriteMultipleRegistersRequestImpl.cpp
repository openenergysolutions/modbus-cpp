#include "messages/WriteMultipleRegistersRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

WriteMultipleRegistersRequestImpl::WriteMultipleRegistersRequestImpl(const WriteMultipleRegistersRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IRequest> WriteMultipleRegistersRequestImpl::clone() const
{
    return std::make_unique<WriteMultipleRegistersRequestImpl>(*this);
}

size_t WriteMultipleRegistersRequestImpl::get_request_length() const
{
    return 6 + 2 * m_request.values.size();
}

void WriteMultipleRegistersRequestImpl::build_request(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x10); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, static_cast<uint16_t>(m_request.values.size())); // Qty of registers
    ser4cpp::UInt8::write_to(buffer, static_cast<uint8_t>(2 * m_request.values.size())); // Byte count

    // Register values
    for(auto value : m_request.values)
    {
        ser4cpp::UInt16::write_to(buffer, value);
    }
}

const WriteMultipleRegistersRequest& WriteMultipleRegistersRequestImpl::get_request() const
{
    return m_request;
}

} // namespace modbus
