// IEPrintPreview.h : main header file for the IEPRINTPREVIEW application
//

#if !defined(AFX_IEPRINTPREVIEW_H__3B2D121E_9E7C_4E59_943F_BE73845C7EE1__INCLUDED_)
#define AFX_IEPRINTPREVIEW_H__3B2D121E_9E7C_4E59_943F_BE73845C7EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIEPrintPreviewApp:
// See IEPrintPreview.cpp for the implementation of this class
//

class CIEPrintPreviewApp : public CWinApp
{
public:
	CIEPrintPreviewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEPrintPreviewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIEPrintPreviewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEPRINTPREVIEW_H__3B2D121E_9E7C_4E59_943F_BE73845C7EE1__INCLUDED_)
