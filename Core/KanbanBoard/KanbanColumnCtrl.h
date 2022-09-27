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
					const CKanbanAttributeArray& aDisplayAttrib);
	
	CString GetAttributeID() const;
	int GetAttributeValues(CStringArray& aValues) const;
	int GetAttributeValueIDs(CStringArray& aValueIDs) const;
	CString GetAttributeValue() const;
	CString GetAttributeValueID() const;
	BOOL HasMultipleValues() const;
	BOOL HasAnyValues() const;
	BOOL IsBacklog() const;
	BOOL IsEmpty() const { return (GetCount() == 0); }
	BOOL AttributeValuesMatch(const CKanbanColumnCtrl& other) const;

	const KANBANCOLUMN& ColumnDefinition() const { return m_columnDef; }
	
	BOOL Create(UINT nID, CWnd* pParentWnd);
	HTREEITEM AddTask(const KANBANITEM& ki);
	BOOL DeleteTask(DWORD dwTaskID);
	BOOL DeleteAll();
	int RemoveDeletedTasks(const CDWordSet& mapCurIDs);

	void Sort(TDC_ATTRIBUTE nBy, BOOL bAscending);
	
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

	BOOL GetLabelEditRect(LPRECT pEdit);
	BOOL GetItemBounds(HTREEITEM hti, LPRECT lpRect) const;
	HTREEITEM HitTestItemSidebar(const CPoint& ptScreen) const;

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
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);
	void RefreshItemLineHeights();
	void RefreshItemLineHeights(DWORD dwTaskID);

	void FilterToolTipMessage(MSG* pMsg);

	const CTreeCtrlHelper& TCH() const { return m_tch; }
	CTreeCtrlHelper& TCH() { return m_tch; }

	static CString GetAttributeLabel(TDC_ATTRIBUTE nAttrib, KBC_ATTRIBLABELS nLabelVis);
	static CString FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sValue, KBC_ATTRIBLABELS nLabelVis);
	static BOOL CanDrag(const CKanbanColumnCtrl* pSrcCol, const CKanbanColumnCtrl* pDestCol);

protected:
	BOOL m_bSelected;
	BOOL m_bSavingToImage;
	BOOL m_bDropTarget;
	BOOL m_bDrawTaskFlags, m_bDrawTaskFileLinks;

	const CKanbanItemMap& m_data;
	CFontCache& m_fonts;
	const CDWordArray& m_aPriorityColors;
	const CKanbanAttributeArray& m_aDisplayAttrib;

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
	COLORREF m_crItemShadow;
	
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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()

protected:
	// Prevent anyone calling these
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
	int BuildSortedSelection(CHTIList& lstHTI) const;
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }
	BOOL WantDisplayAttribute(TDC_ATTRIBUTE nAttrib, const KANBANITEM* pKI) const;
	int CalcIndentation(HTREEITEM hti) const;
	void RecalcItemShadowColor();

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
	void DrawAttribute(CDC* pDC, CRect& rLine, TDC_ATTRIBUTE nAttrib, const CString& sValue, int nFlags, COLORREF crText) const;
	void DrawItemShadow(CDC* pDC, CRect& rItem) const;
	void DrawItemBackground(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const;
	void DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);
	void DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);
	void DrawItemImage(CDC* pDC, const CRect& rImage, KBC_IMAGETYPE nType, BOOL bSet, HIMAGELIST hIL = NULL, int nIndex = -1) const;
	BOOL DrawTaskIcon(CDC* pDC, const KANBANITEM* pKI, const CRect& rIcon) const;

	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CompareAttributeValues(const KANBANITEM* pKI1, const KANBANITEM* pKI2, const KANBANSORT& sort);
	static UINT GetDisplayFormat(TDC_ATTRIBUTE nAttrib, BOOL bLong);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANCOLUMNCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
