#include "modbus/messages/ExceptionType.h"

namespace modbus
{

ExceptionType parse_exception_type(uint8_t data)
{
    switch(data)
    {
        case 0x01:
            return ExceptionType::IllegalFunction;
        case 0x02:
            return ExceptionType::IllegalDataAddress;
        case 0x03:
            return ExceptionType::IllegalDataValue;
        case 0x04:
            return ExceptionType::ServerDeviceFailure;
        case 0x05:
            return ExceptionType::Acknowledge;
        case 0x06:
            return ExceptionType::ServerDeviceBusy;
        case 0x08:
            return ExceptionType::MemoryParityError;
        case 0x0A:
            return ExceptionType::GatewayPathUnavailable;
        case 0x0B:
            return ExceptionType::GatewayTargetDeviceFailedToRespond;
        default:
            return ExceptionType::Unknown;
    }
}

std::ostream& operator<<(std::ostream& stream, const ExceptionType& exception_type)
{
    switch(exception_type)
    {
        case ExceptionType::IllegalFunction:
            return stream << "Illegal function";
        case ExceptionType::IllegalDataAddress:
            return stream << "Illegal data address";
        case ExceptionType::IllegalDataValue:
            return stream << "Illegal data value";
        case ExceptionType::ServerDeviceFailure:
            return stream << "Server device failure";
        case ExceptionType::Acknowledge:
            return stream << "Acknowledge";
        case ExceptionType::ServerDeviceBusy:
            return stream << "Server device busy";
        case ExceptionType::MemoryParityError:
            return stream << "Memory parity error";
        case ExceptionType::GatewayPathUnavailable:
            return stream << "Gateway path unavailable";
        case ExceptionType::GatewayTargetDeviceFailedToRespond:
            return stream << "Gateway target device failed to respond";
        default:
            return stream << "Unknown";
    }
}

} // namespace modbus
