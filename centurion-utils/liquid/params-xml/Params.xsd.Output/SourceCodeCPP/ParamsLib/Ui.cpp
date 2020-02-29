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
#include "../ParamsLib/Ui.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Name                    : ui
// Long Name               : ui
// Element Name            : ui
// Class Namespace         : ParamsLib
// Namespace Alias         : 
// Schema Namespace        : 
// Mapped Class Name       : CUi
// Mapped Class Full Name  : ParamsLib::CUi
// Mapped Class File Name  : CUi
// IsAbstract              : False
// IsElement               : True
// IsComplexType           : True

namespace ParamsLib
{

LtXmlLib18Data::CParentElementInfo* CUi::ms_pParentElementInfo = NULL;
LtXmlLib18Data::CAttributeInfo** CUi::ms_ppAttributeInfo = NULL;
LtXmlLib18Data::CElementInfo** CUi::ms_ppElementInfo = NULL;

CUiPtr CUi::CreateInstance(LPCTSTR lpctElementName/*=_T("ui")*/)
{
	return new ParamsLib::CUi(lpctElementName);
}

/* 	
 * Constructor for CUi
 *
 * The class is created with all the mandatory fields populated with the
 * default data.
 * All Collection objects are created.
 * However any 1-n relationships (these are represented as collections) are
 * empty. To comply with the schema these must be populated before the xml
 * obtained from ToXml is valid against the schema C:\Users\utente\source\repos\centurion\centurion-source\Params.xsd
 */
CUi::CUi(LPCTSTR lpctElementName/*=_T("ui")*/)
 : CInstanceMonitor(_T("CUi"))
{
	m_elementName = lpctElementName;
	Init();
}

CUi::~CUi()
{
	Cleanup();
}

void CUi::Cleanup()
{
	// unregister for any events we have asked for
	// cos there'll be no one left to hear soon
}

void CUi::OnEvent(LtXmlLib18::CXmlObjectBase* pMsgSource, LtXmlLib18::IEventSink::MsgType eMsgType, void* pData)
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
void CUi::Init()
{
	Cleanup();

	this->m_BottomBarHeight = 0;
	this->m_TopBarHeight = 0;
	


// ##HAND_CODED_BLOCK_START ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Init Settings...

// ##HAND_CODED_BLOCK_END ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
}
	

void CUi::AccessProperty(int iPropertyIndex, bool bRead, LtXmlLib18::LtVariant& rValue)
{
	if (bRead)
	{
		switch(iPropertyIndex)
		{
		case 1:
				rValue.SetInteger(GetBottomBarHeight());
			break;
		case 2:
				rValue.SetInteger(GetTopBarHeight());
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
			SetBottomBarHeight(rValue.GetInteger());
			break;
		case 2:
			SetTopBarHeight(rValue.GetInteger());
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
LtXmlLib18::CBigInteger CUi::GetBottomBarHeight() const
{
	return this->m_BottomBarHeight;
}
void CUi::SetBottomBarHeight(LtXmlLib18::CBigInteger value)
{ 
	this->m_BottomBarHeight = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CUi::GetTopBarHeight() const
{
	return this->m_TopBarHeight;
}
void CUi::SetTopBarHeight(LtXmlLib18::CBigInteger value)
{ 
	this->m_TopBarHeight = value; 
}

/*
 * Allows the class to be copied
 * Performs a 'deep copy' of all the data in the class (and its children)
 */
ParamsLib::CUiPtr CUi::Clone() const
{
	ParamsLib::CUiPtr newObject = CreateInstance(m_elementName.c_str());

	int index = 0;
	newObject->m_BottomBarHeight = m_BottomBarHeight;
	newObject->m_TopBarHeight = m_TopBarHeight;


// ##HAND_CODED_BLOCK_START ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional clone code here...

// ##HAND_CODED_BLOCK_END ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	return newObject.Ptr();
}

std::tstring CUi::GetTargetNamespace() const
{
	return _T("");
}	

std::tstring CUi::GetNamespace() const
{
	return _T("");
}	

LtXmlLib18::CXmlObjectBase* CUi::GetBase()
{
	return this;
}	

void CUi::CleanMetaData()
{
	LtXmlLib18::CXmlGeneratedClass::CleanMetaData(ms_pParentElementInfo, ms_ppElementInfo, ms_ppAttributeInfo);
}

LtXmlLib18Data::CParentElementInfo* CUi::GetClassInfo() const
{
	if (ms_pParentElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_pParentElementInfo == NULL)
		{
			ms_pParentElementInfo = new LtXmlLib18Data::CParentElementInfo(
																			LtXmlLib18Data::XmlElementGroupType_SEQUENCE,
																			LtXmlLib18Data::XmlElementType_ELEMENT, 
																			_T("ui"),					 
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

LtXmlLib18Data::CElementInfo** CUi::GetClassElementInfo() const
{
	if (ms_ppElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_ppElementInfo == NULL)
		{
			ms_ppElementInfo = new LtXmlLib18Data::CElementInfo*[3];
		
			ms_ppElementInfo[0] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("bottomBarHeight"), _T(""), 1, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[1] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("topBarHeight"), _T(""), 2, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[2] = NULL;
		}
		m_csInit.Leave();
	}
	return ms_ppElementInfo;
}

LtXmlLib18Data::CAttributeInfo**	CUi::GetClassAttributeInfo() const
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





