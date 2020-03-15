#ifndef ERRORCODES_H
#define ERRORCODES_H

#include <string>
#include <map>

using namespace std;

class ErrorCodes {
public:
	static void SetErrorCode(string error, string code);
	static string GetErrorCode(string error);
	static void ReadErrorCodesXml(void);

private:
	static map<string, string> errorCodes;
};

#endif