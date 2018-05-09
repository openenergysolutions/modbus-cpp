#ifndef MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
#define MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H

/** @file
 * @brief Struct @ref modbus::WriteMultipleRegistersResponse
 */

#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Write multiple registers response message
 *
 * Response to @ref WriteMultipleRegistersRequest.
 *
 * This is the response for function code @cpp 0x10 @ce of the Modbus protocol.
 *
 * @see @ref WriteMultipleRegistersRequest
 */
struct WriteMultipleRegistersResponse
{
    /**
     * @brief Starting address written to
     */
    Address starting_address;

    /**
     * @brief Quantity of registers written to
     */
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSRESPONSE_H
