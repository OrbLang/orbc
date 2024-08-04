#include "status.hpp"

#include "color.hpp"
#include "log.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

namespace logging
{
static std::vector<LoadingBar*> loadingbarList{};

LoadingBar::LoadingBar(std::string_view name, float percent, std::string_view description,
                       bool deleteBar)
        : percent{percent}, name{name}, description{description}, rowNum{m_totalLineCount},
          deleteBar{deleteBar}
{
    loadingbarList.push_back(this);

    m_totalLineCount += 1;
    std::cout << "\n";

    Draw();
}

LoadingBar::~LoadingBar(void)
{
    loadingbarList.erase(std::find(loadingbarList.begin(), loadingbarList.end(), this));

    if (!deleteBar)
        return;

    // Saves the cursors current position.
    std::cout << "\x1b[s";

    // Moves the cursor to column `0`, and moves it up to the the loading bar's position
    std::cout << "\x1b[0G\x1b[" << m_totalLineCount - rowNum << "F";
}

void LoadingBar::UpdateBar(float percent, std::string_view description)
{
    this->percent = percent;
    this->description = description;

    Draw();
}

void LoadingBar::UpdatePercent(float percent)
{
    this->percent = percent;

    Draw();
}
void LoadingBar::UpdateDescription(std::string_view description)
{
    this->description = description;

    Draw();
}

void LoadingBar::Draw()
{
    // Saves the cursors current position.
    std::cout << "\x1b[s";

    // Moves the cursor to column `0`, and moves it up to the the loading bar's position
    std::cout << "\x1b[0G\x1b[" << m_totalLineCount - rowNum << "A";


    std::string barString = "[";

    // Get the amount of filled chars in the bar
    uint8_t length = std::floor(static_cast<float>(barLength) * (percent / 100));

    // Sets the filled chars in the bar
    barString += std::string(length, '#');
    // Sets the remaining chars in the bar
    barString += std::string(barLength - length, ' ');

    barString += "]";

    color::Modifier nameColor{color::Color::FgYellow, color::Effect::Bold};
    color::Modifier barColor{color::Color::FgGreen, color::Effect::Bold};
    color::Modifier noColor{color::Color::FgDefault, color::Effect::None};

    // Erase the line
    std::cout << "\x1b[2K";

    // print the bar
    std::cout << " - " << nameColor << name << " \t" << barColor << barString << '(' << percent
              << "%)" << noColor << ": " << description << std::endl;

    // Revert the cursor to the last saved position
    std::cout << "\x1b[u";
}
} // namespace logging
