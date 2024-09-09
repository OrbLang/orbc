#include "cli.hpp"

#include "ctx/ctx.hpp"

#include <__expected/unexpect.h>
#include <__expected/unexpected.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
// #include <expected>
#include <iostream>
#include <sstream>
#include <string_view>


void cli::PrintVersion() { printf("orbc: v%s\n", VERSION); }

void cli::PrintHelpPage() { printf("ORB COMPILER (ORBC) v%s\n", VERSION); }

std::expected<int, std::string> cli::ParseArgs(ctx::GlobalCtx* ctx, int argc, char** argv)
{
    int argsSupplied = 0;

    std::stringstream errorStream{""};

    // If there are isn't any given arguments.
    // Argv is 1 long by default, since argv[0] holds the path to
    // the executable
    if (argc <= 1)
    {
        return 0;
    }

    // Print help page
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        PrintHelpPage();
        return argsSupplied + 1;
    }

    // Print version
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        PrintVersion();
        return argsSupplied + 1;
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
                errorStream << "Entry path already set to \"" << ctx->entryPath
                            << "\", but got another positional argument \"" << argv[i] << "\"";

                return std::unexpected(errorStream.str());
            }

            ctx->entryPath = argv[i];
            hasEntrypath = true;
            argsSupplied += 1;
        }

        // Output path
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            if (argc <= i + 1)
            {
                errorStream << "Expected a value to the --output argument";
                return std::unexpected(errorStream.str());
            }

            i += 1;
            ctx->outPath = argv[i];
            argsSupplied += 1;
        }

        // File includes
        else if (strcmp(argv[i], "--file-include") == 0)
        {
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                i += 1;
                ctx->fileInclude.push_back(argv[i]);
            }
            if (ctx->fileInclude.size() == 0)
            {
                errorStream << "Expected atleast one string value for the --file-include argument";
                return std::unexpected(errorStream.str());
            }
            argsSupplied += 1;
        }

        // Lib includes
        else if (strcmp(argv[i], "--lib-include") == 0)
        {
            while (i + 1 < argc && argv[i + 1][0] != '-')
            {
                i += 1;
                ctx->libInclude.push_back(argv[i]);
            }
            if (ctx->libInclude.size() == 0)
            {
                errorStream << "Expected atleast one string value for the --lib-include argument";
                return std::unexpected(errorStream.str());
            }
            argsSupplied += 1;
        }

        // Stdlib path
        else if (strcmp(argv[i], "--stdlib") == 0)
        {
            if (argc <= i + 1)
            {
                errorStream << "Expected a string value for the --stdlib argument";
                return std::unexpected(errorStream.str());
            }

            i += 1;
            ctx->stdlib = argv[i];
            argsSupplied += 1;
        }

        // Optimization level
        else if (strcmp(argv[i], "--opt") == 0)
        {
            if (argc <= i + 1)
            {
                errorStream << "Expected an integer value for the --opt argument";
                return std::unexpected(errorStream.str());
            }

            i += 1;
            try
            {
                ctx->optLevel = std::stoi(argv[i]);
            }
            catch (std::exception const& e)
            {
                errorStream << "Expected integer argument for --opt, got \"" << argv[i] << "\"";
                return std::unexpected(errorStream.str());
            }
            argsSupplied += 1;
        }

        // Target platform
        else if (strcmp(argv[i], "--target") == 0)
        {
            if (argc <= i + 1)
            {
                errorStream << "Expected a string value for the --target argument";
                return std::unexpected(errorStream.str());
            }

            i += 1;
            ctx->targetPlatform = argv[i];
            argsSupplied += 1;
        }

        // Release mode
        else if (strcmp(argv[i], "--release") == 0)
        {
            ctx->inReleaseMode = true;
            argsSupplied += 1;
        }

        // Unknown command / argument
        else
        {
            errorStream << "Got unknown argument: \"" << argv[i] << "\"";
            return std::unexpected(errorStream.str());
        }
    }

    return argsSupplied;
}
