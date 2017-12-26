#ifndef MODBUS_IMODBUSMANAGER_H
#define MODBUS_IMODBUSMANAGER_H

#include <memory>

namespace modbus
{

namespace logging
{
    class Logger;
}

class IModbusManager
{
public:
    static std::unique_ptr<IModbusManager> Create();
    static std::unique_ptr<IModbusManager> Create(std::shared_ptr<logging::Logger> logger);

public:
    IModbusManager() = default;
    virtual ~IModbusManager() = default;

    virtual void run() = 0;
};

} // namespace modbus

#endif // MODBUS_IMODBUSMANAGER_H
