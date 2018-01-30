#ifndef MODBUS_WRITESINGLEREGISTERREQUEST_H
#define MODBUS_WRITESINGLEREGISTERREQUEST_H

#include "modbus/messages/IRequest.h"
#include "modbus/messages/Address.h"

namespace modbus
{

class WriteSingleRegisterRequest : public IRequest
{
public:
    WriteSingleRegisterRequest(Address address, uint16_t value);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(openpal::wseq_t& buffer) const override;

    const Address& get_address() const;
    uint16_t get_value() const;

private:
    Address m_address;
    uint16_t m_value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERREQUEST_H
