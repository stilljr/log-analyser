# Log Analyser

A C++17 command-line tool that parses application log files and
filters them by date range, severity level and keyword, then
produces a clean report with a severity summary.

## Problem It Solves
Application logs get large quickly. Finding the errors from a
particular day, or every line mentioning a database, means
scrolling or writing throwaway scripts. This tool does it with
one command.

## Features
- Parses log lines in the format `YYYY-MM-DD HH:MM:SS LEVEL message`
- Filters by date range, minimum severity, or keyword
- Case-insensitive keyword search
- Errors-only mode
- Summary of entry counts by severity
- Optional report saved to a file
- Clear error messages for a missing file or an invalid level

## Technologies
C++17, standard library only (fstream, sstream, algorithm, map)

## Project Structure

    LogEntry.h        data structure and severity enum
    LogParser.h/.cpp  reads the file and parses each line
    LogFilter.h/.cpp  applies date, severity and keyword filters
    LogReporter.h/.cpp builds the report and summary
    log-analyser.cpp  command-line entry point
    sample.log        sample data for testing

## Build
Open `log-analyser.slnx` in Visual Studio, set the C++ language
standard to C++17, and build (Ctrl+Shift+B).

## Usage

    log-analyser.exe <logfile> [options]

Options:

    --from   <date>     Start date (YYYY-MM-DD)
    --to     <date>     End date (YYYY-MM-DD)
    --level  <level>    Minimum severity: DEBUG, INFO, WARN, ERROR
    --find   <keyword>  Search messages for a keyword
    --errors            Show errors only
    --output <file>     Save the report to a file

Examples:

    log-analyser.exe sample.log --errors
    log-analyser.exe sample.log --level WARN
    log-analyser.exe sample.log --find "database"
    log-analyser.exe sample.log --level ERROR --output report.txt

## Author
G&H Tech Group - ghtechgroup.net