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
#ifndef MODBUS_TRANSACTIONIDENTIFIER_H
#define MODBUS_TRANSACTIONIDENTIFIER_H

#include <cstdint>
#include <ostream>

namespace modbus
{

class TransactionIdentifier
{
public:
    explicit TransactionIdentifier(uint16_t value);

    uint16_t get_value() const;
    bool operator==(const TransactionIdentifier& other) const;
    bool operator!=(const TransactionIdentifier& other) const;
    TransactionIdentifier& operator++();
    TransactionIdentifier operator++(int);

private:
    uint16_t m_value;
};

std::ostream& operator<<(std::ostream& stream, const TransactionIdentifier& transaction_id);

} // namespace modbus

#endif //MODBUS_TRANSACTIONIDENTIFIER_H
