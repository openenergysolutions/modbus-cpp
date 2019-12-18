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
#include "mocks/IClientSessionMock.h"

IClientSessionMock::IClientSessionMock()
    :m_num_read_holding_registers_request_sent{0},
     m_last_read_holding_registers_request_handler{nullptr}
{

}

void IClientSessionMock::shutdown()
{

}

// One-time requests
void IClientSessionMock::send_request(const modbus::ReadCoilsRequest& request,
                                      modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::ReadCoilsRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::ReadDiscreteInputsRequest& request,
                                      modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::ReadDiscreteInputsRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::ReadHoldingRegistersRequest& request,
                                      modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    send_request(request, std::chrono::seconds(1), handler);
}

void IClientSessionMock::send_request(const modbus::ReadHoldingRegistersRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    ++m_num_read_holding_registers_request_sent;
    m_last_read_holding_registers_request_handler = handler;
}

void IClientSessionMock::send_request(const modbus::ReadInputRegistersRequest& request,
                                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::ReadInputRegistersRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteSingleCoilRequest& request,
                                      modbus::ResponseHandler<modbus::WriteSingleCoilResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteSingleCoilRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::WriteSingleCoilResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteSingleRegisterRequest& request,
                                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteSingleRegisterRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteMultipleCoilsRequest& request,
                                      modbus::ResponseHandler<modbus::WriteMultipleCoilsResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteMultipleCoilsRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::WriteMultipleCoilsResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteMultipleRegistersRequest& request,
                                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler)
{

}

void IClientSessionMock::send_request(const modbus::WriteMultipleRegistersRequest& request,
                                      const modbus::duration_t& timeout,
                                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler)
{

}

// Scheduled requests
std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadCoilsRequest& request,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadCoilsRequest& request,
                                                                                const modbus::duration_t& timeout,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadDiscreteInputsRequest& request,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadDiscreteInputsRequest& request,
                                                                                const modbus::duration_t& timeout,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                                const modbus::duration_t& timeout,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> IClientSessionMock::schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                                const modbus::duration_t& timeout,
                                                                                const modbus::duration_t& frequency,
                                                                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{
    return nullptr;
}

unsigned int IClientSessionMock::get_num_read_holding_registers_request_sent() const
{
    return m_num_read_holding_registers_request_sent;
}

modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> IClientSessionMock::get_last_read_holding_registers_request_handler() const
{
    return m_last_read_holding_registers_request_handler;
}

std::unique_ptr<modbus::ITimer> IClientSessionMock::start(const modbus::duration_t &duration, const modbus::action_t &action)
{
    return nullptr;
}

std::unique_ptr<modbus::ITimer> IClientSessionMock::start(const modbus::steady_time_t &expiration, const modbus::action_t &action)
{
    return nullptr;
}
