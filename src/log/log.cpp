#include "log.hpp"

#include "color.hpp"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <string_view>

using namespace logging::color;

void logging::m_UpdateLineCount(std::string_view string)
{
    int newLines = static_cast<int>(std::ranges::count(string, '\n'));

    m_totalLineCount += 1 + newLines;
}

void logging::Error(std::string_view msg, std::ostream& stream)
{
    void Error(std::string_view msg, std::ostream& stream = std::cerr);
    color::Modifier red{Color::FgRed, color::Effect::Bold};
    color::Modifier reset{color::Color::FgDefault, color::Effect::None};

    stream << red << "[ERROR]: " << msg << reset << std::endl;

    m_UpdateLineCount(msg);
}

void logging::Warn(std::string_view msg, std::ostream& stream)
{
    color::Modifier red{color::Color::FgYellow, color::Effect::Bold};
    color::Modifier reset{color::Color::FgDefault, color::Effect::None};

    stream << red << "[WARNING]: " << msg << reset << std::endl;

    m_UpdateLineCount(msg);
}
