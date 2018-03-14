#include "modbus/messages/ReadRegistersResponse.h"

#include "loopser/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

template <uint8_t function_code>
Expected<ReadRegistersResponse<function_code>> ReadRegistersResponse<function_code>::parse(const ReadRegistersRequest<function_code>& req,
                                                                                           const loopser::rseq_t& data)
{
    auto view = data;

    // Check function code and Modbus exceptions
    auto function_code_result = parse_function_code(function_code, view);
    if(!function_code_result.is_valid())
    {
        return Expected<ReadRegistersResponse<function_code>>::from_exception(function_code_result.get_exception());
    }

    // Read number of bytes
    if(view.length() < 1)
    {
        return Expected<ReadRegistersResponse<function_code>>::from_exception(MalformedModbusResponseException{"Response is too short."});
    }
    uint8_t length;
    loopser::UInt8::read_from(view, length);
    if(length % 2 != 0)
    {
        return Expected<ReadRegistersResponse<function_code>>::from_exception(MalformedModbusResponseException{ "Response should contain an even number of register bytes." });
    }

    // Check that each number of registers match
    if(view.length() != 2 * req.get_qty_of_registers())
    {
        return Expected<ReadRegistersResponse<function_code>>::from_exception(MalformedModbusResponseException{"Response does not contain the expected number of registers."});
    }

    // Extract the values
    ReadRegistersResponse<function_code> response;
    auto current_address = req.get_starting_address();
    uint16_t current_value;
    while(loopser::UInt16::read_from(view, current_value))
    {
        response.add_value(current_address, current_value);
        ++current_address;
    }

    return Expected<ReadRegistersResponse<function_code>>(response);
}

template <uint8_t function_code>
void ReadRegistersResponse<function_code>::add_value(Address address, uint16_t value)
{
    m_values.push_back(RegisterValue {
            address,
            value
    });
}

template <uint8_t function_code>
const std::vector<RegisterValue>& ReadRegistersResponse<function_code>::get_values() const
{
    return m_values;
}

template class ReadRegistersResponse<0x03>;
template class ReadRegistersResponse<0x04>;

} // namespace modbus
