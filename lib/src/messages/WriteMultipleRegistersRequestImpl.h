#ifndef MODBUS_WRITEMULTIPLEREGISTERSREQUESTIMPL_H
#define MODBUS_WRITEMULTIPLEREGISTERSREQUESTIMPL_H

#include <vector>
#include "modbus/messages/WriteMultipleRegistersRequest.h"
#include "messages/IRequest.h"

namespace modbus
{

class WriteMultipleRegistersRequestImpl : public IRequest
{
public:
    WriteMultipleRegistersRequestImpl(const WriteMultipleRegistersRequest& request);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    const WriteMultipleRegistersRequest& get_request() const;

private:
    WriteMultipleRegistersRequest m_request;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSREQUESTIMPL_H
