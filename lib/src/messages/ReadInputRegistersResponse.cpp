#include "modbus/messages/ReadInputRegistersResponse.h"

#include "openpal/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

Expected<ReadInputRegistersResponse> ReadInputRegistersResponse::parse(const ReadInputRegistersRequest& req,
                                                                       const openpal::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(0x04, view);
    if(!function_code_result.is_valid())
    {
        return Expected<ReadInputRegistersResponse>::from_exception(function_code_result.get_exception());
    }

    // Read number of bytes
    if(view.length() < 1)
    {
        return Expected<ReadInputRegistersResponse>::from_exception(MalformedModbusResponseException{"Response is too short."});
    }
    uint8_t length;
    openpal::UInt8::read_from(view, length);
    if(length % 2 != 0)
    {
        return Expected<ReadInputRegistersResponse>::from_exception(MalformedModbusResponseException{ "Response should contain an even number of register bytes." });
    }

    // Check that each number of registers match
    if(view.length() != 2 * req.get_qty_of_registers())
    {
        return Expected<ReadInputRegistersResponse>::from_exception(MalformedModbusResponseException{"Response does not contain the expected number of registers."});
    }

    // Extract the values
    ReadInputRegistersResponse response;
    auto current_address = req.get_starting_address() + 1;
    uint16_t current_value;
    while(openpal::UInt16::read_from(view, current_value))
    {
        response.add_value(current_address, current_value);
        ++current_address;
    }

    return Expected<ReadInputRegistersResponse>(response);
}

void ReadInputRegistersResponse::add_value(Address address, uint16_t value)
{
    m_values.push_back(RegisterValue {
            address,
            value
    });
}

const std::vector<RegisterValue>& ReadInputRegistersResponse::get_values() const
{
    return m_values;
}

} // namespace modbus
