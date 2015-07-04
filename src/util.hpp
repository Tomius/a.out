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

