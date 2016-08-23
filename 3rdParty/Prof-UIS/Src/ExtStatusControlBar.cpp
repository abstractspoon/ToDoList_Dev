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

// ExtStatusControlBar.cpp : implementation file
//

#include "stdafx.h"

#if (!defined __EXT_STATUSCONTROLBAR_H)
	#include <ExtStatusControlBar.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#include <../src/mfc/AfxImpl.h>
#endif

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	#if (!defined __EXTCUSTOMIZE_H)
		#include <ExtCustomize.h>
	#endif
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#if (!defined __EXT_MEMORY_DC_H)
	#include <../src/ExtMemoryDC.h>
#include "..\include\extstatuscontrolbar.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtStatusControlBar

IMPLEMENT_DYNCREATE( CExtStatusControlBar, CStatusBar )

CExtStatusControlBar::CExtStatusControlBar()
	: m_bOuterRectInFirstBand( false )
	, m_bCompleteRepaint( true )
	, m_bDrawPaneSeparatorsInsteadOfBorders( false ) // default status bar control behavior
	, m_bHideTextOnDisabledPanes( true ) // default status bar control behavior
	, m_bDoNotPaintIcons( false )
	, m_nDrawPaneTextFlags( DT_SINGLELINE|DT_LEFT|DT_VCENTER ) // default status bar control behavior
{
	m_clrStatusBk = (COLORREF)(-1);
}

CExtStatusControlBar::~CExtStatusControlBar()
{
	for ( int i = 0; i < m_arrPaneControls.GetSize(); i++ ){
		if( m_arrPaneControls[i]->hWnd && ::IsWindow(m_arrPaneControls[i]->hWnd) ) {
			::SendMessage(m_arrPaneControls[i]->hWnd, WM_CLOSE, 0, 0); 
			if( m_arrPaneControls[i]->bAutoDestroy ) {
				::DestroyWindow(m_arrPaneControls[i]->hWnd);
			}
		}
		_STATUSBAR_PANE_CTRL_ *pPaneCtrl = m_arrPaneControls[i];
		if( pPaneCtrl )
			delete pPaneCtrl;
	}
}

