#ifndef MODBUS_READHOLDINGREGISTERSRESPONSE_H
#define MODBUS_READHOLDINGREGISTERSRESPONSE_H

#include <vector>

#include "modbus/messages/RegisterValue.h"

namespace modbus
{
namespace messages
{

class ReadHoldingRegistersResponse
{
public:
    ReadHoldingRegistersResponse();

    void AddValue(Address address, uint16_t value);
    const std::vector<RegisterValue>& GetValues() const;

private:
    std::vector<RegisterValue> m_values;
};

} // namespace messages
} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSRESPONSE_H
