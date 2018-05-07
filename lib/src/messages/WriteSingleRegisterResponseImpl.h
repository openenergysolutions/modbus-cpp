#ifndef MODBUS_WRITESINGLEREGISTERRESPONSEIMPL_H
#define MODBUS_WRITESINGLEREGISTERRESPONSEIMPL_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/WriteSingleRegisterResponse.h"
#include "messages/IResponse.h"
#include "messages/WriteSingleRegisterRequestImpl.h"

namespace modbus
{

class WriteSingleRegisterResponseImpl : public IResponse
{
public:
    static Expected<WriteSingleRegisterResponse> parse(const WriteSingleRegisterRequestImpl& req,
                                                           const ser4cpp::rseq_t& data);
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERRESPONSEIMPL_H
