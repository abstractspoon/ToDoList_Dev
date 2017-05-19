#if !defined(AFX_KANBANLISTCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
#define AFX_KANBANLISTCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "Kanbanstruct.h"

#include "..\Shared\EnHeaderCtrl.h"
#include "..\Shared\fontcache.h"

/////////////////////////////////////////////////////////////////////////////

#define KLCN_CHECKCHANGE (WM_APP+1) // WPARAM = HWND, LPARAM = Task ID

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

	BOOL Create(UINT nID, CWnd* pParentWnd);
	int AddTask(LPCTSTR szTitle, DWORD dwTaskID, BOOL bSelect);
	void RefreshColumnTitle();
	void SetDoneTaskAttributes(COLORREF color, BOOL bStrikeThru);
	void Sort(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent);
	
	BOOL SaveToImage(CBitmap& bmImage, int nColWidth);
	int CalcRequiredColumnWidthForImage() const;

	int FindTask(DWORD dwItemID) const;
	int FindTask(const CPoint& ptScreen) const;

	int GetSelectedTasks(CDWordArray& aTaskIDs) const;
	BOOL SelectTasks(const CDWordArray& aTaskIDs);
	BOOL SelectTask(DWORD dwTaskID);
	BOOL IsSelectingTask() const { return (m_dwSelectingTask != 0); }
	void ScrollToSelection();
	BOOL SelectItem(int nItem, BOOL bFocus = FALSE);
	BOOL GetLabelEditRect(LPRECT pEdit);

	void SetBackgroundColor(COLORREF color);
	void SetExcessColor(COLORREF color);
	void SetMaximumTaskCount(int nMaxTasks);

	void SetTextColorIsBackground(BOOL bSet = TRUE);
	void SetShowTaskColorAsBar(BOOL bSet = TRUE);
	void SetStrikeThruDoneTasks(BOOL bSet = TRUE);
	void SetColorTasksByPriority(BOOL bSet = TRUE);
	void SetDrawAttributeLabels(BOOL bDraw = TRUE);
	void SetSelected(BOOL bSelected);
	void SetShowCompletionCheckboxes(BOOL bShow = TRUE);

	void OnDisplayAttributeChanged();
	int CalcAvailableAttributeWidth(int nListWidth = -1) const;
	BOOL SelectionHasLockedTasks() const;

	static BOOL IsSelectionChange(NMLISTVIEW* pNMLV);
	static CString FormatAttribute(IUI_ATTRIBUTE nAttrib, const CString& sValue, BOOL bWithLabel = TRUE);

protected:
	BOOL m_bTextColorIsBkgnd;
	BOOL m_bSelected;
	BOOL m_bShowTaskColorAsBar;
	BOOL m_bColorByPriority;
	BOOL m_bStrikeThruDoneTasks;
	BOOL m_bDrawAttribLabels;
	BOOL m_bSavingToImage;
	BOOL m_bShowCompletionCheckboxes;

	const CKanbanItemMap& m_data;
	CFontCache& m_fonts;
	const CDWordArray& m_aPriorityColors;
	const CKanbanAttributeArray& m_aDisplayAttrib;

	CEnHeaderCtrl m_header;
	CImageList m_ilHeight, m_ilCheckboxes;

	KANBANCOLUMN m_columnDef;
	DWORD m_dwDisplay;
	int m_nLineHeight;
	DWORD m_dwSelectingTask;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKanbanListCtrl();

protected:

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
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	const KANBANITEM* GetKanbanItem(DWORD dwTaskID) const;
	int CalcRequiredItemHeight(int nNumLines = -1) const;
	int CalcItemTitleHeight() const;
	int CalcLineHeight() const;
	BOOL NeedVScrollbar() const;
	void RefreshBkgndColor();
	BOOL HandleLButtonClick(CPoint point);
	BOOL GetItemCheckboxRect(int nItem, CRect& rItem);
	BOOL GetItemCheckboxRect(CRect& rItem);

	void DrawItemCheckbox(CDC* pDC, const KANBANITEM* pKI, CRect& rItem);
	void DrawAttribute(CDC* pDC, CRect& rLine, IUI_ATTRIBUTE nAttrib, const CString& sValue, int nFlags) const;

	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static UINT GetDisplayFormat(IUI_ATTRIBUTE nAttrib);

};

/////////////////////////////////////////////////////////////////////////////

class CKanbanListCtrlArray : public CArray<CKanbanListCtrl*, CKanbanListCtrl*&>
{
public:
	virtual ~CKanbanListCtrlArray();
	
	void RemoveAll();
	BOOL RemoveAt(int nList);
	int FindListCtrl(DWORD dwTaskID) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANLISTCTRL_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
