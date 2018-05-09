#ifndef MODBUS_READHOLDINGREGISTERSRESPONSE_H
#define MODBUS_READHOLDINGREGISTERSRESPONSE_H

#include <vector>
#include "modbus/messages/RegisterValue.h"

namespace modbus
{

/**
 * @brief Read Holding Registers response message
 *
 * Response to @ref ReadHoldingRegistersRequest.
 *
 * This is the response for function code @cpp 0x03 @ce of the Modbus protocol.
 *
 * @see @ref ReadHoldingRegistersRequest
 */
struct ReadHoldingRegistersResponse
{
    /**
     * @brief Register values read on the device
     *
     * Each value contains both the address and the value of the register.
     */
    std::vector<RegisterValue> values;
};

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSRESPONSE_H
