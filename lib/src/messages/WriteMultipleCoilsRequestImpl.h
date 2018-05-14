#ifndef MODBUS_WRITEMULTIPLECOILSREQUESTIMPL_H
#define MODBUS_WRITEMULTIPLECOILSREQUESTIMPL_H

#include <vector>
#include "modbus/messages/WriteMultipleCoilsRequest.h"
#include "messages/IRequest.h"

namespace modbus
{

class WriteMultipleCoilsRequestImpl : public IRequest
{
public:
    WriteMultipleCoilsRequestImpl(const WriteMultipleCoilsRequest& request);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    const WriteMultipleCoilsRequest& get_request() const;

private:
    WriteMultipleCoilsRequest m_request;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLECOILSREQUESTIMPL_H
