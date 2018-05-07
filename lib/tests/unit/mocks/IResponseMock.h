#ifndef MODBUS_IRESPONSEMOCK_H
#define MODBUS_IRESPONSEMOCK_H

#include "messages/IResponse.h"

class IResponseMock : public modbus::IResponse
{
public:
    static modbus::Expected<bool> parse_function_code(uint8_t function_code, ser4cpp::rseq_t& data);
};

#endif //MODBUS_IRESPONSEMOCK_H
