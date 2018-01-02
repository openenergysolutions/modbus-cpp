#ifndef MODBUS_READHOLDINGREGISTERSRESPONSE_H
#define MODBUS_READHOLDINGREGISTERSRESPONSE_H

#include <vector>

#include "modbus/messages/IResponse.h"
#include "modbus/messages/RegisterValue.h"

namespace modbus
{

class ReadHoldingRegistersResponse : public IResponse
{
public:
    ReadHoldingRegistersResponse() = default;

    void AddValue(Address address, uint16_t value);
    const std::vector<RegisterValue>& GetValues() const;

private:
    std::vector<RegisterValue> m_values;
};

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSRESPONSE_H
