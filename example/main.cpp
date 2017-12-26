#include <iostream>

#include "modbus/IModbusManager.h"
#include "modbus/logging/LoggerFactory.h"

using namespace modbus;

int main(int argc, char* argv[])
{
    auto logger = modbus::logging::LoggerFactory().CreateConsoleLogger("Hello");
    auto modbusManager = IModbusManager::Create(logger);
    modbusManager->run();

    return 0;
}
