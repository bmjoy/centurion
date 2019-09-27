#pragma once

#include <iostream>
#include <objects.h>


#include "window.h"
#include "mouse.h"

#include "../menu/menu.h"
#include "../game/game.h"
#include "../primitives/image.h"

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
	// compile shaders
	Image image_setup;         // temporary
	//USprite unit_sprite_setup; // temporary

	// objects 
	myWindow window;
	Mouse *mouse;
	Menu *startMenu;
	Game *game;
	DebugUI *debugUI;


	gui::SimpleText text;

	

	// Players Informations
	std::vector<Player> playersList;

	// fps
	double currentTime, lastTime, finalTime;
	int nbFrames;
	int Fps;
	int Mpfs;

	void compile_shaders();  // temporary
	void fps();
	void fps_sleep();	
	void readDataClasses();
};

