#if !defined(AFX_ORDEREDEDIT_H__41FF766F_F91C_4FEE_A562_35DAA2599E78__INCLUDED_)
#define AFX_ORDEREDEDIT_H__41FF766F_F91C_4FEE_A562_35DAA2599E78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// orderededit.h : header file
//

#include "..\shared\ncgutter.h"

/////////////////////////////////////////////////////////////////////////////
// COrderedEdit window

class COrderedEdit : public CEdit
{
// Construction
public:
	COrderedEdit();

protected:
	CNcGutter m_gutter;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderedEdit)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COrderedEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(COrderedEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	// callbacks for gutter
	afx_msg LRESULT OnGutterGetFirstVisibleTopLevelItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetNextItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetFirstChildItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterPostDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterPostNcDraw(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterRecalcColWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetItemRect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetSelectedItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterSetSelectedItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterHitTest(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	static int GetWidth(int nNumber, CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDEREDEDIT_H__41FF766F_F91C_4FEE_A562_35DAA2599E78__INCLUDED_)
