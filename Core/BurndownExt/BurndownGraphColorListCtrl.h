#if !defined(AFX_BURNDOWNGRAPHCOLORLISTCTRL_H__D35F8E6E_730B_4170_8B17_6C73E0176D51__INCLUDED_)
#define AFX_BURNDOWNGRAPHCOLORLISTCTRL_H__D35F8E6E_730B_4170_8B17_6C73E0176D51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownGraphColorListCtrl.h : header file
//

#include "..\Shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CBurndownChart;

/////////////////////////////////////////////////////////////////////////////
// CBurndownGraphColorListCtrl window

class CBurndownGraphColorListCtrl : public CInputListCtrl
{
// Construction
public:
	CBurndownGraphColorListCtrl();
	virtual ~CBurndownGraphColorListCtrl();

	BOOL Initialize(const CBurndownChart& chart);

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurndownGraphColorListCtrl)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CBurndownGraphColorListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL GetButtonRect(int nRow, int nCol, CRect& rButton) const;
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURNDOWNGRAPHCOLORLISTCTRL_H__D35F8E6E_730B_4170_8B17_6C73E0176D51__INCLUDED_)
