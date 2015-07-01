#pragma once

#include "config.h"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <iostream>
#include <memory>
#ifdef DEBUG
#include <map>
#endif

#include "logger/locmacros.hpp"

namespace io = boost::iostreams;

namespace Logger
{

class FileLinePutter;
class FunctionPutter;
class DebugLevelPutter;

/// Manages logging streams
class LoggerMgr
{
public:
    LoggerMgr();
    ~LoggerMgr();

    template <typename T>
    void AddFileOut(const T& sink);

    // Gets the output streams with position updated.
    std::ostream& Info(CATPAR);
    std::ostream& Warn(CATPAR);
    std::ostream& Err(CATPAR);

#ifdef DEBUG
    /**
     * Set global debug level
     * @param lvl the new debug level
     */
    inline void SetLvl(int lvl) { dbg_lvl = lvl; }
    /**
     * Set debug level for a specified module.
     * @param mod name of the module.
     * @paran lvl new debug level.
     */
    inline void SetLvl(const std::string mod, int lvl)
    { dbg_map[mod] = lvl; }

    /**
     * Takes a module and a level, and checks if you should print the debug
     * text or not.
     * @param mod name of the module.
     * @param lvl level of the message.
     * @return true if you should print it.
     */
    bool IsToDebug(const std::string mod, int lvl);
    std::ostream& Debug(CATPAR);
#endif //DEBUG

private:
    io::filtering_ostream info, warn, err;
    std::unique_ptr<std::stringstream> sstream;

#ifdef DEBUG
    io::filtering_ostream debug;
    int dbg_lvl;
    std::map<std::string, int> dbg_map;

    boost::intrusive_ptr<FileLinePutter> file_line;
    boost::intrusive_ptr<FunctionPutter> func;
    boost::intrusive_ptr<DebugLevelPutter> dbglvl;
#endif

};

extern LoggerMgr mgr;

}

#ifdef DEBUG
#define DBG(m, l) ::Logger::mgr.IsToDebug((m), (l)) && ::Logger::mgr.Debug(AT)
#else
#define DBG(m, l) while (false) std::cout
#endif

#define INFO ::Logger::mgr.Info(AT)
#define WARN ::Logger::mgr.Warn(AT)
#define ERR  ::Logger::mgr.Err (AT)
