#include "data.h"
#include "header.h"

#include <file_manager.h>
#include <logger.h>

namespace Engine
{
	namespace Data
	{
		// Private variables
		namespace
		{
			std::string DICTIONARY_PATH = "dictionary.xml";
			std::string ERRORCODES_PATH = "errorCodes.xml";

			std::unordered_map<unsigned int, std::string> DICTIONARY;
			std::unordered_map<std::string, std::string> ERRORCODES;
		};

		/// <summary>
		/// This function sets an error code. It throws an exception if the error code doesn't have a length equal to 10.
		/// </summary>
		/// <param name="error">The error message.</param>
		/// <param name="code">An error code. It's length should be equal to 10.</param>
		void SetErrorCode(const std::string error, const std::string code);

		/// <summary>
		/// This function reads assets/data/errorcodes.xml
		/// </summary>
		void ReadErrorCodes(void);

		/// <summary>
		/// This function reads assets/data/dictionary.xml
		/// </summary>
		void ReadDictionary(void);


		void Read(void)
		{
			DICTIONARY_PATH = Folders::DATA + DICTIONARY_PATH;
			ERRORCODES_PATH = Folders::DATA + ERRORCODES_PATH;

			ReadErrorCodes();
			ReadDictionary();
		}

		void ReadErrorCodes(void)
		{
			try
			{
				tinyxml2::XMLDocument xmlFile;
				xmlFile.LoadFile(ERRORCODES_PATH.c_str());
				tinyxml2::XMLElement* levelElement = xmlFile.FirstChildElement("errorCodes");
				for (tinyxml2::XMLElement* child = levelElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
				{
					std::string name = child->Attribute("name");
					std::string code = child->Attribute("code");
					SetErrorCode(name, code);
				}
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading \"ErrorCodes.XML\"", "Error", "Engine::Data", "", "ReadErrorCodes");
				Logger::Error(msg);
				throw;
			}
		}

		void ReadDictionary(void)
		{
			try
			{
				tinyxml2::XMLDocument xmlFile;
				xmlFile.LoadFile(DICTIONARY_PATH.c_str());
				tinyxml2::XMLElement* levelElement = xmlFile.FirstChildElement("dictionary");
				for (tinyxml2::XMLElement* child = levelElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
				{
					unsigned int id = (unsigned int)child->IntAttribute("id");
					std::string value = child->Attribute("value");
					DICTIONARY[id] = value;
				}
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading \"dictionary.XML\"", "Error", "Engine::Data", "", "ReadDictionary");
				Logger::Error(msg);
				throw;
			}
		}

		std::string GetWordFromDictionaryById(unsigned int id)
		{
			return DICTIONARY[id];
		}

		std::string GetErrorCodeByName(std::string name)
		{
			return ERRORCODES[name];
		}

		void SetErrorCode(const std::string error, const std::string code)
		{
			try
			{
				if (error.size() > 4)
				{
					if (code.size() == 10)
					{
						ERRORCODES[error] = code;
					}
					else
					{
						Logger::LogMessage msg = Logger::LogMessage("The error code string length has to be equal to 10 (e.g. 0x00000001)", "Warn", "Engine::Data", "", "SetErrorCode");
						Logger::Warn(msg);
					}
				}
				else
				{
					Logger::LogMessage msg = Logger::LogMessage("The error string is too short", "Warn", "Engine::Data", "", "SetErrorCode");
					Logger::Warn(msg);
				}
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred setting the code of the following error string: \"" + error + "\"", "Error", "Engine::Data", "", "SetErrorCode");
				Logger::Error(msg);
				throw;
			}
		}

	};
};