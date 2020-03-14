#include "mapObjects.h"
#include "mapObjects-xml.hxx"

#include "game.h"
#include <object/object.h>
#include <object/building.h>
#include <object/unit.h>
#include <object/decoration.h>
#include <fstream>

void MapObjects::ReadMapObjectsFromXml(string xmlPath)
{
}

void MapObjects::SaveMapObjectsToXml(string xmlPath)
{
	try
	{
		c_mapObjects mapObjs = c_mapObjects();

		for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {

			if (Game::IsGameObjectNotNull(i) == false) continue;

			if (Game::GameObjects[i]->IsBuilding()) {

				Building* gobj = Game::GameObjects[i]->AsBuilding();

				object o = object(gobj->GetClassName(), gobj->GetPickingID(), 1, gobj->GetPosition().x, gobj->GetPosition().y);
				o.healthperc(100);
				o.name(gobj->GetName());

				mapObjs.object().push_back(o);
				//o.icon = "";

			}

			if (Game::GameObjects[i]->IsDecoration()) {

				Decoration* gobj = Game::GameObjects[i]->AsDecoration();

				object o = object(gobj->GetClassName(), gobj->GetPickingID(), 1, gobj->GetPosition().x, gobj->GetPosition().y);
				//o.healthperc(100);
				//o.name(gobj->GetName());
				mapObjs.object().push_back(o);

			}

			// if is unit
		}

		xml_schema::namespace_infomap map;
		map[""].schema = "../../assets/xml-schemas/mapObjects.xsd";
		ofstream ofs(xmlPath.c_str());
		c_mapObjects_(ofs, mapObjs, map);
	}
	catch (const xml_schema::exception & e) {
		std::cout << e << std::endl;
		throw;
	}
	catch (...)
	{
		throw;
	}
	
}
