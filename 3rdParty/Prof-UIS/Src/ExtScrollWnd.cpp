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

#include "stdafx.h"

#if (!defined __EXT_MFC_NO_SCROLLWND)

#if (!defined __EXT_SCROLLWND_H)
	#include <ExtScrollWnd.h>
#endif 

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif 

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../src/ExtMemoryDC.h>
#endif

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#ifndef __AFXSTATE_H__
		#include <../src/mfc/afxstat_.h>
	#endif
	#include <../src/mfc/AfxImpl.h>
#endif

#if (!defined __EXT_LOCALIZATION_H)
	#include <../Src/ExtLocalization.h>
#endif

#include <../profuisdll/resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtScrollBar

IMPLEMENT_DYNCREATE( CExtScrollBar, CScrollBar );

CExtScrollBar::CExtScrollBar()
	: m_bAutoDeleteOnPostNcDestroy( false )
	, m_bReflectParentSizing( true )
	, m_eSO( CExtScrollBar::__ESO_AUTO )
	, m_bDisableAutoReserveSpace( false )
	, m_bAutoReserveSpaceBefore( false )
	, m_bAutoReserveSpaceAfter( false )
	, m_bSmoothPainting( true )
{
	_ScanSysMertics();
}

CExtScrollBar::~CExtScrollBar()
{
}

BEGIN_MESSAGE_MAP(CExtScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CExtScrollBar)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	__EXT_MFC_SAFE_ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
END_MESSAGE_MAP()

void CExtScrollBar::_ScanSysMertics()
{
	ASSERT_VALID( this );
	m_nHorzBarHeight	= ::GetSystemMetrics( SM_CYHSCROLL );
	m_nVertBarWidth		= ::GetSystemMetrics( SM_CXVSCROLL );
}

void CExtScrollBar::SyncReservedSpace( CExtScrollBar * pOtherBar )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pOtherBar );
	ASSERT( this != pOtherBar );
CWnd * pWndNext = GetWindow( GW_HWNDNEXT );
	if( pWndNext != pOtherBar )
		return;
	m_bAutoReserveSpaceBefore = m_bAutoReserveSpaceAfter = false;
	pOtherBar->m_bAutoReserveSpaceBefore = pOtherBar->m_bAutoReserveSpaceAfter = false;
	if( m_bDisableAutoReserveSpace )
		return;
	if( (GetStyle()&WS_VISIBLE) == 0 || (pOtherBar->GetStyle()&WS_VISIBLE) == 0 )
		return;
eScrollerOrientation_t _eSOown = GetScrollerOrientation();
eScrollerOrientation_t _eSOother = pOtherBar->GetScrollerOrientation();
	switch( _eSOown )
	{
	case __ESO_TOP:
	case __ESO_BOTTOM:
		switch( _eSOother )
		{
		case __ESO_LEFT:
			m_bAutoReserveSpaceBefore = true;
			break;
		case __ESO_RIGHT:
			m_bAutoReserveSpaceAfter = true;
			break;
		case __ESO_TOP:
		case __ESO_BOTTOM:
		case __ESO_NONE:
			return;
#ifdef _DEBUG
		default:
			ASSERT( FALSE );
			return;
#endif // _DEBUG
		} // switch( _eSOother )
		break;
	case __ESO_LEFT:
	case __ESO_RIGHT:
		switch( _eSOother )
		{
		case __ESO_TOP:
			m_bAutoReserveSpaceBefore = true;
			break;
		case __ESO_BOTTOM:
			m_bAutoReserveSpaceAfter = true;
			break;
		case __ESO_LEFT:
		case __ESO_RIGHT:
		case __ESO_NONE:
			return;
#ifdef _DEBUG
		default:
			ASSERT( FALSE );
			return;
#endif // _DEBUG
		} // switch( _eSOother )
		break;
	case __ESO_NONE:
		return;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		return;
#endif // _DEBUG
	} // switch( _eSOown )
}

CExtScrollBar::eScrollerOrientation_t
	CExtScrollBar::GetScrollerOrientation() const
{
	ASSERT_VALID( this );
	ASSERT( __ESO_MIN_VALUE <= m_eSO && m_eSO <= __ESO_MAX_VALUE );
eScrollerOrientation_t _eSO = m_eSO;
	if( _eSO == __ESO_AUTO )
	{
		DWORD dwWndStyle = CWnd::GetStyle();
		if( ( dwWndStyle & (SBS_TOPALIGN|SBS_VERT) ) == (SBS_TOPALIGN) )
			_eSO = __ESO_TOP;
		else if( ( dwWndStyle & (SBS_BOTTOMALIGN|SBS_VERT) ) == (SBS_BOTTOMALIGN) )
			_eSO = __ESO_BOTTOM;
		else if( ( dwWndStyle & (SBS_LEFTALIGN|SBS_VERT) ) == (SBS_LEFTALIGN|SBS_VERT) )
			_eSO = __ESO_LEFT;
		else if( ( dwWndStyle & (SBS_RIGHTALIGN|SBS_VERT) ) == (SBS_RIGHTALIGN|SBS_VERT) )
			_eSO = __ESO_RIGHT;
		else
			_eSO = __ESO_NONE;
	} // if( _eSO == __ESO_AUTO )
	return _eSO;
}

void CExtScrollBar::PreSubclassWindow()
{
	CScrollBar::PreSubclassWindow();
	_ScanSysMertics();
}

void CExtScrollBar::PostNcDestroy()
{
	ASSERT_VALID( this );
	if( m_bAutoDeleteOnPostNcDestroy )
		delete this;
}

BOOL CExtScrollBar::OnEraseBkgnd(CDC* pDC) 
{
	if( !m_bSmoothPainting )
		return CScrollBar::OnEraseBkgnd( pDC );
	return TRUE;
}

void CExtScrollBar::OnPaint() 
{
	if( !m_bSmoothPainting )
	{
		CScrollBar::OnPaint();
		return;
	} // if( !m_bSmoothPainting )
CRect rcClient;
	GetClientRect( &rcClient );
CPaintDC dcPaint( this );
CExtMemoryDC dc( &dcPaint, &rcClient );
	DefWindowProc( WM_PAINT, (WPARAM)dc.GetSafeHdc(), 0L );
}

void CExtScrollBar::OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection) 
{
	ASSERT_VALID( this );
	CScrollBar::OnSettingChange(uFlags, lpszSection);
	_ScanSysMertics();
}

