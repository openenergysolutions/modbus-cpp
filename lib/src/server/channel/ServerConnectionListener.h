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
#ifndef MODBUS_SERVERCONNECTIONLISTENER_H
#define MODBUS_SERVERCONNECTIONLISTENER_H

#include <memory>

#include "IConnectionListener.h"
#include "IServerChannelImpl.h"
#include "ITcpConnection.h"
#include "messages/mbap/IMbapSink.h"
#include "messages/mbap/MbapParser.h"

namespace modbus
{

class ServerConnectionListener : public IConnectionListener, public IMbapSink
{
public:
    ServerConnectionListener(std::shared_ptr<IServerChannelImpl> channel,
                             std::shared_ptr<ITcpConnection> connection);

    // IConnectionListener
    void on_write_done() override;
    void on_receive(const ser4cpp::rseq_t& data) override;
    void on_error(const std::string& message) override;

    // IMbapSink
    void on_mbap_message(const MbapMessage& message) override;

private:
    std::shared_ptr<IServerChannelImpl> m_channel;
    std::shared_ptr<ITcpConnection> m_connection;
    MbapParser m_parser;
};

} // namespace modbus

#endif //MODBUS_SERVERCONNECTIONLISTENER_H
