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
		void GetParentData(string _parent);
		void AddProperty(string k, string v) { propertiesMap[k] = v; }
		void AddPropertyIfMissing(string k, string v);
		void AddSound(string k, string v) { soundsMap[k] = v; }
		void AddSoundIfMissing(string k, string v);
		void AddMethod(string k, string v) { methodsMap[k] = v; }
		void AddMethodIfMissing(string k, string v);
		bool HasProperty(string k) { return (propertiesMap.count(k) > 0); }
		bool HasMethod(string k) { return (methodsMap.count(k) > 0); }
		bool HasSound(string k) { return (soundsMap.count(k) > 0); }
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
		map<string, string> GetPropertiesMap() { return propertiesMap; }
		map<string, string> GetMethodsMap() { return methodsMap; }
		map<string, string> GetSoundsMap() { return soundsMap; }
		~ObjectXMLClassData();
	private:
		
		map<string, string> propertiesMap;
		map<string, string> soundsMap;
		map<string, string> methodsMap;
		string className, parent, type;
		ObjectSprite::SpriteData spriteData;
	};

	static void TryParseInteger(map<string, string> _map, string _name, int *variable_ptr);
	static void TryParseFloat(map<string, string> _map, string _name, float *variable_ptr);
	static void TryParseString(map<string, string> _map, string _name, string *variable_ptr);
	static void SetFixedPtr(ObjectXMLClassData* _ptr) { objDataXmlFixedPtr = _ptr; }
	static ObjectXMLClassData* GetObjectData(string _class);
	static void ReadDataClassesFromXml();

private:
	static ObjectXMLClassData* objDataXmlFixedPtr;
	static void AddObjectXMLClassData(string _class, ObjectXMLClassData objData) { objectsData[_class] = objData; }
	static map<string, ObjectXMLClassData> objectsData;
	static string dataClassesPath;
};



#endif