LRESULT CExtScrollBar::OnSizeParent( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	if( !m_bReflectParentSizing )
		return (LRESULT)0;
	wParam;
DWORD dwWndStyle = CWnd::GetStyle();
	if( (dwWndStyle & WS_VISIBLE) == 0 )
		return 0;
AFX_SIZEPARENTPARAMS * lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	ASSERT( lpLayout != NULL );

CRect rcOwnLayout( lpLayout->rect );
eScrollerOrientation_t _eSO = GetScrollerOrientation();
CSize _sizeNeeded( 0, 0 );
	switch( _eSO )
	{
	case __ESO_TOP:
		_sizeNeeded.cy = m_nHorzBarHeight;
		lpLayout->rect.top += _sizeNeeded.cy;
		rcOwnLayout.bottom = rcOwnLayout.top + _sizeNeeded.cy;
		lpLayout->sizeTotal.cy += _sizeNeeded.cy;
		if( m_bAutoReserveSpaceBefore )
			rcOwnLayout.left += m_nVertBarWidth;
		if( m_bAutoReserveSpaceAfter )
			rcOwnLayout.right -= m_nVertBarWidth;
		break;
	case __ESO_BOTTOM:
		_sizeNeeded.cy = m_nHorzBarHeight;
		lpLayout->rect.bottom -= _sizeNeeded.cy;
		rcOwnLayout.top = rcOwnLayout.bottom - _sizeNeeded.cy;
		lpLayout->sizeTotal.cy += _sizeNeeded.cy;
		if( m_bAutoReserveSpaceBefore )
			rcOwnLayout.left += m_nVertBarWidth;
		if( m_bAutoReserveSpaceAfter )
			rcOwnLayout.right -= m_nVertBarWidth;
		break;
	case __ESO_LEFT:
		_sizeNeeded.cx = m_nVertBarWidth;
		lpLayout->rect.left += _sizeNeeded.cx;
		rcOwnLayout.right = rcOwnLayout.left + _sizeNeeded.cx;
		lpLayout->sizeTotal.cx += _sizeNeeded.cx;
		if( m_bAutoReserveSpaceBefore )
			rcOwnLayout.top += m_nHorzBarHeight;
		if( m_bAutoReserveSpaceAfter )
			rcOwnLayout.bottom -= m_nHorzBarHeight;
		break;
	case __ESO_RIGHT:
		_sizeNeeded.cx = m_nVertBarWidth;
		lpLayout->rect.right -= _sizeNeeded.cx;
		rcOwnLayout.left = rcOwnLayout.right - _sizeNeeded.cx;
		lpLayout->sizeTotal.cx += _sizeNeeded.cx;
		if( m_bAutoReserveSpaceBefore )
			rcOwnLayout.top += m_nHorzBarHeight;
		if( m_bAutoReserveSpaceAfter )
			rcOwnLayout.bottom -= m_nHorzBarHeight;
		break;
	case __ESO_NONE:
		break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		break;
#endif // _DEBUG
	} // switch( _eSO )

//	ASSERT( ! rcOwnLayout.IsRectEmpty() );
	if( lpLayout->hDWP != NULL )
	{
		::AfxRepositionWindow(
			lpLayout,
			m_hWnd,
			&rcOwnLayout
			);
	} // if( lpLayout->hDWP != NULL )
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CExtScrollWnd

IMPLEMENT_DYNCREATE( CExtScrollWnd, CWnd );

CExtScrollWnd::CExtScrollWnd()
	: m_nUpdateScrollBars( 0 )
	, m_bScrollPhysical( false )
	, m_bEatErasing( true )
	, m_bBufferedPainting( true )
	, m_bScrollUpdateWindow( false )
	, m_bScrollInvalidate( true )
	, m_bScrollErase( false )
	, m_bRedrawUpdateWindow( false )
	, m_bRedrawInvalidate( true )
	, m_bRedrawErase( false )
	, m_bUse32BitScrollInfo( true )
{
}

CExtScrollWnd::~CExtScrollWnd()
{
}


BEGIN_MESSAGE_MAP(CExtScrollWnd, CWnd)
	//{{AFX_MSG_MAP(CExtScrollWnd)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOLORCHANGE()
	__EXT_MFC_SAFE_ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(__ExtMfc_WM_THEMECHANGED, OnThemeChanged)
END_MESSAGE_MAP()

void CExtScrollWnd::OnSysColorChange() 
{
	ASSERT_VALID( this );
	CWnd::OnSysColorChange();
	g_PaintManager.OnSysColorChange( this );
	g_CmdManager.OnSysColorChange( this );
	Invalidate();
}

LRESULT CExtScrollWnd::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = CWnd::OnDisplayChange( wParam, lParam );
	g_PaintManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	g_CmdManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	return lResult;
}

LRESULT CExtScrollWnd::OnThemeChanged( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = Default();
	g_PaintManager.OnThemeChanged( this, wParam, lParam );
	g_CmdManager.OnThemeChanged( this, wParam, lParam );
	OnSwRecalcLayout( true );
	return lResult;
}

void CExtScrollWnd::OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection) 
{
	ASSERT_VALID( this );
	CWnd::OnSettingChange(uFlags, lpszSection);
	g_PaintManager.OnSettingChange( this, uFlags, lpszSection );
	g_CmdManager.OnSettingChange( this, uFlags, lpszSection );
	OnSwRecalcLayout( true );
}

CSize CExtScrollWnd::OnSwGetTotalSize() const
{
	ASSERT_VALID( this );
	return CSize( 0, 0 );
}

CSize CExtScrollWnd::OnSwGetPageSize( int nDirection ) const
{
	ASSERT_VALID( this );
	nDirection;
	return CSize( 0, 0 );
}

CSize CExtScrollWnd::OnSwGetLineSize( int nDirection ) const
{
	ASSERT_VALID( this );
	nDirection;
	return CSize( 0, 0 );
}

bool CExtScrollWnd::OnSwScrollInfoAdjust(
	int nBar,
	SCROLLINFO & si,
	bool bRedraw // = true
	)
{
	ASSERT_VALID( this );
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
CScrollBar * pScrollBarWnd = GetScrollBarCtrl( nBar );
	if( pScrollBarWnd != NULL )
	{
		if( pScrollBarWnd->SetScrollInfo(
				&si,
				bRedraw ? TRUE : FALSE
				)
			)
			return true;
		return false;
	} // if( pScrollBarWnd != NULL )
	if( CWnd::SetScrollInfo(
			nBar,
			&si,
			bRedraw ? TRUE : FALSE
			)
		)
		return true;
	return false;
}

void CExtScrollWnd::OnSwSetScrollRange(
	int nBar,
	LONG nMinPos,
	LONG nMaxPos,
	bool bRedraw // = true
	)
{
	ASSERT_VALID( this );
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
CScrollBar * pScrollBarWnd = GetScrollBarCtrl( nBar );
	if( pScrollBarWnd != NULL )
	{
		pScrollBarWnd->SetScrollRange(
			(int)nMinPos,
			(int)nMaxPos,
			bRedraw ? TRUE : FALSE
			);
		return;
	} // if( pScrollBarWnd != NULL )
	CWnd::SetScrollRange(
		nBar,
		(int)nMinPos,
		(int)nMaxPos,
		bRedraw ? TRUE : FALSE
		);
}

