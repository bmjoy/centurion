#include <logger.h>
#include <fstream>
#include <settings.h>
#include <iostream>
#include <engine.h>
#include <file_manager.h>

vector<Logger::LogMessage> Logger::Messages;
string Logger::fileDebugName = "logs/debug/Debug " + FileManager::CurrentDateTime("%Y%m%d-%H%M%S") + ".xml";
string Logger::fileParamsName = "logs/params/Params " + FileManager::CurrentDateTime("%Y%m%d-%H%M%S") + ".xml";

#pragma region Logger

Logger::Logger() { }

void Logger::Info(LogMessage msg) {
	AddMessage(msg);
	PrintLogMessage(msg);
	SaveDebugXML();
}

void Logger::Info(string msg) {
	LogMessage message = LogMessage(msg);
	AddMessage(message);
	PrintLogMessage(message);
	SaveDebugXML();
}

void Logger::Warn(LogMessage msg) {
	AddMessage(msg);
	PrintLogMessage(msg);
	SaveDebugXML();
}

void Logger::Warn(string msg) {
	LogMessage message = LogMessage(msg, "Warning");
	AddMessage(message);
	PrintLogMessage(message);
	SaveDebugXML();
}

void Logger::Error(LogMessage msg) {
	AddMessage(msg);
	PrintLogMessage(msg);
	SaveDebugXML();
}

void Logger::Error(string msg) {
	LogMessage message = LogMessage(msg, "Error");
	AddMessage(message);
	PrintLogMessage(message);
	SaveDebugXML();
}

void Logger::CleanLogs(void)
{
	try
	{
		vector<string> debugFiles = FileManager::GetAllFilesNamesWithinFolder("logs/debug/");
		if (debugFiles.size() >= 10) {
			FileManager::RemoveFile("logs/debug/" + debugFiles[0]);
		}
		vector<string> paramsFiles = FileManager::GetAllFilesNamesWithinFolder("logs/params/");
		if (paramsFiles.size() >= 10) {
			FileManager::RemoveFile("logs/params/" + paramsFiles[0]);
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred cleaning the log files", "", "Logger", "CleanLogs");
		Logger::Error(msg);
		Engine::GameClose();
	}	
}

void Logger::SaveDebugXML() {
	//Saving all debug informations
	ofstream logFile(fileDebugName);
	if (logFile.is_open()) {
		logFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl <<
			"<Log>" << endl <<
			"\t<LogMessages>" << endl;
		for (int i = 0; i < Messages.size(); i++) {
			logFile << "\t\t<LogMessage>" << endl <<
				"\t\t\t<Date>" << Messages[i].date << "</Date>" << endl <<
				"\t\t\t<Type>" << Messages[i].type << "</Type>" << endl <<
				"\t\t\t<CppNamespace>" << Messages[i].cpp_namespace << "</CppNamespace>" << endl <<
				"\t\t\t<CppClass>" << Messages[i].cpp_class << "</CppClass>" << endl <<
				"\t\t\t<Method>" << Messages[i].method << "</Method>" << endl <<
				"\t\t\t<Text>" << Messages[i].text << "</Text>" << endl <<
				"\t\t</LogMessage>" << endl;
		}
		logFile << "\t</LogMessages>" << endl;
		logFile << "</Log>" << endl;
	}
	logFile.close();
}

void Logger::SaveParamsXML() {
	//Saving all parameters
	ofstream logFile(fileParamsName);
	if (logFile.is_open()) {
		logFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Log>\n\t<Params>\n" <<
			"\t\t<Camera>\n" <<
			"\t\t\t<xPosition>" << Engine::Camera::GetXPosition() << "</xPosition>\n" <<
			"\t\t\t<yPosition>" << Engine::Camera::GetYPosition() << "</yPosition>\n" <<
			"\t\t</Camera>\n" <<
			"\t\t<Mouse>\n" <<
			"\t\t\t<scrollValue>" << Engine::Mouse::ScrollValue << "</scrollValue>\n" <<
			"\t\t\t<xLeftClick>" << Engine::Mouse::GetXLeftClick() << "</xLeftClick>\n" <<
			"\t\t\t<xPosition>" << Engine::Mouse::GetXPosition() << "</xPosition>\n" <<
			"\t\t\t<xRightClick>" << Engine::Mouse::GetXRightClick() << "</xRightClick>\n" <<
			"\t\t\t<y2dPosition>" << Engine::Mouse::GetY2DPosition() << "</y2dPosition>\n" <<
			"\t\t\t<y2dRightClick>" << Engine::Mouse::GetY2DRightClick() << "</y2dRightClick>\n" <<
			"\t\t\t<yLeftClick>" << Engine::Mouse::GetYLeftClick() << "</yLeftClick>\n" <<
			"\t\t\t<yPosition>" << Engine::Mouse::GetYPosition() << "</yPosition>\n" <<
			"\t\t\t<yRightClick>" << Engine::Mouse::GetYRightClick() << "</yRightClick>\n" <<
			"\t\t\t<leftClick>" << Engine::Mouse::LeftClick << "</leftClick>\n" <<
			"\t\t\t<leftHold>" << Engine::Mouse::LeftHold << "</leftHold>\n" <<
			"\t\t\t<release>" << Engine::Mouse::Release << "</release>\n" <<
			"\t\t\t<rightClick>" << Engine::Mouse::RightClick << "</rightClick>\n" <<
			"\t\t\t<scrollBool>" << Engine::Mouse::ScrollBool << "</scrollBool>\n" <<
			"\t\t</Mouse>\n" <<
			"\t\t<Window>\n" <<
			"\t\t\t<heightZoomed>" << Engine::myWindow::HeightZoomed << "</heightZoomed>\n" <<
			"\t\t\t<ratio>" << Engine::myWindow::Ratio << "</ratio>\n" <<
			"\t\t\t<widthZoomed>" << Engine::myWindow::WidthZoomed << "</widthZoomed>\n" <<
			"\t\t\t<shouldClose>" << Engine::myWindow::ShouldClose << "</shouldClose>\n" <<
			"\t\t\t<bottomBarHeight>" << Engine::myWindow::BottomBarHeight << "</bottomBarHeight>\n" <<
			"\t\t\t<topBarHeight>" << Engine::myWindow::TopBarHeight << "</topBarHeight>\n" <<
			"\t\t</Window>\n\t</Params>\n" <<
			"</Log>";
	}
	logFile.close();
}

void Logger::PrintLogMessage(LogMessage msg) {
	if (Settings::DebugIsActive) {
		cout << "[DEBUG] " << msg.text << endl;
	}
}

Logger::~Logger() { }

#pragma endregion

#pragma region LogMessage
Logger::LogMessage::LogMessage() { }

Logger::LogMessage::LogMessage(string txt, string typ, string nms, string clss, string mtd) {
	date = FileManager::CurrentDateTime("%Y/%m/%d - %X");
	type = typ;
	text = txt;
	cpp_namespace = nms;
	cpp_class = clss;
	method = mtd;
}

Logger::LogMessage::~LogMessage() { }
#pragma endregion