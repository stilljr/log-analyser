#pragma once

#include <string>
#include <vector>
#include "LogEntry.h"
#include "LogFilter.h"

// ─────────────────────────────────────────────────────────────────────────────
// LogReporter — generates reports from filtered log entries
// ─────────────────────────────────────────────────────────────────────────────
class LogReporter
{
public:

    // ── Prints a full report to the terminal ─────────────────────────────
    static void printReport(
        const std::vector<LogEntry>& entries,
        const FilterOptions& options
    );

    // ── Saves a report to a text file ────────────────────────────────────
    static void saveReport(
        const std::vector<LogEntry>& entries,
        const FilterOptions& options,
        const std::string& outputPath
    );

    // ── Prints a summary of severity counts ──────────────────────────────
    static void printSummary(
        const std::vector<LogEntry>& entries
    );

private:

    // ── Builds the report content as a string ────────────────────────────
    static std::string buildReport(
        const std::vector<LogEntry>& entries,
        const FilterOptions& options
    );

    // ── Builds the summary content as a string ───────────────────────────
    static std::string buildSummary(
        const std::vector<LogEntry>& entries
    );
};
