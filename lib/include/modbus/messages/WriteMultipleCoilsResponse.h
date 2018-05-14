#ifndef MODBUS_WRITEMULTIPLECOILSRESPONSE_H
#define MODBUS_WRITEMULTIPLECOILSRESPONSE_H

/** @file
 * @brief Struct @ref modbus::WriteMultipleCoilsResponse
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Write multiple coils response message
 *
 * Response to @ref WriteMultipleCoilsRequest.
 *
 * This is the response for function code @cpp 0x0F @ce of the Modbus protocol.
 *
 * @see @ref WriteMultipleCoilsRequest
 */
struct WriteMultipleCoilsResponse
{
    /**
     * @brief Starting address written to
     */
    Address starting_address;

    /**
     * @brief Quantity of coils forced
     */
    uint16_t qty_of_coils;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLECOILSRESPONSE_H
