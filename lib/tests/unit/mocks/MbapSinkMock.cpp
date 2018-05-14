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
#include "mocks/MbapSinkMock.h"

void MbapSinkMock::on_mbap_message(const modbus::MbapMessage& message)
{
    m_messages.push_back(message);
}

std::size_t MbapSinkMock::get_num_messages() const
{
    return m_messages.size();
}

const std::vector<modbus::MbapMessage>& MbapSinkMock::get_messages() const
{
    return m_messages;
}
