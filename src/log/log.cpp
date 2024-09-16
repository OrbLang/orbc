#define LOGGING_LIB
#include "log.hpp"

#include "color.hpp"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <string_view>

using namespace logging::color;

namespace logging
{
void UpdateLineCount(std::string_view string)
{
    int newLines = static_cast<int>(std::ranges::count(string, '\n'));

    totalLineCount += 1 + newLines;
}

void Error(std::string_view msg, std::ostream& stream)
{
    color::Modifier red{Color::FgRed, color::Effect::Bold};
    color::Modifier reset{color::Color::FgDefault, color::Effect::None};

    stream << red << "[ERROR]: " << msg << reset << std::endl;

    UpdateLineCount(msg);
}

void Warn(std::string_view msg, std::ostream& stream)
{
    color::Modifier red{color::Color::FgYellow, color::Effect::Bold};
    color::Modifier reset{color::Color::FgDefault, color::Effect::None};

    stream << red << "[WARNING]: " << msg << reset << std::endl;

    UpdateLineCount(msg);
}

void Info(std::string_view msg, std::ostream& stream)
{
    color::Modifier reset{color::Color::FgDefault, color::Effect::None};

    stream << reset << "[INFO]: " << msg << reset << std::endl;

    UpdateLineCount(msg);
}
} // namespace logging
