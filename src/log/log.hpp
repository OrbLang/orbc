#pragma once

#include <iostream>
#include <ostream>
#include <string_view>

#ifdef LOGGING_LIB
#include <log/_log.hpp>
#endif

namespace logging
{
/// Send an error message to a stream.
/// * `msg` - The error message
/// * `stream` - The `ostream`. Defaults to `std::cerr` (stderr)
void Error(std::string_view msg, std::ostream& stream = std::cerr);

/// Send a warning message to a stream.
/// * `msg` - The warning message
/// * `stream` - The `ostream`. Defaults to `std::cout` (stdout)
void Warn(std::string_view msg, std::ostream& stream = std::cout);

/// Send a info message to a stream.
/// * `msg` - The info message
/// * `stream` - The `ostream`. Defaults to `std::cout` (stdout)
void Info(std::string_view msg, std::ostream& stream = std::cout);
}; // namespace logging