BEGIN_MESSAGE_MAP(CExtStatusControlBar, CStatusBar)
	//{{AFX_MSG_MAP(CExtStatusControlBar)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOLORCHANGE()
	__EXT_MFC_SAFE_ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(__ExtMfc_WM_THEMECHANGED, OnThemeChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExtStatusControlBar message handlers

void CExtStatusControlBar::OnPaint() 
{
	// XP painting & last pane size fixed by Dmitry Yakovlev
	_SyncStatusBarColors();
CRect rcClient;
	GetClientRect( &rcClient );
CPaintDC dcPaint( this );
	CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint,
		GetSafeHwnd(),
		CExtPaintManager::stat_DefExcludeChildAreaCallback
		);
CExtMemoryDC dc( &dcPaint, &rcClient );
	CStatusBar::UpdateAllPanes( FALSE, TRUE );
	// erase client area with the current color/gradient
CRect rcFill( rcClient );
	rcFill.left = rcFill.top = 0;
	rcFill.right  -= rcClient.left;
	rcFill.bottom -= rcClient.top;
	OnPaintEntireBackground( dc, rcFill );
	// pre-compute gripper rect, exclude it's area
CRect rcGrip( 0, 0, 0, 0 );
CWnd * pWndTLP = GetTopLevelParent();
	if( pWndTLP != NULL )
	{
		WINDOWPLACEMENT _wp;
		::memset( (void*)&_wp, 0, sizeof(WINDOWPLACEMENT) );
		_wp.length = sizeof(WINDOWPLACEMENT);
		pWndTLP->GetWindowPlacement( &_wp );
		if( _wp.showCmd != SW_SHOWMAXIMIZED )
		{
			GetClientRect( &rcGrip );
			rcGrip.left = rcGrip.right - ::GetSystemMetrics( SM_CXVSCROLL );
			dc.ExcludeClipRect( &rcGrip );
		} // if( _wp.showCmd != SW_SHOWMAXIMIZED )
	} // if( pWndTLP != NULL )
CFont * pOldFont = NULL;
int nOldBkMode = 0;
COLORREF nOldTextColor = 0;
	if( m_bCompleteRepaint )
	{
		pOldFont = dc.SelectObject( &g_PaintManager->m_FontNormal );
		nOldBkMode = dc.SetBkMode( TRANSPARENT );
		nOldTextColor = dc.SetTextColor( m_clrPaneTextNormal );
	} // if( m_bCompleteRepaint )
	// re-paint borders/text/icons
	CRect rcPanePrev( 0, 0, 0, 0 );
	for( INT nPaneIdx = 0; nPaneIdx < m_nCount; nPaneIdx++ )
	{
		CRect rcPane;
		GetItemRect( nPaneIdx, &rcPane );
		// CStatusBar::GetItemRect() sometimes returns invalid size 
		// of the last pane - we will re-compute it
        DWORD dwPaneStyle = GetPaneStyle( nPaneIdx );
		if( nPaneIdx == (m_nCount-1) )
		{
			int cx = ::GetSystemMetrics( SM_CXEDGE );
			if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
			{
				UINT nID, nStyle;
				int  cxWidth;
				GetPaneInfo( nPaneIdx, nID, nStyle, cxWidth );
				rcPane.right = rcPane.left + cxWidth + cx*3;
			} // if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
			else
			{
				CRect rcClient;
				GetClientRect( &rcClient );
				rcPane.right = rcClient.right;
				if( (GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP )
				{
					int cxSmIcon = ::GetSystemMetrics( SM_CXSMICON );
					rcPane.right -= cxSmIcon + cx;
				} // if( (GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP )
			} // else from if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
		} // if( nPaneIdx == (m_nCount-1) )
		HICON hIcon = NULL;
		if( ! m_bDoNotPaintIcons )
			hIcon = (HICON)
				CStatusBar::GetStatusBarCtrl().
					SendMessage( SB_GETICON, nPaneIdx );
		CString sPaneText = GetPaneText( nPaneIdx );
		OnErasePaneBackground(
			dc,
			nPaneIdx,
			rcPane
			);
		OnPaintPane(
			dc,
			nPaneIdx,
			rcPane,
			dwPaneStyle,
			hIcon,
			sPaneText.IsEmpty()
				? __EXT_MFC_SAFE_LPCTSTR( NULL )
				: __EXT_MFC_SAFE_LPCTSTR( LPCTSTR(sPaneText) )
				,
			m_nDrawPaneTextFlags
			);
		if( m_bCompleteRepaint && m_bDrawPaneSeparatorsInsteadOfBorders && nPaneIdx > 0 )
			OnPaintSeparator(
				dc,
				nPaneIdx,
				rcPanePrev,
				rcPane
				);
		rcPanePrev = rcPane;
	} // for( INT nPaneIdx = m_bOuterRectInFirstBand ? 0 : 1; nPaneIdx < m_nCount; nPaneIdx++ )
	if( m_bCompleteRepaint )
	{
		dc.SetTextColor( nOldTextColor );
		dc.SetBkMode( nOldBkMode );
		dc.SelectObject( pOldFont );
	} // if( m_bCompleteRepaint )

	// paint resizing gripper after all pane borders - this will allow gripper
	// to be on the top of all panes when window size is near its minimal size 
	if( !rcGrip.IsRectEmpty() )
	{
		dc.SelectClipRgn( NULL );
		if( ! m_bCompleteRepaint )
			dc.FillSolidRect( &rcGrip, m_clrStatusBk );
		rcGrip.OffsetRect( -2, -2 );
		g_PaintManager->PaintResizingGripper(
			dc,
			rcGrip,
			this
			);
	} // if( !rcGrip.IsRectEmpty() )
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnPaintSeparator(
	CDC & dc,
	int nPaneIdxAfterSep,
	const CRect & rcPaneBefore,
	const CRect & rcPaneAfter
	)
{
CRect rcSeparator(
		rcPaneBefore.right,
		rcPaneAfter.top,
		rcPaneAfter.left,
		rcPaneAfter.bottom
		);
	rcSeparator.DeflateRect( 0, 1 );
	g_PaintManager->PaintSeparator(
		dc,
		rcSeparator,
		true,
		true,
		this,
		nPaneIdxAfterSep
		);
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnPaintEntireBackground(
	CDC & dc,
	const CRect & rcBk
	)
{
	if(		m_bCompleteRepaint
		&&	g_PaintManager->GetCb2DbTransparentMode(this)
		)
		g_PaintManager->PaintDockerBkgnd( dc, this );
	else
		dc.FillSolidRect( &rcBk, m_clrStatusBk );
	// invoke default handler
	if( ! m_bCompleteRepaint )
		CStatusBar::DefWindowProc( WM_PAINT, (WPARAM)dc.GetSafeHdc(), 0 );
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnErasePaneBackground(
	CDC & dc,
	int nPaneIdx,
	const CRect & rcPane
	)
{
	dc;
	nPaneIdx;
	rcPane;
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnPaintPane(
	CDC & dc,
	int nPaneIdx,
	const CRect & rcPane,
	DWORD dwPaneStyle,
	HICON hIcon,
	__EXT_MFC_SAFE_LPCTSTR sPaneText, // by default it's text from the status bar control
	UINT nDrawTextFlags // by default m_nDrawPaneTextFlags is used
	)
{
	if( m_bCompleteRepaint )
	{
		CRect rcText( rcPane );
		if( hIcon != NULL )
		{
			CExtCmdIcon _icon;
			_icon.AssignFromHICON( hIcon, false );
			CSize sizeIcon = _icon.GetSize();
			ASSERT( sizeIcon.cx > 0 && sizeIcon.cy > 0 );
			rcText.left += sizeIcon.cx + 1;
			_icon.Paint(
				dc,
				rcPane.left + 1,
				rcPane.top + ( rcPane.Height() - sizeIcon.cy ) / 2
				);
			_icon.DetachAsHICON();
		} // if( hIcon != NULL )
		bool bDisabled = ((dwPaneStyle & SBPS_DISABLED) != 0) ? true : false;
		int nPaneTextLen =
			(sPaneText == NULL)
				? 0
				: _tcslen( sPaneText )
				;
		if(		(!bDisabled)
			||	(bDisabled && (!m_bHideTextOnDisabledPanes))
			&&	nPaneTextLen > 0
			)
		{ // if text painting is allowed
			rcText.DeflateRect( 2, 0 );
			if( rcText.right > rcText.left )
			{
				COLORREF clrTextRestore = COLORREF(-1L);
				COLORREF clrTextCustom = OnQueryPaneTextColor( nPaneIdx );
				if( clrTextCustom != COLORREF(-1L) )
					clrTextRestore = dc.SetTextColor( clrTextCustom );
				else if( bDisabled )
					clrTextRestore = dc.SetTextColor( m_clrPaneTextDisabled );
				dc.DrawText(
					LPCTSTR(sPaneText),
					nPaneTextLen,
					&rcText,
					nDrawTextFlags
					);
				if( clrTextRestore != COLORREF(-1L) )
					dc.SetTextColor( clrTextRestore );
			} // if( rcText.right > rcText.left )
		} // if text painting is allowed
	} // if( m_bCompleteRepaint )
	if(		(!m_bCompleteRepaint)
		||	( m_bCompleteRepaint && (!m_bDrawPaneSeparatorsInsteadOfBorders) )
		)
	{ // if paint pane borders
		if( m_bOuterRectInFirstBand || nPaneIdx > 0 )
		{
//			if( m_bCompleteRepaint )
//			{
//				dc.Draw3dRect( &rcPane, m_clrPaneRect, m_clrPaneRect );
//			} // if( m_bCompleteRepaint )
//			else
			{
				CExtPaintManager::PAINTCONTROLBARBORDERSDATA _pcbbd(
					this,
					CExtPaintManager::__CB_INNER_STATUSBAR_ITEM,
					0,
					rcPane
					);
				g_PaintManager->PaintControlBarBorders( dc, _pcbbd );
			} // else from if( m_bCompleteRepaint )
		} // if( m_bOuterRectInFirstBand || nPaneIdx > 0 )
	} // if paint pane borders
	if( (!m_bCompleteRepaint) && g_PaintManager.m_bUxApiInitPassed )
	{
		CRect rcPaneInner( rcPane );
		rcPaneInner.DeflateRect( 1, 1 );
		dc.Draw3dRect( &rcPaneInner, m_clrStatusBk, m_clrStatusBk );
	} // if( (!m_bCompleteRepaint) && g_PaintManager.m_bUxApiInitPassed )
}

//////////////////////////////////////////////////////////////////////////

COLORREF CExtStatusControlBar::OnQueryPaneTextColor(
	int nPaneIdx
	)
{
	nPaneIdx;
	return COLORREF(-1L); // <- detect color automatically
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnNcPaint() 
{
	_SyncStatusBarColors();
CRect rcClient, rcWnd;
	GetClientRect( rcClient );
	ClientToScreen( rcClient );
	GetWindowRect( rcWnd );
	rcClient.OffsetRect( -rcWnd.TopLeft() );
	rcWnd.OffsetRect( -rcWnd.TopLeft() );
CWindowDC dc( this );
	dc.ExcludeClipRect( &rcClient );
CExtPaintManager::PAINTCONTROLBARBORDERSDATA _pcbbd(
		this,
		CExtPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		rcWnd
		);
	g_PaintManager->PaintControlBarBorders( dc, _pcbbd );
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::DrawBorders( CDC * pDC, CRect& rc )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDC );
CExtPaintManager::PAINTCONTROLBARBORDERSDATA _pcbbd(
		this,
		CExtPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		rc
		);
	g_PaintManager->PaintControlBarBorders( *pDC, _pcbbd );
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::_SyncStatusBarColors()
{
COLORREF clrStatusBkNeeded =
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			);
	if( m_clrStatusBk != clrStatusBkNeeded )
	{
		m_clrStatusBk = clrStatusBkNeeded;
		GetStatusBarCtrl().SetBkColor( m_clrStatusBk );
	} // if( m_clrStatusBk != clrStatusBkNeeded )
	m_clrGripper = g_PaintManager->GetColor( CExtPaintManager::CLR_TEXT_OUT );
	m_clrPaneTextNormal = g_PaintManager->GetColor( COLOR_BTNTEXT );
	m_clrPaneTextDisabled = g_PaintManager->GetColor( COLOR_3DSHADOW );
	m_clrPaneRect = m_clrStatusBk;
	if( m_bCompleteRepaint )
		m_clrPaneRect = g_PaintManager->GetColor( COLOR_3DSHADOW );
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnSysColorChange() 
{
	CStatusBar::OnSysColorChange();
	g_PaintManager.OnSysColorChange( this );
	g_CmdManager.OnSysColorChange( this );
	_SyncStatusBarColors();
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection) 
{
//USES_CONVERSION;
//TRACE2("  ==> CExtStatusControlBar::OnSettingChange(0x%08X,\"%S\")\n", uFlags, (lpszSection == NULL) ? "NULL" : T2CA(lpszSection) );
	CStatusBar::OnSettingChange( uFlags, lpszSection );
	g_PaintManager.OnSettingChange( this, uFlags, lpszSection );
	g_CmdManager.OnSettingChange( this, uFlags, lpszSection );
	_SyncStatusBarColors();
}

//////////////////////////////////////////////////////////////////////////

LRESULT CExtStatusControlBar::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = CStatusBar::OnDisplayChange( wParam, lParam );
	g_PaintManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	g_CmdManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	return lResult;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CExtStatusControlBar::OnThemeChanged( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = Default();
	g_PaintManager.OnThemeChanged( this, wParam, lParam );
	g_CmdManager.OnThemeChanged( this, wParam, lParam );
	return lResult;
}

//////////////////////////////////////////////////////////////////////////

int CExtStatusControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CStatusBar::OnCreate(lpCreateStruct) == -1 )
		return -1;
	
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(GetSafeHwnd()) );
HWND hWndParent = ::GetParent( GetSafeHwnd() );
	ASSERT( hWndParent != NULL );
	ASSERT( ::IsWindow(hWndParent) );
	VERIFY(
		::SetWindowPos(
			GetSafeHwnd(),
			HWND_TOP,
			0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE
				|SWP_NOREDRAW|SWP_NOACTIVATE|SWP_NOCOPYBITS
				|SWP_NOSENDCHANGING
			)
		);

	_SyncStatusBarColors();
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::OnContextMenu(CWnd* pWnd, CPoint point) 
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

CFrameWnd * pFrame = _GetDockingFrameImpl();
	if( pFrame == NULL )
	{
		CStatusBar::OnContextMenu( pWnd, point );
		return;
	}

	ASSERT_VALID( pFrame );
HWND hWndTrack = GetOwner()->GetSafeHwnd();
	ASSERT( hWndTrack != NULL && ::IsWindow(hWndTrack) );
CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu(hWndTrack) );

	if(	! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
			pFrame,
			pPopup,
			CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_STATUSBAR_CTX,
			this,
			WM_CONTEXTMENU,
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
	::SetFocus( hWndTrack );
	if( ! pPopup->TrackPopupMenu(
			TPMX_OWNERDRAW_FIXED,
			point.x,
			point.y
			)
		)
	{
		delete pPopup;
	}
}

//////////////////////////////////////////////////////////////////////////

CFrameWnd * CExtStatusControlBar::_GetDockingFrameImpl()
{
	ASSERT_VALID( this );

CFrameWnd * pFrame = GetParentFrame();
	if( pFrame == NULL )
		return NULL;

	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		//ASSERT_VALID( pFrame );
		ASSERT( pFrame != NULL );
		ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
	}

	return pFrame;
}

//////////////////////////////////////////////////////////////////////////

BOOL CExtStatusControlBar::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CExtStatusControlBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult =CStatusBar::WindowProc(message, wParam, lParam);
	if( message == WM_SIZE ){
		RepositionControls();
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////////

void CExtStatusControlBar::RepositionControls()
{
	HDWP _hDWP = ::BeginDeferWindowPos( m_arrPaneControls.GetSize() );
	
	CRect rcClient;
	GetClientRect(&rcClient);
	for (int i = 0; i < m_arrPaneControls.GetSize(); i++ )
	{
		int   iIndex  = CommandToIndex(m_arrPaneControls[i]->nID);
		HWND hWnd    = m_arrPaneControls[i]->hWnd;
		
		CRect rcPane;
		GetItemRect(iIndex, &rcPane);
		
		// CStatusBar::GetItemRect() sometimes returns invalid size 
		// of the last pane - we will re-compute it
		int cx = ::GetSystemMetrics( SM_CXEDGE );
		DWORD dwPaneStyle = GetPaneStyle( iIndex );
		if( iIndex == (m_nCount-1) )
		{
			if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
			{
				UINT nID, nStyle;
				int  cxWidth;
				GetPaneInfo( iIndex, nID, nStyle, cxWidth );
				rcPane.right = rcPane.left + cxWidth + cx*3;
			} // if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
			else
			{
				CRect rcClient;
				GetClientRect( &rcClient );
				rcPane.right = rcClient.right;
				if( (GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP )
				{
					int cxSmIcon = ::GetSystemMetrics( SM_CXSMICON );
					rcPane.right -= cxSmIcon + cx;
				} // if( (GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP )
			} // else from if( (dwPaneStyle & SBPS_STRETCH ) == 0 )
		} // if( iIndex == (m_nCount-1) )
		
		if ((GetPaneStyle (iIndex) & SBPS_NOBORDERS) == 0 && 
			!m_bDrawPaneSeparatorsInsteadOfBorders){
			rcPane.DeflateRect(cx,cx);
		}else{
			rcPane.DeflateRect(cx,1,cx,1);
		}
		
		if (hWnd && ::IsWindow(hWnd)){
			_hDWP = ::DeferWindowPos(
				_hDWP, 
				hWnd, 
				NULL, 
				rcPane.left,
				rcPane.top, 
				rcPane.Width(), 
				rcPane.Height(),
				SWP_NOZORDER|SWP_NOACTIVATE
				);
		} // if (hWnd && ::IsWindow(hWnd)){ 
	}
	
	VERIFY( ::EndDeferWindowPos( _hDWP ) );
	Refresh();
};

//////////////////////////////////////////////////////////////////////////

bool CExtStatusControlBar::AddPane(
	 UINT nID,	// ID of the  pane
	 int nIndex	// index of the pane
	 )
{
	if (nIndex < 0 || nIndex > m_nCount){
		ASSERT(FALSE);
		return false;
	}
	
	if (CommandToIndex(nID) != -1){
		ASSERT(FALSE);
		return FALSE;
	}
	
	CArray<_STATUSBAR_PANE_*,_STATUSBAR_PANE_*> arrPanesTmp;
	int iIndex = 0;
	for (iIndex = 0; iIndex < m_nCount+1; iIndex++)
	{
		_STATUSBAR_PANE_* pNewPane = new _STATUSBAR_PANE_;
		
		if (iIndex == nIndex){
			pNewPane->nID    = nID;
			pNewPane->nStyle = SBPS_NORMAL;
		}else{
			int idx = iIndex;
			if (iIndex > nIndex) idx--;
			
			_STATUSBAR_PANE_* pOldPane  = GetPanePtr(idx);
			pNewPane->cxText  = pOldPane->cxText;
			pNewPane->nFlags  = pOldPane->nFlags;
			pNewPane->nID     = pOldPane->nID;
			pNewPane->nStyle  = pOldPane->nStyle;
			pNewPane->strText = pOldPane->strText;
		}
		arrPanesTmp.Add(pNewPane);
	}
	
	int nPanesCount = arrPanesTmp.GetSize();
	UINT* lpIDArray = new UINT[ nPanesCount ];
	for (iIndex = 0; iIndex < nPanesCount; iIndex++) {
		lpIDArray[iIndex] = arrPanesTmp[iIndex]->nID;
	}
	
	// set the indicators 
	SetIndicators(lpIDArray, nPanesCount);
	// free memory
	for (iIndex = 0; iIndex < nPanesCount; iIndex++){
		_STATUSBAR_PANE_* pPane = arrPanesTmp[iIndex];
		if (iIndex != nIndex)
			PaneInfoSet(iIndex, pPane);
		if(pPane) 
			delete pPane;
	}
	
	arrPanesTmp.RemoveAll();
	if(lpIDArray) 
		delete []lpIDArray;
	
	RepositionControls();
	
	Refresh();
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CExtStatusControlBar::RemovePane(
	UINT nID	// ID of the pane
	)
{
	if ( CommandToIndex(nID) == -1 || m_nCount == 1 ){
		ASSERT(FALSE);
		return false;
	}
	
	CArray<_STATUSBAR_PANE_*,_STATUSBAR_PANE_*> arrPanesTmp;
	int nIndex;
	for (nIndex = 0; nIndex < m_nCount; nIndex++)
	{
		_STATUSBAR_PANE_* pOldPane = GetPanePtr(nIndex);
		
		if (pOldPane->nID == nID)
			continue;
		
		_STATUSBAR_PANE_* pNewPane = new _STATUSBAR_PANE_;
		
		pNewPane->cxText  = pOldPane->cxText;
		pNewPane->nFlags  = pOldPane->nFlags;
		pNewPane->nID     = pOldPane->nID;
		pNewPane->nStyle  = pOldPane->nStyle;
		pNewPane->strText = pOldPane->strText;
		arrPanesTmp.Add(pNewPane);
	}
	
	UINT* lpIDArray = new UINT[arrPanesTmp.GetSize()];
	for (nIndex = 0; nIndex < arrPanesTmp.GetSize(); nIndex++) {
		lpIDArray[nIndex] = arrPanesTmp[nIndex]->nID;
	}
	
	// set the indicators
	SetIndicators(lpIDArray, arrPanesTmp.GetSize());
	// free memory
	for (nIndex = 0; nIndex < arrPanesTmp.GetSize(); nIndex++){
		_STATUSBAR_PANE_* pPane = arrPanesTmp[nIndex];
		PaneInfoSet(nIndex, pPane);
		if(pPane) 
			delete pPane;
	}
	
	for ( int i = 0; i < m_arrPaneControls.GetSize(); i++ ){
		if (m_arrPaneControls[i]->nID == nID){
			if( m_arrPaneControls[i]->hWnd && ::IsWindow(m_arrPaneControls[i]->hWnd) ) {
				::SendMessage(m_arrPaneControls[i]->hWnd, WM_CLOSE, 0, 0); 
				if( m_arrPaneControls[i]->bAutoDestroy ) {
					::DestroyWindow(m_arrPaneControls[i]->hWnd);
				}
			}
			_STATUSBAR_PANE_CTRL_ *pPaneCtrl = m_arrPaneControls[i];
			if( pPaneCtrl )
				delete pPaneCtrl;
			m_arrPaneControls.RemoveAt(i);
			break;
		}
	}
	
	arrPanesTmp.RemoveAll();
	if(lpIDArray) 
		delete []lpIDArray;
	
	RepositionControls();
	
	Refresh();
	return true;
}

//////////////////////////////////////////////////////////////////////////

BOOL CExtStatusControlBar::AddPaneControl(HWND hWnd, UINT nID, bool bAutoDestroy)
{
	if (CommandToIndex (nID) == -1) {
		return false;
	}
	
	_STATUSBAR_PANE_CTRL_* pPaneCtrl = new _STATUSBAR_PANE_CTRL_;
	pPaneCtrl->nID         = nID;
	pPaneCtrl->hWnd        = hWnd;
	pPaneCtrl->bAutoDestroy = bAutoDestroy;
	
	m_arrPaneControls.Add(pPaneCtrl);
	RepositionControls();
	return true;
}

bool CExtStatusControlBar::PaneInfoGet(int nIndex, _STATUSBAR_PANE_* pPane)
{
	if( nIndex < m_nCount  && nIndex >= 0 )
	{
		GetPaneInfo( nIndex,  pPane->nID, pPane->nStyle, pPane->cxText );
		CString strPaneText;
		GetPaneText( nIndex , strPaneText );
		pPane->strText = LPCTSTR(strPaneText);
		return true;
	}
	return false;
}

bool CExtStatusControlBar::PaneInfoSet(int nIndex, _STATUSBAR_PANE_* pPane)
{
	if( nIndex < m_nCount  && nIndex >= 0 ){
		SetPaneInfo( nIndex, pPane->nID, pPane->nStyle, pPane->cxText );
		SetPaneText( nIndex, LPCTSTR( pPane->strText) );
		return true;
	}
	return false;
}

