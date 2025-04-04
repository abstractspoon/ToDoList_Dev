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
#include "..\Shared\TreeSelectionHelper.h"

#include "..\3rdParty\DragDrop.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_KLCN_EDITTASKDONE		= (WM_APP+1); // WPARAM = HWND, LPARAM = TRUE/FALSE
const UINT WM_KLCN_GETTASKICON		= (WM_APP+2); // WPARAM = TaskID, LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_KLCN_EDITTASKICON		= (WM_APP+3); // WPARAM = HWND, LPARAM = 
const UINT WM_KLCN_EDITTASKFLAG		= (WM_APP+4); // WPARAM = HWND, LPARAM = TRUE/FALSE
const UINT WM_KLCN_EDITTASKPIN		= (WM_APP+5); // WPARAM = HWND, LPARAM = TRUE/FALSE
const UINT WM_KLCN_EDITTASKLABEL	= (WM_APP+6); // WPARAM = HWND, LPARAM = TaskID
const UINT WM_KLCN_SHOWFILELINK		= (WM_APP+7); // WPARAM = HWND, LPARAM = LPCTSTR
const UINT WM_KLCN_EDITTASKLOCK		= (WM_APP+8); // WPARAM = HWND, LPARAM = TRUE/FALSE

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx window

class CKanbanColumnCtrl : public CTreeCtrl, protected CDragDropData
{
	DECLARE_DYNAMIC(CKanbanColumnCtrl);

// Construction
public:
	CKanbanColumnCtrl(const CKanbanItemMap& data,
					  const KANBANCOLUMN& columnDef,
					  CFontCache& fonts,
					  const CDWordArray& aPriorityColors,
					  const CKanbanAttributeArray& aDisplayAttrib,
					  const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs);
	
	CString GetAttributeID() const;
	int GetAttributeValues(CStringArray& aValues) const;
	int GetAttributeValueIDs(CStringArray& aValueIDs) const;
	CString GetAttributeValue() const;
	CString GetAttributeValueID() const;
	BOOL HasMultipleValues() const;
	BOOL HasAnyValues() const;
	BOOL IsBacklog() const;
	BOOL AttributeValuesMatch(const CKanbanColumnCtrl& other) const;
	BOOL IsEmpty() const { return (GetCount() == 0); }
	BOOL IsFull() const;
	int GetCount() const { return (int)(CTreeCtrl::GetCount() - m_mapGroupHeaders.GetCount()); }
	int GetMaxCount() const { return m_columnDef.nMaxTaskCount; }

	const KANBANCOLUMN& ColumnDefinition() const { return m_columnDef; }
	
	BOOL Create(UINT nID, CWnd* pParentWnd);
	HTREEITEM AddTask(const KANBANITEM& ki);
	BOOL RemoveTask(DWORD dwTaskID);
	BOOL RemoveAll();
	int RemoveDeletedTasks(const CDWordSet& mapCurIDs);

	BOOL Sort(TDC_ATTRIBUTE nBy, BOOL bAscending);
	BOOL GroupBy(TDC_ATTRIBUTE nAttribID);
	void SetGroupHeaderBackgroundColor(COLORREF color);

	BOOL SaveToImage(CBitmap& bmImage, const CSize& reqSize);
	CSize CalcRequiredSizeForImage() const;
	BOOL CreateDragImage(CImageList& ilDrag, CSize& sizeImage);

	HTREEITEM FindItem(DWORD dwTaskID) const;
	HTREEITEM FindItem(const IUISELECTTASK& select, BOOL bNext, HTREEITEM htiStart = NULL) const;

	DWORD GetTaskID(HTREEITEM hti) const { return (hti ? GetItemData(hti) : 0); }

