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
#ifndef MODBUS_ICLIENTSESSIONMOCK_H
#define MODBUS_ICLIENTSESSIONMOCK_H

#include "modbus/client/IClientSession.h"

class IClientSessionMock : public modbus::IClientSession
{
public:
    IClientSessionMock();

    void shutdown() override;

    // One-time requests
    void send_request(const modbus::ReadCoilsRequest& request,
                      modbus::ResponseHandler<modbus::ReadCoilsResponse> handler) override;
    void send_request(const modbus::ReadCoilsRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadCoilsResponse> handler) override;

    void send_request(const modbus::ReadDiscreteInputsRequest& request,
                      modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler) override;
    void send_request(const modbus::ReadDiscreteInputsRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler) override;

    void send_request(const modbus::ReadHoldingRegistersRequest& request,
                      modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;
    void send_request(const modbus::ReadHoldingRegistersRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;

    void send_request(const modbus::ReadInputRegistersRequest& request,
                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;
    void send_request(const modbus::ReadInputRegistersRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;

    void send_request(const modbus::WriteSingleCoilRequest& request,
                      modbus::ResponseHandler<modbus::WriteSingleCoilResponse> handler) override;
    void send_request(const modbus::WriteSingleCoilRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteSingleCoilResponse> handler) override;

    void send_request(const modbus::WriteSingleRegisterRequest& request,
                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler) override;
    void send_request(const modbus::WriteSingleRegisterRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler) override;

    void send_request(const modbus::WriteMultipleCoilsRequest& request,
                      modbus::ResponseHandler<modbus::WriteMultipleCoilsResponse> handler) override;
    void send_request(const modbus::WriteMultipleCoilsRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteMultipleCoilsResponse> handler) override;

    void send_request(const modbus::WriteMultipleRegistersRequest& request,
                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler) override;
    void send_request(const modbus::WriteMultipleRegistersRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler) override;

    // Scheduled requests
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadCoilsRequest& request,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadCoilsResponse> handler) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadCoilsRequest& request,
                                                                const modbus::duration_t& timeout,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadCoilsResponse> handler) override;

    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadDiscreteInputsRequest& request,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadDiscreteInputsRequest& request,
                                                                const modbus::duration_t& timeout,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler) override;

    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                const modbus::duration_t& timeout,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;

    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                const modbus::duration_t& timeout,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;

    std::unique_ptr<modbus::ITimer> start(const modbus::duration_t &duration, const modbus::action_t &action) override;
    std::unique_ptr<modbus::ITimer> start(const modbus::steady_time_t &expiration, const modbus::action_t &action) override;


    unsigned int get_num_read_holding_registers_request_sent() const;
    modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> get_last_read_holding_registers_request_handler() const;

private:
    unsigned int m_num_read_holding_registers_request_sent;
    modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> m_last_read_holding_registers_request_handler;
};

#endif //MODBUS_ICLIENTSESSIONMOCK_H
