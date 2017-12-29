#ifndef MODBUS_REGISTERVALUE_H
#define MODBUS_REGISTERVALUE_H

#include "Address.h"

namespace modbus
{
namespace messages
{

struct RegisterValue
{
    Address address;
    uint16_t value;
};

} // namespace messages
} // modbus

#endif //MODBUS_REGISTERVALUE_H
