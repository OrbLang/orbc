#pragma once
// Project Headers
#include <orb/concepts.hpp>

// STDLIB
#include <expected>
#include <string>

// Libraries
// ICU
#include <unicode/unistr.h>

namespace orb
{

using u8string = std::basic_string<char8_t>;
using u8string_view = std::basic_string_view<char8_t>;

namespace fileparsing
{
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
template <IsOneOf<const char8_t*, const u8string&, u8string_view> T>
std::expected<icu::UnicodeString, std::string> LoadFile(T path);

} // namespace fileparsing

} // namespace orb
