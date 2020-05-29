//_ **********************************************************
//_ 
//_ Name: EnHeaderCtrl.h 
//_ Purpose: 
//_ Created: 15 September 1998 
//_ Author: D.R.Godson
//_ Modified By: 
//_ 
//_ Copyright (c) 1998 Brilliant Digital Entertainment Inc. 
//_ 
//_ **********************************************************

// EnHeaderCtrl.h : header file
//
#if !defined ( ENHEADERCTRL_H )
#define ENHEADERCTRL_H 

#include "ToolTipCtrlEx.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

enum EHC_SORT
{
	EHCS_UP   = -1,
	EHCS_NONE =  0,
	EHCS_DOWN =  1,
};

/////////////////////////////////////////////////////////////////////////////

typedef CArray<int, int> CIntArray;

/////////////////////////////////////////////////////////////////////////////
// CEnHeaderCtrl window

class CEnHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CEnHeaderCtrl();

	void EnableTracking(BOOL bAllow);
	void EnableItemTracking(int nItem, BOOL bAllow);
	BOOL IsItemTrackable(int nItem) const;
	BOOL IsItemTracked(int nItem) const;
	BOOL SetItemTracked(int nItem, BOOL bTracked);
	void ClearAllTracked();

	void EnableItemDragging(int nItem, BOOL bAllow);
	BOOL IsItemDraggable(int nItem) const;

	BOOL SetItemSorted(int nItem, EHC_SORT nDir);
	EHC_SORT GetItemSorted(int nItem) const;
	int GetSortedItem() const;
	void ClearAllSorted();
	BOOL DrawItemSortArrow(CDC* pDC, int nItem) const;
	BOOL DrawItemSortArrow(CDC* pDC, int nItem, BOOL bUp) const;

	void ShowItem(int nItem, BOOL bShow);
	BOOL IsItemVisible(int nItem) const;

	void SetRowCount(int nRows);
	int GetRowCount() const { return m_nRowCount; }
	BOOL GetRowRect(int nRow, CRect& rRow) const;
	BOOL RedrawRow(int nRow, BOOL bErase, BOOL bUpdate);

	BOOL SetItemWidth(int nItem, int nWidth);
	int GetItemWidth(int nItem) const;
	BOOL SetItemData(int nItem, DWORD dwData);
	DWORD GetItemData(int nItem) const;
	BOOL SetItemText(int nItem, LPCTSTR szText);
	CString GetItemText(int nItem) const;
	int GetItemTextWidth(int nItem, CDC* pDC) const;
	int GetItemFormat(int nItem) const;
	BOOL SetItemFormat(int nItem, int nFormat);
	BOOL ModifyItemFormat(int nItem, int nRemove, int nAdd);

	int GetItemOrder(CIntArray& aOrder) const;
	BOOL SetItemOrder(const CIntArray& aOrder);
	int GetItemWidths(CIntArray& aWidths) const;
	BOOL SetItemWidths(const CIntArray& aWidths);
	int GetTrackedItems(CIntArray& aTracked) const;
	BOOL SetTrackedItems(const CIntArray& aTracked);
	BOOL HasTrackedItems() const;

	int CalcTotalItemWidth(int nIgnoreItem = -1) const;
	int CalcAverageItemWidth(int nIgnoreItem = -1) const;

	int InsertItem(int nItem, int nWidth, LPCTSTR szText = _T(""), int nFormat = HDF_LEFT, UINT uIDBitmap = 0, DWORD dwItemData = 0);
	int AppendItem(int nWidth, LPCTSTR szText = _T(""), int nFormat = HDF_LEFT, UINT uIDBitmap = 0, DWORD dwItemData = 0);
	BOOL SetItem(int nItem, int nWidth, LPCTSTR szText, DWORD dwData);
	BOOL SetItem(int nItem, HDITEM* pHeaderItem);
	void DeleteAllItems();

	int HitTest(CPoint ptClient, UINT* pFlags = NULL) const;
	int FindItem(DWORD dwData) const;

	int GetFirstVisibleItem() const;
	int GetLastVisibleItem() const;
	int GetNextVisibleItem(int nItem, BOOL bNext = TRUE) const;
	int GetItemPosition(int nItem) const;

#ifdef _DEBUG
	void TraceItemWidths(BOOL bVisibleOnly, LPCTSTR szKey = NULL) const;
	void TraceItemStates(LPCTSTR szKey = NULL) const;
#endif

	BOOL EnableToolTips(BOOL bEnable = TRUE);
	BOOL SetItemToolTip(int nItem, LPCTSTR szTip);
	CString GetItemToolTip(int nItem) const;
	void FilterToolTipMessage(MSG* pMsg);

// Attributes
private:
	BOOL m_bEnableTracking;
	int m_nRowCount;
	CToolTipCtrlEx m_tooltips;

	struct ITEMEXTRA
	{
		ITEMEXTRA(DWORD flags = 0, int width = 0) : dwFlags(flags), nWidth(width) {}

		DWORD dwFlags;
		int nWidth;
		CString sTooltip;
	};
	mutable CArray<ITEMEXTRA, ITEMEXTRA&> m_aItemExtras;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnHeaderCtrl)
	//}}AFX_VIRTUAL
	BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CEnHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnHeaderCtrl)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnBeginTrackHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndTrackHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDragHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEndDragHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnLayout(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnInsertItem(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDeleteItem(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnToolHitTest(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL ItemHasFlag(int nItem, DWORD dwFlag) const;
	DWORD GetItemFlags(int nItem) const;
	BOOL ModifyItemFlags(int nItem, DWORD dwFlag, BOOL bAdd);
	BOOL ModifyAllItemFlags(DWORD dwFlag, BOOL bAdd);
	BOOL InitializeTooltips();
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	ITEMEXTRA& GetItemExtra(int nItem) const;

	static int GetItemPosition(int nItem, const CIntArray& aOrder);
};

#endif
/////////////////////////////////////////////////////////////////////////////
////
