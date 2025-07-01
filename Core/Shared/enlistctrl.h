// EnListCtrl.h : header file
//
#if !defined ( ENLISTCTRL_H )
#define ENLISTCTRL_H 

#include "enheaderctrl.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CEnListCtrl window

#define LVN_USERSELCHANGE			(LVN_LAST+1)
#define LVN_COPY					(LVN_LAST+2)
#define LVN_CUT						(LVN_LAST+3)
#define LVN_PASTE					(LVN_LAST+4)
#define LVN_DELETE					(LVN_LAST+5)
#define LVN_USERSELCHANGEDBLCLK		(LVN_LAST+6)
#define LVN_CHECKCHANGE				(LVN_LAST+7)

/////////////////////////////////////////////////////////////////////////////

// helper class for manipulating item grouping
class CListCtrlItemGrouping
{
public:
	CListCtrlItemGrouping(HWND hwndList = NULL);

	BOOL IsEnabled() const { return m_bEnabled; }
	BOOL EnableGroupView(BOOL bEnable = TRUE);
	BOOL EnableGroupView(HWND hwndList, BOOL bEnable = TRUE);
	BOOL InsertGroupHeader(int nIndex, int nGroupID, const CString& strHeader);
	BOOL SetItemGroupId(int nRow, int nGroupID);
	void RemoveAllGroups();

	BOOL HasGroups() const;
	int GetItemGroupId(int nRow) const;
	CString GetGroupHeaderText(int nGroupID) const;

	void SetGroupHeaderBackColor(COLORREF crBack);
	COLORREF GetGroupHeaderBackColor() const { return m_crBkgnd; }

	static BOOL IsSupported();

protected:
	HWND m_hwndList;
	BOOL m_bEnabled;
	COLORREF m_crBkgnd;
};

/////////////////////////////////////////////////////////////////////////////

class CEnListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEnListCtrl)

