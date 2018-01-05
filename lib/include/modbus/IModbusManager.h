#ifndef MODBUS_IMODBUSMANAGER_H
#define MODBUS_IMODBUSMANAGER_H

#include <memory>

namespace modbus
{

class IChannel;
class Ipv4Endpoint;
class ISchedule;
class Logger;

class IModbusManager
{
public:
    static std::unique_ptr<IModbusManager> create(std::shared_ptr<Logger> logger);
    static std::unique_ptr<IModbusManager> create(std::shared_ptr<Logger> logger, unsigned int number_of_threads);

public:
    IModbusManager() = default;
    virtual ~IModbusManager() = default;

    virtual std::shared_ptr<IChannel> create_tcp_channel(const Ipv4Endpoint& endpoint,
                                                         std::unique_ptr<ISchedule> channel_retry_schedule) = 0;
    virtual void shutdown() = 0;
};

} // namespace modbus

#endif // MODBUS_IMODBUSMANAGER_H
