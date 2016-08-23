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

#if (!defined __EXT_COMBO_BOX_H)
#define __EXT_COMBO_BOX_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CExtComboEditCtrlHook window
// (internal combobox edit ctrl)

class CExtComboBox;

class __PROF_UIS_API CExtComboEditCtrlHook : public CEdit
{
	DECLARE_DYNCREATE(CExtComboEditCtrlHook)

// Construction
public:
	CExtComboEditCtrlHook();

// Attributes
public:

// Operations
public:
	CExtComboBox * GetExtComboBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtComboEditCtrlHook)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtComboEditCtrlHook();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtComboEditCtrlHook)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	friend class CExtComboBox;

}; // class __PROF_UIS_API CExtComboEditCtrlHook

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox window

class __PROF_UIS_API CExtComboBox
	: public CComboBox
	, public CExtMouseCaptureSink
{
	DECLARE_DYNCREATE(CExtComboBox)

// Construction
public:
	CExtComboBox();

// Attributes
public:
	bool m_bEnableAutoComplete:1;
private:
	bool m_bDrawing:1;
	bool m_bWatching:1;
	bool m_bAutoComplete:1;
	CExtComboEditCtrlHook * m_pInnerEditHook;
protected:
	DWORD m_dwUpdateTimer, m_dwUpdatePeriod;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtComboBox();

	virtual bool OnTrackComboContextMenu( // just return false for default WM_CONTEXTMENU processing
		HWND hWndSrc // handle of combo window or child edit control
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtComboBox)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnEditCtrlUpdate();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void _OnPaintImpl(
		bool bPressed,
		bool bHover
		);
	virtual void _OnDrawComboImpl(
		bool bPressed,
		bool bHover,
		CDC * pDC = NULL
		);

	friend class CExtComboEditCtrlHook;

}; // class __PROF_UIS_API CExtComboBox

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_COMBO_BOX_H
