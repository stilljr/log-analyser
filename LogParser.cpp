#include "LogParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// parseFile — opens the log file and parses every valid line
// ─────────────────────────────────────────────────────────────────────────────
std::vector<LogEntry> LogParser::parseFile(const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open log file: " + filepath);
    }

    std::vector<LogEntry> entries;
    std::string           line;
    int                   lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;

        if (isValidLine(line))
        {
            entries.push_back(parseLine(line, lineNumber));
        }
    }

    file.close();

    std::cout << "  Parsed " << entries.size()
        << " entries from " << filepath << "\n";

    return entries;
}

// ─────────────────────────────────────────────────────────────────────────────
// parseLine — extracts date, time, severity, and message from one log line
//
// Expected format:
//   2024-01-15 07:23:11 INFO  Application started successfully
// ─────────────────────────────────────────────────────────────────────────────
LogEntry LogParser::parseLine(const std::string& line, int lineNumber)
{
    LogEntry    entry;
    std::istringstream stream(line);

    entry.lineNumber = lineNumber;

    // Extract date, time, severity
    stream >> entry.date >> entry.time >> entry.rawSeverity;

    // Convert severity string to enum
    entry.severity = LogEntry::stringToSeverity(
        trim(entry.rawSeverity)
    );

    // Everything remaining on the line is the message
    std::getline(stream, entry.message);
    entry.message = trim(entry.message);

    return entry;
}

// ─────────────────────────────────────────────────────────────────────────────
// isValidLine — returns true if the line has the expected log format
// ─────────────────────────────────────────────────────────────────────────────
bool LogParser::isValidLine(const std::string& line)
{
    // Must be long enough to contain date, time, severity, message
    if (line.length() < 25) return false;

    // Must start with a date in YYYY-MM-DD format
    if (line[4] != '-' || line[7] != '-') return false;

    // Must have a space after the date
    if (line[10] != ' ') return false;

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// trim — removes leading and trailing whitespace from a string
// ─────────────────────────────────────────────────────────────────────────────
std::string LogParser::trim(const std::string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");

    if (start == std::string::npos) return "";

    return s.substr(start, end - start + 1);
}