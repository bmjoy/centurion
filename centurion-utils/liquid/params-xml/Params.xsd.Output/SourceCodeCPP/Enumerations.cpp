
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
#include "ParamsLib.h"

// ##HAND_CODED_BLOCK_START ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Add Additional Includes here...

// ##HAND_CODED_BLOCK_END ID="Additional Includes"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

// Forward declarations - done like this to keep the intellisense happy
namespace ParamsLib	{	class CClassFactory; };

namespace ParamsLib 
{
	// This class is constructed when the dll is loaded,
	// and destroyed when the dll is unloaded.
	class CAppLifetime
	{
	public:
		CAppLifetime()
		{
			RegisterLibrary();
		}
		~CAppLifetime()
		{
			CleanupMetaData();
		}
		
		// remove all cached meta data
		// this method is NOT thread safe.
		static void CleanupMetaData()
		{
			ParamsLib::CClassFactory::CleanMetaData();
		}


		// Register the redist when the library is loaded 
		// Stops the nag screen popping up!
		static void RegisterLibrary()
		{
			LtXmlLib18::Register(18, 0, "Trial 15/03/2020", "Params.xsd", "TT8LWWNN1MC266JF000000AA");

			// ##HAND_CODED_BLOCK_START ID="Namespace Declarations"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
			// Add Additional namespace declarations here...
			LtXmlLib18::CSerializationContext::GetDefaultContext().SetSchemaType(LtXmlLib18::SchemaType_XSD);
			// LtXmlLib18::CSerializationContext::GetDefaultContext().SetDefaultNamespaceURI(_T("http://www.fpml.org/2003/FpML-4-0"));
			// LtXmlLib18::CSerializationContext::GetDefaultContext().GetNamespaceAliases().Add(_T("dsig"), _T("http://www.w3.org/2000/09/xmldsig#"));

			LtXmlLib18::CSerializationContext::GetDefaultContext().GetNamespaceAliases().Add(_T("xs"), _T("http://www.w3.org/2001/XMLSchema-instance"));

			// ##HAND_CODED_BLOCK_END ID="Namespace Declarations"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS
		}
	};
	static ParamsLib::CAppLifetime s_oDuumy;
};
	
namespace ParamsLib 
{
	// ##HAND_CODED_BLOCK_START ID="ParamsLib::CEnumerations Additional Methods"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	// Add Additional Methods here...

	// ##HAND_CODED_BLOCK_END ID="ParamsLib::CEnumerations Additional Methods"## DO NOT MODIFY ANYTHING OUTSIDE OF THESE TAGS

	
};






