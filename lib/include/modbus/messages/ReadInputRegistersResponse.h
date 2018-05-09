#ifndef MODBUS_READINPUTREGISTERSRESPONSE_H
#define MODBUS_READINPUTREGISTERSRESPONSE_H

#include <vector>
#include "modbus/messages/RegisterValue.h"

namespace modbus
{

/**
 * @brief Read Input Registers response message
 *
 * Response to @ref ReadInputRegistersRequest.
 *
 * This is the response for function code @cpp 0x04 @ce of the Modbus protocol.
 *
 * @see @ref ReadInputRegistersRequest
 */
struct ReadInputRegistersResponse
{
    /**
     * @brief Register values read on the device
     *
     * Each value contains both the address and the value of the register.
     */
    std::vector<RegisterValue> values;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSRESPONSE_H
