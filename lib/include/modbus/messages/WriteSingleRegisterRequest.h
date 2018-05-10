#ifndef MODBUS_WRITESINGLEREGISTERREQUEST_H
#define MODBUS_WRITESINGLEREGISTERREQUEST_H

/** @file
 * @brief Struct @ref modbus::WriteSingleRegisterRequest
 */

#include "modbus/messages/RegisterValue.h"

namespace modbus
{

/**
 * @brief Write Single Register request message
 *
 * Write a single holding register.
 *
 * This is the function code @cpp 0x06 @ce of the Modbus protocol.
 *
 * @see @ref WriteSingleRegisterResponse, @ref WriteMultipleRegistersRequest
 */
struct WriteSingleRegisterRequest
{
    /**
     * @brief Address of the register to write and its new value
     *
     * The address must be in a valid range of the device.
     */
    RegisterValue value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERREQUEST_H
