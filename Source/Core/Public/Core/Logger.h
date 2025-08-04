// Copyright (c) Eric Jeker. All Rights Reserved.

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#ifndef LOG_LEVEL
#if defined(DEBUG) || defined(_DEBUG)
#define LOG_LEVEL 4
#else
#define LOG_LEVEL 3
#endif
#endif

/**
 * Logger is a utility made for easy logging of messages in the game.
 * It can log messages at different levels such as Error, Warning, Info, and Debug.
 * It can be used in any part of the game to log messages.
 */
namespace Logger
{

enum struct LogLevel
{
	Fatal = 0,
	Error = 1,
	Warning = 2,
	Info = 3,
	Debug = 4,
	Trace = 5
};

// TODO: Potentially, at some point, add the possibility for unlimited arguments
void Fatal(const std::string& message);
void Error(const std::string& message);
void Warn(const std::string& message);
void Info(const std::string& message);
void Debug(const std::string& message);
void Trace(const std::string& message);

}; // namespace Logger

#define LOG_FATAL(message) Logger::Fatal(message)

#if LOG_LEVEL >= 1
#define LOG_ERROR(message) Logger::Error(message)
#else
#define LOG_ERROR(message)
#endif

#if LOG_LEVEL >= 2
#define LOG_WARN(message) Logger::Warn(message)
#else
#define LOG_WARN(message)
#endif

#if LOG_LEVEL >= 3
#define LOG_INFO(message) Logger::Info(message)
#else
#define LOG_INFO(message)
#endif

#if LOG_LEVEL >= 4
#define LOG_DEBUG(message) Logger::Debug(message)
#else
#define LOG_DEBUG(message)
#endif

#if LOG_LEVEL >= 5
#define LOG_TRACE(message) Logger::Trace(message)
#else
#define LOG_TRACE(message)
#endif

#endif
