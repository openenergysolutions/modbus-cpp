#include "messages/WriteMultipleCoilsRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

WriteMultipleCoilsRequestImpl::WriteMultipleCoilsRequestImpl(const WriteMultipleCoilsRequest& request)
    : m_request{request}
{

}

std::unique_ptr<IRequest> WriteMultipleCoilsRequestImpl::clone() const
{
    return std::make_unique<WriteMultipleCoilsRequestImpl>(*this);
}

size_t WriteMultipleCoilsRequestImpl::get_request_length() const
{
    auto num_bytes = m_request.values.size() / 8;
    if (m_request.values.size() % 8 != 0)
    {
        ++num_bytes;
    }

    return 6 + num_bytes;
}

void WriteMultipleCoilsRequestImpl::build_request(ser4cpp::wseq_t& buffer) const
{
    auto num_bytes = m_request.values.size() / 8;
    if (m_request.values.size() % 8 != 0)
    {
        ++num_bytes;
    }

    ser4cpp::UInt8::write_to(buffer, 0x0F); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, static_cast<uint16_t>(m_request.values.size())); // Qty of coils
    ser4cpp::UInt8::write_to(buffer, static_cast<uint8_t>(num_bytes)); // Byte count

    // Coil values
    auto current_bit = 0;
    for(unsigned int i = 0; i < num_bytes; ++i)
    {
        uint8_t value = 0x00;
        for(unsigned int j = 0; j < 8; ++j)
        {
            if(current_bit >= m_request.values.size())
            {
                break;
            }

            value |= static_cast<uint8_t>(m_request.values[current_bit]) << j;
            ++current_bit;
        }

        ser4cpp::UInt8::write_to(buffer, value);
    }
}

const WriteMultipleCoilsRequest& WriteMultipleCoilsRequestImpl::get_request() const
{
    return m_request;
}

} // namespace modbus
