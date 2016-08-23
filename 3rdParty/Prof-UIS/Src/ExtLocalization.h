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

#if (!defined __EXT_LOCALIZATION_H)
#define __EXT_LOCALIZATION_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

class __PROF_UIS_API CExtLocalResourceHelper
{
	HINSTANCE m_hInstPrevResource;
	bool m_bFree:1;
	
	void Init(
		HINSTANCE hInstResourceModule
		)
	{
		ASSERT( m_bFree );
		ASSERT( hInstResourceModule != NULL );
		m_hInstPrevResource =
			::AfxGetResourceHandle();
		::AfxSetResourceHandle(
			hInstResourceModule
			);
		m_bFree = false;
	};

	void Init(
		__EXT_MFC_SAFE_LPCTSTR sResourceModulePath
		)
	{
		HINSTANCE hInstResourceModule =
			::GetModuleHandle(
				sResourceModulePath
				);
		Init( hInstResourceModule );
	};

public:
	
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
	CExtLocalResourceHelper()
		: m_hInstPrevResource( NULL )
		, m_bFree( true )
	{
		Init( __PROF_UIS_MODULE_NAME );
	};
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

	CExtLocalResourceHelper(
		HINSTANCE hInstResourceModule
		)
		: m_hInstPrevResource( NULL )
		, m_bFree( true )
	{
		Init( hInstResourceModule );
	};
	
	CExtLocalResourceHelper(
		__EXT_MFC_SAFE_LPCTSTR sResourceModulePath
		)
		: m_hInstPrevResource( NULL )
		, m_bFree( true )
	{
		Init( sResourceModulePath );
	};

	~CExtLocalResourceHelper()
	{
		Free();
	};
	
	void Free()
	{
		if( m_bFree )
			return;
		::AfxSetResourceHandle(
			m_hInstPrevResource
			);
		m_hInstPrevResource = NULL;
		m_bFree = true;
	};

}; // class CExtLocalResourceHelper

#endif // __EXT_LOCALIZATION_H
