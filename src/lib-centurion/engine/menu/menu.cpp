#include "menu.h"

#include <settings.h>
#include <logger.h>
#include <file_manager.h>
#include <picking.h>
#include <engine.h>
#include <translationsTable.h>

#include <tinyxml2.h>


using namespace std;
using namespace glm;


#pragma region Static variables

array<Menu::MenuPage*, MAX_NUMBER_OF_PAGES> Menu::listOfPages = { nullptr };
bool Menu::isCreated = false;
int Menu::currentPageId = 0;

#pragma endregion


#pragma region Menupage class

Menu::MenuPage::MenuPage(void) {}

string Menu::MenuPage::GetPageName(void)
{
	return this->pageName;
}

unsigned int Menu::MenuPage::Create(const string name)
{
	string fileName = Folders::INTERFACE_MENU + name;
	listOfButtons = vector<gui::Button>();
	listOfImages = vector<gui::Image>();

	tinyxml2::XMLDocument xmlFile;
	xmlFile.LoadFile(fileName.c_str());

	tinyxml2::XMLElement *_buttons = xmlFile.FirstChildElement("menuPage")->FirstChildElement("buttons");
	for (tinyxml2::XMLElement* child = _buttons->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		try
		{
			string btn_name = string(child->Attribute("name"));
			string image_name = string(child->Attribute("image_name"));
			string x = string(child->Attribute("x"));
			string y = string(child->Attribute("y"));
			string onclick = string(child->Attribute("onclick"));

			gui::Button btn = gui::Button();
			btn.create(image_name, TranslationsTable::GetTranslation(btn_name), stoi(x), stoi(y), PickingUI::ObtainPickingID(), glm::vec4(0.f, 0.f, 0.f, 255.f), onclick);
			this->AddButton(btn);
		}
		catch (const std::exception&)
		{
			string msg = "An error occurred creating a button on menu page \"" + name + "\"";
			Logger::LogMessage logmsg = Logger::LogMessage(msg, "Error", "", "Menu::MenuPage", "Create");
			Logger::Error(logmsg);
			throw;
		}
	}

	tinyxml2::XMLElement *_images = xmlFile.FirstChildElement("menuPage")->FirstChildElement("images");
	for (tinyxml2::XMLElement* child = _images->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		try
		{
			string img_name = string(child->Attribute("name"));
			string image_name = string(child->Attribute("image_name"));
			string x = string(child->Attribute("x"));
			string y = string(child->Attribute("y"));
			string onclick = string(child->Attribute("onclick"));
			string size = string(child->Attribute("size"));
			string align = string(child->Attribute("align"));

			int clickable = int(onclick != "");
			gui::Image img = gui::Image(image_name);
			if (size == "auto") {
				img.create(align, stof(x), stof(y), 0.f, 0.f, PickingUI::ObtainPickingID() * clickable);
			}
			else if (size == "max") {
				img.create(align, stof(x), stof(y), Engine::myWindow::Width, Engine::myWindow::Height, PickingUI::ObtainPickingID() * clickable);
			}
			this->AddImage(img);
		}
		catch (const std::exception&)
		{
			string msg = "An error occurred creating an image on menu page \"" + name + "\"";
			Logger::LogMessage logmsg = Logger::LogMessage(msg, "Error", "", "Menu::MenuPage", "Create");
			Logger::Error(logmsg);
			throw;
		}
	}
	pageName = (string)xmlFile.FirstChildElement("menuPage")->Attribute("name");
	int id;
	try
	{
		string idStr = (string)xmlFile.FirstChildElement("menuPage")->Attribute("id");
		id = stoi(idStr);
	}
	catch (const std::exception&)
	{
		string msg = "An error occurred reading the id of the menu page \"" + name + "\"";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Error", "", "Menu::MenuPage", "Create");
		Logger::Error(logmsg);
		throw;
	}
	return id;
}

void Menu::MenuPage::Render(const bool picking)
{
	for (int i = 0; i < listOfImages.size(); i++)
	{
		listOfImages[i].render(picking);
	}
	for (int i = 0; i < listOfButtons.size(); i++)
	{
		listOfButtons[i].render(picking, Picking::leftClickID_UI);
	}
}

void Menu::MenuPage::AddButton(const gui::Button btn)
{
	listOfButtons.push_back(btn);
}

void Menu::MenuPage::AddImage(const gui::Image img)
{
	listOfImages.push_back(img);
}

Menu::MenuPage::~MenuPage(void) {}

#pragma endregion



void Menu::RenderPage(const unsigned int id, const bool picking)
{
	listOfPages[id]->Render(picking);
}

void Menu::OpenMenuPage(const unsigned int id)
{
	currentPageId = id;
	Engine::Mouse::LeftClick = false;
}

bool Menu::IsCreated(void)
{
	return Menu::isCreated;
}

void Menu::AddMenuPage(const unsigned int id, MenuPage * mp)
{
	Menu::listOfPages[id] = mp;
}

void Menu::Reset(void)
{
}

void Menu::Clear(void)
{
	for (int i = 0; i < MAX_NUMBER_OF_PAGES; i++)
	{
		if (listOfPages[i] != nullptr)
		{
			delete listOfPages[i];
		}
		listOfPages[i] = nullptr;
	}
}

void Menu::Create(void)
{
	try
	{
		vector<string> files = FileManager::GetAllFilesNamesWithinFolder(Folders::INTERFACE_MENU, "xml");

		for (vector<string>::iterator it = files.begin(); it != files.end(); it++)
		{

			MenuPage* mpage = new MenuPage();
			int pageId = mpage->Create(*it);

			AddMenuPage(pageId, mpage);
		}

		isCreated = true;
		currentPageId = 0;
		applyMenuMatrices();
	}
	catch (const std::exception&)
	{
		Engine::GameClose();
	}
}

void Menu::Run(void)
{
	if (IsCreated() == false) {
		//Audio()->MusicPlay("assets/music/menu.ogg");
		Menu::Create();
		Logger::Info("Main menu has been created!");
	}

	// picking
	Picking::leftClickID_UI = 0;
	RenderPage(currentPageId, true);
	if (Engine::Mouse::LeftClick)
		Picking::leftClickID_UI = Picking::GetIdFromClick();

	// rendering
	RenderPage(currentPageId, false);
}

Menu::~Menu(void)
{
	this->Clear();
}

