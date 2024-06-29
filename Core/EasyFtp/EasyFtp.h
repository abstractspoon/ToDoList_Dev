// EasyFtp.h : main header file for the EASYFTP application
//

#if !defined(AFX_EASYFTP_H__51EEF4A4_7B5E_4FB8_AC27_96DF75AD81B2__INCLUDED_)
#define AFX_EASYFTP_H__51EEF4A4_7B5E_4FB8_AC27_96DF75AD81B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEasyFtpApp:
// See EasyFtp.cpp for the implementation of this class
//

class CEasyFtpApp : public CWinApp
{
public:
	CEasyFtpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyFtpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEasyFtpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYFTP_H__51EEF4A4_7B5E_4FB8_AC27_96DF75AD81B2__INCLUDED_)
