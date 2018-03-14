#ifndef MODBUS_MBAPMESSAGE_H
#define MODBUS_MBAPMESSAGE_H

#include "loopser/container/SequenceTypes.h"
#include "modbus/channel/UnitIdentifier.h"
#include "modbus/messages/IRequest.h"
#include "channel/TransactionIdentifier.h"

namespace modbus
{

struct MbapMessage
{
    static loopser::rseq_t build_message(UnitIdentifier unit_id,
                                         TransactionIdentifier transaction_id,
                                         const IRequest& request,
                                         loopser::wseq_t& buffer);

    MbapMessage(UnitIdentifier unit_id,
                TransactionIdentifier transaction_id,
                loopser::rseq_t data);

    UnitIdentifier unit_id;
    TransactionIdentifier transaction_id;
    loopser::rseq_t data;
};

} // namespace modbus

#endif //MODBUS_MBAPMESSAGE_H
