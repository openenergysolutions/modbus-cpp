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
#ifndef MODBUS_EXPECTED_H
#define MODBUS_EXPECTED_H

/** @file
 * @brief Class @ref modbus::Expected
 */

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <typeinfo>

namespace modbus
{

/**
 * @brief Expected type often used in return values and callbacks
 * @tparam T Type of the expected value
 *
 * This type represents a value that is expected to be returned, but may be impossible
 * to obtain. If @ref is_valid() returns @cpp true @ce, it means that the value was indeed
 * returned and it may be obtained by @ref get(). Otherwise, it means an error occured and the
 * value could not be returned. In this case, you can determine the type with @ref has_exception()
 * templated with the type of error it may have returned. You can also get the error with @ref get_exception().
 *
 * It is very similar to Rust's <a href="https://doc.rust-lang.org/std/result/">Result</a> type.
 * This class uses the @cpp std::exception_ptr @ce union trick to work.
 *
 * This class is heavily inspired by Andrei Alexandescu `Expected` class that he presented in 2012
 * in his talk "Systematic Error Handling in C++" available on Channel9:
 * https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C
 */
template<typename T> class Expected
{
public:
    /**
     * @brief Constructor for a valid value
     * @param value Value to be created
     *
     * This constructor is used whenever you want to create a valid value (i.e. no error) by copy.
     * Here's an example of its usage:
     *
     * @cpp
     * Expected<int> foo()
     * {
     *      return Expected<int>{42};
     * }
     * @ce
     */
    Expected(const T& value) : m_value{value}, m_has_value{true} {}

    /**
     * @brief Constructor for a valid value
     * @param rhs Value to be created
     *
     * This constructor is used whenever you want to create a valid value (i.e. no error) by move.
     */
    Expected(T&& rhs) : m_value{std::move(rhs)}, m_has_value{true} {}

    /**
     * @brief Copy contructor
     * @param rhs   Right-hand side value
     *
     * This copy constructor will create a valid value or an exception value based on the
     * right-hand side value.
     */
    Expected(const Expected& rhs)
        : m_has_value{rhs.m_has_value}
    {
        if(m_has_value)
        {
            new(&m_value) T{rhs.m_value};
        }
        else
        {
            new(&m_exception) std::exception_ptr{rhs.m_exception};
        }
    }

    /**
     * @brief Move contructor
     * @param rhs   Right-hand side value
     *
     * This copy constructor will create a valid value or an exception value based on the
     * right-hand side value.
     */
    Expected(Expected&& rhs)
        : m_has_value{rhs.m_has_value}
    {
        if(m_has_value)
        {
            new(&m_value) T{std::move(rhs.m_value)};
        }
        else
        {
            new(&m_exception) std::exception_ptr{std::move(rhs.m_exception)};
        }
    }

    /**
     * @brief Destructor
     *
     * Effectively calls the destructor of the value or of the error.
     */
    ~Expected()
    {
        if (m_has_value) m_value.~T();
        else m_exception.~exception_ptr();
    };

    /**
     * @brief Creates an error
     * @tparam E Error type (can be deduced by the parameter)
     * @param exception Error value
     * @return New error for an expectation
     *
     * This method should be used when the value could not be obtained (i.e. an error occured).
     * Here's an example usage:
     *
     * @cpp
     * Expected<int> foo()
     * {
     *      return Expected<int>::from_exception("Could not foo...");
     * }
     * @ce
     */
    template<typename E>
    static Expected<T> from_exception(const E& exception)
    {
        if(typeid(exception) != typeid(E))
        {
            throw std::invalid_argument("slicing detected");
        }
        return from_exception(std::make_exception_ptr(exception));
    }

    /**
     * @brief Creates an error from and @cpp std::exception_ptr @ce
     * @param exception Error value as an @cpp std::exception_ptr @ce
     * @return New error for an expectation
     */
    static Expected<T> from_exception(std::exception_ptr exception)
    {
        Expected<T> result;
        result.m_has_value = false;
        new(&result.m_exception) std::exception_ptr(std::move(exception));
        return result;
    }

    /**
     * @brief Check if expectation is valid
     * @return @cpp true @ce if the expectation is valid, @cpp false @ce if it contains an error
     *
     * If this method returns @cpp true @ce, the value can be obtained with @ref get(). Otherwise,
     * the error can be obtained with @ref get_exception().
     * @see @ref get(), @ref has_exception(), @ref get_exception()
     */
    bool is_valid() const
    {
        return m_has_value;
    }

    /**
     * @brief Return the contained value
     * @return Contained value
     * @throw Re-throws the exception if it contains one
     *
     * @note Always check that @ref is_valid() return @cpp true @ce before calling this method.
     * Otherwise, it may throw an exception.
     * @see @ref is_valid()
     */
    T& get()
    {
        if(!m_has_value)
        {
            std::rethrow_exception(m_exception);
        }
        return m_value;
    }

    /**
     * @brief Return the contained value
     * @return Contained value
     * @throw Re-throws the exception if it contains one
     *
     * @note Always check that @ref is_valid() return @cpp true @ce before calling this method.
     * Otherwise, it may throw an exception.
     * @see @ref is_valid()
     */
    const T& get() const
    {
        if(!m_has_value)
        {
            std::rethrow_exception(m_exception);
        }
        return m_value;
    }

    /**
     * @brief Check if the expectation contains an error of the specified type
     * @tparam E    Type of the error to check
     * @return @cpp true @ce if it contains an error of the specified type, @cpp false @ce otherwise.
     *
     * @see @ref is_valid(), @ref get_exception()
     */
    template<typename E>
    bool has_exception() const
    {
        try
        {
            if(!m_has_value) std::rethrow_exception(m_exception);
        }
        catch(const E&)
        {
            return true;
        }
        catch(...) {}
        return false;
    }

    /**
     * @brief Get the internal @cpp std::exception_ptr @ce
     * @return Internal @cpp std::exception_ptr @ce
     * @throw std::invalid_argument if the expectation is valid
     *
     * @see @ref is_valid()
     */
    std::exception_ptr get_exception() const
    {
        if (m_has_value) throw std::invalid_argument("no exception");
        return m_exception;
    }

    /**
     * @brief Get the error of the specified type
     * @tparam E Error type
     * @return Internal @cpp std::exception_ptr @ce
     * @throw std::invalid_argument if the expectation does not contain the right type
     *
     * @see @ref is_valid(), @ref has_exception()
     */
    template<typename E>
    E get_exception() const
    {
        try
        {
            if(!m_has_value) std::rethrow_exception(m_exception);
        }
        catch(const E& exception)
        {
            return exception;
        }
        catch(...) {}
        throw std::invalid_argument("not the right exception");
    }

private:
    Expected() {}

    union
    {
        T m_value;
        std::exception_ptr m_exception;
    };
    bool m_has_value;
};

}

#endif //MODBUS_EXPECTED_H
