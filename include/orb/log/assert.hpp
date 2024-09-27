#pragma once

// Project Headers
#include <orb/concepts.hpp>
#include <orb/log/log.hpp>

// STDLIB
// Ignore warnings they are incorrect
#include <cassert>
#include <sstream>
#include <string_view>

#define ORB_LITTERAL(x) #x
#define ORB_EQ(first, second) static_cast<bool>((first) == (second))

#if defined DEBUG
// clang-format off
#define ORB_ASSERT(expr, errMsg) do { } while (false)
// clang-format on
#else
#define ORB_ASSERT(expr, errMsg)                                                                   \
    do                                                                                             \
    {                                                                                              \
        static_assert(                                                                             \
            orb::StdOstreamPrintable<decltype(errMsg)>,                                            \
            "errMsg needs to be printable to cout and cerr"                                        \
        );                                                                                         \
        if (static_cast<bool>(expr))                                                               \
            break;                                                                                 \
                                                                                                   \
        std::stringstream description;                                                             \
        description << __FILE__ << " at line " << __LINE__ << ' ' << "assertion failed: " << #expr \
                    << " with message: " << (errMsg);                                              \
                                                                                                   \
        logging::Error(description.str());                                                         \
        assert(                                                                                    \
            !static_cast<bool>(expr) && "Internal Orbc assertion failed in file: " __FILE__        \
                                        ", at line: " ORB_LITTERAL(__LINE__) " expr: (" #expr ")"  \
        );                                                                                         \
    } while (false)
#endif
