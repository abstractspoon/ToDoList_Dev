#if !defined(AFX_KANBANCOLUMNCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
#define AFX_KANBANCOLUMNCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "Kanbanstruct.h"
#include "Kanbanenum.h"

#include "..\Shared\fontcache.h"
#include "..\Shared\tooltipctrlex.h"
#include "..\Shared\mapex.h"
#include "..\Shared\TreeCtrlHelper.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_KLCN_TOGGLETASKDONE	= (WM_APP+1); // WPARAM = HWND, LPARAM = Task ID
const UINT WM_KLCN_GETTASKICON		= (WM_APP+2); // WPARAM = TaskID, LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_KLCN_EDITTASKICON		= (WM_APP+3); // WPARAM = HWND, LPARAM = Task ID
const UINT WM_KLCN_TOGGLETASKFLAG	= (WM_APP+4); // WPARAM = HWND, LPARAM = Task ID

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx window

class CKanbanColumnCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CKanbanColumnCtrl);

// Construction
public:
	CKanbanColumnCtrl(const CKanbanItemMap& data, 
					const KANBANCOLUMN& columnDef, 
					CFontCache& fonts,
					const CDWordArray& aPriorityColors,
					const CKanbanAttributeArray& aDisplayAttrib);
	
	CString GetAttributeID() const;
	int GetAttributeValues(CStringArray& aValues) const;
	int GetAttributeValueIDs(CStringArray& aValueIDs) const;
	CString GetAttributeValue() const;
	CString GetAttributeValueID() const;
	BOOL HasMultipleValues() const;
	BOOL HasAnyValues() const;
	BOOL IsBacklog() const;
	BOOL AttributeValuesMatch(const CKanbanColumnCtrl& other) const;

	const KANBANCOLUMN& ColumnDefinition() const { return m_columnDef; }
	
	BOOL Create(UINT nID, CWnd* pParentWnd);
	HTREEITEM AddTask(const KANBANITEM& ki, BOOL bSelect);
	BOOL DeleteTask(DWORD dwTaskID);
	BOOL DeleteAll();
	int RemoveDeletedTasks(const CDWordSet& mapCurIDs);

	void Sort(TDC_ATTRIBUTE nBy, BOOL bAscending);
	
	BOOL SaveToImage(CBitmap& bmImage, const CSize& reqSize);
	CSize CalcRequiredSizeForImage() const;

	HTREEITEM FindTask(DWORD dwTaskID) const;
	HTREEITEM FindTask(const CPoint& ptScreen) const;
	HTREEITEM FindTask(const IUISELECTTASK& select, BOOL bNext, HTREEITEM htiStart = NULL) const;

	DWORD GetTaskID(HTREEITEM hti) const { return GetItemData(hti); }

	DWORD GetSelectedTaskID() const;
	BOOL SelectTasks(const CDWordArray& aTaskIDs) { return FALSE; }
	BOOL SelectTask(DWORD dwTaskID);
	void ScrollToSelection();

	BOOL GetLabelEditRect(LPRECT pEdit);
	BOOL GetItemBounds(HTREEITEM hti, LPRECT lpRect) const;

	void ClearSelection();
	void SetSelected(BOOL bSelected);
	BOOL SelectItem(HTREEITEM hItem, BOOL bByMouse);

	void SetDropTarget(BOOL bTarget);
	void SetBackgroundColor(COLORREF color);
	void SetExcessColor(COLORREF color);
	void SetMaximumTaskCount(int nMaxTasks);

	void SetOptions(DWORD dwOptions);
	void OnDisplayAttributeChanged();
	int CalcAvailableAttributeWidth(int nColWidth = -1) const;
	BOOL SelectionHasLockedTasks() const;
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);
	void RefreshItemLineHeights();
	void RefreshItemLineHeights(DWORD dwTaskID);

	void FilterToolTipMessage(MSG* pMsg);

	const CTreeCtrlHelper& TCH() const { return m_tch; }
	CTreeCtrlHelper& TCH() { return m_tch; }

	static CString FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sValue, KBC_ATTRIBLABELS nLabelVis);
	static BOOL CanDrag(const CKanbanColumnCtrl* pSrcCol, const CKanbanColumnCtrl* pDestCol);

protected:
	BOOL m_bSelected;
	BOOL m_bSavingToImage;
	BOOL m_bDropTarget;
	BOOL m_bDrawTaskFlags;
	BOOL m_bDrawTaskParents;

	const CKanbanItemMap& m_data;
	CFontCache& m_fonts;
	const CDWordArray& m_aPriorityColors;
	const CKanbanAttributeArray& m_aDisplayAttrib;

	// For quick lookup
	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM&> m_mapItems;

	CImageList m_ilCheckboxes, m_ilFlags;
	CToolTipCtrlEx m_tooltip;
	CTreeCtrlHelper m_tch;

	KANBANCOLUMN m_columnDef;
	DWORD m_dwDisplay, m_dwOptions;
	int m_nItemTextHeight, m_nItemTextBorder;
	KBC_ATTRIBLABELS m_nAttribLabelVisiability;
	HTREEITEM m_htiHot;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKanbanColumnCtrl();

protected:
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CKanbanListCtrlEx)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTooltipShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnThemeChanged(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnMouseWheel(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	// Prevent parent calling these
	void DeleteAllItems() {}
	void InsertItem() {}
	void DeleteItem() {}
	
protected:
	int CalcItemTitleTextHeight() const;
	void RecalcItemLineHeight();
	void RefreshBkgndColor();
	BOOL HandleButtonClick(CPoint point, HTREEITEM& htiHit);
	BOOL GetItemCheckboxRect(HTREEITEM hti, CRect& rItem, const KANBANITEM* pKI) const;
	BOOL GetItemCheckboxRect(CRect& rItem) const;
	BOOL GetItemLabelTextRect(HTREEITEM hti, CRect& rItem, BOOL bEdit = FALSE, const KANBANITEM* pKI = NULL) const;
	BOOL InitTooltip();
	BOOL GetItemTooltipRect(HTREEITEM hti, CRect& rItem, const KANBANITEM* pKI) const;
	BOOL GetItemRect(HTREEITEM hti, CRect& rItem, const KANBANITEM* pKI) const;
	void RefreshItemLineHeights(HTREEITEM hti);
	int GetItemDisplayAttributeCount(const KANBANITEM& ki) const;
	BOOL HitTestCheckbox(HTREEITEM hti, CPoint point) const;
	BOOL HitTestIcon(HTREEITEM hti, CPoint point) const;
	BOOL HitTestFlag(HTREEITEM hti, CPoint point) const;
	void GetIconRect(HTREEITEM hti, CRect& rIcon) const;
	void GetFlagRect(HTREEITEM hti, CRect& rFlag) const;
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }
	void UpdateHotItem();

	void DrawItemCheckbox(CDC* pDC, const KANBANITEM* pKI, CRect& rItem);
	void DrawItemParents(CDC* pDC, const KANBANITEM* pKI, CRect& rItem, COLORREF crText) const;
	void DrawItemIcons(CDC* pDC, const KANBANITEM* pKI, CRect& rItem, BOOL bHot) const;
	void DrawItemBar(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const;
	void DrawAttribute(CDC* pDC, CRect& rLine, TDC_ATTRIBUTE nAttrib, const CString& sValue, int nFlags, COLORREF crText) const;
	void FillItemBackground(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText, BOOL bSelected) const;
	void DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);
	void DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);

	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static UINT GetDisplayFormat(TDC_ATTRIBUTE nAttrib, BOOL bLong);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANCOLUMNCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
