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

#if( !defined __EXTMINIDOCKFRAMEWND_H)
	#include "ExtMiniDockFrameWnd.h"
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	#if (!defined __EXT_CONTROLBAR_TABBED_FEATURES_H)
		#include "ExtControlBarTabbedFeatures.h"
	#endif // __EXT_CONTROLBAR_TABBED_FEATURES_H
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define __REMOVED_NONFIXMODE_STYLES \
	FWS_SNAPTOBARS

#define __REMOVED_STYLES_NORMAL \
	(MFS_4THICKFRAME|WS_THICKFRAME|WS_CAPTION|WS_BORDER|WS_DLGFRAME)
//|WS_SYSMENU

#define __REMOVED_STYLES_EXTENDED \
	(WS_EX_TOOLWINDOW|WS_EX_STATICEDGE|WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME)

#define __SWP_FOR_REMOVED_STYLES \
	(SWP_FRAMECHANGED|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_NOSENDCHANGING|SWP_NOREDRAW)

/////////////////////////////////////////////////////////////////////////////
// CExtMiniDockFrameWnd

bool CExtMiniDockFrameWnd::g_bOnWndPosChgProcessing = false;
bool CExtMiniDockFrameWnd::g_bAutoMakeTopmostOnMouseEvent = false;

IMPLEMENT_DYNCREATE(CExtMiniDockFrameWnd, CMiniDockFrameWnd);

BEGIN_MESSAGE_MAP(CExtMiniDockFrameWnd, CMiniDockFrameWnd)
    //{{AFX_MSG_MAP(CExtMiniDockFrameWnd)
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCRBUTTONDOWN()
    ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CExtMiniDockFrameWnd::Create(CWnd* pParent, DWORD dwBarStyle)
{
//	if(!MDFW_Create(pParent, dwBarStyle))
//		return FALSE;
	if( !CMiniDockFrameWnd::Create(pParent, dwBarStyle))
		return FALSE;

	if( GetStyle() & __REMOVED_STYLES_NORMAL )
		ModifyStyle( __REMOVED_STYLES_NORMAL, 0, __SWP_FOR_REMOVED_STYLES );
	if( GetExStyle() & __REMOVED_STYLES_EXTENDED )
		ModifyStyleEx( __REMOVED_STYLES_EXTENDED, 0, __SWP_FOR_REMOVED_STYLES );
	
	SetTimer( 1, 1, NULL );
	EnableToolTips();
	m_wndToolTip.Create( this );
	m_wndToolTip.Activate( TRUE );

	return TRUE;
}

void CExtMiniDockFrameWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}
	
	if( CExtControlBar::_DraggingGetBar() != NULL )
		return;

CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
CPoint ptWnd( point + ptDevOffset );

CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL && pExtBar->m_bNcAreaBtnTracking )
	{
		INT nCountOfNcButtons = pExtBar->NcButtons_GetCount();
		for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		{
			CExtBarNcAreaButton * pBtn =
				pExtBar->NcButtons_GetAt( nBtn );
			if( !pBtn->OnQueryVisibility() )
				continue;
			if( pBtn->OnNcAreaClicked( ptWnd ) )
				return;
		} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		return;
	} // if( pExtBar != NULL && pExtBar->m_bNcAreaBtnTracking )

	if( m_rcBtnHideBar.PtInRect( ptWnd ) )
	{
		CControlBar * pBar =
			GetControlBar();
		if( pBar == NULL )
			return;
		ASSERT_VALID( pBar );
		GetParentFrame()->
			ShowControlBar(
				pBar,
				FALSE,
				FALSE
				);
		return;
	}

	CMiniDockFrameWnd::OnNcLButtonUp(nHitTest, point);
}

void CExtMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		pSite->DropTargetPopupCancel();
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

