#ifndef MODBUS_WRITEMULTIPLECOILSRESPONSEIMPL_H
#define MODBUS_WRITEMULTIPLECOILSRESPONSEIMPL_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/WriteMultipleCoilsResponse.h"
#include "messages/IResponse.h"
#include "messages/WriteMultipleCoilsRequestImpl.h"

namespace modbus
{

class WriteMultipleCoilsResponseImpl : public IResponse
{
public:
    static Expected<WriteMultipleCoilsResponse> parse(const WriteMultipleCoilsRequestImpl& req,
                                                      const ser4cpp::rseq_t& data);
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLECOILSRESPONSEIMPL_H
