#pragma once

#include <iostream>
#include <objects.h>


#include "window.h"
#include "mouse.h"

#include "../menu/menu.h"
#include "../game/game.h"
#include "../primitives/image.h"
#include "../primitives/filled_rectangle.h"
#include "../building/building_sprite.h"
#include "../unit/unit_sprite.h"

#include "../gui/div_text.h"
#include "../gui/rectangle.h"
#include "../player/player.h"

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
	DivText text;
	gui::Rectangle background_ui;

	// Players Informations
	std::vector<Player> playersList;

	// fps
	double currentTime, lastTime, finalTime;
	int nbFrames;
	bool bFPS;
	std::string Fps1, Mspf1;

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

