#pragma once 

#include <ui.h>
#include <json.hpp>
#include <classes/object.h>
#include <classes/building.h>
#include <classes/unit.h>
#include <classes/decoration.h>
#include <player/player.h>
#include "match.h"

using namespace std;
// global game variables  --> see source/game/game_functions.cpp

/// <summary>
/// (???) Da rivedere --> le variabili extern non sono il massimo nella programmazione.
/// </summary>
extern vector<string> outposts;
extern array<Player, 8> playersList;


/// <summary>
/// (???) Classe probabilemnte da rivedere, completare.
/// </summary>
class Strategy : public Match 
{
public:
	static void HandleKeyboardControls(void);
	static void Reset(void);
	static void Create(void);
	static void Run(void);
	
	~Strategy(void);
private:
	Strategy(void);
};

