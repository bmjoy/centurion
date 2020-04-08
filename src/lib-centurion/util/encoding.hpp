/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef ENCODING_H
#define ENCODING_H

#include <string>

namespace Encode {

	static std::size_t GetStringSize(const char* src)
	{
		std::string_view s(src);
		return s.size();
	}

	static std::wstring GetWideString(const char* src) {

		std::wstring ws = L"";
		std::size_t len = GetStringSize(src);

		for (int i = 0; i < len; i++) {

			int u = (int)src[i];
			if (u < 0) u = 256 + u;

			if (u <= 126) {
				ws += (wchar_t)u;
			}
			else if (u > 126 && i < len - 1) {
				int u2 = (int)src[i + 1];
				if (u2 < 0) u2 = 256 + u2;
				ws += (wchar_t)u2 + 64;
				i++;
			}
		}

		return ws;
	}

	static const char* HexCharToBin(char c)
	{
		// TODO handle default / error
		switch (toupper(c))
		{
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		}
		return "0000";
	}

	static std::string HexStrToBinStr(const std::string& hex)
	{
		// TODO use a loop from <algorithm> or smth
		std::string bin;
		for (unsigned i = 0; i != hex.length(); ++i)
			bin += HexCharToBin(hex[i]);
		return bin;
	}
};

#endif 