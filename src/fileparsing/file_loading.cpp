// Project Headers
#include <exception>
#include <expected>
#include <fstream>
#include <ios>
#include <memory>
#include <orb/fileparsing/file_loading.hpp>
#include <orb/log/assert.hpp>
#include <ranges>


// Libraries
// ICU
#include <span>
#include <system_error>
#include <unicode/errorcode.h>
#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>
#include <unicode/ucsdet.h>
#include <unicode/urename.h>

namespace orb::fileparsing
{

void InitDefaultUnicode() { ucnv_setDefaultName(ucnv_getDefaultName()); }

void InitUnicodeParsing(const char* rawFile, int32_t len)
{
    icu::ErrorCode result;
    UCharsetDetector* detector = ucsdet_open(result);
    ASSERT(result.isSuccess(), "There was an error opening the characterset detector from ICU lib");

    ucsdet_setText(detector, rawFile, len, result);
    ASSERT(result.isSuccess(),
           "There was an error loading the text while detecting the fileencoding");

    const UCharsetMatch* charset = ucsdet_detect(detector, result);
    ASSERT(result.isSuccess(), "There was an error detecting the fileencoding of the current file");
    ASSERT(charset != NULL, "Couldn't find suitable fileencoding of file");

    const char* converterName = ucsdet_getName(charset, result);
    ASSERT(result.isSuccess(), "Couldn't get name of the files characterset");

    ucnv_setDefaultName(converterName);
}


std::expected<icu::UnicodeString, std::string> LoadFile(std::filesystem::path path)
{
    try
    {
        std::error_code err;
        size_t fileSize = std::filesystem::file_size(path, err);
        if (err.value())
            return std::unexpected(err.message());

        if (fileSize == 0)
            return icu::UnicodeString();

        std::ifstream fileStream{path};
        if (fileStream.fail())
            return std::unexpected(std::string("Failed to open file") + path.string());

        // Read from the input stream into memory
        std::unique_ptr<char[]> fileBytes = std::make_unique_for_overwrite<char[]>(fileSize);
        fileStream.read(fileBytes.get(), static_cast<std::streamsize>(fileSize));
        std::span byteSpan(fileBytes.get(), fileSize);

        // Remove whitespace by finding first and last characters that arent whitespace
        auto Whitespace = [](char c) { return std::isspace(c); };
        char* begin = std::ranges::find_if_not(byteSpan, Whitespace).base();
        char* end =
            std::ranges::find_if_not(byteSpan | std::views::reverse, Whitespace).base().base();

        return icu::UnicodeString(begin, static_cast<int32_t>(end - begin));
    }
    catch (const std::exception& e)
    {
        return std::unexpected(e.what());
    }
}

} // namespace orb::fileparsing
