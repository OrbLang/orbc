#pragma once

#include <string_view>
namespace logging
{
void Error(std::string_view msg);
void Warn(std::string_view msg);
}; // namespace logging
