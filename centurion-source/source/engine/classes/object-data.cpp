#include "object-data.h"

#include <file_manager.h>
#include <primitives.h>
#include <building_sprite.h>
#include <decoration_sprite.h>
#include <unit_sprite.h>
#include <logger.h>

#include <tinyxml2.h>


using namespace tinyxml2;

#pragma region Static variables

map<string, ObjectData::ObjectXMLClassData> ObjectData::objectsData;
string ObjectData::dataClassesPath = "assets/data/classes/";
ObjectData::ObjectXMLClassData* ObjectData::objDataXmlFixedPtr = nullptr;
#pragma endregion

#pragma region ObjectXMLClassData class

ObjectData::ObjectXMLClassData::ObjectXMLClassData(){}

void ObjectData::ObjectXMLClassData::GetParentData(string _parent)
{
	if (objDataXmlFixedPtr == nullptr) return;
	if (_parent == "") return;

	ObjectXMLClassData *objData = GetObjectData(_parent);
	if (objData == nullptr) return;
	for (map<string, string>::iterator _prop = objData->propertiesMap.begin(); _prop != objData->propertiesMap.end(); _prop++)
	{
		string name = _prop->first;
		string value = _prop->second;
		objDataXmlFixedPtr->AddPropertyIfMissing(name, value);
	}
	for (map<string, string>::iterator _method = objData->methodsMap.begin(); _method != objData->methodsMap.end(); _method++)
	{
		string name = _method->first;
		string script = _method->second;
		objDataXmlFixedPtr->AddMethodIfMissing(name, script);
	}
	for (map<string, string>::iterator _sound = objData->soundsMap.begin(); _sound != objData->soundsMap.end(); _sound++)
	{
		string name = _sound->first;
		string path = _sound->second;
		objDataXmlFixedPtr->AddSoundIfMissing(name, path);
	}

	// recursive call
	string p = objData->GetParentClass();
	GetParentData(p);
}

void ObjectData::ObjectXMLClassData::AddPropertyIfMissing(const string k, const string v)
{
	if (this->HasProperty(k)) return;
	this->AddProperty(k, v);
}

void ObjectData::ObjectXMLClassData::AddSoundIfMissing(const string k, const string v)
{
	if (this->HasMethod(k)) return;
	this->AddMethod(k, v);
}

void ObjectData::ObjectXMLClassData::AddMethodIfMissing(const string k, const string v)
{
	if (this->HasSound(k)) return;
	this->AddSound(k, v);
}

string ObjectData::ObjectXMLClassData::GetPropertyValue(const string _property)
{
	if (propertiesMap.count(_property) > 0) {
		return propertiesMap[_property];
	}
	else {
		return "NOT_VALID";
	}
}

string ObjectData::ObjectXMLClassData::GetSoundPath(const string _sound)
{
	if (soundsMap.count(_sound) > 0) {
		return soundsMap[_sound];
	}
	else {
		return "NOT_VALID";
	}
}

string ObjectData::ObjectXMLClassData::GetMethodScript(const string _method)
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

void ObjectData::TryParseInteger(map<string, string> _map, string _name, int * variable_ptr)
{
	if (_map.count(_name) > 0) {
		string value = _map[_name];
		try {
			int value_int = std::stoi(value);
			(*variable_ptr) = value_int;
		}
		catch (...) {
			Logger::LogMessage msg = Logger::LogMessage("The value of variable \"" + _name + "\" is not an integer", "Warn", "", "ObjectData", "TryParseFloat");
			Logger::Warn(msg);
			(*variable_ptr) = -1;
		}
	}
	else {
		(*variable_ptr) = -1;
	}
}

void ObjectData::TryParseFloat(map<string, string> _map, string _name, float * variable_ptr)
{
	if (_map.count(_name) > 0) {
		string value = _map[_name];
		try {
			float value_int = std::stof(value);
			(*variable_ptr) = value_int;
		}
		catch (...) {
			Logger::LogMessage msg = Logger::LogMessage("The value of variable \"" + _name + "\" is not a float", "Warn", "", "ObjectData", "TryParseFloat");
			Logger::Warn(msg);
			(*variable_ptr) = -1;
		}
	}
	else {
		(*variable_ptr) = -1;
	}
}

void ObjectData::TryParseString(map<string, string> _map, string _name, string * variable_ptr)
{
	if (_map.count(_name) > 0) {
		string value = _map[_name];
		(*variable_ptr) = value;
	}
	else {
		(*variable_ptr) = "";
	}
}

ObjectData::ObjectXMLClassData * ObjectData::GetObjectData(string _class)
{
	if (objectsData.count(_class) > 0) {
		return &(objectsData[_class]);
	}
	else {
		return nullptr;
	}
}

void ObjectData::ReadDataClassesFromXml(void)
{
	vector<string> files = FileManager::GetAllFilesNamesWithinFolder(dataClassesPath, "xml");

	for (int i = 0; i < files.size(); ++i) {

		string path = dataClassesPath + files[i];

		try {
			XMLDocument xmlFile;
			xmlFile.LoadFile(path.c_str());
			XMLElement *_objectXml = xmlFile.FirstChildElement("object");

			ObjectXMLClassData objData = ObjectXMLClassData();

			objData.SetClassName(string(_objectXml->Attribute("class_name")));
			objData.SetClassType(string(_objectXml->Attribute("type")));
			objData.SetParentClass(string(_objectXml->Attribute("parent")));


			for (XMLElement* _it_prop = _objectXml->FirstChildElement("properties")->FirstChildElement(); _it_prop != NULL; _it_prop = _it_prop->NextSiblingElement())
			{
				objData.AddProperty(string(_it_prop->Attribute("name")), string(_it_prop->Attribute("value")));
			}
			
			for (XMLElement* _it_mtd = _objectXml->FirstChildElement("methods")->FirstChildElement(); _it_mtd != NULL; _it_mtd = _it_mtd->NextSiblingElement())
			{
				objData.AddMethod(string(_it_mtd->Attribute("name")), string(_it_mtd->Attribute("script")));
			}
			
			for (XMLElement* _it_snd = _objectXml->FirstChildElement("sounds")->FirstChildElement(); _it_snd != NULL; _it_snd = _it_snd->NextSiblingElement())
			{
				objData.AddSound(string(_it_snd->Attribute("name")), string(_it_snd->Attribute("path")));
			}

			AddObjectXMLClassData(objData.GetClassName(), objData);

			// READ THE ENTITY.XML FILES AND PREPARE THE PRIMITIVES

			string path = objData.GetPropertyValue("ent_path");
			if (path == "NOT_VALID") continue;
			if (objData.GetClassType() == "cpp_buildingclass") {
				primitives::BSprite()->AddEntityPath(path);
			}
			else if (objData.GetClassType() == "cpp_decorationclass") {
				primitives::DSprite()->AddEntityPath(path);
			}
			else if (objData.GetClassType() == "cpp_unitclass") {
				primitives::USprite()->AddEntityPath(path);
			}
		}
		catch (...) {
			std::cout << "An error occurred" << std::endl;
		}
	}
}

#pragma endregion