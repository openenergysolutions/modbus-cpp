#ifndef MODBUS_READINPUTREGISTERSRESPONSE_H
#define MODBUS_READINPUTREGISTERSRESPONSE_H

#include <vector>
#include "modbus/Expected.h"
#include "modbus/messages/IResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/RegisterValue.h"

namespace modbus
{

class ReadInputRegistersResponse : public IResponse
{
public:
    static Expected<ReadInputRegistersResponse> parse(const ReadInputRegistersRequest& req,
                                                      const openpal::rseq_t& data);

public:
    ReadInputRegistersResponse() = default;

    void add_value(Address address, uint16_t value);
    const std::vector<RegisterValue>& get_values() const;

private:
    std::vector<RegisterValue> m_values;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSRESPONSE_H
