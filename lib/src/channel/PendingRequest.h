#ifndef MODBUS_PENDINGREQUEST_H
#define MODBUS_PENDINGREQUEST_H

#include <memory>
#include "exe4cpp/Typedefs.h"
#include "modbus/ResponseHandler.h"
#include "modbus/channel/UnitIdentifier.h"
#include "channel/TransactionIdentifier.h"
#include "messages/IRequest.h"

namespace modbus
{

struct PendingRequest
{
   PendingRequest(const UnitIdentifier& unit_id,
                  const TransactionIdentifier& transaction_id,
                  const IRequest& request,
                  const exe4cpp::duration_t& timeout,
                  ResponseHandler<ser4cpp::rseq_t> response_handler)
           : unit_id{unit_id},
             transaction_id{transaction_id},
             request{request.clone()},
             timeout{timeout},
             response_handler{response_handler}
    {

    }

    UnitIdentifier unit_id;
    TransactionIdentifier transaction_id;
    std::unique_ptr<IRequest> request;
    exe4cpp::duration_t timeout;
    ResponseHandler<ser4cpp::rseq_t> response_handler;
};

} // namespace modbus

#endif //MODBUS_PENDINGREQUEST_H
