/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

/// <summary>
/// This class handles logging system.
/// </summary>
class Logger
{
public:

	/// <summary>
	/// This class instances a complete log message of a specified function or method.
	/// </summary>
	class LogMessage
	{
	public:
		LogMessage();
		LogMessage(std::string txt, std::string typ = "Info", std::string nms = "", std::string clss = "", std::string mtd = "");
		~LogMessage();
		std::string date, type, cpp_namespace, cpp_class, method, text;
	};

	/// <summary>
	/// This function creates an instance of LogMessage about game informations.
	/// </summary>
	/// <param name="msg">This parameter can only accept an instance of LogMessage.</param>
	static void Info(LogMessage msg);

	/// <summary>
	/// This function creates an instance of LogMessage about game informations, using a std::string.
	/// </summary>
	/// <param name="msg">This parameter sets a LogMessage information message from a std::string.</param>
	static void Info(std::string msg);

	/// <summary>
	/// This function creates an instance of LogMessage about game warnings.
	/// </summary>
	/// <param name="msg">This parameter can only accept an instance of LogMessage.</param>
	static void Warn(LogMessage msg);

	/// <summary>
	/// This function creates an instance of LogMessage about game warnings, using a std::string.
	/// </summary>
	/// <param name="msg">This parameter sets a LogMessage warning message from a std::string.</param>
	static void Warn(std::string msg);

	/// <summary>
	/// This function creates an instance of LogMessage about game errors.
	/// </summary>
	/// <param name="msg">This parameter can only accept an instance of LogMessage.</param>
	static void Error(LogMessage msg);

	/// <summary>
	/// This function creates an instance of LogMessage about game errors, using a std::string.
	/// </summary>
	/// <param name="msg">This parameter sets a LogMessage error message from a std::string.</param>
	static void Error(std::string msg);

	/// <summary>
	/// This function clear the entire Log file.
	/// </summary>
	static void CleanLogs(void);

	/// <summary>
	/// This function saves last debug information into a xml. It should be used each time a LogMessage is instanced.
	/// </summary>
	static void SaveDebugXML();

	/// <summary>
	/// This function saves game parameters into a xml when the application is closed or interrupted.
	/// </summary>
	static void SaveParamsXML();

	/// <summary>
	/// This function adds an instance of a LogMessage into a LogMessages array.
	/// </summary>
	/// <param name="msg">This parameter adds a LogMessage into a LogMessages array.</param>
	static void AddMessage(LogMessage msg) { Messages.push_back(msg); }

	/// <summary>
	/// This function prints a LogMessage text into debug console.
	/// </summary>
	/// <param name="msg">This parameter sets a LogMessage that should be printed into debug console.</param>
	static void PrintLogMessage(LogMessage msg);
	~Logger();
private:
	static std::vector<LogMessage> Messages;
	static std::string fileDebugName;
	static std::string fileParamsName;
	Logger();
};

#endif