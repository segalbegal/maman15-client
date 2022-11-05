#pragma once
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <mutex>

#define ROOT_LOGGER "ROOT"

using std::string;

enum LogLevel
{
	Debug = 0,
	Info = 1,
	Warn = 2,
	Error = 3,
	Critical = 4,
};

struct LoggerInfo
{
	LogLevel level;
	std::ostream* output;
};

class Logging
{
private:
	static std::map<string, std::list<LoggerInfo>> mLoggers;
	static std::map<LogLevel, string> mLevelsStr;

	static std::mutex mLock;

public:
	static void initialize();
	static void uninitialize();

	static void addLogger(const string& loggername = ROOT_LOGGER, const string& filename = "", LogLevel loglevel = LogLevel::Error);

	static void log(const string& message, LogLevel level, const string& loggername = "ROOT_LOGGER");
	static void debug(const string& message, const string& loggername = "ROOT_LOGGER");
	static void info(const string& message, const string& loggername = "ROOT_LOGGER");
	static void warn(const string& message, const string& loggername = "ROOT_LOGGER");
	static void error(const string& message, const string& loggername = "ROOT_LOGGER");
	static void critical(const string& message, const string& loggername = "ROOT_LOGGER");
};