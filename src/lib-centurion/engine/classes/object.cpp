#include "object.h"

#include <player/player.h>
#include <picking.h>
#include <game/strategy.h>
#include <pathfinding/pathfinding.h>
#include <game/editor.h>
#include <game/pass.h>

using namespace std;
using namespace glm;


GObject::GObject(void)
{
	this->bSelected = false;
	this->scriptName = "Clarissa";
}

unsigned short int GObject::GetPlayer(void)
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

unsigned int GObject::GetPickingID(void)
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

float GObject::GetRadius(void)
{
	return this->radius;
}

void GObject::SetRadius(const float par_radius)
{
	this->radius = par_radius;
}

float GObject::GetSelectionRadius(void)
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

string GObject::GetType(void)
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

string GObject::GetSingularName(void)
{
	return this->singularName;
}

void GObject::SetPluralName(const string par_pluralName)
{
	this->pluralName = par_pluralName;
}

string GObject::GetPluralName(void)
{
	return this->pluralName;
}

void GObject::SetScriptName(const std::string _scriptName)
{
	//(???) TODO: verifica univocita' stringa.
	this->scriptName = _scriptName;
}

std::string GObject::GetScriptName(void)
{
	return this->scriptName;
}

unsigned int GObject::GetRace(void)
{
	return this->race;
}
string GObject::GetRaceName(void)
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

unsigned int GObject::GetSight(void)
{
	return this->sight;
}
void GObject::SetSight(const unsigned short int par_sight)
{
	this-> sight = par_sight;
}

bool GObject::CanBePositionedIntoWater(void)
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

bool GObject::GetCanBeClonedInEditor(void)
{
	return this->canBeClonedInEditor;
}
void GObject::SetCanBeClonedInEditor(const bool par_canBeClonedInEditor)
{
	this->canBeClonedInEditor = par_canBeClonedInEditor;
}

bool GObject::GetAlwaysVisibleInGameMinimap(void)
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

Building* GObject::AsBuilding()
{ 
	return (Building*)this; 
}
Unit* GObject::AsUnit()
{ 
	return (Unit*)this; 
}
Decoration* GObject::AsDecoration()
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

bool GObject::IsPlaceable(void)
{
	return this->bIsPlaceable;
}

bool GObject::Create(const string _className, const bool _temporary)
{
	bool bObjectCreated = true;
	ObjectData::ObjectXMLClassData objData = *ObjectData::GetObjectData(_className);
	ObjectData::SetFixedPtr(&objData);
	objData.GetParentData(objData.GetParentClass());
	ObjectData::SetFixedPtr(nullptr);

	// class data
	this->SetClassName(_className);
	(_temporary) ? this->SetPickingID(0) : this->SetPickingID(Picking::Obj::ObtainPickingID());

	// entity data
	this->spriteData = objData.GetSpriteData();
	this->spriteData.pickingId = this->GetPickingID();
	this->spriteData.pickingColor = Picking::GetPickingColorFromID(this->GetPickingID());

	// pass data
	this->pass_grid = Pass::GetPassGridPtr(this->className);

	//if (_temporary == true) return bObjectCreated;
	this->SetObjectProperties(objData, _temporary);
	if (this->IsBuilding() == true)
	{
		bObjectCreated = this->AsBuilding()->SetBuildingProperties(objData, _temporary);
	}
	else if (this->IsDecoration() == true)
	{
		bObjectCreated = true;
	}
	else if (this->IsUnit() == true)
	{
		;
	}
	if (bObjectCreated == true && _temporary == false)
	{
		GObject::AddGameObject(this->GetPickingID(), this);
	}
	return bObjectCreated;
}

void GObject::SetPosition(const vec3 pos)
{
	this->position = pos;
}
vec3 GObject::GetPosition(void)
{
	return this->position;
}

float GObject::get_xPos(void)
{
	return (float)position.x;
}
float GObject::get_yPos(void)
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
		if (GameObjects[index] != nullptr)
		{
			Picking::Obj::AddUnsedPickingID(GameObjects[index]->GetPickingID());
			if (GameObjects[index]->IsBuilding() == true)
			{
				GObject::numberOfBuildings -= 1;
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
	for (unsigned int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) 
	{
		if (GObject::GameObjects[i] != nullptr)
		{
			delete GObject::GetObjectByID(i);
		}
		GObject::GameObjects[i] = nullptr;
	}
	GObject::numberOfObjects = 0;
	GObject::numberOfBuildings = 0;
	GObject::numberOfDecorations = 0;
	GObject::numberOfUnits = 0;
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
void GObject::SetObjectProperties(ObjectData::ObjectXMLClassData &objData, const bool _temporary)
{
	// TryParseFloat, TryParseInteger, TryParseString
	float fProperty = 0.f;
	int iProperty = 0;
	string strProperty = "";

	//Object's properties:
	ObjectData::TryParseFloat(objData.GetPropertiesMap(), "radius", &fProperty);
	this->radius = fProperty;
	if (_temporary == true) 
		return;
	ObjectData::TryParseString(objData.GetPropertiesMap(), "singularName", &strProperty);
	this->singularName = strProperty;
	ObjectData::TryParseString(objData.GetPropertiesMap(), "pluralName", &strProperty);
	this->pluralName = strProperty;
	ObjectData::TryParseString(objData.GetPropertiesMap(), "race", &strProperty);
	this->raceName = strProperty;
	ObjectData::TryParseInteger(objData.GetPropertiesMap(), "sight", &iProperty);
	this->sight = iProperty;
	ObjectData::TryParseFloat(objData.GetPropertiesMap(), "selectionRadius", &fProperty);
	this->selectionRadius = fProperty;
	ObjectData::TryParseString(objData.GetPropertiesMap(), "canBeClonedInEditor", &strProperty);
	this->canBeClonedInEditor = strProperty == "true" ? true : false;
	ObjectData::TryParseString(objData.GetPropertiesMap(), "isWaterObject", &strProperty);
	this->bIsWaterObject = strProperty == "true" ? true : false;
	ObjectData::TryParseString(objData.GetPropertiesMap(), "alwaysVisibleInGameMinimap", &strProperty);
	this->bAlwaysVisibleInGameMinimap = strProperty == "true" ? true : false;
}
#pragma endregion