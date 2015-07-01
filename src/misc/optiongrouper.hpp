#pragma once

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

#include "misc/optiongrouper_traits.hpp"

namespace OptionGrouper
{
class Group;
class Option;
class OptionGrouper;

/**
 * "Exception" thrown when the parser wants to quit successfully (e.g. because
 * of a --help or --version option)
 */
struct ExitSuccessfully {};
/// General OptionGrouper parsing error. Do not throw this, use subclasses.
class Error : public std::exception
{
public:
    virtual ~Error() throw() {}
    const char* what() const throw();
protected:
    std::string msg;
};
/// User supplied an invalid group name.
class InvalidGroup : public Error
{
public:
    InvalidGroup(const std::string& p);
};
/// User supplied an invalid option (no such option, not enough parameters, ...)
class InvalidOption : public Error
{
public:
    InvalidOption(const std::string& p);
};
/// Ambigous group
class AmbigousGroup : public Error
{
public:
    AmbigousGroup(const std::string& p, const std::vector<Group*>& c);
};
/// Ambigous option
class AmbigousOption : public Error
{
public:
    AmbigousOption(const std::string& p, const std::vector<Option*>& c);
};

/**
 * A group can hold many options. An option name must be unique only within a
 * group. (But short names must be unique globally).
 */
class Group
{
public:
    /**
     * Construct a new group.
     * @param grouper OptionGrouper instance.
     * @param name name of the group (used between -- and :, should only contain
     *        lowercase letters, dash, and maybe numbers).
     * @param desc human parseable description of the group.
     * @pre name unique in grouper
     */
    Group(OptionGrouper& grouper, const char * name, const char * desc = "");
    ~Group();

    const std::string& GetName() const { return name; }
    const std::string& GetDescription() const { return desc; }

    const std::list<Option*>& GetOptions() const { return options; }
    const OptionGrouper& GetGrouper() const { return grouper; }
private:
    friend class Option;
    OptionGrouper& grouper;
    std::string name, desc;
    std::list<Option*> options;
};

/**
 * An option is that you can specify on the command line, can take parameters,
 * and something is executed when you specify it.
 */
class Option
{
public:
    typedef std::vector<std::string> params_t;

    /**
     * Constructor.
     * @param group put option into the specified group.
     * @param name name of the option
     * @param desc human readable description of the option.
     * @param params string describing the parameters. Something like
     *        <code>FILE</code> or <code>IMAGE1 IMAGE2 ...</code>.
     * @param short_name -c form of the option, if available. 0 means no short
     *        form. Short names must be globally unique, so use them carefully!
     * @param min_params minimum number of parameter the option takes.
     * @param max_params maxumum number of parameter the option takes.
     * @pre short_name == 0 || (32 ≤ short_name < 127)
     * @pre max_param ≥ min_param
     * @pre name unique in group
     */
    Option(Group& group, const std::string& name, const std::string& desc,
           const std::string& params, char short_name, size_t min_params,
           size_t max_params);
    virtual ~Option();

    const std::string& GetName() const { return name; }
    const std::string& GetDescription() const { return desc; }
    const std::string& GetParamsDesc() const { return params; }
    char GetShortName() const { return short_name; }
    size_t GetMinParams() const { return min_params; }
    size_t GetMaxParams() const { return max_params; }
    const Group& GetGroup() const { return group; }

