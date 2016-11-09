// TDLTransEdit.h : main header file for the TDLTRANSEDIT application
//

#if !defined(AFX_TDLTRANSEDIT_H__E915F0C0_D396_4CCA_AD3C_BFD1B8B10A4D__INCLUDED_)
#define AFX_TDLTRANSEDIT_H__E915F0C0_D396_4CCA_AD3C_BFD1B8B10A4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditApp:
// See TDLTransEdit.cpp for the implementation of this class
//

class CTDLTransEditApp : public CWinApp
{
public:
	CTDLTransEditApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTransEditApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTDLTransEditApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTRANSEDIT_H__E915F0C0_D396_4CCA_AD3C_BFD1B8B10A4D__INCLUDED_)
