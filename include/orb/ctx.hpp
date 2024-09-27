#pragma once
// STDLIB
#include <string>
#include <vector>

namespace orb
{
struct GlobalCtx
{
    /// The path to the resulting executable.
    std::string outPath;
    /// A list of paths to files to include in the compilation.
    std::vector<std::string> fileInclude;
    /// A list of paths to libraries to include in the compilation. (May be unnecessary).
    std::vector<std::string> libInclude;
    /// A path to the stdlib.
    std::string stdlib;
    /// The optimization level (`1` by default).
    size_t optLevel;
    /// A string which specifies the target platform.
    std::string targetPlatform;
    /// If the executable is for release (`true`) or debug (`false`).
    bool inReleaseMode;

    /// Default initializer for `GlobalCtx`
    static inline GlobalCtx Default()
    {
        return GlobalCtx {
            .outPath = "",
            .fileInclude = {},
            .libInclude = {},
            .stdlib = "",
            .optLevel = 1,
            .targetPlatform = "",
            .inReleaseMode = false
        };
    }
};
} // namespace orb
