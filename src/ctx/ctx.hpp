#pragma once

#include <string_view>
#include <vector>

namespace ctx
{

struct GlobalCtx
{
    /// The path to the `main` file, which holds the entry funtion.
    std::string_view entryPath;
    /// The path to the resulting executable.
    std::string_view outPath;
    /// A list of paths to files to include in the compilation.
    std::vector<std::string_view> fileInclude;
    /// A list of paths to libraries to include in the compilation. (May be unnecessary).
    std::vector<std::string_view> libInclude;
    /// A path to the stdlib.
    std::string_view stdlib;
    /// The optimization level (`1` by default).
    size_t optLevel;
    /// A string which specifies the target platform.
    std::string_view targetPlatform;
    /// If the executable is for release (`true`) or debug (`false`).
    bool inReleaseMode;
};

/// Get the default values for the `GlobalCtx`
inline GlobalCtx DefaultCtx()
{
    return GlobalCtx{.entryPath = "main.orb",
                     .outPath = "main",
                     .fileInclude = {},
                     .libInclude = {},
                     .stdlib = "",
                     .optLevel = 1,
                     .targetPlatform = "",
                     .inReleaseMode = false};
}

} // namespace ctx
