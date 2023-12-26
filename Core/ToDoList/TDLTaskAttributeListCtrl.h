#if !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
#define AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskAttributeListCtrl.h : header file
//

#include "..\shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl window

class CTDLTaskAttributeListCtrl : public CInputListCtrl
{
// Construction
public:
	CTDLTaskAttributeListCtrl();
	virtual ~CTDLTaskAttributeListCtrl();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTaskAttributeListCtrl)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLTaskAttributeListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual IL_COLUMNTYPE GetCellType(int nRow, int nCol) const;
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
