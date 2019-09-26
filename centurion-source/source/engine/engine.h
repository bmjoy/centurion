#pragma once

#include <iostream>
#include <objects.h>


#include "window.h"
#include "mouse.h"

#include "../menu/menu.h"
#include "../game/game.h"
#include "../primitives/image.h"
#include "../unit/unit_sprite.h"

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
	Image image_setup;
	USprite unit_sprite_setup;

	// objects 
	myWindow window;
	Mouse mouse;
	Menu *startMenu;
	Game *game;
	gui::SimpleText text;
	gui::SimpleText timeText;
	//gui::Rectangle background_ui;

	DebugUI debugUI;

	// Players Informations
	std::vector<Player> playersList;

	// fps
	double currentTime, lastTime, finalTime;
	int nbFrames;
	int Fps;
	int Mpfs;

	//time
	double lastTime2;
	int seconds;
	std::string seconds_str;
	int minutes;
	std::string minutes_str;
	int hours;
	std::string hours_str;

	void compile_shaders();
	void init_objects();
	void fps();
	void calculateTime();
	void fps_sleep();	
	void render_ui();
	void readDataClasses();
};

