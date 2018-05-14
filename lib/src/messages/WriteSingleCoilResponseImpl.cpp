#include "messages/WriteSingleCoilResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

Expected<WriteSingleCoilResponse> WriteSingleCoilResponseImpl::parse(const WriteSingleCoilRequestImpl& req,
                                                                     const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(0x05, view);
    if(!function_code_result.is_valid())
    {
        return Expected<WriteSingleCoilResponse>::from_exception(function_code_result.get_exception());
    }

    // Check length
    if(view.length() != 4)
    {
        return Expected<WriteSingleCoilResponse>::from_exception(MalformedModbusResponseException{"Invalid response size."});
    }

    // Read address
    uint16_t address;
    ser4cpp::UInt16::read_from(view, address);

    // Read value
    uint16_t value;
    ser4cpp::UInt16::read_from(view, value);

    // Convert to boolean value
    bool bool_value;
    switch(value)
    {
        case WriteSingleCoilRequestImpl::ON:
            bool_value = true;
            break;

        case WriteSingleCoilRequestImpl::OFF:
            bool_value = false;
            break;

        default:
            return Expected<WriteSingleCoilResponse>::from_exception(MalformedModbusResponseException{"Invalid coil state."});
    }

    // Return result
    WriteSingleCoilResponse response{SingleBitValue{address, bool_value}};
    return Expected<WriteSingleCoilResponse>(response);
}

} // namespace modbus