LONG CExtScrollWnd::ScrollLimit32Get( int nBar ) const
{
	ASSERT_VALID( this );
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
CScrollBar * pScrollBarWnd = GetScrollBarCtrl( nBar );
	if( pScrollBarWnd != NULL )
	{
		if( m_bUse32BitScrollInfo )
		{
			SCROLLINFO _scroll_info;
			::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
			_scroll_info.cbSize = sizeof(SCROLLINFO);
			if(	pScrollBarWnd->GetScrollInfo(
					&_scroll_info,
					SIF_RANGE|SIF_PAGE
					)
				)
			{
				LONG nRetVal = (LONG)
					(_scroll_info.nMax
					- _scroll_info.nMin
					- _scroll_info.nPage
					+ 1);
				ASSERT( nRetVal >= 0 );
				return nRetVal;
			}
			//ASSERT( FALSE );
		} // if( m_bUse32BitScrollInfo )
		LONG nRetVal = (LONG)
			pScrollBarWnd->GetScrollLimit();
		return nRetVal;
	} // if( pScrollBarWnd != NULL )
	if( m_bUse32BitScrollInfo )
	{
		SCROLLINFO _scroll_info;
		::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
		_scroll_info.cbSize = sizeof(SCROLLINFO);
		if(	( const_cast < CExtScrollWnd * > ( this ) ) ->
				CWnd::GetScrollInfo(
					nBar,
					&_scroll_info,
					SIF_RANGE|SIF_PAGE
					)
			)
		{
			LONG nRetVal = (LONG)
				(_scroll_info.nMax
				- _scroll_info.nMin
				- _scroll_info.nPage
				+ 1);
			ASSERT( nRetVal >= 0 );
			return nRetVal;
		}
		//ASSERT( FALSE );
	} // if( m_bUse32BitScrollInfo )
LONG nRetVal = (LONG)
		(	( const_cast < CExtScrollWnd * > ( this ) ) ->
				CWnd::GetScrollLimit( nBar )
		);
	return nRetVal;
}

void CExtScrollWnd::ScrollInfo32Get(
	int nBar,
	LONG * p_nMinPos,
	LONG * p_nMaxPos,
	LONG * p_nPageSize,
	LONG * p_nCurrPos,
	LONG * p_nTrackPos
	) const
{
	ASSERT_VALID( this );
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
	if( p_nMinPos != NULL )
		*p_nMinPos = 0L;
	if( p_nMaxPos != NULL )
		*p_nMaxPos = 0L;
	if( p_nPageSize != NULL )
		*p_nPageSize = 0L;
	if( p_nCurrPos != NULL )
		*p_nCurrPos = 0L;
	if( p_nTrackPos != NULL )
		*p_nTrackPos = 0L;
CScrollBar * pScrollBarWnd = GetScrollBarCtrl( nBar );
	if( pScrollBarWnd != NULL )
	{
		if( m_bUse32BitScrollInfo )
		{
			SCROLLINFO _scroll_info;
			::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
			_scroll_info.cbSize = sizeof(SCROLLINFO);
			if(	pScrollBarWnd->GetScrollInfo(
					&_scroll_info,
					SIF_RANGE|SIF_PAGE|SIF_POS|SIF_TRACKPOS
					)
				)
			{
				if( p_nMinPos != NULL )
					*p_nMinPos = _scroll_info.nMin;
				if( p_nMaxPos != NULL )
					*p_nMaxPos = _scroll_info.nMax;
				if( p_nPageSize != NULL )
					*p_nPageSize = _scroll_info.nPage;
				if( p_nCurrPos != NULL )
					*p_nCurrPos = _scroll_info.nPos;
				if( p_nTrackPos != NULL )
					*p_nTrackPos = _scroll_info.nTrackPos;
				return;
			}
			//ASSERT( FALSE );
		} // if( m_bUse32BitScrollInfo )
		if( p_nMinPos != NULL || p_nMaxPos != NULL )
		{
			INT nMin = 0, nMax = 0;
			pScrollBarWnd->GetScrollRange( &nMin, &nMax );
			if( p_nMinPos != NULL )
				*p_nMinPos = nMin;
			if( p_nMaxPos != NULL )
				*p_nMaxPos = nMax;
			
		} // if( p_nMinPos != NULL || p_nMaxPos != NULL )
		if( p_nCurrPos != NULL || p_nTrackPos != NULL )
		{
			INT nPos = pScrollBarWnd->GetScrollPos();
			if( p_nCurrPos != NULL )
				*p_nCurrPos = nPos;
			if( p_nTrackPos != NULL )
				*p_nTrackPos = nPos;
		} // if( p_nCurrPos != NULL || p_nTrackPos != NULL )
		return;
	} // if( pScrollBarWnd != NULL )
	if( m_bUse32BitScrollInfo )
	{
		SCROLLINFO _scroll_info;
		::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
		_scroll_info.cbSize = sizeof(SCROLLINFO);
		if(	( const_cast < CExtScrollWnd * > ( this ) ) ->
				CWnd::GetScrollInfo(
					nBar,
					&_scroll_info,
					SIF_RANGE|SIF_PAGE|SIF_POS|SIF_TRACKPOS
					)
			)
		{
			if( p_nMinPos != NULL )
				*p_nMinPos = _scroll_info.nMin;
			if( p_nMaxPos != NULL )
				*p_nMaxPos = _scroll_info.nMax;
			if( p_nPageSize != NULL )
				*p_nPageSize = _scroll_info.nPage;
			if( p_nCurrPos != NULL )
				*p_nCurrPos = _scroll_info.nPos;
			if( p_nTrackPos != NULL )
				*p_nTrackPos = _scroll_info.nTrackPos;
			return;
		}
		//ASSERT( FALSE );
	} // if( m_bUse32BitScrollInfo )
	if( p_nMinPos != NULL || p_nMaxPos != NULL )
	{
		INT nMin = 0, nMax = 0;
		CWnd::GetScrollRange( nBar, &nMin, &nMax );
		if( p_nMinPos != NULL )
			*p_nMinPos = nMin;
		if( p_nMaxPos != NULL )
			*p_nMaxPos = nMax;
		
	} // if( p_nMinPos != NULL || p_nMaxPos != NULL )
	if( p_nCurrPos != NULL || p_nTrackPos != NULL )
	{
		INT nPos = CWnd::GetScrollPos( nBar );
		if( p_nCurrPos != NULL )
			*p_nCurrPos = nPos;
		if( p_nTrackPos != NULL )
			*p_nTrackPos = nPos;
	} // if( p_nCurrPos != NULL || p_nTrackPos != NULL )
	return;
}

