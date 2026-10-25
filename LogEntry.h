#pragma once

#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Severity levels for log entries — ordered from lowest to highest
// ─────────────────────────────────────────────────────────────────────────────
enum class Severity
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    UNKNOWN
};

// ─────────────────────────────────────────────────────────────────────────────
// Represents a single parsed line from a log file
// ─────────────────────────────────────────────────────────────────────────────
struct LogEntry
{
    std::string date;        // e.g. "2024-01-15"
    std::string time;        // e.g. "07:23:11"
    Severity    severity;    // parsed severity level
    std::string rawSeverity; // original string e.g. "ERROR"
    std::string message;     // the log message text
    int         lineNumber;  // line number in the original file

    // ── Helper — convert Severity enum to display string ──────────────────
    static std::string severityToString(Severity s)
    {
        switch (s)
        {
        case Severity::DEBUG:   return "DEBUG";
        case Severity::INFO:    return "INFO ";
        case Severity::WARN:    return "WARN ";
        case Severity::ERROR:   return "ERROR";
        default:                return "UNKN ";
        }
    }

    // ── Helper — convert string to Severity enum ──────────────────────────
    static Severity stringToSeverity(const std::string& s)
    {
        if (s == "DEBUG") return Severity::DEBUG;
        if (s == "INFO")  return Severity::INFO;
        if (s == "WARN")  return Severity::WARN;
        if (s == "ERROR") return Severity::ERROR;
        return Severity::UNKNOWN;
    }
};
