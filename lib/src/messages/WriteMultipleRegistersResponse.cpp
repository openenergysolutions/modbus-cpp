#include "modbus/messages/WriteMultipleRegistersResponse.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "modbus/exceptions/MalformedModbusResponseException.h"

namespace modbus
{

Expected<WriteMultipleRegistersResponse> WriteMultipleRegistersResponse::parse(const WriteMultipleRegistersRequest& req,
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

WriteMultipleRegistersResponse::WriteMultipleRegistersResponse(Address starting_address, uint16_t qty_of_registers)
        : m_starting_address{starting_address},
          m_qty_of_registers{qty_of_registers}
{

}

Address WriteMultipleRegistersResponse::get_starting_address() const
{
    return m_starting_address;
}

uint16_t WriteMultipleRegistersResponse::get_qty_of_registers() const
{
    return m_qty_of_registers;
}

} // namespace modbus
