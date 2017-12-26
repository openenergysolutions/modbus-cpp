#ifndef MODBUS_MODBUSMANAGERIMPL_H
#define MODBUS_MODBUSMANAGERIMPL_H

#include "modbus/IModbusManager.h"

#include "logging/CustomLoggerSink.h"
#include "logging/Logger.h"

namespace modbus
{

class ModbusManagerImpl : public IModbusManager
{
public:
    ModbusManagerImpl();
    ModbusManagerImpl(std::shared_ptr<logging::ILoggerSink> loggerSink);
    virtual ~ModbusManagerImpl() = default;

    void run() override;

private:
    logging::Logger m_logger;
};

} // namespace modbus

#endif // MODBUS_MODBUSMANAGERIMPL_H
