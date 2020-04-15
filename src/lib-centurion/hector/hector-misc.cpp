#include "hector-misc.h"
#include "hector-lua.h"

namespace Hector
{
	namespace
	{
		struct QuestionWindowInfo
		{
			bool isOpened = false;
			std::string text = "";
			std::string luaCmdYes = "";
			std::string luaCmdNo = "";
		};

		QuestionWindowInfo questionWindowInfo = QuestionWindowInfo();
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

	void EnableQuestionWindow(void)
	{
		questionWindowInfo.isOpened = true;
	}

	void ExposeQuestionWindowText(std::string text)
	{
		questionWindowInfo.text = text;
	}

	bool IsQuestionWindowActive(void)
	{
		return questionWindowInfo.isOpened;
	}

	std::string GetQuestionWindowText(void)
	{
		return questionWindowInfo.text;
	}

	void ResetQuestionWindow(void)
	{
		questionWindowInfo.isOpened = false;
		questionWindowInfo.text = "";
		questionWindowInfo.luaCmdYes = "";
		questionWindowInfo.luaCmdNo = "";
	}

	void SetQuestionWindowYesCmd(std::string cmd)
	{
		questionWindowInfo.luaCmdYes = cmd;
	}

	void SetQuestionWindowNoCmd(std::string cmd)
	{
		questionWindowInfo.luaCmdNo = cmd;
	}

	void ExecuteYesCmdOfQuestionWindow(void)
	{
		Hector::ExecuteCommand(questionWindowInfo.luaCmdYes);
	}

	void ExecuteNoCmdOfQuestionWindow(void)
	{
		Hector::ExecuteCommand(questionWindowInfo.luaCmdNo);
	}
}