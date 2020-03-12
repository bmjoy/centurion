#include "translationsTable.h"
#include "translationsTable-xml.hxx"

#include <logger.h>
#include <file_manager.h>

#pragma region Static variables

map<string, int> TranslationsTable::languagesMap;
map<string, string> TranslationsTable::translationsTable;

#pragma endregion

unsigned int TranslationsTable::GetLanguageId(string lan)
{
	try {
		if (languagesMap.count(lan) > 0) {
			return languagesMap[lan];
		}
		return 0;
	}
	catch (...) {
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the language id", "Error", "", "TranslationsTable", "GetLanguageId");
		Logger::Error(msg);
		throw;
	}
}

string TranslationsTable::GetTranslation(string string_name)
{
	try {
		if (translationsTable.count(string_name) > 0) {
			return translationsTable[string_name];
		}
		else {
			Logger::LogMessage msg = Logger::LogMessage("Translation of the word \"" + string_name + "\" is missing", "Warn", "TranslationsTable", "GetTranslation");
			Logger::Warn(msg);
			throw;
		}
	}
	catch (...) {
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the translation of \"" + string_name + "\"", "Error", "", "TranslationsTable", "GetTranslation");
		Logger::Error(msg);
		throw;
	}
}

void TranslationsTable::ReadTranslationsTableXml(string lang)
{
	try
	{
		if (languagesMap.empty()) {
			int nLanguages = 0;
			vector<string> filesName = FileManager::GetAllFilesNamesWithinFolder("assets/data");
			for (int i = 0; i < filesName.size(); i++) {
				if (filesName[i].substr(0, filesName[i].find('_')) == "translationTable") {
					string lan = filesName[i].substr(filesName[i].find('_'));
					languagesMap[lan.substr(1, lan.find('.') - 1)] = nLanguages;
					nLanguages++;
				}
			}
		}

		string path = "assets/data/translationTable_" + lang + ".xml";
		auto_ptr<translationTable> tTable = translationTable_(path);
		translationTable::entry_iterator it;
		for (it = tTable->entry().begin(); it != tTable->entry().end(); it++) {
			translationsTable[it->stringName()] = it->result();
		}
	}
	catch (const xml_schema::exception & e) {
		string errorMsg = (string)e.what();
		Logger::LogMessage msg = Logger::LogMessage(errorMsg, "Error", "", "TranslationsTable", "ReadTranslationsTableXml");
		Logger::Error(msg);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred parsing the translations table XML file", "Error", "", "TranslationsTable", "ReadTranslationsTableXml");
		Logger::Error(msg);
		throw;
	}
}
