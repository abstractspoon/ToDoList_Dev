#if !defined(AFX_RRECEnToolBar_H__E0596D7D_418C_49B9_AC57_2F0BF93053C9__INCLUDED_)
#define AFX_RRECEnToolBar_H__E0596D7D_418C_49B9_AC57_2F0BF93053C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RRECEnToolBar.h : header file
//

#include "..\3rdparty\FontComboBox.h"
#include "..\3rdparty\SizeComboBox.h"

#include "..\shared\entoolbar.h"

/////////////////////////////////////////////////////////////////////////////
// CRRECEnToolBar window

class CRRECToolBar : public CEnToolBar
{
// Construction
public:
	CRRECToolBar();
	BOOL Create(CWnd* parent, LPCTSTR szImageFile = NULL, COLORREF crMask = -1);

	BOOL SetButtonState(int nID, UINT nState);
	BOOL IsButtonChecked(int nID) const;
	BOOL CheckButton(int nID, BOOL bChecked);
	operator CToolBarCtrl&() { return GetToolBarCtrl(); }

	BOOL GetDroppedState() const;

// Attributes
public:

// Operations
public:

	BOOL SetFontName( const CString& font );
	BOOL SetFontSize( int size );
	BOOL IncrementFontSize(int nBy = 1);
	void SetFontColor( COLORREF color, BOOL bForeground );
	COLORREF GetFontColor(BOOL bForeground, BOOL bTranslateDefault = FALSE) const;
	int GetFontSize() const;
	CString GetFontName() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRRECEnToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRRECToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRRECEnToolBar)
	afx_msg void OnSelchangeFont();
	afx_msg void OnSelchangeSize();
	afx_msg void OnKillFocusCombo();
	afx_msg LRESULT OnColorButton( WPARAM color, LPARAM nCtrlID);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg void OnColorDropDown(NMHDR* pNMHDR, LRESULT* pResult);
DECLARE_MESSAGE_MAP()

	LRESULT OnButtonPostPaint(LPNMTBCUSTOMDRAW lpNMCustomDraw);

private:

	CFontNameComboBox	m_font;
	CSizeComboBox	m_size;
	COLORREF		m_crText, m_crBack;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RRECEnToolBar_H__E0596D7D_418C_49B9_AC57_2F0BF93053C9__INCLUDED_)
