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
#include "../ParamsLib/Window.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Name                    : window
// Long Name               : window
// Element Name            : window
// Class Namespace         : ParamsLib
// Namespace Alias         : 
// Schema Namespace        : 
// Mapped Class Name       : CWindow
// Mapped Class Full Name  : ParamsLib::CWindow
// Mapped Class File Name  : CWindow
// IsAbstract              : False
// IsElement               : True
// IsComplexType           : True

namespace ParamsLib
{

LtXmlLib18Data::CParentElementInfo* CWindow::ms_pParentElementInfo = NULL;
LtXmlLib18Data::CAttributeInfo** CWindow::ms_ppAttributeInfo = NULL;
LtXmlLib18Data::CElementInfo** CWindow::ms_ppElementInfo = NULL;

CWindowPtr CWindow::CreateInstance(LPCTSTR lpctElementName/*=_T("window")*/)
{
	return new ParamsLib::CWindow(lpctElementName);
}

/* 	
 * Constructor for CWindow
 *
 * The class is created with all the mandatory fields populated with the
 * default data.
 * All Collection objects are created.
 * However any 1-n relationships (these are represented as collections) are
 * empty. To comply with the schema these must be populated before the xml
 * obtained from ToXml is valid against the schema C:\Users\utente\source\repos\centurion\centurion-source\Params.xsd
 */
CWindow::CWindow(LPCTSTR lpctElementName/*=_T("window")*/)
 : CInstanceMonitor(_T("CWindow"))
{
	m_elementName = lpctElementName;
	Init();
}

CWindow::~CWindow()
{
	Cleanup();
}

void CWindow::Cleanup()
{
	// unregister for any events we have asked for
	// cos there'll be no one left to hear soon
}

void CWindow::OnEvent(LtXmlLib18::CXmlObjectBase* pMsgSource, LtXmlLib18::IEventSink::MsgType eMsgType, void* pData)
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
void CWindow::Init()
{
	Cleanup();

	this->m_HeightZoomed = 0;
	this->m_Ratio = 0;
	this->m_WidthZoomed = 0;
	this->m_ShouldClose = false;
	


// ##HAND_CODED_BLOCK_START ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Init Settings...

// ##HAND_CODED_BLOCK_END ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
}
	

void CWindow::AccessProperty(int iPropertyIndex, bool bRead, LtXmlLib18::LtVariant& rValue)
{
	if (bRead)
	{
		switch(iPropertyIndex)
		{
		case 1:
				rValue.SetDecimal(GetHeightZoomed());
			break;
		case 2:
				rValue.SetDecimal(GetRatio());
			break;
		case 3:
				rValue.SetDecimal(GetWidthZoomed());
			break;
		case 4:
				rValue.SetBoolean(GetShouldClose());
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
			SetHeightZoomed(rValue.GetDecimal());
			break;
		case 2:
			SetRatio(rValue.GetDecimal());
			break;
		case 3:
			SetWidthZoomed(rValue.GetDecimal());
			break;
		case 4:
			SetShouldClose(rValue.GetBoolean());
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
LtXmlLib18::CDecimal CWindow::GetHeightZoomed() const
{
	return this->m_HeightZoomed;
}
void CWindow::SetHeightZoomed(LtXmlLib18::CDecimal value)
{ 
	this->m_HeightZoomed = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CDecimal CWindow::GetRatio() const
{
	return this->m_Ratio;
}
void CWindow::SetRatio(LtXmlLib18::CDecimal value)
{ 
	this->m_Ratio = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CDecimal CWindow::GetWidthZoomed() const
{
	return this->m_WidthZoomed;
}
void CWindow::SetWidthZoomed(LtXmlLib18::CDecimal value)
{ 
	this->m_WidthZoomed = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to false.
 */
bool CWindow::GetShouldClose() const
{
	return this->m_ShouldClose;
}
void CWindow::SetShouldClose(bool value)
{ 
	this->m_ShouldClose = value; 
}

/*
 * Allows the class to be copied
 * Performs a 'deep copy' of all the data in the class (and its children)
 */
ParamsLib::CWindowPtr CWindow::Clone() const
{
	ParamsLib::CWindowPtr newObject = CreateInstance(m_elementName.c_str());

	int index = 0;
	newObject->m_HeightZoomed = m_HeightZoomed;
	newObject->m_Ratio = m_Ratio;
	newObject->m_WidthZoomed = m_WidthZoomed;
	newObject->m_ShouldClose = m_ShouldClose;


// ##HAND_CODED_BLOCK_START ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional clone code here...

// ##HAND_CODED_BLOCK_END ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	return newObject.Ptr();
}

std::tstring CWindow::GetTargetNamespace() const
{
	return _T("");
}	

std::tstring CWindow::GetNamespace() const
{
	return _T("");
}	

LtXmlLib18::CXmlObjectBase* CWindow::GetBase()
{
	return this;
}	

void CWindow::CleanMetaData()
{
	LtXmlLib18::CXmlGeneratedClass::CleanMetaData(ms_pParentElementInfo, ms_ppElementInfo, ms_ppAttributeInfo);
}

LtXmlLib18Data::CParentElementInfo* CWindow::GetClassInfo() const
{
	if (ms_pParentElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_pParentElementInfo == NULL)
		{
			ms_pParentElementInfo = new LtXmlLib18Data::CParentElementInfo(
																			LtXmlLib18Data::XmlElementGroupType_SEQUENCE,
																			LtXmlLib18Data::XmlElementType_ELEMENT, 
																			_T("window"),					 
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

LtXmlLib18Data::CElementInfo** CWindow::GetClassElementInfo() const
{
	if (ms_ppElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_ppElementInfo == NULL)
		{
			ms_ppElementInfo = new LtXmlLib18Data::CElementInfo*[5];
		
			ms_ppElementInfo[0] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("heightZoomed"), _T(""), 1, false, LtXmlLib18::ItemType_decimal, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[1] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("ratio"), _T(""), 2, false, LtXmlLib18::ItemType_decimal, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[2] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("widthZoomed"), _T(""), 3, false, LtXmlLib18::ItemType_decimal, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[3] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("shouldClose"), _T(""), 4, false, LtXmlLib18::ItemType_boolean, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[4] = NULL;
		}
		m_csInit.Leave();
	}
	return ms_ppElementInfo;
}

LtXmlLib18Data::CAttributeInfo**	CWindow::GetClassAttributeInfo() const
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





