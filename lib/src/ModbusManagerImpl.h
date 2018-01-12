#ifndef MODBUS_MODBUSMANAGERIMPL_H
#define MODBUS_MODBUSMANAGERIMPL_H

#include "asiopal/ThreadPool.h"

#include "modbus/IModbusManager.h"

namespace modbus
{

class ModbusManagerImpl : public IModbusManager
{
public:
    explicit ModbusManagerImpl(std::shared_ptr<Logger> logger, unsigned int number_of_threads);
    ~ModbusManagerImpl();

    std::shared_ptr<IChannel> create_tcp_channel(const std::string& name,
                                                 const Ipv4Endpoint& endpoint,
                                                 std::unique_ptr<ISchedule> channel_retry_schedule) override;
    void shutdown() override;

private:
    std::shared_ptr<asio::io_service> m_io_service;
    std::shared_ptr<Logger> m_logger;
    openpal::ThreadPool m_thread_pool;

    std::vector<std::weak_ptr<IChannel>> m_created_channels;
};

} // namespace modbus

#endif // MODBUS_MODBUSMANAGERIMPL_H
