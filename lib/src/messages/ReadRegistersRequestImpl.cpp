#include "messages/ReadRegistersRequestImpl.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

template <uint8_t function_code, typename request_t>
ReadRegistersRequestImpl<function_code, request_t>::ReadRegistersRequestImpl(const request_t& request)
        : m_request{request}
{

}

template <uint8_t function_code, typename request_t>
std::unique_ptr<IRequest> ReadRegistersRequestImpl<function_code, request_t>::clone() const
{
    return std::make_unique<ReadRegistersRequestImpl>(m_request);
}

template <uint8_t function_code, typename request_t>
size_t ReadRegistersRequestImpl<function_code, request_t>::get_request_length() const
{
    return 5;
}

template <uint8_t function_code, typename request_t>
void ReadRegistersRequestImpl<function_code, request_t>::build_request(ser4cpp::wseq_t& buffer) const
{
    ser4cpp::UInt8::write_to(buffer, function_code); // Function code
    ser4cpp::UInt16::write_to(buffer, m_request.starting_address); // Starting address
    ser4cpp::UInt16::write_to(buffer, m_request.qty_of_registers); // Qty of registers
}

template <uint8_t function_code, typename request_t>
const request_t& ReadRegistersRequestImpl<function_code, request_t>::get_request() const
{
    return m_request;
}

template class ReadRegistersRequestImpl<0x03, ReadHoldingRegistersRequest>;
template class ReadRegistersRequestImpl<0x04, ReadInputRegistersRequest>;

} // namespace modbus
