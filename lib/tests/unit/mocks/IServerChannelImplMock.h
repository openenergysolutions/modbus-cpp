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
#ifndef MODBUS_ISERVERCHANNELIMPLMOCK_H
#define MODBUS_ISERVERCHANNELIMPLMOCK_H

#include <memory>

#include "server/channel/IServerChannelImpl.h"

class IServerChannelImplMock final : public modbus::IServerChannelImpl
{
public:
    IServerChannelImplMock();

    void start() final;
    void shutdown() final;
    void add_session(const modbus::UnitIdentifier& unit_identifier, std::shared_ptr<modbus::IServerSession> session) final;
    void on_mbap(const modbus::MbapMessage& message, modbus::ITcpConnection& connection) final;

    modbus::MbapMessage& get_last_message_received() const;
    size_t get_num_messages_received() const;

private:
    std::unique_ptr<modbus::MbapMessage> m_last_message_received;
    size_t m_num_messages_received;
};

#endif //MODBUS_ISERVERCHANNELIMPLMOCK_H
