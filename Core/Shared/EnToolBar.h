#if !defined(AFX_ENTOOLBAR_H__9AA29CEC_1405_4BBC_BBD0_94C1BD6D3120__INCLUDED_)
#define AFX_ENTOOLBAR_H__9AA29CEC_1405_4BBC_BBD0_94C1BD6D3120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// entoolbar.h : header file
//

#include "EnImageList.h"

/////////////////////////////////////////////////////////////////////////////

class CEnBitmapEx;

/////////////////////////////////////////////////////////////////////////////
// CEnToolBar window

class CEnToolBar : public CToolBar
{
	// Construction
public:
	CEnToolBar();
	virtual ~CEnToolBar();
	
	BOOL LoadToolBar(LPCTSTR lpszResourceName, LPCTSTR szImagePath = NULL, COLORREF crMask = CLR_NONE);
	BOOL LoadToolBar(UINT nIDResource, LPCTSTR szImagePath = NULL, COLORREF crMask = CLR_NONE);
	BOOL LoadToolBar(UINT nIDResource, UINT nIDImage, COLORREF crMask = CLR_NONE);
	BOOL SetImage(const CString& sImagePath, COLORREF crMask = CLR_NONE);
	BOOL SetImage(UINT nIDImage, COLORREF crMask = CLR_NONE);

	void RefreshButtonStates(BOOL bImmediate = TRUE);
	void SetBackgroundColor(COLORREF color);
	void SetBackgroundColors(COLORREF crFrom, COLORREF crTo, BOOL bGradient, BOOL bGlass);
	void SetHotColor(COLORREF crHot) { m_crHot = crHot; }

	int GetButtonCount(BOOL bIgnoreSeparators = FALSE) const;
	CSize GetButtonSize() const { return m_sizeButton; }
	int GetHeight() const;
	int Resize(int cx, CPoint ptTopLeft = CPoint(0, 0), int nMaxHeight = -1);
	int GetMinReqLength(int nLastBtnPos = -1) const; // single row

	BOOL IsItemSeparator(int nPos) const;
	BOOL LastItemIsSeparator() const;
	BOOL AppendSeparator();
	BOOL InsertSeparator(int nPos);
	int InsertSeparatorAfter(UINT nCmdID); // returns pos
	BOOL DeleteLastItem();
	BOOL DeleteTrailingSeparator();
	int DeleteDuplicateSeparators(int nStartPos = 0);
	BOOL DeleteItem(int nPos);
	BOOL SetItemWidth(int nPos, int nWidth, CRect& rect);
	BOOL SetItemWidth(int nPos, int nWidth);
	int GetItemWidth(int nPos) const;
	int GetItemImage(int nPos) const;
	BOOL SetItemImage(int nPos, int iImage);

	void SetExtendedStyle(DWORD dwExStyle);
	DWORD GetExtendedStyle() const;
	BOOL EnableMixedButtons(BOOL bEnable = TRUE);

	// Attributes
protected:
	CEnImageList m_ilDisabled, m_ilNormal;
	COLORREF m_crFrom, m_crTo, m_crHot;
	BOOL m_bGradient, m_bGlass;
	CUIntArray m_aRowHeights;
	
protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnToolBar)
	//}}AFX_VIRTUAL
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnToolBar)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnRefreshButtonStates(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSizeParent(WPARAM /*wParam*/, LPARAM /*lParam*/) { return 0L; }
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	DECLARE_MESSAGE_MAP()
		
	virtual LRESULT OnButtonPrePaint(LPNMTBCUSTOMDRAW lpNMCustomDraw);
	virtual LRESULT OnButtonPostPaint(LPNMTBCUSTOMDRAW lpNMCustomDraw);
	
	// pseudo message handler
	void DrawToolbarBackground(CDC* pDC, BOOL bClient);
	COLORREF GetButtonHotColor() const;
	
	void SetImageSize(int cx, int cy);
	BOOL SetImage(CEnBitmapEx* pBitmap, COLORREF crMask);
	void RefreshDisabledImageList(CEnBitmapEx* pBitmap, COLORREF crMask);
	void RefreshDisabledImageList();

	int EstimateHeightRequired(int cx) const;
	int EstimateRowsRequired(int cx) const;
	int RefreshRowHeights();
	
	static void DrawButtonBackground(CDC* pDC, const CRect& rBtn, COLORREF crFill);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTOOLBAR_H__9AA29CEC_1405_4BBC_BBD0_94C1BD6D3120__INCLUDED_)
