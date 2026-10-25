#pragma once

#include <string>
#include <vector>
#include "LogEntry.h"

// ─────────────────────────────────────────────────────────────────────────────
// FilterOptions — holds all filtering criteria for a log analysis run
// ─────────────────────────────────────────────────────────────────────────────
struct FilterOptions
{
    std::string dateFrom;       // filter from this date — "" means no limit
    std::string dateTo;         // filter to this date   — "" means no limit
    Severity    minSeverity;    // minimum severity level to include
    std::string keyword;        // keyword to search for — "" means no filter
    bool        errorsOnly;     // if true show only ERROR entries

    // ── Default constructor — no filters applied ──────────────────────────
    FilterOptions()
        : dateFrom("")
        , dateTo("")
        , minSeverity(Severity::DEBUG)
        , keyword("")
        , errorsOnly(false)
    {
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// LogFilter — applies filter criteria to a vector of LogEntry objects
// ─────────────────────────────────────────────────────────────────────────────
class LogFilter
{
public:

    // ── Applies all active filters and returns matching entries ───────────
    static std::vector<LogEntry> apply(
        const std::vector<LogEntry>& entries,
        const FilterOptions& options
    );

    // ── Individual filter functions ───────────────────────────────────────
    static bool matchesDateRange(
        const LogEntry& entry,
        const FilterOptions& options
    );

    static bool matchesSeverity(
        const LogEntry& entry,
        const FilterOptions& options
    );

    static bool matchesKeyword(
        const LogEntry& entry,
        const FilterOptions& options
    );
};