	int GetSelectedTaskIDs(CDWordArray& aTaskIDs) const;
	DWORD GetOnlySelectedTask() const;
	BOOL SelectTasks(const CDWordArray& aTaskIDs);
	BOOL SelectAll();
	void ScrollToSelection();
	BOOL HasTasks(const CDWordArray& aTaskIDs) const;
	BOOL IsTaskSelected(DWORD dwTaskID) const;
	int GetSelectedCount() const;
	HTREEITEM GetFirstSelectedItem() const;
	HTREEITEM GetLastSelectedItem() const;
	HTREEITEM GetNextTopLevelItem(HTREEITEM hti, BOOL bNext) const;

	BOOL GetLabelEditRect(LPRECT pEdit);
	BOOL GetItemBounds(HTREEITEM hti, LPRECT lpRect) const;
	HTREEITEM HitTestItemSidebar(const CPoint& ptScreen) const;

	void ClearSelection();
	void SetSelected(BOOL bSelected);
	BOOL SelectItem(HTREEITEM hItem, BOOL bByMouse);

	void SetDropTarget(BOOL bTarget);
	void SetBackgroundColor(COLORREF color);
	void SetFullColor(COLORREF color);
	void SetMaximumTaskCount(int nMaxTasks);

	void SetOptions(DWORD dwOptions);
	void SetReadOnly(BOOL bReadOnly) { m_bReadOnly = bReadOnly; Invalidate(); }
	void OnDisplayAttributeChanged();
	int CalcAvailableAttributeWidth(int nColWidth = -1) const;
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);
	void RefreshItemLineHeights();
	void RefreshItemLineHeights(DWORD dwTaskID);

	void FilterToolTipMessage(MSG* pMsg);

	const CTreeCtrlHelper& TCH() const { return m_tch; }
	CTreeCtrlHelper& TCH() { return m_tch; }

	static BOOL CanDrag(const CKanbanColumnCtrl* pSrcCol, const CKanbanColumnCtrl* pDestCol);