CExtControlBar * pExtBar = GetControlBarExt();
CPoint p(point);
CRect rcWnd;
	GetWindowRect( &rcWnd );
	p.Offset( -rcWnd.left, -rcWnd.top );
	if( m_rcBtnHideBar.PtInRect( p ) )
		return;
	if( nHitTest == HTCAPTION )
	{
		ActivateTopParent();
		if(		pExtBar != NULL
			&&	(! pExtBar->_IsDockSiteCustomizeMode() )
			)
			BringWindowToTop();
		if( (m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0 )
		{
			int nPos = 1;
			CControlBar* pBar = NULL;
			while(
				pBar == NULL
				&& nPos < m_wndDockBar.m_arrBars.GetSize()
				)
				pBar =
					reinterpret_cast < CExtDockBar & >
						(m_wndDockBar). GetDockedControlBar(nPos++);
			ASSERT(pBar != NULL);
			ASSERT_KINDOF( CControlBar, pBar );
			ASSERT( pBar->m_pDockContext != NULL );
			if( pExtBar == NULL
				|| (	pExtBar != NULL
					&&	pExtBar->IsFixedMode()
					&&	!pExtBar->_IsShowContentWhenDragging()
					)
				)
			{
				pBar->m_pDockContext->StartDrag(point);
				return;
			}
			int nCyGripper =
				min(
					pExtBar->m_rcGrip.Width(),
					pExtBar->m_rcGrip.Height()
					);
			pExtBar->_DraggingStart(
				point,
				point
					- rcWnd.TopLeft()
					- CPoint( nCyGripper, nCyGripper/2 )
				);
			return;
		}
	}
	else if( nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST )
	{
		ActivateTopParent();
		int nPos = 1;
		CControlBar * pBar = NULL;
		while(
			pBar == NULL
			&& nPos < m_wndDockBar.m_arrBars.GetSize()
			)
			pBar =
				reinterpret_cast < CExtDockBar & >
					(m_wndDockBar).GetDockedControlBar(nPos++);
		ASSERT(pBar != NULL);
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		ASSERT( (m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0 );
		if(	!pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			CMiniDockFrameWnd::OnNcLButtonDown( nHitTest, point);
		else
		{
			CExtControlBar * pExtBar =
				(CExtControlBar *)pBar;
			if( !pExtBar->_IsShowContentWhenDragging()
				&&	pExtBar->IsFixedMode()
				)
			{
				pBar->m_pDockContext->StartResize(nHitTest,point);
				return;
			}
			ClientToScreen( &point );
			_ResizingStart( nHitTest, point );
		}
		return;
	}
	else if( nHitTest == HTMINBUTTON
		|| nHitTest == HTMAXBUTTON
		|| nHitTest == HTGROWBOX
		)
		return;
	
	CMiniFrameWnd::OnNcLButtonDown( nHitTest, point );
}

void CExtMiniDockFrameWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	if( nHitTest == HTCAPTION )
	{
		CRect rcWnd;
		GetWindowRect( &rcWnd );
		CPoint ptTestBtnHideBar( point - rcWnd.TopLeft() );
		if( m_rcBtnHideBar.PtInRect(ptTestBtnHideBar) )
			return;
	} // if( nHitTest == HTCAPTION )

CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL )
	{
		ASSERT_VALID( pExtBar );
		pExtBar->ToggleDocking();
		return;
	}

	CMiniDockFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CExtMiniDockFrameWnd::_ResizingStart(
	UINT nHitTest,
	const CPoint & point
	)
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	ASSERT( m_nResizingMode == HTNOWHERE );
	ASSERT( IsWindowVisible() );

	CExtPopupMenuWnd::CancelMenuTracking();
	CWnd::CancelToolTips();

CExtControlBar * pExtBar = GetControlBarExt();
	ASSERT_VALID( pExtBar );
	if( !pExtBar->_IsShowContentWhenDragging()
		&&	pExtBar->IsFixedMode()
		)
	{
		m_nResizingMode = HTNOWHERE;
		pExtBar->m_pDockContext->StartResize(nHitTest, point);
		return;
	}

	switch( nHitTest )
	{
	case HTLEFT:
	case HTRIGHT:
	case HTTOP:
	case HTTOPLEFT:
	case HTTOPRIGHT:
	case HTBOTTOM:
	case HTBOTTOMLEFT:
	case HTBOTTOMRIGHT:
		m_nResizingMode = nHitTest;
	break;
	default:
		return;
	} // switch( nHitTest )

#ifdef _DEBUG
int nPos = 1;
CControlBar * pDebugDummyBar = NULL;
	while(
		pDebugDummyBar == NULL
		&& nPos < m_wndDockBar.m_arrBars.GetSize()
		)
		pDebugDummyBar =
			reinterpret_cast<CExtDockBar&>
				(m_wndDockBar).
					GetDockedControlBar(nPos++);
	ASSERT(pDebugDummyBar != NULL);
	ASSERT_KINDOF(CExtControlBar, pDebugDummyBar);
	ASSERT(pDebugDummyBar->m_pDockContext != NULL);
	// CBRS_SIZE_DYNAMIC toolbars cannot have the CBRS_FLOAT_MULTI style
	ASSERT((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0);
#endif // _DEBUG
	
	ASSERT( pExtBar == pDebugDummyBar );
	ASSERT(pExtBar->m_pDockContext != NULL);

	if( !pExtBar->IsFixedMode() )
		ModifyStyle( __REMOVED_NONFIXMODE_STYLES, 0 );
	BringWindowToTop();
	CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
	GetWindowRect( &m_rcWndResizingStart );

	while( m_nResizingMode != HTNOWHERE )
	{
		::WaitMessage();
		MSG msg;
		// Process all the messages in the message queue
		while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		{
			bool bStop = false;
			switch( msg.message )
			{
			case WM_ACTIVATEAPP:
			case WM_COMMAND:
				bStop = true;
			break;
			default:
				if(	WM_KEYFIRST <= msg.message
					&& msg.message <= WM_KEYLAST
					)
				bStop = true;
			break;
			} // switch( msg.message )
			if( (!bStop) &&
				CExtMouseCaptureSink::GetCapture() != GetSafeHwnd()
				)
				bStop = true;
			if( bStop )
			{
				_ResizingEnd();
				return;
			}
			if( !AfxGetThread()->PumpMessage() )
			{
				PostQuitMessage(0);
				return; // Signal WM_QUIT received
			} // if( !AfxGetThread()->PumpMessage() )
		} // while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		
		if( CExtControlBar::g_bEnableOnIdleCalls )
		{
			for(	LONG nIdleCounter = 0L;
					::AfxGetThread()->OnIdle(nIdleCounter);
					nIdleCounter ++
					);
		}
	} // while( m_nResizingMode != HTNOWHERE )
}

void CExtMiniDockFrameWnd::_ResizingEnd()
{
	m_ptLastResizing.x = m_ptLastResizing.y = -1;
	//ASSERT( m_nResizingMode != HTNOWHERE );
	if( m_nResizingMode == HTNOWHERE )
		return;
	if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
		CExtMouseCaptureSink::ReleaseCapture();
CExtControlBar * pExtBar = GetControlBarExt();
	ASSERT_VALID( pExtBar );
	if( !pExtBar->IsFixedMode() )
		ModifyStyle( 0, __REMOVED_NONFIXMODE_STYLES );
	m_nResizingMode = HTNOWHERE;
}

bool CExtMiniDockFrameWnd::g_bInResizingUpdateState = false;

void CExtMiniDockFrameWnd::_ResizingUpdateState(
	const CPoint & point
	)
{
	if(		g_bInResizingUpdateState
		||	m_ptLastResizing == point
		)
		return;
	m_ptLastResizing = point;
	g_bInResizingUpdateState = true;

CPoint _pt( point );
	ASSERT( m_nResizingMode != HTNOWHERE );
CExtControlBar * pExtBar = GetControlBarExt();
	ASSERT_VALID( pExtBar );

	//BringWindowToTop();
CRect rcClient, rcClientOrg, rcWnd, rcWndOrg;
	GetClientRect( &rcClient );
	GetWindowRect( &rcWnd );
	rcClientOrg = rcClient;
	rcWndOrg = rcWnd;
CSize sizeDiff =
		rcWnd.Size() - rcClient.Size();
	ASSERT( sizeDiff.cx >= 0 && sizeDiff.cy >= 0 );
CSize sizeNcDiff( sizeDiff );

int nCalcMinHW = pExtBar->_CalcDesiredMinHW();
int nCalcMinVH = pExtBar->_CalcDesiredMinVH();
	if( sizeDiff.cx < nCalcMinHW )
		sizeDiff.cx = nCalcMinHW;
	if( sizeDiff.cy < nCalcMinVH )
		sizeDiff.cy = nCalcMinVH;

UINT nHit = m_nResizingMode;
bool	bLeft = false, bTop = false, bHorz = false,
		bInvTop = false, bInvLeft = false;
CPoint ptOffsetWnd( 0, 0 );
	if( pExtBar->IsFixedMode() )
	{
		switch( nHit )
		{
		case HTTOPLEFT:
		case HTTOPRIGHT:
			nHit = HTTOP;
		break;
		case HTBOTTOMLEFT:
		case HTBOTTOMRIGHT:
			nHit = HTBOTTOM;
		break;
		} // switch( nHit )
		switch( nHit )
		{
		case HTTOP:
			_pt.y =
				rcWndOrg.bottom
				+ rcWndOrg.top - _pt.y;
			nHit = HTBOTTOM;
			bInvLeft = true;
			bInvTop = true;
		break;
		case HTLEFT:
			_pt.x =
				rcWndOrg.right
				+ rcWndOrg.left - _pt.x;
			nHit = HTRIGHT;
			bInvLeft = true;
			bInvTop = true;
		break;
		} // switch( nHit )
	} // if( pExtBar->IsFixedMode() )
	if( bInvLeft || bInvTop || (!pExtBar->IsFixedMode()) )
	{
		CRect rcWnd( rcWndOrg );
		ScreenToClient( & rcWnd );
		if( bInvLeft || (!pExtBar->IsFixedMode()) )
			ptOffsetWnd.x =
				abs( rcWnd.left - rcClientOrg.left );
		if( bInvTop || (!pExtBar->IsFixedMode()) )
			ptOffsetWnd.y =
				abs( rcWnd.top - rcClientOrg.top );
	} // if( bInvLeft || bInvTop )

	switch( nHit )
	{
	case HTLEFT:
		bHorz = true;
		rcWnd.left = min( _pt.x, rcWnd.right );
		bLeft = true;
	break;
	case HTRIGHT:
		bHorz = true;
		rcWnd.right = max( _pt.x, rcWnd.left );
	break;
	case HTTOP:
		rcWnd.top = min( _pt.y, rcWnd.bottom );
		bTop = true;
	break;
	case HTTOPLEFT:
		rcWnd.left = min( _pt.x, rcWnd.right );
		rcWnd.top = min( _pt.y, rcWnd.bottom );
		bLeft = true;
		bTop = true;
	break;
	case HTTOPRIGHT:
		rcWnd.right = max( _pt.x, rcWnd.left );
		rcWnd.top = min( _pt.y, rcWnd.bottom );
		bTop = true;
	break;
	case HTBOTTOM:
		rcWnd.bottom = max( _pt.y, rcWnd.top );
	break;
	case HTBOTTOMLEFT:
		rcWnd.left = min( _pt.x, rcWnd.right );
		rcWnd.bottom = max( _pt.y, rcWnd.top );
		bLeft = true;
	break;
	case HTBOTTOMRIGHT:
		rcWnd.right = max( _pt.x, rcWnd.left );
		rcWnd.bottom = max( _pt.y, rcWnd.top );
	break;
	default:
		ASSERT( FALSE );
		g_bInResizingUpdateState = false;
		return;
	} // switch( nHit )

	if( rcWnd.Width() < sizeDiff.cx )
		rcWnd.right = rcWnd.left + sizeDiff.cx;
	if( rcWnd.Height() < sizeDiff.cy )
		rcWnd.bottom = rcWnd.top + sizeDiff.cy;
	if( rcWnd.right > rcWndOrg.right
		&& bLeft
		)
		rcWnd.OffsetRect(
			rcWndOrg.right - rcWnd.right,
			0
			);
	if( rcWnd.bottom > rcWndOrg.bottom
		&& bTop
		)
		rcWnd.OffsetRect(
			0,
			rcWndOrg.bottom - rcWnd.bottom
			);
	rcClient.SetRect(
		rcWnd.left - (rcClient.left-rcWndOrg.left),
		rcWnd.top - (rcClient.top-rcWndOrg.top),
		rcWnd.right - (rcClient.right-rcWndOrg.right),
		rcWnd.bottom - (rcClient.bottom-rcWndOrg.bottom)
		);

	if( !pExtBar->IsFixedMode() )
	{
		ASSERT( (GetStyle() & __REMOVED_NONFIXMODE_STYLES) == 0 );
		CSize _sizeMinFloated =
			pExtBar->_CalcDesiredMinFloatedSize();
		CSize _size = rcWnd.Size();
		if( _size.cx < _sizeMinFloated.cx )
			_size.cx = _sizeMinFloated.cx;
		if( _size.cy < _sizeMinFloated.cy )
			_size.cy = _sizeMinFloated.cy;
		CExtPaintManager::monitor_parms_t _mp;
		CExtPaintManager::stat_GetMonitorParms( _mp, this );
		CSize _sizeMaxFloated = _mp.m_rcWorkArea.Size();
		if( _size.cx > _sizeMaxFloated.cx )
			_size.cx = _sizeMaxFloated.cx;
		if( _size.cy > _sizeMaxFloated.cy )
			_size.cy = _sizeMaxFloated.cy;
		CRect rcWndValidated( rcWnd.TopLeft(), _size );
		pExtBar->m_sizeFloated = _size - ptOffsetWnd;
		MoveWindow( &rcWndValidated );
		SetTimer( 3, 1, NULL );
		g_bInResizingUpdateState = false;
		return;
	} // if( !pExtBar->IsFixedMode() )

int nWinResizingFrameDX = ::GetSystemMetrics( SM_CXSIZEFRAME );
int nWinResizingFrameDY = ::GetSystemMetrics( SM_CYSIZEFRAME );
DWORD dwMode = LM_HORZ|LM_COMMIT;
LONG nLendth = 0;
	if( bHorz )	
		nLendth = rcClient.Width();
	else
	{
		dwMode |= LM_LENGTHY;
		nLendth =
			rcClient.Height()
			- nWinResizingFrameDY;
	} // else from if( bHorz )	
CSize _sizeFloatedPrev = pExtBar->m_sizeFloated;
CSize _size =
		pExtBar->CalcDynamicLayout( nLendth, dwMode );
	if( _sizeFloatedPrev == _size )
	{
		g_bInResizingUpdateState = false;
		return;
	}

//	if( _size.cx < _sizeMinFloated.cx )
//		_size.cx = _sizeMinFloated.cx;
//	if( _size.cy < _sizeMinFloated.cy )
//		_size.cy = _sizeMinFloated.cy;

	pExtBar->m_sizeFloated = _size;

	if( bInvTop )
	{
		ASSERT( rcWnd.top == rcWndOrg.top );
		rcWnd.bottom = m_rcWndResizingStart.bottom;
		rcWnd.top =
			rcWnd.bottom - _size.cy - ptOffsetWnd.y;
	}
	if( bInvLeft )
	{
		ASSERT( rcWnd.left == rcWndOrg.left );
		rcWnd.right = m_rcWndResizingStart.right;
		rcWnd.left =
			rcWnd.right - _size.cx - ptOffsetWnd.x;
	}

	ASSERT( pExtBar->m_pDockSite != NULL );
	ASSERT( pExtBar->m_pDockBar != NULL );
	pExtBar->m_ptFloatHelper = rcWnd.TopLeft();

	_size += sizeNcDiff;
	_size.cx -= nWinResizingFrameDX; // (+ v.2.22)
	_size.cy -= nWinResizingFrameDY;
CRect rcToMoveMinFrame(
		pExtBar->m_ptFloatHelper,
		_size
		);

	MoveWindow( &rcToMoveMinFrame);
	SendMessage( WM_NCPAINT );
	RecalcLayout();
	pExtBar->RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE|RDW_UPDATENOW
			|RDW_ERASE|RDW_ERASENOW
			|RDW_ALLCHILDREN
		);
	g_bInResizingUpdateState = false;
}

