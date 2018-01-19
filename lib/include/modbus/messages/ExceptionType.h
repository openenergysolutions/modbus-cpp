#ifndef MODBUS_EXCEPTIONTYPE_H
#define MODBUS_EXCEPTIONTYPE_H

#include <ostream>

namespace modbus
{

enum class ExceptionType
{
    IllegalFunction = 0x01,
    IllegalDataAddress = 0x02,
    IllegalDataValue = 0x03,
    ServerDeviceFailure = 0x04,
    Acknowledge = 0x05,
    ServerDeviceBusy = 0x06,
    MemoryParityError = 0x07,
    GatewayPathUnavailable = 0x0A,
    GatewayTargetDeviceFailedToRespond = 0x0B
};

std::ostream& operator<<(std::ostream& stream, const ExceptionType& exception_type);

} // namespace modbus

#endif //MODBUS_EXCEPTIONTYPE_H
