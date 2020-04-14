#include "object.h"

#include <engine.h>
#include <data.h>
#include <player/player.h>
#include <picking.h>
#include <game/strategy.h>
#include <pathfinding/pathfinding.h>
#include <game/editor.h>
#include <game/pass.h>
#include <logger.h>
#include "game/interface/editorUi.h"
#include <translationsTable.h>

using namespace std;
using namespace glm;

#pragma region Static GObject properties:
unordered_map<std::string, unsigned int> GObject::idNamesMap;
#pragma endregion

GObject::GObject(void)
{
	this->bSelected = false;
	this->idName = "";
}

unsigned short int GObject::GetPlayer(void) const
{
	return this->playerID;
}

void GObject::SetPlayer(const unsigned short int par_playerID)
{
	// DA SISTEMARE !
	this->player = &playersList[par_playerID];
}

bool GObject::IsSelected(void)
{
	return this->bSelected;
}

void GObject::Select(void)
{
	this->bSelected = true;
}

unsigned int GObject::GetPickingID(void) const
{
	return this->pickingID;
}

void GObject::SetPickingID(const unsigned int par_pickingID)
{
	this->pickingID = par_pickingID;
	int r = (par_pickingID & 0x000000FF) >> 0;
	int g = (par_pickingID & 0x0000FF00) >> 8;
	int b = (par_pickingID & 0x00FF0000) >> 16;
	pickingColor = vec3(r / 255.f, g / 255.f, b / 255.f);
}

float GObject::GetSelectionRadius(void) const
{
	return this->selectionRadius;
}

void GObject::SetSelectionRadius(const float par_selectionRadius)
{
	this->selectionRadius = par_selectionRadius >= 0 ? par_selectionRadius : 0;
}

string GObject::GetClassName(void) const 
{
	return this->className;
}

void GObject::SetClassName(string parClassName) 
{
	this->className = parClassName;
}

string GObject::GetType(void) const
{
	return this->type;
}

void GObject::SetType(const string par_type)
{
	this->type = par_type;
}

void GObject::SetSingularName(const string par_singularName)
{
	this->singularName = par_singularName;
}

string GObject::GetSingularName(void) const
{
	return this->singularName;
}

void GObject::SetPluralName(const string par_pluralName)
{
	this->pluralName = par_pluralName;
}

string GObject::GetPluralName(void) const
{
	return this->pluralName;
}

void GObject::SetDisplayedName(const std::string _displayedName)
{
	this->displayedName = _displayedName;
}

std::string GObject::GetDisplayedName(void) const
{
	return (this->displayedName != singularName) ? displayedName : "";
}

void GObject::SetIDName(const std::string _idName)
{
	//Checks if identifcation name belogs to an other existing object.
	if (_idName != "" && GObject::idNamesMap.count(_idName) >= 1)
	{
		if(GObject::idNamesMap[_idName] != this->GetPickingID())
		{
			Logger::Warn("Identification name " + _idName + " belongs to another object (ID = " + to_string(pickingID) + ")");
			return;
		}
	}
	GObject::idNamesMap.erase(this->GetIDName()); //Avoid object with two different keys.

	this->idName = _idName;
	if(_idName != "")
		GObject::idNamesMap[_idName] = this->GetPickingID();; //Assign or replace identification name.

}

std::string GObject::GetIDName(void) const
{
	return this->idName;
}

unsigned int GObject::GetRace(void) const
{
	return this->race;
}
string GObject::GetRaceName(void) const
{
	return this->raceName;
}

void GObject::SetRaceName(const string par_raceName)
{
	this->raceName = par_raceName;
}
void GObject::SetRace(const unsigned short int par_race)
{
	this->race = par_race;
}

unsigned int GObject::GetSight(void) const
{
	return this->sight;
}
void GObject::SetSight(const unsigned short int par_sight)
{
	this-> sight = par_sight;
}

bool GObject::CanBePositionedIntoWater(void) const
{
	return this->bIsWaterObject;
}
void GObject::AllowPositioningIntoWater(void)
{
	this->bIsWaterObject = true;
}
void GObject::DenyPositioningIntoWater(void)
{
	this->bIsWaterObject = false;
}

