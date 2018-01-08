// This class is heavily inspired by Andrei Alexandescu `Expected` class
// that he presented in 2012 in his talk "Systematic Error Handling in C++"
// available on Channel9: https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C

#ifndef MODBUS_EXPECTED_H
#define MODBUS_EXPECTED_H

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <typeinfo>

namespace modbus
{

template<typename T> class Expected
{
public:
    Expected(const T& value) : m_value{value}, m_has_value{true} {}

    Expected(T&& rhs) : m_value{std::move(rhs)}, m_has_value{true} {}

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

    ~Expected()
    {
        if (m_has_value) m_value.~T();
        else m_exception.~exception_ptr();
    };

    template<typename E>
    static Expected<T> from_exception(const E& exception)
    {
        if(typeid(exception) != typeid(E))
        {
            throw std::invalid_argument("slicing detected");
        }
        return from_exception(std::make_exception_ptr(exception));
    }

    static Expected<T> from_exception(std::exception_ptr exception)
    {
        Expected<T> result;
        result.m_has_value = false;
        new(&result.m_exception) std::exception_ptr(std::move(exception));
        return result;
    }

    static Expected<T> from_exception()
    {
        return from_exception(std::current_exception());
    }

    bool is_valid() const
    {
        return m_has_value;
    }

    T& get()
    {
        if(!m_has_value)
        {
            std::rethrow_exception(m_exception);
        }
        return m_value;
    }

    const T& get() const
    {
        if(!m_has_value)
        {
            std::rethrow_exception(m_exception);
        }
        return m_value;
    }

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

    std::exception_ptr get_exception() const
    {
        if (m_has_value) throw std::invalid_argument("no exception");
        return m_exception;
    }

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
