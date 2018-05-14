#ifndef MODBUS_READBITSREQUESTIMPL_H
#define MODBUS_READBITSREQUESTIMPL_H

#include "modbus/messages/ReadCoilsRequest.h"
#include "modbus/messages/ReadDiscreteInputsRequest.h"
#include "messages/IRequest.h"

namespace modbus
{

template <uint8_t function_code, typename request_t>
class ReadBitsRequestImpl : public IRequest
{
public:
    explicit ReadBitsRequestImpl(const request_t& request);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    const request_t& get_request() const;

private:
    request_t m_request;
};

extern template class ReadBitsRequestImpl<0x01, ReadCoilsRequest>;
extern template class ReadBitsRequestImpl<0x02, ReadDiscreteInputsRequest>;

} // namespace modbus

#endif //MODBUS_READBITSREQUESTIMPL_H
