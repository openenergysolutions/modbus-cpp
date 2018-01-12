#include "mocks/AsioTcpConnectionWrapper.h"

AsioTcpConnectionWrapper::AsioTcpConnectionWrapper(std::shared_ptr<modbus::AsioTcpConnection> connection)
        : m_connection{std::move(connection)}
{

}

AsioTcpConnectionWrapper::~AsioTcpConnectionWrapper()
{
    m_connection->close();
}

std::shared_ptr<modbus::AsioTcpConnection> AsioTcpConnectionWrapper::get()
{
    return m_connection;
}

modbus::AsioTcpConnection* AsioTcpConnectionWrapper::operator->()
{
    return m_connection.get();
}
