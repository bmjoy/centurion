#include "object-data.h"

#include <file_manager.h>
#include "object-xml.hxx"
#include <primitives.h>
#include <building_sprite.h>
#include <decoration_sprite.h>
#include <unit_sprite.h>

#pragma region Static variables

map<string, ObjectData::ObjectXMLClassData> ObjectData::objectsData;
string ObjectData::dataClassesPath = "assets/data/classes/";

#pragma endregion

#pragma region ObjectXMLClassData class

ObjectData::ObjectXMLClassData::ObjectXMLClassData(){}

string ObjectData::ObjectXMLClassData::GetPropertyValue(string _property)
{
	if (propertiesMap.count(_property) > 0) {
		return propertiesMap[_property];
	}
	else {
		return "NOT_VALID";
	}
}

string ObjectData::ObjectXMLClassData::GetSoundPath(string _sound)
{
	if (soundsMap.count(_sound) > 0) {
		return soundsMap[_sound];
	}
	else {
		return "NOT_VALID";
	}
}

string ObjectData::ObjectXMLClassData::GetMethodScript(string _method)
{
	if (methodsMap.count(_method) > 0) {
		return methodsMap[_method];
	}
	else {
		return "NOT_VALID";
	}
}

ObjectData::ObjectXMLClassData::~ObjectXMLClassData(){}

#pragma endregion

#pragma region ObjectData class

void ObjectData::ReadDataClassesFromXml()
{
	vector<string> files = FileManager::GetAllFilesNamesWithinFolder(dataClassesPath, "xml");

	for (int i = 0; i < files.size(); ++i) {

		string path = dataClassesPath + files[i];

		try {
			xml_schema::properties props;
			props.no_namespace_schema_location(Folders::XML_SCHEMAS + "object.xsd");
			auto_ptr<c_object> dataXML = c_object_(path, 0, props);

			ObjectXMLClassData objData = ObjectXMLClassData();

			objData.SetClassName(string(dataXML->class_name()));
			objData.SetClassType(string(dataXML->type()));
			objData.SetParentClass(string(dataXML->parent()));

			properties::property_iterator _it_prop;
			for (_it_prop = dataXML->properties().property().begin(); _it_prop != dataXML->properties().property().end(); _it_prop++) {
				objData.AddProperty(string(_it_prop->name()), string(_it_prop->value()));
			}

			methods::method_iterator _it_mtd;
			for (_it_mtd = dataXML->methods().method().begin(); _it_mtd != dataXML->methods().method().end(); _it_mtd++) {
				objData.AddMethod(string(_it_mtd->name()), string(_it_mtd->script()));
			}
			
			sounds::sound_iterator _it_snd;
			for (_it_snd = dataXML->sounds().sound().begin(); _it_snd != dataXML->sounds().sound().end(); _it_snd++) {
				objData.AddSound(string(_it_snd->name()), string(_it_snd->path()));
			}

			AddObjectXMLClassData(objData.GetClassName(), objData);

			// READ THE ENTITY.XML FILES AND PREPARE THE PRIMITIVES

			string path = objData.GetPropertyValue("ent_path");
			if (path == "NOT_VALID") continue;
			if (objData.GetClassType() == "building") {
				primitives::BSprite()->AddEntityPath(path);
			}
			else if (objData.GetClassType() == "decoration") {
				primitives::DSprite()->AddEntityPath(path);
			}
			else if (objData.GetClassType() == "unit") {
				primitives::USprite()->AddEntityPath(path);
			}
		}
		catch (const xml_schema::exception & e) {
			std::cout << e << std::endl;
		}
	}
}

#pragma endregion