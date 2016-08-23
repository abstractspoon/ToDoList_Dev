// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_BUTTON_H)
#define __EXT_BUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

class CExtPopupMenuWnd;

/////////////////////////////////////////////////////////////////////////////
// CExtButton window

class __PROF_UIS_API CExtButton
	: public CButton
	, public CExtMouseCaptureSink
{
	DECLARE_DYNCREATE(CExtButton);

// Construction
public:
    CExtButton();
	~CExtButton();

// Attributes
public:

// Operations
public:
	CMenu m_menu;
	DWORD m_dwMenuOpt; // TPMX_TOPALIGN is default
	UINT m_nMenuLeftAreaWidth; // 0 is default

	virtual void _OnClick(
		bool bSelectAny,
		bool bSeparatedDropDownClicked
		);
	virtual bool _OnTrackPopup(
		bool bSelectAny
		);
	virtual bool _IsMenuAvail();

	void _CalcClientRects(
		CRect & rcMainArea,
		CRect & rcDropDown
		) const;
	INT _HtMouseOnButton( const POINT & ptClient ) const; // 0 - not, 1-main area, 2-dropdown area
	INT _HtMouseOnButton() const; // 0 - not, 1-main area, 2-dropdown area

	virtual bool OnPutToPopupMenu( CExtPopupMenuWnd * pPopup );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetDefault() const;

	void SetTooltipText(int nId, BOOL bActivate = TRUE);
	void SetTooltipText(CExtSafeString* spText, BOOL bActivate = TRUE);
	void SetTooltipText(CExtSafeString & sText, BOOL bActivate = TRUE);
	void SetTooltipText(__EXT_MFC_SAFE_LPCTSTR sText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);

	BOOL SetBtnCursor( int nCursorId = -1 );

	void SetAlign( int nAlign );
	int GetAlign() const;

	BOOL GetSeparatedDropDown() const;
	void SetSeparatedDropDown(
		BOOL bSeparatedDropDown = TRUE
		);

	BOOL GetAutoChangeID() const;
	void SetAutoChangeID(
		BOOL bAutoChangeID = TRUE
		);
	BOOL GetAutoChangeCmdIcon() const;
	void SetAutoChangeCmdIcon(
		BOOL bAutoChangeCmdIcon = TRUE
		);

	void SetFlat( BOOL bState = TRUE );
	BOOL GetFlat() const;

	BOOL GetDrawBorder() const;
	void SetDrawBorder( BOOL bEnable = TRUE );

	void SetIcon( int nIconInId, int nIconOutId = NULL );
	void SetIcon( HICON hIconIn, HICON hIconOut = NULL );

protected:
    //{{AFX_MSG(CExtButton)
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	afx_msg LRESULT _OnPutToPopupMenuImpl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	void _FreeWinObjects();

protected:
	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);
protected:
	void _DrawImpl( // buffered paint
		CDC & dc,
		bool bTransparent = false
		);
	virtual void _RenderImpl( // non-buffered paint
		CDC & dc,
		bool bTransparent = false
		);
	void InitToolTip();

	int m_nAlign;
	BOOL m_bDrawBorder, m_bIsFlat, m_bIsDefault,
		m_bMouseOver, m_bPushed,
		m_bPushTracking, m_bKeyTracking,
		m_bSeparatedDropDown, m_bAutoChangeID, m_bAutoChangeCmdIcon;
	INT m_nTrackingHT;

	HCURSOR m_hCursor;
	CToolTipCtrl m_wndToolTip;

	HICON m_hIconIn;
	HICON m_hIconOut;

}; // class CExtButton

/////////////////////////////////////////////////////////////////////////////

class __PROF_UIS_API CExtColorButton : public CExtButton
{
	DECLARE_DYNCREATE(CExtColorButton);

// Construction
public:
    CExtColorButton();
	~CExtColorButton();

	CExtSafeString m_sBtnTextColorDefault;
	CExtSafeString m_sBtnTextColorCustom;

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtColorButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bEnableBtnColorDefault;
	bool m_bEnableBtnColorCustom;
	bool m_bUseStdColorDlg;
	COLORREF m_clrDefault,m_clrSelected;
	LPARAM m_lParamCookie;

	virtual void OnGenerateColorBtnIcon(
		HICON * p_hIcon,
		bool * p_bDestroyIcon
		);

protected:
	virtual bool _OnTrackPopup(
		bool bSelectAny
		);
	virtual bool _IsMenuAvail();
	virtual void _RenderImpl( // non-buffered paint
		CDC & dc,
		bool bTransparent = false
		);
	virtual void _OnClick(
		bool bSelectAny,
		bool bSeparatedDropDownClicked
		);
	
    //{{AFX_MSG(CExtColorButton)
	//}}AFX_MSG
	afx_msg LRESULT OnColorChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtColorButton

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif /// __EXT_BUTTON_H
