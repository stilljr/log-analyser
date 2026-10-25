#pragma once

#include <string>
#include <vector>
#include "LogEntry.h"

// ─────────────────────────────────────────────────────────────────────────────
// LogParser — reads a log file and returns a vector of parsed LogEntry objects
// ─────────────────────────────────────────────────────────────────────────────
class LogParser
{
public:

    // ── Reads a log file and returns all parsed entries ───────────────────
    static std::vector<LogEntry> parseFile(const std::string& filepath);

    // ── Parses a single line into a LogEntry ──────────────────────────────
    static LogEntry parseLine(const std::string& line, int lineNumber);

    // ── Returns true if the line is a valid log entry ─────────────────────
    static bool isValidLine(const std::string& line);

private:

    // ── Trims whitespace from both ends of a string ───────────────────────
    static std::string trim(const std::string& s);
};