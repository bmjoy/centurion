/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <gl_object_sprite.h>


/// <summary>
/// This namespace contains methods that read the properties of the object's family from some XML files and store the information into a map; 
/// the key of the map is the name of a property.
/// (???) Forse alcune di queste funzioni dovrebbero essere private e i metodi andrebbero tutti implementati nel file cpp per rispettare
/// i principi della programmazione modulare e ad oggetti.
/// </summary>
namespace ClassesData 
{
	class XMLClassData 
	{
	public:
		/// <summary>
		/// Public constructor.
		/// </summary>
		XMLClassData(void);

		void GetParentData(std::string _parent);

		/// <summary>
		/// This function adds a property into a map if itcould be read from an xml.
		/// </summary>
		/// <param name="k">The key used to identify a property.</param>
		/// <param name="v">The value of the property.</param>
		void AddProperty(const std::string k, const std::string v) { propertiesMap[k] = v; }

		/// <summary>
		/// This function adds a property into a map when it isn't present into an xml.
		/// </summary>
		/// <param name="k">The key used to identify a property.</param>
		/// <param name="v">The value of the property.</param>
		void AddPropertyIfMissing(const std::string k, const std::string v);

		/// <summary>
		/// This function adds an audio path into a map if it could be read from an xml.
		/// </summary>
		/// <param name="k">The key used to identify an audio.</param>
		/// <param name="v">the value of the audio.</param>
		void AddSound(const std::string k, const std::string v) { soundsMap[k] = v; }

		/// <summary>
		/// This function adds an audio path into a map when it isn't present into an xml. 
		/// </summary>
		/// <param name="k">The key used to identify an audio.</param>
		/// <param name="v">the value of the audio.</param>
		void AddSoundIfMissing(const std::string k, const std::string v);

		/// <summary>
		/// This function add the path of a script into a map if it could be read from an xml.
		/// </summary>
		/// <param name="k">The key used to identify the path of a script.</param>
		/// <param name="v">The path of a script.</param>
		void AddMethod(const std::string k, const std::string v) { methodsMap[k] = v; }

		/// <summary>
		/// This function add the path of a script into a map if it insn't present into an xml.
		/// </summary>
		/// <param name="k">The key used to identify the path of a script.</param>
		/// <param name="v">The path of a script.</param>
		void AddMethodIfMissing(const std::string k, const std::string v);

		/// <summary>
		/// This function checks if the map has already read a property.
		/// </summary>
		/// <param name="k">The key used to identify a property.</param>
		/// <returns>True or false.</returns>
		bool HasProperty(const std::string k) { return (propertiesMap.count(k) > 0); }

		/// <summary>
		/// This function checks if the map has already read the path of a script.
		/// </summary>
		/// <param name="k">The key used to identify the path of a script.</param>
		/// <returns>True or false.</returns>
		bool HasMethod(const std::string k) { return (methodsMap.count(k) > 0); }

		/// <summary>
		/// This function checks if the map has already read the path of an audio.
		/// </summary>
		/// <param name="k">The key used to identify the path of an audio.</param>
		/// <returns></returns>
		bool HasSound(const std::string k) { return (soundsMap.count(k) > 0); }

		/// <summary>
		/// This function returns the value of a property.
		/// </summary>
		/// <param name="_property">The identifier of the property.</param>
		/// <returns>The value of the property if it was previously read; otherwise, it returns the "NOT_VALID" string.</returns>
		std::string GetPropertyValue(const std::string _property);

		/// <summary>
		/// This function returns the value of an audio path.
		/// </summary>
		/// <param name="_property">The identifier of the audio path.</param>
		/// <returns>The value of the audip path if it was previously read; otherwise, it returns the "NOT_VALID" string.</returns>
		std::string GetSoundPath(const std::string _sound);

		/// <summary>
		/// This function returns the value of a path of a script.
		/// </summary>
		/// <param name="_property">The identifier of the path of a script.</param>
		/// <returns>The value of the path of the script if it was previously read; otherwise, it returns the "NOT_VALID" string.</returns>
		std::string GetMethodScript(const std::string _method);

