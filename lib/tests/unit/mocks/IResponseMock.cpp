#include "mocks/IResponseMock.h"

modbus::Expected<bool> IResponseMock::parse_function_code(uint8_t function_code, loopser::rseq_t& data)
{
    return IResponse::parse_function_code(function_code, data);
}