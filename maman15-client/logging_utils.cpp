#define _CRT_SECURE_NO_WARNINGS

#include "logging_utils.h"
#include <fstream>
#include <chrono>

#define DEBUG "DEBUG"
#define INFO "INFO"
#define WARN "WARN"
#define ERROR "ERROR"
#define CRITICAL "CRITICAL"

#define FIELDS_SEPARATOR "\t"

std::map<string, std::list<LoggerInfo>> Logging::mLoggers;
std::map<LogLevel, string> Logging::mLevelsStr;
std::mutex Logging::mLock;

void Logging::initialize()
{
	Logging::mLevelsStr[LogLevel::Debug] = DEBUG;
	Logging::mLevelsStr[LogLevel::Info] = INFO;
	Logging::mLevelsStr[LogLevel::Warn] = WARN;
	Logging::mLevelsStr[LogLevel::Error] = ERROR;
	Logging::mLevelsStr[LogLevel::Critical] = CRITICAL;
}

void Logging::uninitialize()
{
	for (auto& output : Logging::mLoggers)
	{
		for (auto& logger : output.second)
		{
			delete logger.output;
		}
		output.second.clear();
	}
	Logging::mLoggers.clear();
}

void Logging::addLogger(const string& loggername, const string& filename, LogLevel loglevel)
{
	LoggerInfo info;
	info.level = loglevel;
	if (filename.length() == 0)
	{
		info.output = &std::cout;
	}
	else
	{
		info.output = new std::ofstream(filename, std::ios::app);
	}
	Logging::mLoggers[loggername].push_back(info);
}

void Logging::log(const string& message, LogLevel level, const string& loggername)
{
	for (auto& logger : Logging::mLoggers[loggername])
	{
		if (level >= logger.level)
		{
			auto now = std::chrono::system_clock::now();
			auto end_time = std::chrono::system_clock::to_time_t(now);
			auto time_str = string(std::ctime(&end_time));
			time_str = time_str.substr(0, time_str.length() - 1);

			mLock.lock();
			*logger.output
				<< time_str << FIELDS_SEPARATOR
				<< loggername << FIELDS_SEPARATOR
				<< Logging::mLevelsStr[level] << FIELDS_SEPARATOR
				<< message << std::endl;
			mLock.unlock();
		}
	}
}

void Logging::debug(const string& message, const string& loggername)
{
	Logging::log(message, LogLevel::Debug, loggername);
}

void Logging::info(const string& message, const string& loggername)
{
	Logging::log(message, LogLevel::Info, loggername);
}

void Logging::warn(const string& message, const string& loggername)
{
	Logging::log(message, LogLevel::Warn, loggername);
}

void Logging::error(const string& message, const string& loggername)
{
	Logging::log(message, LogLevel::Error, loggername);
}

void Logging::critical(const string& message, const string& loggername)
{
	Logging::log(message, LogLevel::Critical, loggername);
}
