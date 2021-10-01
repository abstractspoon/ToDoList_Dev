#if !defined(AFX_STATUSBARACTEX_H__CBD0F161_D604_44DE_B934_042C6086804F__INCLUDED_)
#define AFX_STATUSBARACTEX_H__CBD0F161_D604_44DE_B934_042C6086804F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusBarACTEx.h : header file
//

#include "..\3rdParty\StatusBarACT.h"

/////////////////////////////////////////////////////////////////////////////
// CStatusBarACTEx window

class CStatusBarACTEx : public CStatusBarACT
{
// Construction
public:
	CStatusBarACTEx();

	BOOL SetPaneTooltipIndex(int nIndex, UINT nTipID);
	BOOL SetPaneTooltipIndex(int nIndex, const CString& sTip);
	void SetUIColors(COLORREF crBackFrom, COLORREF crBackTo, COLORREF crText, BOOL bGradient, BOOL bGlass);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarACTEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStatusBarACTEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStatusBarACTEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawRectBkgnd(CDC* pDC, const CRect& rect);
	virtual BOOL DrawPaneText(CDC* pDC, int nPane, int nOffset);

	void InvalidatePane(int nPane);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSBARACTEX_H__CBD0F161_D604_44DE_B934_042C6086804F__INCLUDED_)
