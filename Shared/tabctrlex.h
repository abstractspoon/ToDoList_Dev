#if !defined(AFX_TABCTRLEX_H__F97F9C15_5623_4A5E_ACB5_C10D4C8EB75D__INCLUDED_)
#define AFX_TABCTRLEX_H__F97F9C15_5623_4A5E_ACB5_C10D4C8EB75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tabctrlex.h : header file
//

#include "..\3rdparty\XPTabCtrl.h"

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

	TCE_ALL				= 0xffff
};

/////////////////////////////////////////////////////////////////////////////

#ifndef TCN_CLOSETAB
#	define TCN_CLOSETAB (TCN_FIRST-11)
#endif

#ifndef TCN_MCLICK
#	define TCN_MCLICK (TCN_FIRST-12)
#endif

#ifndef TCN_ENDDRAG
#	define TCN_ENDDRAG (TCN_FIRST-13)
#endif

struct NMTABCTRLEX
{
    NMHDR hdr;
    int iTab;
	UINT nExtra;
};

/////////////////////////////////////////////////////////////////////////////

class CTabCtrlEx : public CXPTabCtrl
{
// Construction
public:
	CTabCtrlEx(DWORD dwFlags = 0, ETabOrientation orientation = e_tabTop);

	BOOL SetItemText(int nTab, LPCTSTR szText);
	CString GetItemText(int nTab) const;

	BOOL SetItemImage(int nTab, int nImage);
	int GetItemImage(int nTab) const;
	
	DWORD GetItemData(int nTab) const;
	BOOL SetItemData(int nTab, DWORD dwItemData);
	int FindItemByData(DWORD dwItemData) const;

	int HitTest(const CPoint& point) const;
	int HitTest(TCHITTESTINFO* pHitTestInfo) const;

	BOOL ModifyFlags(DWORD dwRemove, DWORD dwAdd);

// Attributes
protected:
	DWORD m_dwFlags;
	UINT m_nBtnDown;
	CPoint m_ptBtnDown;
	CFont m_fontClose;
	int m_nMouseInCloseButton; // tab index
	BOOL m_bDragging;
	int m_nDragTab, m_nDropTab, m_nDropPos;
	CSize m_sizeClose;
	BOOL m_bUpdatingTabWidth;
	CFont m_fontBold;
	BOOL m_bFirstPaint;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabCtrlEx();

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL WantTabCloseButton(int /*nTab*/) const { return TRUE; }

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabCtrlEx)
	//}}AFX_MSG
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnUpdateTabItemWidth(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	virtual void PostDrawTab(CDC& dc, int nTab, BOOL bSelected, const CRect& rClip);
	virtual CFont* GetTabFont(int nTab);
	virtual CRect GetTabTextRect(int nTab, LPCRECT pRect);

	BOOL HasFlag(DWORD dwFlag) const { return ((m_dwFlags & dwFlag) == dwFlag); }
	BOOL IsValidClick(UINT nBtn, const CPoint& ptUp) const;
	void OnButtonDown(UINT nBtn, UINT nFlags, CPoint point);
	void OnButtonUp(UINT nBtn, UINT nFlags, CPoint point);
	BOOL GetTabCloseButtonRect(int nTab, CRect& rBtn) const;
	int GetTabDropIndex(CPoint point, int& nDropPos) const;
	void DrawTabDropMark(CDC* pDC);
	BOOL HasTabMoved() const;
	void DrawTabCloseButton(CDC& dc, int nTab);
	int HitTestCloseButton(const CPoint& point) const;
	BOOL NeedCustomPaint() const;
	void UpdateTabItemWidths(BOOL bSel = FALSE);
	CString GetRequiredTabText(int nTab);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRLEX_H__F97F9C15_5623_4A5E_ACB5_C10D4C8EB75D__INCLUDED_)
