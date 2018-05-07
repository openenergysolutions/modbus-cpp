#ifndef MODBUS_WRITESINGLEREGISTERREQUESTIMPL_H
#define MODBUS_WRITESINGLEREGISTERREQUESTIMPL_H

#include "modbus/messages/WriteSingleRegisterRequest.h"
#include "messages/IRequest.h"

namespace modbus
{

class WriteSingleRegisterRequestImpl : public IRequest
{
public:
    WriteSingleRegisterRequestImpl(const WriteSingleRegisterRequest& request);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    const WriteSingleRegisterRequest& get_request() const;

private:
    WriteSingleRegisterRequest m_request;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERREQUESTIMPL_H
