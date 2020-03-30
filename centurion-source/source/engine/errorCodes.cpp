#include "errorCodes.h"
#include <logger.h>
#include <file_manager.h>

#include <engine.h>

#include <tinyxml2.h>

map<string, string> ErrorCodes::errorCodes;

void ErrorCodes::SetErrorCode(const string error, const string code)
{
	try
	{
		if (error.size() > 4) 
		{
			if (code.size() == 10)
			{
				errorCodes[error] = code;
			}
			else 
			{
				Logger::LogMessage msg = Logger::LogMessage("The error code string length has to be equal to 10 (e.g. 0x00000001)", "Warn", "", "ErrorCodes", "SetErrorCode");
				Logger::Warn(msg);
			}
		}
		else 
		{
			Logger::LogMessage msg = Logger::LogMessage("The error string is too short", "Warn", "", "ErrorCodes", "SetErrorCode");
			Logger::Warn(msg);
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred setting the code of the following error string: \"" + error + "\"", "Error", "", "ErrorCodes", "SetErrorCode");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

string ErrorCodes::GetErrorCode(const string error)
{
	try
	{
		if (errorCodes.count(error) > 0) 
		{
			return errorCodes[error];
		}
		else 
		{
			Logger::LogMessage msg = Logger::LogMessage("The required error has no code", "Warn", "", "ErrorCodes", "GetErrorCode");
			Logger::Warn(msg);
			return "";
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the code of the following error string: \"" + error + "\"", "Error", "", "ErrorCodes", "GetErrorCode");
		Logger::Error(msg);
		Engine::GameClose();
		throw;
	}
}

void ErrorCodes::ReadErrorCodesXml(void)
{
	try
	{
		string path = Folders::DATA + "ErrorCodes.xml";
		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());
		tinyxml2::XMLElement *levelElement = xmlFile.FirstChildElement("errorCodes");
		for (tinyxml2::XMLElement* child = levelElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{
			string name = string(child->Attribute("name"));
			string code = string(child->Attribute("code"));
			errorCodes[name] = code;
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading \"ErrorCodes.XML\"", "Error", "", "ErrorCodes", "ReadErrorCodesXml");
		Logger::Error(msg);
		Engine::GameClose();
	}
}
