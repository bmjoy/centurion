/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <map>
#include <string>

/// <summary>
/// This class handles the translation in different languages of all the words in the game.
/// </summary>
class TranslationsTable
{
public:
	/// <summary>
	/// This function returns the ID of a specific language available in the game. 
	/// </summary>
	/// <param name="lan">The wanted language.</param>
	/// <returns>The ID of a specific language.</returns>
	static unsigned int GetLanguageId(const std::string lan);
	/// <summary>
	/// This function returns the map of all available languages. 
	/// </summary>
	/// <returns>The map of all available languages.</returns>
	static std::map<std::string, unsigned int> GetLanguagesMap(void);
	/// <summary>
	/// This function returns the translation of a specific word.
	/// </summary>
	/// <param name="string_name">The word you want to translate.</param>
	/// <returns>The translation of a specific word.</returns>
	static std::string GetTranslation(std::string string_name);
	/// <summary>
	/// This function reads the translations table of a specific language from an XML file.
	/// It could throw an exception if an error occurs. 
	/// </summary>
	/// <param name="lang">The wanted language.</param>
	static void ReadTranslationsTableXml(const std::string lang);
private:
	static std::map<std::string, unsigned int> languagesMap;
	static std::map<std::string, std::string> translationsTable;
};
