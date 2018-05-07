#ifndef MODBUS_READREGISTERSRESPONSEIMPL_H
#define MODBUS_READREGISTERSRESPONSEIMPL_H

#include <vector>
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"
#include "messages/IResponse.h"
#include "messages/ReadRegistersRequestImpl.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
class ReadRegistersResponseImpl : public IResponse
{
public:
    static Expected<response_t> parse(const ReadRegistersRequestImpl<function_code, request_t>& req,
                                      const ser4cpp::rseq_t& data);
};

extern template class ReadRegistersResponseImpl<0x03, ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>;
extern template class ReadRegistersResponseImpl<0x04, ReadInputRegistersRequest, ReadInputRegistersResponse>;

} // namespace modbus

#endif //MODBUS_READREGISTERSRESPONSEIMPL_H
