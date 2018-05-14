/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
