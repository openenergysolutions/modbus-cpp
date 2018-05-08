#ifndef MODBUS_WRITEMULTIPLEREGISTERSRESPONSEIMPL_H
#define MODBUS_WRITEMULTIPLEREGISTERSRESPONSEIMPL_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"
#include "messages/IResponse.h"
#include "messages/WriteMultipleRegistersRequestImpl.h"

namespace modbus
{

class WriteMultipleRegistersResponseImpl : public IResponse
{
public:
    static Expected<WriteMultipleRegistersResponse> parse(const WriteMultipleRegistersRequestImpl& req,
                                                          const ser4cpp::rseq_t& data);
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSRESPONSEIMPL_H
