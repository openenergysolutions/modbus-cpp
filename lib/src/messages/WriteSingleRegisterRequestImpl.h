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
#ifndef MODBUS_WRITESINGLEREGISTERREQUESTIMPL_H
#define MODBUS_WRITESINGLEREGISTERREQUESTIMPL_H

#include "modbus/messages/WriteSingleRegisterRequest.h"
#include "messages/IMessage.h"

namespace modbus
{

class WriteSingleRegisterRequestImpl : public IMessage
{
public:
    explicit WriteSingleRegisterRequestImpl(const WriteSingleRegisterRequest& request);

    std::unique_ptr<IMessage> clone() const override;

    bool is_valid() const override;
    size_t get_message_length() const override;
    void build_message(ser4cpp::wseq_t& buffer) const override;

    const WriteSingleRegisterRequest& get_request() const;

public:
    static Expected<WriteSingleRegisterRequest> parse(const ser4cpp::rseq_t& data);

private:
    WriteSingleRegisterRequest m_request;
};

} // namespace modbus

#endif //MODBUS_WRITESINGLEREGISTERREQUESTIMPL_H