bool GObject::GetCanBeClonedInEditor(void) const
{
	return this->canBeClonedInEditor;
}
void GObject::SetCanBeClonedInEditor(const bool par_canBeClonedInEditor)
{
	this->canBeClonedInEditor = par_canBeClonedInEditor;
}

bool GObject::GetAlwaysVisibleInGameMinimap(void) const
{
	return this->bAlwaysVisibleInGameMinimap;
}
void GObject::SetAlwaysVisibleInGameMinimap(const bool par_alwaysVisibleInGameMinimap)
{
	this->bAlwaysVisibleInGameMinimap = par_alwaysVisibleInGameMinimap;
}

bool GObject::IsBuilding(void)
{
	return (this->type == "cpp_buildingclass");
}
bool GObject::IsUnit(void)
{
	return this->type == "unit";
}
bool GObject::IsDecoration(void)
{
	return (this->type == "decoration");
}

Building* GObject::AsBuilding(void)
{ 
	return (Building*)this; 
}
Unit* GObject::AsUnit(void)
{ 
	return (Unit*)this; 
}
Decoration* GObject::AsDecoration(void)
{ 
	return (Decoration*)this;
}

void GObject::UpdatePass(void)
{
	Pass::UpdateObjectPass(this->pass_grid, this->position, PASS_OVERLAP);
}

void GObject::ClearPass(void) 
{
	Pass::UpdateObjectPass(this->pass_grid, this->position, PASS_CLEAR);
}

void GObject::MarkAsMoving(void)
{
	this->bIsBeingMoved = true;
}

void GObject::MarkAsNotMoving(void)
{
	this->bIsBeingMoved = false;
}

bool GObject::IsBeingMoved(void)
{
	return this->bIsBeingMoved;
}

bool GObject::IsPlaceable(void)
{
	return this->bIsPlaceable;
}

bool GObject::IsSaved(void)
{
	return bIsSaved;
}

void GObject::MarkAsSaved(void)
{
	bIsSaved = true;
}

void GObject::MarkAsNotSaved(void)
{
	bIsSaved = false;
}

void GObject::Create(const string _className, const bool _temporary)
{
	ClassesData::XMLClassData objData = *ClassesData::GetClassesData(_className);
	ClassesData::SetFixedPtr(&objData);
	objData.GetParentData(objData.GetParentClass());
	ClassesData::SetFixedPtr(nullptr);

	// class data
	this->SetClassName(_className);
	(_temporary) ? this->SetPickingID(0) : this->SetPickingID(Picking::Obj::ObtainPickingID());

	// entity data
	this->spriteData = objData.GetSpriteData();
	this->spriteData.pickingId = this->GetPickingID();
	this->spriteData.pickingColor = Picking::GetPickingColorFromID(this->GetPickingID());

	// pass data
	this->pass_grid = Pass::GetPassGridPtr(this->className);
	if (_temporary == false) 
	{
		this->UpdatePass();
	}

	//if (_temporary == true) return bObjectCreated;
	this->SetObjectProperties(objData, _temporary);
	if (this->IsBuilding() == true)
	{
		this->AsBuilding()->SetBuildingProperties(objData, _temporary);
		if (_temporary == false)
		{
			this->AsBuilding()->AssignSettlement();
		}
	}
	else if (this->IsDecoration() == true)
	{
		;
	}
	else if (this->IsUnit() == true)
	{
		;
	}
	if (_temporary == false)
	{
		GObject::AddGameObject(this->GetPickingID(), this);
	}
}

void GObject::SetPosition(const vec3 pos)
{
	this->position = vec3(int(pos.x), int(pos.y), int(pos.z));
}

vec3 GObject::GetPosition(void) const
{
	return this->position;
}

float GObject::get_xPos(void) const
{
	return (float)position.x;
}
float GObject::get_yPos(void) const
{
	return (float)position.y;
}

GObject::~GObject(void) 
{
}

#pragma region Static variables
unsigned int GObject::numberOfObjects = 0;
unsigned int GObject::numberOfBuildings = 0;
unsigned int GObject::numberOfDecorations = 0;
unsigned int GObject::numberOfUnits = 0;
GObject* GObject::GameObjects[MAX_NUMBER_OF_OBJECTS] = { nullptr };
#pragma endregion

