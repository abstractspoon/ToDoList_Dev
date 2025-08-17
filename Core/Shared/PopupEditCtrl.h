
#if !defined(AFX_POPUPEDITCTRL_H__2E5810B2_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_)
#define AFX_POPUPEDITCTRL_H__2E5810B2_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InputListCtrlEdit.h : header file
//

#include "enedit.h"

/////////////////////////////////////////////////////////////////////////////
// CPopupEditCtrl window

const UINT WM_PENDEDIT = ::RegisterWindowMessage(_T("WM_PENDEDIT"));
const UINT WM_PCANCELEDIT = ::RegisterWindowMessage(_T("WM_PCANCELEDIT"));

enum PEC_CLEANUP
{
	PEC_AUTOHIDE,
	PEC_AUTODESTROY,
	PEC_NOCLEANUP,
};

class CPopupEditCtrl : public CEnEdit
{
	DECLARE_DYNAMIC(CPopupEditCtrl)

// Construction
public:
	CPopupEditCtrl(PEC_CLEANUP nCleanup = PEC_AUTOHIDE);
	virtual ~CPopupEditCtrl();

	virtual BOOL Create(CWnd* pParentWnd, UINT nID, DWORD dwStyle = WS_CHILD);

	void Reset() { m_bEditEnded = FALSE; }
	void Show(CRect rPos = CRect(0, 0, 0, 0), BOOL bDisableMouseWheel = TRUE); // screen or client depending on original creation mode
	void CleanUp();
	void SetCleanUp(PEC_CLEANUP nCleanUp) { m_nCleanUp = nCleanUp; }
	void EndEdit(BOOL bCancel);
	void SetSpinBuddy(CSpinButtonCtrl* pBuddy);

// Attributes
protected:
	BOOL m_bEditEnded;
	UINT m_nID;
	CWnd* m_pParent;
	CSpinButtonCtrl* m_pSpinBuddy;
	PEC_CLEANUP m_nCleanUp;

	// Generated message map functions
protected:
	//{{AFX_MSG(CPopupEditCtrl)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg LRESULT OnPECShow(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnFloatStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void EndEdit(BOOL bCancel, BOOL bIntentional);
	void Hide();
	BOOL ShowSpinBuddy(BOOL bShow);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTLISTCTRLEDIT_H__2E5810B2_D7DF_11D1_AB19_0000E8425C3E__INCLUDED_)
