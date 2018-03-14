#include "mocks/TcpConnectionMock.h"

TcpConnectionMock::TcpConnectionMock()
    : m_parser{this},
      m_num_close{0}
{

}

void TcpConnectionMock::set_listener(std::weak_ptr<modbus::IConnectionListener> listener)
{

}

void TcpConnectionMock::send(const loopser::rseq_t& data)
{
    m_parser.parse(data);
}

void TcpConnectionMock::close()
{
    ++m_num_close;
}

void TcpConnectionMock::on_mbap_message(const modbus::MbapMessage& message)
{
    m_request_buffers.push_back(std::make_unique<loopser::Buffer>(message.data));
    m_requests.emplace_back(message.unit_id, message.transaction_id, m_request_buffers.back()->as_rslice());
}

std::size_t TcpConnectionMock::get_num_requests() const
{
    return m_requests.size();
}

const std::vector<modbus::MbapMessage>& TcpConnectionMock::get_requests() const
{
    return m_requests;
}

unsigned int TcpConnectionMock::get_num_close() const
{
    return m_num_close;
}
