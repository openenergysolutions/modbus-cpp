#ifndef MODBUS_WRITESINGLEREGISTERREQUEST_H
#define MODBUS_WRITESINGLEREGISTERREQUEST_H

#include "modbus/messages/RegisterValue.h"

namespace modbus
{

struct WriteSingleRegisterRequest
{
    RegisterValue value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERREQUEST_H
