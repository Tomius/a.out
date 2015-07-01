#include <cstring>
#include <iomanip>
#include <sstream>
#include "misc/optiongrouper.hpp"

namespace OptionGrouper
{

const char* Error::what() const throw()
{
    try
    {
        return msg.c_str();
    }
    catch (...)
    {
        return "???";
    }
}

InvalidGroup::InvalidGroup(const std::string& p)
{
    std::stringstream ss;
    ss << "Invalid group for option '" << p << "'!";
    msg = ss.str();
}

InvalidOption::InvalidOption(const std::string& p)
{
    std::stringstream ss;
    ss << "Invalid option '" << p << "'!";
    msg = ss.str();
}

AmbigousGroup::AmbigousGroup(
    const std::string& p, const std::vector<Group*>& c)
{
    std::stringstream ss;
    ss << "Ambigous group for option '" << p << "'!\n\nCandidates:";
    for (auto g: c)
        ss << "\n  --" << g->GetName() << ":...";
    msg = ss.str();
}

AmbigousOption::AmbigousOption(const std::string& p,
                               const std::vector<Option*>& c)
{
    std::stringstream ss;
    ss << "Ambigous option '" << p << "'!\n\nCandidates:";
    for (auto o: c)
        ss << "\n  --" << o->GetGroup().GetName() << ':' << o->GetName();
    msg = ss.str();
}

Option::Option(Group& group, const std::string& name,
               const std::string& desc, const std::string& params,
               char short_name, size_t min_params, size_t max_params)
    : group(group), name(name), desc(desc), params(params), short_name(short_name),
      min_params(min_params), max_params(max_params)
{
    if (short_name != 0)
        if (short_name < 32 || short_name >= 127)
            throw std::logic_error("Invalid short parameter");
    if (min_params > max_params)
        throw std::logic_error("min_params > max_params");
    group.options.push_back(this);
}

Option::~Option()
{
    group.options.remove(this);
}

Group::Group(OptionGrouper& grouper, const char * name, const char * desc)
    : grouper(grouper), name(name), desc(desc)
{
    grouper.groups.push_back(this);
}

Group::~Group()
{
    grouper.groups.remove(this);
}

void HelpOption::Parse(const params_t&)
{
    GetGroup().GetGrouper().PrintHelp();
    throw ExitSuccessfully();
}

void VersionOption::Parse(const params_t&)
{
    const OptionGrouper& gr = GetGroup().GetGrouper();
    gr.GetOut() << gr.GetVersion() << std::endl;
    throw ExitSuccessfully();
}

OptionGrouper::OptionGrouper(std::ostream& out, std::ostream& err)
    : out(out), err(err), default_group(*this, "default", "General options"),
      help_option(default_group) {}

void OptionGrouper::Parse()
{
    Option *shorts[95];
    std::memset(shorts, 0, 95 * sizeof(void *));
    for (auto grp: groups)
    {
        for (auto opt: grp->GetOptions())
        {
            if (opt->GetShortName())
            {
                if (shorts[opt->GetShortName() - 32])
                    throw std::logic_error("Short name collission");
                shorts[opt->GetShortName() - 32] = opt;
            }
        }
    }
    for (iterator_t it = arguments.begin(); it != arguments.end();
         it = ParseArg(it, shorts));
}

void OptionGrouper::Run()
{
    try
    {
        Parse();
    }
    catch (const ExitSuccessfully&)
    {
        exit(0);
    }
    catch (const Error& e)
    {
        PrintHelp();
        GetErr() << e.what() << std::endl;
        exit(-1);
    }
}

OptionGrouper::iterator_t OptionGrouper::ParseArg(
    iterator_t it, Option** shorts)
{
    size_t l = it->length();
    if (l <= 1 || (*it)[0] != '-') return ++it;
    if ((*it)[1] != '-') return ParseShort(it, shorts);
    else if (l == 2)
    {
        arguments.erase(it);
        return arguments.end();
    }
    else return ParseLong(it);
}

OptionGrouper::iterator_t OptionGrouper::ParseShort(
    iterator_t it, Option** shorts)
{
    while (it->length() > 1)
    {
        char c = (*it)[1];
        if (c < 32 || c >= 127)
            throw InvalidOption(std::string("-") + c);
        Option* o = shorts[c - 32];
        if (!o) throw InvalidOption(std::string("-") + c);
        if (o->GetMaxParams() > 0)
            return ParseParams(o, it, 2);
        else
            o->Parse(Option::params_t());
        it->erase(1, 1);
    }
    return arguments.erase(it);
}

OptionGrouper::iterator_t OptionGrouper::ParseLong(iterator_t it)
{
    size_t colon = it->find(':', 2);
    size_t equal = it->find('=', 2);

    Option* o;
    if (colon != std::string::npos && colon < equal)
    {
        Group* g = FindGroup(it->substr(2, colon - 2), *it);
        o = FindOption(g, it->substr(colon + 1, equal - colon - 2), *it);
    }
    else
    {
        try
        {
            o = FindOption(&default_group, it->substr(2, equal - 2), *it);
        }
        catch (const Error& e)
        {
            o = FindOption(NULL, it->substr(2, equal - 2), *it);
        }
    }
    return ParseParams(o, it, equal);
}

OptionGrouper::iterator_t OptionGrouper::ParseParams(
    Option* o, iterator_t it, size_t st)
{
    size_t min = o->GetMinParams();
    size_t opt = o->GetMaxParams() - o->GetMinParams();
    Option::params_t params;
    iterator_t ito = it;
    if (st && st >= it->length())
    {
        ++it;
        st = 0;
    }
    if (st)
    {
        if (o->GetMaxParams() == 0)
            throw InvalidOption(*ito);
        if ((*it)[st] == '=') ++st;
        params.push_back(it->substr(st));
        ++it;
        if (min) --min; else --opt;
    }
    for (size_t i = 0; i < min; ++i, ++it)
    {
        if (it == arguments.end()) throw InvalidOption(*ito);
        params.push_back(*it);
    }
    for (size_t i = 0; i < opt; ++i, ++it)
    {
        if (it == arguments.end()) break;
        if (it->length() >= 1 && (*it)[0] == '-') break;
        params.push_back(*it);
    }
    arguments.erase(ito, it);
    o->Parse(params);
    return it;
}

Group* OptionGrouper::FindGroup(const std::string& name, const std::string& p)
{
    std::vector<Group*> candidates;
    for (auto g: groups)
    {
        if (g->GetName().compare(0, name.length(), name) == 0)
        {
            if (g->GetName().length() == name.length())
                return g;
            else
                candidates.push_back(g);
        }
    }
    if (candidates.empty()) throw InvalidGroup(p);
    if (candidates.size() > 1) throw AmbigousGroup(p, candidates);
    return candidates[0];
}

Option* OptionGrouper::FindOption(const Group* group, const std::string& name,
                                  const std::string& p)
{
    std::vector<Option*> cmatch, camb;
    if (group)
        FindSubOption(*group, name, cmatch, camb);
    else
        for (auto g: groups)
            FindSubOption(*g, name, cmatch, camb);

    size_t n = cmatch.size() + camb.size();
    if (n == 0) throw InvalidOption(p);
    else if (cmatch.size() == 1) return cmatch[0];
    else if (camb.size() == 1) return camb[0];
    else if (cmatch.size() > 1) throw AmbigousOption(p, cmatch);
    else throw AmbigousOption(p, camb);
}

void OptionGrouper::FindSubOption(
    const Group& g, const std::string& name, std::vector<Option*>& cmatch,
    std::vector<Option*>& camb)
{
    for (auto o: g.GetOptions())
    {
        if (o->GetName().compare(0, name.length(), name) == 0)
        {
            if (o->GetName().length() == name.length())
                cmatch.push_back(o);
            else
                camb.push_back(o);
        }
    }
}

void OptionGrouper::PrintHelp() const
{
    if (!version.empty()) out << version << "\n";
    if (!header.empty()) out << header << "\n";
    size_t max_g = 0, max_n = 0, max_ps = 0;
    for (auto g: groups)
    {
        if (g != &default_group && g->GetName().length() > max_g)
            max_g = g->GetName().length();
        for (auto o: g->GetOptions())
        {
            if (o->GetName().length() > max_n)
                max_n = o->GetName().length();
            if (o->GetParamsDesc().length() > max_ps)
                max_ps = o->GetParamsDesc().length();
        }
    }

    size_t w1 = max_g + max_n + 8;
    size_t w2 = max_ps;
    for (auto g: groups)
    {
        if (g->GetDescription().empty())
            out << '\n' << g->GetName() << ":\n";
        else
            out << '\n' << g->GetDescription() << " (" << g->GetName() << "):\n";
        for (auto o: g->GetOptions())
        {
            std::stringstream ss;
            if (o->GetShortName()) ss << '-' << o->GetShortName() << ", ";
            ss << "--";
            if (g != &default_group)
                ss << g->GetName() << ':';
            ss << o->GetName();
            out << std::setw(w1) << ss.str();
            if (w2)
            {
                auto f = out.flags(std::ios::right);
                out << ' ' << std::setw(w2) << std::left << o->GetParamsDesc();
                out.flags(f);
            }
            out << ": " << o->GetDescription() << '\n';
        }
    }
}

void OptionGrouper::SetVersion(const std::string& ver, char short_option)
{
    version = ver;
    version_option.reset();
    version_option.reset(new VersionOption(default_group, short_option));
}

}
