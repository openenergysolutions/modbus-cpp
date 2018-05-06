#ifndef MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H
#define MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H

#include <vector>
#include "modbus/messages/IRequest.h"
#include "modbus/messages/Address.h"

namespace modbus
{

class WriteMultipleRegistersRequest : public IRequest
{
public:
    WriteMultipleRegistersRequest(Address starting_address);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    void add_register(uint16_t value);
    const Address& get_starting_address() const;

private:
    Address m_starting_address;
    std::vector<uint16_t> m_values;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H
