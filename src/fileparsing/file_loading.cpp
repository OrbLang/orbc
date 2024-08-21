// Project Headers
#include <orb/fileparsing/file_loading.hpp>
#include <orb/log/assert.hpp>


// Libraries
// ICU
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

template <>
std::expected<icu::UnicodeString, std::string> LoadFile<const char8_t*>(const char8_t* path);

template <>
std::expected<icu::UnicodeString, std::string> LoadFile<const u8string&>(const u8string& path);
template <>
std::expected<icu::UnicodeString, std::string> LoadFile<u8string_view>(u8string_view path);

} // namespace orb::fileparsing
