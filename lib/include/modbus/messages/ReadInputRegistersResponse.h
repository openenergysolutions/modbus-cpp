#ifndef MODBUS_READINPUTREGISTERSRESPONSE_H
#define MODBUS_READINPUTREGISTERSRESPONSE_H

#include <vector>
#include "modbus/messages/RegisterValue.h"

namespace modbus
{

struct ReadInputRegistersResponse
{
    std::vector<RegisterValue> values;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSRESPONSE_H
