/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <string>
#include <map>

class ErrorCodes
{
public:
	/// <summary>
	/// This function sets an error code. It throws an exception if the error code doesn't have a length equal to 10.
	/// </summary>
	/// <param name="error">The error message.</param>
	/// <param name="code">An error code. It's length should be equal to 10.</param>
	static void SetErrorCode(const std::string error, const std::string code);
	/// <summary>
	/// This function returns the error code corresponding to an error message. 
	/// It could throw an exception if the required error doesn't have a code or if an other error occurs.
	/// </summary>
	/// <param name="error">The error message.</param>
	/// <returns>An erro code.</returns>
	static std::string GetErrorCode(const std::string error);
	/// <summary>
	/// This function reads some errors code from an XML file. 
	/// It could throw an exception if some error occurs during the reading from file. 
	/// </summary>
	static void ReadErrorCodesXml(void);

private:
	static std::map<std::string, std::string> errorCodes;
};
