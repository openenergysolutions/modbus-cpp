#ifndef MODBUS_MBAPMESSAGE_H
#define MODBUS_MBAPMESSAGE_H

#include "openpal/container/SequenceTypes.h"
#include "modbus/channel/UnitIdentifier.h"
#include "modbus/messages/IRequest.h"
#include "channel/TransactionIdentifier.h"

namespace modbus
{

struct MbapMessage
{
    static openpal::rseq_t build_message(UnitIdentifier unit_id,
                                         TransactionIdentifier transaction_id,
                                         const IRequest& request,
                                         openpal::wseq_t& buffer);

    MbapMessage(UnitIdentifier unit_id,
                TransactionIdentifier transaction_id,
                openpal::rseq_t data);

    UnitIdentifier unit_id;
    TransactionIdentifier transaction_id;
    openpal::rseq_t data;
};

} // namespace modbus

#endif //MODBUS_MBAPMESSAGE_H
