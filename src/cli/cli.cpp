#include "cli.hpp"

#include "ctx/ctx.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <string_view>


void cli::PrintVersion() { printf("orbc: v%s\n", VERSION); }

void cli::PrintHelpPage() { printf("ORB COMPILER (ORBC) v%s\n", VERSION); }

std::ecpected<void, std::string_view> cli::ParseArgs(ctx::GlobalCtx* ctx, int argc, char** argv)
{
    // If there are isn't any given arguments.
    // Argv is 1 long by default, since argv[0] holds the path to
    // the executable
    if (argc <= 1)
    {
        return;
    }

    // Print help page
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        PrintHelpPage();
        return;
    }

    // Print version
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        PrintVersion();
        return;
    }

    bool hasEntrypath = false;

    // Parse args
    for (int i = 1; i < argc; i++)
    {
        // If argument is not a flag, or a named argument
        if (argv[i][0] != '-')
        {
            // Entrypath is the only positional argument so far.
            if (hasEntrypath)
            {
                printf("Already have entry path\n");
                return; // TODO: return error
            }

            ctx->entryPath = argv[i];
            hasEntrypath = true;
        }

        // Output path
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            if (argc <= i + 1)
                return; // TODO: Return error

            i += 1;
            ctx->outPath = argv[i];
        }

        // File includes
        else if (strcmp(argv[i], "--file-include") == 0)
        {
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                i += 1;
                ctx->fileInclude.push_back(argv[i]);
            }
        }

        // Lib includes
        else if (strcmp(argv[i], "--lib-include") == 0)
        {
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                i += 1;
                ctx->libInclude.push_back(argv[i]);
            }
        }

        // Stdlib path
        else if (strcmp(argv[i], "--stdlib") == 0)
        {
            if (argc <= i + 1)
                return; // TODO: Return error

            i += 1;
            ctx->stdlib = argv[i];
        }

        // Optimization level
        else if (strcmp(argv[i], "--opt") == 0)
        {
            if (argc <= i + 1)
                return; // TODO: Return error

            i += 1;
            ctx->optLevel = atoi(argv[i]);
        }

        // Target platform
        else if (strcmp(argv[i], "--target") == 0)
        {
            if (argc <= i + 1)
                return; // TODO: Return error

            i += 1;
            ctx->targetPlatform = argv[i];
        }

        // Release mode
        else if (strcmp(argv[i], "--release") == 0)
        {
            ctx->inReleaseMode = true;
        }
    }
}
