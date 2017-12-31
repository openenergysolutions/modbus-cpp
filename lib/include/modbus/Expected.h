// This class is heavily inspired by Andrei Alexandescu `Expected` class
// that he presented in 2012 in his talk "Systematic Error Handling in C++"
// available on Channel9: https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C

#ifndef MODBUS_EXPECTED_H
#define MODBUS_EXPECTED_H

#include <algorithm>
#include <exception>

namespace modbus
{

template<typename T> class Expected
{
public:
    Expected(const T& value) : m_value{value}, m_hasValue{true} {}

    Expected(T&& rhs) : m_value{std::move(rhs)}, m_hasValue{true} {}

    Expected(const Expected& rhs)
        : m_value{rhs.m_value}
    {
        if(m_hasValue)
        {
            new(&m_value) T{rhs.m_value};
        }
        else
        {
            new(&m_exception) std::exception_ptr{rhs.m_exception};
        }
    }

    Expected(Expected&& rhs)
        : m_value{rhs.m_value}
    {
        if(m_hasValue)
        {
            new(&m_value) T{std::move(rhs.m_value)};
        }
        else
        {
            new(&m_exception) std::exception_ptr{std::move(rhs.m_exception)};
        }
    }

    template<typename E>
    static Expected<T> FromException(const E& exception)
    {
        if(typeid(exception) != typeid(E))
        {
            throw std::invalid_argument("slicing detected");
        }
        return FromException(std::make_exception_ptr(exception));
    }

    static Expected<T> FromException(std::exception_ptr exception)
    {
        Expected<T> result;
        result.m_hasValue = false;
        new(&result.m_exception) std::exception_ptr(std::move(exception));
        return result;
    }

    static Expected<T> FromException()
    {
        return FromException(std::current_exception());
    }

    bool IsValid() const
    {
        return m_hasValue;
    }

    T& Get()
    {
        if(!m_hasValue)
        {
            std::rethrow_exception(m_exception);
        }
        return m_value;
    }

    const T& Get() const
    {
        if(!m_hasValue)
        {
            std::rethrow_exception(m_exception);
        }
        return m_value;
    }

    template<typename E>
    bool HasException() const
    {
        try
        {
            if(!m_hasValue) std::rethrow_exception(m_exception);
        }
        catch(const E& exception)
        {
            return true;
        }
        catch(...) {}
        return false;
    }

    template<typename E>
    E GetException() const
    {
        try
        {
            if(!m_hasValue) std::rethrow_exception(m_exception);
        }
        catch(const E& exception)
        {
            return exception;
        }
        catch(...) {}
        throw std::invalid_argument("not the right exception");
    }

private:
    union
    {
        T m_value;
        std::exception_ptr m_exception;
    };
    bool m_hasValue;
};

}

#endif //MODBUS_EXPECTED_H