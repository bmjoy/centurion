#include "classes-data.h"

#include <file_manager.h>
#include <gl_items.h>
#include <gl_building_sprite.h>
#include <gl_decoration_sprite.h>
#include <gl_unit_sprite.h>
#include <logger.h>
#include <game/editor.h>
#include <game/pass.h>

#include <tinyxml2.h>

using namespace std;
using namespace glm;
using namespace tinyxml2;

namespace ClassesData
{
	// Private variables
	namespace
	{
		XMLClassData* objDataXmlFixedPtr = nullptr;
		std::map<std::string, XMLClassData> objectsData = std::map<std::string, XMLClassData>();
		std::string dataClassesPath = "assets/data/classes/";
		std::string PROPERTY_NOT_VALID = "NA";
	};

	ClassesData::XMLClassData::XMLClassData(void) {}

	void ClassesData::XMLClassData::GetParentData(string _parent)
	{
		if (objDataXmlFixedPtr == nullptr) return;
		if (_parent == "") return;

		XMLClassData* objData = GetClassesData(_parent);
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

	void ClassesData::XMLClassData::AddPropertyIfMissing(const string k, const string v)
	{
		if (this->HasProperty(k)) return;
		this->AddProperty(k, v);
	}

	void ClassesData::XMLClassData::AddSoundIfMissing(const string k, const string v)
	{
		if (this->HasMethod(k)) return;
		this->AddMethod(k, v);
	}

	void ClassesData::XMLClassData::AddMethodIfMissing(const string k, const string v)
	{
		if (this->HasSound(k)) return;
		this->AddSound(k, v);
	}

	string ClassesData::XMLClassData::GetPropertyValue(const string _property)
	{
		if (propertiesMap.count(_property) > 0) 
		{
			return propertiesMap[_property];
		}
		else 
		{
			return PROPERTY_NOT_VALID;
		}
	}

	string ClassesData::XMLClassData::GetSoundPath(const string _sound)
	{
		if (soundsMap.count(_sound) > 0) 
		{
			return soundsMap[_sound];
		}
		else 
		{
			return PROPERTY_NOT_VALID;
		}
	}

	string ClassesData::XMLClassData::GetMethodScript(const string _method)
	{
		if (methodsMap.count(_method) > 0)
		{
			return methodsMap[_method];
		}
		else 
		{
			return PROPERTY_NOT_VALID;
		}
	}

	ClassesData::XMLClassData::~XMLClassData(void) {}

	void ClassesData::TryParseInteger(map<string, string> _map, string _name, int* variable_ptr)
	{
		if (_map.count(_name) > 0) 
		{
			string value = _map[_name];
			try 
			{
				int value_int = std::stoi(value);
				(*variable_ptr) = value_int;
			}
			catch (...) 
			{
				Logger::LogMessage msg = Logger::LogMessage("The value of variable \"" + _name + "\" is not an integer", "Warn", "", "ClassesData", "TryParseFloat");
				Logger::Warn(msg);
				(*variable_ptr) = -1;
			}
		}
		else 
		{
			(*variable_ptr) = -1;
		}
	}

	void ClassesData::TryParseFloat(map<string, string> _map, string _name, float* variable_ptr)
	{
		if (_map.count(_name) > 0) 
		{
			string value = _map[_name];
			try 
			{
				float value_int = std::stof(value);
				(*variable_ptr) = value_int;
			}
			catch (...) 
			{
				Logger::LogMessage msg = Logger::LogMessage("The value of variable \"" + _name + "\" is not a float", "Warn", "", "ClassesData", "TryParseFloat");
				Logger::Warn(msg);
				(*variable_ptr) = -1;
			}
		}
		else 
		{
			(*variable_ptr) = -1;
		}
	}

	void ClassesData::TryParseString(map<string, string> _map, string _name, string* variable_ptr)
	{
		if (_map.count(_name) > 0)
		{
			string value = _map[_name];
			(*variable_ptr) = value;
		}
		else 
		{
			(*variable_ptr) = "";
		}
	}

	void ClassesData::SetFixedPtr(XMLClassData* _ptr)
	{
		objDataXmlFixedPtr = _ptr;
	}

	ClassesData::XMLClassData* ClassesData::GetClassesData(string _class)
	{
		if (objectsData.count(_class) > 0) 
		{
			return &(objectsData[_class]);
		}
		else 
		{
			return nullptr;
		}
	}

	void ClassesData::ReadDataClassesFromXml(void)
	{
		vector<string> files = FileManager::GetAllFilesNamesWithinFolder(dataClassesPath, "xml");

		for (int i = 0; i < files.size(); ++i) 
		{
			string path = dataClassesPath + files[i];
			try
			{
				XMLDocument xmlFile;
				xmlFile.LoadFile(path.c_str());
				XMLElement* _objectXml = xmlFile.FirstChildElement("object");

				XMLClassData objData = XMLClassData();

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

				AddXMLClassData(objData.GetClassName(), objData);

				// READ THE ENTITY.XML FILES AND PREPARE THE PRIMITIVES

				string path = objData.GetPropertyValue("ent_path");
				if (path != PROPERTY_NOT_VALID)
				{
					if (objData.GetClassType() == "cpp_buildingclass")
					{
						GLItems::BSprite()->AddEntityPath(path);
					}
					else if (objData.GetClassType() == "cpp_decorationclass")
					{
						GLItems::DSprite()->AddEntityPath(path);
					}
					else if (objData.GetClassType() == "cpp_unitclass")
					{
						GLItems::USprite()->AddEntityPath(path);
					}
				}

				// ADD VALUES TO EDITOR TREE
				std::string editorFilter1 = objData.GetPropertyValue("editorFilter1");
				std::string editorFilter2 = objData.GetPropertyValue("editorFilter2");
				if (editorFilter1 != PROPERTY_NOT_VALID || editorFilter2 != PROPERTY_NOT_VALID)
				{
					std::string editorFilter3 = objData.GetClassName();
					if (objData.GetClassType() == "cpp_buildingclass" || objData.GetClassType() == "cpp_decorationclass" || objData.GetClassType() == "cpp_unitclass")
					{
						Game::Editor::AddEditorObjectTreeElement(editorFilter1, editorFilter2, editorFilter3);
					}
				}

				// SAVING OBJECT PASS
				std::string passPath = objData.GetPropertyValue("pass_path");
				if (passPath != PROPERTY_NOT_VALID)
				{
					Pass::AddPassGrid(passPath, objData.GetClassName());
				}
			}
			catch (...)
			{
				std::cout << "An error occurred" << std::endl;
			}
		}
	}

	void ClassesData::AddXMLClassData(std::string _class, XMLClassData objData)
	{
		objectsData[_class] = objData;
	}
};
