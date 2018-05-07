#ifndef MODBUS_READHOLDINGREGISTERSRESPONSE_H
#define MODBUS_READHOLDINGREGISTERSRESPONSE_H

#include <vector>
#include "modbus/messages/RegisterValue.h"

namespace modbus
{

struct ReadHoldingRegistersResponse
{
    std::vector<RegisterValue> values;
};

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSRESPONSE_H
