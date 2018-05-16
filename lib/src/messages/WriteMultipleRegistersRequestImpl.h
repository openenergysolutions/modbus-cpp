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
#ifndef MODBUS_WRITEMULTIPLEREGISTERSREQUESTIMPL_H
#define MODBUS_WRITEMULTIPLEREGISTERSREQUESTIMPL_H

#include <vector>
#include "modbus/messages/WriteMultipleRegistersRequest.h"
#include "messages/IRequest.h"

namespace modbus
{

class WriteMultipleRegistersRequestImpl : public IRequest
{
public:
    WriteMultipleRegistersRequestImpl(const WriteMultipleRegistersRequest& request);

    std::unique_ptr<IRequest> clone() const override;

    bool is_valid() const override;
    size_t get_request_length() const override;
    void build_request(ser4cpp::wseq_t& buffer) const override;

    const WriteMultipleRegistersRequest& get_request() const;

private:
    WriteMultipleRegistersRequest m_request;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSREQUESTIMPL_H
