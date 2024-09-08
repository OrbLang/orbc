#include "cli/cli.hpp"
#include "ctx/ctx.hpp"

#include <cstdio>
#include <iostream>

int main(int argc, char** argv)
{
    ctx::GlobalCtx ctx = ctx::DefaultCtx();

    cli::ParseArgs(&ctx, argc, argv);

    std::cout << "Entrypath:\t" << ctx.entryPath << std::endl;
    std::cout << "OutPath:\t" << ctx.outPath << std::endl;

    printf("File includes %zu\n", ctx.fileInclude.size());
    for (size_t i = 0; i < ctx.fileInclude.size(); i++)
    {
        std::cout << "\t" << ctx.fileInclude[i] << std::endl;
    }

    printf("Lib includes %zu\n", ctx.libInclude.size());
    for (size_t i = 0; i < ctx.libInclude.size(); i++)
    {
        std::cout << "\t" << ctx.libInclude[i] << std::endl;
    }

    std::cout << "Stdlib:\t\t" << ctx.stdlib << std::endl;
    std::cout << "Optlevel:\t" << ctx.optLevel << std::endl;
    std::cout << "Target:\t\t" << ctx.targetPlatform << std::endl;
    std::cout << "Release mode:\t" << (ctx.inReleaseMode ? "true" : "false") << std::endl;
}
