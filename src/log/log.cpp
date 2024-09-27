// Project Headers
#define LOGGING_LIB
#include <orb/log/color.hpp>
#include <orb/log/log.hpp>


// STDLIB
#include <algorithm>
#include <iostream>
#include <ostream>
#include <string_view>

namespace logging
{
void UpdateLineCount(std::string_view string)
{
    int newLines = static_cast<int>(std::ranges::count(string, '\n'));

    totalLineCount += 1 + newLines;
}

void Error(std::string_view msg, std::ostream& stream)
{
    Modifier red {Color::FgRed, Effect::Bold};
    Modifier reset {Color::FgDefault, Effect::None};

    stream << red << "[ERROR]: " << msg << reset << std::endl;

    UpdateLineCount(msg);
}

void Warn(std::string_view msg, std::ostream& stream)
{
    Modifier red {Color::FgYellow, Effect::Bold};
    Modifier reset {Color::FgDefault, Effect::None};

    stream << red << "[WARNING]: " << msg << reset << std::endl;

    UpdateLineCount(msg);
}

void Info(std::string_view msg, std::ostream& stream)
{
    Modifier reset {Color::FgDefault, Effect::None};

    stream << reset << "[INFO]: " << msg << reset << std::endl;

    UpdateLineCount(msg);
}
} // namespace logging
