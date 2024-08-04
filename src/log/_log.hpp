#pragma once

#ifndef LOGGING_LIB
#error You may not use this headerfile
#endif

#include <string_view>
namespace logging
{
inline int totalLineCount = 0;

void UpdateLineCount(std::string_view string);
} // namespace logging
