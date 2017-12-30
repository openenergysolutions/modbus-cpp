#ifndef MODBUS_MODBUSMANAGERIMPL_H
#define MODBUS_MODBUSMANAGERIMPL_H

#include <memory>

#include "modbus/IModbusManager.h"

namespace modbus
{

class IChannel;
class Logger;

class ModbusManagerImpl : public IModbusManager
{
public:
    ModbusManagerImpl(std::shared_ptr<Logger> logger);
    virtual ~ModbusManagerImpl() = default;

    std::unique_ptr<IChannel> CreateTcpChannel() override;
    void run() override;

private:
    std::shared_ptr<Logger> m_logger;
};

} // namespace modbus

#endif // MODBUS_MODBUSMANAGERIMPL_H
