#pragma once 

#include <gui>
#include <json.hpp>
#include <object/object.h>
#include <object/building.h>
#include <object/unit.h>
#include <object/decoration.h>
#include <player>
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