CControlBar * CExtMiniDockFrameWnd::GetControlBar()
{
CWnd* pTempWnd = GetWindow( GW_CHILD );
    if( pTempWnd == NULL )
        return NULL;
	pTempWnd = pTempWnd->GetWindow( GW_CHILD );
    if( pTempWnd == NULL )
        return NULL;
    if( !pTempWnd->IsKindOf( RUNTIME_CLASS(CControlBar) ) )
        return NULL;
    return reinterpret_cast < CControlBar * >( pTempWnd );
}

const CControlBar * CExtMiniDockFrameWnd::GetControlBar() const
{
CControlBar * pBar =
		( const_cast < CExtMiniDockFrameWnd * > ( this ) ) ->
			GetControlBar();
	return pBar;
}

CExtControlBar * CExtMiniDockFrameWnd::GetControlBarExt()
{
CControlBar * pTempWnd = GetControlBar();
    if( pTempWnd == NULL )
        return NULL;
//	ASSERT_KINDOF( CExtControlBar, pTempWnd );
    if( !pTempWnd->IsKindOf( RUNTIME_CLASS(CExtControlBar) ) )
        return NULL;
    return reinterpret_cast < CExtControlBar * >( pTempWnd );
}

const CExtControlBar * CExtMiniDockFrameWnd::GetControlBarExt() const
{
CExtControlBar * pExtBar =
		( const_cast < CExtMiniDockFrameWnd * > ( this ) ) ->
			GetControlBarExt();
	return pExtBar;
}