    /**
     * Parse the option.
     * @param params the parameters passed on command line.
     * @pre <code>params.size() >= min_params</code>
     * @pre <code>params.size() <= max_params</code>
     */
    virtual void Parse(const params_t& params) = 0;

private:
    Group& group;
    std::string name, desc, params;
    char short_name;
    size_t min_params;
    size_t max_params;
};

/// Do not use directly.
class HelpOption : public Option
{
public:
    HelpOption(Group& grp)
        : Option(grp, "help", "Print this message", "", 'h', 0, 0) {}
    void Parse(const params_t& params);
};

/// Do not use directly.
class VersionOption : public Option
{
public:
    VersionOption(Group& grp, char short_name)
        : Option(grp, "version", "Print version number", "", short_name, 0, 0)
    {}
    void Parse(const params_t& params);
};

template <typename T>
class AnySetOption : public Option
{
public:
    AnySetOption(Group& group, T& to_set, const T& nval,
                 const std::string& name,
                 const std::string& description,
                 char short_name = 0)
        : Option(group, name, description, "", short_name, 0, 0),
          to_set(to_set), nval(nval) {}
    void Parse(const params_t&)
    {
        to_set = nval;
    }
private:
    T& to_set;
    T nval;
};
typedef AnySetOption<bool> BoolSetOption;

/**
 * Option, that stores the supplied parameter into a variable.
 * @tparam T type of the variable
 * @tparam Cast struct describing how to cast the string into the type of the
 *         variable (see NoCast, LexicalCast for an example)
 */
template <typename T, typename Cast = LexicalCast<T> >
class AnyStoreOption : public Option
{
public:
    AnyStoreOption(Group& group, T& to_set,
                   const std::string& name,
                   const std::string& description,
                   const std::string& param_str = Cast::param_name,
                   char short_name = 0)
        : Option(group, name, description, param_str, short_name, 1, 1),
          to_set(to_set) {}
    void Parse(const params_t& params)
    {
        to_set = Cast::Cast(params[0]);
    }
private:
    T& to_set;
};
typedef AnyStoreOption<std::string, NoCast> StringStoreOption;
typedef AnyStoreOption<int> IntStoreOption;
typedef AnyStoreOption<unsigned int> UIntStoreOption;
typedef AnyStoreOption<float> FloatStoreOption;
typedef AnyStoreOption<double> DoubleStoreOption;

typedef AnyStoreOption<char> SingleCharStoreOption;
typedef AnyStoreOption<char, IntCharCast<char> > IntCharStoreOption;
typedef AnyStoreOption<char, IntCharCast<unsigned char> > IntUCharStoreOption;

/**
 * Option, that appends the user supplied parameter into a container.
 * @tparam T type of the container elements
 * @tparam Container container type (including <code>&lt;T&gt;</code> template
 *         argument, when needed)
 * @tparam Cast struct describing how to cast the string into the type of the
 *         variable (see NoCast, LexicalCast for an example)
 */
template <typename T, typename Container = std::vector<T>,
          typename Cast = LexicalCast<T> >
class AnyContainerStoreOption : public Option
{
public:
    AnyContainerStoreOption(
        Group& group, Container& cnt,
        const std::string& name,
        const std::string& description,
        const std::string& param_str = std::string(Cast::param_name) + "S...",
        char short_name = 0,
        size_t min_params = 1,
        size_t max_params = -1)
        : Option(group, name, description, param_str, short_name, min_params,
                 max_params), cnt(cnt) {}
    void Parse(const params_t& params)
    {
        for (const std::string& p: params)
        {
            cnt.push_back(Cast::Cast(p));
        }
    }
private:
    Container& cnt;
};
// nyeh...
// def td(t, cnt, x = t.to_s.capitalize, cast=""); puts "typedef AnyContainerStoreOption<#{t}, std::#{cnt}<#{t}>#{cast != '' ? ", #{cast}" : ' '}> #{x}#{cnt.to_s.capitalize}StoreOption;"; end
// [:vector, :list].each {|cnt| td 'std::string', cnt, 'String', 'NoCast'; td 'int', cnt; td 'unsigned int', cnt, 'UInt'; td 'float', cnt; td 'double', cnt; puts; td 'char', cnt, 'SingleChar'; td 'char', cnt, 'IntChar', 'IntCharCast<char> '; td 'unsigned char', cnt, 'IntUChar', 'IntCharCast<unsigned int> '; puts; }
typedef AnyContainerStoreOption<std::string, std::vector<std::string>, NoCast> StringVectorStoreOption;
typedef AnyContainerStoreOption<int, std::vector<int> > IntVectorStoreOption;
typedef AnyContainerStoreOption<unsigned int, std::vector<unsigned int> > UIntVectorStoreOption;
typedef AnyContainerStoreOption<float, std::vector<float> > FloatVectorStoreOption;
typedef AnyContainerStoreOption<double, std::vector<double> > DoubleVectorStoreOption;

typedef AnyContainerStoreOption<char, std::vector<char> > SingleCharVectorStoreOption;
typedef AnyContainerStoreOption<char, std::vector<char>, IntCharCast<char> > IntCharVectorStoreOption;
typedef AnyContainerStoreOption<unsigned char, std::vector<unsigned char>, IntCharCast<unsigned int> > IntUCharVectorStoreOption;

typedef AnyContainerStoreOption<std::string, std::list<std::string>, NoCast> StringListStoreOption;
typedef AnyContainerStoreOption<int, std::list<int> > IntListStoreOption;
typedef AnyContainerStoreOption<unsigned int, std::list<unsigned int> > UIntListStoreOption;
typedef AnyContainerStoreOption<float, std::list<float> > FloatListStoreOption;
typedef AnyContainerStoreOption<double, std::list<double> > DoubleListStoreOption;

typedef AnyContainerStoreOption<char, std::list<char> > SingleCharListStoreOption;
typedef AnyContainerStoreOption<char, std::list<char>, IntCharCast<char> > IntCharListStoreOption;
typedef AnyContainerStoreOption<unsigned char, std::list<unsigned char>, IntCharCast<unsigned int> > IntUCharListStoreOption;
// p.s.: sorry for the long lines

/**
 * Class that manages option parsing
 */
class OptionGrouper
{
public:
    /**
     * Construct a new OptionGrouper.
     * @param out grouper will write normal messages to this.
     * @param err grouper will write error messages to this.
     */
    OptionGrouper(std::ostream& out = std::cout, std::ostream& err = std::cerr);

