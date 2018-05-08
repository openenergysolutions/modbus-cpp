#ifndef MODBUS_IMODBUSMANAGER_H
#define MODBUS_IMODBUSMANAGER_H

#include <string>
#include <memory>
#include "modbus/channel/IChannel.h"
#include "modbus/Ipv4Endpoint.h"

namespace modbus
{

class Logger;

class IModbusManager
{
public:
    static std::unique_ptr<IModbusManager> create(std::shared_ptr<Logger> logger);
    static std::unique_ptr<IModbusManager> create(std::shared_ptr<Logger> logger, unsigned int number_of_threads);

public:
    IModbusManager() = default;
    virtual ~IModbusManager() = default;

    virtual std::shared_ptr<IChannel> create_tcp_channel(const std::string& name,
                                                         const Ipv4Endpoint& endpoint) = 0;
    virtual void shutdown() = 0;
};

} // namespace modbus

#endif // MODBUS_IMODBUSMANAGER_H
