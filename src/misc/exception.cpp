#include "misc/exception.hpp"
#include <typeinfo>

std::string Except2String()
{
    using namespace std::string_literals;
    try
    {
        throw;
    }
    catch (const std::bad_alloc&)
    {
        return "Out of memory"s;
    }
    catch (const ExceptionBase& e)
    {
        return e.GetDesc();
    }
    catch (const std::exception& e)
    {
        return typeid(e).name() + ": "s + e.what();
    }
    catch (...)
    {
        return "Unknown exception"s;
    }
}
