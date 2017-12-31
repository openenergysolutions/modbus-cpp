#ifndef MODBUS_IMODBUSMANAGER_H
#define MODBUS_IMODBUSMANAGER_H

#include <memory>

namespace modbus
{

class IChannel;
class Ipv4Endpoint;
class Logger;

class IModbusManager
{
public:
    static std::unique_ptr<IModbusManager> Create(std::shared_ptr<Logger> logger);

public:
    IModbusManager() = default;
    virtual ~IModbusManager() = default;

    virtual std::unique_ptr<IChannel> CreateTcpChannel(const Ipv4Endpoint& endpoint) = 0;
    virtual void run() = 0;
};

} // namespace modbus

#endif // MODBUS_IMODBUSMANAGER_H
