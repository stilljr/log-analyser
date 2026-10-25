#include <iostream>
#include <string>
#include <vector>

#include "LogEntry.h"
#include "LogParser.h"
#include "LogFilter.h"
#include "LogReporter.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// printUsage — shows command line usage instructions
// ─────────────────────────────────────────────────────────────────────────────
void printUsage(const std::string& programName)
{
    std::cout << "\n";
    std::cout << "Usage: " << programName << " <logfile> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --from   <date>     Filter from date (YYYY-MM-DD)\n";
    std::cout << "  --to     <date>     Filter to date   (YYYY-MM-DD)\n";
    std::cout << "  --level  <level>    Minimum severity (DEBUG/INFO/WARN/ERROR)\n";
    std::cout << "  --find   <keyword>  Search for keyword in messages\n";
    std::cout << "  --errors            Show errors only\n";
    std::cout << "  --output <file>     Save report to file\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "  log-analyser sample.log\n";
    std::cout << "  log-analyser sample.log --errors\n";
    std::cout << "  log-analyser sample.log --level WARN\n";
    std::cout << "  log-analyser sample.log --find \"database\"\n";
    std::cout << "  log-analyser sample.log --level ERROR --output report.txt\n";
    std::cout << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// parseArgs — reads command line arguments into FilterOptions
// ─────────────────────────────────────────────────────────────────────────────
FilterOptions parseArgs(int argc, char* argv[])
{
    FilterOptions options;

    for (int i = 2; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--from" && i + 1 < argc)
        {
            options.dateFrom = argv[++i];
        }
        else if (arg == "--to" && i + 1 < argc)
        {
            options.dateTo = argv[++i];
        }
        else if (arg == "--level" && i + 1 < argc)
        {
            std::string level = argv[++i];

            // Accept lowercase input such as "warn" or "error"
            std::transform(level.begin(), level.end(), level.begin(),
                [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

            options.minSeverity = LogEntry::stringToSeverity(level);

            if (options.minSeverity == Severity::UNKNOWN)
            {
                throw std::invalid_argument(
                    "Invalid level '" + std::string(argv[i]) +
                    "'. Use DEBUG, INFO, WARN or ERROR.");
            }
        }
        else if (arg == "--find" && i + 1 < argc)
        {
            options.keyword = argv[++i];
        }
        else if (arg == "--errors")
        {
            options.errorsOnly = true;
        }
        else if (arg == "--output" && i + 1 < argc)
        {
            // handled in main
            ++i;
        }
    }

    return options;
}

// ─────────────────────────────────────────────────────────────────────────────
// main — entry point
// ─────────────────────────────────────────────────────────────────────────────
int main(int argc, char* argv[])
{
    std::cout << "\n";
    std::cout << "================================================\n";
    std::cout << "  G&H Tech Group — Log Analyser v1.0\n";
    std::cout << "================================================\n";

    // Require at least a log file argument
    if (argc < 2)
    {
        printUsage(argv[0]);
        return 1;
    }

    std::string logFile = argv[1];
    std::string outputFile = "";

    // Check for output file argument
    for (int i = 2; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--output" && i + 1 < argc)
        {
            outputFile = argv[++i];
        }
    }

    try
    {
        // Step 1 — Parse
        std::cout << "\nStep 1: Parsing log file...\n";
        std::vector<LogEntry> entries = LogParser::parseFile(logFile);

        // Step 2 — Filter
        std::cout << "\nStep 2: Applying filters...\n";
        FilterOptions options = parseArgs(argc, argv);
        std::vector<LogEntry> filtered = LogFilter::apply(entries, options);

        // Step 3 — Report
        std::cout << "\nStep 3: Generating report...\n";
        LogReporter::printReport(filtered, options);
        LogReporter::printSummary(filtered);

        // Step 4 — Save if output file specified
        if (!outputFile.empty())
        {
            std::cout << "Step 4: Saving report...\n";
            LogReporter::saveReport(filtered, options, outputFile);
        }

        std::cout << "Done.\n\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nError: " << e.what() << "\n\n";
        return 1;
    }

    return 0;
}