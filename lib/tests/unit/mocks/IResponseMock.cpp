#include "mocks/IResponseMock.h"

modbus::Expected<bool> IResponseMock::parse_function_code(uint8_t function_code, ser4cpp::rseq_t& data)
{
    return IResponse::parse_function_code(function_code, data);
}