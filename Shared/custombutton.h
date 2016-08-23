#if !defined(AFX_CUSTOMBUTTON_H__B467510A_66F2_4BE6_85C3_D34109401DAE__INCLUDED_)
#define AFX_CUSTOMBUTTON_H__B467510A_66F2_4BE6_85C3_D34109401DAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// custombutton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomButton window

class CCustomButton : public CButton
{
// Construction
public:
	CCustomButton();

public:
	void SetWindowText(LPCTSTR lpszString);
	void SetWindowText(TCHAR nChar);
	void SetTooltip(LPCTSTR lpszString);

protected:
	CToolTipCtrl m_tooltip;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomButton();

protected:
	virtual BOOL DoAction() { return FALSE; }
	virtual void DoExtraPaint(CDC* /*pDC*/, const CRect& /*rExtra*/) {}
	virtual void CalcExtraSpace(const CRect& rClient, CRect& rExtra) const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnClicked();
	afx_msg LRESULT OnShowAccelerators(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMBUTTON_H__B467510A_66F2_4BE6_85C3_D34109401DAE__INCLUDED_)
