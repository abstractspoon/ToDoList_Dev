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

#if (!defined __EXTCOLORDLG_H)
#define __EXTCOLORDLG_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXTCOLORCTRL_H)
	#include <ExtColorCtrl.h>
#endif

#if (!defined __EXT_BUTTON_H)
	#include <ExtButton.h>
#endif

#if (!defined __EXT_COMBO_BOX_H)
	#include <ExtComboBox.h>
#endif

#if (!defined __EXT_RESIZABLE_DIALOG_H)
	#include <ExtResizableDialog.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtColorDlg dialog

class __PROF_UIS_API CExtColorDlg : public CExtResizableDialog
{
// Construction
public:
	CExtColorDlg(
		COLORREF clrInit = RGB(255,255,255),
		COLORREF clrNew = RGB(255,255,255),
		CWnd * pParent = NULL
		);	// standard constructor
	~CExtColorDlg();

	COLORREF m_clrInit,m_clrNew;
	CExtSafeString m_strCaption; // if non-empty - used to setup color dialog caption

// Dialog Data
	//{{AFX_DATA(CExtColorDlg)
	CSpinButtonCtrl	m_SpinC;
	CSpinButtonCtrl	m_SpinM;
	CSpinButtonCtrl	m_SpinY;
	CSpinButtonCtrl	m_SpinS;
	CSpinButtonCtrl	m_SpinR;
	CSpinButtonCtrl	m_SpinL;
	CSpinButtonCtrl	m_SpinH;
	CSpinButtonCtrl	m_SpinG;
	CSpinButtonCtrl	m_SpinB;
	CEdit	m_EditC;
	CEdit	m_EditM;
	CEdit	m_EditY;
	CEdit	m_EditS;
	CEdit	m_EditR;
	CEdit	m_EditL;
	CEdit	m_EditH;
	CEdit	m_EditG;
	CEdit	m_EditB;
	CExtComboBox	m_ComboType;
	CExtButton	m_BtnCancel;
	CExtButton	m_BtnOK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bDialogInitColplete,m_bSyncingValues;
	HICON m_hIcon;
	CExtColorCtrl m_wndColorCtrl,m_wndColorDiff;

	// Generated message map functions
	//{{AFX_MSG(CExtColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelendokComboSelType();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnColorChanged(WPARAM wParam,LPARAM lParam);
	afx_msg void SyncFromEditRGB();
	afx_msg void SyncFromEditCMY();
	afx_msg void SyncFromEditHSL();
	DECLARE_MESSAGE_MAP()

private:
	void SyncToEditRGB(
		COLORREF clr,
		bool bSyncRGB,
		bool bSyncCMY,
		bool bSyncHLS
		);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXTCOLORDLG_H
