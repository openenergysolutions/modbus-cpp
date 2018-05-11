#ifndef MODBUS_READDISCRETEINPUTSREQUEST_H
#define MODBUS_READDISCRETEINPUTSREQUEST_H

/** @file
 * @brief Struct @ref modbus::ReadDiscreteInputsRequest
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Read Discrete Inputs request message
 *
 * Read the content of contiguous status of discrete inputs.
 *
 * This is the function code @cpp 0x02 @ce of the Modbus protocol.
 *
 * @see @ref ReadDiscreteInputsResponse, @ref ReadCoilsRequest
 */
struct ReadDiscreteInputsRequest
{
    /**
     * @brief Starting address to read from
     *
     * The starting address must be in a valid range of the device.
     */
    Address starting_address;

    /**
     * @brief Quantity of discrete inputs to read
     *
     * The quantity must be between 1 and 2000 inclusively and must be in a valid range of the device.
     */
    uint16_t qty_of_bits;
};

} // namespace modbus

#endif //MODBUS_READDISCRETEINPUTSREQUEST_H
