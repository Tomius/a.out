#include <vector>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/tee.hpp>

#include "logger/filter.hpp"
#include "logger/logger.hpp"
#include "logger/putter.hpp"

namespace Logger
{

struct Pair
{
    io::filtering_ostream& stream;
    std::string str;
    int descriptor;
    bool debug;
};

LoggerMgr::LoggerMgr()
    : sstream(new std::stringstream)
#ifdef DEBUG
    , dbg_lvl(0)
#endif
{
    Pair list[] = {
        { info,  "[Info]", 1, false },
        { warn,  "[WARN]", 2, false },
        { err,   "*ERROR", 2, false },
#ifdef DEBUG
        { debug, "[ dbg]", 1, true },
#endif
    };

    BeginPutterPtr time(new TimePutter);
#ifdef DEBUG
    file_line.reset(new FileLinePutter);
    func.reset(new FunctionPutter);
    dbglvl.reset(new DebugLevelPutter);
#endif

    for (Pair& p: list)
    {
        BeginPutterPtr msg(new MessagePutter(p.str));

        std::vector<BeginPutterPtr> vec;
#if !defined(DEBUG) && !defined(POSIX)
        vec.push_back(time);
#endif
#ifdef DEBUG
        vec.push_back(file_line);
        vec.push_back(func);
#endif
        vec.push_back(msg);
#ifdef DEBUG
        if (p.debug)
            vec.push_back(dbglvl);
#endif
        p.stream.push(LineBeginFilter(vec));

// If we're on unix use stdout/stderr, and also on windows in debug mode.
#if defined(POSIX) || defined(DEBUG)
        p.stream.push(io::tee_filter<io::file_descriptor_sink>
                      (io::file_descriptor_sink(p.descriptor, io::never_close_handle)));
        // todo: file logging
        //p.stream.push(LineBeginFilter(time));
#endif
        //p.stream.push(*sstream);
        // antitodo file logging
        p.stream.push(io::null_sink());
    }
}

LoggerMgr::~LoggerMgr()
{
    AddFileOut(io::null_sink());
}

template <typename T>
void LoggerMgr::AddFileOut(const T& sink)
{
    io::filtering_ostream* x[] = {&info, &warn, &err,
#ifdef DEBUG
              &debug
#endif
    };
    if (sstream)
    {
        T s2(sink);
        s2.write(sstream->str().data(), sstream->str().length());
        sstream.reset();
    }

    for (auto& p: x)
    {
        bool ac = p->auto_close();
        try
        {
            p->set_auto_close(false);
            p->pop();
            p->push(sink);
            p->set_auto_close(ac);
        }
        catch (...)
        {
            p->set_auto_close(ac);
            throw;
        }
    }
}
// TODO: majd ha lesz physfs
//template void LoggerMgr::AddFileOut(const FileSystem::FileDevice&);
template void LoggerMgr::AddFileOut(const io::null_sink&);

#ifdef DEBUG
#define SETPOS                     \
    file_line->SetPos(file, line); \
    func->SetFunc(function)
#else
#define SETPOS
#endif

std::ostream& LoggerMgr::Info(CATPAR)
{
    SETPOS;
    return info;
}

std::ostream& LoggerMgr::Warn(CATPAR)
{
    SETPOS;
    return warn;
}

std::ostream& LoggerMgr::Err(CATPAR)
{
    SETPOS;
    return err;
}

#ifdef DEBUG

bool LoggerMgr::IsToDebug(const std::string mod, int lvl)
{
    dbglvl->SetInfo(mod, lvl);
    if (dbg_map.count(mod))
        return dbg_map[mod] > lvl;
    else
        return dbg_lvl > lvl;
}

std::ostream& LoggerMgr::Debug(CATPAR)
{
    SETPOS;
    return debug;
}

#endif

LoggerMgr mgr;

}
