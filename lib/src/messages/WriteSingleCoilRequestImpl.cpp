#include "messages/WriteSingleCoilRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

WriteSingleCoilRequestImpl::WriteSingleCoilRequestImpl(const WriteSingleCoilRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IRequest> WriteSingleCoilRequestImpl::clone() const
{
    return std::make_unique<WriteSingleCoilRequestImpl>(m_request);
}

size_t WriteSingleCoilRequestImpl::get_request_length() const
{
    return 5;
}

void WriteSingleCoilRequestImpl::build_request(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, 0x05); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.value.address); // Address

    auto value = WriteSingleCoilRequestImpl::OFF;
    if(m_request.value.value == true)
    {
        value = WriteSingleCoilRequestImpl::ON;
    }
    ser4cpp::UInt16::write_to(buffer, value); // Value
}

const WriteSingleCoilRequest& WriteSingleCoilRequestImpl::get_request() const
{
    return m_request;
}

} // namespace modbus
