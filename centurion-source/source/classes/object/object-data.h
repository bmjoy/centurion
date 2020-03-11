#ifndef OBJECT_DATA_H
#define OBJECT_DATA_H

#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class ObjectData {
public:

	class ObjectXMLClassData {
	public:
		ObjectXMLClassData();
		~ObjectXMLClassData();

		
	};

	class BuildingXMLClassData : public ObjectXMLClassData {
		bool getIsCentralBuilding() { return isCentralBuilding; }
	private:
		bool isCentralBuilding;
	};

	static ObjectXMLClassData* GetObjectData(string _class) { return objectsData[_class]; }
	static void ReadDataClassesFromXml();
	static void ResetObjectData();

private:
	static map<string, ObjectXMLClassData*> objectsData;
	static string dataClassesPath;
};



#endif
