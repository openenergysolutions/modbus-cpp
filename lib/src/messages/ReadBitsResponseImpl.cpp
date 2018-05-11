#include "messages/ReadBitsResponseImpl.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
Expected<response_t>
ReadBitsResponseImpl<function_code, request_t, response_t>::parse(const ReadBitsRequestImpl<function_code, request_t>& req,
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

    // Check that the number of bytes fits
    if(view.length() != length)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response length does not fit with the value in the header."});
    }

    // Check that the response has the expected number of bits
    auto expected_num_bytes = req.get_request().qty_of_bits / 8;
    if(req.get_request().qty_of_bits % 8 != 0)
    {
        ++expected_num_bytes;
    }
    if(length != expected_num_bytes)
    {
        return Expected<response_t>::from_exception(MalformedModbusResponseException{"Response does not have the expected length."});
    }

    // Extract the values
    response_t response;
    auto current_address = req.get_request().starting_address;
    uint8_t current_value;
    while(ser4cpp::UInt8::read_from(view, current_value))
    {
        auto num_bits_to_extract = req.get_request().qty_of_bits - response.values.size();
        if(num_bits_to_extract > 8)
        {
            num_bits_to_extract = 8;
        }

        for(unsigned int i = 0; i < num_bits_to_extract; ++i)
        {
            auto bit_value = static_cast<bool>((current_value >> i) & 0x01);
            response.values.emplace_back(SingleBitValue{current_address, bit_value});
            ++current_address;
        }
    }

    return Expected<response_t>(response);
}

template class ReadBitsResponseImpl<0x01, ReadCoilsRequest, ReadCoilsResponse>;
template class ReadBitsResponseImpl<0x02, ReadDiscreteInputsRequest, ReadDiscreteInputsResponse>;

} // namespace modbus
