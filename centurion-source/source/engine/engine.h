#pragma once

#include <iostream>
#include <objects.h>

#include "window.h"
#include "mouse.h"

#include "../menu/menu.h"
#include "../game/game.h"
#include "../editor/editor.h"
#include "../audio/audio_manager.h"

#include "../gui/simple_text.h"
#include "../gui/rectangle.h"
#include "../player/player.h"

#include "../interface/debug_ui.h"

class Engine
{
public:
	Engine();
	int launch();
	~Engine();

private:
	// objects 
	myWindow window;
	Mouse *mouse;
	Menu *startMenu;
	Game *game;
	Editor *editor;
	DebugUI *debugUI;

	gui::SimpleText text;

	//Audio Manager settings
	AudioManager audioManager;

	// Players Informations
	std::vector<Player> playersList;

	// fps
	double currentTime, lastTime, finalTime;
	int nbFrames;
	int Fps;
	int Mpfs;

	void fps();
	void fps_sleep();	
	void readDataClasses();
};

