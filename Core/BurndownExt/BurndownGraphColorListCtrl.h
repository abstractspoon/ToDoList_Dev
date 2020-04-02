#if !defined(AFX_BURNDOWNGRAPHCOLORLISTCTRL_H__D35F8E6E_730B_4170_8B17_6C73E0176D51__INCLUDED_)
#define AFX_BURNDOWNGRAPHCOLORLISTCTRL_H__D35F8E6E_730B_4170_8B17_6C73E0176D51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownGraphColorListCtrl.h : header file
//

#include "BurndownStruct.h"

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
	const CGraphColorMap& GetGraphColors() const { return m_mapColors; }

protected:
	CGraphColorMap m_mapColors;

protected:
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual void EditCell(int nItem, int nCol, BOOL bBtnClick);
	virtual BOOL CanEditCell(int nRow, int nCol) const;
	virtual void DrawCellText(CDC* pDC, int nRow, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURNDOWNGRAPHCOLORLISTCTRL_H__D35F8E6E_730B_4170_8B17_6C73E0176D51__INCLUDED_)
