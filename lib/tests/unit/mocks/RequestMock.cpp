#include "RequestMock.h"

RequestMock::RequestMock(std::size_t length, uint8_t value)
    : m_length{length},
      m_value{value}
{

}

std::unique_ptr<modbus::IRequest> RequestMock::clone() const
{
    return std::make_unique<RequestMock>(*this);
}

size_t RequestMock::get_request_length() const
{
    return m_length;
}

void RequestMock::build_request(openpal::wseq_t& buffer) const
{
    for(std::size_t i = 0; i < m_length; ++i)
    {
        buffer.put(m_value);
    }
}