    /**
     * Gets the default group. The default group has a special ability that it's
     * options are never prefixed.
     */
    Group& GetDefaultGroup() { return default_group; }

    /**
     * Set arguments for parsing from <code>main(argc, argv)</code>.
     * @param argc number of command line arguments.
     * @param argv pointer to command line arguments.
     * @note It doesn't modify them, to obtain non-option and non-parameter
     *       arguments, use GetArguments() after parsing.
     */
    void SetArguments(int argc, char const* const* argv)
    {
        arguments.clear();
        arguments.insert(arguments.begin(), argv + 1, argv + argc);
    }
    template <typename T>
    void SetArguments(const T& args)
    {
        arguments.clear();
        arguments.insert(arguments.begin(), args.begin(), args.end());
    }
    /// @return arguments to parse/arguments remaining after parse
    const std::list<std::string>& GetArguments() const
    { return arguments; }

    /// Parse supplied arguments
    void Parse();
    void Parse(int argc, char const* const* argv)
    {
        SetArguments(argc, argv);
        Parse();
    }
    template <typename T>
    void Parse(const T& args)
    {
        SetArguments(args);
        Parse();
    }

    /// Parse arguments, handle exit (on error/--help/etc.)
    void Run();
    /**
     * Parse the specified arguments, handle exit
     * @param [in] argc number of arguments
     * @param [in] argv the arguments
     */
    void Run(int argc, char const* const* argv)
    {
        SetArguments(argc, argv);
        Run();
    }
    template <typename T>
    void Run(const T& args)
    {
        SetArguments(args);
        Run();
    }

    std::ostream& GetOut() const { return out; }
    std::ostream& GetErr() const { return err; }

    /// Print out a help message
    void PrintHelp() const;
    /// Get the header (printed before help message)
    const std::string& GetHeader() const { return header; }
    /**
     * Set header (that is printed before the help message)
     * @param header set header to this
     */
    void SetHeader(const std::string& header) { this->header = header; }
    /// Get program version
    const std::string& GetVersion() const { return version; }
    /**
     * Set program version, add an option (--version) to get it.
     * @param ver program version
     * @param short_option declare short option, 0 means no short option.
     */
    void SetVersion(const std::string& ver, char short_option = 0);

private:
    std::ostream& out, & err;

    typedef std::list<std::string>::iterator iterator_t;
    iterator_t ParseArg(iterator_t it, Option** shorts);
    iterator_t ParseShort(iterator_t it, Option** shorts);
    iterator_t ParseLong(iterator_t it);
    iterator_t ParseParams(Option* o, iterator_t it, size_t i);

    Group* FindGroup(const std::string& name, const std::string& p);
    Option* FindOption(const Group* group, const std::string& name,
                       const std::string& p);
    void FindSubOption(const Group& g, const std::string& name,
                       std::vector<Option*>& cmatch, std::vector<Option*>& camb);

    friend class Group;
    std::list<Group*> groups;
    Group default_group;
    HelpOption help_option;
    std::unique_ptr<VersionOption> version_option;
    std::list<std::string> arguments;

    std::string version;
    std::string header;
};

}

inline OptionGrouper::OptionGrouper& GetOptionGrouper()
{
    static OptionGrouper::OptionGrouper gp;
    return gp;
}
