#include "modbus/IModbusManager.h"

#include "ModbusManagerImpl.h"

namespace modbus
{

std::unique_ptr<IModbusManager> IModbusManager::Create(std::shared_ptr<logging::ILoggerSink> loggerSink)
{
    if (loggerSink == nullptr)
    {
        return std::make_unique<ModbusManagerImpl>();
    }

    return std::make_unique<ModbusManagerImpl>(loggerSink);
}

} // namespace modbus
