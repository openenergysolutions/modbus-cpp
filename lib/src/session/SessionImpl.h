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
#ifndef MODBUS_SESSIONIMPL_H
#define MODBUS_SESSIONIMPL_H

#include <memory>
#include <vector>
#include "exe4cpp/IExecutor.h"
#include "modbus/channel/UnitIdentifier.h"
#include "modbus/session/IScheduledRequest.h"
#include "modbus/session/ISession.h"

namespace modbus
{

class Logger;
class IChannelImpl;
class UnitIdentifier;

class SessionImpl final : public ISession
{
public:
    SessionImpl(std::shared_ptr<exe4cpp::IExecutor> executor,
                std::shared_ptr<Logger> logger,
                std::shared_ptr<IChannelImpl> channel,
                const UnitIdentifier& unit_identifier,
                const duration_t& default_timeout);
    ~SessionImpl() = default;

    void shutdown() override;

    // One-time requests
    void send_request(const ReadCoilsRequest& request,
                      ResponseHandler<ReadCoilsResponse> handler) override;
    void send_request(const ReadCoilsRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<ReadCoilsResponse> handler) override;

    void send_request(const ReadDiscreteInputsRequest& request,
                      ResponseHandler<ReadDiscreteInputsResponse> handler) override;
    void send_request(const ReadDiscreteInputsRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<ReadDiscreteInputsResponse> handler) override;

    void send_request(const ReadHoldingRegistersRequest& request,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;
    void send_request(const ReadHoldingRegistersRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;

    void send_request(const ReadInputRegistersRequest& request,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;
    void send_request(const ReadInputRegistersRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;

    void send_request(const WriteSingleCoilRequest& request,
                      ResponseHandler<WriteSingleCoilResponse> handler) override;
    void send_request(const WriteSingleCoilRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<WriteSingleCoilResponse> handler) override;

    void send_request(const WriteSingleRegisterRequest& request,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;
    void send_request(const WriteSingleRegisterRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;

    void send_request(const WriteMultipleCoilsRequest& request,
                      ResponseHandler<WriteMultipleCoilsResponse> handler) override;
    void send_request(const WriteMultipleCoilsRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<WriteMultipleCoilsResponse> handler) override;

    void send_request(const WriteMultipleRegistersRequest& request,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;
    void send_request(const WriteMultipleRegistersRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;


    // Scheduled requests
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadCoilsRequest& request,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadCoilsResponse> handler) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadCoilsRequest& request,
                                                        const duration_t& timeout,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadCoilsResponse> handler) override;

    std::shared_ptr<IScheduledRequest> schedule_request(const ReadDiscreteInputsRequest& request,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadDiscreteInputsResponse> handler) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadDiscreteInputsRequest& request,
                                                        const duration_t& timeout,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadDiscreteInputsResponse> handler) override;

    std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadHoldingRegistersResponse> handler) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                        const duration_t& timeout,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadHoldingRegistersResponse> handler) override;

    std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadInputRegistersResponse> handler) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                        const duration_t& timeout,
                                                        const duration_t& frequency,
                                                        ResponseHandler<ReadInputRegistersResponse> handler) override;


    std::unique_ptr<ITimer> start(const duration_t& duration, const action_t& action) override;
    std::unique_ptr<ITimer> start(const steady_time_t& expiration, const action_t& action) override;

private:
    template<typename TRequestImpl, typename TResponseImpl, typename TRequest, typename TResponse>
    void meta_send_request(const TRequest& request,
                           const duration_t& timeout,
                           ResponseHandler<TResponse> handler);

    template<typename TRequest, typename TResponse>
    std::shared_ptr<IScheduledRequest> meta_schedule_request(const TRequest& request,
                                                             const duration_t& timeout,
                                                             const duration_t& frequency,
                                                             ResponseHandler<TResponse> handler);

    std::shared_ptr<exe4cpp::IExecutor> m_executor;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<IChannelImpl> m_channel;
    UnitIdentifier m_unit_identifier;
    duration_t m_default_timeout;

    std::vector<std::shared_ptr<IScheduledRequest>> m_scheduled_requests;
};

} // namespace modbus

#endif //MODBUS_SESSIONIMPL_H
