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
#include "mocks/IServerChannelImplMock.h"

using namespace modbus;

IServerChannelImplMock::IServerChannelImplMock()
    : m_num_messages_received{0}
{

}

void IServerChannelImplMock::start()
{

}

void IServerChannelImplMock::shutdown()
{

}

void IServerChannelImplMock::add_session(const UnitIdentifier& unit_identifier, std::shared_ptr<IServerSession> session)
{

}

void IServerChannelImplMock::on_mbap(const MbapMessage& message, ITcpConnection& connection)
{
    m_last_message_received = std::make_unique<MbapMessage>(message);
    ++m_num_messages_received;
}

MbapMessage& IServerChannelImplMock::get_last_message_received() const
{
    return *m_last_message_received;
}

size_t IServerChannelImplMock::get_num_messages_received() const
{
    return m_num_messages_received;
}
