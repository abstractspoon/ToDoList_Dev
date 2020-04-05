// EnListCtrl.h : header file
//
#if !defined ( ENLISTCTRL_H )
#define ENLISTCTRL_H 

#include "enheaderctrl.h"
#include "enstring.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CEnListCtrl window

#define LVN_USERSELCHANGE			(LVN_FIRST-21)
#define LVN_COPY					(LVN_FIRST-22)
#define LVN_CUT						(LVN_FIRST-23)
#define LVN_PASTE					(LVN_FIRST-24)
#define LVN_DELETE					(LVN_FIRST-25)
#define LVN_USERSELCHANGEDBLCLK		(LVN_FIRST-26)
#define LVN_CHECKCHANGE				(LVN_FIRST-40)

/////////////////////////////////////////////////////////////////////////////

// helper class for extending column data
class CColumnData
{
public:
	CColumnData() { crText = ::GetSysColor(COLOR_WINDOWTEXT); nFormat = ES_END; }
	COLORREF crText;
	int nFormat;
};

/////////////////////////////////////////////////////////////////////////////

class CEnListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEnListCtrl)

		// Construction
public:
	CEnListCtrl();

public:
	CEnHeaderCtrl* GetHeader();
	const CEnHeaderCtrl* GetHeader() const;
	virtual int SetCurSel(int nIndex, bool bNotifyParent = FALSE); // single selection
	virtual int GetCurSel() const;
	void SetMulSel(int nIndexStart, int nIndexEnd, BOOL bSelect = TRUE, BOOL bNotifyParent = FALSE); // multiple selection
	void SetItemFocus(int nIndex, BOOL bFocused); 
	int GetFirstSel() const;
	int GetNextSel(int nPrevSel) const;
	int GetLastSel() const;
	int GetPrevSel(int nNextSel) const;
	int GetCountPerPage() const;
	void SelectAll();
	void ClearAll();
	void EnableHeaderTracking(BOOL bAllow);
	void ShowGrid(BOOL bVert, BOOL bHorz);
	void IsShowingGrid(BOOL& bVert, BOOL& bHorz) const { bVert = m_bVertGrid; bHorz = m_bHorzGrid; }
	void SetView(int nView);
	int GetView() const { return m_nCurView; }
	int GetFocusedItem() const;
	void SetLastColumnStretchy(BOOL bStretchy);
	void SetFirstColumnStretchy(BOOL bStretchy);
	int FindItemFromData(DWORD dwItemData) const;
	int FindItemFromLabel(CString sLabel, BOOL bExact = TRUE, int nFromIndex = 0) const;
	void EnableTooltipCtrl(BOOL bEnable);
	BOOL SetTooltipCtrlText(CString sText);
	BOOL SetMinItemHeight(int nHeight);
	int GetMinItemHeight() const { return m_nMinItemHeight; }
	void DeleteAllColumns();
	void SetReadOnly(BOOL bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL IsReadOnly() const { return m_bReadOnly; }
	BOOL SelectDropTarget(int nItem);
	void SetItemImage(int nItem, int nImage);
	BOOL IsItemSelected(int nItem) const;
	void SetSortAscending(BOOL bAscending) { m_bSortAscending = bAscending; }
	BOOL GetSortAscending() const { return m_bSortAscending; }
	virtual void Sort();
	void EnableSorting(BOOL bEnable) { m_bSortingEnabled = bEnable; }
	void SetItemIndent(int nItem, int nIndent);
	int GetItemIndent(int nItem) const;
	void EnableAlternateRowColoring(BOOL bEnable = TRUE);
	
	// column methods
	int GetColumnCount() const;
	int GetSortColumn() const { return m_nSortColumn; }
	void SetSortColumn(int nColumn, BOOL bResort = TRUE);
	COLORREF GetColumnTextColor(int nCol) const;
	void SetColumnTextColor(int nCol, COLORREF color);
	void SetColumnFormat(int nCol, int nFormat);
	int GetColumnFormat(int nCol) const;
	BOOL SetColumnText(int nCol, LPCTSTR szText);

	// group methods
	BOOL EnableGroupView(BOOL bEnable = TRUE);
	BOOL InsertGroupHeader(int nIndex, int nGroupID, const CString& strHeader/*, DWORD dwState = LVGS_NORMAL, DWORD dwAlign = LVGA_HEADER_LEFT*/);
	int GetItemGroupId(int nRow);
	BOOL SetItemGroupId(int nRow, int nGroupID);
	void RemoveAllGroups();
	
	void OverrideSelectionTheming(BOOL bThemed, BOOL bClassic) { m_dwSelectionTheming = MAKELONG(bThemed, bClassic); }
	static void EnableSelectionTheming(BOOL bThemed, BOOL bClassic) { s_dwSelectionTheming = MAKELONG(bThemed, bClassic); }

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

private:
	CMap<int, int, CColumnData*, CColumnData*> m_mapColumnData; 
	CMap<DWORD, DWORD, CString, CString&> m_mapSortStrings; 
	
	int m_nMinItemHeight;
	DWORD m_dwSelectionTheming;
	static DWORD s_dwSelectionTheming;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnListCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
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
	DECLARE_MESSAGE_MAP()

	// helpers
	virtual void Copy() {} // derived class override provides logic
	virtual void Delete() {} // derived class override provides logic
	virtual void Cut() {} // derived class override provides logic
	virtual void Paste() {} // derived class override provides logic
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn);
	virtual COLORREF GetItemTextColor(int nItem, int nSubItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemBackColor(int nItem, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual CFont* GetItemFont(int /*nItem*/, int /*nSubItem*/) { return NULL; }
	virtual CString GetNoItemsText() const { return ""; }
	virtual CColumnData* GetNewColumnData() const { return new CColumnData; }
	virtual void GetCellRect(int nRow, int nCol, CRect& rCell) const;
	virtual void GetCellEditRect(int nRow, int nCol, CRect& rCell) const;

	int GetImageStyle(BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	int GetImageIndex(int nItem, int nSubItem) const;
	void NotifySelChange();
	void DeleteAllColumnData();
	CColumnData* CreateColumnData(int nCol);
	const CColumnData* GetColumnData(int nCol) const;
	CString GetSortText(DWORD dwItemData) const;
	void RefreshItemHeight();
	void ResizeStretchyColumns();
	CString GetSortString(DWORD dwItemData) const;
	void BuildSortMap(int nCol, CMap<DWORD, DWORD, CString, CString&>& mapSortStrings) const;
	BOOL IsSelectionThemed(BOOL bClassic) const;

private:
	void BuildSortMap(int nCol);
	int CalcItemHeight() const;
	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam);
};

#endif

/////////////////////////////////////////////////////////////////////////////
/////////
