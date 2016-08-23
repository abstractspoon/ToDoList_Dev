// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_MFC_DEF_H)
#define __EXT_MFC_DEF_H

#define __EXTMFC_ALL_FORM_MOVABLE_WND_STYLES (0x0000FFFFL)

// disable warning 4201
#pragma warning( push )
#pragma warning ( disable : 4201 )
	#include <mmsystem.h>
// rollback warning 4201
#pragma warning( pop )

// __PROF_UIS_VERSION_DWORD is required for static library builds
#define __PROF_UIS_VERSION_DWORD DWORD( 0x02020301 )

// the multiply monitor stub can be included into the library
// builds (used in ExtPaintManager.cpp to define
// COMPILE_MULTIMON_STUBS symbol before include multimon.h)
#if (defined _AFXDLL)
	#define __PROF_UIS_BUILTIN_MULTIMON_STUB
#endif // _AFXDLL
#pragma message("   Prof-UIS multiply monitor support:")
#if (defined __PROF_UIS_BUILTIN_MULTIMON_STUB)
	#pragma message("      built-in")
#else
	#pragma message("      external")
#endif // __PROF_UIS_BUILTIN_MULTIMON_STUB

// Prof-UIS basic library
#if (defined _AFXDLL)
	#if (defined __STATPROFUIS_WITH_DLLMFC__)
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223yud.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223ymd.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223ynd.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223yu.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223ym.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223yn.lib"
			#endif // _UNICODE
		#endif // _DEBUG
	#else
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS223ud.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223ud.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS223md.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223md.lib"
			#else
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS223nd.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223nd.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS223u.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223u.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS223m.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223m.lib"
			#else
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS223n.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223n.lib"
			#endif // _UNICODE
		#endif // _DEBUG
	#endif // __STATPROFUIS_WITH_DLLMFC__
#else
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223sud.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223smd.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223snd.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223su.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223sm.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS223sn.lib"
			#endif // _UNICODE
		#endif // _DEBUG
#endif // _AFXDLL

#if (defined __PROF_UIS_IMPL__)
	#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		#define __PROF_UIS_API _declspec(dllexport)
	#else
		#define __PROF_UIS_API
	#endif // _AFXDLL
#else 
	#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		#define __PROF_UIS_API _declspec(dllimport)
	#else
		#define __PROF_UIS_API
	#endif // _AFXDLL

	#pragma message("   Automatically linking with ProfUIS223 library")
	#pragma message("      (Professional User Interface Suite)")
	#pragma comment( lib, __PROF_UIS_LIB_NAME ) 
#endif

#if (defined __PROF_UIS_ADV_IMPL__)
	#if (!defined __ENABLED_PROF_UIS_ADV__)
		#define __ENABLED_PROF_UIS_ADV__
	#endif
#endif // #if (defined __PROF_UIS_ADV_IMPL__)

#if (defined __ENABLED_PROF_UIS_ADV__)

	// __PROF_UIS_VERSION_DWORD is required for static library builds
	#define __PROF_UIS_ADV_VERSION_DWORD DWORD( 0x02010501 )

	// Prof-UIS advanced library
	// (now for internal use only)

	#if (defined _AFXDLL)
		#if (defined __STATPROFUIS_WITH_DLLMFC__)
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223yud.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223ymd.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223ynd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223yu.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223ym.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223yn.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#else
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv223ud.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223ud.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv223md.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223md.lib"
				#else
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv223nd.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223nd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv223u.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223u.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv223m.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223m.lib"
				#else
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv223n.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223n.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#endif // __STATPROFUIS_WITH_DLLMFC__
	#else
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223sud.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223smd.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223snd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223su.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223sm.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv223sn.lib"
				#endif // _UNICODE
			#endif // _DEBUG
	#endif // _AFXDLL

	#if defined __PROF_UIS_ADV_IMPL__
		#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			#define __PROF_UIS_ADV_API _declspec(dllexport)
		#else
			#define __PROF_UIS_ADV_API
		#endif
	#else 
		#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			#define __PROF_UIS_ADV_API _declspec(dllimport)
		#else
			#define __PROF_UIS_ADV_API
		#endif

		#pragma message("   Automatically linking with ProfUISAdv223 library")
		#pragma message("      (Professional User Interface Suite / Advanced Stuff)")
		#pragma comment( lib, __PROF_UIS_ADV_LIB_NAME ) 
	#endif
#endif // __ENABLED_PROF_UIS_ADV__

// Prof-UIS registry names
#define __PROF_UIS_REG_SECTION			_T("ProfUIS223")
#define __PROF_UIS_REG_PROFILES			_T("Profiles")
#define __PROF_UIS_REG_COMMAND_MANAGER	_T("CommandManager")
#define __PROF_UIS_REG_CONTROL_BAR		_T("ControlBar")
#define __PROF_UIS_REG_CUSTOMIZATION	_T("Customization")

// Prof-UIS native unicode character type support for VC++ .NET 2002 or later
#if (!defined __ExtMfcSafeNativeTCHAR_H)
	#include <ExtMfcSafeNativeTCHAR.h>
#endif

#endif // __EXT_MFC_DEF_H
