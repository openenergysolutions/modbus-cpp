#ifndef MODBUS_WRITESINGLECOILRESPONSE_H
#define MODBUS_WRITESINGLECOILRESPONSE_H

/** @file
 * @brief Struct @ref modbus::WriteSingleCoilResponse
 */

#include "modbus/messages/SingleBitValue.h"

namespace modbus
{

/**
 * @brief Write single coil response message
 *
 * Response to @ref WriteSingleCoilRequest.
 *
 * This is the response for function code @cpp 0x05 @ce of the Modbus protocol.
 *
 * @see @ref WriteSingleCoilRequest
 */
struct WriteSingleCoilResponse
{
    /**
     * @brief Address of the coil and the value written to
     */
    SingleBitValue value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLECOILRESPONSE_H
