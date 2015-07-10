#pragma once

namespace Gl
{

template <typename T, void Func(T)>
class BasicManagedObject
{
public:
    BasicManagedObject(T t) noexcept : t{t} {}
    ~BasicManagedObject() noexcept { Func(t); }

    BasicManagedObject(BasicManagedObject&& o) : t{o.t} { o.t = 0; }
    BasicManagedObject(const BasicManagedObject&) = delete;
    void operator=(const BasicManagedObject&) = delete;

    const T& Get() const noexcept { return t; }
    T& Get() noexcept { return t; }
private:
    T t;
};

}
