#ifndef MODBUS_READINPUTREGISTERSREQUEST_H
#define MODBUS_READINPUTREGISTERSREQUEST_H

#include "modbus/messages/Address.h"

namespace modbus
{

struct ReadInputRegistersRequest
{
    Address starting_address;
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSREQUEST_H
