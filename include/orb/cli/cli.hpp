#pragma once
// Project Headers
#include <orb/ctx.hpp>

// STDLIB
#include <expected>
#include <string>

#define VERSION "0.0.1"

namespace orb::cli
{

/// Parses the given arguments from `argv`, and puts
/// them into the given `GlobalCtx`.
/// Returns a string with an error message, if a parse error happened.
/// Otherwise it returns the number of arguments supplied by the user.
std::expected<int, std::string> ParseArgs(orb::GlobalCtx* ctx, int argc, char** argv);

void PrintHelpPage(char* path);

void PrintVersion();

} // namespace orb::cli
