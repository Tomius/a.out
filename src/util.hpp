#pragma once

#include <memory>

/**
 * Replacement std::make_unique that doesn't value initialize array members.
 * @tparam T make a std::unique_ptr out of this. It should be an array of
 *  unspecified size (e.g. `int[]`).
 * @param n length of the array to make.
 * @return an std::unique_ptr to the allocated array.
 * @throws std::bad_alloc if there's not enough memory.
 * @throws anything that T's constructor throws.
 */
template<class T>
inline std::unique_ptr<T> make_unique(size_t n)
{
    return std::unique_ptr<T>{new typename std::remove_extent<T>::type[n]};
}

/**
 * Class that executes something in the destructor.
 * See AtScopeExit() for usage.
 */
template <typename T>
class AtScopeExitT
{
    const T& func;
    bool call = true;
public:
    explicit AtScopeExitT(const T& func) noexcept : func(func) {}
    AtScopeExitT(const AtScopeExitT&) = delete;
    AtScopeExitT(AtScopeExitT&& o) noexcept : func(o.func) { o.call = false; }
    ~AtScopeExitT() noexcept(noexcept(func())) { if (call) func(); }
};

/**
 * Execute a function when the variable is destroyed.
 * Designed as a simpler, macro less, C++11 alternate to
 * [http://www.boost.org/doc/libs/1_57_0/libs/scope_exit/doc/html/index.html](Boost.ScopeExit),
 * this allows you to execute arbitrary code when a variable goes out of
 * scope. It can be used when writing a RAII class to manage some resource would
 * be overkill.
 * @par Usage
 * @code{.cpp}
 * {
 *     auto something = InitSomething()
 *     auto x = AtScopeExit([&]() { DeinitSomething(something); });
 *
 *     DoSomething(something);
 *     // ...
 * } // DeinitSomething (or earlier in case of an exception)
 * @endcode
 * @warning The function you give should be `noexcept`, because it'll executed
 *     in a destructor, and destructors [shouldn't throw during stack
 *     unwinding](https://isocpp.org/wiki/faq/exceptions#dtors-shouldnt-throw).
 */
template <typename T>
AtScopeExitT<T> AtScopeExit(const T& func) noexcept
{ return AtScopeExitT<T>(func); }
