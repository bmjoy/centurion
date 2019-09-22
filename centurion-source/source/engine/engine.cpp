#include "engine.h"


Engine::Engine(){
	startMenu = Menu();
	mouse = Mouse();
	game = Game();
	window = myWindow();
	
	image_setup = Image();
	rectangle_setup = FilledRectangle();
	unit_sprite_setup = USprite();
	building_sprite_setup = BSprite();
	font_setup = CBitmapFont();


	nbFrames = 0; bFPS = false;
}


int Engine::launch() {
	window.init();

	GLB::MAIN_WINDOW = window.get_GLFW_window();

	std::cout << "Window has been initialised. \n";

	compile_shaders();
	init_objects();

	lastTime = glfwGetTime();
	bool game_is_created = false;
	bool menu_is_created = false;

	seconds = 0; seconds_str = "00";
	minutes = 0; minutes_str = "00";
	hours = 0; hours_str = "00";

	std::cout << "I am in the while() loop! \n";




	while (!GLB::WINDOW_CLOSE) {

		glfwPollEvents();
		window.clear_buffers();
		fps();
		mouse.mouse_control(window.get_mouse_x(), window.get_mouse_y());
		mouse.apply_projection_matrix(GLB::MENU_PROJECTION);
		
		if (GLB::MAIN_MENU){
			if (!menu_is_created){
				startMenu.create();
				menu_is_created = true;
				std::cout << "Menu has been created! \n";
			}
			startMenu.render();
		}

		if (GLB::GAME) {
			if (!game_is_created) {

				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glfwSwapBuffers(GLB::MAIN_WINDOW);
				
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				tahoma8_text.set_align("center", "middle");
				tahoma8_text.set_position(glm::vec2(GLB::WINDOW_WIDTH/2.f, GLB::WINDOW_HEIGHT / 2.f));
				tahoma8_text.set_text("Game is being created...");
				tahoma8_text.render();
				glfwSwapBuffers(GLB::MAIN_WINDOW);

				game.create();
				game_is_created = true;
				std::cout << "Game has been created! \n";
				lastTime2 = glfwGetTime();
			}			
			
			game.run();	
			calculateTime();

		}


		// ui

		render_ui();

		// mouse
		mouse.render();

		
		glfwSwapBuffers(GLB::MAIN_WINDOW);
		//window.swap_buffers();
		fps_sleep();
	}

	glfwTerminate(); 
	return 0;
}

void Engine::compile_shaders() {
	SHD::IMAGE_SHADER_ID = image_setup.compile();
	SHD::F_RECTANGLE_SHADER_ID = rectangle_setup.compile();
	SHD::USPRITE_SHADER_ID = unit_sprite_setup.compile();
	SHD::BSPRITE_SHADER_ID = building_sprite_setup.compile();
	SHD::FONT_SHADER_ID = font_setup.compile();
}

void Engine::init_objects() {

	mouse.compile();
	mouse.create();
	std::cout << "Mouse is ready!\n";

	div_ui = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	div_ui.apply_projection_matrix(GLB::MENU_PROJECTION);
	div_ui.create(220.f, 200.f, "bottom-left");

	tahoma6_text.create("tahoma_6");
	tahoma8_text.create("tahoma_8");
}


void Engine::fps() {
	currentTime = glfwGetTime();
	nbFrames++;
	std::string Fps = "frames/s = ";
	std::string Mspf = "ms/frame = ";

	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		Fps1 = Fps.append(std::to_string(double(nbFrames)));
		Mspf1 = Mspf.append(std::to_string(1000.0 / double(nbFrames)));
		lastTime += 1.0;
		bFPS = true;

		// here we change the movespeed basing on the "millisecond per frame" measure
		// because if it's a constant, it will change for every PC 
		// depending on the "speed" of the while cycle
		// Obviously we could multiply or add a constant to choose the movespeed
		
		//GAME::CAMERA_MOVESPEED = 1000.0 / double(nbFrames);

		nbFrames = 0;
	}
}

void Engine::calculateTime() {
	if (glfwGetTime() - lastTime2 >= 1.0f) {
		seconds++;
		lastTime2 += 1.0f;
	}
	if (seconds == 60) {
		seconds = 0;
		minutes++;
	}
	if (minutes == 60) {
		minutes = 0;
		hours++;
	}
	if (seconds < 10) { seconds_str = "0" + std::to_string(seconds); }
	else { seconds_str = std::to_string(seconds); }
	if (minutes < 10) { minutes_str = "0" + std::to_string(minutes); }
	else { minutes_str = std::to_string(minutes); }
	if (hours < 10) { hours_str = "0" + std::to_string(hours); }
	else { hours_str = std::to_string(hours); }
}

void Engine::fps_sleep() {
	finalTime = glfwGetTime();
	if (finalTime - currentTime < 1.0 / 60.0) {
		Sleep(1000 * (1.0 / 60.0 - (finalTime - currentTime)));
	}
}

void Engine::render_ui() {

	/* DEBUG */

	if (GLB::DEBUG) {

		div_ui.apply_view_matrix();
		div_ui.set_color(glm::vec4(0.f, 0.f, 0.f, 0.5f));
		div_ui.render(10.f, GAME::UI_BOTTOM_HEIGHT + 10.f);

		if (bFPS) {
			tahoma6_text.set_position(glm::vec2(14.0f, GAME::UI_BOTTOM_HEIGHT + 12.f));
			tahoma6_text.set_text(Fps1);
			tahoma6_text.render();

			tahoma6_text.set_position(glm::vec2(14.0f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f));
			tahoma6_text.set_text(Mspf1);
			tahoma6_text.render();
		}

		if (GLB::GAME) {
			tahoma6_text.set_position(glm::vec2(14.f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f*2.f));
			tahoma6_text.set_text(
				"window position: x = "
				+ std::to_string(GLB::MOUSE_X)
				+ ", y = "
				+ std::to_string(GLB::MOUSE_Y));
			tahoma6_text.render();

			tahoma6_text.set_position(glm::vec2(14.f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f*3.f));
			tahoma6_text.set_text(
				"map position: x = "
				+ std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).x)
				+ ", y = "
				+ std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).y));			
			tahoma6_text.render();

			tahoma6_text.set_position(glm::vec2(14.f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f*4.f));
			tahoma6_text.set_text(
				"selected units: "
				+ std::to_string(game.getSelectedUnits()));
			tahoma6_text.render();
		}

	}


	

	/* TIME */
	tahoma8_text.set_align();
	if (GLB::GAME){
		
		tahoma8_text.set_position(glm::vec2(GLB::WINDOW_WIDTH - 99.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 29.0f));
		tahoma8_text.set_text(hours_str + ":" + minutes_str + ":" + seconds_str);
		tahoma8_text.render(glm::vec4(0.f,0.f,0.f,255.f));

		tahoma8_text.set_position(glm::vec2(GLB::WINDOW_WIDTH - 100.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 28.0f));
		tahoma8_text.set_text(hours_str + ":" + minutes_str + ":" + seconds_str);
		tahoma8_text.render(glm::vec4(255.f));
	}

}

Engine::~Engine()
{
}