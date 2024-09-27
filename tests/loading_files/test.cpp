// Project Headers
#include <orb/fileparsing/file_loading.hpp>
#include <orb/log/assert.hpp>

// STDLIB
#include <string>

int main(int argc, char* argv[])
{
    using namespace orb::fileparsing;

    ORB_ASSERT(argc == 3, "Must give both filename and expected output");
    InitDefaultUnicode();

    const char* filePath = argv[1];

    auto result = LoadFile(filePath);

    ORB_ASSERT(result.has_value(), result.error());


    icu::UnicodeString expectedValue(argv[2]);
    icu::UnicodeString value(result.value());

    if (expectedValue != value)
    {
        logging::Error("The value from `LoadFile` did not match the expected result");

        std::string valueStr;
        value.toUTF8String(valueStr);
        std::string expectedStr;
        expectedValue.toUTF8String(expectedStr);

        logging::Info("Gotten value: `" + valueStr + '`');
        logging::Info("Expected value: `" + expectedStr + '`');

        // Fail the test
        exit(1);
    }
}
