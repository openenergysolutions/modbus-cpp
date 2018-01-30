#ifndef MODBUS_IRESPONSEMOCK_H
#define MODBUS_IRESPONSEMOCK_H

#include "modbus/messages/IResponse.h"

class IResponseMock : public modbus::IResponse
{
public:
    static modbus::Expected<bool> parse_function_code(uint8_t function_code, openpal::rseq_t& data);
};

#endif //MODBUS_IRESPONSEMOCK_H