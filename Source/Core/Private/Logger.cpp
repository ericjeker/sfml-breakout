// Copyright (c) Eric Jeker. All Rights Reserved.

#include "Logger.h"

#include <iostream>
#include <ostream>

namespace
{

void Log(const Logger::LogLevel level, const std::string& message)
{
	// Use std::cerr when the log level is below or equal Error
	auto& oStream = (level <= Logger::LogLevel::Error) ? std::cerr : std::cout;
	switch (level)
	{
		case Logger::LogLevel::Fatal:
			oStream << "[FATAL] " << message << std::endl;
			break;
		case Logger::LogLevel::Error:
			oStream << "[ERROR] " << message << std::endl;
			break;
		case Logger::LogLevel::Warning:
			oStream << "[WARN]  " << message << std::endl;
			break;
		case Logger::LogLevel::Info:
			oStream << "[INFO]  " << message << std::endl;
			break;
		case Logger::LogLevel::Debug:
			oStream << "[DEBUG] " << message << std::endl;
			break;
		case Logger::LogLevel::Trace:
			oStream << "[TRACE] " << message << std::endl;
			break;
	}
}

}

void Logger::Fatal(const std::string& message)
{
	Log(LogLevel::Fatal, message);
}

void Logger::Error(const std::string& message)
{
	Log(LogLevel::Error, message);
}

void Logger::Warn(const std::string& message)
{
	Log(LogLevel::Warning, message);
}

void Logger::Info(const std::string& message)
{
	Log(LogLevel::Info, message);
}

void Logger::Debug(const std::string& message)
{
	Log(LogLevel::Debug, message);
}

void Logger::Trace(const std::string& message)
{
	Log(LogLevel::Trace, message);
}
