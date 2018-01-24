#include "modbus/messages/IResponse.h"

#include "openpal/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"

namespace modbus
{

Expected<bool> IResponse::parse_function_code(uint8_t function_code, openpal::rseq_t& data)
{
    // Check length for at least a function code
    if(data.length() < 1)
    {
        return Expected<bool>::from_exception(MalformedModbusResponseException{"Response is empty."});
    }

    uint8_t read_function_code;
    openpal::UInt8::read_from(data, read_function_code);

    // Check if the function code matches the expected one (ignoring the MSB)
    if((read_function_code & 0x7F) != function_code)
    {
        return Expected<bool>::from_exception(MalformedModbusResponseException{"Wrong function code."});
    }

    // If we have a Modbus exception
    if((read_function_code & 0x80) != 0)
    {
        if(data.length() != 1)
        {
            return Expected<bool>::from_exception(MalformedModbusResponseException{"Modbus exception should have a single byte of data."});
        }

        uint8_t exception_code;
        openpal::UInt8::read_from(data, exception_code);
        auto exception_type = parse_exception_type(exception_code);
        return Expected<bool>::from_exception(ModbusException{exception_type});
    }

    return Expected<bool>{true};
}

} // namespace modbus
