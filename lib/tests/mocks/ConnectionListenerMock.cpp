#include "mocks/ConnectionListenerMock.h"

ConnectionListenerMock::ConnectionListenerMock()
        : m_pending_data{false},
          m_num_errors{0},
          m_pending_error{false}
{

}

void ConnectionListenerMock::on_receive(const openpal::rseq_t& data)
{
    std::lock_guard<std::mutex> lock{m_data_lock};
    m_pending_data= true;
    m_data_cv.notify_all();
}

void ConnectionListenerMock::on_error()
{
    std::lock_guard<std::mutex> lock{m_error_lock};
    m_pending_error = true;
    ++m_num_errors;
    m_error_cv.notify_all();
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
