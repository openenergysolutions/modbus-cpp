#ifndef MODBUS_READREGISTERSRESPONSE_H
#define MODBUS_READREGISTERSRESPONSE_H

#include <vector>
#include "modbus/messages/Address.h"
#include "modbus/messages/IResponse.h"
#include "modbus/messages/ReadRegistersRequest.h"
#include "modbus/messages/RegisterValue.h"

namespace modbus
{

template <uint8_t function_code>
class ReadRegistersResponse : public IResponse
{
public:
    static Expected<ReadRegistersResponse<function_code>> parse(const ReadRegistersRequest<function_code>& req,
                                                                const loopser::rseq_t& data);

public:
    ReadRegistersResponse() = default;

    void add_value(Address address, uint16_t value);
    const std::vector<RegisterValue>& get_values() const;

private:
    std::vector<RegisterValue> m_values;
};

extern template class ReadRegistersResponse<0x03>;
extern template class ReadRegistersResponse<0x04>;

} // namespace modbus

#endif //MODBUS_READREGISTERSRESPONSE_H