LONG CExtScrollWnd::ScrollPos32Get(
	int nBar,
	bool bTrackPos // = false
	) const
{
	ASSERT_VALID( this );
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
CScrollBar * pScrollBarWnd = GetScrollBarCtrl( nBar );
	if( pScrollBarWnd != NULL )
	{
		if( m_bUse32BitScrollInfo )
		{
			SCROLLINFO _scroll_info;
			::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
			_scroll_info.cbSize = sizeof(SCROLLINFO);
			if(	pScrollBarWnd->GetScrollInfo(
					&_scroll_info,
					bTrackPos ? SIF_TRACKPOS : SIF_POS
					)
				)
			{
				LONG nRetVal = (LONG)
					(	bTrackPos
							? _scroll_info.nTrackPos
							: _scroll_info.nPos
					);
				return nRetVal;
			}
			//ASSERT( FALSE );
		} // if( m_bUse32BitScrollInfo )
		LONG nRetVal = (LONG)
			pScrollBarWnd->GetScrollPos();
		return nRetVal;
	} // if( pScrollBarWnd != NULL )
DWORD dwWndStyle = CWnd::GetStyle();
	if(		( nBar == SB_HORZ && (dwWndStyle & WS_HSCROLL) == 0 )
		||	( nBar == SB_VERT && (dwWndStyle & WS_VSCROLL) == 0 )
		)
		return 0;
	if( m_bUse32BitScrollInfo )
	{
		SCROLLINFO _scroll_info;
		::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
		_scroll_info.cbSize = sizeof(SCROLLINFO);
		if( ( const_cast < CExtScrollWnd * > ( this ) ) ->
				CWnd::GetScrollInfo(
					nBar,
					&_scroll_info,
					bTrackPos ? SIF_TRACKPOS : SIF_POS
					)
				)
		{
			LONG nRetVal = (LONG)
				(	bTrackPos
						? _scroll_info.nTrackPos
						: _scroll_info.nPos
				);
			return nRetVal;
		}
		//ASSERT( FALSE );
	} // if( m_bUse32BitScrollInfo )
LONG nRetVal = (LONG)
		(	( const_cast < CExtScrollWnd * > ( this ) ) ->
				CWnd::GetScrollPos( nBar )
		);
	return nRetVal;
}

void CExtScrollWnd::ScrollPos32Set(
	int nBar,
	LONG nPos,
	bool bRedraw // = true
	)
{
	ASSERT_VALID( this );
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
CScrollBar * pScrollBarWnd = GetScrollBarCtrl( nBar );
	if( pScrollBarWnd != NULL )
	{
		if( m_bUse32BitScrollInfo )
		{
			SCROLLINFO _scroll_info;
			::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
			_scroll_info.cbSize = sizeof(SCROLLINFO);
			_scroll_info.fMask = SIF_POS;
			_scroll_info.nPos = (int)nPos;
			if( pScrollBarWnd->SetScrollInfo(
					&_scroll_info,
					bRedraw ? TRUE : FALSE
					)
				)
				return;
			//ASSERT( FALSE );
		} // if( m_bUse32BitScrollInfo )
		pScrollBarWnd->SetScrollPos(
			(int)nPos,
			bRedraw ? TRUE : FALSE
			);
		return;
	} // if( pScrollBarWnd != NULL )
	if( m_bUse32BitScrollInfo )
	{
		SCROLLINFO _scroll_info;
		::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
		_scroll_info.cbSize = sizeof(SCROLLINFO);
		_scroll_info.fMask = SIF_POS;
		_scroll_info.nPos = (int)nPos;
		if( CWnd::SetScrollInfo(
				nBar,
				&_scroll_info,
				bRedraw ? TRUE : FALSE
				)
			)
			return;
		//ASSERT( FALSE );
	} // if( m_bUse32BitScrollInfo )
	CWnd::SetScrollPos(
		nBar,
		(int)nPos,
		bRedraw ? TRUE : FALSE
		);
}

CPoint CExtScrollWnd::OnSwGetScrollPos() const
{
	ASSERT_VALID( this );
CPoint pt(
		ScrollPos32Get( SB_HORZ ),
		ScrollPos32Get( SB_VERT )
		);
	ASSERT( pt.x >= 0 && pt.y >= 0 );
	return pt;
}

CPoint CExtScrollWnd::OnSwGetScrollPaintPos() const
{
	ASSERT_VALID( this );
	return OnSwGetScrollPos();
}

CRect CExtScrollWnd::OnSwRecalcLayout(
	bool bDoLayout,
	LPCRECT pRectClientSrc // = NULL
	)
{
	ASSERT_VALID( this );
	if( bDoLayout )
	{
		CScrollBar * pScrollBarWndH = GetScrollBarCtrl( SB_HORZ );
		CScrollBar * pScrollBarWndV = GetScrollBarCtrl( SB_VERT );
		if( pScrollBarWndH != NULL && pScrollBarWndV != NULL )
		{
			CExtScrollBar * pExtScrollBarWndH = DYNAMIC_DOWNCAST( CExtScrollBar, pScrollBarWndH );
			CExtScrollBar * pExtScrollBarWndV = DYNAMIC_DOWNCAST( CExtScrollBar, pScrollBarWndV );
			if( pExtScrollBarWndH != NULL && pExtScrollBarWndV != NULL )
			{
				pExtScrollBarWndH->SyncReservedSpace( pExtScrollBarWndV );
				pExtScrollBarWndV->SyncReservedSpace( pExtScrollBarWndH );
			} // if( pExtScrollBarWndH != NULL && pExtScrollBarWndV != NULL )
		} // if( pScrollBarWndH != NULL && pScrollBarWndV != NULL )
	} // if( bDoLayout )
CRect rcClient;
	if( pRectClientSrc != NULL )
		rcClient = *pRectClientSrc;
	else
		CWnd::GetClientRect( &rcClient );
	CWnd::RepositionBars(
		0,
		0x0FFFF,
		AFX_IDW_PANE_FIRST,
		bDoLayout ? CWnd::reposDefault : CWnd::reposQuery,
		&rcClient,
		&rcClient,
		TRUE
		);
	return rcClient;
}

CRect CExtScrollWnd::OnSwGetClientRect() const
{
	ASSERT_VALID( this );
CRect rcClient =
		( const_cast < CExtScrollWnd * > ( this ) )
			-> OnSwRecalcLayout( false );
	return rcClient;
}

bool CExtScrollWnd::OnSwHasScrollBar( bool bHorz ) const
{
	ASSERT_VALID( this );
CScrollBar * pScrollBarWnd = GetScrollBarCtrl( bHorz ? SB_HORZ : SB_VERT );
	if( pScrollBarWnd != NULL )
	{
		if( pScrollBarWnd->IsWindowEnabled() )
			return true;
		return false;
	} // if( pScrollBarWnd != NULL )
DWORD dwWndStyle = CWnd::GetStyle();
DWORD dwTestStyle = bHorz ? WS_HSCROLL : WS_VSCROLL;
	if( ( dwWndStyle & dwTestStyle ) != 0 )
		return true;
	return false;
}

