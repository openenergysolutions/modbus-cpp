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
    void on_receive(const loopser::rseq_t& data) override;
    void on_error() override;

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
