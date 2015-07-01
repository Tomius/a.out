#pragma once

#include <cstddef>
#include <boost/preprocessor/repetition/enum.hpp>

namespace FilestripImpl
{
constexpr size_t SrcAt(const char* x)
{
    for (size_t i = 5; x[i]; ++i)
        if (x[i-5] == '/' && x[i-4] == 's' && x[i-3] == 'r' && x[i-2] == 'c' &&
            x[i-1] == '/')
            return i;
    return 0;
}

template <size_t N>
constexpr size_t StrLen(const char (&)[N]) { return N-1; }

template <size_t N>
constexpr char StrAt(const char (&x)[N], size_t i)
{
    if (i >= N) return 0;
    else return x[i];
}

template <size_t Len, char... S>
struct String
{
    template <char SN> using Add = String<Len+1, S..., SN>;
    static constexpr const char str[Len+1] = { S..., 0 };
};
template <size_t Len, char... S>
constexpr const char String<Len, S...>::str[Len+1];

template <typename Str, size_t Len, char S0, char... S>
struct ToString
{
    typedef typename ToString<typename Str::template Add<S0>, Len-1, S...>::type type;
};
template <typename Str, char S0, char... S>
struct ToString<Str, 0, S0, S...>
{
    typedef Str type;
};

template <size_t Start, size_t Len, char S0, char... S>
struct OffsString
{
    typedef typename OffsString<Start-1, Len-1, S...>::type type;
};
template <size_t Len, char S0, char... S>
struct OffsString<0, Len, S0, S...>
{
    typedef typename ToString<String<0>, Len, S0, S...>::type type;
};

#define FILESTRIP_IMPL_ATMACRO(z,n,data) FilestripImpl::StrAt(__FILE__, n)
#define PRETTY_FILE                                                        \
    FilestripImpl::OffsString<                                             \
        FilestripImpl::SrcAt(__FILE__), FilestripImpl::StrLen(__FILE__),   \
        BOOST_PP_ENUM(100, FILESTRIP_IMPL_ATMACRO, x)>::type::str
}
