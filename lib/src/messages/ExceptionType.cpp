#include "modbus/messages/ExceptionType.h"

namespace modbus
{

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
            return stream << "Unknown error";
    }
}

} // namespace modbus
