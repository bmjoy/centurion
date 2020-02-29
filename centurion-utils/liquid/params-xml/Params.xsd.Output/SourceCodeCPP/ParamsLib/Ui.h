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
#ifndef _ParamsLib_ParamsLib_CUi_h
#define _ParamsLib_ParamsLib_CUi_h

// Include Base classes

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Forward declarations - done like this to keep the intellisense happy
namespace ParamsLib	{	class CClassFactory; };

namespace ParamsLib
{
	/*
	 * CUi
	 *  
	 * This class wraps the element ui in the schema
	 */
	class ParamsLib_DLL CUi : public CInstanceMonitor
					, public virtual ParamsLib::CXmlCommonBase
// ##HAND_CODED_BLOCK_START ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
// Add Additional base classes here...
// ##HAND_CODED_BLOCK_END ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
	{
	public:
		static ParamsLib::CUiPtr CreateInstance(LPCTSTR lpctElementName=_T("ui"));

	protected:
		CUi(LPCTSTR lpctElementName=_T("ui"));
		virtual ~CUi();
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
		LtXmlLib18::CBigInteger GetBottomBarHeight() const;
		void SetBottomBarHeight(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_BottomBarHeight;

	public:
		LtXmlLib18::CBigInteger GetTopBarHeight() const;
		void SetTopBarHeight(LtXmlLib18::CBigInteger val);
	protected:
 	    LtXmlLib18::CBigInteger m_TopBarHeight;



	public:
		// Performs a 'deep copy' of all the data in the class (and its children)
		virtual ParamsLib::CUiPtr Clone() const;

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


#endif // _ParamsLib_CUi_h

