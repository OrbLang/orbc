#include "cli.hpp"

#include "ctx/ctx.hpp"

#include <__expected/unexpect.h>
#include <__expected/unexpected.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string_view>


void cli::PrintVersion() { printf("orbc: v%s\n", VERSION); }

void cli::PrintHelpPage(char* path)
{
    printf("ORB COMPILER (ORBC) v%s\n", VERSION);
    printf("Path: %s\n", path);

    printf("OPTIONS:\n");
    printf("\t--help, -h                    Prints the help page\n");
    printf("\t--version, -v                 Prints the version\n");
    printf("\t<entry>                       A positional argument. The path to the file which the "
           "program starts from\n");
    printf("\t[--out, -o] <path>            The path to where the resulting executable should be "
           "stored\n");
    printf("\t[--file-include] <paths>...   A list of space seperated paths to files that should "
           "be included in the compilation\n");
    printf(
        "\t[--lib-include] <paths>...    A list of space seperated paths to libraries that should "
        "be included in the compilation\n");
    printf("\t[--stdlib] <path>             The path to the standard library to use. Is "
           "$HOME/.orb/stdlib by default\n");
    printf("\t[--opt] <level>               The optimization level of the program. Is an integer "
           "between 1 and 3\n");
    printf("\t[--target] <platform>         Specifies the target platform of the program\n");
    printf("\t[--release]                   Use this to compile in release mode, otherwise it will "
           "compile in debug mode\n");
}


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
        PrintHelpPage(argv[0]);
        exit(0); // Exit the program on `--help`
    }

    // Print version
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
    {
        PrintVersion();
        exit(0); // Exit the program on `--version`
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
