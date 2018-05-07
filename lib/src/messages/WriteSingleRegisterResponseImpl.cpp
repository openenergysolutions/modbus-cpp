#include "messages/WriteSingleRegisterResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

Expected<WriteSingleRegisterResponse> WriteSingleRegisterResponseImpl::parse(const WriteSingleRegisterRequestImpl& req,
                                                                             const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(0x06, view);
    if(!function_code_result.is_valid())
    {
        return Expected<WriteSingleRegisterResponse>::from_exception(function_code_result.get_exception());
    }

    // Check length
    if(view.length() != 4)
    {
        return Expected<WriteSingleRegisterResponse>::from_exception(MalformedModbusResponseException{"Invalid response size."});
    }

    // Read address
    uint16_t address;
    ser4cpp::UInt16::read_from(view, address);

    // Read value
    uint16_t value;
    ser4cpp::UInt16::read_from(view, value);

    // Return result
    WriteSingleRegisterResponse response{RegisterValue{address, value}};
    return Expected<WriteSingleRegisterResponse>(response);
}

} // namespace modbus
