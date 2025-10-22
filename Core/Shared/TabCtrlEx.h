#if !defined(AFX_TABCTRLEX_H__F97F9C15_5623_4A5E_ACB5_C10D4C8EB75D__INCLUDED_)
#define AFX_TABCTRLEX_H__F97F9C15_5623_4A5E_ACB5_C10D4C8EB75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tabctrlex.h : header file
//

#include "..\3rdparty\XPTabCtrl.h"
#include "..\3rdparty\DragDrop.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx window

// flag
enum
{
	TCE_POSTDRAW		= 0x0001,
	TCE_MBUTTONCLICK	= 0x0002,
	TCE_DRAGDROP		= 0x0004,
	TCE_MBUTTONCLOSE	= 0x0008,
	TCE_CLOSEBUTTON		= 0x0010,
	TCE_BOLDSELTEXT		= 0x0020,
	TCE_TABCOLORS		= 0x0040,
	TCE_TAGCOLORS		= 0x0080,

	TCE_ALL				= 0xffff
};

/////////////////////////////////////////////////////////////////////////////

#ifndef TCN_CLOSETAB
#	define TCN_CLOSETAB		(TCN_FIRST-11)
#endif

#ifndef TCN_MCLICK
#	define TCN_MCLICK		(TCN_FIRST-12)
#endif

#ifndef TCN_ENDDRAG
#	define TCN_ENDDRAG		(TCN_FIRST-13)
#endif

#ifndef TCN_GETBACKCOLOR
#	define TCN_GETBACKCOLOR	(TCN_FIRST-14)
#endif

#ifndef TCN_POSTDRAW
#	define TCN_POSTDRAW		(TCN_FIRST-15)
#endif

#ifndef TCN_GETTAGCOLOR
#	define TCN_GETTAGCOLOR	(TCN_FIRST-16)
#endif

struct NMTABCTRLEX
{
    NMHDR hdr;
    int iTab;
	DWORD dwExtra;
};

/////////////////////////////////////////////////////////////////////////////

class CTabCtrlEx : public CXPTabCtrl, protected CDragDropData
{
public:
	CTabCtrlEx(DWORD dwFlags = 0, ETabOrientation orientation = e_tabTop);
	virtual ~CTabCtrlEx();

	BOOL SetItemText(int nTab, LPCTSTR szText);
	CString GetItemText(int nTab) const;

	BOOL SetItemImage(int nTab, int nImage);
	int GetItemImage(int nTab) const;
	
	DWORD GetItemData(int nTab) const;
	BOOL SetItemData(int nTab, DWORD dwItemData);
	int FindItemByData(DWORD dwItemData) const;

	int HitTest(const CPoint& point) const;
	int HitTest(TCHITTESTINFO* pHitTestInfo) const;
	
	int GetScrollPos() const;
	BOOL SetScrollPos(int nPos);

	void EnsureSelVisible();
	void EnsureVisible(int nTab);
	BOOL ModifyFlags(DWORD dwRemove, DWORD dwAdd);
	void GetTabContentRect(const CRect& rTab, int nTab, CRect& rContent) const;

	BOOL MoveTab(int nFrom, int nTo);
	BOOL CanMoveTab(int nFrom, int nTo) const;

	static BOOL IsSupportedFlag(DWORD dwFlag);
		
protected:
	DWORD m_dwFlags;
	UINT m_nBtnDown; // VK_ mouse button
	CPoint m_ptBtnDown;
	CFont m_fontClose, m_fontBold;
	int m_nMouseInCloseButton; // tab index
	BOOL m_bFirstPaint;
	BOOL m_bUpdatingTabWidth;

	// Drag state
	BOOL m_bDragging;
	HWND m_hwndPreDragFocus;
	int m_nDragTab, m_nDropTab;
	CImageList m_ilDragImage;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL WantTabCloseButton(int /*nTab*/) const { return TRUE; }

protected:
	afx_msg void OnPaint();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnChangeTabItem(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetCurSel(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnUpdateTabItemWidth(WPARAM wp, LPARAM lp);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

	// Pseudo message handlers
	void OnButtonDown(UINT nBtn, UINT nFlags, CPoint point);
	void OnButtonUp(UINT nBtn, UINT nFlags, CPoint point);

protected:
	// IDragDropRenderer interface
	virtual CSize OnGetDragSize(CDC& dc);
	virtual void OnDrawDragData(CDC& dc, const CRect& rc, COLORREF& crMask);

protected:
	virtual void PostDrawTab(CDC& dc, int nTab, const CRect& rClip);
	virtual CFont* GetTabFont(int nTab);
	virtual CRect GetTabTextRect(int nTab, LPCRECT pRect);
	virtual void DrawTabItem(CDC* pDC, int nTab, const CRect& rcItem, UINT uiFlags);

	BOOL HasFlag(DWORD dwFlag) const { return ((m_dwFlags & dwFlag) == dwFlag); }
	BOOL IsValidClick(UINT nBtn, const CPoint& ptUp) const;
	BOOL IsDragging() const { return (HasFlag(TCE_DRAGDROP) && m_bDragging); }
	BOOL GetTabCloseButtonRect(int nTab, CRect& rBtn) const;
	BOOL HasTabMoved() const;
	BOOL NeedCustomPaint() const;
	void UpdateTabItemWidths();
	CString GetRequiredTabText(int nTab);
	CString GetRequiredTabText(int nTab, const CString& sCurText);
	COLORREF GetItemBkColor(int nTab);
	COLORREF GetItemTagColor(int nTab);
	void InvalidateTabsUnderSpinButtonCtrl();

	int HitTestDropTab(CPoint point, LPRECT prInsertionMark = NULL) const;
	int HitTestCloseButton(const CPoint& point) const;
	int HitTestDragScrollZone(CPoint pt) const; // -1, 0, 1

	void DrawTabDropMark(CDC* pDC);
	void DrawTabCloseButton(CDC* pDC, int nTab);
	void DrawTabTag(CDC* pDC, int nTab, const CRect& rTab);
	void DrawTabBackColor(CDC* pDC, int nTab, BOOL bHot, CRect& rTab, COLORREF& crText);
	
	static void RemoveUnsupportedFlags(DWORD& dwFlags);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TABCTRLEX_H__F97F9C15_5623_4A5E_ACB5_C10D4C8EB75D__INCLUDED_)
