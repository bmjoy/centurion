/**********************************************************************************************
 * Copyright (c) 2001-2020 Liquid Technologies Limited. All rights reserved.
 * See www.liquid-technologies.com for product details.
 *
 * Please see products End User License Agreement for distribution permissions.
 *
 * WARNING: THIS FILE IS GENERATED
 * Changes made outside of ##HAND_CODED_BLOCK_START blocks will be overwritten
 *
 * Generation  :  by Liquid XML Data Binder 18.0.2.9849
 * Using Schema: C:\Users\utente\source\repos\centurion\centurion-source\Params.xsd
 **********************************************************************************************/
#ifndef _ParamsLib_CLASS_FACTORY_H
#define _ParamsLib_CLASS_FACTORY_H

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

namespace ParamsLib
{
	class ParamsLib_DLL CClassFactory  : public LtXmlLib18::CClassfactoryBase
	{
	public:
		// These Methods can be used to construct and load data into a generated object, 
		// without needing to know the type of XML Document being loaded
		static LtXmlLib18::CXmlObjectBasePtr FromXml(LPCTSTR lpszXmlIn);
		static LtXmlLib18::CXmlObjectBasePtr FromXml(LPCTSTR lpszXmlIn, const LtXmlLib18::CSerializationContext& context);
		static LtXmlLib18::CXmlObjectBasePtr FromXmlFile(LPCTSTR lpszFileName);
		static LtXmlLib18::CXmlObjectBasePtr FromXmlFile(LPCTSTR lpszFileName, const LtXmlLib18::CSerializationContext&  context);
		static LtXmlLib18::CXmlObjectBasePtr FromXmlStream(const LtXmlLib18::CBinaryData& stream);
		static LtXmlLib18::CXmlObjectBasePtr FromXmlStream(const LtXmlLib18::CBinaryData& stream, const LtXmlLib18::CSerializationContext& context);
		static LtXmlLib18::CXmlObjectBasePtr FromXmlElement(LtXmlLib18::CXmlElement* pXmlParent);
		static LtXmlLib18::CXmlObjectBasePtr FromXmlElement(LtXmlLib18::CXmlElement* pXmlParent, const LtXmlLib18::CSerializationContext& context);
	
		enum ParamsLib_Classes
		{
			ClsName_NONE
			, ClsName_CCamera
			, ClsName_CMouse
			, ClsName_CParams
			, ClsName_CUi
			, ClsName_CWindow
		};
		
		
		static LtXmlLib18::CXmlObjectBasePtr CreateClass(ParamsLib_Classes eCls, LPCTSTR strElementName);
		static LtXmlLib18::CXmlCollectionBasePtr CreateClassCollection(ParamsLib_Classes eCls, LPCTSTR strElementName, LPCTSTR strElementNamespaceURI, long iMinOccurs, long iMaxOccurs);
		static LtXmlLib18::CXmlCollectionBasePtr CreateEnumCollection(ParamsLib_Classes eCls, LPCTSTR strElementName, LPCTSTR strNamespace, long iMinOccurs, long iMaxOccurs);

		// removes all cached data help by the data binding library.
		// can be called at the end of the program to free memory that may look like its leaked
		// or if the data binding code is no longer needed within the application, and you need to free up space.
		static void CleanMetaData();

// ##HAND_CODED_BLOCK_START ID="Additional Methods - ParamsLib"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Methods here...

// ##HAND_CODED_BLOCK_END ID="Additional Methods - ParamsLib"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	};
};

#endif // _ParamsLib_CLASS_FACTORY_H

