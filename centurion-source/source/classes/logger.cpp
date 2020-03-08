#include <logger.h>
#include <fstream>
#include <settings.h>
#include <iostream>
#include <global>
#include <engine/camera.h>
#include <engine/mouse.h>
#include <engine/window.h>

using namespace glb;
using namespace engine;

vector<Logger::LogMessage> Logger::Messages;
string Logger::fileDebugName = "logs/Debug " + currentDateTime("%Y%m%d-%H%M%S") + ".xml";
string Logger::fileParamsName = "logs/Params " + currentDateTime("%Y%m%d-%H%M%S") + ".xml";

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

void Logger::SaveDebugXML() {
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
	ofstream logFile(fileParamsName);
	if (logFile.is_open()) {
		logFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Log>\n\t<Params>\n" <<
			"\t\t<Camera>\n" <<
			"\t\t\t<xPosition>" << Camera::GetXPosition() << "</xPosition>\n" <<
			"\t\t\t<yPosition>" << Camera::GetYPosition() << "</yPosition>\n" <<
			"\t\t</Camera>\n" <<
			"\t\t<Mouse>\n" <<
			"\t\t\t<scrollValue>" << Mouse::ScrollValue << "</scrollValue>\n" <<
			"\t\t\t<xLeftClick>" << Mouse::GetXLeftClick() << "</xLeftClick>\n" <<
			"\t\t\t<xPosition>" << Mouse::GetXPosition() << "</xPosition>\n" <<
			"\t\t\t<xRightClick>" << Mouse::GetXRightClick() << "</xRightClick>\n" <<
			"\t\t\t<y2dPosition>" << Mouse::GetY2DPosition() << "</y2dPosition>\n" <<
			"\t\t\t<y2dRightClick>" << Mouse::GetY2DRightClick() << "</y2dRightClick>\n" <<
			"\t\t\t<yLeftClick>" << Mouse::GetYLeftClick() << "</yLeftClick>\n" <<
			"\t\t\t<yPosition>" << Mouse::GetYPosition() << "</yPosition>\n" <<
			"\t\t\t<yRightClick>" << Mouse::GetYRightClick() << "</yRightClick>\n" <<
			"\t\t\t<leftClick>" << Mouse::LeftClick << "</leftClick>\n" <<
			"\t\t\t<leftHold>" << Mouse::LeftHold << "</leftHold>\n" <<
			"\t\t\t<release>" << Mouse::Release << "</release>\n" <<
			"\t\t\t<rightClick>" << Mouse::RightClick << "</rightClick>\n" <<
			"\t\t\t<scrollBool>" << Mouse::ScrollBool << "</scrollBool>\n" <<
			"\t\t</Mouse>\n" <<
			"\t\t<Window>\n" <<
			"\t\t\t<heightZoomed>" << myWindow::HeightZoomed << "</heightZoomed>\n" <<
			"\t\t\t<ratio>" << myWindow::Ratio << "</ratio>\n" <<
			"\t\t\t<widthZoomed>" << myWindow::WidthZoomed << "</widthZoomed>\n" <<
			"\t\t\t<shouldClose>" << myWindow::ShouldClose << "</shouldClose>\n" <<
			"\t\t\t<bottomBarHeight>" << myWindow::BottomBarHeight << "</bottomBarHeight>\n" <<
			"\t\t\t<topBarHeight>" << myWindow::TopBarHeight << "</topBarHeight>\n" <<
			"\t\t</Window>\n\t</Params>\n" <<
			"</Log>";
	}
	logFile.close();
}

void Logger::PrintLogMessage(LogMessage msg) {
	if (glb::Settings::DebugIsActive) {
		cout << "[DEBUG] " << msg.text << endl;
	}
}

Logger::~Logger() { }

#pragma endregion

#pragma region LogMessage
Logger::LogMessage::LogMessage() { }

Logger::LogMessage::LogMessage(string txt, string typ, string nms, string clss, string mtd) {
	date = currentDateTime("%Y/%m/%d - %X");
	type = typ;
	text = txt;
	cpp_namespace = nms;
	cpp_class = clss;
	method = mtd;
}

Logger::LogMessage::~LogMessage() { }
#pragma endregion