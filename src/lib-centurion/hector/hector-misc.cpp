#include "hector-misc.h"
#include "hector-lua.h"

namespace Hector
{
	namespace
	{
		struct MessageWindowInfo
		{
			bool isOpened = false;
			std::string text = "";
			std::string luaCmdYes = "";
			std::string luaCmdNo = "";
		};

		MessageWindowInfo messageWindowInfo = MessageWindowInfo();
	};

	void PrCmd(const std::string s)
	{
		std::cout << s << std::endl;
	}

	void ToggleWireframe(void)
	{
		if (Engine::GetEnvironment() == MENU_ENV) return;

		Game::Map::Wireframe = !Game::Map::Wireframe;
		Game::Map::Wireframe ? Logger::Info("Wireframe ON!") : Logger::Info("Wireframe OFF!");
	}

	void ToggleGrid(void)
	{
		if (Engine::GetEnvironment() == MENU_ENV) return;

		if (Game::Map::IsGridEnabled()) Game::Map::DisableGrid();
		else Game::Map::EnableGrid();
		Game::Map::IsGridEnabled() ? Logger::Info("Grid ON!") : Logger::Info("Grid OFF!");
	}

	GObject * Selo(void)
	{
		return Game::GetSelectedObject();
	}

	void EnableMessageWindow(void)
	{
		messageWindowInfo.isOpened = true;
	}

	void ExposeMessageWindowText(std::string text)
	{
		messageWindowInfo.text = text;
	}

	bool IsMessageWindowActive(void)
	{
		return messageWindowInfo.isOpened;
	}

	std::string GetMessageWindowText(void)
	{
		return messageWindowInfo.text;
	}

	void ResetMessageWindow(void)
	{
		messageWindowInfo.isOpened = false;
		messageWindowInfo.text = "";
		messageWindowInfo.luaCmdYes = "";
		messageWindowInfo.luaCmdNo = "";
	}

	void SetMessageWindowYesCmd(std::string cmd)
	{
		messageWindowInfo.luaCmdYes = cmd;
	}

	void SetMessageWindowNoCmd(std::string cmd)
	{
		messageWindowInfo.luaCmdNo = cmd;
	}

	void ExecuteYesCmdOfMessageWindow(void)
	{
		Hector::ExecuteCommand(messageWindowInfo.luaCmdYes);
	}

	void ExecuteNoCmdOfMessageWindow(void)
	{
		Hector::ExecuteCommand(messageWindowInfo.luaCmdNo);
	}
}