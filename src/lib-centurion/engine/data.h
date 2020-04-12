/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <string>

namespace Engine
{
	namespace Data
	{
		/// <summary>
		/// This function reads all the data from the xml, using the functions below.
		/// </summary>
		void Read(void);

		/// <summary>
		/// This function returns a word of the dictionary, typically ready to be translated
		/// </summary>
		/// <param name="id">The word id.</param>
		/// <returns>The word, of type std::string.</returns>
		std::string GetWordFromDictionaryById(unsigned int id);

		/// <summary>
		/// This function returns the error code corresponding to an error message. 
		/// It could throw an exception if the required error doesn't have a code or if an other error occurs.
		/// </summary>
		/// <param name="name">The error name.</param>
		/// <returns>An error code, of type std::string.</returns>
		std::string GetErrorCodeByName(std::string name);

	};
};
