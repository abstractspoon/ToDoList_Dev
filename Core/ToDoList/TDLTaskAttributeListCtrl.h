#if !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
#define AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskAttributeListCtrl.h : header file
//

#include "ToDoCtrlDataUtils.h"

#include "..\shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskCtrlBase;
class CToDoCtrlData;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl window

class CTDLTaskAttributeListCtrl : public CInputListCtrl
{
// Construction
public:
	CTDLTaskAttributeListCtrl(const CTDLTaskCtrlBase& taskCtrl,
							  const CToDoCtrlData& data,
							  const TDCCOLEDITVISIBILITY& vis);

	virtual ~CTDLTaskAttributeListCtrl();

public:
	void RefreshSelectedTaskAttributeValues(BOOL bForceClear = FALSE);
	void SetAttributeVisibility(const TDCCOLEDITVISIBILITY& vis);
	void SetCustomAttributeDefinitions(const CTDCCustomAttribDefinitionArray& aAttribDefs);

protected:
	const CTDLTaskCtrlBase& m_taskCtrl;
	const CToDoCtrlData& m_data;

	CTDCTaskFormatter m_formatter;
	TDCCOLEDITVISIBILITY m_vis;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

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
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;

protected:
	void Populate();
	void CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID);
	TDC_ATTRIBUTE GetAttributeID(int nRow) const { return (TDC_ATTRIBUTE)GetItemData(nRow); }

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKATTRIBUTELISTCTRL_H__4173F4AA_2026_4FE1_B669_E6631D12C8F1__INCLUDED_)