#pragma region Static Members
unsigned int GObject::GetNumberOfObjects(void)
{
	return GObject::numberOfObjects;
}

unsigned int GObject::GetNumberOfBuildings(void)
{
	return GObject::numberOfBuildings;
}

unsigned int GObject::GetNumberOfUnits(void)
{
	return GObject::numberOfUnits;
}

unsigned int GObject::GetNumberOfDecorations(void)
{
	return GObject::numberOfDecorations;
}

void GObject::AddGameObject(const unsigned int index, GObject* object)
{
	GObject::GameObjects[index] = object;
	if (object->IsBuilding() == true)
	{
		GObject::numberOfBuildings += 1;
	}
	else if (object->IsDecoration() == true)
	{
		GObject::numberOfDecorations += 1;
	}
	else if (object->IsUnit() == true)
	{
		GObject::numberOfUnits += 1;
	}
	GObject::numberOfObjects += 1;
}

void GObject::RemoveGameObject(const unsigned int index)
{
	if (index >= 1 && index < MAX_NUMBER_OF_OBJECTS) 
	{
		if (GObject::GameObjects[index] != nullptr)
		{
			Logger::Info("Building " + GameObjects[index]->GetSingularName() + " deleted!");
			//The picking ID and the script name of the object can be reused:
			Picking::Obj::AddUnsedPickingID(GameObjects[index]->GetPickingID());
			GObject::idNamesMap.erase(GObject::GameObjects[index]->GetIDName());

			if (GameObjects[index]->IsBuilding() == true)
			{
				GObject::numberOfBuildings -= 1;
				Building* b = GameObjects[index]->AsBuilding();
				b->GetSettlement()->RemoveBuildingFromSettlement(b);
				if (b->IsCentralBuilding() == true)
				{
					//Remove all the building belong to the settlement by recursive call.
					vector<Building*> buildings = b->GetSettlement()->GetBuildingsBelongToSettlement();
					for (auto element : buildings)
					{
						GObject::RemoveGameObject(element->GetPickingID());
					}
					//Remove the settlement from the list of the settlemet used in the editor.
					b->RemoveElementFromSettlementsList(b->GetSettlement());
				}
				GameObjects[index]->ClearPass();
			}
			else if (GameObjects[index]->IsDecoration() == true)
			{
				GObject::numberOfDecorations -= 1;
			}
			else if (GameObjects[index]->IsUnit() == true)
			{
				GObject::numberOfUnits -= 1;
			}
			GObject::numberOfObjects -= 1;
			delete GameObjects[index];
		}
		GameObjects[index] = nullptr;
	}
}

void GObject::ResetGameObjects(void)
{
	//for (unsigned int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) 
	//{
	//	if (GObject::GameObjects[i] != nullptr)
	//	{
	//		delete GObject::GetObjectByID(i);
	//	}
	//	GObject::GameObjects[i] = nullptr;
	//}
	//GObject::numberOfObjects = 0;
	//GObject::numberOfBuildings = 0;
	//GObject::numberOfDecorations = 0;
	//GObject::numberOfUnits = 0;
	for (unsigned int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++)
	{
		if (GObject::GameObjects[i] != nullptr)
		{
			//GObject::RemoveGameObject(GObject::GameObjects[i]->GetPickingID());
			GObject::RemoveGameObject(i);
		}
	}
	GObject::idNamesMap.clear(); //All script names can now reusable.
	Building::ResetSettlementsList(); //No settlement.
}

GObject* GObject::GetObjectByID(const unsigned int ID)
{
	return (ID >= 1 && ID < MAX_NUMBER_OF_OBJECTS) ? GObject::GameObjects[ID] : nullptr;
}

