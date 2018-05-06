#ifndef MODBUS_MODBUSMANAGERIMPL_H
#define MODBUS_MODBUSMANAGERIMPL_H

#include <mutex>
#include "exe4cpp/asio/ThreadPool.h"
#include "modbus/IModbusManager.h"

namespace modbus
{

class ModbusManagerImpl : public IModbusManager
{
public:
    explicit ModbusManagerImpl(std::shared_ptr<Logger> logger, unsigned int number_of_threads);
    ~ModbusManagerImpl();

    std::shared_ptr<IChannel> create_tcp_channel(const std::string& name,
                                                 const Ipv4Endpoint& endpoint) override;
    void shutdown() override;

private:
    std::shared_ptr<asio::io_service> m_io_service;
    std::shared_ptr<Logger> m_logger;
    exe4cpp::ThreadPool m_thread_pool;

    std::mutex m_mutex;
    std::vector<std::shared_ptr<IChannel>> m_created_channels;
};

} // namespace modbus

#endif // MODBUS_MODBUSMANAGERIMPL_H
