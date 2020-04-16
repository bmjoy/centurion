#include "hector-misc.h"
#include "hector-lua.h"

namespace Hector
{
	namespace
	{
		struct QuestionWindowInfo
		{
			bool isOpened = false;
			std::wstring text = L"";
			std::string luaCmdYes = "";
			std::string luaCmdNo = "";
		};

		struct InfoBoxWindowInfo
		{
			bool isOpened = false;
			std::wstring text = L"";
		};

		QuestionWindowInfo questionWindowInfo = QuestionWindowInfo();
		InfoBoxWindowInfo infoboxWindowInfo = InfoBoxWindowInfo();
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

	void ExposeQuestionWindowText(std::wstring text)
	{
		questionWindowInfo.text = text;
	}

	bool IsQuestionWindowActive(void)
	{
		return questionWindowInfo.isOpened;
	}

	std::wstring * GetQuestionWindowText(void)
	{
		return &questionWindowInfo.text;
	}

	void ResetQuestionWindow(void)
	{
		questionWindowInfo.isOpened = false;
		questionWindowInfo.text = L"";
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

	void EnableInfoBoxWindow(void)
	{
		infoboxWindowInfo.isOpened = true;
	}

	void ExposeInfoBoxWindowText(std::wstring text)
	{
		infoboxWindowInfo.text = text;
	}

	bool IsInfoBoxWindowActive(void)
	{
		return infoboxWindowInfo.isOpened;
	}

	std::wstring *GetInfoBoxWindowText(void)
	{
		return &infoboxWindowInfo.text;
	}

	void ResetInfoBoxWindow(void)
	{
		infoboxWindowInfo.isOpened = false;
		infoboxWindowInfo.text = L"";
	}
}