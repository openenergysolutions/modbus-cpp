#ifndef MODBUS_WRITESINGLECOILREQUEST_H
#define MODBUS_WRITESINGLECOILREQUEST_H

/** @file
 * @brief Struct @ref modbus::WriteSingleCoilRequest
 */

#include "modbus/messages/SingleBitValue.h"

namespace modbus
{

/**
 * @brief Write Single Coil request message
 *
 * Write a single coil.
 *
 * This is the function code @cpp 0x05 @ce of the Modbus protocol.
 *
 * @see @ref WriteSingleCoilResponse, @ref WriteMultipleCoilsRequest
 */
    struct WriteSingleCoilRequest
    {
        /**
         * @brief Address of the coil to write and its new value
         *
         * The address must be in a valid range of the device.
         */
        SingleBitValue value;
    };

} // namespace modbus

#endif //MODBUS_WRITESINGLECOILREQUEST_H
