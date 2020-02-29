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
#include "../ParamsLib/Camera.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Name                    : camera
// Long Name               : camera
// Element Name            : camera
// Class Namespace         : ParamsLib
// Namespace Alias         : 
// Schema Namespace        : 
// Mapped Class Name       : CCamera
// Mapped Class Full Name  : ParamsLib::CCamera
// Mapped Class File Name  : CCamera
// IsAbstract              : False
// IsElement               : True
// IsComplexType           : True

namespace ParamsLib
{

LtXmlLib18Data::CParentElementInfo* CCamera::ms_pParentElementInfo = NULL;
LtXmlLib18Data::CAttributeInfo** CCamera::ms_ppAttributeInfo = NULL;
LtXmlLib18Data::CElementInfo** CCamera::ms_ppElementInfo = NULL;

CCameraPtr CCamera::CreateInstance(LPCTSTR lpctElementName/*=_T("camera")*/)
{
	return new ParamsLib::CCamera(lpctElementName);
}

/* 	
 * Constructor for CCamera
 *
 * The class is created with all the mandatory fields populated with the
 * default data.
 * All Collection objects are created.
 * However any 1-n relationships (these are represented as collections) are
 * empty. To comply with the schema these must be populated before the xml
 * obtained from ToXml is valid against the schema C:\Users\utente\source\repos\centurion\centurion-source\Params.xsd
 */
CCamera::CCamera(LPCTSTR lpctElementName/*=_T("camera")*/)
 : CInstanceMonitor(_T("CCamera"))
{
	m_elementName = lpctElementName;
	Init();
}

CCamera::~CCamera()
{
	Cleanup();
}

void CCamera::Cleanup()
{
	// unregister for any events we have asked for
	// cos there'll be no one left to hear soon
}

void CCamera::OnEvent(LtXmlLib18::CXmlObjectBase* pMsgSource, LtXmlLib18::IEventSink::MsgType eMsgType, void* pData)
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
void CCamera::Init()
{
	Cleanup();

	this->m_XPosition = 0;
	this->m_YPosition = 0;
	


// ##HAND_CODED_BLOCK_START ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Init Settings...

// ##HAND_CODED_BLOCK_END ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
}
	

void CCamera::AccessProperty(int iPropertyIndex, bool bRead, LtXmlLib18::LtVariant& rValue)
{
	if (bRead)
	{
		switch(iPropertyIndex)
		{
		case 1:
				rValue.SetInteger(GetXPosition());
			break;
		case 2:
				rValue.SetInteger(GetYPosition());
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
			SetXPosition(rValue.GetInteger());
			break;
		case 2:
			SetYPosition(rValue.GetInteger());
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
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CCamera::GetXPosition() const
{
	return this->m_XPosition;
}
void CCamera::SetXPosition(LtXmlLib18::CBigInteger value)
{ 
	this->m_XPosition = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CCamera::GetYPosition() const
{
	return this->m_YPosition;
}
void CCamera::SetYPosition(LtXmlLib18::CBigInteger value)
{ 
	this->m_YPosition = value; 
}

/*
 * Allows the class to be copied
 * Performs a 'deep copy' of all the data in the class (and its children)
 */
ParamsLib::CCameraPtr CCamera::Clone() const
{
	ParamsLib::CCameraPtr newObject = CreateInstance(m_elementName.c_str());

	int index = 0;
	newObject->m_XPosition = m_XPosition;
	newObject->m_YPosition = m_YPosition;


// ##HAND_CODED_BLOCK_START ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional clone code here...

// ##HAND_CODED_BLOCK_END ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	return newObject.Ptr();
}

std::tstring CCamera::GetTargetNamespace() const
{
	return _T("");
}	

std::tstring CCamera::GetNamespace() const
{
	return _T("");
}	

LtXmlLib18::CXmlObjectBase* CCamera::GetBase()
{
	return this;
}	

void CCamera::CleanMetaData()
{
	LtXmlLib18::CXmlGeneratedClass::CleanMetaData(ms_pParentElementInfo, ms_ppElementInfo, ms_ppAttributeInfo);
}

LtXmlLib18Data::CParentElementInfo* CCamera::GetClassInfo() const
{
	if (ms_pParentElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_pParentElementInfo == NULL)
		{
			ms_pParentElementInfo = new LtXmlLib18Data::CParentElementInfo(
																			LtXmlLib18Data::XmlElementGroupType_SEQUENCE,
																			LtXmlLib18Data::XmlElementType_ELEMENT, 
																			_T("camera"),					 
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

LtXmlLib18Data::CElementInfo** CCamera::GetClassElementInfo() const
{
	if (ms_ppElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_ppElementInfo == NULL)
		{
			ms_ppElementInfo = new LtXmlLib18Data::CElementInfo*[3];
		
			ms_ppElementInfo[0] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("xPosition"), _T(""), 1, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[1] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("yPosition"), _T(""), 2, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[2] = NULL;
		}
		m_csInit.Leave();
	}
	return ms_ppElementInfo;
}

LtXmlLib18Data::CAttributeInfo**	CCamera::GetClassAttributeInfo() const
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





