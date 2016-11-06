// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__601FD668_0A5C_458E_8E08_CAB7FB31643D__INCLUDED_)
#define AFX_STDAFX_H__601FD668_0A5C_458E_8E08_CAB7FB31643D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER
#define WINVER 0x0600
#endif	// WINVER

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef _WIN32_IE 
#define _WIN32_IE 0x0600
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components

#include <afxext.h>         // MFC extensions
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#pragma warning(disable:4706)
#include <multimon.h>

#pragma warning(disable:4239)
#pragma warning(disable:4505)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__601FD668_0A5C_458E_8E08_CAB7FB31643D__INCLUDED_)
