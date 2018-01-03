#include "modbus/IModbusManager.h"

#include <algorithm>
#include <thread>

#include "ModbusManagerImpl.h"
#include "logging/Logger.h"

namespace modbus
{

std::unique_ptr<IModbusManager> IModbusManager::create(std::shared_ptr<Logger> logger)
{
    return IModbusManager::create(std::move(logger), std::thread::hardware_concurrency());
}

std::unique_ptr<IModbusManager> IModbusManager::create(std::shared_ptr<Logger> logger, unsigned int number_of_threads)
{
    number_of_threads = std::max(1u, number_of_threads);
    return std::make_unique<ModbusManagerImpl>(logger, number_of_threads);
}

} // namespace modbus