void CExtMiniDockFrameWnd::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}
	if( nHitTest == HTCAPTION )
	{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeSite * pSite =
			CExtCustomizeSite::GetCustomizeSite( m_hWnd );
		if(		pSite != NULL
			&&	pSite->IsCustomizeMode()
			)
			return;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		if( CExtControlBar::FindHelpMode(this) )
			return;
		CExtControlBar * pExtBar =
			GetControlBarExt();
		if( pExtBar != NULL )
		{
			ASSERT_VALID( pExtBar );
			
			CFrameWnd * pFrame = pExtBar->_GetDockingFrameImpl();
			ASSERT_VALID( pFrame );

			CPoint point;
			if( ! ::GetCursorPos(&point) )
				return;
			HWND hWndTrack = pFrame-> /*pExtBar->GetOwner()->*/ GetSafeHwnd();
			ASSERT( hWndTrack != NULL && ::IsWindow(hWndTrack) );
			CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
			VERIFY( pPopup->CreatePopupMenu(hWndTrack) );
			if(	! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
					pFrame,
					pPopup,
					CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_MINIFRAME_NC_CTX,
					pExtBar,
					WM_NCRBUTTONDOWN,
					NULL
					)
				)
			{
				delete pPopup;
				return;
			}
			if( pPopup->ItemGetCount() == 0 )
			{
				delete pPopup;
				return;
			}
			//::SetFocus( hWndTrack );
			if( ! pPopup->TrackPopupMenu(
					TPMX_OWNERDRAW_FIXED,
					point.x,
					point.y
					)
				)
			{
				delete pPopup;
			}
			return;
		} // if( pExtBar != NULL )
	} // if( nHitTest == HTCAPTION )
	CMiniDockFrameWnd::OnNcRButtonDown(nHitTest,point);
}

void CExtMiniDockFrameWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
//	CMiniDockFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	bCalcValidRects;
int nWinResizingFrameDX = ::GetSystemMetrics( SM_CXSIZEFRAME );
int nWinResizingFrameDY = ::GetSystemMetrics( SM_CYSIZEFRAME );
int nSmallCaptionDY = ::GetSystemMetrics( SM_CYSMCAPTION );
	::InflateRect(
		&lpncsp->rgrc[0],
		- nWinResizingFrameDX,
		- nWinResizingFrameDY
		);
	lpncsp->rgrc[0].top += nSmallCaptionDY;
}

void CExtMiniDockFrameWnd::CalcWindowRect(
	LPRECT lpClientRect,
	UINT nAdjustType // = CWnd::AdjustType::adjustBorder
	)
{
	ASSERT_VALID( this );
	ASSERT( lpClientRect != NULL );
	nAdjustType;
int nWinResizingFrameDX = ::GetSystemMetrics( SM_CXSIZEFRAME );
int nWinResizingFrameDY = ::GetSystemMetrics( SM_CYSIZEFRAME );
int nSmallCaptionDY = ::GetSystemMetrics( SM_CYSMCAPTION );
	::InflateRect(
		lpClientRect,
		nWinResizingFrameDX,
		nWinResizingFrameDY
		);
	lpClientRect->top -= nSmallCaptionDY;
}

