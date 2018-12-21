#include "server/channel/ServerConnectionListener.h"

namespace modbus
{

ServerConnectionListener::ServerConnectionListener(std::weak_ptr<IServerChannelImpl> channel,
                                                   std::weak_ptr<ITcpConnection> connection)
    : m_channel{channel},
      m_connection{connection},
      m_parser{this}
{

}

void ServerConnectionListener::on_write_done()
{

}

void ServerConnectionListener::on_receive(const ser4cpp::rseq_t& data)
{
    m_parser.parse(data);
}

void ServerConnectionListener::on_error(const std::string& message)
{

}

void ServerConnectionListener::on_mbap_message(const MbapMessage& message)
{
    auto channel = m_channel.lock();
    auto connection = m_connection.lock();
    if(channel && connection)
    {
        channel->on_mbap(message, *connection);
    }
}

} // namespace modbus
