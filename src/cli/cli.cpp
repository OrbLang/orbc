// Project Headers
#include <orb/cli/cli.hpp>
#include <orb/ctx.hpp>

// STDLIB
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <ostream>
#include <print>
#include <sstream>

void orb::cli::PrintVersion() { puts("orbc: v" VERSION "\n"); }

void orb::cli::PrintHelpPage(char* path)
{
    std::print("ORB COMPILER (ORBC) v{}\n", VERSION);
    std::print("Path: {}\n", path);

    puts("OPTIONS:\n"
         "\t--help, -h                      Prints the help page\n"
         "\t--version, -v                   Prints the version\n"
         "\t<files>...                      The paths to the files that will be included in the "
         "compilation\n"
         "\t[--out, -o] <path>              The path to where the resulting executable should be "
         "stored\n"
         "\t[--lib-include, -l] <paths>...  A list of space seperated paths to libraries that "
         "should "
         "be included in the compilation\n"
         "\t[--stdlib] <path>               The path to the standard library to use. Is "
         "$HOME/.orb/stdlib by default\n"
         "\t[--opt] <level>                 The optimization level of the program. Is an integer "
         "between 1 and 3\n"
         "\t[--target] <platform>           Specifies the target platform of the program\n"
         "\t[--release]                     Use this to compile in release mode, otherwise it will "
         "compile in debug mode\n");
}


std::expected<int, std::string> orb::cli::ParseArgs(orb::GlobalCtx* ctx, int argc, char** argv)
{
    int argsSupplied = 0;

    std::stringstream errorStream{""};

    // If there are isn't any given arguments.
    // Argv is 1 long by default, since argv[0] holds the path to
    // the executable
    if (argc <= 1)
    {
        errorStream << "No file supplied";
        return std::unexpected(errorStream.str());
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

    // Parse args
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            while (i < argc && argv[i][0] != '-')
            {
                ctx->fileInclude.push_back(argv[i]);
                i += 1;
            }
            i -= 1;
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

        // Lib includes
        else if (strcmp(argv[i], "--lib-include") == 0 || strcmp(argv[i], "-l") == 0)
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

            // TODO: Figure out the correct max opt level
            constexpr int maxOptLevel = 3;
            if (ctx->optLevel > maxOptLevel)
            {
                errorStream << "Expected optimization value to be between 0 and 3, got " << argv[i];
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

    // TODO: Check for required args

    if (ctx->fileInclude.size() == 0)
    {
        errorStream << "No file supplied";
        return std::unexpected(errorStream.str());
    }

    // Implicit values if not given an explicit value
    if (ctx->outPath == "")
    {
        std::stringstream result{""};

        for (char c : ctx->fileInclude[0])
        {
            if (c == '.')
                break;

            result << c;
        }

        // Check if the outpath and the entrypath are the same
        if (result.str().length() == ctx->fileInclude[0].length())
            result << ".out";

        ctx->outPath = result.str();
    }

    return argsSupplied;
}