void CExtMiniDockFrameWnd::OnNcPaint() 
{

CExtControlBar * pExtBar =
		GetControlBarExt();
bool bExtBar = false;
	if( pExtBar != NULL
		&& !pExtBar->IsFixedMode()
		)
		bExtBar = true;

CRect rcClient, rcBar;
	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);
	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());

CWindowDC dcSrc( this );
	dcSrc.ExcludeClipRect( &rcClient );
CExtMemoryDC dcWin( &dcSrc, &rcBar );

CExtPaintManager::PAINTDOCKINGFRAMEDATA _pdfd(
		this,
		rcBar,
		rcClient,
		true,
		bExtBar
		);
	g_PaintManager->PaintDockingFrame( dcWin, _pdfd );
bool bEnableCaptionText = true;
CExtSafeString sCaption;
	if( pExtBar != NULL )
	{
		if( pExtBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			&& ( !pExtBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar)) )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			)
			bEnableCaptionText = false;
		else
		{
			int nTextLen = pExtBar->GetWindowTextLength();
			if( nTextLen > 0 )
			{
				pExtBar->GetWindowText( sCaption.GetBuffer( nTextLen+1 ), nTextLen+1 );
				sCaption.ReleaseBuffer();
			} // if( nTextLen > 0 )
		}
	}
	if( !bEnableCaptionText )
	{
		if( pExtBar != NULL
			&& pExtBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
			)
			bEnableCaptionText =
				_GetSingleVisibleCaptionText(
					sCaption
					);
	}
	else
		if( bEnableCaptionText && sCaption.IsEmpty() )
		{
			int nTextLen = GetWindowTextLength();
			if( nTextLen > 0 )
			{
				GetWindowText( sCaption.GetBuffer( nTextLen+1 ), nTextLen+1 );
				sCaption.ReleaseBuffer();
			} // if( nTextLen > 0 )
		}

CRect rcCapt( m_rcFrameCaption );
CRect rcText( m_rcFrameCaption );
	rcText.right = m_rcBtnHideBar.left;

	if( pExtBar != NULL )
		pExtBar->NcButtons_CalcGripperTextLocation( rcText );

bool bActive = false;
HWND hTmpWndFocus = ::GetFocus();
	if( hTmpWndFocus != NULL
		&& (  GetSafeHwnd() == hTmpWndFocus
			  || ::IsChild( GetSafeHwnd(), hTmpWndFocus )
			)
		)
		bActive = true;

	rcText.DeflateRect(2,0);
CExtPaintManager::PAINTGRIPPERDATA _pgd(
		this,
		rcCapt,
		rcText,
		bActive,
		true,
		false,
		bExtBar,
		sCaption.IsEmpty() ? LPCTSTR( NULL ) : sCaption
		);
	g_PaintManager->PaintGripper( dcWin, _pgd );

	if( pExtBar != NULL )
	{
		pExtBar->NcButtons_Paint( dcWin );
	} // if( pExtBar != NULL )
	else
	{
		CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA _pdcbd(
			this,
			m_rcBtnHideBar,
			CExtPaintManager::__DCBT_CLOSE,
			m_bBtnHideBar,
			false,
			true,
			bExtBar,
			bActive,
			true
			);
		g_PaintManager->PaintDockingCaptionButton( dcWin, _pdcbd );
	} // else from if( pExtBar != NULL )

	g_PaintManager->OnPaintSessionComplete( this );
}

bool CExtMiniDockFrameWnd::_GetSingleVisibleCaptionText( CExtSafeString & sCaption )
{
	sCaption = _T("");
CFrameWnd * pDockSite = GetParentFrame();
	if( pDockSite == NULL )
		return false;
	ASSERT( !pDockSite->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) );
POSITION pos = pDockSite->m_listControlBars.GetHeadPosition();
	for( ; pos != NULL; )
	{
		CExtControlBar * pBar =
			DYNAMIC_DOWNCAST(
				CExtControlBar,
				((CControlBar *)(pDockSite->m_listControlBars.GetNext(pos)))
				);
		if(		pBar == NULL
			||	pBar->m_pDockSite == NULL
			||	pBar->IsFixedMode()
			||	(	pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
					&& !pBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar))
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
				)
			||	pBar->GetParentFrame() != this
			||	pBar->IsFloating()
			||	!pBar->IsVisible()
			)
			continue;
		ASSERT( pBar->m_pDockBar != NULL );
		if(
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar))
			||
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			!pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar))
			)
			continue;
		if( !pBar->_IsSingleVisibleInFloatingPalette() )
			return false;
		int nTextLen = pBar->GetWindowTextLength();
		if( nTextLen > 0 )
		{
			pBar->GetWindowText( sCaption.GetBuffer( nTextLen+1 ), nTextLen+1 );
			sCaption.ReleaseBuffer();
		} // if( nTextLen > 0 )
		return sCaption.IsEmpty() ? false : true;
	}
	return false;
}

BOOL CExtMiniDockFrameWnd::OnNcActivate(BOOL bActive) 
{
BOOL rc = CMiniDockFrameWnd::OnNcActivate(bActive);
	SendMessage( WM_NCPAINT );
	if( bActive )
	{
		CExtControlBar * pExtBar =
			GetControlBarExt();
		if( pExtBar != NULL )
			pExtBar->RedrawWindow(
				NULL,
				NULL,
				RDW_INVALIDATE|RDW_UPDATENOW
					|RDW_ERASE|RDW_ALLCHILDREN
				);
	} // if( bActive )
	return rc;
}

void CExtMiniDockFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
//	if( _ResizingIsInProgress() )
//		return;

	ASSERT( cx >= 0 && cy >= 0 );
CExtPaintManager::monitor_parms_t _mp;
	CExtPaintManager::stat_GetMonitorParms( _mp, this );
	if(		cx > _mp.m_rcWorkArea.Width()
		||	cy > _mp.m_rcWorkArea.Height()
		)
	{
		CRect rcWnd;
		GetWindowRect( &rcWnd );
		rcWnd.right =
			rcWnd.left
			+ ( min( cx, _mp.m_rcWorkArea.Width() ) )
			;
		rcWnd.bottom =
			rcWnd.top
			+ ( min( cy, _mp.m_rcWorkArea.Height() ) )
			;
		MoveWindow( &rcWnd );
		return;
	}

	CMiniDockFrameWnd::OnSize(nType, cx, cy);


