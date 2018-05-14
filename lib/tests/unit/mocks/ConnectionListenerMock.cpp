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
#include "mocks/ConnectionListenerMock.h"

ConnectionListenerMock::ConnectionListenerMock()
        : m_pending_write_done{false},
          m_pending_data{false},
          m_pending_error{false},
          m_num_errors{0}
{

}

void ConnectionListenerMock::on_write_done()
{
    std::lock_guard<std::mutex> lock{m_write_done_lock};
    m_pending_write_done = true;
    m_write_done_cv.notify_all();
}

void ConnectionListenerMock::on_receive(const ser4cpp::rseq_t& data)
{
    std::lock_guard<std::mutex> lock{m_data_lock};
    m_pending_data= true;
    m_data_cv.notify_all();
}

void ConnectionListenerMock::on_error(const std::string& message)
{
    std::lock_guard<std::mutex> lock{m_error_lock};
    m_pending_error = true;
    ++m_num_errors;
    m_error_cv.notify_all();
}

bool ConnectionListenerMock::wait_for_write_done()
{
    std::unique_lock<std::mutex> lock(m_write_done_lock);

    auto result = m_write_done_cv.wait_for(lock, std::chrono::seconds(5), [=]() { return m_pending_write_done; });
    m_pending_write_done = false;

    return result;
}

bool ConnectionListenerMock::wait_for_data()
{
    std::unique_lock<std::mutex> lock(m_data_lock);

    auto result = m_data_cv.wait_for(lock, std::chrono::seconds(5), [=]() { return m_pending_data; });
    m_pending_data = false;

    return result;
}

bool ConnectionListenerMock::wait_for_error()
{
    std::unique_lock<std::mutex> lock(m_error_lock);

    auto result = m_error_cv.wait_for(lock, std::chrono::seconds(5), [=]() { return m_pending_error; });
    m_pending_error = false;

    return result;
}

unsigned int ConnectionListenerMock::get_num_errors()
{
    std::lock_guard<std::mutex> lock{m_error_lock};
    return m_num_errors;
}
