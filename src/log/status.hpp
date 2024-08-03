#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace logging
{

class LoadingBar
{
public:
    LoadingBar() = delete;

    static size_t Insert(std::string_view name, float percent, std::string_view currentTask);
    static void UpdateBar(size_t index, float percent, std::string_view currentTask);

private:
    struct LoadingBarData
    {
        float percent;
        std::string_view name;
        std::string_view currentTask;
        int rowNum;
    };

    static std::vector<LoadingBarData> loadingbarList;

    static const uint8_t barLength = 10;

    static void Draw(size_t index);
};





} // namespace logging
