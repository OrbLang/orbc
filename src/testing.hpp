#pragma once
#include <concepts>
#include <cstdio>
#include <iostream>
#include <string_view>

#define ASSERT_EQ(lhs, rhs, errMsg, failCode)

inline void Assert(bool statement, std::string_view errorMsg, int failCode = 1)
{
    if (statement)
        return;

    std::cerr << "[ERROR]: " << errorMsg;
    exit(failCode);
}

template <typename T, typename U>
    requires(std::equality_comparable_with<T, U>)
inline void AssertEq(T lhs, U rhs, std::string_view errorMsg, int failCode = 1)
{
    if (lhs == rhs)
        return;

    // Make a better error message
    std::string lhsTypeName = typeid(lhs).name();
    std::string rhsTypeName = typeid(rhs).name();

    Assert(false, errorMsg, failCode);
}
