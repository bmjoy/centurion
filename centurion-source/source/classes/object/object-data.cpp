#include "object-data.h"
#include <global>

#include "object-xml.hxx"

#pragma region Static variables

map<string, ObjectData::ObjectXMLClassData*> ObjectData::objectsData;
string ObjectData::dataClassesPath = "assets/data/classes_new/";

#pragma endregion



#pragma region ObjectXMLClassData class

ObjectData::ObjectXMLClassData::ObjectXMLClassData(){}

ObjectData::ObjectXMLClassData::~ObjectXMLClassData(){}

#pragma endregion



#pragma region ObjectData class

void ObjectData::ReadDataClassesFromXml()
{
	vector<string> files = glb::get_all_files_names_within_folder(dataClassesPath, "xml");

	for (int i = 0; i < files.size(); ++i) {

		string path = dataClassesPath + files[i];

		try {
			auto_ptr<c_object> dataXML = c_object_(path);
			std::cout << dataXML->class_name() << std::endl;
		}
		catch (const xml_schema::exception & e) {
			std::cout << e << std::endl;
		}


		/*std::cout << dataXML->class_name() << std::endl;*/

	}
}

void ObjectData::ResetObjectData()
{
	for (map<string, ObjectXMLClassData*>::iterator i = objectsData.begin(); i != objectsData.end(); i++) {
		string _class = i->first;
		if (i->second != nullptr) {
			delete objectsData[_class];
		}
		objectsData[_class] = nullptr;
	}
}

#pragma endregion