public:
	CEnListCtrl();
	virtual ~CEnListCtrl();

	CEnHeaderCtrl* GetHeader();
	const CEnHeaderCtrl* GetHeader() const;

	CListCtrlItemGrouping& GetGrouping() { return m_grouping; }
	const CListCtrlItemGrouping& GetGrouping() const { return m_grouping; }

	void EnableHeaderTracking(BOOL bAllow = TRUE);
	BOOL EnableGroupView(BOOL bEnable = TRUE);
	void EnableTooltipCtrl(BOOL bEnable = TRUE);
	void EnableSorting(BOOL bEnable = TRUE) { m_bSortingEnabled = bEnable; }
	void EnableAlternateRowColoring(BOOL bEnable = TRUE);
	void AllowOffItemClickDeselection(BOOL bAllow = TRUE) { m_bAllowOffItemClickDeslection = bAllow; }

	void SetMulSel(int nIndexStart, int nIndexEnd, BOOL bSelect = TRUE, BOOL bNotifyParent = FALSE); // multiple selection
	void SetItemFocus(int nIndex, BOOL bFocused); 
	int GetFirstSel() const;
	int GetNextSel(int nPrevSel) const;
	int GetLastSel() const;
	int GetPrevSel(int nNextSel) const;
	int GetCountPerPage() const;
	void SelectAll();
	void ClearAll();
	BOOL DeleteAllItems();
	void ShowGrid(BOOL bVert, BOOL bHorz);
	void IsShowingGrid(BOOL& bVert, BOOL& bHorz) const { bVert = m_bVertGrid; bHorz = m_bHorzGrid; }
	void SetView(int nView);
	int GetView() const { return m_nCurView; }
	int GetFocusedItem() const;
	int FindItemFromData(DWORD dwItemData) const;
	int FindItemFromLabel(CString sLabel, BOOL bExact = TRUE, int nFromIndex = 0) const;
	BOOL SetTooltipCtrlText(CString sText);
	BOOL SetMinItemHeight(int nHeight);
	int GetMinItemHeight() const { return m_nMinItemHeight; }
	int GetItemHeight() const { return m_nItemHeight; }
	void SetReadOnly(BOOL bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL IsReadOnly() const { return m_bReadOnly; }
	BOOL SelectDropTarget(int nItem);
	BOOL SetItemImage(int nItem, int nImage);
	BOOL SetItemImage(int nItem, int nSubItem, int nImage);
	int GetItemImage(int nItem, int nSubItem = 0) const;
	BOOL IsItemSelected(int nItem) const;
	void SetSortAscending(BOOL bAscending) { m_bSortAscending = bAscending; }
	BOOL GetSortAscending() const { return m_bSortAscending; }
	void SetSortEmptyValuesBelow(BOOL bBelow) { m_bSortEmptyBelow = bBelow; }
	void SetItemIndent(int nItem, int nIndent);

	virtual int SetCurSel(int nIndex, bool bNotifyParent = FALSE); // single selection
	virtual int GetCurSel() const;
	virtual void Sort();
	virtual int GetItemIndent(int nItem) const;
	
	// column methods
	int GetColumnCount() const;
	int GetSortColumn() const { return m_nSortColumn; }
	BOOL IsSorting() const { return (m_nSortColumn != -1); }
	void SetSortColumn(int nCol, BOOL bResort = TRUE);
	COLORREF GetColumnTextColor(int nCol) const;
	void SetColumnTextColor(int nCol, COLORREF color);
	void SetColumnFormat(int nCol, int nFormat);
	int GetColumnFormat(int nCol) const;
	BOOL SetColumnText(int nCol, LPCTSTR szText);
	void DeleteAllColumns();
	void SetLastColumnStretchy(BOOL bStretchy);
	void SetFirstColumnStretchy(BOOL bStretchy);
	void OverrideSelectionTheming(BOOL bThemed, BOOL bClassic) { m_dwSelectionTheming = MAKELONG(bThemed, bClassic); }

	static BOOL IsSelectionChange(NMLISTVIEW* pNMLV, int* pItem = NULL);
	static void EnableSelectionTheming(BOOL bThemed, BOOL bClassic) { s_dwSelectionTheming = MAKELONG(bThemed, bClassic); }

#if _MSC_VER >= 1300
protected:
#endif
	// helper class for extending column data
	class CColumnData
	{
	public:
		CColumnData();
		COLORREF crText;
		int nFormat;
	};

// Attributes
protected:
	CEnHeaderCtrl m_header;

	BOOL m_bVertGrid, m_bHorzGrid;
	int m_nCurView;
	NMHDR m_nmhdr; // for notification
	BOOL m_bLastColStretchy;
	BOOL m_bFirstColStretchy;
	BOOL m_bTooltipsEnabled;
	CToolTipCtrl m_ttCtrl;
	BOOL m_bReadOnly;
	int m_nItemDropHilite;
	BOOL m_bDropHiliteItemSelected;
	BOOL m_bContextPopupEnabled;
	BOOL m_bDrawCheckboxes;
	CPoint m_ptPopupPos;
	BOOL m_bUserSelChange;
	BOOL m_bSortingEnabled;
	int m_nSortColumn;
	BOOL m_bSortAscending;
	BOOL m_bInitColumns; // up to derived class to set: gets cleared in OnDestroy
	BOOL m_bAlternateRowColoring;
	BOOL m_bAllowOffItemClickDeslection;
	BOOL m_bSortEmptyBelow;
	BOOL m_bResizingCols;

private:
	CMap<int, int, CColumnData*, CColumnData*> m_mapColumnData; 
	CMap<DWORD, DWORD, CString, CString&> m_mapSortStrings; 
	
	int m_nMinItemHeight, m_nItemHeight;
	DWORD m_dwSelectionTheming;
	CListCtrlItemGrouping m_grouping;

	static DWORD s_dwSelectionTheming;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnListCtrl)
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	//}}AFX_MSG
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LPARAM* lResult);
	afx_msg void OnHeaderCustomDraw(NMHDR* pNMHDR, LPARAM* lResult);
	afx_msg BOOL OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	// helpers
	virtual void Copy() {} // derived class override provides logic
	virtual void Delete() {} // derived class override provides logic
	virtual void Cut() {} // derived class override provides logic
	virtual void Paste() {} // derived class override provides logic
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn) const;
	virtual COLORREF GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual CFont* GetItemFont(int /*nItem*/, int /*nSubItem*/) const { return NULL; }
	virtual CString GetNoItemsText() const { return ""; }
	virtual CColumnData* GetNewColumnData() const { return new CColumnData; }
	virtual void GetCellRect(int nRow, int nCol, CRect& rCell) const;
	virtual void GetCellEditRect(int nRow, int nCol, CRect& rCell) const;
	virtual void DrawItemBackground(CDC* pDC, int nItem, const CRect& rItem, COLORREF crBack, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);
	virtual void DrawCellBackground(CDC* pDC, int nItem, int nCol, const CRect& rCell, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);
	virtual void DrawCellText(CDC* pDC, int nItem, int nCol, const CRect& rText, const CString& sText, COLORREF crText, UINT nDrawTextFlags);
	virtual void DrawCell(CDC* pDC, int nItem, int nCol, const CRect& rCell, const CString& sText, BOOL bSelected, BOOL bDropHighlighted, BOOL bFocused);
	virtual void DrawGroupHeader(CDC* pDC, CRect& rRow, const CString& sText, COLORREF crBack);
	virtual UINT GetTextDrawFlags(int nCol, BOOL bDate = FALSE) const;

	void NotifySelChange();
	void DeleteAllColumnData();
	CColumnData* CreateColumnData(int nCol);
	const CColumnData* GetColumnData(int nCol) const;
	CString GetSortText(DWORD dwItemData) const;
	void ResizeStretchyColumns();
	CString GetSortString(DWORD dwItemData) const;
	BOOL BuildSortMap(int nCol, CMap<DWORD, DWORD, CString, CString&>& mapSortStrings) const;
	BOOL IsSelectionThemed(BOOL bClassic) const;
	BOOL WantSelChange(int nSel) const;
	int CompareEmptiness(BOOL bItem1Empty, BOOL bItem2Empty) const;
	void ForceResize(BOOL bRecalcItemHeight);

private:
	BOOL BuildSortMap(int nCol);

	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam);
};

#endif

/////////////////////////////////////////////////////////////////////////////
/////////
