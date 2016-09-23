#if !defined(AFX_TDLCOLUMNATTRIBVISIBILITYLISTCTRL_H__8135B058_507C_4B82_A825_3E5847BB5856__INCLUDED_)
#define AFX_TDLCOLUMNATTRIBVISIBILITYLISTCTRL_H__8135B058_507C_4B82_A825_3E5847BB5856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLColumnAttribVisibilityListCtrl.h : header file
//

#include "tdcstruct.h"

#include "..\Shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnAttribVisibilityListCtrl window

class CTDLColumnAttribVisibilityListCtrl : public CInputListCtrl
{
// Construction
public:
	CTDLColumnAttribVisibilityListCtrl();

	void SetVisibility(const TDCCOLEDITFILTERVISIBILITY& vis);
	void GetVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;

	void SetAllVisible(BOOL bVisible = TRUE);
	void SetShowEditsAndFilters(TDL_SHOWATTRIB nShow);
	
// Attributes
protected:
	TDCCOLEDITFILTERVISIBILITY m_vis;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLColumnAttribVisibilityListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLColumnAttribVisibilityListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLColumnAttribVisibilityListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void BuildListCtrl();
	void UpdateVisibility();
	BOOL IsTimeCellEnabled(int nItem, int nCol) const;
	TDC_COLUMN GetItemColumn(int nItem) const;
	TDC_ATTRIBUTE GetItemAttrib(int nItem) const;

	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void EditCell(int nItem, int nCol);
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void InitState();
	virtual BOOL IsButtonEnabled(int nRow, int nCol) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCOLUMNATTRIBVISIBILITYLISTCTRL_H__8135B058_507C_4B82_A825_3E5847BB5856__INCLUDED_)
