// TDLUninstall.h : main header file for the TDLUNINSTALL application
//

#if !defined(AFX_TDLUNINSTALL_H__8F865BDB_C093_4331_A1C8_9D58F56BF446__INCLUDED_)
#define AFX_TDLUNINSTALL_H__8F865BDB_C093_4331_A1C8_9D58F56BF446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTDLUninstallApp:
// See TDLUninstall.cpp for the implementation of this class
//

class CTDLUninstallApp : public CWinApp
{
public:
	CTDLUninstallApp();

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLUninstallApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	static void DoUnistall(const CString& sAppFolder);
	static int DeleteFilesAndFolders(const CString& sAppFolder);

// Implementation

	//{{AFX_MSG(CTDLUninstallApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLUNINSTALL_H__8F865BDB_C093_4331_A1C8_9D58F56BF446__INCLUDED_)
