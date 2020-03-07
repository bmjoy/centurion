#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

using namespace std;

class Logger
{
public:
	class LogMessage
	{
	public:
		LogMessage();
		LogMessage(string txt, string typ = "Info");
		~LogMessage();
		string date, type, text;
	};
	static void Info(LogMessage msg);
	static void Info(string msg);
	static void SaveDebugXML();
	static void SaveParamsXML();
	static void AddMessage(LogMessage msg) { Messages.push_back(msg); }
	static void PrintLogMessage(LogMessage msg);
	~Logger();
private:
	static vector<LogMessage> Messages;
	static string fileDebugName;
	static string fileParamsName;
	Logger();
};

#endif