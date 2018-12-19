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
#ifndef MODBUS_CLIENTTCPCONNECTIONMOCK_H
#define MODBUS_CLIENTTCPCONNECTIONMOCK_H

#include <memory>
#include <vector>
#include "ser4cpp/container/Buffer.h"
#include "client/channel/IClientTcpConnection.h"
#include "messages/mbap/IMbapSink.h"
#include "messages/mbap/MbapParser.h"

class ClientTcpConnectionMock : public modbus::IClientTcpConnection, public modbus::IMbapSink
{
public:
    ClientTcpConnectionMock();

    void set_listener(std::weak_ptr<modbus::IConnectionListener> listener) override;
    void send(const ser4cpp::rseq_t& data) override;
    void close() override;

    void on_mbap_message(const modbus::MbapMessage& message) override;

    std::size_t get_num_requests() const;
    const std::vector<modbus::MbapMessage>& get_requests() const;
    unsigned int get_num_close() const;

private:
    modbus::MbapParser m_parser;
    std::vector<modbus::MbapMessage> m_requests;
    std::vector<std::unique_ptr<ser4cpp::Buffer>> m_request_buffers;
    unsigned int m_num_close;
};

#endif //MODBUS_CLIENTTCPCONNECTIONMOCK_H
