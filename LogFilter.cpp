#include "LogFilter.h"

#include <algorithm>
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
// apply — runs all active filters and returns only matching entries
// ─────────────────────────────────────────────────────────────────────────────
std::vector<LogEntry> LogFilter::apply(
    const std::vector<LogEntry>& entries,
    const FilterOptions& options)
{
    std::vector<LogEntry> results;

    for (const auto& entry : entries)
    {
        // Skip if outside date range
        if (!matchesDateRange(entry, options)) continue;

        // Skip if below minimum severity
        if (!matchesSeverity(entry, options)) continue;

        // Skip if keyword not found
        if (!matchesKeyword(entry, options)) continue;

        // Skip if errors only mode and not an error
        if (options.errorsOnly && entry.severity != Severity::ERROR) continue;

        results.push_back(entry);
    }

    std::cout << "  Filter matched " << results.size()
        << " of " << entries.size() << " entries\n";

    return results;
}

// ─────────────────────────────────────────────────────────────────────────────
// matchesDateRange — returns true if entry date is within the filter range
// ─────────────────────────────────────────────────────────────────────────────
bool LogFilter::matchesDateRange(
    const LogEntry& entry,
    const FilterOptions& options)
{
    // No date filter set
    if (options.dateFrom.empty() && options.dateTo.empty())
        return true;

    // Check from date
    if (!options.dateFrom.empty() && entry.date < options.dateFrom)
        return false;

    // Check to date
    if (!options.dateTo.empty() && entry.date > options.dateTo)
        return false;

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// matchesSeverity — returns true if entry severity meets the minimum level
// ─────────────────────────────────────────────────────────────────────────────
bool LogFilter::matchesSeverity(
    const LogEntry& entry,
    const FilterOptions& options)
{
    // Unrecognised severities only appear when no level filter is active
    if (entry.severity == Severity::UNKNOWN)
        return options.minSeverity == Severity::DEBUG;

    return static_cast<int>(entry.severity) >=
        static_cast<int>(options.minSeverity);
}

// ─────────────────────────────────────────────────────────────────────────────
// matchesKeyword — returns true if the message contains the keyword
// Case-insensitive search
// ─────────────────────────────────────────────────────────────────────────────
bool LogFilter::matchesKeyword(
    const LogEntry& entry,
    const FilterOptions& options)
{
    // No keyword filter set
    if (options.keyword.empty()) return true;

    // Convert both to lowercase for case-insensitive search
    std::string message = entry.message;
    std::string keyword = options.keyword;

    std::transform(message.begin(), message.end(),
        message.begin(), ::tolower);
    std::transform(keyword.begin(), keyword.end(),
        keyword.begin(), ::tolower);

    return message.find(keyword) != std::string::npos;
}