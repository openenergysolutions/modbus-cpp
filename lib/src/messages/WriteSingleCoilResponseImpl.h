#ifndef MODBUS_WRITESINGLECOILRESPONSEIMPL_H
#define MODBUS_WRITESINGLECOILRESPONSEIMPL_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/WriteSingleCoilResponse.h"
#include "messages/IResponse.h"
#include "messages/WriteSingleCoilRequestImpl.h"

namespace modbus
{

class WriteSingleCoilResponseImpl : public IResponse
{
public:
    static Expected<WriteSingleCoilResponse> parse(const WriteSingleCoilRequestImpl& req,
                                                   const ser4cpp::rseq_t& data);
};

} // namespace modbus

#endif //MODBUS_WRITESINGLECOILRESPONSEIMPL_H