bool CExtScrollWnd::OnSwCanAutoHideScrollBar( bool bHorz ) const
{
	ASSERT_VALID( this );
	bHorz;
	return true;
}

void CExtScrollWnd::OnSwEnableScrollBarCtrl( int nBar, bool bEnable )
{
	ASSERT_VALID( this );
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT || nBar == SB_BOTH );
DWORD dwWndStyle = CWnd::GetStyle();
CPoint ptMove = OnSwGetScrollPos();
CScrollBar * pScrollBarWndH = GetScrollBarCtrl( SB_HORZ );
CScrollBar * pScrollBarWndV = GetScrollBarCtrl( SB_VERT );
	if( nBar == SB_HORZ || nBar == SB_BOTH )
	{
		if( (!bEnable) && ptMove.x != 0 )
		{
			ptMove.x = 0;
			OnSwSetScrollPos( ptMove );
		} // if( (!bEnable) && ptMove.x != 0 )
		if( pScrollBarWndH != NULL )
		{
			if( (dwWndStyle & WS_HSCROLL) != 0 )
				CWnd::ShowScrollBar( SB_HORZ, FALSE );
			bool bAreadyEnabled = pScrollBarWndH->IsWindowEnabled() ? true : false;
			if(	!(		( bAreadyEnabled && bEnable )
					||	( (!bAreadyEnabled) && (!bEnable) )
					)
				)
			{
				pScrollBarWndH->EnableWindow( bEnable ? TRUE : FALSE );
				if( OnSwCanAutoHideScrollBar(true) )
					pScrollBarWndH->ShowWindow( bEnable ? SW_SHOW : SW_HIDE );
				else if( bEnable && (pScrollBarWndH->GetStyle()&WS_VISIBLE) == 0 )
					pScrollBarWndH->ShowWindow( SW_SHOW );
			}
		} // if( pScrollBarWndH != NULL )
		else
		{
			if( OnSwCanAutoHideScrollBar(true) )
				CWnd::ShowScrollBar(
					SB_HORZ,
					bEnable ? TRUE : FALSE
					);
			else
				CWnd::EnableScrollBar(
					SB_HORZ,
					bEnable ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH
					);
		} // else from if( pScrollBarWndH != NULL )
	} // if( nBar == SB_HORZ || nBar == SB_BOTH )
	if( nBar == SB_VERT || nBar == SB_BOTH )
	{
		if( (!bEnable) && ptMove.y != 0 )
		{
			ptMove.y = 0;
			OnSwSetScrollPos( ptMove );
		} // if( (!bEnable) && ptMove.y != 0 )
		if( pScrollBarWndV != NULL )
		{
			if( (dwWndStyle & WS_VSCROLL) != 0 )
				CWnd::ShowScrollBar( SB_VERT, FALSE );
			bool bAreadyEnabled = pScrollBarWndV->IsWindowEnabled() ? true : false;
			if(	!(		( bAreadyEnabled && bEnable )
					||	( (!bAreadyEnabled) && (!bEnable) )
					)
				)
			{
				pScrollBarWndV->EnableWindow( bEnable ? TRUE : FALSE );
				if( OnSwCanAutoHideScrollBar(false) )
					pScrollBarWndV->ShowWindow( bEnable ? SW_SHOW : SW_HIDE );
				else if( bEnable && (pScrollBarWndV->GetStyle()&WS_VISIBLE) == 0 )
					pScrollBarWndV->ShowWindow( SW_SHOW );
			}
		} // if( pScrollBarWndV != NULL )
		else
		{
			if( OnSwCanAutoHideScrollBar(false) )
				CWnd::ShowScrollBar(
					SB_VERT,
					bEnable ? TRUE : FALSE
					);
			else
				CWnd::EnableScrollBar(
					SB_VERT,
					bEnable ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH
					);
		} // else from if( pScrollBarWndV != NULL )
	} // if( nBar == SB_VERT || nBar == SB_BOTH )
CExtScrollBar * pExtScrollBarWndH = DYNAMIC_DOWNCAST( CExtScrollBar, pScrollBarWndH );
CExtScrollBar * pExtScrollBarWndV = DYNAMIC_DOWNCAST( CExtScrollBar, pScrollBarWndV );
	if( pExtScrollBarWndH != NULL && pExtScrollBarWndV != NULL )
	{
		pExtScrollBarWndH->SyncReservedSpace( pExtScrollBarWndV );
		pExtScrollBarWndV->SyncReservedSpace( pExtScrollBarWndH );
	} // if( pExtScrollBarWndH != NULL && pExtScrollBarWndV != NULL )
}

void CExtScrollWnd::OnSwSetScrollPos( POINT pt )
{
	ASSERT_VALID( this );
	ASSERT( pt.x >= 0 && pt.y >= 0 );
int xOrgValue = ScrollPos32Get( SB_HORZ );
	ScrollPos32Set( SB_HORZ, pt.x );
int yOrgValue = ScrollPos32Get( SB_VERT );
	ScrollPos32Set( SB_VERT, pt.y );
	OnSwDoScrollWindow(
		xOrgValue - pt.x,
		yOrgValue - pt.y
		);
}

UINT CExtScrollWnd::OnSwCalcMouseScrollLines(
	UINT fFlags,
	short zDelta,
	CPoint point
	)
{
	ASSERT_VALID( this );
	fFlags;
	zDelta;
	point;
	return g_PaintManager.GetMouseWheelScrollLines();
}

bool CExtScrollWnd::OnSwDoMouseWheel(
	UINT fFlags,
	short zDelta,
	CPoint point
	)
{
	ASSERT_VALID( this );
	point;
	if(		(fFlags&(MK_CONTROL)) != 0
		||	CExtToolControlBar::g_bMenuTracking
		||	CExtPopupMenuWnd::IsMenuTracking()
		)
		return false;

	if( ! ( OnSwHasScrollBar(true) || OnSwHasScrollBar(false) ) )
		return false;
bool bResult = false;
UINT uWheelScrollLines =
		OnSwCalcMouseScrollLines(
			fFlags,
			zDelta,
			point
			);
	if(		OnSwHasScrollBar(false)
		&&	( (fFlags&(MK_SHIFT)) == 0 )
		)
	{
		int nDisplacement;
		int nToScroll =
			::MulDiv( -zDelta, uWheelScrollLines, WHEEL_DELTA );
		if(		nToScroll == -1
			||	uWheelScrollLines == WHEEL_PAGESCROLL
			)
		{
			nDisplacement = OnSwGetPageSize( -1 ).cy;
			if( zDelta > 0 )
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement =
				nToScroll * OnSwGetLineSize( 1 ).cy;
			nDisplacement =
				min(
					nDisplacement,
					OnSwGetPageSize( 1 ).cy
					);
		}
		bResult =
			OnSwDoScrollBy(
				CSize( 0, nDisplacement ),
				true
				);
	}
	else if(
			OnSwHasScrollBar( true )
		&&	( (fFlags&(MK_SHIFT)) != 0 )
		)
	{
		int nDisplacement;
		int nToScroll =
			::MulDiv( -zDelta, uWheelScrollLines, WHEEL_DELTA );
		if(		nToScroll == -1
			||	uWheelScrollLines == WHEEL_PAGESCROLL
			)
		{
			nDisplacement = OnSwGetPageSize( -1 ).cx;
			if( zDelta > 0 )
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement =
				nToScroll * OnSwGetLineSize( 1 ).cx;
			nDisplacement =
				min(
					nDisplacement,
					OnSwGetPageSize( 1 ).cx
					);
		}
		bResult =
			OnSwDoScrollBy(
				CSize( nDisplacement, 0 ),
				true
				);
	}

	if( bResult )
	{
		OnSwInvalidate( m_bScrollErase );
		if( m_bScrollUpdateWindow )
			CWnd::UpdateWindow();
	}

	return bResult;
}