int nWinResizingFrameDX = ::GetSystemMetrics( SM_CXSIZEFRAME );
int nWinResizingFrameDY = ::GetSystemMetrics( SM_CYSIZEFRAME );
int nSmallCaptionDY = ::GetSystemMetrics( SM_CYSMCAPTION );
CRect rcFrameCaption;
	GetWindowRect( &rcFrameCaption );
	rcFrameCaption -= rcFrameCaption.TopLeft();
	rcFrameCaption.left  += nWinResizingFrameDX;
	rcFrameCaption.right -= nWinResizingFrameDX;
	rcFrameCaption.top   += nWinResizingFrameDY;
	rcFrameCaption.bottom =
		rcFrameCaption.top
		+ nSmallCaptionDY
		- ::GetSystemMetrics( SM_CYSMSIZE );
	rcFrameCaption.bottom =
		rcFrameCaption.top
		+ ::GetSystemMetrics( SM_CYSMSIZE );
	m_rcFrameCaption = rcFrameCaption;

int nDummyMetric = ::GetSystemMetrics( SM_CXSMSIZE ) - 1;
int nRight =
		m_rcFrameCaption.right - nDummyMetric - 1;
	m_rcBtnHideBar.SetRect(
		nRight,
		m_rcFrameCaption.top + 2,
		nRight + nDummyMetric - 2,
		m_rcFrameCaption.bottom - 2
		);

CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL )
	{
		pExtBar->OnNcAreaButtonsReinitialize();
		pExtBar->OnNcAreaButtonsReposition();
	} // if( pExtBar != NULL )

	if( !_ResizingIsInProgress() )
		RedrawWindow(
			NULL,
			NULL,
			RDW_FRAME|RDW_INVALIDATE|RDW_ERASE
				|RDW_UPDATENOW|RDW_ALLCHILDREN
			);
	CExtPaintManager::stat_PassPaintMessages();
}

void CExtMiniDockFrameWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CMiniDockFrameWnd::OnWindowPosChanged(lpwndpos);
	if(		_ResizingIsInProgress()
		||	g_bOnWndPosChgProcessing
		||	(lpwndpos->flags & SWP_NOMOVE) != 0
		)
		return;
CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar == NULL || !pExtBar->IsFloating() )
		return;
CRect rcWnd;
	GetWindowRect( &rcWnd );

int nSmallCaptionDY = ::GetSystemMetrics( SM_CYSMCAPTION );
CExtPaintManager::monitor_parms_t _mp;
	CExtPaintManager::stat_GetMonitorParms( _mp, this );
	ASSERT( !g_bOnWndPosChgProcessing );
	if( rcWnd.top < _mp.m_rcWorkArea.top )
	{
		rcWnd.top = _mp.m_rcWorkArea.top;
		g_bOnWndPosChgProcessing = true;
	}
	else if( rcWnd.top + nSmallCaptionDY > _mp.m_rcWorkArea.bottom )
	{
		rcWnd.top =
			max(
			_mp.m_rcWorkArea.top, 
			_mp.m_rcWorkArea.bottom
				- nSmallCaptionDY
			);
		g_bOnWndPosChgProcessing = true;
	}
	if( !g_bOnWndPosChgProcessing )
		return;
	SetWindowPos(
		NULL,
		rcWnd.left, rcWnd.top, -1, -1,
		SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE
		);

	pExtBar->OnNcAreaButtonsReposition();
	PostMessage( WM_NCPAINT );
	CExtPaintManager::stat_PassPaintMessages();
	g_bOnWndPosChgProcessing = false;
}

void CExtMiniDockFrameWnd::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
	CMiniDockFrameWnd::OnWindowPosChanging( lpwndpos );
	if( GetStyle() & __REMOVED_STYLES_NORMAL )
		ModifyStyle( __REMOVED_STYLES_NORMAL, 0, __SWP_FOR_REMOVED_STYLES );
	if( GetExStyle() & __REMOVED_STYLES_EXTENDED )
		ModifyStyleEx( __REMOVED_STYLES_EXTENDED, 0, __SWP_FOR_REMOVED_STYLES );
}

void CExtMiniDockFrameWnd::_AnalyzeButtonsHover(CPoint point)
{
CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL )
	{
		pExtBar->NcButtons_HitTest( point );
		return;
	}
CRect rcClient, rcBar;
	GetClientRect( &rcClient );
	ClientToScreen( &rcClient );
	GetWindowRect( &rcBar );
	rcClient.OffsetRect( -rcBar.TopLeft() );
	rcBar.OffsetRect (-rcBar.TopLeft() );
	ScreenToClient( &point );
	point.y -= rcBar.top - rcClient.top;
bool bBtnHideBarHover = false;
	if( m_rcBtnHideBar.PtInRect(point) )
		bBtnHideBarHover = true;
	if(	m_bBtnHideBar != bBtnHideBarHover )
	{
		m_bBtnHideBar = bBtnHideBarHover;
		SendMessage( WM_NCPAINT );
	}
	if( m_bBtnHideBar || bBtnHideBarHover )
		SetTimer( 2, 20, NULL );
	else
		KillTimer( 2 );
}

void CExtMiniDockFrameWnd::OnNcMouseMove( UINT nHitTest, CPoint point )
{
	if( g_bAutoMakeTopmostOnMouseEvent
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		&& (CExtControlBar::_DraggingGetBar() == NULL)
		)
		BringWindowToTop();

	if( _ResizingIsInProgress() )
	{
		return;
	}
	CMiniDockFrameWnd::OnNcMouseMove( nHitTest, point );
	_AnalyzeButtonsHover(point);
}

