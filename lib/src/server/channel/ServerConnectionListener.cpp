#include "server/channel/ServerConnectionListener.h"

namespace modbus
{

ServerConnectionListener::ServerConnectionListener(std::shared_ptr<IServerChannelImpl> channel,
                                                   std::shared_ptr<ITcpConnection> connection)
    : m_channel{std::move(channel)},
      m_connection{std::move(connection)},
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
    m_channel->on_mbap(message, *m_connection);
}

} // namespace modbus
