// GPExport.h : main header file for the GPEXPORT DLL
//

#if !defined(AFX_GPEXPORT_H__DF3308EC_6319_429C_B017_039EE9E5AE4F__INCLUDED_)
#define AFX_GPEXPORT_H__DF3308EC_6319_429C_B017_039EE9E5AE4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// Some common defines
const LPCTSTR GP_MENUTEXT	= _T("GanttProject"); 
const LPCTSTR GP_FILEFILTER = _T("GanttProject Files (*.gan)|*.gan||"); 
const LPCTSTR GP_FILEEXT	= _T("gan"); 

const LPCTSTR GANTTVIEW_ID	= _T("83F6CBD3-5F3C-4567-9BA8-9544B899F949");

/////////////////////////////////////////////////////////////////////////////
// CGPExportApp
// See GPExport.cpp for the implementation of this class
//

class CGPExportApp : public CWinApp
{
public:
	CGPExportApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPExportApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGPExportApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPEXPORT_H__DF3308EC_6319_429C_B017_039EE9E5AE4F__INCLUDED_)
