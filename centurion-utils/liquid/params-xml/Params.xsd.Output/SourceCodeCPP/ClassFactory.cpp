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
#include "StdAfx.h"
#pragma warning( disable : 4786 )
#pragma warning( disable : 4251 )
#include "ParamsLib.h"
#include "ClassFactory.h"
#include "ParamsLib/Camera.h"
#include "ParamsLib/Mouse.h"
#include "ParamsLib/Params.h"
#include "ParamsLib/Ui.h"
#include "ParamsLib/Window.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

namespace ParamsLib
{

LtXmlLib18::CXmlObjectBasePtr CClassFactory::CreateClass(ParamsLib_Classes eCls, LPCTSTR strElementName)
{
	// This is structured like this to get around compiler limitations
	if (eCls == ClsName_NONE)
		throw LtXmlLib18::CLtException(_T("Invalid Class (NONE)"));

	switch (eCls)
	{
	case ClsName_CCamera:
		return new ParamsLib::CCamera(strElementName);
	case ClsName_CMouse:
		return new ParamsLib::CMouse(strElementName);
	case ClsName_CParams:
		return new ParamsLib::CParams(strElementName);
	case ClsName_CUi:
		return new ParamsLib::CUi(strElementName);
	case ClsName_CWindow:
		return new ParamsLib::CWindow(strElementName);
	default:
		throw LtXmlLib18::CLtException(_T("Unknown Class id"));
	}
	return NULL; // appease compiler
}

LtXmlLib18::CXmlCollectionBasePtr CClassFactory::CreateClassCollection(ParamsLib_Classes eCls, LPCTSTR strElementName, LPCTSTR strElementNamespaceURI, long iMinOccurs, long iMaxOccurs)
{
	switch(eCls)
	{
	case ClsName_NONE:
		throw LtXmlLib18::CLtException(_T("Invalid Class (NONE)"));
	default:
		throw LtXmlLib18::CLtException(_T("Unknown Collection Class id"));
	}
	return NULL; // appease compiler
}

LtXmlLib18::CXmlCollectionBasePtr CClassFactory::CreateEnumCollection(ParamsLib_Classes eCls, LPCTSTR strElementName, LPCTSTR strNamespace, long iMinOccurs, long iMaxOccurs)
{
	switch(eCls)
	{
	case ClsName_NONE:
		throw LtXmlLib18::CLtException(_T("Invalid Class (NONE)"));
	default:
		throw LtXmlLib18::CLtException(_T("Unknown Collection Class id"));
	}
	return NULL; // appease compiler
}


void CClassFactory::CleanMetaData()
{
	ParamsLib::CCamera::CleanMetaData();
	ParamsLib::CMouse::CleanMetaData();
	ParamsLib::CParams::CleanMetaData();
	ParamsLib::CUi::CleanMetaData();
	ParamsLib::CWindow::CleanMetaData();
}

// These Methods can be used to construct and load data into a generated object, 
// without needing to know the type of XML Document being loaded
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXml(LPCTSTR lpszXmlIn)
{
	return FromXml(lpszXmlIn, LtXmlLib18::CSerializationContext::GetDefaultContext());
}
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXml(LPCTSTR lpszXmlIn, const LtXmlLib18::CSerializationContext& context)
{
	LtXmlLib18::CXmlDocument XmlDoc(context);
	XmlDoc.LoadXml(lpszXmlIn);
	return FromXmlElement(XmlDoc.GetDocumentElement(), context);
}
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXmlFile(LPCTSTR lpszFileName )
{
	return FromXmlFile(lpszFileName, LtXmlLib18::CSerializationContext::GetDefaultContext());
}
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXmlFile(LPCTSTR lpszFileName, const LtXmlLib18::CSerializationContext& context)
{
	LtXmlLib18::CXmlDocument XmlDoc(context);
	XmlDoc.Load(lpszFileName);
	return FromXmlElement(XmlDoc.GetDocumentElement(), context);
}
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXmlStream(const LtXmlLib18::CBinaryData& stream)
{
	return FromXmlStream(stream, LtXmlLib18::CSerializationContext::GetDefaultContext());
}
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXmlStream(const LtXmlLib18::CBinaryData& stream, const LtXmlLib18::CSerializationContext& context)
{
	LtXmlLib18::CXmlDocument XmlDoc(context);
	XmlDoc.LoadXmlStream(stream.GetData(), stream.GetLength());
	return FromXmlElement(XmlDoc.GetDocumentElement(), context);
}
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXmlElement(LtXmlLib18::CXmlElement* pXmlParent)
{
	return FromXmlElement(pXmlParent, LtXmlLib18::CSerializationContext::GetDefaultContext());
}
LtXmlLib18::CXmlObjectBasePtr CClassFactory::FromXmlElement(LtXmlLib18::CXmlElement* pXmlParent, const LtXmlLib18::CSerializationContext& context)
{
	LtXmlLib18::CXmlObjectBasePtr spRetVal;
	std::tstring strElementName;
	std::tstring strElementNamespaceUri;

	// Get the type name this is either 
	// from the element i.e. <Parent>... = Parent
	// or from the type i.e. <Parent xsi:type="someNS:SomeElement">... = SomeElement
	if (GetElementType(pXmlParent) == _T(""))
	{
		strElementName = pXmlParent->GetLocalName();
		strElementNamespaceUri = pXmlParent->GetNamespaceURI();
	}
	else
	{
		strElementName = GetElementType(pXmlParent);
		strElementNamespaceUri = GetElementTypeNamespaceUri(pXmlParent);
	}

	// create the appropriate object
	if (strElementName == _T(""))
		throw LtXmlLib18::CLtInvalidStateException(_T("The element to load has no name")); 
	if (strElementName == _T("camera") && strElementNamespaceUri == _T(""))
		spRetVal = new ParamsLib::CCamera();
	if (strElementName == _T("mouse") && strElementNamespaceUri == _T(""))
		spRetVal = new ParamsLib::CMouse();
	if (strElementName == _T("params") && strElementNamespaceUri == _T(""))
		spRetVal = new ParamsLib::CParams();
	if (strElementName == _T("ui") && strElementNamespaceUri == _T(""))
		spRetVal = new ParamsLib::CUi();
	if (strElementName == _T("window") && strElementNamespaceUri == _T(""))
		spRetVal = new ParamsLib::CWindow();
	if (spRetVal == NULL)
	{
		throw LtXmlLib18::CLtException(
			LtXmlLib18::FormatString(_T("Failed load the element ") _T_STR_FORMATTER _T(":") _T_STR_FORMATTER _T(". No appropriate class exists to load the data into. Ensure that the XML document complies with the schema."), 
			pXmlParent->GetLocalName().c_str(), pXmlParent->GetNamespaceURI().c_str()));
	}
			
	// load the data into the object
	spRetVal->FromXmlElement(pXmlParent, context);

	return spRetVal;
}

// ##HAND_CODED_BLOCK_START ID="Additional Methods - ParamsLib"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Methods here...

// ##HAND_CODED_BLOCK_END ID="Additional Methods - ParamsLib"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

}; // namespace ParamsLib
// EOF

