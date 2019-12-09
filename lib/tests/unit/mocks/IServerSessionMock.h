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
#ifndef MODBUS_ISERVERSESSOPMMOCK_H
#define MODBUS_ISERVERSESSOPMMOCK_H

#include "trompeloeil/trompeloeil.hpp"

#include "modbus/server/IServerSession.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

class IServerSessionMock : public modbus::IServerSession
{
public:
    MAKE_MOCK0(shutdown, void(), override);

    MAKE_MOCK1(on_request, modbus::Expected<modbus::ReadCoilsResponse>(const modbus::ReadCoilsRequest&), override);
    MAKE_MOCK1(on_request, modbus::Expected<modbus::ReadDiscreteInputsResponse>(const modbus::ReadDiscreteInputsRequest&), override);
    MAKE_MOCK1(on_request, modbus::Expected<modbus::ReadHoldingRegistersResponse>(const modbus::ReadHoldingRegistersRequest&), override);
    MAKE_MOCK1(on_request, modbus::Expected<modbus::ReadInputRegistersResponse>(const modbus::ReadInputRegistersRequest&), override);

    MAKE_MOCK1(on_request, modbus::Expected<modbus::WriteSingleCoilResponse>(const modbus::WriteSingleCoilRequest&), override);
    MAKE_MOCK1(on_request, modbus::Expected<modbus::WriteSingleRegisterResponse>(const modbus::WriteSingleRegisterRequest&), override);
    MAKE_MOCK1(on_request, modbus::Expected<modbus::WriteMultipleCoilsResponse>(const modbus::WriteMultipleCoilsRequest&), override);
    MAKE_MOCK1(on_request, modbus::Expected<modbus::WriteMultipleRegistersResponse>(const modbus::WriteMultipleRegistersRequest&), override);
};

#endif //MODBUS_ISERVERSESSOPMMOCK_H
