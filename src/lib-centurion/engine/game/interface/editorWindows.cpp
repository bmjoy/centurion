#include "editorWindows.h"
#include <file_manager.h>
#include <logger.h>
#include <settings.h>
#include <picking.h>
#include <hector-lua.h>

#include <engine.h>
#include <translationsTable.h>

#include <encoding.hpp>

using namespace std;
using namespace glm;

namespace EditorWindows
{
	// Private variables
	namespace
	{
		std::array<EditorWindow*, MAX_NUMBER_OF_EDITOR_WINDOWS> listOfWindows = { nullptr };
		bool isHidden = false;
		bool bWindowsOpened = false;
	};


	bool EditorWindows::EditorWindow::IsOpened(void)
	{
		return this->isOpened;
	}

	void EditorWindows::EditorWindow::Open(void)
	{
		EditorWindows::CloseEveryWindow();
		this->isOpened = true;
		this->iframe.Open();
		EditorWindows::bWindowsOpened = true;
	}

	void EditorWindows::EditorWindow::Toggle(void)
	{
		if (this->isOpened == false)
		{
			EditorWindows::CloseEveryWindow();
			this->Open();
		}
		else
		{
			this->Close();
		}
	}

	void EditorWindows::EditorWindow::Close(void)
	{
		this->iframe.Close();
		this->isOpened = false;
		EditorWindows::bWindowsOpened = false;
	}

	void EditorWindows::EditorWindow::Clear(void)
	{
		iframe.Clear();
		EditorWindows::bWindowsOpened = false;
	}

	void EditorWindows::EditorWindow::Create(gui::Iframe _iframe)
	{
		iframe = _iframe;
		isOpened = false;
	}

	void EditorWindows::EditorWindow::Render(const bool picking)
	{
		iframe.Render(picking);
	}

#pragma endregion

	void EditorWindows::OpenWindow(const unsigned int id)
	{
		if (id < 0 || id > MAX_NUMBER_OF_EDITOR_WINDOWS) return;
		if (listOfWindows[id] == nullptr) return;

		listOfWindows[id]->Open();
	}

	void EditorWindows::CloseWindow(const unsigned int id)
	{
		if (id < 0 || id > MAX_NUMBER_OF_EDITOR_WINDOWS) return;
		if (listOfWindows[id] == nullptr) return;

		listOfWindows[id]->Close();
	}

	void EditorWindows::ToggleWindow(const unsigned int id)
	{
		if (id < 0 || id > MAX_NUMBER_OF_EDITOR_WINDOWS) return;
		if (listOfWindows[id] == nullptr) return;

		listOfWindows[id]->Toggle();
	}

	void EditorWindows::Create(void)
	{
		try
		{
			string path = Folders::INTERFACE_EDITOR + "editorWindows.xml";
			tinyxml2::XMLDocument xmlFile;
			xmlFile.LoadFile(path.c_str());

			for (tinyxml2::XMLElement* _it_wind = xmlFile.FirstChildElement("editorWindows")->FirstChildElement(); _it_wind != NULL; _it_wind = _it_wind->NextSiblingElement())
			{
				EditorWindow* eWind = new EditorWindow();
				int id = _it_wind->IntAttribute("id");

				gui::Iframe iframe = gui::Iframe();

				tinyxml2::XMLElement* iframeXML = _it_wind->FirstChildElement("iframe");
				bool bIframeSuccess = iframe.CreateFromXmlElement(iframeXML);

				if (bIframeSuccess)
				{
					eWind->Create(iframe);
					gui::Iframe::AddIframe(iframe.GetId(), eWind->GetIframePtr());
					if (iframe.IsOpened()) eWind->Open();
					AddWindow(id, eWind);
				}

			}
			isHidden = false;
		}
		catch (const std::exception&)
		{
			Engine::GameClose();
		}
	}

	void EditorWindows::Render(const bool picking)
	{
		if (isHidden) return;

		for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++) {
			if (listOfWindows[i] != nullptr) {
				listOfWindows[i]->Render(picking);
			}
		}
	}

	void Hide(void)
	{
		isHidden = true;
	}

	void Show(void)
	{
		isHidden = false;
	}


	void EditorWindows::Clear(void)
	{
		for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++)
		{
			if (listOfWindows[i] != nullptr)
			{
				listOfWindows[i]->Clear();
				delete listOfWindows[i];
			}
			listOfWindows[i] = nullptr;
		}
	}

	bool EditorWindows::AnyWindowIsOpened(void)
	{
		for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++)
		{
			if (listOfWindows[i] != nullptr)
			{
				if (listOfWindows[i]->IsOpened()) return true;
			}
		}
		return false;
	}

	void EditorWindows::CloseEveryWindow(void)
	{
		for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++)
		{
			if (listOfWindows[i] != nullptr)
			{
				listOfWindows[i]->Close();
			}
		}
	}

	bool IsHidden(void)
	{
		return isHidden;
	}

	void EditorWindows::AddWindow(const unsigned int id, EditorWindow* win)
	{
		if (id < 0 || id >= MAX_NUMBER_OF_EDITOR_WINDOWS) return;
		EditorWindows::listOfWindows[id] = win;
	}
	EditorWindow* GetWindowById(const unsigned int id)
	{
		if (id < 0 || id >= MAX_NUMBER_OF_EDITOR_WINDOWS) return nullptr;
		return EditorWindows::listOfWindows[id];
	}

	bool IsThereAnyWindowOpen(void)
	{
		return bWindowsOpened;
	}
};

