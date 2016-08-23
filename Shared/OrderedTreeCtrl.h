#if !defined(AFX_ORDEREDTREECTRL_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_)
#define AFX_ORDEREDTREECTRL_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrl.h : header file
//

#include "..\shared\ncgutter.h"
#include "..\shared\treectrlhelper.h"

#include <afxtempl.h>
typedef CMap<UINT, UINT, CString, CString&> CMapUIntToString;

/////////////////////////////////////////////////////////////////////////////
// COrderedTreeCtrl window

#define OTC_GRIDCOLOR ::GetSysColor(COLOR_3DFACE)
#define OTC_POSCOLUMNID (NCG_CLIENTCOLUMNID - 1)

class COrderedTreeCtrl : public CTreeCtrl
{
// Construction
public:
	COrderedTreeCtrl(DWORD dwGutterStyles = NCGS_SHOWHEADER);
	virtual ~COrderedTreeCtrl();

	BOOL AddGutterColumn(UINT nColID, LPCTSTR szTitle = NULL, UINT nWidth = 0, UINT nTextAlign = DT_LEFT);
	BOOL InsertGutterColumn(int nPos, UINT nColID, LPCTSTR szTitle = NULL, UINT nWidth = 0, UINT nTextAlign = DT_LEFT);
	void RemoveGutterColumn(UINT nColID);
	void PressGutterColumnHeader(UINT nColID, BOOL bPress = TRUE);
	void SetGutterColumnHeaderTitle(UINT nColID, LPCTSTR szTitle, LPCTSTR szFont = NULL);
	void SetGutterColumnHeaderTitle(UINT nColID, UINT nSymbol, LPCTSTR szFont = NULL);
	void SetGutterColumnHeaderTip(UINT nColID, LPCTSTR szTip);
	void EnableGutterColumnHeaderClicking(UINT nColID, BOOL bEnable = TRUE);
	void SetGutterColumnSort(UINT nColID, NCGSORT nSortDir);
	NCGSORT GetGutterColumnSort(UINT nColID) const;
	int GetColumnWidth(UINT nColID) const { return m_gutter.GetColumnWidth(nColID); }
	void SetColumnTextAlignment(UINT nColID, UINT nTextAlign, BOOL bRedraw = TRUE);
	UINT GetColumnTextAlignment(UINT nColID) const;

	BOOL ShowGutterPosColumn(BOOL bShow = TRUE); // returns TRUE if changed
	BOOL IsGutterPosColumnShowing() const { return m_bShowingPosColumn; }
	void SetGridlineColor(COLORREF color);
    COLORREF GetGridlineColor() const { return m_crGridlines; }

	void EnableGutterStyle(DWORD dwStyle, BOOL bEnable = TRUE) { m_gutter.EnableStyle(dwStyle, bEnable); }
	BOOL GutterHasStyle(DWORD dwStyle) const { return m_gutter.HasStyle(dwStyle); }
	
	void RedrawGutter() { m_gutter.Redraw(); }
	void RedrawGutterItem(DWORD dwItem) { m_gutter.RedrawItem(dwItem); }
	BOOL RecalcGutterColumn(UINT nColID) { return m_gutter.RecalcColumn(nColID); }
	int GetGutterWidth() const { return m_gutter.GetGutterWidth(); }
	void SetHeaderHeight(int nHeight) { m_gutter.SetHeaderHeight(nHeight); }
	void RecalcGutter(BOOL bForceRedraw = TRUE);

	COLORREF GetAlternateLineColor() const { return m_crAltLines; }
	void SetAlternateLineColor(COLORREF color);
	COLORREF GetItemLineColor(HTREEITEM hti);

	BOOL PtInHeader(const CPoint& ptScreen) const;
	UINT ColumnHitTest(const CPoint& ptScreen) const;

	CTreeCtrlHelper& TCH() { return m_ht; }
	const CTreeCtrlHelper& TCH() const { return m_ht; }

	CString FormatPosition(DWORD dwItem, DWORD dwParentItem, int nLevel = -1, int nPos = -1);

protected:
	CNcGutter m_gutter;
	BOOL m_bShowingPosColumn;
	COLORREF m_crGridlines, m_crAltLines;
	BOOL m_bWantInit;
	CTreeCtrlHelper m_ht;
	CMapUIntToString m_mapTooltips;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderedTreeCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(COrderedTreeCtrl)
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG
	afx_msg BOOL OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpSS);
	afx_msg BOOL OnClick(NMHDR* pNMHDR, LRESULT* pResult);

	// callbacks for gutter
	afx_msg LRESULT OnGutterGetFirstVisibleTopLevelItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetNextItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetFirstChildItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterDrawItemColumn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterPreDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterPostDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterPostNcDraw(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterRecalcColWidth(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetItemRect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetItemHeight(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterNotifyItemClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetParentID(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterWantRecalc(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterWantRedraw(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterGetItemColors(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterNotifyWidthChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	void NcDrawItemColumn(CDC* pDC, DWORD dwItem, DWORD dwParentItem, UINT nColID, CRect& rItem, 
					int nLevel, int nPos, const CRect& rWindow);
	void PostNcDrawItem(CDC* pDC, DWORD dwItem, const CRect& rItem, int nLevel);
	void PostNcDraw(CDC* pDC, const CRect& rWindow);

	BOOL RecalcColumnWidth(CDC* pDC, UINT nColID, UINT& nWidth);
	UINT GetGutterWidth(HTREEITEM hti, int nLevel, int nPos, CDC* pDC);
	static UINT GetWidth(int nNumber, CDC* pDC); // includes a trailing '.'

	void UpdateHeaderTooltipRects();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDEREDTREECTRL_H__7E73ADE2_3848_4ED1_9E8B_8881813B4262__INCLUDED_)
