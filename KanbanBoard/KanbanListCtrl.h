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

#include "..\Shared\EnHeaderCtrl.h"
#include "..\Shared\fontcache.h"
#include "..\Shared\tooltipctrlex.h"
#include "..\Shared\mapex.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_KLCN_CHECKCHANGE = (WM_APP+1); // WPARAM = HWND, LPARAM = Task ID
const UINT WM_KLCN_GETTASKICON = (WM_APP+2); // WPARAM = TaskID, LPARAM = int* (imageIndex), return HIMAGELIST

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrl window

class CKanbanListCtrl : public CListCtrl
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

	BOOL Create(UINT nID, CWnd* pParentWnd);
	int AddTask(const KANBANITEM& ki, BOOL bSelect);
	void RefreshColumnTitle();
	void SetDoneTaskAttributes(COLORREF color, BOOL bStrikeThru);
	void Sort(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent);
	
	BOOL SaveToImage(CBitmap& bmImage, int nColWidth);
	int CalcRequiredColumnWidthForImage() const;

	int FindTask(DWORD dwTaskID) const;
	int FindTask(const CPoint& ptScreen) const;
	int FindTask(const IUISELECTTASK& select, BOOL bNext, int nStart = 0) const;

	DWORD GetTaskID(int nItem) const { return GetItemData(nItem); }
	BOOL DeleteTask(DWORD dwTaskID);

	int GetSelectedTasks(CDWordArray& aTaskIDs) const;
	BOOL SelectTasks(const CDWordArray& aTaskIDs);
	BOOL SelectTask(DWORD dwTaskID, BOOL bAppend = FALSE);
	BOOL SelectItem(int nItem, BOOL bFocus, BOOL bAppend = FALSE);
	BOOL IsSelectingTask() const { return (m_dwSelectingTask != 0); }
	void ScrollToSelection();
	BOOL GetLabelEditRect(LPRECT pEdit);
	void ClearSelection();
	void SetSelected(BOOL bSelected);
	int GetFirstSelectedItem() const;
	int GetLastSelectedItem() const;

	void SetBackgroundColor(COLORREF color);
	void SetExcessColor(COLORREF color);
	void SetMaximumTaskCount(int nMaxTasks);

	void SetTextColorIsBackground(BOOL bSet = TRUE);
	void SetShowTaskColorAsBar(BOOL bSet = TRUE);
	void SetStrikeThruDoneTasks(BOOL bSet = TRUE);
	void SetColorTaskBarByPriority(BOOL bSet = TRUE);
	void SetShowCompletionCheckboxes(BOOL bShow = TRUE);

	void OnDisplayAttributeChanged();
	int CalcAvailableAttributeWidth(int nListWidth = -1) const;
	BOOL SelectionHasLockedTasks() const;
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);

	bool FilterToolTipMessage(MSG* pMsg) { m_tooltip.FilterToolTipMessage(pMsg); return false; }

	static BOOL IsSelectionChange(NMLISTVIEW* pNMLV);
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

	const CKanbanItemMap& m_data;
	CFontCache& m_fonts;
	const CDWordArray& m_aPriorityColors;
	const CKanbanAttributeArray& m_aDisplayAttrib;

	CEnHeaderCtrl m_header;
	CImageList m_ilHeight, m_ilCheckboxes, m_ilFlags;
	CToolTipCtrlEx m_tooltip;

	KANBANCOLUMN m_columnDef;
	DWORD m_dwDisplay;
	int m_nLineHeight;
	DWORD m_dwSelectingTask;
	KBC_ATTRIBLABELS m_nAttribLabelVisiability;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKanbanListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKanbanListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnThemeChanged(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg void OnTooltipShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnToolHitTest(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	const KANBANITEM* GetKanbanItem(DWORD dwTaskID) const;
	int CalcRequiredItemHeight(int nNumLines = -1) const;
	int CalcItemTitleTextHeight() const;
	int CalcLineHeight() const;
	BOOL NeedVScrollbar() const;
	void RefreshBkgndColor();
	BOOL HandleLButtonClick(CPoint point);
	BOOL GetItemCheckboxRect(int nItem, CRect& rItem) const;
	BOOL GetItemCheckboxRect(CRect& rItem) const;
	BOOL GetItemLabelTextRect(int nItem, CRect& rItem, BOOL bEdit = FALSE) const;
	BOOL InitTooltip();
	BOOL GetItemTooltipRect(int nItem, CRect& rItem) const;

	BOOL DrawItemCheckbox(CDC* pDC, const KANBANITEM* pKI, CRect& rItem);
	BOOL DrawItemIcons(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem) const;
	BOOL DrawItemBar(CDC* pDC, const KANBANITEM* pKI, BOOL bHasIcon, CRect& rItem) const;
	void DrawAttribute(CDC* pDC, CRect& rLine, IUI_ATTRIBUTE nAttrib, const CString& sValue, int nFlags) const;
	void FillItemBackground(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText, BOOL bSelected) const;
	void DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);
	void DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText);

	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static UINT GetDisplayFormat(IUI_ATTRIBUTE nAttrib, BOOL bLong);

};

