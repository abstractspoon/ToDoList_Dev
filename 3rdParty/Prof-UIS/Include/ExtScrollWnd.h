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

#if (!defined __EXT_SCROLLWND_H)
#define __EXT_SCROLLWND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#if (!defined __EXT_MFC_NO_SCROLLWND)

#define __EXT_SCROLL_NUMERIC_MAX INT_MAX

/////////////////////////////////////////////////////////////////////////////
// CExtScrollBar window


class __PROF_UIS_API CExtScrollBar : public CScrollBar
{
// Construction
public:
	DECLARE_DYNCREATE( CExtScrollBar );
	CExtScrollBar();

// Attributes
public:
	enum eScrollerOrientation_t
	{
		__ESO_TOP = 0,
		__ESO_BOTTOM = 1,
		__ESO_LEFT = 2,
		__ESO_RIGHT = 3,
		__ESO_AUTO = 4,
		__ESO_NONE = 5,
		__ESO_MIN_VALUE = 0,
		__ESO_MAX_VALUE = 5,
	};
	eScrollerOrientation_t m_eSO;
	bool m_bAutoDeleteOnPostNcDestroy:1,m_bReflectParentSizing:1,
		m_bDisableAutoReserveSpace:1,
		m_bAutoReserveSpaceBefore:1, m_bAutoReserveSpaceAfter:1,
		m_bSmoothPainting:1;

	virtual void SyncReservedSpace( CExtScrollBar * pOtherBar );
	virtual eScrollerOrientation_t GetScrollerOrientation() const;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtScrollBar)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtScrollBar();
	virtual void PostNcDestroy();
protected:
	INT m_nHorzBarHeight, m_nVertBarWidth;
	void _ScanSysMertics();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtScrollBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtScrollBar

/////////////////////////////////////////////////////////////////////////////
// CExtScrollWnd window

// based on MFC source code in viewscrl.cpp
class __PROF_UIS_API CExtScrollWnd
	: public CWnd
	, public CExtMouseCaptureSink
{
// Construction
public:
	DECLARE_DYNCREATE( CExtScrollWnd );
	CExtScrollWnd();

// Attributes
public:
	bool m_bScrollPhysical:1, m_bEatErasing:1,
		m_bBufferedPainting:1,
		m_bScrollInvalidate:1, m_bScrollErase:1,
		m_bScrollUpdateWindow:1, m_bRedrawUpdateWindow:1,
		m_bRedrawInvalidate:1, m_bRedrawErase:1,
		m_bUse32BitScrollInfo:1;

protected:
	int m_nUpdateScrollBars;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtScrollWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtScrollWnd();

	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const;
	virtual CSize OnSwGetLineSize( int nDirection ) const;
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc = NULL
		);
	virtual CRect OnSwGetClientRect() const;
	
	virtual bool OnSwCanAutoHideScrollBar( bool bHorz ) const;
	virtual bool OnSwHasScrollBar( bool bHorz ) const;
	virtual void OnSwEnableScrollBarCtrl( int nBar, bool bEnable );
	virtual bool OnSwScrollInfoAdjust(
		int nBar,
		SCROLLINFO & si,
		bool bRedraw = true
		);
	virtual void OnSwSetScrollRange(
		int nBar,
		LONG nMinPos,
		LONG nMaxPos,
		bool bRedraw = true
		);
	virtual bool OnSwQueryThumbTrackEnabled( bool bHorz ) const;

	virtual LONG ScrollLimit32Get( int nBar ) const;
	virtual void ScrollInfo32Get(
		int nBar,
		LONG * p_nMinPos,
		LONG * p_nMaxPos,
		LONG * p_nPageSize,
		LONG * p_nCurrPos,
		LONG * p_nTrackPos
		) const;
	virtual LONG ScrollPos32Get(
		int nBar,
		bool bTrackPos = false
		) const;
	virtual void ScrollPos32Set(
		int nBar,
		LONG nPos,
		bool bRedraw = true
		);

	virtual CPoint OnSwGetScrollPos() const;
	virtual CPoint OnSwGetScrollPaintPos() const;
	virtual void OnSwSetScrollPos( POINT pt );

	virtual void OnSwGetScrollBarState(
		CSize sizeClient,
		CSize & sizeNeedSB,
		CSize & sizeRange,
		CPoint & ptMove,
		bool bInsideClient
		) const;
	virtual CSize OnSwGetScrollBarSizes() const;
	virtual bool OnSwCalcClientSizes(
		CSize & size,
		CSize & sizeSb
		);
	
	virtual UINT OnSwCalcMouseScrollLines(
		UINT fFlags,
		short zDelta,
		CPoint point
		);
	virtual bool OnSwDoMouseWheel(
		UINT fFlags,
		short zDelta,
		CPoint point
		);

	virtual bool OnSwDoScroll(
		UINT nScrollCode,
		UINT nPos,
		bool bDoScroll = true
		);
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
		);
	virtual void OnSwUpdateScrollBars();

	virtual void OnSwDoScrollWindow(
		int xAmount,
		int yAmount,
		LPCRECT lpRect = NULL,
		LPCRECT lpClipRect = NULL
		);

	virtual void OnSwEnshurePointAvail( CPoint point );

	virtual void OnSwInvalidate( bool bErase );
	virtual void OnSwDoRedraw();
	virtual void OnSwPaint( CDC & dc );

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtScrollWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; // class CExtScrollWnd

#endif // (!defined __EXT_MFC_NO_SCROLLWND)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined __EXT_SCROLLWND_H
