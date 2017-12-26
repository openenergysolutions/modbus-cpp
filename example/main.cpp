#include <iostream>

#include "modbus/IModbusManager.h"
#include "modbus/logging/ILoggerSink.h"

using namespace modbus;

class ExampleLoggerSink : public logging::ILoggerSink
{
public:
    void log(const std::string& loggerName, logging::LogLevel logLevel, const std::string& msg) override
    {
        std::cout << loggerName << " says:" << msg << std::endl;
    }
};

int main(int argc, char* argv[])
{
    auto exampleLoggerSink = std::make_shared<ExampleLoggerSink>();
    auto modbusManager = IModbusManager::Create(exampleLoggerSink);
    modbusManager->run();

    return 0;
}
