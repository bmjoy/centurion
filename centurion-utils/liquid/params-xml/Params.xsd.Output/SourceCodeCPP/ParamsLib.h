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
#ifndef __ParamsLib_h
#define __ParamsLib_h

#ifdef _WIN32
#include <tchar.h>
#endif

#include <string>
#include <vector>

#ifdef _WIN32
	
	// If the lib file is not found then you will need to set the
	// library path to point to Redist18/cpp/win32/lib or Redist18/cpp/win64/lib
	// in the project settings.
	// The associated .dll will need to be on the system path at runtime.
	
	#ifdef _WIN64
		#ifdef _DEBUG
			#ifdef _UNICODE
				// load appropriate unicode debug library
				#if _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18UD_vc80x64.lib")
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18UD_vc90x64.lib")
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18UD_vc100x64.lib")
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18UD_vc110x64.lib")
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18UD_vc120x64.lib")
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18UD_vc140x64.lib")
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18UD_vc141x64.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18UD_vc142x64.lib")
				#endif
			#else
				// load appropriate debug library
				#if _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18D_vc80x64.lib") 
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18D_vc90x64.lib") 
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18D_vc100x64.lib") 
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18D_vc110x64.lib") 
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18D_vc120x64.lib") 
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18D_vc140x64.lib") 
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18D_vc141x64.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18D_vc142x64.lib")
				#endif
			#endif
		#else
			#ifdef _UNICODE
				// load appropriate unicode release library
				#if _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18U_vc80x64.lib") 
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18U_vc90x64.lib") 
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18U_vc100x64.lib") 
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18U_vc110x64.lib") 
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18U_vc120x64.lib") 
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18U_vc140x64.lib") 
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18U_vc141x64.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18U_vc142x64.lib")
				#endif
			#else
				// load appropriate debug release library
				#if _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18_vc80x64.lib") 
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18_vc90x64.lib") 
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18_vc100x64.lib") 
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18_vc110x64.lib") 
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18_vc120x64.lib") 
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18_vc140x64.lib") 
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18_vc141x64.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18_vc142x64.lib")
				#endif
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#ifdef _UNICODE
				// load appropriate unicode debug library
				#if _MSC_VER == 1200 // 6.0
					#pragma comment(lib, "LtXmlLib18UD_vc60.lib")
				#elif _MSC_VER == 1310 // 7.1
					#pragma comment(lib, "LtXmlLib18UD_vc71.lib")
				#elif _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18UD_vc80.lib")
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18UD_vc90.lib")
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18UD_vc100.lib")
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18UD_vc110.lib")
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18UD_vc120.lib")
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18UD_vc140.lib")
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18UD_vc141.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18UD_vc142.lib")
				#endif
			#else
				// load appropriate debug library
				#if _MSC_VER == 1200 // 6.0
					#pragma comment(lib, "LtXmlLib18D_vc60.lib")
				#elif _MSC_VER == 1310 // 7.1
					#pragma comment(lib, "LtXmlLib18D_vc71.lib")
				#elif _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18D_vc80.lib")
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18D_vc90.lib")
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18D_vc100.lib")
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18D_vc110.lib")
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18D_vc120.lib")
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18D_vc140.lib")
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18D_vc141.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18D_vc142.lib")
				#endif
			#endif
		#else
			#ifdef _UNICODE
				// load appropriate unicode release library
				#if _MSC_VER == 1200 // 6.0
					#pragma comment(lib, "LtXmlLib18U_vc60.lib")
				#elif _MSC_VER == 1310 // 7.1
					#pragma comment(lib, "LtXmlLib18U_vc71.lib")
				#elif _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18U_vc80.lib")
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18U_vc90.lib")
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18U_vc100.lib")
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18U_vc110.lib")
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18U_vc120.lib")
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18U_vc140.lib")
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18U_vc141.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18U_vc142.lib")
				#endif
			#else
				// load appropriate debug release library
				#if _MSC_VER == 1200 // 6.0
					#pragma comment(lib, "LtXmlLib18_vc60.lib")
				#elif _MSC_VER == 1310 // 7.1
					#pragma comment(lib, "LtXmlLib18_vc71.lib")
				#elif _MSC_VER == 1400 // 8.0
					#pragma comment(lib, "LtXmlLib18_vc80.lib")
				#elif _MSC_VER == 1500 // 9.0
					#pragma comment(lib, "LtXmlLib18_vc90.lib")
				#elif _MSC_VER == 1600 // 10.0
					#pragma comment(lib, "LtXmlLib18_vc100.lib")
				#elif _MSC_VER == 1700 // 11.0
					#pragma comment(lib, "LtXmlLib18_vc110.lib")
				#elif _MSC_VER == 1800 // 12.0
					#pragma comment(lib, "LtXmlLib18_vc120.lib")
				#elif _MSC_VER == 1900 // 14.0
					#pragma comment(lib, "LtXmlLib18_vc140.lib")
				#elif _MSC_VER >= 1910 && _MSC_VER < 1920 // 14.1
					#pragma comment(lib, "LtXmlLib18_vc141.lib")
				#elif _MSC_VER >= 1920 // 14.2
					#pragma comment(lib, "LtXmlLib18_vc142.lib")
				#endif
			#endif
		#endif
	#endif

	#pragma warning(disable:4786) // long debug names
	#pragma warning(disable:4251) // exporting from dll
	#pragma warning(disable:4250) // derivation warning

	#ifdef ParamsLib_STATIC
		// static library (.lib)
		#define ParamsLib_DLL
	#else
		// dynamic library (.dll)
		#ifndef ParamsLib_EXPORTS
			#define ParamsLib_DLL __declspec(dllimport)
		#else
			#define ParamsLib_DLL __declspec(dllexport)
		#endif
	#endif
#endif

#ifdef _UNIX_
	#define ParamsLib_DLL
#endif


// If this is not found then you will need to set the include path 
// to point to Redist2020/cpp/include in the project settings.
#include "LtXmlLib18.h"

namespace ParamsLib
{
	class CCamera;
	typedef CSmartPtr<CCamera> CCameraPtr;
	class CMouse;
	typedef CSmartPtr<CMouse> CMousePtr;
	class CParams;
	typedef CSmartPtr<CParams> CParamsPtr;
	class CUi;
	typedef CSmartPtr<CUi> CUiPtr;
	class CWindow;
	typedef CSmartPtr<CWindow> CWindowPtr;
};

namespace ParamsLib
{
	class CClassFactory;
	class CXmlCollectionCommonBase;
	class CXmlCommonBase;
};

#include "Enumerations.h"
#include "ClassFactory.h"
#include "XmlCollectionCommonBase.h"
#include "XmlCommonBase.h"

#endif // __ParamsLib_h

