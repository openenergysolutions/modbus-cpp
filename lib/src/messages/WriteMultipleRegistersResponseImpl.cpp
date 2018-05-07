#include "messages/WriteMultipleRegistersResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

Expected<WriteMultipleRegistersResponse> WriteMultipleRegistersResponseImpl::parse(const WriteMultipleRegistersRequestImpl& req,
                                                                                   const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(0x10, view);
    if(!function_code_result.is_valid())
    {
        return Expected<WriteMultipleRegistersResponse>::from_exception(function_code_result.get_exception());
    }

    // Check length
    if(view.length() != 4)
    {
        return Expected<WriteMultipleRegistersResponse>::from_exception(MalformedModbusResponseException{"Invalid response size."});
    }

    // Read address
    uint16_t starting_address;
    ser4cpp::UInt16::read_from(view, starting_address);

    // Read value
    uint16_t qty_of_registers;
    ser4cpp::UInt16::read_from(view, qty_of_registers);

    // Return result
    WriteMultipleRegistersResponse response{starting_address, qty_of_registers};
    return Expected<WriteMultipleRegistersResponse>(response);
}

} // namespace modbus