vector<Building*> GObject::GetListOfBuildings(void)
{
	vector<Building*> buildingsList = vector<Building*>();
	for (size_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) 
	{
		if (GObject::GameObjects[i] != nullptr && GObject::GameObjects[i]->IsBuilding() == true)
		{
			buildingsList.push_back(GObject::GameObjects[i]->AsBuilding());
		}
	}
	return buildingsList;
}
vector<Unit*> GObject::GetListOfUnits(void)
{
	vector<Unit*> unitsList = vector<Unit*>();
	for (size_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
	{
		if (GObject::GameObjects[i] != nullptr && GObject::GameObjects[i]->IsUnit() == true)
		{
			unitsList.push_back(GameObjects[i]->AsUnit());
		}
	}
	return unitsList;
}
vector<Decoration*> GObject::GetListOfDecorations(void)
{
	vector<Decoration*> decorationsList = vector<Decoration*>();
	for (size_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
	{
		if (GObject::GameObjects[i] != nullptr && GObject::GameObjects[i]->IsDecoration())
		{
			decorationsList.push_back(GObject::GameObjects[i]->AsDecoration());
		}
	}
	return decorationsList;
}
#pragma endregion

#pragma region Protected Members
bool GObject::CheckIfSelected(const unsigned int par_clickID)
{
	bool bSelected = (this->GetPickingID() == par_clickID);
	MarkAsSelected(bSelected);
	return bSelected;
}

void GObject::SendInfoText(const unsigned int method)
{
	if (this->AsBuilding()->IsCentralBuilding() == true)
	{
		if (this->IsPlaceable() == true)
		{
			std::wstring infoText;
			if (method == OBJ_INFOTEXT_INSERTING)
			{
				infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(0));
			}
			else if (method == OBJ_INFOTEXT_MOVING)
			{
				infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(4));
			}
			EditorUI::UpdateInfoText(infoText);
		}
		else
		{
			std::wstring infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(1));
			EditorUI::UpdateInfoText(infoText);
		}
	}
	else
	{
		std::tuple near = this->AsBuilding()->IsNearToFriendlySettlement();
		if (std::get<0>(near) == false)
		{
			std::wstring infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(2));
			EditorUI::UpdateInfoText(infoText);
		}
		else
		{
			if (this->IsPlaceable() == true)
			{
				std::wstring infoText;
				if (method == OBJ_INFOTEXT_INSERTING)
				{
					infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(0));
				}
				else if (method == OBJ_INFOTEXT_MOVING)
				{
					infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(4));
				}
				EditorUI::UpdateInfoText(infoText);
			}
			else
			{
				std::wstring infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(1));
				EditorUI::UpdateInfoText(infoText);
			}
		}
	}
}

void GObject::MarkAsSelected(const bool par_selected)
{
	this->bSelected = par_selected;
	if (par_selected == true && Game::Editor::IsInsertingObject() == false)
	{
		Game::SetSelectedObject(this);
	}
	else if(Picking::Obj::GetLeftClickId() == PICKING_TERRAIN_ID)
	{
		Game::SetSelectedObject(nullptr);
	}
}
#pragma endregion

#pragma region Private members
void GObject::SetObjectProperties(ClassesData::XMLClassData &objData, const bool _temporary)
{
	// TryParseFloat, TryParseInteger, TryParseString
	float fProperty = 0.f;
	int iProperty = 0;
	string strProperty = "";

	//Object's properties:
	if (_temporary == true) 
		return;
	ClassesData::TryParseString(objData.GetPropertiesMap(), "singularName", &strProperty);
	this->singularName = strProperty;
	this->displayedName = strProperty;
	ClassesData::TryParseString(objData.GetPropertiesMap(), "pluralName", &strProperty);
	this->pluralName = strProperty;
	ClassesData::TryParseString(objData.GetPropertiesMap(), "race", &strProperty);
	this->raceName = strProperty;
	ClassesData::TryParseInteger(objData.GetPropertiesMap(), "sight", &iProperty);
	this->sight = iProperty;
	ClassesData::TryParseFloat(objData.GetPropertiesMap(), "selectionRadius", &fProperty);
	this->selectionRadius = fProperty;
	ClassesData::TryParseString(objData.GetPropertiesMap(), "canBeClonedInEditor", &strProperty);
	this->canBeClonedInEditor = strProperty == "true" ? true : false;
	ClassesData::TryParseString(objData.GetPropertiesMap(), "isWaterObject", &strProperty);
	this->bIsWaterObject = strProperty == "true" ? true : false;
	ClassesData::TryParseString(objData.GetPropertiesMap(), "alwaysVisibleInGameMinimap", &strProperty);
	this->bAlwaysVisibleInGameMinimap = strProperty == "true" ? true : false;
}
#pragma endregion