bool CExtScrollWnd::OnSwDoScroll(
	UINT nScrollCode,
	UINT nPos,
	bool bDoScroll // = true
	)
{
	ASSERT_VALID( this );
LONG xPos, xOrgValue, yPos, yOrgValue;
	xOrgValue = xPos = ScrollPos32Get( SB_HORZ );
	switch( LOBYTE(nScrollCode) )
	{
	case SB_TOP:		xPos  = 0;							break;
	case SB_BOTTOM:		xPos  = __EXT_SCROLL_NUMERIC_MAX;	break;
	case SB_LINEUP:		xPos -= OnSwGetLineSize( -1 ).cx;	break;
	case SB_LINEDOWN:	xPos += OnSwGetLineSize(  1 ).cx;	break;
	case SB_PAGEUP:		xPos -= OnSwGetPageSize( -1 ).cx;	break;
	case SB_PAGEDOWN:	xPos += OnSwGetPageSize(  1 ).cx;	break;
	case SB_THUMBTRACK:
		if( ! OnSwQueryThumbTrackEnabled(true) )
			break;
	case SB_THUMBPOSITION:
			if( m_bUse32BitScrollInfo )
				nPos = ScrollPos32Get( SB_HORZ, true );
			xPos  = nPos;
		break;
	}
	yOrgValue = yPos = ScrollPos32Get( SB_VERT );
	switch( HIBYTE(nScrollCode) )
	{
	case SB_TOP:		yPos  = 0;							break;
	case SB_BOTTOM:		yPos  = __EXT_SCROLL_NUMERIC_MAX;	break;
	case SB_LINEUP:		yPos -= OnSwGetLineSize( -1 ).cy;	break;
	case SB_LINEDOWN:	yPos += OnSwGetLineSize(  1 ).cy;	break;
	case SB_PAGEUP:		yPos -= OnSwGetPageSize( -1 ).cy;	break;
	case SB_PAGEDOWN:	yPos += OnSwGetPageSize(  1 ).cy;	break;
	case SB_THUMBTRACK:
		if( ! OnSwQueryThumbTrackEnabled(false) )
			break;
	case SB_THUMBPOSITION:
			if( m_bUse32BitScrollInfo )
				nPos = ScrollPos32Get( SB_VERT, true );
			yPos  = nPos;
		break;
	}
bool bResult =
		OnSwDoScrollBy(
			CSize( xPos - xOrgValue, yPos - yOrgValue ),
			bDoScroll
			);
	if( bResult && bDoScroll && m_bScrollUpdateWindow )
		CWnd::UpdateWindow();
	return bResult;
}

bool CExtScrollWnd::OnSwDoScrollBy(
	CSize sizeScroll,
	bool bDoScroll // = true
	)
{
	ASSERT_VALID( this );
int xMaxValue, xOrgValue, xPos, yMaxValue, yOrgValue, yPos;
	if( ! OnSwHasScrollBar(false) )
		sizeScroll.cy = 0;
	if( ! OnSwHasScrollBar(true) )
		sizeScroll.cx = 0;
	// adjust current positions
	xOrgValue = xPos = ScrollPos32Get( SB_HORZ );
	xMaxValue = ScrollLimit32Get( SB_HORZ );
	xPos += sizeScroll.cx;
	if( xPos < 0 )
		xPos = 0;
	else if( xPos > xMaxValue )
		xPos = xMaxValue;
	yOrgValue = yPos = ScrollPos32Get( SB_VERT );
	yMaxValue = ScrollLimit32Get( SB_VERT );
	yPos += sizeScroll.cy;
	if( yPos < 0 )
		yPos = 0;
	else if( yPos > yMaxValue )
		yPos = yMaxValue;
	if( xPos == xOrgValue && yPos == yOrgValue )
		return false;
	if( bDoScroll )
	{
		OnSwDoScrollWindow(
			xOrgValue - xPos,
			yOrgValue - yPos
			);
		if( xPos != xOrgValue )
			ScrollPos32Set( SB_HORZ, xPos );
		if( yPos != yOrgValue )
			ScrollPos32Set( SB_VERT, yPos );
	}
	return true;
}

CSize CExtScrollWnd::OnSwGetScrollBarSizes() const
{
	ASSERT_VALID( this );
CSize sizeSb( 0, 0 );
DWORD dwWndStyle = CWnd::GetStyle();
	if( GetScrollBarCtrl(SB_HORZ) == NULL )
	{
		sizeSb.cx = afxData.cxVScroll;
		if( dwWndStyle & WS_BORDER )
			sizeSb.cx -= CX_BORDER;
	} // if( GetScrollBarCtrl(SB_HORZ) == NULL )
	if( GetScrollBarCtrl(SB_VERT) == NULL )
	{
		sizeSb.cy = afxData.cyHScroll;
		if( dwWndStyle & WS_BORDER )
			sizeSb.cy -= CY_BORDER;
	} // if( GetScrollBarCtrl(SB_VERT) == NULL )
	return sizeSb;
}

bool CExtScrollWnd::OnSwCalcClientSizes(
	CSize & size,
	CSize & sizeSb
	)
{
	// return true if enough room to add scrollbars if needed
	ASSERT_VALID( this );
CRect rcClient = OnSwGetClientRect();
	size = rcClient.Size();
	sizeSb = OnSwGetScrollBarSizes();
	if(		sizeSb.cx != 0
		&&	OnSwHasScrollBar( false )
		)
		size.cx += sizeSb.cx;
	if(		sizeSb.cy != 0
		&&	OnSwHasScrollBar( true )
		)
		size.cy += sizeSb.cy;
	return
		( size.cx > sizeSb.cx && size.cy > sizeSb.cy )
			? true : false;
}

