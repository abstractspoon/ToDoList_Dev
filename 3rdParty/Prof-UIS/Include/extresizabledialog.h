// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_RESIZABLE_DIALOG_H)
#define __EXT_RESIZABLE_DIALOG_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif // __EXT_TEMPL_H

#if (!defined __EXT_BUTTON_H)
	#include <ExtButton.h>
#endif // __EXT_BUTTON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CExtResizableDialog window

#if _MFC_VER < 0x700
	#define __BASEOF_CExtResizableDialog__ CDialog
#else
	#define __BASEOF_CExtResizableDialog__ CExtADLG < CDialog >
#endif

class __PROF_UIS_API CExtResizableDialog
	: public CExtWA < CExtWS < __BASEOF_CExtResizableDialog__ > >
{
	bool m_bEnabledControlBarUpdate:1;
	bool m_bInConrolBarUpdate:1;

// Construction
public:
	DECLARE_DYNCREATE( CExtResizableDialog )
	
	CExtResizableDialog();
	CExtResizableDialog(
		UINT nIDTemplate,
		CWnd * pParentWnd = NULL
		);
	CExtResizableDialog(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd = NULL
		);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtResizableDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

// Generated message map functions
protected:
	//{{AFX_MSG(CExtResizableDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();
}; // class CExtResizableDialog

/////////////////////////////////////////////////////////////////////////////
// CExtResizablePropertyPage window

#if _MFC_VER < 0x700
	#define __BASEOF_CExtResizablePropertyPage__ CPropertyPage
#else
	#define __BASEOF_CExtResizablePropertyPage__ CExtAPPG< CPropertyPage >
#endif

class __PROF_UIS_API CExtResizablePropertyPage
	: public CExtWA < CExtWS < __BASEOF_CExtResizablePropertyPage__ > >
{
	bool m_bEnabledControlBarUpdate:1;
	bool m_bInConrolBarUpdate:1;

// Construction
public:
	DECLARE_DYNCREATE( CExtResizablePropertyPage )
	
	CExtResizablePropertyPage();
	CExtResizablePropertyPage(
		UINT nIDTemplate,
		UINT nIDCaption = 0 
		);
	CExtResizablePropertyPage(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		UINT nIDCaption = 0 
		);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtResizablePropertyPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

// Generated message map functions
protected:
	//{{AFX_MSG(CExtResizablePropertyPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();
}; // class CExtResizablePropertyPage

/////////////////////////////////////////////////////////////////////////////
// CExtResizablePropertySheet window

#if _MFC_VER < 0x700
	#define __BASEOF_CExtResizablePropertySheet__ CPropertySheet
#else
	#define __BASEOF_CExtResizablePropertySheet__ CExtAPSH < CPropertySheet >
#endif

class __PROF_UIS_API CExtResizablePropertySheet
	: public CExtWA < CExtWS < __BASEOF_CExtResizablePropertySheet__ > >
{
	bool m_bSheetInitialized:1;

	void _SyncActivePage();

	class __PROF_UIS_API CButtonInSheetHook : public CExtButton
	{
	public:
		virtual void PostNcDestroy()
		{
			CExtButton::PostNcDestroy();
			delete this;
		}
	}; // class CButtonInSheetHook

// Construction
public:
	DECLARE_DYNCREATE( CExtResizablePropertySheet )
	
	CExtResizablePropertySheet();

	CExtResizablePropertySheet(
		UINT nIDCaption,
		CWnd *pParentWnd = NULL,
		UINT iSelectPage = 0
		);

	CExtResizablePropertySheet(
		__EXT_MFC_SAFE_LPCTSTR pszCaption,
		CWnd *pParentWnd = NULL,
		UINT iSelectPage = 0
		);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtResizablePropertySheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnResizablePropertySheetInitialized();
protected:
	virtual void _TrickSyncActviveChild();

// Generated message map functions
protected:
	//{{AFX_MSG(CExtResizablePropertySheet)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtResizablePropertySheet

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXT_RESIZABLE_DIALOG_H)
