#ifndef MODBUS_WRITESINGLECOILREQUESTIMPL_H
#define MODBUS_WRITESINGLECOILREQUESTIMPL_H

#include "modbus/messages/WriteSingleCoilRequest.h"
#include "messages/IRequest.h"

namespace modbus
{

class WriteSingleCoilRequestImpl : public IRequest
{
public:
    static constexpr uint16_t ON = 0xFF00;
    static constexpr uint16_t OFF = 0x0000;

    explicit WriteSingleCoilRequestImpl(const WriteSingleCoilRequest& request);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    const WriteSingleCoilRequest& get_request() const;

private:
    WriteSingleCoilRequest m_request;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLECOILREQUESTIMPL_H
