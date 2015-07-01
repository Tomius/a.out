#pragma once

#include "config.h"

#include <memory>
#include <stdexcept>
#include <map>

#include <boost/lexical_cast.hpp>

#include "logger/locmacros.hpp"
#include "logger/logger.hpp"

/// Basic exception
class ExceptionBase : public std::exception
{
public:
    ExceptionBase(CATPARC const char* what) :
#ifdef DEBUG
        line(line), file(file), function(function),
#endif
        whatstr(what) {}

    /// @return a short description of exception
    const char* what() const noexcept { return whatstr; }

    /**
     * @return a detailed description of exception (file, line, function in
     *   debug mode, and all info addedd via AddInfo()
     */
    std::string GetDesc() const
    {
        std::stringstream str;
#ifdef DEBUG
        str << "Exception " << typeid(*this).name() << " @ " << file << ":" << line
            << "\nIn function " << function << ":\n"
#else
        str << "Exception: "
#endif
            << whatstr << '\n';

        if (info)
            for (auto p: *info)
                str << '\n' << p.first << ": " << p.second;

        return str.str();
    }

    /**
     * Add some info to the exception.
     * @param name a keyword for the value. It must be unique, or it'll
     *   overwrite the previously added one with the same name.
     * @param value a description, variable value, etc for name
     */
    void AddInfo(const std::string& name, const std::string& value)
    {
        if (!info)
            info.reset(new info_t());

        (*info)[name] = value;
    }
    /**
     * A convient method for converting various types into string using
     * boost::lexical_cast. Parameters same as AddInfo() above.
     */
    template <typename Type>
    void AddInfo(const std::string& name,
                 typename boost::call_traits<Type>::param_type value)
    {
        if (!info)
            info.reset(new info_t());

        (*info)[name] = boost::lexical_cast<std::string>(value);
    }

protected:
#ifdef DEBUG
    unsigned line;
    const char* file;
    const char* function;
#endif
    const char* whatstr;
    typedef std::map<std::string, std::string> info_t;
    std::unique_ptr<info_t> info;
};

#define DEF_CUSTOM_EXCEPTION_BEG(name, super)                               \
    class name : public super                                               \
    {                                                                       \
    public:                                                                 \
    name&& AddInfo(const std::string& name, const std::string& value) &&    \
    {                                                                       \
        ::ExceptionBase::AddInfo(name, value);                              \
        return std::move(*this);                                            \
    }                                                                       \
    name& AddInfo(const std::string& name, const std::string& value) &      \
    {                                                                       \
        ::ExceptionBase::AddInfo(name, value);                              \
        return *this;                                                       \
    }                                                                       \
                                                                            \
    template <typename Type>                                                \
    name&& AddInfo(const std::string& name,                                 \
                   typename boost::call_traits<Type>::param_type value) &&  \
    {                                                                       \
        ::ExceptionBase::AddInfo<Type>(name, value);                        \
        return std::move(*this);                                            \
    }                                                                       \
    template <typename Type>                                                \
    name& AddInfo(const std::string& name,                                  \
                  typename boost::call_traits<Type>::param_type value) &    \
    {                                                                       \
        ::ExceptionBase::AddInfo<Type>(name, value);                        \
        return *this;                                                       \
    }

#define DEF_CUSTOM_EXCEPTION_END }
#define DEF_SUB_EXCEPTION_BEG(name, super, desc)                        \
    DEF_CUSTOM_EXCEPTION_BEG(name, super)                               \
        name(CATPAR) : super(FLFC desc) {}                              \
        name(CATPARC const char* what) : super(FLFC what) {}
#define DEF_SUB_EXCEPTION_END                                           \
    }
#define DEF_SUB_EXCEPTION(name, super, desc) \
    DEF_SUB_EXCEPTION_BEG(name, super, desc) \
    DEF_SUB_EXCEPTION_END

#define DEF_EXCEPTION_BEG(name, desc) \
    DEF_SUB_EXCEPTION_BEG(name, ::ExceptionBase, desc)
#define DEF_EXCEPTION_END DEF_SUB_EXCEPTION_END
#define DEF_EXCEPTION(name, desc) \
    DEF_EXCEPTION_BEG(name, desc) \
    DEF_EXCEPTION_END

#ifndef NDEBUG
#  define UNREACHABLE() \
    do { ERR << "UNREACHABLE()" << std::endl; abort(); } while(1)

#  define ASSERT(x)                                             \
    do                                                          \
    {                                                           \
        if (!(x))                                               \
        {                                                       \
            ERR << "Assertion `" #x "' failed." << std::endl;   \
            abort();                                            \
        }                                                       \
    }                                                           \
    while(0)
#else
#  ifdef __GNUC__
#    define UNREACHABLE() __builtin_unreachable()
#  else
#    define UNREACHABLE() do {} while(0)
#  endif
#  define ASSERT(x) do {} while(0)
#endif

std::string Except2String();
