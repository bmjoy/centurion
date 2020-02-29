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
#ifndef _ParamsLib_ParamsLib_CMouse_h
#define _ParamsLib_ParamsLib_CMouse_h

// Include Base classes

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Forward declarations - done like this to keep the intellisense happy
namespace ParamsLib	{	class CClassFactory; };

namespace ParamsLib
{
	/*
	 * CMouse
	 *  
	 * This class wraps the element mouse in the schema
	 */
	class ParamsLib_DLL CMouse : public CInstanceMonitor
					, public virtual ParamsLib::CXmlCommonBase
// ##HAND_CODED_BLOCK_START ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
// Add Additional base classes here...
// ##HAND_CODED_BLOCK_END ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
	{
	public:
		static ParamsLib::CMousePtr CreateInstance(LPCTSTR lpctElementName=_T("mouse"));

	protected:
		CMouse(LPCTSTR lpctElementName=_T("mouse"));
		virtual ~CMouse();
		friend class ParamsLib::CClassFactory;
	
		virtual void Init();
		virtual void AccessProperty(int iPropertyIndex, bool read, LtXmlLib18::LtVariant& rValue);
		virtual LtXmlLib18Data::CParentElementInfo* GetClassInfo() const;
		virtual LtXmlLib18Data::CElementInfo** GetClassElementInfo() const;
		virtual LtXmlLib18Data::CAttributeInfo** GetClassAttributeInfo() const;
		
		static	void CleanMetaData();
				void Cleanup();
		virtual void OnEvent(LtXmlLib18::CXmlObjectBase* pMsgSource, LtXmlLib18::IEventSink::MsgType eMsgType, void* pData);

	public:
		LtXmlLib18::CBigInteger GetScrollValue() const;
		void SetScrollValue(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_ScrollValue;

	public:
		LtXmlLib18::CBigInteger GetXLeftClick() const;
		void SetXLeftClick(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_XLeftClick;

	public:
		LtXmlLib18::CBigInteger GetXPosition() const;
		void SetXPosition(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_XPosition;

	public:
		LtXmlLib18::CBigInteger GetXRightClick() const;
		void SetXRightClick(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_XRightClick;

	public:
		LtXmlLib18::CBigInteger GetY2dPosition() const;
		void SetY2dPosition(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_Y2dPosition;

	public:
		LtXmlLib18::CBigInteger GetY2dRightClick() const;
		void SetY2dRightClick(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_Y2dRightClick;

	public:
		LtXmlLib18::CBigInteger GetYLeftClick() const;
		void SetYLeftClick(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_YLeftClick;

	public:
		LtXmlLib18::CBigInteger GetYPosition() const;
		void SetYPosition(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_YPosition;

	public:
		LtXmlLib18::CBigInteger GetYRightClick() const;
		void SetYRightClick(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_YRightClick;

	public:
		bool GetLeftClick() const;
		void SetLeftClick(bool val);
	protected:
 	    bool m_LeftClick;

	public:
		bool GetLeftHold() const;
		void SetLeftHold(bool val);
	protected:
 	    bool m_LeftHold;

	public:
		bool GetRelease() const;
		void SetRelease(bool val);
	protected:
 	    bool m_Release;

	public:
		bool GetRightClick() const;
		void SetRightClick(bool val);
	protected:
 	    bool m_RightClick;

	public:
		bool GetScrollBool() const;
		void SetScrollBool(bool val);
	protected:
 	    bool m_ScrollBool;



	public:
		// Performs a 'deep copy' of all the data in the class (and its children)
		virtual ParamsLib::CMousePtr Clone() const;

		virtual std::tstring GetTargetNamespace() const;
		virtual std::tstring GetNamespace() const;
		virtual LtXmlLib18::CXmlObjectBase* GetBase();

	// Internal data for XML serialization
	private:
		static LtXmlLib18Data::CParentElementInfo* ms_pParentElementInfo;
		static LtXmlLib18Data::CElementInfo** ms_ppElementInfo;
		static LtXmlLib18Data::CAttributeInfo** ms_ppAttributeInfo;

// ##HAND_CODED_BLOCK_START ID="Additional Methods"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Methods here...

// ##HAND_CODED_BLOCK_END ID="Additional Methods"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
	};
}; // end namespace (ParamsLib)


#endif // _ParamsLib_CMouse_h

