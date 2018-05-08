#ifndef MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
#define MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H

#include "modbus/messages/Address.h"

namespace modbus
{

struct WriteMultipleRegistersResponse
{
    Address starting_address;
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
