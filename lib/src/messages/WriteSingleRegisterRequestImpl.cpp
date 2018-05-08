#include "messages/WriteSingleRegisterRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

WriteSingleRegisterRequestImpl::WriteSingleRegisterRequestImpl(const WriteSingleRegisterRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IRequest> WriteSingleRegisterRequestImpl::clone() const
{
    return std::make_unique<WriteSingleRegisterRequestImpl>(m_request);
}

size_t WriteSingleRegisterRequestImpl::get_request_length() const
{
    return 5;
}

void WriteSingleRegisterRequestImpl::build_request(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x06); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.value.address); // Address
    ser4cpp::UInt16::write_to(buffer, m_request.value.value); // Value
}

const WriteSingleRegisterRequest& WriteSingleRegisterRequestImpl::get_request() const
{
    return m_request;
}

} // namespace modbus