void CExtMiniDockFrameWnd::OnTimer( UINT nIDEvent )
{
CExtControlBar * pExtBar = GetControlBarExt();
	if( nIDEvent == 1 )
	{
		if( pExtBar == NULL )
			return;
		KillTimer( 1 );
		pExtBar->OnNcAreaButtonsReposition();
//		SetWindowPos(
//			NULL,
//			0, 0, 0, 0,
//			SWP_NOREDRAW|SWP_NOSIZE
//				|SWP_NOMOVE|SWP_NOACTIVATE
//				|SWP_NOZORDER|SWP_NOOWNERZORDER 
//				|SWP_FRAMECHANGED
//			);
		return;
	}
	if( nIDEvent == 2 )
	{
		_AnalyzeButtonsHover();
		return;
	}
	if( nIDEvent == 3 )
	{
		if(		pExtBar == NULL
			//|| _ResizingIsInProgress()
			)
			return;
		KillTimer( 3 );
		RecalcLayout();
		pExtBar->OnNcAreaButtonsReposition();
		SendMessage( WM_NCPAINT );
		return;
	}
	if( nIDEvent == 4 )
	{
		if( pExtBar != NULL )
		{
			ASSERT_VALID( pExtBar );
			CPoint point;
			if( ! ::GetCursorPos(&point) )
				return;
			UINT nHT = pExtBar->NcButtons_HitTest( point );
			if( nHT == HTCLIENT )
			{
				PostMessage( WM_NCPAINT );
				pExtBar->m_bNcAreaBtnTracking = false;
			}
		}
		if( !pExtBar->m_bNcAreaBtnTracking )
			KillTimer( 4 );
		return;
	}

	if( pExtBar == NULL )
		return;
	ASSERT_VALID( pExtBar );
	if( nIDEvent == (UINT)pExtBar->GetDlgCtrlID() )
	{
		if( _ResizingIsInProgress() )
			return;
		CPoint pt;
		if( ! ::GetCursorPos( &pt ) )
			return;
		CWnd * pTempWnd = WindowFromPoint( pt );
		int IsMouseInsideTheWindow =
			IsChild( pTempWnd ) || pTempWnd->m_hWnd == m_hWnd;

		if(	IsMouseInsideTheWindow
			|| IsChild(GetFocus())  
			|| (CExtMouseCaptureSink::GetCapture() && pExtBar->m_hWnd == CExtMouseCaptureSink::GetCapture()) 
			|| CExtPopupMenuWnd::IsMenuTracking()
			) 
		{

			if(	IsMouseInsideTheWindow 
				&& !CExtPopupMenuWnd::IsMenuTracking()
				&& IsWindowEnabled()
				&& GetFocus()
				) 
				BringWindowToTop();
		}
	} // if( nIDEvent == (UINT)pExtBar->GetDlgCtrlID() )
}

void CExtMiniDockFrameWnd::OnClose() 
{
	CMiniDockFrameWnd::OnClose();
}

void CExtMiniDockFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CMiniDockFrameWnd::OnGetMinMaxInfo( lpMMI );
CExtPaintManager::monitor_parms_t _mp;
	CExtPaintManager::stat_GetMonitorParms( _mp, this );
	if( lpMMI->ptMaxTrackSize.x > _mp.m_rcWorkArea.Width() )
		lpMMI->ptMaxTrackSize.x = _mp.m_rcWorkArea.Width();
	if( lpMMI->ptMaxTrackSize.y > _mp.m_rcWorkArea.Height() ) 
		lpMMI->ptMaxTrackSize.y = _mp.m_rcWorkArea.Height();
}

void CExtMiniDockFrameWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( g_bAutoMakeTopmostOnMouseEvent
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		)
		BringWindowToTop();
	if( _ResizingIsInProgress() )
	{
		ClientToScreen( &point );
		_ResizingUpdateState( point );
		return;
	}
	CMiniDockFrameWnd::OnMouseMove(nFlags, point);
}

void CExtMiniDockFrameWnd::OnCaptureChanged(CWnd *pWnd) 
{
	if( _ResizingIsInProgress()
		&& pWnd != this
		)
	{
		_ResizingEnd();
		return;
	}
	CMiniDockFrameWnd::OnCaptureChanged(pWnd);
}

void CExtMiniDockFrameWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}
	CMiniDockFrameWnd::OnLButtonUp(nFlags, point);
}

void CExtMiniDockFrameWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}
	CMiniDockFrameWnd::OnRButtonDown(nFlags, point);
}

BOOL CExtMiniDockFrameWnd::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

