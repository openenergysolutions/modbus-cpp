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
#ifndef MODBUS_MBAPMESSAGE_H
#define MODBUS_MBAPMESSAGE_H

#include "ser4cpp/container/SequenceTypes.h"

#include "modbus/UnitIdentifier.h"
#include "TransactionIdentifier.h"
#include "messages/IMessage.h"

namespace modbus
{

struct MbapMessage
{
    static ser4cpp::rseq_t build_message(UnitIdentifier unit_id,
                                         TransactionIdentifier transaction_id,
                                         const IMessage& message,
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
