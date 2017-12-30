#include "modbus/IModbusManager.h"

#include "ModbusManagerImpl.h"
#include "logging/Logger.h"

namespace modbus
{

std::unique_ptr<IModbusManager> IModbusManager::Create(std::shared_ptr<Logger> logger)
{
    return std::make_unique<ModbusManagerImpl>(logger);
}

} // namespace modbus
