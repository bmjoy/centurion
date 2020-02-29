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
#include "../ParamsLib/Mouse.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Name                    : mouse
// Long Name               : mouse
// Element Name            : mouse
// Class Namespace         : ParamsLib
// Namespace Alias         : 
// Schema Namespace        : 
// Mapped Class Name       : CMouse
// Mapped Class Full Name  : ParamsLib::CMouse
// Mapped Class File Name  : CMouse
// IsAbstract              : False
// IsElement               : True
// IsComplexType           : True

namespace ParamsLib
{

LtXmlLib18Data::CParentElementInfo* CMouse::ms_pParentElementInfo = NULL;
LtXmlLib18Data::CAttributeInfo** CMouse::ms_ppAttributeInfo = NULL;
LtXmlLib18Data::CElementInfo** CMouse::ms_ppElementInfo = NULL;

CMousePtr CMouse::CreateInstance(LPCTSTR lpctElementName/*=_T("mouse")*/)
{
	return new ParamsLib::CMouse(lpctElementName);
}

/* 	
 * Constructor for CMouse
 *
 * The class is created with all the mandatory fields populated with the
 * default data.
 * All Collection objects are created.
 * However any 1-n relationships (these are represented as collections) are
 * empty. To comply with the schema these must be populated before the xml
 * obtained from ToXml is valid against the schema C:\Users\utente\source\repos\centurion\centurion-source\Params.xsd
 */
CMouse::CMouse(LPCTSTR lpctElementName/*=_T("mouse")*/)
 : CInstanceMonitor(_T("CMouse"))
{
	m_elementName = lpctElementName;
	Init();
}

CMouse::~CMouse()
{
	Cleanup();
}

void CMouse::Cleanup()
{
	// unregister for any events we have asked for
	// cos there'll be no one left to hear soon
}

void CMouse::OnEvent(LtXmlLib18::CXmlObjectBase* pMsgSource, LtXmlLib18::IEventSink::MsgType eMsgType, void* pData)
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
void CMouse::Init()
{
	Cleanup();

	this->m_ScrollValue = 0;
	this->m_XLeftClick = 0;
	this->m_XPosition = 0;
	this->m_XRightClick = 0;
	this->m_Y2dPosition = 0;
	this->m_Y2dRightClick = 0;
	this->m_YLeftClick = 0;
	this->m_YPosition = 0;
	this->m_YRightClick = 0;
	this->m_LeftClick = false;
	this->m_LeftHold = false;
	this->m_Release = false;
	this->m_RightClick = false;
	this->m_ScrollBool = false;
	


// ##HAND_CODED_BLOCK_START ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Init Settings...

// ##HAND_CODED_BLOCK_END ID="Additional Inits"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
}
	

void CMouse::AccessProperty(int iPropertyIndex, bool bRead, LtXmlLib18::LtVariant& rValue)
{
	if (bRead)
	{
		switch(iPropertyIndex)
		{
		case 1:
				rValue.SetInteger(GetScrollValue());
			break;
		case 2:
				rValue.SetInteger(GetXLeftClick());
			break;
		case 3:
				rValue.SetInteger(GetXPosition());
			break;
		case 4:
				rValue.SetInteger(GetXRightClick());
			break;
		case 5:
				rValue.SetInteger(GetY2dPosition());
			break;
		case 6:
				rValue.SetInteger(GetY2dRightClick());
			break;
		case 7:
				rValue.SetInteger(GetYLeftClick());
			break;
		case 8:
				rValue.SetInteger(GetYPosition());
			break;
		case 9:
				rValue.SetInteger(GetYRightClick());
			break;
		case 10:
				rValue.SetBoolean(GetLeftClick());
			break;
		case 11:
				rValue.SetBoolean(GetLeftHold());
			break;
		case 12:
				rValue.SetBoolean(GetRelease());
			break;
		case 13:
				rValue.SetBoolean(GetRightClick());
			break;
		case 14:
				rValue.SetBoolean(GetScrollBool());
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
			SetScrollValue(rValue.GetInteger());
			break;
		case 2:
			SetXLeftClick(rValue.GetInteger());
			break;
		case 3:
			SetXPosition(rValue.GetInteger());
			break;
		case 4:
			SetXRightClick(rValue.GetInteger());
			break;
		case 5:
			SetY2dPosition(rValue.GetInteger());
			break;
		case 6:
			SetY2dRightClick(rValue.GetInteger());
			break;
		case 7:
			SetYLeftClick(rValue.GetInteger());
			break;
		case 8:
			SetYPosition(rValue.GetInteger());
			break;
		case 9:
			SetYRightClick(rValue.GetInteger());
			break;
		case 10:
			SetLeftClick(rValue.GetBoolean());
			break;
		case 11:
			SetLeftHold(rValue.GetBoolean());
			break;
		case 12:
			SetRelease(rValue.GetBoolean());
			break;
		case 13:
			SetRightClick(rValue.GetBoolean());
			break;
		case 14:
			SetScrollBool(rValue.GetBoolean());
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
LtXmlLib18::CBigInteger CMouse::GetScrollValue() const
{
	return this->m_ScrollValue;
}
void CMouse::SetScrollValue(LtXmlLib18::CBigInteger value)
{ 
	this->m_ScrollValue = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CMouse::GetXLeftClick() const
{
	return this->m_XLeftClick;
}
void CMouse::SetXLeftClick(LtXmlLib18::CBigInteger value)
{ 
	this->m_XLeftClick = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CMouse::GetXPosition() const
{
	return this->m_XPosition;
}
void CMouse::SetXPosition(LtXmlLib18::CBigInteger value)
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
LtXmlLib18::CBigInteger CMouse::GetXRightClick() const
{
	return this->m_XRightClick;
}
void CMouse::SetXRightClick(LtXmlLib18::CBigInteger value)
{ 
	this->m_XRightClick = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CMouse::GetY2dPosition() const
{
	return this->m_Y2dPosition;
}
void CMouse::SetY2dPosition(LtXmlLib18::CBigInteger value)
{ 
	this->m_Y2dPosition = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CMouse::GetY2dRightClick() const
{
	return this->m_Y2dRightClick;
}
void CMouse::SetY2dRightClick(LtXmlLib18::CBigInteger value)
{ 
	this->m_Y2dRightClick = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CMouse::GetYLeftClick() const
{
	return this->m_YLeftClick;
}
void CMouse::SetYLeftClick(LtXmlLib18::CBigInteger value)
{ 
	this->m_YLeftClick = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CMouse::GetYPosition() const
{
	return this->m_YPosition;
}
void CMouse::SetYPosition(LtXmlLib18::CBigInteger value)
{ 
	this->m_YPosition = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to 0.
 */
LtXmlLib18::CBigInteger CMouse::GetYRightClick() const
{
	return this->m_YRightClick;
}
void CMouse::SetYRightClick(LtXmlLib18::CBigInteger value)
{ 
	this->m_YRightClick = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to false.
 */
bool CMouse::GetLeftClick() const
{
	return this->m_LeftClick;
}
void CMouse::SetLeftClick(bool value)
{ 
	this->m_LeftClick = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to false.
 */
bool CMouse::GetLeftHold() const
{
	return this->m_LeftHold;
}
void CMouse::SetLeftHold(bool value)
{ 
	this->m_LeftHold = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to false.
 */
bool CMouse::GetRelease() const
{
	return this->m_Release;
}
void CMouse::SetRelease(bool value)
{ 
	this->m_Release = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to false.
 */
bool CMouse::GetRightClick() const
{
	return this->m_RightClick;
}
void CMouse::SetRightClick(bool value)
{ 
	this->m_RightClick = value; 
}

/*
 * Represents a mandatory Element in the XML document
 *
 * 
 * This property is represented as an Element in the XML.
 * It is mandatory and therefore must be populated within the XML.
 * It is defaulted to false.
 */
bool CMouse::GetScrollBool() const
{
	return this->m_ScrollBool;
}
void CMouse::SetScrollBool(bool value)
{ 
	this->m_ScrollBool = value; 
}

/*
 * Allows the class to be copied
 * Performs a 'deep copy' of all the data in the class (and its children)
 */
ParamsLib::CMousePtr CMouse::Clone() const
{
	ParamsLib::CMousePtr newObject = CreateInstance(m_elementName.c_str());

	int index = 0;
	newObject->m_ScrollValue = m_ScrollValue;
	newObject->m_XLeftClick = m_XLeftClick;
	newObject->m_XPosition = m_XPosition;
	newObject->m_XRightClick = m_XRightClick;
	newObject->m_Y2dPosition = m_Y2dPosition;
	newObject->m_Y2dRightClick = m_Y2dRightClick;
	newObject->m_YLeftClick = m_YLeftClick;
	newObject->m_YPosition = m_YPosition;
	newObject->m_YRightClick = m_YRightClick;
	newObject->m_LeftClick = m_LeftClick;
	newObject->m_LeftHold = m_LeftHold;
	newObject->m_Release = m_Release;
	newObject->m_RightClick = m_RightClick;
	newObject->m_ScrollBool = m_ScrollBool;


// ##HAND_CODED_BLOCK_START ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional clone code here...

// ##HAND_CODED_BLOCK_END ID="Additional clone"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	return newObject.Ptr();
}

std::tstring CMouse::GetTargetNamespace() const
{
	return _T("");
}	

std::tstring CMouse::GetNamespace() const
{
	return _T("");
}	

LtXmlLib18::CXmlObjectBase* CMouse::GetBase()
{
	return this;
}	

void CMouse::CleanMetaData()
{
	LtXmlLib18::CXmlGeneratedClass::CleanMetaData(ms_pParentElementInfo, ms_ppElementInfo, ms_ppAttributeInfo);
}

LtXmlLib18Data::CParentElementInfo* CMouse::GetClassInfo() const
{
	if (ms_pParentElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_pParentElementInfo == NULL)
		{
			ms_pParentElementInfo = new LtXmlLib18Data::CParentElementInfo(
																			LtXmlLib18Data::XmlElementGroupType_SEQUENCE,
																			LtXmlLib18Data::XmlElementType_ELEMENT, 
																			_T("mouse"),					 
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

LtXmlLib18Data::CElementInfo** CMouse::GetClassElementInfo() const
{
	if (ms_ppElementInfo == NULL)
	{
		m_csInit.Enter();
		if (ms_ppElementInfo == NULL)
		{
			ms_ppElementInfo = new LtXmlLib18Data::CElementInfo*[15];
		
			ms_ppElementInfo[0] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("scrollValue"), _T(""), 1, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[1] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("xLeftClick"), _T(""), 2, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[2] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("xPosition"), _T(""), 3, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[3] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("xRightClick"), _T(""), 4, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[4] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("y2dPosition"), _T(""), 5, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[5] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("y2dRightClick"), _T(""), 6, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[6] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("yLeftClick"), _T(""), 7, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[7] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("yPosition"), _T(""), 8, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[8] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("yRightClick"), _T(""), 9, false, LtXmlLib18::ItemType_integer, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[9] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("leftClick"), _T(""), 10, false, LtXmlLib18::ItemType_boolean, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[10] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("leftHold"), _T(""), 11, false, LtXmlLib18::ItemType_boolean, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[11] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("release"), _T(""), 12, false, LtXmlLib18::ItemType_boolean, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[12] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("rightClick"), _T(""), 13, false, LtXmlLib18::ItemType_boolean, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[13] = new LtXmlLib18Data::CElementInfoSeqPrimMnd(_T("scrollBool"), _T(""), 14, false, LtXmlLib18::ItemType_boolean, NULL, LtXmlLib18::CWhitespaceUtils::WhitespaceRule_Collapse, LtXmlLib18::CPrimitiveRestrictions(_T(""), -1, -1, _T(""), _T(""), _T(""), _T(""), -1, -1, -1), NULL);
			ms_ppElementInfo[14] = NULL;
		}
		m_csInit.Leave();
	}
	return ms_ppElementInfo;
}

LtXmlLib18Data::CAttributeInfo**	CMouse::GetClassAttributeInfo() const
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





