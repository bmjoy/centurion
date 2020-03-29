#include "menu.h"

#include <settings.h>
#include <logger.h>
#include <file_manager.h>
#include <picking.h>
#include <engine.h>

#include "menuPage-xml.hxx"
#include <translationsTable.h>

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

	xml_schema::properties props;
	props.no_namespace_schema_location(Folders::XML_SCHEMAS + "menuPage.xsd");
	auto_ptr<c_menuPage> dataXML = c_menuPage_(fileName, 0, props);

	c_buttons::c_button_iterator _it_btn;
	for (_it_btn = dataXML->c_buttons().c_button().begin(); _it_btn != dataXML->c_buttons().c_button().end(); _it_btn++)
	{
		gui::Button btn = gui::Button();
		btn.create(
			string(_it_btn->image_name()),
			TranslationsTable::GetTranslation(string(_it_btn->name())),
			int(_it_btn->x()),
			int(_it_btn->y()),
			PickingUI::ObtainPickingID(),
			glm::vec4(0.f, 0.f, 0.f, 255.f),
			string(_it_btn->onclick())
		);
		this->AddButton(btn);
	}

	c_images::c_image_iterator _it_img;
	for (_it_img = dataXML->c_images().c_image().begin(); _it_img != dataXML->c_images().c_image().end(); _it_img++)
	{
		gui::Image img = gui::Image(string(_it_img->image_name()));
		int clickable = int(string(_it_img->onclick()) != "");
		if (string(_it_img->size()) == "auto") {
			img.create(
				string(_it_img->align()),
				float(_it_img->x()),
				float(_it_img->y()),
				0.f, 0.f, PickingUI::ObtainPickingID() * clickable
			);
		}
		else if (string(_it_img->size()) == "max") {
			img.create(
				string(_it_img->align()),
				float(_it_img->x()),
				float(_it_img->y()),
				Engine::myWindow::Width, Engine::myWindow::Height, PickingUI::ObtainPickingID() * clickable
			);
		}
		this->AddImage(img);
	}

	pageName = dataXML->name();
	return int(dataXML->id());
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
	catch (const xml_schema::exception & e) 
	{
		std::cout << e << std::endl;
	}
	catch (const std::exception&)
	{
		Engine::GameClose();
	}
}

void Menu::Run(void)
{
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

