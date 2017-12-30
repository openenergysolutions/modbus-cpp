#ifndef MODBUS_REGISTERVALUE_H
#define MODBUS_REGISTERVALUE_H

#include "Address.h"

namespace modbus
{

struct RegisterValue
{
    Address address;
    uint16_t value;
};

} // modbus

#endif //MODBUS_REGISTERVALUE_H
