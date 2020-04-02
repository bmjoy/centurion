#include "translationsTable.h"

#include <logger.h>
#include <file_manager.h>
#include <engine.h>
#include <tinyxml2.h>
#include <sstream>

#include <encoding.hpp>

using namespace std;
using namespace glm;

#pragma region Static variables

map<string, unsigned int> TranslationsTable::languagesMap;
map<string, wstring> TranslationsTable::translationsTable;

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
			return std::string(translationsTable[string_name].begin(), translationsTable[string_name].end());
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

std::wstring TranslationsTable::GetWTranslation(std::string string_name)
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
			return std::wstring(string_name.begin(), string_name.end());
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the translation of \"" + string_name + "\"", "Error", "", "TranslationsTable", "GetTranslation");
		Logger::Error(msg);
		return std::wstring(string_name.begin(), string_name.end());
	}
}

//int utf8_to_codepoint(const wchar_t wchar)
//{
//	wstring u(&wchar);
//	int l = u.length();
//	if (l < 1) return -1; unsigned char u0 = u[0]; if (u0 >= 0 && u0 <= 127) return u0;
//	if (l < 2) return -1; unsigned char u1 = u[1]; if (u0 >= 192 && u0 <= 223) return (u0 - 192) * 64 + (u1 - 128);
//	if (u[0] == 0xed && (u[1] & 0xa0) == 0xa0) return -1; //code points, 0xd800 to 0xdfff
//	if (l < 3) return -1; unsigned char u2 = u[2]; if (u0 >= 224 && u0 <= 239) return (u0 - 224) * 4096 + (u1 - 128) * 64 + (u2 - 128);
//	if (l < 4) return -1; unsigned char u3 = u[3]; if (u0 >= 240 && u0 <= 247) return (u0 - 240) * 262144 + (u1 - 128) * 4096 + (u2 - 128) * 64 + (u3 - 128);
//	return -1;
//}
//
//std::wstring s2ws(const std::string& src)
//{
//	std::wstring res = L"";
//	size_t const wcs_len = mbstowcs(NULL, src.c_str(), 0);
//	std::vector<wchar_t> buffer(wcs_len + 1);
//	mbstowcs(&buffer[0], src.c_str(), src.size());
//	res.assign(buffer.begin(), buffer.end() - 1);
//	
//	std::wostringstream oss;
//	for (int i = 0; i < src.size(); i++) {
//		int codepoint = utf8_to_codepoint(src[i]);
//		
//		oss << static_cast<int>(codepoint) << L": " << codepoint;
//		wcout << static_cast<int>(codepoint) << L": " << codepoint << endl;
//	}
//
//	return oss.str();
//}

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
			wstring result = encode::GetWideString(child->Attribute("result"));
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
