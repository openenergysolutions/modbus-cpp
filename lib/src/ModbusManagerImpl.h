#ifndef MODBUS_MODBUSMANAGERIMPL_H
#define MODBUS_MODBUSMANAGERIMPL_H

#include <memory>

#include "modbus/IModbusManager.h"

namespace modbus
{

class ModbusManagerImpl : public IModbusManager
{
public:
    explicit ModbusManagerImpl(std::shared_ptr<Logger> logger);

    std::unique_ptr<IChannel> CreateTcpChannel(const Ipv4Endpoint& endpoint,
                                               std::unique_ptr<ISchedule> channelRetrySchedule) override;
    void run() override;

private:
    std::shared_ptr<Logger> m_logger;
};

} // namespace modbus

#endif // MODBUS_MODBUSMANAGERIMPL_H
