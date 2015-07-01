#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "misc/exception.hpp"
#include "misc/optiongrouper.hpp"
#include "logger/logger.hpp"
#include "logger/putter.hpp"

namespace Logger
{

#ifdef DEBUG
DEF_EXCEPTION(DebugLvlParameterException,
              "The debug level parameter is ill-formed");


class DebugOption : public OptionGrouper::Option
{
public:
    DebugOption(OptionGrouper::Group& grp)
        : Option(grp, "debug",
                 "Set debug level, in form GLOBAL_LVL,MODULE=LVL,...",
                 "LEVEL", 'd', 1, 1) {}
    void Parse(const params_t& params);
};

void DebugOption::Parse(const params_t& params)
{
    const std::string& str = params[0];
    boost::char_separator<char> sep(",");
    boost::tokenizer<boost::char_separator<char> > tokens(str, sep);

    for (const std::string& p: tokens)
    {
        size_t pos;
        if ((pos = p.find_first_of("=")) == std::string::npos)
        {
            // no "=" in the input, so it sets global debug level
            try
            {
                mgr.SetLvl(boost::lexical_cast<boost::int_fast16_t>(p));
            }
            catch (const boost::bad_lexical_cast& le)
            {
                DebugLvlParameterException e(AT);
                e.AddInfo("Description", "Invalid global level");
                e.AddInfo("boost::bad_lexical_cast.what()", le.what());
                throw e;
            }
        }
        else
        {
            try
            {
                mgr.SetLvl(p.substr(0, pos),
                           boost::lexical_cast<boost::int_fast16_t>(
                               p.substr(pos + 1, std::string::npos)));
            }
            catch (const boost::bad_lexical_cast& le)
            {
                DebugLvlParameterException e(AT);
                e.AddInfo("Description", "Invalid level for " +
                          p.substr(0, pos));
                e.AddInfo("boost::bad_lexical_cast.what()", le.what());
                throw e;
            }
        }
    }
}

class FunctionsEnablerOption : public OptionGrouper::Option
{
public:
    FunctionsEnablerOption(OptionGrouper::Group& grp)
        : Option(grp, "functions", "Print function names in logs", "", 'f', 0, 0) {}
    void Parse(const params_t&) { FunctionPutter::Enable(); }
};

static OptionGrouper::Group grp(GetOptionGrouper(), "logger", "Logger settings");
static DebugOption xx_o1(grp);
static FunctionsEnablerOption xx_o2(grp);

#endif

}