UINT CExtMiniDockFrameWnd::OnNcHitTest(CPoint point) 
{
	if( GetStyle() & MFS_MOVEFRAME )
		ModifyStyle( MFS_MOVEFRAME, 0 );
//UINT nHit = CMiniDockFrameWnd::OnNcHitTest(point);
CRect rcBar;
	GetWindowRect( &rcBar );
	if( !rcBar.PtInRect( point ) )
		return HTNOWHERE;
UINT nHit = HTCLIENT;
int nWinResizingFrameDX = ::GetSystemMetrics( SM_CXSIZEFRAME );
int nWinResizingFrameDY = ::GetSystemMetrics( SM_CYSIZEFRAME );
int nSmallCaptionDY = ::GetSystemMetrics( SM_CYSMCAPTION );
CRect rcTest(
		rcBar.left,
		rcBar.top,
		rcBar.right,
		rcBar.top + nWinResizingFrameDY
		);
	if( rcTest.PtInRect( point ) )
		nHit = HTTOP;
	else
	{
		rcTest.SetRect(
			rcBar.left,
			rcBar.bottom - nWinResizingFrameDY,
			rcBar.right,
			rcBar.bottom
			);
		if( rcTest.PtInRect( point ) )
			nHit = HTBOTTOM;
		else
		{
			rcTest.SetRect(
				rcBar.left,
				rcBar.top,
				rcBar.left + nWinResizingFrameDX,
				rcBar.bottom
				);
			if( rcTest.PtInRect( point ) )
				nHit = HTLEFT;
			else
			{
				rcTest.SetRect(
					rcBar.right - nWinResizingFrameDX,
					rcBar.top,
					rcBar.right,
					rcBar.bottom
					);
				if( rcTest.PtInRect( point ) )
					nHit = HTRIGHT;
				else
				{
					rcTest.SetRect(
						rcBar.left,
						rcBar.top + nWinResizingFrameDY,
						rcBar.right,
						rcBar.top + nWinResizingFrameDY + nSmallCaptionDY
						);
					if( rcTest.PtInRect( point ) )
						nHit = HTCAPTION;
				}
			}
		}
	}
	switch( nHit )
	{
	case HTTOP:
	{
		rcTest.SetRect(
			rcBar.left,
			rcBar.top,
			rcBar.left + nSmallCaptionDY,
			rcBar.top + nWinResizingFrameDY
			);
		if( rcTest.PtInRect(point) )
			nHit = HTTOPLEFT;
		else
		{
			rcTest.SetRect(
				rcBar.right - nWinResizingFrameDY,
				rcBar.top,
				rcBar.right,
				rcBar.top + nWinResizingFrameDY
				);
			if( rcTest.PtInRect(point) )
				nHit = HTTOPRIGHT;
		}
	}
	break;
	case HTBOTTOM:
	{
		rcTest.SetRect(
			rcBar.left,
			rcBar.bottom - nWinResizingFrameDY,
			rcBar.left + nSmallCaptionDY,
			rcBar.bottom
			);
		if( rcTest.PtInRect(point) )
			nHit = HTBOTTOMLEFT;
		else
		{
			rcTest.SetRect(
				rcBar.right - nWinResizingFrameDY,
				rcBar.bottom - nWinResizingFrameDY,
				rcBar.right,
				rcBar.bottom
				);
			if( rcTest.PtInRect(point) )
				nHit = HTBOTTOMRIGHT;
		}
	}
	break;
	case HTLEFT:
	{
		rcTest.SetRect(
			rcBar.left,
			rcBar.top,
			rcBar.left + nWinResizingFrameDX,
			rcBar.top + nSmallCaptionDY
			);
		if( rcTest.PtInRect(point) )
			nHit = HTTOPLEFT;
		else
		{
			rcTest.SetRect(
				rcBar.left,
				rcBar.bottom - nSmallCaptionDY,
				rcBar.left + nWinResizingFrameDX,
				rcBar.bottom
				);
			if( rcTest.PtInRect(point) )
				nHit = HTBOTTOMLEFT;
		}
	}
	break;
	case HTRIGHT:
	{
		rcTest.SetRect(
			rcBar.right - nWinResizingFrameDX,
			rcBar.top,
			rcBar.right,
			rcBar.top + nSmallCaptionDY
			);
		if( rcTest.PtInRect(point) )
			nHit = HTTOPRIGHT;
		else
		{
			rcTest.SetRect(
				rcBar.right - nWinResizingFrameDX,
				rcBar.bottom - nSmallCaptionDY,
				rcBar.right,
				rcBar.bottom
				);
			if( rcTest.PtInRect(point) )
				nHit = HTBOTTOMRIGHT;
		}
	}
	break;
	} // switch( nHit )

bool bNcRepaint = false;
CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL )
	{
		ASSERT_VALID( pExtBar );
		UINT nHT = pExtBar->NcButtons_HitTest( point );
		if( nHT != HTCLIENT )
		{
			pExtBar->m_bNcAreaBtnTracking = true;
			if( pExtBar->_IsDockSiteCustomizeMode() )
				SetTimer( 4, 20, NULL );
			return nHT;
		}
		else
		{
			bNcRepaint = true;
			pExtBar->m_bNcAreaBtnTracking = false;
		}
		if( pExtBar->IsFixedMode() )
		{
			bool bPaletteMode = false;
			CExtToolControlBar * pToolBar =
				DYNAMIC_DOWNCAST(
					CExtToolControlBar,
					pExtBar
					);
			if( pToolBar != NULL )
			{
				ASSERT_VALID( pToolBar );
				bPaletteMode = pToolBar->m_bPaletteMode;
			} // if( pToolBar != NULL )
			switch( nHit )
			{
			case HTTOP:
			case HTBOTTOM:
			case HTLEFT:
			case HTRIGHT:
				if( bPaletteMode )
					nHit = HTCAPTION;
			break;
			case HTTOPLEFT:
			case HTTOPRIGHT:
				nHit = bPaletteMode ? HTCAPTION : HTTOP;
			break;
			case HTBOTTOMLEFT:
			case HTBOTTOMRIGHT:
				nHit = bPaletteMode ? HTCAPTION : HTBOTTOM;
			break;
			} // switch( nHit )
		} // if( pExtBar->IsFixedMode() )
	} // if( pExtBar != NULL )
	if( bNcRepaint )
		SendMessage( WM_NCPAINT );
	return nHit;
}

void CExtMiniDockFrameWnd::OnCancelMode() 
{
	CMiniDockFrameWnd::OnCancelMode();
	CExtPopupMenuWnd::CancelMenuTracking();
	CWnd::CancelToolTips();
//	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );
	if( _ResizingIsInProgress() )
		_ResizingEnd();
}

void CExtMiniDockFrameWnd::OnPaint() 
{
CPaintDC dcPaint( this );
	return;
//CRect rcClient;
//	GetClientRect( &rcClient );
//	if( rcClient.IsRectEmpty()
//		|| !dcPaint.RectVisible( &rcClient )
//		)
//		return;
//	CExtPaintManager::stat_ExcludeChildAreas(
//		dcPaint.GetSafeHdc(),
//		GetSafeHwnd()
//		);
//COLORREF clrBack =
//		g_PaintManager->GetColor(
//			CExtPaintManager::CLR_3DFACE_OUT
//			);
//	dcPaint.FillSolidRect( &rcClient, clrBack );
}

int CExtMiniDockFrameWnd::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	) const
{
	ASSERT_VALID( this );
	
	if( pTI == NULL || m_wndToolTip.GetSafeHwnd() == NULL )
		return -1;

const CControlBar * pBar = GetControlBar();
	if( pBar == NULL )
		return -1;
	ASSERT_VALID( pBar );
	if( (pBar->m_dwStyle & CBRS_TOOLTIPS) == NULL )
		return -1;

int nRet = pBar->OnToolHitTest( point, pTI );
	if( nRet < 0 )
		return -1;

	if( pTI->lpszText != NULL )
	{
		CExtMiniDockFrameWnd * pNCT = (CExtMiniDockFrameWnd *)this;
		VERIFY(
			pNCT->m_wndToolTip.AddTool(
				pNCT,
				pTI->lpszText,
				&pTI->rect,
				1
				)
			);
	}
	
	return -1;
}

BOOL CExtMiniDockFrameWnd::PreTranslateMessage( MSG* pMsg ) 
{
	if( m_wndToolTip.GetSafeHwnd() != NULL )
		m_wndToolTip.RelayEvent( pMsg );
	return CMiniDockFrameWnd::PreTranslateMessage( pMsg );
}

