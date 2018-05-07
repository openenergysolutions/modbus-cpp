#ifndef MODBUS_READREGISTERSREQUESTIMPL_H
#define MODBUS_READREGISTERSREQUESTIMPL_H

#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "messages/IRequest.h"

namespace modbus
{

template <uint8_t function_code, typename request_t>
class ReadRegistersRequestImpl : public IRequest
{
public:
    ReadRegistersRequestImpl(const request_t& request);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    const request_t& get_request() const;

private:
    request_t m_request;
};

extern template class ReadRegistersRequestImpl<0x03, ReadHoldingRegistersRequest>;
extern template class ReadRegistersRequestImpl<0x04, ReadInputRegistersRequest>;

} // namespace modbus

#endif //MODBUS_READREGISTERSREQUESTIMPL_H
