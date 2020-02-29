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
#pragma warning (push)
#pragma warning (disable:4251)	// template export warning
#pragma warning (disable:4786)	// long debug names
#include "../ParamsLib.h"
#include "../ParamsLib/Params.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Name                    : params
// Long Name               : params
// Element Name            : params
// Class Namespace         : ParamsLib
// Namespace Alias         : 
// Schema Namespace        : 
// Mapped Class Name       : CParams
// Mapped Class Full Name  : ParamsLib::CParams
// Mapped Class File Name  : CParams
// IsAbstract              : False
// IsElement               : True
// IsComplexType           : False

namespace ParamsLib
{

LtXmlLib18Data::CParentElementInfo* CParams::ms_pParentElementInfo = NULL;
LtXmlLib18Data::CAttributeInfo** CParams::ms_ppAttributeInfo = NULL;
LtXmlLib18Data::CElementInfo** CParams::ms_ppElementInfo = NULL;

CParamsPtr CParams::CreateInstance(LPCTSTR lpctElementName/*=_T("params")*/)
{
	return new ParamsLib::CParams(lpctElementName);
}

/* 	
 * Constructor for CParams
 *
 * The class is created with all the mandatory fields populated with the
 * default data.
 * All Collection objects are created.
 * However any 1-n relationships (these are represented as collections) are
 * empty. To comply with the schema these must be populated before the xml
 * obtained from ToXml is valid against the schema C:\Users\utente\source\repos\centurion\centurion-source\Params.xsd
 */
CParams::CParams(LPCTSTR lpctElementName/*=_T("params")*/)
 : CInstanceMonitor(_T("CParams"))
{
	m_elementName = lpctElementName;
	Init();
}

CParams::~CParams()
{
	Cleanup();
}

void CParams::Cleanup()
{
	// unregister for any events we have asked for
	// cos there'll be no one left to hear soon
}

void CParams::OnEvent(LtXmlLib18::CXmlObjectBase* pMsgSource, LtXmlLib18::IEventSink::MsgType eMsgType, void* pData)
{
	if (eMsgType == LtXmlLib18::IEventSink::MT_CollectionChangeEvent)
	{
	}
}

/*
 * Initializes the class
 *
 * The Creates all the mandatory fields (populated with the default data) 
 * All Collection object are created.
 * However any 1-n relationships (these are represented as collections) are
 * empty. To comply with the schema these must be populated before the xml
 * obtained from ToXml is valid against the schema C:\Users\utente\source\repos\centurion\centurion-source\Params.xsd.
 */
void CParams::Init()
{
	Cleanup();

	this->m_Camera = dynamic_cast<ParamsLib::CCamera*>(ParamsLib::CClassFactory::CreateClass(ParamsLib::CClassFactory::ClsName_CCamera, _T("camera")).Ptr());
	this->m_Mouse = dynamic_cast<ParamsLib::CMouse*>(ParamsLib::CClassFactory::CreateClass(ParamsLib::CClassFactory::ClsName_CMouse, _T("mouse")).Ptr());
	this->m_Window = dynamic_cast<ParamsLib::CWindow*>(ParamsLib::CClassFactory::CreateClass(ParamsLib::CClassFactory::ClsName_CWindow, _T("window")).Ptr());
	this->m_Ui = dynamic_cast<ParamsLib::CUi*>(ParamsLib::CClassFactory::CreateClass(ParamsLib::CClassFactory::ClsName_CUi, _T("ui")).Ptr());
	


// ##HAND_CODED_BLOCK_START ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Init Settings...

// ##HAND_CODED_BLOCK_END ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
}
	

void CParams::AccessProperty(int iPropertyIndex, bool bRead, LtXmlLib18::LtVariant& rValue)
{
	if (bRead)
	{
		switch(iPropertyIndex)
		{
		case 1:
				rValue.SetXmlObject(GetCamera().Ptr());
			break;
		case 2:
				rValue.SetXmlObject(GetMouse().Ptr());
			break;
		case 3:
				rValue.SetXmlObject(GetWindow().Ptr());
			break;
		case 4:
				rValue.SetXmlObject(GetUi().Ptr());
			break;
		default:
			throw LtXmlLib18::CLtException(_T("Unknown Property Index"));
		};
	}
	else
	{
		switch(iPropertyIndex)
		{
		case 1:
			SetCamera(dynamic_cast<ParamsLib::CCamera*>(rValue.GetXmlObject().Ptr()));
			break;
		case 2:
			SetMouse(dynamic_cast<ParamsLib::CMouse*>(rValue.GetXmlObject().Ptr()));
			break;
		case 3:
			SetWindow(dynamic_cast<ParamsLib::CWindow*>(rValue.GetXmlObject().Ptr()));
			break;
		case 4:
			SetUi(dynamic_cast<ParamsLib::CUi*>(rValue.GetXmlObject().Ptr()));
			break;
		default:
			throw LtXmlLib18::CLtException(_T("Unknown Property Index"));
		}
	}
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * If this property is set, then the object will be COPIED. If the property is set to NULL an exception is raised.
 */
ParamsLib::CCameraPtr CParams::GetCamera() const
{ 
	return this->m_Camera;  
}
void CParams::SetCamera(ParamsLib::CCamera* value)
{ 
	Throw_IfPropertyIsNull(value, _T("camera"));
	if (value != NULL)
		SetElementName(value, _T("camera"));
	this->m_Camera = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * If this property is set, then the object will be COPIED. If the property is set to NULL an exception is raised.
 */
ParamsLib::CMousePtr CParams::GetMouse() const
{ 
	return this->m_Mouse;  
}
void CParams::SetMouse(ParamsLib::CMouse* value)
{ 
	Throw_IfPropertyIsNull(value, _T("mouse"));
	if (value != NULL)
		SetElementName(value, _T("mouse"));
	this->m_Mouse = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * If this property is set, then the object will be COPIED. If the property is set to NULL an exception is raised.
 */
ParamsLib::CWindowPtr CParams::GetWindow() const
{ 
	return this->m_Window;  
}
void CParams::SetWindow(ParamsLib::CWindow* value)
{ 
	Throw_IfPropertyIsNull(value, _T("window"));
	if (value != NULL)
		SetElementName(value, _T("window"));
	this->m_Window = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * If this property is set, then the object will be COPIED. If the property is set to NULL an exception is raised.
 */
ParamsLib::CUiPtr CParams::GetUi() const
{ 
	return this->m_Ui;  
}
void CParams::SetUi(ParamsLib::CUi* value)
{ 
	Throw_IfPropertyIsNull(value, _T("ui"));
	if (value != NULL)
		SetElementName(value, _T("ui"));
	this->m_Ui = value; 
}

/*
 * Allows the class to be copied
 * Performs a 'deep copy' of all the data in the class (and its children)
 */
ParamsLib::CParamsPtr CParams::Clone() const
{
	ParamsLib::CParamsPtr newObject = CreateInstance(m_elementName.c_str());

	int index = 0;
	newObject->m_Camera = NULL;
	if (m_Camera != NULL)
		newObject->m_Camera = dynamic_cast<ParamsLib::CCamera*>(m_Camera->Clone().Ptr());
	newObject->m_Mouse = NULL;
	if (m_Mouse != NULL)
		newObject->m_Mouse = dynamic_cast<ParamsLib::CMouse*>(m_Mouse->Clone().Ptr());
	newObject->m_Window = NULL;
	if (m_Window != NULL)
		newObject->m_Window = dynamic_cast<ParamsLib::CWindow*>(m_Window->Clone().Ptr());
	newObject->m_Ui = NULL;
	if (m_Ui != NULL)
		newObject->m_Ui = dynamic_cast<ParamsLib::CUi*>(m_Ui->Clone().Ptr());


// ##HAND_CODED_BLOCK_START ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional clone code here...

// ##HAND_CODED_BLOCK_END ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	return newObject.Ptr();
}

std::tstring CParams::GetTargetNamespace() const
{
	return _T("");
}	

std::tstring CParams::GetNamespace() const
{
	return _T("");
}	

LtXmlLib18::CXmlObjectBase* CParams::GetBase()
{
	return this;
}	

void CParams::CleanMetaData()
{
	LtXmlLib18::CXmlGeneratedClass::CleanMetaData(ms_pParentElementInfo, ms_ppElementInfo, ms_ppAttributeInfo);
}

LtXmlLib18Data::CParentElementInfo* CParams::GetClassInfo() const
{
	if (ms_pParentElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_pParentElementInfo == NULL)
		{
			ms_pParentElementInfo = new LtXmlLib18Data::CParentElementInfo(
																			LtXmlLib18Data::XmlElementGroupType_SEQUENCE,
																			LtXmlLib18Data::XmlElementType_ELEMENT, 
																			_T("params"),					 
																			_T(""), 
																			true,
																			false,
																			-1,
																			LtXmlLib18::ItemType_none,
																			NULL,
																			false);
		}
		m_csInit.Leave();
	}
	
	return ms_pParentElementInfo;
}

LtXmlLib18Data::CElementInfo** CParams::GetClassElementInfo() const
{
	if (ms_ppElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_ppElementInfo == NULL)
		{
			ms_ppElementInfo = new LtXmlLib18Data::CElementInfo*[5];
		
			ms_ppElementInfo[0] = new LtXmlLib18Data::CElementInfoSeqClsMnd(_T("camera"), _T(""), 1, LtXmlLib18Data::XmlElementType_ELEMENT, (LtXmlLib18Data::pfnCreateClassDef)&ParamsLib::CClassFactory::CreateClass, ParamsLib::CClassFactory::ClsName_CCamera, true);
			ms_ppElementInfo[1] = new LtXmlLib18Data::CElementInfoSeqClsMnd(_T("mouse"), _T(""), 2, LtXmlLib18Data::XmlElementType_ELEMENT, (LtXmlLib18Data::pfnCreateClassDef)&ParamsLib::CClassFactory::CreateClass, ParamsLib::CClassFactory::ClsName_CMouse, true);
			ms_ppElementInfo[2] = new LtXmlLib18Data::CElementInfoSeqClsMnd(_T("window"), _T(""), 3, LtXmlLib18Data::XmlElementType_ELEMENT, (LtXmlLib18Data::pfnCreateClassDef)&ParamsLib::CClassFactory::CreateClass, ParamsLib::CClassFactory::ClsName_CWindow, true);
			ms_ppElementInfo[3] = new LtXmlLib18Data::CElementInfoSeqClsMnd(_T("ui"), _T(""), 4, LtXmlLib18Data::XmlElementType_ELEMENT, (LtXmlLib18Data::pfnCreateClassDef)&ParamsLib::CClassFactory::CreateClass, ParamsLib::CClassFactory::ClsName_CUi, true);
			ms_ppElementInfo[4] = NULL;
		}
		m_csInit.Leave();
	}
	return ms_ppElementInfo;
}

LtXmlLib18Data::CAttributeInfo**	CParams::GetClassAttributeInfo() const
{
	if (ms_ppAttributeInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_ppAttributeInfo == NULL)
		{
			ms_ppAttributeInfo = new LtXmlLib18Data::CAttributeInfo*[1];
			ms_ppAttributeInfo[0] = NULL;
		}
		m_csInit.Leave();
	}
	return ms_ppAttributeInfo;
}

// ##HAND_CODED_BLOCK_START ID="Additional Methods"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Methods here...

// ##HAND_CODED_BLOCK_END ID="Additional Methods"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

}; // namespace





