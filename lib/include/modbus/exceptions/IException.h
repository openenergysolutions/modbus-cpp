#ifndef MODBUS_IEXCEPTION_H
#define MODBUS_IEXCEPTION_H

#include <string>

namespace modbus
{

/**
 * @brief Interface for all the exception types
 *
 * This class contains the @cpp std::string @ce of the exception message in
 * order to have copyable without any issues of strings getting destroyed.
 *
 * All the errors you can have in @ref Expected inherits from this class.
 *
 * @see @ref Expected
 */
class IException
{
public:
    /**
     * @brief Constructor
     * @param message Message describing the error
     */
    explicit IException(const std::string& message)
        : m_message{message}
    {}

    /**
     * @brief Get the error message
     * @return Error message as string
     */
    const std::string& get_message() const
    {
        return m_message;
    }

private:
    std::string m_message;
};

} // namespace modbus

#endif //MODBUS_IEXCEPTION_H
