#include "messages/ReadRegistersResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
Expected<response_t>
ReadRegistersResponseImpl<function_code, request_t, response_t>::parse(const ReadRegistersRequestImpl<function_code, request_t>& req,
                                                                       const ser4cpp::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(function_code, view);
    if(!function_code_result.is_valid())
    {
        return Expected<response_t>::from_exception(function_code_result.get_exception());
    }

    // Read number of bytes
    if(view.length() < 1)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response is too short."});
    }
    uint8_t length;
    ser4cpp::UInt8::read_from(view, length);
    if(length % 2 != 0)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{ "Response should contain an even number of register bytes." });
    }

    // Check that each number of registers match
    if(view.length() != 2 * req.get_request().qty_of_registers)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response does not contain the expected number of registers."});
    }

    // Extract the values
    response_t response;
    auto current_address = req.get_request().starting_address;
    uint16_t current_value;
    while(ser4cpp::UInt16::read_from(view, current_value))
    {
        response.values.emplace_back(RegisterValue{current_address, current_value});
        ++current_address;
    }

    return Expected<response_t>(response);
}

template class ReadRegistersResponseImpl<0x03, ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>;
template class ReadRegistersResponseImpl<0x04, ReadInputRegistersRequest, ReadInputRegistersResponse>;

} // namespace modbus
