#include "mocks/TcpConnectionMock.h"

TcpConnectionMock::TcpConnectionMock()
{

}

void TcpConnectionMock::set_listener(std::weak_ptr<modbus::IConnectionListener> listener)
{

}

void TcpConnectionMock::send(const openpal::rseq_t& data)
{
    m_requests.push_back(std::make_unique<openpal::Buffer>(data));
}

void TcpConnectionMock::close()
{

}

std::size_t TcpConnectionMock::get_num_requests() const
{
    return m_requests.size();
}

const std::vector<std::unique_ptr<openpal::Buffer>>& TcpConnectionMock::get_requests() const
{
    return m_requests;
}
