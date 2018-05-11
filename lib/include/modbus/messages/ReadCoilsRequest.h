#ifndef MODBUS_READCOILSREQUEST_H
#define MODBUS_READCOILSREQUEST_H

/** @file
 * @brief Struct @ref modbus::ReadCoilsRequest
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Read Coils request message
 *
 * Read the content of contiguous status of coils.
 *
 * This is the function code @cpp 0x01 @ce of the Modbus protocol.
 *
 * @see @ref ReadCoilsResponse, @ref ReadDiscreteInputsRequest
 */
struct ReadCoilsRequest
{
    /**
     * @brief Starting address to read from
     *
     * The starting address must be in a valid range of the device.
     */
    Address starting_address;

    /**
     * @brief Quantity of coils to read
     *
     * The quantity must be between 1 and 2000 inclusively and must be in a valid range of the device.
     */
    uint16_t qty_of_bits;
};

} // namespace modbus

#endif //MODBUS_READCOILSREQUEST_H
