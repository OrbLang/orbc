#pragma once
#include "src/log/log.hpp"

#include <boost/preprocessor/facilities/overload.hpp>
#include <concepts>
#include <cstdio>
#include <iostream>
#include <source_location>
#include <sstream>
#include <string_view>

#define ASSERT_EQ_4(lhs, rhs, errMsg, failCode)                                                 \
    {                                                                                           \
        const auto src_loc = std::source_location::current();                                   \
        static_assert(std::equality_comparable_with<decltype(lhs), decltype(rhs)>);             \
        static_assert(std::is_convertible<decltype(errMsg), std::string_view>::value);          \
        static_assert(std::is_same<decltype(failCode), int>::value);                            \
        if ((lhs) != (rhs))                                                                     \
        {                                                                                       \
            const std::string lhsTypeName = typeid(lhs).name();                                 \
            const std::string rhsTypeName = typeid(rhs).name();                                 \
                                                                                                \
            std::stringstream ss;                                                               \
            ss << "In " << src_loc.file_name() << ": " << src_loc.line() << ','                 \
               << src_loc.column() << ": " #lhs << '(' << lhsTypeName << ") " << " != " << #rhs \
               << '(' << rhsTypeName << ")\n"                                                   \
               << "lhs = " << (lhs) << "\nrhs = " << (rhs) << '\n'                              \
               << (errMsg);                                                                     \
                                                                                                \
            logging::Error(ss.str());                                                           \
            exit(failCode);                                                                     \
        }                                                                                       \
    }

#define ASSERT_EQ_3(lhs, rhs, errMsg) ASSERT_EQ_4(lhs, rhs, errMsg, 1)

#define ASSERT_EQ(...) BOOST_PP_OVERLOAD(ASSERT_EQ_, __VA_ARGS__)(__VA_ARGS__)

#define ASSERT_3(expr, errMsg, failCode)                                               \
    {                                                                                  \
        const auto src_loc = std::source_location::current();                          \
        static_assert(std::is_convertible<decltype(errMsg), std::string_view>::value); \
        static_assert(std::is_same<decltype(failCode), int>::value);                   \
        if (!(expr))                                                                   \
        {                                                                              \
            std::stringstream ss;                                                      \
            ss << "In " << src_loc.file_name() << ": " << src_loc.line() << ','        \
               << src_loc.column() << ": " #expr << '\n'                               \
               << (errMsg);                                                            \
                                                                                       \
            logging::Error(ss.str());                                                  \
            exit(failCode);                                                            \
        }                                                                              \
    }

#define ASSERT_2(expr, errMsg) ASSERT_3(expr, errMsg, 1)

#define ASSERT(...) BOOST_PP_OVERLOAD(ASSERT_, __VA_ARGS__)(__VA_ARGS__)

// This is / was just a tester function, don't worry about it
/*
static void func()
{
    int bar = 1;
    int foo = 2;

    ASSERT_EQ_4(foo, bar, "oh no fail code 1", 1);
    ASSERT_EQ_3(foo, bar, "Bruda we fail");
    ASSERT_EQ(foo, bar, "We fail today");
    ASSERT_EQ(foo, bar, "We fail today 2", 2);

    ASSERT_3(bar > 55, "Wow, so small!", 55);
    ASSERT_2(foo < 1, "Wow, so BIG!!!");
    ASSERT(bar > foo, "Wow, foo is better!!!!!")
    ASSERT(bar > foo, "Wow, foo is better 2: Electric boogaloo", 2)
} */
