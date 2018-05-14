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
#ifndef MODBUS_IEXCEPTION_H
#define MODBUS_IEXCEPTION_H

/** @file
 * @brief Class @ref modbus::IException
 */

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
