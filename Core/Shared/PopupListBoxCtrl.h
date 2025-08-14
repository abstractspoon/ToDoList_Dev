//_ **********************************************************
//_ 
//_ Name: InputListCtrlListbox.h 
//_ Purpose: 
//_ Created: 15 September 1998 
//_ Author: D.R.Godson
//_ Modified By: 
//_ 
//_ Copyright (c) 1998 Brilliant Digital Entertainment Inc. 
//_ 
//_ **********************************************************

#if !defined(AFX_POPUPLISTBOXCTRL_H__2E5810B2_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_)
#define AFX_POPUPLISTBOXCTRL_H__2E5810B2_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InputListCtrlEdit.h : header file
//

//#include "messages.h"

/////////////////////////////////////////////////////////////////////////////
// CPopupListBoxCtrl window

class CPopupListBoxCtrl : public CListBox
{
// Construction
public:
	CPopupListBoxCtrl();

	void Reset() { m_bEditEnded = FALSE; }
	void Show(CRect rPos = CRect(0, 0, 0, 0)); // screen coords
	void Hide();
	void AutoHide(BOOL bEnable = TRUE) { m_bAutoHide = bEnable; }
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE); // screen coords
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE); // screen coords
	void AutoTrack(BOOL bEnable = TRUE) { m_bAutoTrack = bEnable; }
	void ValidateRect(CRect& rPos);
	UINT GetDlgCtrlID() const { return m_nID; }

// Attributes
protected:
	BOOL m_bEditEnded;
	UINT m_nID;
	CWnd* m_pParent;
	BOOL m_bAutoHide;
	BOOL m_bAutoTrack;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupListBoxCtrl)
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID, BOOL bSort = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPopupListBoxCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPopupListBoxCtrl)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnFloatStatus(WPARAM wParam, LPARAM lParam);

	// Pseudo message handlers
	void OnEnter();
	void OnCancel();

	void ValidateSize(int& nWidth, int& nHeight);
	void ValidateRect(int& nX, int& nY, int& nWidth, int& nHeight);
	CWnd* GetParent();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTLISTCTRLEDIT_H__2E5810B2_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_)
