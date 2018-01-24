#include "modbus/messages/ReadHoldingRegistersResponse.h"

#include "openpal/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

Expected<ReadHoldingRegistersResponse> ReadHoldingRegistersResponse::parse(const ReadHoldingRegistersRequest& req,
                                                                           const openpal::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(0x03, view);
    if(!function_code_result.is_valid())
    {
        return Expected<ReadHoldingRegistersResponse>::from_exception(function_code_result.get_exception());
    }

    // Check that each number of registers match
    if(view.length() != 2 * req.get_qty_of_registers())
    {
        return Expected<ReadHoldingRegistersResponse>::from_exception(MalformedModbusResponseException{"Response does not contain the expected number of registers."});
    }

    // Extract the values
    ReadHoldingRegistersResponse response;
    auto current_address = req.get_starting_address() + 1;
    uint16_t current_value;
    while(openpal::UInt16::read_from(view, current_value))
    {
        response.add_value(current_address, current_value);
        ++current_address;
    }

    return Expected<ReadHoldingRegistersResponse>(response);
}

void ReadHoldingRegistersResponse::add_value(Address address, uint16_t value)
{
    m_values.push_back(RegisterValue {
        address,
        value
    });
}

const std::vector<RegisterValue>& ReadHoldingRegistersResponse::get_values() const
{
    return m_values;
}

} // namespace modbus
