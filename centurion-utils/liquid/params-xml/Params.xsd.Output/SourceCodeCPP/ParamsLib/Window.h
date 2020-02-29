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
#ifndef _ParamsLib_ParamsLib_CWindow_h
#define _ParamsLib_ParamsLib_CWindow_h

// Include Base classes

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Forward declarations - done like this to keep the intellisense happy
namespace ParamsLib	{	class CClassFactory; };

namespace ParamsLib
{
	/*
	 * CWindow
	 *  
	 * This class wraps the element window in the schema
	 */
	class ParamsLib_DLL CWindow : public CInstanceMonitor
					, public virtual ParamsLib::CXmlCommonBase
// ##HAND_CODED_BLOCK_START ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
// Add Additional base classes here...
// ##HAND_CODED_BLOCK_END ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
	{
	public:
		static ParamsLib::CWindowPtr CreateInstance(LPCTSTR lpctElementName=_T("window"));

	protected:
		CWindow(LPCTSTR lpctElementName=_T("window"));
		virtual ~CWindow();
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
		LtXmlLib18::CDecimal GetHeightZoomed() const;
		void SetHeightZoomed(LtXmlLib18::CDecimal val);
	protected:
 	    LtXmlLib18::CDecimal m_HeightZoomed;

	public:
		LtXmlLib18::CDecimal GetRatio() const;
		void SetRatio(LtXmlLib18::CDecimal val);
	protected:
 	    LtXmlLib18::CDecimal m_Ratio;

	public:
		LtXmlLib18::CDecimal GetWidthZoomed() const;
		void SetWidthZoomed(LtXmlLib18::CDecimal val);
	protected:
 	    LtXmlLib18::CDecimal m_WidthZoomed;

	public:
		bool GetShouldClose() const;
		void SetShouldClose(bool val);
	protected:
 	    bool m_ShouldClose;



	public:
		// Performs a 'deep copy' of all the data in the class (and its children)
		virtual ParamsLib::CWindowPtr Clone() const;

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


#endif // _ParamsLib_CWindow_h

