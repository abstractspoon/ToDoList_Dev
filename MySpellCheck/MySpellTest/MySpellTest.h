// MySpellTest.h : main header file for the MYSPELLTEST application
//

#if !defined(AFX_MYSPELLTEST_H__AC5C355A_DF6A_4A24_AF47_A98C043A4A83__INCLUDED_)
#define AFX_MYSPELLTEST_H__AC5C355A_DF6A_4A24_AF47_A98C043A4A83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMySpellTestApp:
// See MySpellTest.cpp for the implementation of this class
//

class CMySpellTestApp : public CWinApp
{
public:
	CMySpellTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySpellTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMySpellTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPELLTEST_H__AC5C355A_DF6A_4A24_AF47_A98C043A4A83__INCLUDED_)
