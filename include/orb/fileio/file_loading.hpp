#pragma once
// Project Headers
#include <orb/concepts.hpp>
#include <orb/fileio/error.hpp>
#include <orb/log/assert.hpp>
#include <orb/log/log.hpp>

// STDLIB
#include <expected>
#include <filesystem>
#include <string>

// Libraries
// ICU
#include <unicode/unistr.h>

namespace orb
{
using std::filesystem::path;

/**
 * @brief Initializes the ICU converter with the system default name
 */
void InitDefaultUnicode();

/**
 * @brief Initialized the internal ICU converter by detecting the most likely used
 * characterset that the input string uses. This function can fail if there are errors
 * in the file contents or the characterset doesn't exist or can't be found.
 * @param rawFile This is the raw file as a pointer to a string in memory
 * @param len This parameter is optional and should only be specified if the string
 * pointed to by `rawFile` is not null terminated
 */
void InitUnicodeParsing(const char* rawFile, int32_t len = -1);


/**
 * @brief Opens a file and returns the files contents as a UnicodeString
 * @param path The path to file to be loaded
 * @return Returns a UnicodeString or an error message if something goes wrong
 */
auto LoadFile(const path& path) noexcept -> std::expected<icu::UnicodeString, Error<std::string>>;

} // namespace orb
