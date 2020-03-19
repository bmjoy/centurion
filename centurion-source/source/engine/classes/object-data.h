#ifndef OBJECT_DATA_H
#define OBJECT_DATA_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <object_sprite.h>

using namespace std;

class ObjectData {
public:

	class ObjectXMLClassData {
	public:
		ObjectXMLClassData();
		void AddProperty(string k, string v) { propertiesMap[k] = v; }
		void AddSound(string k, string v) { soundsMap[k] = v; }
		void AddMethod(string k, string v) { methodsMap[k] = v; }
		string GetPropertyValue(string _property);
		string GetSoundPath(string _sound);
		string GetMethodScript(string _method);
		void SetClassName(string _class) { className = _class; }
		void SetParentClass(string _parent) { parent = _parent; }
		void SetClassType(string _type) { type = _type; }
		string GetClassName() { return className; }
		string GetParentClass() { return parent; }
		string GetClassType() { return type; }
		void SetSpriteData(ObjectSprite::SpriteData _spriteData) { spriteData = _spriteData; }
		ObjectSprite::SpriteData GetSpriteData() { return spriteData; }
		~ObjectXMLClassData();
	private:
		map<string, string> propertiesMap;
		map<string, string> soundsMap;
		map<string, string> methodsMap;
		string className, parent, type;
		ObjectSprite::SpriteData spriteData;
	};

	static ObjectXMLClassData* GetObjectData(string _class) { return &(objectsData[_class]); }
	static void ReadDataClassesFromXml();

private:
	static void AddObjectXMLClassData(string _class, ObjectXMLClassData objData) { objectsData[_class] = objData; }
	static map<string, ObjectXMLClassData> objectsData;
	static string dataClassesPath;
};



#endif
