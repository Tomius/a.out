#pragma once
/**
 * @file logger/locmacros.hpp
 * Macros for file/line/function managing.
 */

#include <string>
#include "config.h"

#ifdef DEBUG
#include "misc/filestrip.hpp"

/// Use in method declarations which needs file/line/function.
#define ATPAR const std::string& file, const boost::uint_fast16_t line,  \
        const std::string& function
#define ATPARC ATPAR,
#define CATPAR const char * file, const boost::uint_fast16_t line, \
        const char * function
#define CATPARC CATPAR,
#define FLF file, line, function
#define FLFC FLF,
#define FLFPAR (FLF)
/// The current file, line number, and function name.
#define AT PRETTY_FILE, __LINE__, __PRETTY_FUNCTION__
#define ATC AT,

#else //DEBUG

#define ATPAR
#define ATPARC
#define CATPAR
#define CATPARC
#define FLF
#define FLFC
#define FLFPAR
#define AT
#define ATC

#endif //DEBUG
