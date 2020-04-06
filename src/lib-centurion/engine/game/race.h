/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <string>
#include <vector>
#include <map>

namespace Game {
	/// <summary>
	/// (???) Classe da rivedere e forse da posizionare altrove perche deve poter interagire con Object.h.
	/// </summary>
	class Race
	{
	public:
		Race();
		void setRaceProperties(int p1, std::string p2, std::string p3, std::string p4) { p_id = p1; name = p2; zone = p3; t_class = p4; }
		int getRaceId() { return p_id; };
		std::string getRaceName() { return name; }
		std::string getEnvironmentalZone() { return zone; }
		std::string getFoodTransportClass() { return t_class; }
		~Race();
	private:
		int p_id;
		std::string name;
		std::string zone;
		std::string t_class;
	};

	extern std::vector<std::string> racesName;
	extern std::map<std::string, Race> races;
};
