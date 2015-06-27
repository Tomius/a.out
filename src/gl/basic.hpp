#ifndef UUID_5CFF6400_2957_4757_BC8F_CB8F2BB64635
#define UUID_5CFF6400_2957_4757_BC8F_CB8F2BB64635
#pragma once

namespace Gl
{

template <typename T, void Func(T)>
class BasicManagedObject
{
public:
    BasicManagedObject(T t) noexcept : t{t} {}
    ~BasicManagedObject() noexcept { Func(t); }

    const T& Get() const noexcept { return t; }
    T& Get() noexcept { return t; }
private:
    T t;
};

}

#endif
