#ifndef MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
#define MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/IResponse.h"
#include "modbus/messages/WriteMultipleRegistersRequest.h"

namespace modbus
{

class WriteMultipleRegistersResponse : public IResponse
{
public:
    static Expected<WriteMultipleRegistersResponse> parse(const WriteMultipleRegistersRequest& req,
                                                          const openpal::rseq_t& data);

public:
    WriteMultipleRegistersResponse(Address starting_address, uint16_t qty_of_registers);

    Address get_starting_address() const;
    uint16_t get_qty_of_registers() const;

private:
    Address m_starting_address;
    uint16_t m_qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
