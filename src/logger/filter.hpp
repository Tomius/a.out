#pragma once

/**
 * @file logger/filter.hpp
 * Header for filters used in logging
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>

#include "logger/putter.hpp"

namespace io = boost::iostreams;

namespace Logger {

class LineBeginFilter : public io::output_filter,
                        public std::vector<BeginPutterPtr>
{
public:
    LineBeginFilter() : is_newline(true) {}
    LineBeginFilter(BeginPutterPtr putter) : is_newline(true)
    {
        push_back(putter);
    }
    LineBeginFilter(std::vector<BeginPutterPtr>& vec)
        : std::vector<BeginPutterPtr>(vec), is_newline(true) {}

    template<typename Sink>
    bool put(Sink& dst, int c)
    {
        if (is_newline)
        {
            std::stringstream stream(std::ios_base::out);
            for(iterator it = begin(); it != end(); ++it)
                (**it)(stream);

            std::string str = stream.str();
            io::write(dst, str.data(), str.length());
        }
        is_newline = (c == '\n');

        return io::put(dst, c);
    }

    template<typename Sink>
    void close(Sink&) { is_newline = true; }

private:
    bool is_newline;
};

}