/////////////////////////////////////////////////////////////////////////////

class CKanbanListCtrlArray : public CArray<CKanbanListCtrl*, CKanbanListCtrl*&>
{
public:
	virtual ~CKanbanListCtrlArray();
	
	void RemoveAll();
	BOOL RemoveAt(int nList);

	int Find(DWORD dwTaskID) const;
	int Find(DWORD dwTaskID, int& nItem) const;
	int Find(HWND hWnd) const;
	int Find(const CString& sAttribValue) const;
	int Find(const CKanbanListCtrl* pList) const;

	CKanbanListCtrl* Get(DWORD dwTaskID) const;
	CKanbanListCtrl* Get(DWORD dwTaskID, int& nItem) const;
	CKanbanListCtrl* Get(HWND hWnd) const;
	CKanbanListCtrl* Get(const CString& sAttribValue) const;
	CKanbanListCtrl* GetFirstNonEmpty() const;
	CKanbanListCtrl* GetLastNonEmpty() const;
	CKanbanListCtrl* GetBacklog() const;

	void SetTextColorIsBackground(BOOL bSet = TRUE);
	void SetShowTaskColorAsBar(BOOL bSet = TRUE);
	void SetStrikeThruDoneTasks(BOOL bSet = TRUE);
	void SetColorTaskBarByPriority(BOOL bSet = TRUE);
	void SetShowCompletionCheckboxes(BOOL bShow = TRUE);
	
	int GetVisibleCount(BOOL bIncBacklog) const;
	int GetVisibleTaskCount() const;
	float GetAverageCharWidth();
	DWORD HitTestTask(const CPoint& ptScreen) const;
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);
	
	int CalcRequiredColumnWidthForImage() const;
	BOOL CanSaveToImage() const;
	BOOL SaveToImage(CBitmap& bmImage);

	CKanbanListCtrl* GetNext(const CKanbanListCtrl* pList, BOOL bNext, BOOL bExcludeEmpty, BOOL bFixedColumns) const;
	CKanbanListCtrl* HitTest(const CPoint& ptScreen, BOOL* pbHeader = NULL) const;

	void OnDisplayAttributeChanged();
	void OnSetFont(HFONT hFont);

	void Sort();
	void SortItems(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent);

	void Exclude(CDC* pDC);
	void ClearOtherSelections(const CKanbanListCtrl* pIgnore);
	void Redraw(BOOL bErase);
	void RemoveDeletedTasks(const CDWordSet& mapCurIDs);
	void RefreshColumnTitles();
	void DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanListCtrl* pIgnore);

protected:
	static int ListSortProc(const void* pV1, const void* pV2);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANLISTCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
