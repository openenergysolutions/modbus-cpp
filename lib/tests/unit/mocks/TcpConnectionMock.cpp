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
#include "mocks/TcpConnectionMock.h"

TcpConnectionMock::TcpConnectionMock()
    : m_parser{this},
      m_num_close{0}
{

}

void TcpConnectionMock::set_listener(std::weak_ptr<modbus::IConnectionListener> listener)
{

}

void TcpConnectionMock::send(const ser4cpp::rseq_t& data)
{
    m_parser.parse(data);
}

void TcpConnectionMock::close()
{
    ++m_num_close;
}

void TcpConnectionMock::on_mbap_message(const modbus::MbapMessage& message)
{
    m_request_buffers.push_back(std::make_unique<ser4cpp::Buffer>(message.data));
    m_requests.emplace_back(message.unit_id, message.transaction_id, m_request_buffers.back()->as_rslice());
}

std::size_t TcpConnectionMock::get_num_requests() const
{
    return m_requests.size();
}

const std::vector<modbus::MbapMessage>& TcpConnectionMock::get_requests() const
{
    return m_requests;
}

unsigned int TcpConnectionMock::get_num_close() const
{
    return m_num_close;
}
