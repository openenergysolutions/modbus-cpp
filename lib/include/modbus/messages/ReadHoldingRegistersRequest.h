#ifndef MODBUS_READHOLDINGREGISTERSREQUEST_H
#define MODBUS_READHOLDINGREGISTERSREQUEST_H

#include "modbus/messages/Address.h"

namespace modbus
{

struct ReadHoldingRegistersRequest
{
    Address starting_address;
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSREQUEST_H
