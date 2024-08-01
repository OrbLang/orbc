#include "log.hpp"

#include "color.hpp"

#include <iostream>

void logging::Error(const char* msg)
{
    color::Modifier red{color::Color::FgRed, color::Effect::Bold};
    color::Modifier reset{color::Color::FgDefault, color::Effect::None};

    std::cerr << red << "[ERROR]: " << msg << reset << std::endl;
}

void logging::Warn(const char* msg)
{
    color::Modifier red{color::Color::FgYellow, color::Effect::Bold};
    color::Modifier reset{color::Color::FgDefault, color::Effect::None};

    std::cerr << red << "[WARNING]: " << msg << reset << std::endl;
}