bool CExtScrollWnd::OnSwQueryThumbTrackEnabled( bool bHorz ) const
{
	ASSERT_VALID( this );
	bHorz;
	return true;
}

void CExtScrollWnd::OnSwGetScrollBarState(
	CSize sizeClient,
	CSize & sizeNeedSB,
	CSize & sizeRange,
	CPoint & ptMove,
	bool bInsideClient
	) const
{
	ASSERT_VALID( this );
CSize sizeSb = OnSwGetScrollBarSizes();
	sizeRange = OnSwGetTotalSize() - sizeClient;
	ptMove = OnSwGetScrollPos();
bool bNeedH = sizeRange.cx > 0;
	if( !bNeedH )
		ptMove.x = 0;
	else if( bInsideClient )
		sizeRange.cy += sizeSb.cy;
bool bNeedV = sizeRange.cy > 0;
	if( !bNeedV )
		ptMove.y = 0;
	else if ( bInsideClient )
		sizeRange.cx += sizeSb.cx;
	if( bNeedV && (!bNeedH) && sizeRange.cx > 0 )
	{
		ASSERT( bInsideClient );
		bNeedH = true;
		sizeRange.cy += sizeSb.cy;
	} // if( bNeedV && (!bNeedH) && sizeRange.cx > 0 )
	if( sizeRange.cx > 0 && ptMove.x >= sizeRange.cx )
		ptMove.x = sizeRange.cx;
	if( sizeRange.cy > 0 && ptMove.y >= sizeRange.cy )
		ptMove.y = sizeRange.cy;
	sizeNeedSB.cx = bNeedH;
	sizeNeedSB.cy = bNeedV;
}

void CExtScrollWnd::OnSwUpdateScrollBars()
{
	ASSERT_VALID( this );
	if( m_nUpdateScrollBars > 1 )
		return;
	m_nUpdateScrollBars ++;

	ASSERT( OnSwGetTotalSize().cx >= 0 && OnSwGetTotalSize().cy >= 0 );
CRect rcClient;
bool bCalcClient = true;
CWnd * pParentWnd = GetParent();
	if(		pParentWnd != NULL
		&&	(BOOL)pParentWnd->SendMessage(
				WM_RECALCPARENT,
				0,
				(LPARAM)(LPCRECT)&rcClient
				) != 0
		)
		bCalcClient = false;
CSize sizeClient;
CSize sizeSb;
	if( bCalcClient )
	{
		if( !OnSwCalcClientSizes( sizeClient, sizeSb ) )
		{ // no room for scroll bars
			CRect rcClient2 = OnSwGetClientRect();
			if( rcClient2.Width() > 0 && rcClient2.Height() > 0 )
			{
				OnSwEnableScrollBarCtrl( SB_BOTH, false );
				OnSwRecalcLayout( true );
			}
			m_nUpdateScrollBars --;
			return;
		}
	}
	else
	{ // let parent window determine the "client" rect
		sizeSb = OnSwGetScrollBarSizes();
		sizeClient.cx = rcClient.right - rcClient.left;
		sizeClient.cy = rcClient.bottom - rcClient.top;
	}

	// if enough room to add scrollbars
CSize sizeRange;
CPoint ptMove;
CSize sizeNeedSB;
	OnSwGetScrollBarState(
		sizeClient,
		sizeNeedSB,
		sizeRange,
		ptMove,
		bCalcClient
		);
	if( sizeNeedSB.cx )
		sizeClient.cy -= sizeSb.cy;
	if( sizeNeedSB.cy )
		sizeClient.cx -= sizeSb.cx;

	// scroll window + update
	OnSwSetScrollPos( ptMove );

	// the scrollbar page range
SCROLLINFO _scroll_info;
	::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
	_scroll_info.cbSize = sizeof(SCROLLINFO);
	_scroll_info.fMask = SIF_PAGE|SIF_RANGE;
	_scroll_info.nMin = 0;
	// update bars
	OnSwEnableScrollBarCtrl( SB_HORZ, sizeNeedSB.cx ? true : false );
	if( sizeNeedSB.cx )
	{
		_scroll_info.nPage = sizeClient.cx;
		_scroll_info.nMax = OnSwGetTotalSize().cx - 1;
		if( ! OnSwScrollInfoAdjust(
				SB_HORZ,
				_scroll_info,
				true
				)
			)
			OnSwSetScrollRange(
				SB_HORZ,
				0,
				sizeRange.cx,
				true
				);
	} // if( sizeNeedSB.cx )
	OnSwEnableScrollBarCtrl( SB_VERT, sizeNeedSB.cy ? true : false );
	if( sizeNeedSB.cy )
	{
		_scroll_info.nPage = sizeClient.cy;
		_scroll_info.nMax = OnSwGetTotalSize().cy - 1;
		if( ! OnSwScrollInfoAdjust(
				SB_VERT,
				_scroll_info,
				true
				)
			)
			OnSwSetScrollRange(
				SB_VERT,
				0,
				sizeRange.cy,
				true
				);
	} // if( sizeNeedSB.cy )
	OnSwRecalcLayout( true );
	m_nUpdateScrollBars --;
}

void CExtScrollWnd::OnSwDoScrollWindow(
	int xAmount,
	int yAmount,
	LPCRECT lpRect, // = NULL
	LPCRECT lpClipRect // = NULL
	)
{
	ASSERT_VALID( this );
	if( m_bScrollPhysical )
		CWnd::ScrollWindow(
			xAmount,
			yAmount,
			lpRect,
			lpClipRect
			);
	if( m_bScrollInvalidate )
		OnSwInvalidate( m_bScrollErase );
	if( m_bScrollUpdateWindow )
		CWnd::UpdateWindow();
}

