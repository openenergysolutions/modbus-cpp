#ifndef MODBUS_READHOLDINGREGISTERSREQUEST_H
#define MODBUS_READHOLDINGREGISTERSREQUEST_H

/** @file
 * @brief Struct @ref modbus::ReadHoldingRegistersRequest
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Read Holding Registers request message
 *
 * Read the content of contiguous block of holding registers.
 *
 * This is the function code @cpp 0x03 @ce of the Modbus protocol.
 *
 * @see @ref ReadHoldingRegistersResponse, @ref ReadInputRegistersRequest
 */
struct ReadHoldingRegistersRequest
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

#endif //MODBUS_READHOLDINGREGISTERSREQUEST_H
