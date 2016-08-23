// OutlookImpExp.h : main header file for the OUTLOOKIMPEXP DLL
//

#if !defined(AFX_OUTLOOKIMPEXP_H__EA78C907_C901_47ED_B683_8C42A0EA555F__INCLUDED_)
#define AFX_OUTLOOKIMPEXP_H__EA78C907_C901_47ED_B683_8C42A0EA555F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR OUTLOOK_METADATAKEY = _T("MSO_956F3091_A2DD_4E1D_B5F7_F58BAD300F0F");

/////////////////////////////////////////////////////////////////////////////
// COutlookImpExpApp
// See OutlookImpExp.cpp for the implementation of this class
//

class COutlookImpExpApp : public CWinApp
{
public:
	COutlookImpExpApp();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTLOOKIMPEXP_H__EA78C907_C901_47ED_B683_8C42A0EA555F__INCLUDED_)
