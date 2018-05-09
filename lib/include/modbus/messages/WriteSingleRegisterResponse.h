#ifndef MODBUS_WRITESINGLEREGISTERRESPONSE_H
#define MODBUS_WRITESINGLEREGISTERRESPONSE_H

#include "modbus/messages/RegisterValue.h"

namespace modbus
{

/**
 * @brief Write single register response message
 *
 * Response to @ref WriteSingleRegisterRequest.
 *
 * This is the response for function code @cpp 0x06 @ce of the Modbus protocol.
 *
 * @see @ref WriteSingleRegisterRequest
 */
struct WriteSingleRegisterResponse
{
    /**
     * @brief Address of the register and the value written to
     */
    RegisterValue value;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERRESPONSE_H
