#include "LogReporter.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

// ─────────────────────────────────────────────────────────────────────────────
// printReport — prints the full filtered report to the terminal
// ─────────────────────────────────────────────────────────────────────────────
void LogReporter::printReport(
    const std::vector<LogEntry>& entries,
    const FilterOptions& options)
{
    std::cout << buildReport(entries, options);
}

// ─────────────────────────────────────────────────────────────────────────────
// saveReport — saves the full filtered report to a text file
// ─────────────────────────────────────────────────────────────────────────────
void LogReporter::saveReport(
    const std::vector<LogEntry>& entries,
    const FilterOptions& options,
    const std::string& outputPath)
{
    std::ofstream file(outputPath);

    if (!file.is_open())
    {
        std::cerr << "Could not open output file: " << outputPath << "\n";
        return;
    }

    file << buildReport(entries, options);
    file << buildSummary(entries);
    file.close();

    std::cout << "  Report saved to: " << outputPath << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// printSummary — prints severity counts to the terminal
// ─────────────────────────────────────────────────────────────────────────────
void LogReporter::printSummary(
    const std::vector<LogEntry>& entries)
{
    std::cout << buildSummary(entries);
}

// ─────────────────────────────────────────────────────────────────────────────
// buildReport — assembles the full report as a string
// ─────────────────────────────────────────────────────────────────────────────
std::string LogReporter::buildReport(
    const std::vector<LogEntry>& entries,
    const FilterOptions& options)
{
    std::ostringstream out;

    out << "\n";
    out << "================================================\n";
    out << "  LOG ANALYSIS REPORT\n";
    out << "================================================\n";

    // Show active filters
    if (!options.dateFrom.empty() || !options.dateTo.empty())
    {
        out << "  Date range:   "
            << (options.dateFrom.empty() ? "start" : options.dateFrom)
            << " to "
            << (options.dateTo.empty() ? "end" : options.dateTo)
            << "\n";
    }

    if (!options.keyword.empty())
        out << "  Keyword:      " << options.keyword << "\n";

    if (options.errorsOnly)
        out << "  Filter:       Errors only\n";

    out << "  Total matches: " << entries.size() << "\n";
    out << "------------------------------------------------\n\n";

    // Print each entry
    for (const auto& entry : entries)
    {
        out << entry.date << " "
            << entry.time << " "
            << "[" << LogEntry::severityToString(entry.severity) << "] "
            << entry.message << "\n";
    }

    out << "\n";
    return out.str();
}

// ─────────────────────────────────────────────────────────────────────────────
// buildSummary — counts entries by severity and assembles summary string
// ─────────────────────────────────────────────────────────────────────────────
std::string LogReporter::buildSummary(
    const std::vector<LogEntry>& entries)
{
    std::ostringstream out;

    // Count by severity
    std::map<Severity, int> counts;
    counts[Severity::DEBUG] = 0;
    counts[Severity::INFO] = 0;
    counts[Severity::WARN] = 0;
    counts[Severity::ERROR] = 0;
    counts[Severity::UNKNOWN] = 0;

    for (const auto& entry : entries)
        counts[entry.severity]++;

    out << "================================================\n";
    out << "  SUMMARY\n";
    out << "================================================\n";
    out << "  Total entries:  " << entries.size() << "\n";
    out << "  DEBUG:          " << counts[Severity::DEBUG] << "\n";
    out << "  INFO:           " << counts[Severity::INFO] << "\n";
    out << "  WARN:           " << counts[Severity::WARN] << "\n";
    out << "  ERROR:          " << counts[Severity::ERROR] << "\n";
    out << "================================================\n\n";

    return out.str();
}