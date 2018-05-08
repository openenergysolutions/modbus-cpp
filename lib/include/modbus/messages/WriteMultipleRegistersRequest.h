#ifndef MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H
#define MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H

#include <vector>
#include "modbus/messages/Address.h"

namespace modbus
{

struct WriteMultipleRegistersRequest
{
    Address starting_address;
    std::vector<uint16_t> values;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H
