#include "status.hpp"

#include "color.hpp"
#include "log.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

namespace logging
{
std::vector<LoadingBar::LoadingBarData> LoadingBar::loadingbarList = {};

size_t LoadingBar::Insert(std::string_view name, float percent, std::string_view currentTask)
{
    LoadingBarData bar{.percent = percent,
                       .name = name,
                       .currentTask = currentTask,
                       .rowNum = m_totalLineCount + 1};

    loadingbarList.push_back(bar);

    size_t index = loadingbarList.size() - 1;

    Draw(index);

    m_totalLineCount += 1;

    return index;
}

void LoadingBar::UpdateBar(size_t index, float percent, std::string_view currentTask)
{
    LoadingBarData& bar = loadingbarList.at(index);

    bar.percent = percent;
    bar.currentTask = currentTask;

    Draw(index);
}

void LoadingBar::Draw(size_t index)
{
    LoadingBarData& bar = loadingbarList[index];

    // Saves the cursors current position.
    std::cout << "\x1b[s";

    // Moves the cursor to column `0`, and moves it up to the the loading bar's position
    std::cout << "\x1b[0G\x1b[" << m_totalLineCount - bar.rowNum << "F";


    std::string barString = "[";

    // Get the amount of filled chars in the bar
    uint8_t length = std::floor(static_cast<float>(barLength) * (bar.percent / 100));

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
    std::cout << " - " << nameColor << bar.name << " \t" << barColor << barString << '('
              << bar.percent << "%)" << noColor << ": " << bar.currentTask << std::endl;

    // Revert the cursor to the last saved position
    std::cout << "\x1b[u";
}
} // namespace logging
