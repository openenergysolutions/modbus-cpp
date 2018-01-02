#ifndef MODBUS_READINPUTREGISTERSRESPONSE_H
#define MODBUS_READINPUTREGISTERSRESPONSE_H

#include "modbus/messages/IResponse.h"

namespace modbus
{

class ReadInputRegistersResponse : public IResponse
{
public:
    ReadInputRegistersResponse();
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSRESPONSE_H
