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

namespace encode {

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
			else if (u > 126 && i < len - 2) {
				int u2 = (int)src[i + 1];
				if (u2 < 0) u2 = 256 + u2;
				ws += (wchar_t)u2 + 64;
				i++;
			}
		}

		return ws;
	}
};

#endif 