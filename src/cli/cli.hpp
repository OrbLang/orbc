#pragma once

#include "ctx/ctx.hpp"

#include <optional>
#include <string_view>

#define VERSION "0.0.1"

namespace cli
{

/// Parses the given arguments from `argv`, and puts
/// them into the given `GlobalCtx`.
/// Returns a string with an error message, if a parse error happened.
/// This should be handled.
void ParseArgs(ctx::GlobalCtx* ctx, int argc, char** argv);

void PrintHelpPage();

void PrintVersion();

} // namespace cli
