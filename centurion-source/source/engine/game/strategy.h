#pragma once 

#include <gui>
#include <json.hpp>
#include <classes/object.h>
#include <classes/building.h>
#include <classes/unit.h>
#include <classes/decoration.h>
#include <player/player.h>
#include "match.h"

using namespace std;
//
//
//

// global game variables  --> see source/game/game_functions.cpp

extern vector<string> outposts;
extern array<Player, 8> playersList;

//
//	GAME CLASS --> source/game/game.cpp
//

class Strategy : public Match {
public:
	static void handleKeyboardControls();
	static void reset();
	static void Create();
	static void Run();
	
	~Strategy();
private:
	Strategy();
};

