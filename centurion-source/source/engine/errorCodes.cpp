#include "errorCodes.h"
#include "ErrorCodes-xml.hxx"
#include <logger.h>
#include <file_manager.h>

#include <engine.h>

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
		xml_schema::properties props;
		props.no_namespace_schema_location(Folders::XML_SCHEMAS + "errorCodes.xsd");
		string path = "assets/data/ErrorCodes.xml";
		auto_ptr<c_error_codes> errorCodesXML = c_error_codes_(path, 0, props);
		c_error_codes::error_iterator it;
		for (it = errorCodesXML->error().begin(); it != errorCodesXML->error().end(); it++) 
		{
			errorCodes[it->name()] = it->code();
		}
	}
	catch (const xml_schema::exception & e) 
	{
		string errorMsg = (string)e.what();
		Logger::LogMessage msg = Logger::LogMessage(errorMsg, "Error", "", "ErrorCodes", "ReadErrorCodesXml");
		Logger::Error(msg);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading \"ErrorCodes.XML\"", "Error", "", "ErrorCodes", "ReadErrorCodesXml");
		Logger::Error(msg);
		Engine::GameClose();
	}
}