protected:
	BOOL m_bSelected;
	BOOL m_bSavingToImage;
	BOOL m_bDropTarget;
	BOOL m_bDrawTaskFlags, m_bDrawTaskFileLinks, m_bDrawTaskLocks;
	BOOL m_bReadOnly;

	CFontCache& m_fonts;
	const CKanbanItemMap& m_data;
	const CDWordArray& m_aPriorityColors;
	const CKanbanAttributeArray& m_aDisplayAttrib;
	const CKanbanCustomAttributeDefinitionArray& m_aCustAttribDefs;

	// For quick lookup
	CHTIMap m_mapHTItems;

	CImageList m_ilCheckboxes, m_ilIcons;
	CToolTipCtrlEx m_tooltip;
	CTreeCtrlHelper m_tch;
	CDWordArray m_aSelTaskIDs;

	KANBANCOLUMN m_columnDef;
	DWORD m_dwDisplay, m_dwOptions;
	int m_nItemTextHeight, m_nItemTextBorder, m_nNumTitleLines;
	KBC_ATTRIBLABELS m_nAttribLabelVisibility;
	COLORREF m_crItemShadow, m_crGroupHeaderBkgnd, m_crFullBkgnd;

	TDC_ATTRIBUTE m_nSortBy, m_nGroupBy;
	BOOL m_bSortAscending;

	typedef CMap<DWORD, DWORD, CString, CString&> CGroupHeaderMap;
	CGroupHeaderMap m_mapGroupHeaders;

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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTooltipShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnThemeChanged(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnHitTest(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGetNextItem(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()

protected:
	// Prevent anyone calling these directly
	void DeleteAllItems() { ASSERT(0); }
	void InsertItem() { ASSERT(0); }
	void DeleteItem() { ASSERT(0); }
	HTREEITEM GetSelectedItem() const { ASSERT(0); return NULL; }

protected:
	// IDragDropRenderer interface
	CSize OnGetDragSize(CDC& dc);
	void  OnDrawDragData(CDC& dc, const CRect& rc, COLORREF& crMask);
	
protected:
	int CalcItemTitleTextHeight() const;
	void RecalcItemLineHeight();
	void RefreshBkgndColor();
	BOOL HandleButtonClick(CPoint point, BOOL bLeftBtn, HTREEITEM& htiHit);
	BOOL HandleExtendedSelection(HTREEITEM htiSelected);
	BOOL InitTooltip();
	void RefreshItemLineHeights(HTREEITEM hti);
	int GetItemDisplayAttributeCount(const KANBANITEM& ki) const;
	void NotifyParentSelectionChange(HTREEITEM hItem, BOOL bByMouse);
	BOOL SelectTask(DWORD dwTaskID);
	BOOL IsOnlySelectedTask(DWORD dwTaskID);
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }
	BOOL WantDisplayAttribute(TDC_ATTRIBUTE nAttribID, const KANBANITEM* pKI) const;
	int CalcIndentation(HTREEITEM hti) const;
	void RecalcItemShadowColor();

	BOOL IsGroupHeaderTask(DWORD dwTaskID) const;
	BOOL IsGroupHeaderItem(HTREEITEM hti) const;
	int GetGroupValues(CStringSet& aValues) const;
	void CheckRebuildGroupHeaders();
	void RebuildGroupHeaders(const CStringSet& aValues);
	int CompareGrouping(LPARAM lParam1, LPARAM lParam2) const;
	CString FormatTaskGroupHeaderText(DWORD dwHeaderID) const;
	BOOL IsGrouping() const { return (m_nGroupBy != TDCA_NONE); }

	void DoSort();
	int CompareItems(LPARAM lParam1, LPARAM lParam2) const;
	int CompareParentAndPins(const KANBANITEM*& pKI1, const KANBANITEM*& pKI2) const;
	int CompareAttributeValues(const KANBANITEM* pKI1, const KANBANITEM* pKI2, TDC_ATTRIBUTE nBy, BOOL bAscending) const;
	int BuildSortedSelection(CHTIList& lstHTI) const;
	BOOL IsSorting() const { return (m_nSortBy != TDCA_NONE); }

	BOOL GetItemLabelTextRect(HTREEITEM hti, CRect& rItem, BOOL bEdit = FALSE) const;
	BOOL GetItemTooltipRect(HTREEITEM hti, CRect& rItem) const;
	BOOL GetItemRect(HTREEITEM hti, CRect& rItem) const;
	BOOL GetItemCheckboxRect(HTREEITEM hti, CRect& rItem) const;
	BOOL GetItemCheckboxRect(CRect& rItem) const;

	KBC_IMAGETYPE HitTestImage(HTREEITEM hti, CPoint point) const;
	BOOL HitTestCheckbox(HTREEITEM hti, CPoint point) const;
	CString HitTestFileLink(HTREEITEM hti, CPoint point) const;

	void DrawItem(CDC* pDC, DWORD dwTaskID, const CRect& rItem);
	void DrawItemCheckbox(CDC* pDC, const KANBANITEM* pKI, CRect& rItem);
	void DrawItemParents(CDC* pDC, const KANBANITEM* pKI, CRect& rItem, COLORREF crText) const;
	void DrawItemFileLinks(CDC* pDC, const KANBANITEM* pKI, CRect& rItem, COLORREF crText) const;
	void DrawItemImages(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const;
	void DrawItemBar(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const;
	void DrawAttribute(CDC* pDC, CRect& rLine, TDC_ATTRIBUTE nAttribID, const CString& sValue, int nFlags, COLORREF crText) const;
	void DrawItemShadow(CDC* pDC, CRect& rItem) const;
	void DrawItemBackground(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const;
	void DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);
	void DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);
	void DrawItemImage(CDC* pDC, const CRect& rImage, KBC_IMAGETYPE nType, BOOL bSet, HIMAGELIST hIL = NULL, int nIndex = -1) const;
	BOOL DrawTaskIcon(CDC* pDC, const KANBANITEM* pKI, const CRect& rIcon) const;

	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANCOLUMNCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
