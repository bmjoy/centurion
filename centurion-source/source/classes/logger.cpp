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

Logger::Logger() { }

void Logger::Info(LogMessage msg) { }

void Logger::Info(string msg) {
	LogMessage message = LogMessage(msg);
	AddMessage(message);
	PrintLogMessage(message);
	SaveDebugXML();
}

void Logger::SaveDebugXML() {
	ofstream logFile(fileDebugName);
	if (logFile.is_open()) {
		logFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl <<
			"<logger>" << endl <<
			"\t<logMessages>" << endl;
		for (int i = 0; i < Messages.size(); i++) {
			logFile << "\t\t<logMessage>" << endl <<
				"\t\t\t<date>" << Messages[i].date << "</date>" << endl <<
				"\t\t\t<type>" << Messages[i].type << "</type>" << endl <<
				"\t\t\t<text>" << Messages[i].text << "</text>" << endl <<
				"\t\t</logMessage>" << endl;
		}
		logFile << "\t</logMessages>" << endl;
		logFile << "</logger>" << endl;
	}
	logFile.close();
}

void Logger::SaveParamsXML() {
	ofstream logFile(fileParamsName);
	if (logFile.is_open()) {
		logFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<logger>\n\t<params>\n" <<
			"\t\t<camera>\n" <<
			"\t\t\t<xPosition>" << Camera::GetXPosition() << "</xPosition>\n" <<
			"\t\t\t<yPosition>" << Camera::GetYPosition() << "</yPosition>\n" <<
			"\t\t</camera>\n" <<
			"\t\t<mouse>\n" <<
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
			"\t\t</mouse>\n" <<
			"\t\t<window>\n" <<
			"\t\t\t<heightZoomed>" << myWindow::HeightZoomed << "</heightZoomed>\n" <<
			"\t\t\t<ratio>" << myWindow::Ratio << "</ratio>\n" <<
			"\t\t\t<widthZoomed>" << myWindow::WidthZoomed << "</widthZoomed>\n" <<
			"\t\t\t<shouldClose>" << myWindow::ShouldClose << "</shouldClose>\n" <<
			"\t\t\t<bottomBarHeight>" << myWindow::BottomBarHeight << "</bottomBarHeight>\n" <<
			"\t\t\t<topBarHeight>" << myWindow::TopBarHeight << "</topBarHeight>\n" <<
			"\t\t</window>\n\t</params>\n" <<
			"</logger>";
	}
	logFile.close();
}

void Logger::PrintLogMessage(LogMessage msg) {
	if (glb::Settings::DebugIsActive) {
		cout << "[DEBUG] " << msg.text << endl;
	}
}

Logger::~Logger() { }

#pragma region LogMessage
Logger::LogMessage::LogMessage() { }

Logger::LogMessage::LogMessage(string txt, string typ) {
	date = currentDateTime("%Y/%m/%d - %X");
	type = typ;
	text = txt;
}

Logger::LogMessage::~LogMessage() { }
#pragma endregion