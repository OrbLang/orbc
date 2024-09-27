// Project Headers
#include <orb/fileio/file_loading.hpp>
#include <orb/log/assert.hpp>
#include <orb/log/log.hpp>

// STDLIB
#include <exception>
#include <expected>
#include <fstream>
#include <ios>
#include <ranges>

// Libraries
// ICU
#include <system_error>
#include <unicode/errorcode.h>
#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>
#include <unicode/ucsdet.h>
#include <unicode/urename.h>

using namespace std::string_literals;

namespace orb
{

void InitDefaultUnicode() { ucnv_setDefaultName(ucnv_getDefaultName()); }

void InitUnicodeParsing(const char* rawFile, int32_t len)
{
    icu::ErrorCode result;
    UCharsetDetector* detector = ucsdet_open(result);
    if (result.isFailure() != 0)
        logging::Error("There was an error opening the characterset detector from ICU lib");

    ucsdet_setText(detector, rawFile, len, result);
    if (result.isFailure() != 0)
        logging::Error("There was an error loading the text while detecting the fileencoding");

    const UCharsetMatch* charset = ucsdet_detect(detector, result);
    if (result.isFailure() != 0)
        logging::Error("There was an error detecting the fileencoding of the current file");

    if (result.isFailure() != 0)
        logging::Error("Couldn't find suitable fileencoding of file");

    const char* converterName = ucsdet_getName(charset, result);
    if (result.isFailure() != 0)
        logging::Error("Couldn't get name of the files characterset");

    ucnv_setDefaultName(converterName);
}

auto LoadFile(const path& filePath) noexcept
    -> std::expected<icu::UnicodeString, Error<std::string>>
{
    using Failure = std::unexpected<Error<std::string>>;

    try
    {
        std::error_code err;
        size_t fileSize = std::filesystem::file_size(filePath, err);
        if (err.value() != 0)
            return Failure(err.message());

        if (fileSize == 0)
            return icu::UnicodeString();

        std::ifstream fileStream(filePath);
        if (fileStream.fail())
            return Failure("Failed to open file: "s + filePath.string());

        // Read from the input stream into memory
        std::string fileStr;
        fileStr.resize_and_overwrite(
            fileSize,
            [&fileStream](char* bytes, std::streamsize len)
            {
                // Read into fileStr and return num of bytes read
                fileStream.read(bytes, len);
                return fileStream.gcount();
            }
        );

        // Remove whitespace by finding first and last characters that arent whitespace
        auto whitespace = [](char chr) { return std::isspace(chr); };

        char* begin = std::ranges::find_if_not(fileStr, whitespace).base();
        char* end =
            std::ranges::find_if_not(fileStr | std::views::reverse, whitespace).base().base();

        return icu::UnicodeString(begin, static_cast<int32_t>(end - begin));
    }
    catch (const std::exception& e)
    {
        return Failure(e.what());
    }
}

} // namespace orb
