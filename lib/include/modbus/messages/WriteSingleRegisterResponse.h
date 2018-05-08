#ifndef MODBUS_WRITESINGLEREGISTERRESPONSE_H
#define MODBUS_WRITESINGLEREGISTERRESPONSE_H

#include "modbus/messages/RegisterValue.h"

namespace modbus
{

struct WriteSingleRegisterResponse
{
    RegisterValue value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERRESPONSE_H
