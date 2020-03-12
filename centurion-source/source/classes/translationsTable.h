#ifndef TRANSLATIONTABLE_H
#define TRANSLATIONTABLE_H

#include <map>
#include <string>

using namespace std;

class TranslationsTable
{
public:
	static unsigned int GetLanguageId(string lan);
	static map<string, int> GetLanguagesMap(void) { return languagesMap; }
	static string GetTranslation(string string_name);
	static void ReadTranslationsTableXml(string lang);
private:
	static map<string, int> languagesMap;
	static map<string, string> translationsTable;
};


#endif