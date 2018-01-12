#ifndef MODBUS_ASIOTCPCONNECTIONWRAPPER_H
#define MODBUS_ASIOTCPCONNECTIONWRAPPER_H

#include <memory>

#include "channel/AsioTcpConnection.h"

class AsioTcpConnectionWrapper
{
public:
    AsioTcpConnectionWrapper(std::shared_ptr<modbus::AsioTcpConnection> connection);
    ~AsioTcpConnectionWrapper();

    std::shared_ptr<modbus::AsioTcpConnection> get();
    modbus::AsioTcpConnection* operator->();

private:
    std::shared_ptr<modbus::AsioTcpConnection> m_connection;
};

#endif //MODBUS_ASIOTCPCONNECTIONWRAPPER_H
