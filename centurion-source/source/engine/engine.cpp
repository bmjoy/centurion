#include "engine.h"


Engine::Engine(){
	mouse = Mouse();
	window = myWindow();
	
	image_setup = Image();
	f_rectangle_setup = FilledRectangle();
	e_rectangle_setup = EmptyRectangle();
	unit_sprite_setup = USprite();
	//building_sprite_setup = BSprite();
	//font_setup = CBitmapFont();

	nbFrames = 0; bFPS = false;
}


int Engine::launch() {
	window.init();

	GLB::MAIN_WINDOW = window.get_GLFW_window();

	std::cout << "Window has been initialised. \n";

	obj::init();

	readDataClasses();
	compile_shaders();
	init_objects();

	lastTime = glfwGetTime();

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
			if (!GLB::MENU_IS_CREATED){
				startMenu = new Menu();
				startMenu->create(&playersList);
				GLB::MENU_IS_CREATED = true;
				std::cout << "Menu has been created! \n";

			}
			startMenu->render();
		}

		if (GLB::GAME) {
			
			if (!GLB::GAME_IS_CREATED) {

				delete startMenu;

				game = new Game();
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glfwSwapBuffers(GLB::MAIN_WINDOW);
				
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				text.set_position(glm::vec2(GLB::WINDOW_WIDTH/2.f, GLB::WINDOW_HEIGHT / 2.f));
				text.set_text("Game is being created...");
				text.render("tahoma_8", glm::vec4(255.f), "center", "middle");
				glfwSwapBuffers(GLB::MAIN_WINDOW);

				game->create(&playersList);
				GLB::GAME_IS_CREATED = true;
				std::cout << "Game has been created! \n";
				lastTime2 = glfwGetTime();

			}			
			
			game->run();	
			calculateTime();

		}
		if (GLB::GAME_CLEAR) {
			delete game;
			GLB::GAME_CLEAR = false;
			GLB::GAME_IS_CREATED = false;
			GLB::MENU_IS_CREATED = false;
			GLB::GAME = false;
			GLB::MAIN_MENU = true;
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
	SHD::F_RECTANGLE_SHADER_ID = f_rectangle_setup.compile();
	SHD::E_RECTANGLE_SHADER_ID = e_rectangle_setup.compile();
	SHD::USPRITE_SHADER_ID = unit_sprite_setup.compile();
	//SHD::BSPRITE_SHADER_ID = building_sprite_setup.compile();
	//SHD::FONT_SHADER_ID = font_setup.compile();
}

void Engine::readDataClasses() {

	std::vector<std::string> files = get_all_files_names_within_folder("assets/data/classes");
	for (int i = 0; i < files.size(); ++i) {
		
		std::ifstream path("assets/data/classes/" + files[i]);
		json dataClass = json::parse(path);

		if (dataClass["type"] == "building") {			
			obj::BuildingSprite()->addPath(dataClass["ent_path"]);
		}
	}	
}

void Engine::init_objects() {

	obj::compile();
	obj::create();

	mouse.compile();
	mouse.create();
	std::cout << "Mouse is ready!\n";

	div_ui = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	div_ui.apply_projection_matrix(GLB::MENU_PROJECTION);
	div_ui.create(220.f, 200.f, "bottom-left");

	text = DivText();

	//tahoma6_text.create("tahoma_6");
	//tahoma8_text.create("tahoma_8");
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
			text.set_position(glm::vec2(14.0f, GAME::UI_BOTTOM_HEIGHT + 12.f));
			text.set_text(Fps1);
			text.render("tahoma_6", glm::vec4(255.f), "left", "normal");

			text.set_position(glm::vec2(14.0f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f));
			text.set_text(Mspf1);
			text.render("tahoma_6", glm::vec4(255.f), "left", "normal");
		}

		if (GLB::GAME) {
			text.set_position(glm::vec2(14.f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f*2.f));
			text.set_text(
				"window position: x = "
				+ std::to_string(GLB::MOUSE_X)
				+ ", y = "
				+ std::to_string(GLB::MOUSE_Y));
			text.render("tahoma_6", glm::vec4(255.f), "left", "normal");

			text.set_position(glm::vec2(14.f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f*3.f));
			text.set_text(
				"map position: x = "
				+ std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).x)
				+ ", y = "
				+ std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).y));			
			text.render("tahoma_6", glm::vec4(255.f), "left", "normal");

			text.set_position(glm::vec2(14.f, GAME::UI_BOTTOM_HEIGHT + 12.f + 15.f*4.f));
			text.set_text(
				"selected units: "
				+ std::to_string(game->getSelectedUnits()));
			text.render("tahoma_6", glm::vec4(255.f), "left", "normal");
		}

	}


	

	/* TIME */
	if (GLB::GAME){
		
		text.set_position(glm::vec2(GLB::WINDOW_WIDTH - 99.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 29.0f));
		text.set_text(hours_str + ":" + minutes_str + ":" + seconds_str);
		text.render("tahoma_8", glm::vec4(0.f,0.f,0.f,255.f), "left", "normal");

		text.set_position(glm::vec2(GLB::WINDOW_WIDTH - 100.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 28.0f));
		text.set_text(hours_str + ":" + minutes_str + ":" + seconds_str);
		text.render("tahoma_8", glm::vec4(255.f), "left", "normal");
	}

}

Engine::~Engine()
{
}