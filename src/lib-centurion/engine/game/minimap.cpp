#include "minimap.h"

#include <minimap_rectangle.h>
#include <engine.h>
#include "game.h"

using namespace std;
using namespace glm;

bool Game::Minimap::isCreated = false;
bool Game::Minimap::isActive = false;
bool Game::Minimap::isBlocked = false;
gui::Rectangle Game::Minimap::minimapRectangle;

Game::Minimap::Minimap(void) {
	minimapRectangle = gui::Rectangle();
}

void Game::Minimap::Create(void)
{
	applyGameMatrices(&projectionMatrix, &viewMatrix);
	Map::Render(false);
	RenderObjects();
	MMRectangle()->update();
	isCreated = true;
}

void Game::Minimap::RenderMapAndObjects(void)
{
	if (IsCreated() == false) {
		Minimap::Create();
	}
	MMRectangle()->render();
}

void Game::Minimap::RenderRectangle(void)
{
	minimapRectangle.render(vec4(), vec4(255.f));
}

void Game::Minimap::Update(void)
{
	Game::Minimap::isCreated = false;
}

bool Game::Minimap::IsCreated(void)
{
	return Game::Minimap::isCreated;
}

void Game::Minimap::Enable(void)
{
	Game::Minimap::isActive = true;

	float x = Engine::Camera::GetXPosition() / MEDIUM_MAP_WIDTH * Engine::myWindow::Width;
	float y = Engine::Camera::GetYPosition() / MEDIUM_MAP_HEIGHT * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) + Engine::myWindow::BottomBarHeight;
	float w = Engine::myWindow::WidthZoomed * Engine::myWindow::Width / MEDIUM_MAP_WIDTH;
	float h = Engine::myWindow::HeightZoomed * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) / MEDIUM_MAP_HEIGHT;
	x = std::max(x, 1.f);
	y = std::max(y, Engine::myWindow::BottomBarHeight + 1.f);
	y = std::min(y, Engine::myWindow::Height - Engine::myWindow::TopBarHeight - h);
	minimapRectangle.create("border", x, y, w, h, "bottom-left", 0);
}

void Game::Minimap::Disable(void)
{
	Game::Minimap::isActive = false;
}

bool Game::Minimap::IsActive(void)
{
	return Game::Minimap::isActive;
}

bool Game::Minimap::IsBlocked(void)
{
	return Game::Minimap::isBlocked;
}

void Game::Minimap::Block(void)
{
	Game::Minimap::isBlocked = true;
}

void Game::Minimap::Unblock(void)
{
	Game::Minimap::isBlocked = false;
}

Game::Minimap::~Minimap(void) {}