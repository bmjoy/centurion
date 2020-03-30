#include "translationsTable.h"

#include <logger.h>
#include <file_manager.h>
#include <engine.h>
#include <tinyxml2.h>

#pragma region Static variables

map<string, unsigned int> TranslationsTable::languagesMap;
map<string, string> TranslationsTable::translationsTable;

#pragma endregion

unsigned int TranslationsTable::GetLanguageId(const string lan)
{
	try 
	{
		if (languagesMap.count(lan) > 0) 
		{
			return languagesMap[lan];
		}
		return 0;
	}
	catch (...) 
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the language id", "Error", "", "TranslationsTable", "GetLanguageId");
		Logger::Error(msg);
		Engine::GameClose();
		throw;
	}
}

map<string, unsigned int> TranslationsTable::GetLanguagesMap(void)
{
	return TranslationsTable::languagesMap;
}

string TranslationsTable::GetTranslation(const string string_name)
{
	try 
	{
		if (translationsTable.count(string_name) > 0) 
		{
			return translationsTable[string_name];
		}
		else 
		{
			Logger::LogMessage msg = Logger::LogMessage("Translation of the word \"" + string_name + "\" is missing", "Warn", "TranslationsTable", "GetTranslation");
			Logger::Warn(msg);
			return string_name;
		}
	}
	catch (...) 
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the translation of \"" + string_name + "\"", "Error", "", "TranslationsTable", "GetTranslation");
		Logger::Error(msg);
		return string_name;
	}
}

void TranslationsTable::ReadTranslationsTableXml(const string lang)
{
	try
	{
		if (languagesMap.empty())
		{
			int nLanguages = 0;
			vector<string> filesName = FileManager::GetAllFilesNamesWithinFolder("assets/data");
			for (int i = 0; i < filesName.size(); i++) 
			{
				if (filesName[i].substr(0, filesName[i].find('_')) == "translationTable") 
				{
					string lan = filesName[i].substr(filesName[i].find('_'));
					languagesMap[lan.substr(1, lan.find('.') - 1)] = nLanguages;
					nLanguages++;
				}
			}
		}

		string path = Folders::DATA + "translationTable_" + lang + ".xml";

		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());

		tinyxml2::XMLElement *levelElement = xmlFile.FirstChildElement("translationTable");
		for (tinyxml2::XMLElement* child = levelElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{
			string stringName = string(child->Attribute("stringName"));
			string result = string(child->Attribute("result"));
			translationsTable[stringName] = result;
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred parsing the translations table XML file", "Error", "", "TranslationsTable", "ReadTranslationsTableXml");
		Logger::Error(msg);
		Engine::GameClose();
	}
}
