#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <string>
#include <cstring>

#include "logger/putter.hpp"

namespace Logger
{

#ifdef DEBUG
// FileLinePutter

/* virtual */ void FileLinePutter::operator()(std::ostream& out)
{
    size_t file_len = std::strlen(file);
    if (file_len > file_maxw)
        file_maxw = file_len;

    std::string line_s = boost::lexical_cast<std::string>(line);
    if (line_s.length() > line_maxw)
        line_maxw = line_s.length();

    out << std::setw(file_maxw) << file << ':' << std::setw(line_maxw) << line
        << ' ';
}


// FunctionPutter
/* virtual */ void FunctionPutter::operator()(std::ostream& out)
{
    if (!enabled) return;

    size_t func_len = std::strlen(func);
    if (func_len > maxw)
        maxw = func_len;

    out << '{' << std::left << std::setw(maxw) << func << "} ";
}

bool FunctionPutter::enabled = false;

// DebugLevelPutter
/* virtual */ void DebugLevelPutter::operator()(std::ostream& out)
{
    out << '(' << module << ", " << lvl << ") ";
}
#endif

// TimePutter
/* virtual */ void TimePutter::operator()(std::ostream& out)
{
    time_t t = time(NULL);
    struct tm * gmt = gmtime(&t);

    if (gmt)
    {
        const size_t STR_SIZE = 30;
        char c[STR_SIZE];
        if (strftime(c, STR_SIZE, "%Y-%m-%dT%H:%M:%S ", gmt))
            out << c;
    }
}

// MessagePutter
/* virtual */ void MessagePutter::operator()(std::ostream& out)
{
    out << msg;
}

}
