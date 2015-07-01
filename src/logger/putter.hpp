#pragma once
/**
 * @file logger/putter.hpp
 * Header file for BeginPutter and various derivatives.
 */

#include <boost/smart_ptr/intrusive_ptr.hpp>
#include "config.h"

namespace Logger
{

/// Class to print out something at the beginning of the line.
class BeginPutter
{
public:
    BeginPutter() : refcount(0) {}
    BeginPutter(const BeginPutter&) = delete;
    BeginPutter& operator=(const BeginPutter&) = delete;

    /**
     * Generate the string and send it to the specified stream. Do not flush, or
     * append newline.
     * @param str write to this stream
     */
    virtual void operator()(std::ostream& str) = 0;

    virtual  ~BeginPutter() {}

private:
    unsigned int refcount;
    friend void intrusive_ptr_add_ref(BeginPutter*);
    friend void intrusive_ptr_release(BeginPutter*);
};

/// Convience typedef
typedef boost::intrusive_ptr<BeginPutter> BeginPutterPtr;

inline void intrusive_ptr_add_ref(BeginPutter* put) { ++put->refcount; }
inline void intrusive_ptr_release(BeginPutter* put)
{ if (!--put->refcount) delete put; }

#ifdef DEBUG
/// Class that prints out current file and line
class FileLinePutter : public BeginPutter
{
public:
    FileLinePutter() : file_maxw(18), line_maxw(3), file(NULL), line(0) {};
    virtual void operator()(std::ostream& out);

    /**
     * Sets current file and line.
     * @param new_file file name.
     * @param new_line line number.
     * @note You probably never need to call this, standard logging macros do
     *       that for you.
     */
    inline void SetPos(const char * new_file,
                       unsigned new_line)
    { file = new_file; line = new_line; }
private:
    // Maximum widths
    unsigned file_maxw, line_maxw;
    /// Current file - char * for performance
    const char * file;
    /// Current line
    unsigned line;
};

/// Class that prints out the current function
class FunctionPutter : public BeginPutter
{
public:
    FunctionPutter() : maxw(60), func(NULL) {}
    virtual void operator()(std::ostream& out);

    /// Enable the putter
    static void Enable() { enabled = true; }

    /**
     * Sets the current function.
     * @param new_func function name.
     * @note You probably never need to call this, standard logging macros do
     *       that for you.
     */
    inline void SetFunc(const char * new_func)
    { func = new_func; }

private:
    /// Whether this putter enabled or not.
    static bool enabled;
    /// Maximum function name width
    unsigned maxw;
    /// Current function name
    const char * func;
};

/// Class that prints out the message's debug module and level
class DebugLevelPutter : public BeginPutter
{
public:
    virtual void operator()(std::ostream& out);

    inline void SetInfo(const std::string& module, int lvl)
    { this->module = module; this->lvl = lvl; }

private:
    std::string module;
    int lvl;
};
#endif

/// Class that prints out current date and time
class TimePutter : public BeginPutter
{
public:
    virtual void operator()(std::ostream& out);
};

/// Class that prints out a specified message
class MessagePutter : public BeginPutter
{
public:
    MessagePutter() : msg(" ") {}
    /// @param str print out that message.
    MessagePutter(const std::string& str) : msg(str + " ") {}
    virtual void operator()(std::ostream& out);

    inline void SetMsg(const std::string& str) { msg = str; }
private:
    std::string msg;
};

}
