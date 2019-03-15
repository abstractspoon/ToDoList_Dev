#if !defined(AFX_KANBANLISTCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
#define AFX_KANBANLISTCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_

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

const UINT WM_KLCN_CHECKCHANGE	= (WM_APP+1); // WPARAM = HWND, LPARAM = Task ID
const UINT WM_KLCN_GETTASKICON	= (WM_APP+2); // WPARAM = TaskID, LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_KLCN_EDITTASKICON	= (WM_APP+3); // WPARAM = HWND, LPARAM = Task ID

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx window

class CKanbanListCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CKanbanListCtrl);

// Construction
public:
	CKanbanListCtrl(const CKanbanItemMap& data, 
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
	BOOL AttributeValuesMatch(const CKanbanListCtrl& other) const;

	const KANBANCOLUMN& ColumnDefinition() const { return m_columnDef; }
	
	BOOL Create(UINT nID, CWnd* pParentWnd);
	HTREEITEM AddTask(const KANBANITEM& ki, BOOL bSelect);
	void Sort(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent);
	
	BOOL SaveToImage(CBitmap& bmImage, const CSize& reqSize);
	CSize CalcRequiredSizeForImage() const;

	HTREEITEM FindTask(DWORD dwTaskID) const;
	HTREEITEM FindTask(const CPoint& ptScreen) const;
	HTREEITEM FindTask(const IUISELECTTASK& select, BOOL bNext, HTREEITEM htiStart = NULL) const;

	DWORD GetTaskID(HTREEITEM hti) const { return GetItemData(hti); }
	BOOL DeleteTask(DWORD dwTaskID);

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

	void SetTextColorIsBackground(BOOL bSet = TRUE);
	void SetShowTaskColorAsBar(BOOL bSet = TRUE);
	void SetStrikeThruDoneTasks(BOOL bSet = TRUE);
	void SetColorTaskBarByPriority(BOOL bSet = TRUE);
	void SetShowCompletionCheckboxes(BOOL bShow = TRUE);
	void SetIndentSubtasks(BOOL bIndent = TRUE);
	void SetHideEmptyAttributes(BOOL bHide = TRUE);

	void OnDisplayAttributeChanged();
	int CalcAvailableAttributeWidth(int nListWidth = -1) const;
	BOOL SelectionHasLockedTasks() const;
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);
	void RefreshItemLineHeights();
	void RefreshItemLineHeights(DWORD dwTaskID);

	bool FilterToolTipMessage(MSG* pMsg) { m_tooltip.FilterToolTipMessage(pMsg); return false; }

	const CTreeCtrlHelper& TCH() const { return m_tch; }
	CTreeCtrlHelper& TCH() { return m_tch; }

	static CString FormatAttribute(IUI_ATTRIBUTE nAttrib, const CString& sValue, KBC_ATTRIBLABELS nLabelVis);
	static BOOL CanDrag(const CKanbanListCtrl* pSrcList, const CKanbanListCtrl* pDestList);

protected:
	BOOL m_bTextColorIsBkgnd;
	BOOL m_bSelected;
	BOOL m_bShowTaskColorAsBar;
	BOOL m_bColorBarByPriority;
	BOOL m_bStrikeThruDoneTasks;
	BOOL m_bSavingToImage;
	BOOL m_bShowCompletionCheckboxes;
	BOOL m_bIndentSubtasks;
	BOOL m_bHideEmptyAttributes;
	BOOL m_bDropTarget;
	BOOL m_bDrawTaskFlags;

	const CKanbanItemMap& m_data;
	CFontCache& m_fonts;
	const CDWordArray& m_aPriorityColors;
	const CKanbanAttributeArray& m_aDisplayAttrib;

	CImageList m_ilCheckboxes, m_ilFlags;
	CToolTipCtrlEx m_tooltip;
	CTreeCtrlHelper m_tch;

	KANBANCOLUMN m_columnDef;
	DWORD m_dwDisplay;
	int m_nItemTextHeight, m_nItemTextBorder;
	KBC_ATTRIBLABELS m_nAttribLabelVisiability;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKanbanListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKanbanListCtrlEx)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnThemeChanged(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg void OnTooltipShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnToolHitTest(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()

protected:
	const KANBANITEM* GetKanbanItem(DWORD dwTaskID) const;
	int CalcItemTitleTextHeight() const;
	void RecalcItemLineHeight();
	void RefreshBkgndColor();
	BOOL HandleLButtonClick(CPoint point, BOOL bDblClk);
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

	void DrawItemCheckbox(CDC* pDC, const KANBANITEM* pKI, CRect& rItem);
	void DrawItemIcons(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const;
	void DrawItemBar(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const;
	void DrawAttribute(CDC* pDC, CRect& rLine, IUI_ATTRIBUTE nAttrib, const CString& sValue, int nFlags) const;
	void FillItemBackground(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText, BOOL bSelected) const;
	void DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);
	void DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);

	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static UINT GetDisplayFormat(IUI_ATTRIBUTE nAttrib, BOOL bLong);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANLISTCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