		/// <summary>
		/// (???) Andrebbe rimossa da Object.h?
		/// This function sets the name of a class of an object.
		/// </summary>
		/// <param name="_class">The class name.</param>
		void SetClassName(const std::string _class) { className = _class; }

		/// <summary>
		/// This function sets the name of a class.
		/// </summary>
		/// <param name="_parent">The class name.</param>
		void SetParentClass(const std::string _parent) { parent = _parent; }

		/// <summary>
		/// This function sets the type of the class.
		/// </summary>
		/// <param name="_type">The type of the class.</param>
		void SetClassType(std::string _type) { type = _type; }

		/// <summary>
		/// This function returns the name of a class.
		/// </summary>
		/// <returns></returns>
		std::string GetClassName(void) { return className; }

		/// <summary>
		/// This function returns the name of a parent class.
		/// </summary>
		/// <returns></returns>
		std::string GetParentClass(void) { return parent; }

		/// <summary>
		/// This function returns the type of the class.
		/// </summary>
		/// <returns></returns>
		std::string GetClassType(void) { return type; }

		/// <summary>
		/// This function sets the sprites related.
		/// </summary>
		/// <param name="_spriteData">The information about the sprite.</param>
		void SetSpriteData(glObjectSprite::SpriteData _spriteData) { spriteData = _spriteData; }

		glObjectSprite::SpriteData GetSpriteData(void) { return spriteData; }

		/// <summary>
		/// This function returns all the properties read.
		/// </summary>
		/// <returns></returns>
		std::map<std::string, std::string> GetPropertiesMap(void) { return propertiesMap; }

		/// <summary>
		/// This function returns all the methods read.
		/// </summary>
		/// <returns></returns>
		std::map<std::string, std::string> GetMethodsMap(void) { return methodsMap; }

		/// <summary>
		/// This function returns all the sound read.
		/// </summary>
		/// <returns></returns>
		std::map<std::string, std::string> GetSoundsMap(void) { return soundsMap; }

		~XMLClassData(void);
	private:
		std::map<std::string, std::string> propertiesMap;
		std::map<std::string, std::string> soundsMap;
		std::map<std::string, std::string> methodsMap;
		std::string className, parent, type;
		glObjectSprite::SpriteData spriteData;
	};

	/// <summary>
	/// This function try to convert into a integer the value of a property.
	/// It throws an exception if it isn't possible.
	/// </summary>
	/// <param name="_map">The map in which should be read the information.</param>
	/// <param name="_name">The indetifier of a property.</param>
	/// <param name="variable_ptr">A pointer used to retrieve a value.</param>
	void TryParseInteger(std::map<std::string, std::string> _map, std::string _name, int *variable_ptr);

	/// <summary>
	/// This function try to convert into a float the value of a property.
	/// It throws an exception if it isn't possible.
	/// </summary>
	/// <param name="_map">The map in which should be read the information.</param>
	/// <param name="_name">The indetifier of a property.</param>
	/// <param name="variable_ptr">A pointer used to retrieve a value.</param>
	void TryParseFloat(std::map<std::string, std::string> _map, std::string _name, float *variable_ptr);

	/// <summary>
	/// This function try to convert into a string the value of a property.
	/// It throws an exception if it isn't possible.
	/// </summary>
	/// <param name="_map">The map in which should be read the information.</param>
	/// <param name="_name">The indetifier of a property.</param>
	/// <param name="variable_ptr">A pointer used to retrieve a value.</param>	
	void TryParseString(std::map<std::string, std::string> _map, std::string _name, std::string *variable_ptr);

	/// <summary>
	/// This function sets a fixed ptr.
	/// </summary>
	/// <param name="_ptr">A ficed ptr.</param>
	void SetFixedPtr(XMLClassData* _ptr);

	XMLClassData* GetClassesData(std::string _class);

	/// <summary>
	/// This function reads the information about a class from an xml file.
	/// </summary>
	void ReadDataClassesFromXml(void);
	
	void AddXMLClassData(std::string _class, XMLClassData objData);

};
