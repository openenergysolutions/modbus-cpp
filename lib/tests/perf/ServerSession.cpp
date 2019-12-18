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
#include "ServerSession.h"

using namespace modbus;

Expected<ReadHoldingRegistersResponse> ServerSession::on_request(const ReadHoldingRegistersRequest& request)
{
    ReadHoldingRegistersResponse response{};
    for(size_t i = 0; i < request.qty_of_registers; ++i)
    {
        response.values.push_back(RegisterValue{static_cast<uint16_t>(request.starting_address + i), static_cast<uint16_t>(std::rand() % 0xFFFF)});
    }

    return Expected<ReadHoldingRegistersResponse>{response};
}
