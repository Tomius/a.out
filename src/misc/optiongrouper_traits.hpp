#pragma once

#include <type_traits>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

namespace OptionGrouper
{

template <typename T, typename Crap = void>
struct Traits
{
    constexpr static const char* param_name = "PARAM";
};

template<>
struct Traits<std::string>
{
    constexpr static const char* param_name = "STRING";
};

template<>
struct Traits<char>
{
    constexpr static const char* param_name = "CHARACTER";
};

template <typename T>
struct Traits<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
    constexpr static const char* param_name = "INTEGER";
};

template<typename T>
struct Traits<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
    constexpr static const char* param_name = "FLOAT";
};

// ---------------------------------------------------------------------------

struct NoCast
{
    static const std::string& Cast(const std::string& x) { return x; }
    constexpr static const char* param_name = "STRING";
};

template <typename T, typename Crap = void>
struct LexicalCast
{
    static T Cast(const std::string& x) { return boost::lexical_cast<T>(x); }
    constexpr static const char* param_name = Traits<T>::param_name;
};

template <typename T>
struct IntCharCast
{
    static T Cast(const std::string& x)
    {
        return boost::numeric_cast<T>(boost::lexical_cast<int>(x));
    }
    constexpr static const char* param_name = "INTEGER";
};

}
