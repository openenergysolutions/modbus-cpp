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
#ifndef MODBUS_CONNECTIONLISTENERMOCK_H
#define MODBUS_CONNECTIONLISTENERMOCK_H

#include <condition_variable>
#include <mutex>

#include "channel/IConnectionListener.h"

class ConnectionListenerMock : public modbus::IConnectionListener
{
public:
    ConnectionListenerMock();

    void on_write_done() override;
    void on_receive(const ser4cpp::rseq_t& data) override;
    void on_error(const std::string& message) override;

    bool wait_for_write_done();

    bool wait_for_data();

    bool wait_for_error();
    unsigned int get_num_errors();

private:
    std::mutex m_write_done_lock;
    std::condition_variable m_write_done_cv;
    bool m_pending_write_done;

    std::mutex m_data_lock;
    std::condition_variable m_data_cv;
    bool m_pending_data;

    std::mutex m_error_lock;
    std::condition_variable m_error_cv;
    bool m_pending_error;
    unsigned int m_num_errors;
};

#endif //MODBUS_CONNECTIONLISTENERMOCK_H
