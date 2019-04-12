// EncryptDecryptTest.h : main header file for the ENCRYPTDECRYPTTEST application
//

#if !defined(AFX_ENCRYPTDECRYPTTEST_H__CF5FF0F0_A5E0_442E_A00C_90EC76C79C74__INCLUDED_)
#define AFX_ENCRYPTDECRYPTTEST_H__CF5FF0F0_A5E0_442E_A00C_90EC76C79C74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEncryptDecryptTestApp:
// See EncryptDecryptTest.cpp for the implementation of this class
//

class CEncryptDecryptTestApp : public CWinApp
{
public:
	CEncryptDecryptTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEncryptDecryptTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEncryptDecryptTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCRYPTDECRYPTTEST_H__CF5FF0F0_A5E0_442E_A00C_90EC76C79C74__INCLUDED_)
