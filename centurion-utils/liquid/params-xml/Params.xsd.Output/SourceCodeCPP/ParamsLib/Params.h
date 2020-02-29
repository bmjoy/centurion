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
#ifndef _ParamsLib_ParamsLib_CParams_h
#define _ParamsLib_ParamsLib_CParams_h

// Include Base classes
#include "../ParamsLib/Camera.h"
#include "../ParamsLib/Mouse.h"
#include "../ParamsLib/Window.h"
#include "../ParamsLib/Ui.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Forward declarations - done like this to keep the intellisense happy
namespace ParamsLib	{	class CClassFactory; };

namespace ParamsLib
{
	/*
	 * CParams
	 *  
	 * This class wraps the element params in the schema
	 */
	class ParamsLib_DLL CParams : public CInstanceMonitor
					, public virtual ParamsLib::CXmlCommonBase
// ##HAND_CODED_BLOCK_START ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
// Add Additional base classes here...
// ##HAND_CODED_BLOCK_END ID="Additional Base Classes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
	{
	public:
		static ParamsLib::CParamsPtr CreateInstance(LPCTSTR lpctElementName=_T("params"));

	protected:
		CParams(LPCTSTR lpctElementName=_T("params"));
		virtual ~CParams();
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
		ParamsLib::CCameraPtr GetCamera() const;
		void SetCamera(ParamsLib::CCamera* value);
	protected:
		ParamsLib::CCameraPtr m_Camera;

	public:
		ParamsLib::CMousePtr GetMouse() const;
		void SetMouse(ParamsLib::CMouse* value);
	protected:
		ParamsLib::CMousePtr m_Mouse;

	public:
		ParamsLib::CWindowPtr GetWindow() const;
		void SetWindow(ParamsLib::CWindow* value);
	protected:
		ParamsLib::CWindowPtr m_Window;

	public:
		ParamsLib::CUiPtr GetUi() const;
		void SetUi(ParamsLib::CUi* value);
	protected:
		ParamsLib::CUiPtr m_Ui;



	public:
		// Performs a 'deep copy' of all the data in the class (and its children)
		virtual ParamsLib::CParamsPtr Clone() const;

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


#endif // _ParamsLib_CParams_h

