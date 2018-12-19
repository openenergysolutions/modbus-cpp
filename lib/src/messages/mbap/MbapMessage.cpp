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
#include "messages/mbap/MbapMessage.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

MbapMessage::MbapMessage(UnitIdentifier unit_id,
                         TransactionIdentifier transaction_id,
                         ser4cpp::rseq_t data)
        : unit_id{unit_id},
          transaction_id{transaction_id},
          data{data}
{

}

ser4cpp::rseq_t MbapMessage::build_message(UnitIdentifier unit_id,
                                           TransactionIdentifier transaction_id,
                                           const IRequest& request,
                                           ser4cpp::wseq_t& buffer)
{
    auto length = request.get_request_length();
    if(length > 253)
    {
        length = 253;
    }

    auto result = buffer.readonly().take(length + 7);

    ser4cpp::UInt16::write_to(buffer, transaction_id.get_value());
    ser4cpp::UInt16::write_to(buffer, 0x0000);
    ser4cpp::UInt16::write_to(buffer, (uint16_t)(length + 1));
    ser4cpp::UInt8::write_to(buffer, unit_id.get_value());
    request.build_request(buffer);

    return result;
}

} // namespace modbus
