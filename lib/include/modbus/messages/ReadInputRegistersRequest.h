#ifndef MODBUS_READINPUTREGISTERSREQUEST_H
#define MODBUS_READINPUTREGISTERSREQUEST_H

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Read Input Registers request message
 *
 * Read the content of contiguous block of input registers.
 *
 * This is the function code @cpp 0x04 @ce of the Modbus protocol.
 *
 * @see @ref ReadInputRegistersResponse, @ref ReadHoldingRegistersRequest
 */
struct ReadInputRegistersRequest
{
    /**
     * @brief Starting address to read from
     *
     * The starting address must be in a valid range of the device.
     */
    Address starting_address;

    /**
     * @brief Quantity of registers to read
     *
     * The quantity must be between 1 and 125 inclusively and must be in a valid range of the device.
     */
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSREQUEST_H