void CExtScrollWnd::OnSwInvalidate( bool bErase )
{
	ASSERT_VALID( this );
CScrollBar * pScrollBarWndH = GetScrollBarCtrl( SB_HORZ );
CScrollBar * pScrollBarWndV = GetScrollBarCtrl( SB_VERT );
	if(		pScrollBarWndH != NULL
		&&	( pScrollBarWndH->GetStyle() & WS_VISIBLE ) == 0
		)
		pScrollBarWndH = NULL;
	if(		pScrollBarWndV != NULL
		&&	( pScrollBarWndV->GetStyle() & WS_VISIBLE ) == 0
		)
		pScrollBarWndV = NULL;
	if( pScrollBarWndH == NULL && pScrollBarWndV == NULL )
	{
		CWnd::Invalidate( bErase ? TRUE : FALSE );
		return;
	} // if( pScrollBarWndH == NULL && pScrollBarWndV == NULL )
CRect rcClientReal;
	CWnd::GetClientRect( &rcClientReal );
CRgn rgnInvalidate;
	if( !rgnInvalidate.CreateRectRgnIndirect(&rcClientReal) )
	{
		CWnd::Invalidate( bErase ? TRUE : FALSE );
		return;
	}
	if( pScrollBarWndH != NULL )
	{
		CRect rcBar;
		pScrollBarWndH->GetWindowRect( &rcBar );
		CWnd::ScreenToClient( &rcBar );
		CRgn rgnBar;
		if(		(! rgnBar.CreateRectRgnIndirect(&rcBar) )
			||	rgnInvalidate.CombineRgn(
					&rgnInvalidate,
					&rgnBar,
					RGN_DIFF
					) == ERROR
			)
		{
			CWnd::Invalidate( bErase ? TRUE : FALSE );
			return;
		}
	} // if( pScrollBarWndH != NULL )
	if( pScrollBarWndV != NULL )
	{
		CRect rcBar;
		pScrollBarWndV->GetWindowRect( &rcBar );
		CWnd::ScreenToClient( &rcBar );
		CRgn rgnBar;
		if(		(! rgnBar.CreateRectRgnIndirect(&rcBar) )
			||	rgnInvalidate.CombineRgn(
					&rgnInvalidate,
					&rgnBar,
					RGN_DIFF
					) == ERROR
			)
		{
			CWnd::Invalidate( bErase ? TRUE : FALSE );
			return;
		}
	} // if( pScrollBarWndV != NULL )
	CWnd::InvalidateRgn( &rgnInvalidate, bErase ? TRUE : FALSE );
}

void CExtScrollWnd::OnSwDoRedraw()
{
	ASSERT_VALID( this );
	if( m_bRedrawInvalidate )
		OnSwInvalidate( m_bRedrawErase );
	if( m_bRedrawUpdateWindow )
		CWnd::UpdateWindow();
}

void CExtScrollWnd::OnSwPaint( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	dc;
}

/////////////////////////////////////////////////////////////////////////////
// CExtScrollWnd message handlers

void CExtScrollWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if( nType != SIZE_MINIMIZED )
	{
		OnSwRecalcLayout( true );
		OnSwUpdateScrollBars();
		OnSwDoRedraw();
	}
}

void CExtScrollWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(		pScrollBar != NULL
		&&	pScrollBar->SendChildNotifyLastMsg()
		)
		return;
	if( pScrollBar != GetScrollBarCtrl(SB_HORZ) )
		return;
	OnSwDoScroll( MAKEWORD(nSBCode, -1), nPos );
}

void CExtScrollWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(		pScrollBar != NULL
		&&	pScrollBar->SendChildNotifyLastMsg()
		)
		return;
	if( pScrollBar != GetScrollBarCtrl(SB_VERT) )
		return;
	OnSwDoScroll( MAKEWORD(-1, nSBCode), nPos );
}

BOOL CExtScrollWnd::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	return
		OnSwDoMouseWheel( fFlags, zDelta, point )
			? true : false;
}

BOOL CExtScrollWnd::OnEraseBkgnd(CDC* pDC) 
{
	if( m_bEatErasing )
		return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CExtScrollWnd::OnPaint() 
{
bool bSingleBufferPaint = false;
CPaintDC dcPaint( this );
CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint.m_hDC,
		m_hWnd,
		CExtPaintManager::stat_DefExcludeChildAreaCallback
		);
CPoint ptSp = OnSwGetScrollPaintPos();
	if( m_bBufferedPainting )
	{
		CRect rcRealClient;
		CWnd::GetClientRect( &rcRealClient );
		CExtMemoryDC dcCompat( &dcPaint, &rcRealClient );
		ASSERT( dcCompat.GetSafeHdc() != NULL );
		if( dcCompat.GetSafeHdc() != NULL )
		{
			CPoint ptVpOld( dcCompat.SetViewportOrg(-ptSp) );
			OnSwPaint( dcCompat );
			dcCompat.SetViewportOrg( ptVpOld );
		} // if( dcCompat.GetSafeHdc() != NULL )
		else
			bSingleBufferPaint = true;
	} // if( m_bBufferedPainting )
	else
		bSingleBufferPaint = true;
	if( bSingleBufferPaint )
	{
		CPoint ptVpOld( dcPaint.SetViewportOrg(-ptSp) );
		OnSwPaint( dcPaint );
		dcPaint.SetViewportOrg( ptVpOld );
	} // if( bSingleBufferPaint )
}

BOOL CExtScrollWnd::PreTranslateMessage(MSG* pMsg) 
{
	if(		pMsg->message == WM_MOUSEWHEEL
		&&	GetSafeHwnd() != NULL
		&&	::IsWindow( GetSafeHwnd() )
		&&	CExtPopupMenuWnd::TestHoverEnabledFromActiveHWND(
				GetSafeHwnd()
				)
		)
	{
		CPoint point = pMsg->lParam;
		HWND hWnd = ::WindowFromPoint( point );
		if( hWnd != NULL && hWnd == m_hWnd )
		{
			UINT fFlags = LOWORD(pMsg->wParam);
			short zDelta = HIWORD(pMsg->wParam);
			if( OnSwDoMouseWheel( fFlags, zDelta, point ) )
				return TRUE;
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CExtScrollWnd::OnSwEnshurePointAvail( CPoint point )
{
	ASSERT_VALID( this );
	if( GetSafeHwnd() == NULL)
		return;
	if( ! ::IsWindow( GetSafeHwnd() ) )
		return;
	if( ! ( OnSwHasScrollBar(false) || OnSwHasScrollBar(true) ) )
		return;
CRect rcClient;
	CWnd::GetClientRect( &rcClient );
	if( rcClient.PtInRect(point) )
		return;

CPoint ptScrollOrg( OnSwGetScrollPos() );
CPoint ptScroll( ptScrollOrg );
CSize sizeTotal( OnSwGetTotalSize() );

	if( OnSwHasScrollBar(true) )
	{
		if( point.x < 0 )
			ptScroll.x += point.x;
		else if( point.x > rcClient.Width() )
			ptScroll.x += point.x - rcClient.Width();
		if( ptScroll.x < 0 )
			ptScroll.x = 0;
		else if( ptScroll.x > sizeTotal.cx )
			ptScroll.x = sizeTotal.cx;
	} // if( OnSwHasScrollBar(true) )
	if( OnSwHasScrollBar(false) )
	{
		if( point.y < 0 )
			ptScroll.y += point.y;
		else if( point.y > rcClient.Height() )
			ptScroll.y += point.y - rcClient.Height();
		if( ptScroll.y < 0 )
			ptScroll.y = 0;
		else if( ptScroll.y > sizeTotal.cy )
			ptScroll.y = sizeTotal.cy;
	} // if( OnSwHasScrollBar(false) )

	if( ptScrollOrg != ptScroll )
		OnSwSetScrollPos( ptScroll );
}


#endif // (!defined __EXT_MFC_NO_SCROLLWND)

