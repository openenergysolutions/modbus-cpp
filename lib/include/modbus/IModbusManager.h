#ifndef MODBUS_IMODBUSMANAGER_H
#define MODBUS_IMODBUSMANAGER_H

#include <memory>

#include "modbus/logging/ILoggerSink.h"

namespace modbus
{

class IModbusManager
{
public:
    static std::unique_ptr<IModbusManager> Create(std::shared_ptr<logging::ILoggerSink> loggerSink = nullptr);

public:
    IModbusManager() = default;
    virtual ~IModbusManager() = default;

    virtual void run() = 0;
};

} // namespace modbus

#endif // MODBUS_IMODBUSMANAGER_H
