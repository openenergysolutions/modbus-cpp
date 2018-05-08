#ifndef MODBUS_LOGGERFACTORY_H
#define MODBUS_LOGGERFACTORY_H

#include <memory>
#include <string>

namespace spdlog
{
    class logger;
}

namespace modbus
{

class Logger;

/**
 * @brief Factory of loggers
 *
 * Creates loggers using <a href="https://github.com/gabime/spdlog">spdlog</a>. Note that
 * the spdlog is completely private dependency and you don't need it, except if you compile from the sources.
 *
 * If you are already using spdlog in your application, you can use
 * @ref create_custom_logger(std::shared_ptr<spdlog::logger> custom_logger) to use an existing
 * logger.
 *
 * @see @ref IModbusManager
 */
class LoggerFactory
{
public:
    /**
     * @brief Creates a console logger
     * @param name  Name associated with the logger
     * @return Shared pointer of the logger
     *
     * The console logger will be colored and is completely thread-safe. It uses
     * @cpp spdlog::stdout_color_mt @ce under the hood.
     */
    static std::shared_ptr<Logger> create_console_logger(const std::string& name);

    /**
     * @brief Creates a logger that simply drops all the messages
     * @param name  name associated with the logger
     * @return Shared pointer of the logger
     *
     * This logger is useful if you don't want to provide logging (e.g. in tests). It simply ignores all the messages
     * passed to it. It uses a @cpp spdlog::sinks::null_sink_st @ce under the hood.
     */
    static std::shared_ptr<Logger> create_null_logger(const std::string& name);

    /**
     * @brief Creates a logger from a custom logger
     * @param custom_logger Shared pointer to a @cpp spdlog::logger @ce logger
     * @return Shared pointer of the logger
     *
     * Use this method if you already use spdlog and want to use a custom logger.
     */
    static std::shared_ptr<Logger> create_custom_logger(std::shared_ptr<spdlog::logger> custom_logger);
};

} // namespace modbus

#endif // define MODBUS_LOGGERFACTORY_H