#include "objectArray.h"
#include "settlementArray.h"

#include "object.h"
#include "building.h"
#include "decoration.h"
#include "unit.h"

#include <logger.h>
#include <picking.h>


#ifndef MAX_NUMBER_OF_OBJECTS
#define MAX_NUMBER_OF_OBJECTS 100
#endif

using namespace std;
using namespace glm;


namespace ObjectArray
{
	// Private variables
	namespace
	{
		GObject* GameObjects[MAX_NUMBER_OF_OBJECTS];
		unsigned int numberOfObjects;
		unsigned int numberOfBuildings;
		unsigned int numberOfUnits;
		unsigned int numberOfDecorations;
		std::unordered_map<std::string, unsigned int> idNamesMap;
	};

	unsigned int ObjectArray::GetNumberOfObjects(void)
	{
		return ObjectArray::numberOfObjects;
	}

	unsigned int ObjectArray::GetNumberOfBuildings(void)
	{
		return ObjectArray::numberOfBuildings;
	}

	unsigned int ObjectArray::GetNumberOfUnits(void)
	{
		return ObjectArray::numberOfUnits;
	}

	unsigned int ObjectArray::GetNumberOfDecorations(void)
	{
		return ObjectArray::numberOfDecorations;
	}

	void ObjectArray::AddGameObject(const unsigned int index, GObject* object)
	{
		ObjectArray::GameObjects[index] = object;
		if (object->IsBuilding() == true)
		{
			ObjectArray::numberOfBuildings += 1;
		}
		else if (object->IsDecoration() == true)
		{
			ObjectArray::numberOfDecorations += 1;
		}
		else if (object->IsUnit() == true)
		{
			ObjectArray::numberOfUnits += 1;
		}
		ObjectArray::numberOfObjects += 1;
	}

	void ObjectArray::RemoveGameObject(const unsigned int index)
	{
		if (index >= 1 && index < MAX_NUMBER_OF_OBJECTS)
		{
			if (ObjectArray::GameObjects[index] != nullptr)
			{
				Logger::Info("Building " + GameObjects[index]->GetSingularName() + " deleted!");
				//The picking ID and the script name of the object can be reused:
				Picking::Obj::AddUnsedPickingID(GameObjects[index]->GetPickingID());
				ObjectArray::idNamesMap.erase(ObjectArray::GameObjects[index]->GetIDName());

				if (GameObjects[index]->IsBuilding() == true)
				{
					ObjectArray::numberOfBuildings -= 1;
					Building* b = GameObjects[index]->AsBuilding();
					b->GetSettlement()->RemoveBuildingFromSettlement(b);
					if (b->IsCentralBuilding() == true)
					{
						//Remove all the buildings belonging to the settlement by RECURSIVE call.
						vector<Building*> buildings = b->GetSettlement()->GetBuildingsBelongToSettlement();
						for (auto element : buildings)
						{
							ObjectArray::RemoveGameObject(element->GetPickingID());
						}
						//Remove the settlement from the list of the settlemet used in the editor.
						b->RemoveElementFromSettlementsList(b->GetSettlement());
					}
					GameObjects[index]->ClearPass();
				}
				else if (GameObjects[index]->IsDecoration() == true)
				{
					ObjectArray::numberOfDecorations -= 1;
				}
				else if (GameObjects[index]->IsUnit() == true)
				{
					ObjectArray::numberOfUnits -= 1;
				}
				ObjectArray::numberOfObjects -= 1;
				delete GameObjects[index];
			}
			GameObjects[index] = nullptr;
		}
	}

	void ObjectArray::ResetGameObjects(void)
	{
		//for (unsigned int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) 
		//{
		//	if (ObjectArray::GameObjects[i] != nullptr)
		//	{
		//		delete ObjectArray::GetObjectByID(i);
		//	}
		//	ObjectArray::GameObjects[i] = nullptr;
		//}
		//ObjectArray::numberOfObjects = 0;
		//ObjectArray::numberOfBuildings = 0;
		//ObjectArray::numberOfDecorations = 0;
		//ObjectArray::numberOfUnits = 0;
		for (unsigned int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++)
		{
			if (ObjectArray::GameObjects[i] != nullptr)
			{
				//ObjectArray::RemoveGameObject(ObjectArray::GameObjects[i]->GetPickingID());
				ObjectArray::RemoveGameObject(i);
			}
		}
		ObjectArray::idNamesMap.clear(); //All script names can now reusable.
		SettlementArray::ResetSettlementsList(); //No settlement.
	}

	GObject* ObjectArray::GetObjectByID(const unsigned int ID)
	{
		return (ID >= 1 && ID < MAX_NUMBER_OF_OBJECTS) ? ObjectArray::GameObjects[ID] : nullptr;
	}

	vector<Building*> ObjectArray::GetListOfBuildings(void)
	{
		vector<Building*> buildingsList = vector<Building*>();
		for (size_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
		{
			if (ObjectArray::GameObjects[i] != nullptr && ObjectArray::GameObjects[i]->IsBuilding() == true)
			{
				buildingsList.push_back(ObjectArray::GameObjects[i]->AsBuilding());
			}
		}
		return buildingsList;
	}
	vector<Unit*> ObjectArray::GetListOfUnits(void)
	{
		vector<Unit*> unitsList = vector<Unit*>();
		for (size_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
		{
			if (ObjectArray::GameObjects[i] != nullptr && ObjectArray::GameObjects[i]->IsUnit() == true)
			{
				unitsList.push_back(GameObjects[i]->AsUnit());
			}
		}
		return unitsList;
	}

	vector<Building*> ObjectArray::GetListOfIndipendentBuildings(void)
	{
		vector<Building*> indipBuildings = vector<Building*>();
		for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
		{
			if (ObjectArray::GetObjectByID(i) != nullptr && ObjectArray::GetObjectByID(i)->IsBuilding())
			{
				if (ObjectArray::GetObjectByID(i)->AsBuilding()->GetSettlement()->IsIndipendent())
				{
					indipBuildings.push_back(ObjectArray::GetObjectByID(i)->AsBuilding());
				}
			}
		}
		return indipBuildings;
	}

	vector<Decoration*> ObjectArray::GetListOfDecorations(void)
	{
		vector<Decoration*> decorationsList = vector<Decoration*>();
		for (size_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
		{
			if (ObjectArray::GameObjects[i] != nullptr && ObjectArray::GameObjects[i]->IsDecoration())
			{
				decorationsList.push_back(ObjectArray::GameObjects[i]->AsDecoration());
			}
		}
		return decorationsList;
	}

	bool ObjectArray::CheckIfIdNameExists(const std::string name)
	{
		return (ObjectArray::idNamesMap.count(name) > 0);
	}

	unsigned int ObjectArray::GetPickingIdByIdName(const std::string name)
	{
		if (CheckIfIdNameExists(name))
		{
			return ObjectArray::idNamesMap[name];
		}
		else return 0;
	}

	void ObjectArray::RemoveIdName(const std::string name)
	{
		if (CheckIfIdNameExists(name))
		{
			ObjectArray::idNamesMap.erase(name);
		}
	}

	void ObjectArray::AssignIdName(const unsigned int pickingid, const std::string name)
	{
		ObjectArray::idNamesMap[name] = pickingid;
	}

	void Render(const bool picking, const unsigned int clickid)
	{
		for (int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) {
			if (GameObjects[i] != nullptr)
			{
				GameObjects[i]->Render(picking, clickid);
			}
		}
	}
};