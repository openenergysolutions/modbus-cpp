#ifndef MODBUS_WRITESINGLEREGISTERRESPONSE_H
#define MODBUS_WRITESINGLEREGISTERRESPONSE_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/IResponse.h"
#include "modbus/messages/RegisterValue.h"
#include "modbus/messages/WriteSingleRegisterRequest.h"

namespace modbus
{

class WriteSingleRegisterResponse : public IResponse
{
public:
    static Expected<WriteSingleRegisterResponse> parse(const WriteSingleRegisterRequest& req,
                                                       const ser4cpp::rseq_t& data);

public:
    WriteSingleRegisterResponse(const RegisterValue& value);

    RegisterValue get_value() const;

private:
    RegisterValue m_value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERRESPONSE_H
