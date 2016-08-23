#if !defined(AFX_ENDATETIMECTRL_H__FF1B6F5F_1132_43F3_9A2B_C7A9277F784F__INCLUDED_)
#define AFX_ENDATETIMECTRL_H__FF1B6F5F_1132_43F3_9A2B_C7A9277F784F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// endatetimectrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnDateTimeCtrl window

class CEnDateTimeCtrl : public CDateTimeCtrl
{
// Construction
public:
	CEnDateTimeCtrl();

// Attributes
protected:
	BOOL m_bWantReset;
	SYSTEMTIME m_st;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnDateTimeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnDateTimeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnDateTimeCtrl)
	afx_msg void OnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnDatetimechange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LRESULT OnPostCloseUp(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENDATETIMECTRL_H__FF1B6F5F_1132_43F3_9A2B_C7A9277F784F__INCLUDED_)
