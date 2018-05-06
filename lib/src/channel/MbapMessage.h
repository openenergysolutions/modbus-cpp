#ifndef MODBUS_MBAPMESSAGE_H
#define MODBUS_MBAPMESSAGE_H

#include "ser4cpp/container/SequenceTypes.h"
#include "modbus/channel/UnitIdentifier.h"
#include "modbus/messages/IRequest.h"
#include "channel/TransactionIdentifier.h"

namespace modbus
{

struct MbapMessage
{
    static ser4cpp::rseq_t build_message(UnitIdentifier unit_id,
                                         TransactionIdentifier transaction_id,
                                         const IRequest& request,
                                         ser4cpp::wseq_t& buffer);

    MbapMessage(UnitIdentifier unit_id,
                TransactionIdentifier transaction_id,
                ser4cpp::rseq_t data);

    UnitIdentifier unit_id;
    TransactionIdentifier transaction_id;
    ser4cpp::rseq_t data;
};

} // namespace modbus

#endif //MODBUS_MBAPMESSAGE_H
