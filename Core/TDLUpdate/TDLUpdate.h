// TDLUpdate.h : main header file for the TDLUPDATE application
//

#if !defined(AFX_TDLUPDATE_H__A2C5AF0D_3BB6_4B4A_992A_BD9A2B0929CC__INCLUDED_)
#define AFX_TDLUPDATE_H__A2C5AF0D_3BB6_4B4A_992A_BD9A2B0929CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTDLUpdateApp:
// See TDLUpdate.cpp for the implementation of this class
//

class CTDLUpdateApp : public CWinApp
{
public:
	CTDLUpdateApp();

protected:
	virtual BOOL InitInstance();
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);

protected:
	static void DoUpdate(const CString& sAppFolder, const CString& sPrevCmdLine, const CPoint& ptPos,
						 BOOL bPreRelease, BOOL bRestartElevated);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLUPDATE_H__A2C5AF0D_3BB6_4B4A_992A_BD9A2B0929CC__INCLUDED_)
