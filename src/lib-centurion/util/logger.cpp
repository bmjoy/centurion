#include <logger.h>
#include <fstream>
#include <settings.h>
#include <iostream>
#include <engine.h>
#include <file_manager.h>

using namespace std;
using namespace glm;

namespace Logger 
{
	// Private members
	namespace {

		std::vector<LogMessage> Messages = std::vector<LogMessage>();
		std::string fileDebugName = "logs/logmessages/Debug " + FileManager::CurrentDateTime("%Y%m%d-%H%M%S") + ".xml";
		std::string fileParamsName = "logs/params/Params " + FileManager::CurrentDateTime("%Y%m%d-%H%M%S") + ".xml";

		/// <summary>
		/// This function adds an instance of a LogMessage into a LogMessages array.
		/// </summary>
		/// <param name="msg">This parameter adds a LogMessage into a LogMessages array.</param>
		void AddMessage(LogMessage msg)
		{
			Messages.push_back(msg);
		};

		/// <summary>
		/// This function prints a LogMessage text into debug console.
		/// </summary>
		/// <param name="msg">This parameter sets a LogMessage that should be printed into debug console.</param>
		void PrintLogMessage(LogMessage msg) {
			if (Settings::DebugIsActive) {
				cout << "[DEBUG] " << msg.text << endl;
			}
		}
	};

	LogMessage::LogMessage(void) { }
	LogMessage::LogMessage(string txt, string typ, string nms, string clss, string mtd) {
		date = FileManager::CurrentDateTime("%Y/%m/%d - %X");
		type = typ;
		text = txt;
		cpp_namespace = nms;
		cpp_class = clss;
		method = mtd;
	}
	LogMessage::~LogMessage(void) { }

	void Info(LogMessage msg) {
		AddMessage(msg);
		PrintLogMessage(msg);
		SaveDebugXML();
	}

	void Info(string msg) {
		LogMessage message = LogMessage(msg, "Info", "", "", "");
		AddMessage(message);
		PrintLogMessage(message);
		SaveDebugXML();
	}

	void Warn(LogMessage msg) {
		AddMessage(msg);
		PrintLogMessage(msg);
		SaveDebugXML();
	}

	void Warn(string msg) {
		LogMessage message = LogMessage(msg, "Warn", "", "", "");
		AddMessage(message);
		PrintLogMessage(message);
		SaveDebugXML();
	}

	void Error(LogMessage msg) {
		AddMessage(msg);
		PrintLogMessage(msg);
		SaveDebugXML();
	}

	void Error(string msg) {
		LogMessage message = LogMessage(msg, "Error", "", "", "");
		AddMessage(message);
		PrintLogMessage(message);
		SaveDebugXML();
	}

	void CleanLogs(void)
	{
		try
		{
			vector<string> debugFiles = FileManager::GetAllFilesNamesWithinFolder("logs/logmessages/");
			if (debugFiles.size() >= 10) {
				FileManager::RemoveFile("logs/logmessages/" + debugFiles[0]);
			}
			vector<string> paramsFiles = FileManager::GetAllFilesNamesWithinFolder("logs/params/");
			if (paramsFiles.size() >= 10) {
				FileManager::RemoveFile("logs/params/" + paramsFiles[0]);
			}
		}
		catch (...)
		{
			Logger::LogMessage msg = LogMessage("An error occurred cleaning the log files", "Error", "", "Logger", "CleanLogs");
			Logger::Error(msg);
			Engine::GameClose();
		}
	}

	void SaveDebugXML(void)
	{
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

	void SaveParamsXML(void)
	{
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
};





