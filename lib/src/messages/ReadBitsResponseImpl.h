#ifndef MODBUS_READBITSRESPONSEIMPL_H
#define MODBUS_READBITSRESPONSEIMPL_H

#include <vector>
#include "modbus/messages/ReadCoilsRequest.h"
#include "modbus/messages/ReadCoilsResponse.h"
#include "modbus/messages/ReadDiscreteInputsRequest.h"
#include "modbus/messages/ReadDiscreteInputsResponse.h"
#include "messages/IResponse.h"
#include "messages/ReadBitsRequestImpl.h"

namespace modbus
{

template <uint8_t function_code, typename request_t, typename response_t>
class ReadBitsResponseImpl : public IResponse
{
public:
    static Expected<response_t> parse(const ReadBitsRequestImpl<function_code, request_t>& req,
                                      const ser4cpp::rseq_t& data);
};

extern template class ReadBitsResponseImpl<0x01, ReadCoilsRequest, ReadCoilsResponse>;
extern template class ReadBitsResponseImpl<0x02, ReadDiscreteInputsRequest, ReadDiscreteInputsResponse>;

} // namespace modbus

#endif //MODBUS_READBITSRESPONSEIMPL_H
