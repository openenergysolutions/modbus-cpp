#include "modbus/ModbusManager.h"

#include <chrono>
#include <iostream>

#include "asio.hpp"

namespace modbus
{

ModbusManager::ModbusManager()
{

}

void ModbusManager::run()
{
    asio::io_service io;

    asio::steady_timer timer1(io);
    timer1.expires_from_now(std::chrono::seconds(5));
    timer1.async_wait([] (const std::error_code& err) {
        std::cout << "The end of timer 1." << std::endl;
    });

    asio::steady_timer timer2(io);
    timer2.expires_from_now(std::chrono::seconds(2));
    timer2.async_wait([] (const std::error_code& err) {
        std::cout << "The end of timer 2." << std::endl;
    });

    io.run();
}

} // namespace modbus
