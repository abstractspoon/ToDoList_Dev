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
#include "ExtButton.h"

#include "ExtCmdManager.h"
#include "ExtPaintManager.h"
#include "ExtMemoryDC.h"
#include "ExtPopupMenuWnd.h"
#include "ExtColorDlg.h"
#include "ExtToolControlBar.h"

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExtButton::CExtButton()
{
	m_hIconIn = NULL;
	m_hIconOut = NULL;
	m_hCursor = NULL;
  
	m_bIsFlat = FALSE; 
	m_bDrawBorder = TRUE; 
	m_bSeparatedDropDown = FALSE;
	m_bAutoChangeID = m_bAutoChangeCmdIcon = FALSE;

	m_bMouseOver = m_bPushed = m_bPushTracking = m_bKeyTracking = FALSE;
	m_nTrackingHT = 0;
  
	m_nAlign =
		CExtPaintManager::e_align_t::__ALIGN_HORIZ_CENTER
		|CExtPaintManager::e_align_t::__ALIGN_VERT
		; 
	m_bIsDefault = FALSE;

	m_wndToolTip.m_hWnd = NULL;

	m_dwMenuOpt = TPMX_TOPALIGN;
	m_nMenuLeftAreaWidth = 0;
}

CExtButton::~CExtButton()
{
	_FreeWinObjects();
}

void CExtButton::_FreeWinObjects()
{
	if( m_hIconIn != NULL )
		::DestroyIcon( m_hIconIn );
	if( m_hIconOut != NULL )
		::DestroyIcon( m_hIconOut );
	if( m_hCursor != NULL )
		::DestroyCursor(m_hCursor);
}

IMPLEMENT_DYNCREATE(CExtButton, CButton);

BEGIN_MESSAGE_MAP(CExtButton, CButton)
    //{{AFX_MSG_MAP(CExtButton)
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtToolControlBar::g_nMsgPutToPopupMenu,
		_OnPutToPopupMenuImpl
		)
	ON_WM_SYSCOLORCHANGE()
	__EXT_MFC_SAFE_ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(__ExtMfc_WM_THEMECHANGED, OnThemeChanged)
END_MESSAGE_MAP()

LRESULT CExtButton::_OnPutToPopupMenuImpl(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtPopupMenuWnd * pPopup = reinterpret_cast < CExtPopupMenuWnd * > (wParam);
	ASSERT_VALID( pPopup );
	return OnPutToPopupMenu( pPopup ) ? TRUE : FALSE;
}

bool CExtButton::OnPutToPopupMenu( CExtPopupMenuWnd * pPopup )
{
	ASSERT_VALID( pPopup );

//bool bInBar = false;
//CWnd * pWnd = GetParent();
//	ASSERT_VALID( pWnd );
//	if( pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)) )
//	{
//		bInBar = true;
//		pWnd = pWnd->GetParentFrame();
//		ASSERT_VALID( pWnd );
//		if( pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
//		{
//			pWnd = pWnd->GetParentFrame();
//			ASSERT_VALID( pWnd );
//		}
//	}
	
	if( m_menu.GetSafeHmenu() != NULL )
	{
		ASSERT( ::IsMenu( m_menu.GetSafeHmenu() ) );
		CExtPopupMenuWnd * pPopupIns = new CExtPopupMenuWnd;
		VERIFY(
			pPopupIns->CreatePopupMenu(
				//pWnd->GetSafeHwnd()
				pPopup->GetCmdRecieverHWND()
				)
			);
		VERIFY(
			pPopupIns->UpdateFromMenu(
				pPopup->GetCmdRecieverHWND(),
				&m_menu
				)
			);
		HICON hIcon = NULL;
		if( m_hIconIn != NULL )
		{
			CExtCmdIcon _icon( m_hIconIn, true );
			hIcon = _icon.DetachAsHICON();
		}
		CString sPopupText( _T("") );
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
				GetDlgCtrlID()
				);
		if( pCmdItem != NULL )
			sPopupText = pCmdItem->m_sMenuText;
		if( sPopupText.IsEmpty() )
			GetWindowText( sPopupText );
		VERIFY(
			pPopup->ItemInsertSpecPopup(
				pPopupIns,
				-1,
				(LPCTSTR)sPopupText,
				hIcon
				)
			);
		return true;
	}

	return false;
}

void CExtButton::SetIcon(int nIconInId, int nIconOutId)
{
HICON hIconIn;
HICON hIconOut;
HINSTANCE hInstResource =
		AfxFindResourceHandle(
			MAKEINTRESOURCE(nIconInId),
			RT_GROUP_ICON
			);

	hIconIn = (HICON)::LoadImage(
		hInstResource,
		MAKEINTRESOURCE(nIconInId),
		IMAGE_ICON,
		0,
		0,
		0
		);
	hIconOut =
		(nIconOutId == NULL) ?
			NULL
			:
			(HICON)::LoadImage(
				hInstResource,
				MAKEINTRESOURCE(nIconOutId),
				IMAGE_ICON,
				0,
				0,
				0
				);
	SetIcon(hIconIn, hIconOut);
}

void CExtButton::SetIcon(HICON hIconIn, HICON hIconOut)
{
	if( m_hIconIn != NULL)
		::DestroyIcon( m_hIconIn );
	if( m_hIconOut != NULL)
		::DestroyIcon( m_hIconOut );

	m_hIconIn = hIconIn;
	m_hIconOut =
		(hIconOut == NULL) ?
			m_hIconIn : hIconOut;

	if(		GetSafeHwnd() != NULL
		&&	::IsWindow( GetSafeHwnd() )
		)
	{
		Invalidate();
		UpdateWindow();
	}
} 

BOOL CExtButton::SetBtnCursor(int nCursorId)
{
HINSTANCE hInstResource;
	if( m_hCursor != NULL )
		::DestroyCursor(m_hCursor);
	m_hCursor = NULL;
	if( nCursorId != -1 )
	{
		hInstResource =
			AfxFindResourceHandle(
				MAKEINTRESOURCE(nCursorId),
				RT_GROUP_CURSOR
				);
		m_hCursor =
			(HCURSOR)::LoadImage(
				hInstResource,
				MAKEINTRESOURCE(nCursorId),
				IMAGE_CURSOR,
				0,
				0,
				0
				);
		if( m_hCursor == NULL )
		{
			m_hCursor =
				::LoadCursor(
					NULL,
					MAKEINTRESOURCE(nCursorId)
					);
			if( m_hCursor == NULL )
				return FALSE;
		}
	}
	return TRUE;
}

void CExtButton::SetFlat( BOOL bState )
{
	m_bIsFlat = bState;
	if(		GetSafeHwnd() != NULL
		&&	::IsWindow( GetSafeHwnd() )
		)
	{
		Invalidate();
		UpdateWindow();
	}
}

BOOL CExtButton::GetFlat() const
{
	return m_bIsFlat;
}

void CExtButton::SetAlign( int nAlign )
{
	switch( nAlign )
	{    
	case CExtPaintManager::e_align_t::__ALIGN_HORIZ_CENTER:
		m_nAlign = CExtPaintManager::e_align_t::__ALIGN_HORIZ_CENTER;
	break;
	case CExtPaintManager::e_align_t::__ALIGN_HORIZ_RIGHT:
		m_nAlign = CExtPaintManager::e_align_t::__ALIGN_HORIZ_RIGHT;
	break;
	case CExtPaintManager::e_align_t::__ALIGN_VERT:
		m_nAlign = CExtPaintManager::e_align_t::__ALIGN_VERT;
	break;
	} // switch( nAlign )

	if(		GetSafeHwnd() != NULL
		&&	::IsWindow( GetSafeHwnd() )
		)
	{
		Invalidate();
		UpdateWindow();
	}
}

int CExtButton::GetAlign() const
{
	return m_nAlign;
}

BOOL CExtButton::GetDrawBorder() const
{
	return m_bDrawBorder;
}

void CExtButton::SetDrawBorder( BOOL bEnable )
{
	m_bDrawBorder = bEnable;
	if(		GetSafeHwnd() != NULL
		&&	::IsWindow( GetSafeHwnd() )
		)
	{
		Invalidate();
		UpdateWindow();
	}
}

BOOL CExtButton::GetSeparatedDropDown() const
{
	return m_bSeparatedDropDown;
}

void CExtButton::SetSeparatedDropDown(
	BOOL bSeparatedDropDown // = TRUE
	)
{
	m_bSeparatedDropDown = bSeparatedDropDown;
	if(		GetSafeHwnd() != NULL
		&&	::IsWindow( GetSafeHwnd() )
		)
	{
		Invalidate();
		UpdateWindow();
	}
}

BOOL CExtButton::GetAutoChangeID() const
{
	return m_bAutoChangeID;
}

void CExtButton::SetAutoChangeID(
	BOOL bAutoChangeID // = TRUE
	)
{
	m_bAutoChangeID = bAutoChangeID;
}

BOOL CExtButton::GetAutoChangeCmdIcon() const
{
	return m_bAutoChangeCmdIcon;
}

void CExtButton::SetAutoChangeCmdIcon(
	BOOL bAutoChangeCmdIcon // = TRUE
	)
{
	m_bAutoChangeCmdIcon = bAutoChangeCmdIcon;
}

void CExtButton::_CalcClientRects(
	CRect & rcMainArea,
	CRect & rcDropDown
	) const
{
	GetClientRect( &rcMainArea );
	rcDropDown = rcMainArea;
	rcDropDown.left = rcDropDown.right;
	if( !m_bSeparatedDropDown )
		return;
	rcDropDown.left -= 
			__DROPDOWN_ARROW_GAP*2
			+ CExtPaintManager::g_glyph_btn_expand_bottom.Size().cx
			;
	rcMainArea.right = rcDropDown.left;
}

INT CExtButton::_HtMouseOnButton( const POINT & ptClient ) const // 0 - not, 1-main area, 2-dropdown area
{
CRect rcMainArea, rcDropDown;
	_CalcClientRects( rcMainArea, rcDropDown );
	if( rcMainArea.PtInRect(ptClient) )
		return 1;
	if( !m_bSeparatedDropDown )
		return 0;
	if( rcDropDown.PtInRect(ptClient) )
		return 2;
	return 0;
}

INT CExtButton::_HtMouseOnButton() const // 0 - not, 1-main area, 2-dropdown area
{
POINT point;
	if( ! ::GetCursorPos( &point ) )
		return 0;
HWND hWndUnderMouse = ::WindowFromPoint( point );
	if( hWndUnderMouse == NULL || hWndUnderMouse != m_hWnd )
		return false;
	if( !m_bSeparatedDropDown )
		return 1;
	ScreenToClient( &point );
	return _HtMouseOnButton( point );
}

void CExtButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bKeyTracking )
		return;

	CButton::OnMouseMove(nFlags, point);

INT nTrackingHT = _HtMouseOnButton();
	if(	(	((!m_bPushTracking) && nTrackingHT!=0)
		||	( m_bPushTracking && nTrackingHT==m_nTrackingHT )
		)

		&&	CExtPopupMenuWnd::TestHoverEnabledFromActiveHWND( GetSafeHwnd() )
		)
	{ // if mouse is really on button
		if( CExtMouseCaptureSink::GetCapture() != GetSafeHwnd() )
			CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
		if( m_bMouseOver )
			return;
		m_bMouseOver = TRUE;
		m_bPushed =
			((nFlags & MK_LBUTTON) && m_bPushTracking )
				? TRUE : FALSE;
		Invalidate();
		UpdateWindow();
	} // if mouse is really on button
	else
	{ // if mouse is really NOT on button
		if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() && ( !m_bPushTracking ) )
			CExtMouseCaptureSink::ReleaseCapture();
		if( !m_bMouseOver )
			return;
		m_bMouseOver = m_bPushed = FALSE;
		Invalidate();
		UpdateWindow();
	} // if mouse is really NOT on button
}

void CExtButton::OnKillFocus(CWnd * pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	if( m_bMouseOver || m_bPushed )
	{
		if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
			CExtMouseCaptureSink::ReleaseCapture();
		m_bMouseOver = m_bPushed = FALSE;
		Invalidate();
		UpdateWindow();
	}
}

void CExtButton::OnCaptureChanged(CWnd *pWnd) 
{
	if( (m_bMouseOver || m_bPushed)
		&& pWnd != this
		)
	{
//		CExtMouseCaptureSink::ReleaseCapture();
		m_bMouseOver = m_bPushed = FALSE;
		Invalidate();
		UpdateWindow();
	}
	CButton::OnCaptureChanged(pWnd);
}

void CExtButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
UINT nBS = GetButtonStyle();
	if( nBS & BS_DEFPUSHBUTTON )
		m_bIsDefault = TRUE;
	if( nBS & BS_OWNERDRAW )
		SetButtonStyle( nBS & ~(BS_OWNERDRAW) );
	SetFont(
		m_bIsDefault
			? &g_PaintManager->m_FontBold
			: &g_PaintManager->m_FontNormal
			,
		FALSE
		);
	PostMessage( // delayed repainting
		WM_ENABLE,
		(WPARAM)IsWindowEnabled()
		);
}

BOOL CExtButton::PreTranslateMessage(MSG* pMsg) 
{
	if( !CExtPopupMenuWnd::IsMenuTracking() )
	{
		InitToolTip();
		m_wndToolTip.RelayEvent(pMsg);
	}

//	if( CExtPopupMenuWnd::IsMenuTracking() )
//		return TRUE;
	if(		(	pMsg->message == WM_KEYDOWN
				|| pMsg->message == WM_KEYUP
			)
		&&
			(	pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_SPACE
			)
		)
	{
		if( IsWindowEnabled() && IsWindowVisible() && (!m_bPushTracking) )
		{
			if(	(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
				||
				(pMsg->message == WM_KEYUP && pMsg->wParam == VK_SPACE)
				)
			{
				if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
					CExtMouseCaptureSink::ReleaseCapture();
				m_bMouseOver = m_bPushed = m_bPushTracking = m_bKeyTracking = FALSE;
				Invalidate();
				UpdateWindow();
				_OnClick( true, false );
				return TRUE;
			}
			if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
			{
				m_bMouseOver = m_bPushed = m_bKeyTracking = TRUE;
				CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
				Invalidate();
				UpdateWindow();
				return TRUE;
			}
		} // if( IsWindowEnabled() && IsWindowVisible() && (!m_bPushTracking) )
		return TRUE;
	}

	return CButton::PreTranslateMessage(pMsg);
}

LRESULT CExtButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
bool bInvalidate = false;
	switch( message )
	{
	case WM_LBUTTONDBLCLK:
		message = WM_LBUTTONDOWN;
	break;
	case BM_SETCHECK:
	case BM_SETSTATE:
	case BM_SETSTYLE:
#if (WINVER >= 0x0400)
	case BM_CLICK:
	case BM_SETIMAGE:
#endif
	case WM_SETTEXT:
	case WM_SIZE:
	case WM_WINDOWPOSCHANGED:
		bInvalidate = true;
	break;
	} // switch( message )

LRESULT lResult =
		CButton::DefWindowProc(message, wParam, lParam);
	if( bInvalidate )
	{
		Invalidate();
		UpdateWindow();
	}
	return lResult;
}

BOOL CExtButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if( m_hCursor != NULL )
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void CExtButton::SetTooltipText(CExtSafeString* spText, BOOL bActivate)
{
	if( spText == NULL )
		return;
	InitToolTip();
	if( m_wndToolTip.GetToolCount() == 0 )
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_wndToolTip.AddTool( this, *spText, rectBtn, 1 );
	}
	m_wndToolTip.UpdateTipText( *spText, this, 1 );
	m_wndToolTip.Activate(bActivate);
}

void CExtButton::SetTooltipText(CExtSafeString & sText, BOOL bActivate)
{
	if( sText.IsEmpty() )
		return;
	InitToolTip();
	if( m_wndToolTip.GetToolCount() == 0 )
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_wndToolTip.AddTool( this, sText, rectBtn, 1 );
	}
	m_wndToolTip.UpdateTipText( sText, this, 1 );
	m_wndToolTip.Activate( bActivate );
}

void CExtButton::SetTooltipText(__EXT_MFC_SAFE_LPCTSTR sText, BOOL bActivate)
{
	if( sText == NULL
		|| _tcslen( sText ) == 0
		)
		return;
	InitToolTip();
	if( m_wndToolTip.GetToolCount() == 0 )
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_wndToolTip.AddTool(this,sText,rectBtn,1);
	}
	m_wndToolTip.UpdateTipText(sText,this,1);
	m_wndToolTip.Activate(bActivate);
}


void CExtButton::SetTooltipText(int nId, BOOL bActivate)
{
CExtSafeString sText;
	sText.LoadString(nId);
	if( !sText.IsEmpty() )
		SetTooltipText(&sText, bActivate);
}

void CExtButton::ActivateTooltip(BOOL bActivate)
{
	if( m_wndToolTip.GetToolCount() == 0 )
		return;
	m_wndToolTip.Activate(bActivate);
}

BOOL CExtButton::GetDefault() const
{
	return m_bIsDefault;
}

void CExtButton::InitToolTip()
{
	if( m_wndToolTip.m_hWnd == NULL )
	{
		m_wndToolTip.Create(this);
		m_wndToolTip.Activate(FALSE);
	}
}

void CExtButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	g_PaintManager.OnSysColorChange( this );
	g_CmdManager.OnSysColorChange( this );
}

void CExtButton::OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection) 
{
	CButton::OnSettingChange( uFlags, lpszSection );
	g_PaintManager.OnSettingChange( this, uFlags, lpszSection );
	g_CmdManager.OnSettingChange( this, uFlags, lpszSection );
}

LRESULT CExtButton::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = CButton::OnDisplayChange( wParam, lParam );
	g_PaintManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	g_CmdManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	return lResult;
}

LRESULT CExtButton::OnThemeChanged( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = Default();
	g_PaintManager.OnThemeChanged( this, wParam, lParam );
	g_CmdManager.OnThemeChanged( this, wParam, lParam );
	return lResult;
}

void CExtButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_bKeyTracking )
		return;

	CButton::OnLButtonDown( nFlags, point );

	m_nTrackingHT = _HtMouseOnButton();
	if( m_nTrackingHT == 0 )
		return;
	m_bMouseOver = m_bPushed = m_bPushTracking = TRUE;
	CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
	
	Invalidate();
	UpdateWindow();
}

void CExtButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	nFlags;
	point;

	if( m_bKeyTracking )
		return;

INT nTrackingHT = _HtMouseOnButton();
BOOL bWasPushed = m_bPushed && m_bPushTracking && (nTrackingHT == m_nTrackingHT);
	m_bMouseOver = m_bPushed = m_bPushTracking = FALSE;
	m_nTrackingHT = 0;

	if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
		CExtMouseCaptureSink::ReleaseCapture();
	Invalidate();
	UpdateWindow();

	if( !bWasPushed )
		return;

	//CButton::OnLButtonDown(nFlags, point);
	if( IsWindowEnabled()
		&& IsWindowVisible()
		)
		_OnClick(
			false,
			(m_bSeparatedDropDown && nTrackingHT == 2) ? true : false
			);
}

void CExtButton::_OnClick(
	bool bSelectAny,
	bool bSeparatedDropDownClicked
	)
{
	if(		(!m_bSeparatedDropDown)
		||	bSeparatedDropDownClicked
		)
	{
		if( _OnTrackPopup( bSelectAny ) )
			return;
	}

CWnd * pWnd = GetParent();
	ASSERT( pWnd != NULL );
	ASSERT_VALID( pWnd );
	if( m_menu.GetSafeHmenu() == NULL
		|| (m_bSeparatedDropDown && (!bSeparatedDropDownClicked))
		)
	{
		HWND hWndThis = GetSafeHwnd();
		int nDlgCtrlID = GetDlgCtrlID();
		pWnd->SendMessage(
			WM_COMMAND,
			MAKEWPARAM(nDlgCtrlID,BN_CLICKED),
			(LPARAM)m_hWnd
			);
		if( ::IsWindow(hWndThis) )
		{
			Invalidate();
			UpdateWindow();
		}
		return;
	}
}

bool CExtButton::_IsMenuAvail()
{
	if( m_menu.GetSafeHmenu() == NULL )
		return false;
	return true;
}

bool CExtButton::_OnTrackPopup(
	bool bSelectAny
	)
{
	if( m_menu.GetSafeHmenu() == NULL )
		return false;

	ASSERT( ::IsMenu(m_menu.GetSafeHmenu()) );
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			m_menu.GetSafeHmenu()
			)
		);
CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	pPopup->SetLeftAreaWidth( m_nMenuLeftAreaWidth );
CWnd * pWnd = GetParent();
	ASSERT_VALID( pWnd );
DWORD dwAlignOpt = 0;
DWORD dwMenuOpt = m_dwMenuOpt;
	while( pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)) )
	{
		if( dwAlignOpt == 0 && ((CControlBar *)pWnd)->m_pDockBar != NULL )
		{
			ASSERT_VALID( ((CControlBar *)pWnd)->m_pDockBar );
			dwMenuOpt &= ~TPMX_ALIGN_MASK;
			UINT nDockBarID = ((CControlBar *)pWnd)->m_pDockBar->GetDlgCtrlID();
			switch( nDockBarID )
			{
			case AFX_IDW_DOCKBAR_TOP:
				dwAlignOpt = TPMX_TOPALIGN;
				break;
			case AFX_IDW_DOCKBAR_BOTTOM:
				dwAlignOpt = TPMX_BOTTOMALIGN;
				break;
			case AFX_IDW_DOCKBAR_LEFT:
				dwAlignOpt = TPMX_LEFTALIGN;
				break;
			case AFX_IDW_DOCKBAR_RIGHT:
				dwAlignOpt = TPMX_RIGHTALIGN;
				break;
			case AFX_IDW_DOCKBAR_FLOAT:
				dwAlignOpt = TPMX_TOPALIGN;
				break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
				break;
#endif // _DEBUG
			} // switch( nDockBarID )
		} // if( dwAlignOpt == 0 && ((CControlBar *)pWnd)->m_pDockBar != NULL )
		pWnd = pWnd->GetParent();
		if( pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		{
			pWnd = pWnd->GetParentFrame();
			ASSERT_VALID( pWnd );
			break;
		}
	}
	ASSERT( pWnd != NULL );
	ASSERT_VALID( pWnd );
	VERIFY(
		pPopup->UpdateFromMenu( pWnd->GetSafeHwnd(), &m_menu )
		);
CRect wr;
	GetWindowRect( &wr );
CPoint pt = wr.CenterPoint();
	SetFocus();

	if( m_bAutoChangeID )
	{
		UINT nResultCmdID = 0;
		if( !pPopup->TrackPopupMenu(
				dwMenuOpt
					| dwAlignOpt
					| ( (bSelectAny) ? TPMX_SELECT_ANY : 0 )
					| TPMX_DO_MESSAGE_LOOP | TPMX_NO_WM_COMMAND
					| TPMX_COMBINE_DEFAULT,
				pt.x,
				pt.y,
				&wr,
				this,
				_CbPaintCombinedContent,
				& nResultCmdID,
				true
				)
			)
		{
			delete pPopup;
			ASSERT( FALSE );
		}
		else
		{
			if( nResultCmdID != 0 )
			{
				SetDlgCtrlID( nResultCmdID );
				if( m_bAutoChangeCmdIcon )
				{
					__EXT_MFC_SAFE_LPCTSTR sProfileName =
						g_CmdManager->ProfileNameFromWnd(m_hWnd);
					if( sProfileName != NULL )
					{
						CExtCmdItem * pCmdItem =
							g_CmdManager->CmdGetPtr(
								sProfileName,
								nResultCmdID
								);
						if( pCmdItem != NULL )
						{
							CExtSafeString sText =
								pCmdItem->m_sToolbarText.IsEmpty()
									? pCmdItem->m_sMenuText
									: pCmdItem->m_sToolbarText
									;
							SetWindowText( sText );
							HICON hIcon = 
								g_CmdManager->CmdGetHICON(
									sProfileName,
									nResultCmdID
									);
							if( hIcon == NULL )
								SetIcon( NULL );
							{
								CExtCmdIcon _icon( hIcon, true );
								SetIcon( _icon.DetachAsHICON() );
							}
						}
					}
				}
				CWnd * pWnd = GetParent();
				ASSERT( pWnd != NULL );
				ASSERT_VALID( pWnd );
				HWND hWndThis = GetSafeHwnd();
				pWnd->SendMessage(
					WM_COMMAND,
					MAKEWPARAM(nResultCmdID,BN_CLICKED),
					(LPARAM)m_hWnd
					);
				if( ::IsWindow(hWndThis) )
				{
					Invalidate();
					UpdateWindow();
				}
			}
		}
	} // if( m_bAutoChangeID )
	else
	{
		if( !pPopup->TrackPopupMenu(
				dwMenuOpt
					| dwAlignOpt
					| ( (bSelectAny) ? TPMX_SELECT_ANY : 0 )
					//| TPMX_DO_MESSAGE_LOOP
					| TPMX_COMBINE_DEFAULT,
				pt.x,
				pt.y,
				&wr,
				this,
				_CbPaintCombinedContent,
				NULL,
				true
				)
			)
		{
			delete pPopup;
			ASSERT( FALSE );
		}
	} // else from if( m_bAutoChangeID )

	return true;
}

void CExtButton::_CbPaintCombinedContent(
	LPVOID pCookie,
	CDC & dc,
	const CWnd & refWndMenu,
	const CRect & rcExcludeArea, // in screen coords
	int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
	)
{
	pCookie;
	dc;
	refWndMenu;
	rcExcludeArea;
	eCombineAlign;
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( refWndMenu.GetSafeHwnd() != NULL );

CExtButton * pBtn = (CExtButton *)pCookie;
	ASSERT( pBtn != NULL );
	ASSERT_VALID( pBtn );
	ASSERT( pBtn->IsKindOf(RUNTIME_CLASS(CExtButton)) );
	
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );

	if( rcExcludeArea.IsRectEmpty() )
		return;

CRect rcClientBar,rcClientExcludeArea;
	pBtn->GetClientRect( &rcClientBar );
	pBtn->ClientToScreen( &rcClientBar );
	refWndMenu.GetClientRect( &rcClientExcludeArea );
	refWndMenu.ClientToScreen( &rcClientExcludeArea );
CPoint ptOffset =
		rcClientBar.TopLeft()
		- rcClientExcludeArea.TopLeft()
		;
CPoint ptViewportOrg = dc.GetViewportOrg();
	dc.SetViewportOrg( ptOffset );
	pBtn->_RenderImpl(dc,true);
	dc.SetViewportOrg( ptViewportOrg );
}

void CExtButton::_RenderImpl(
	CDC & dc,
	bool bTransparent // = false
	)
{
	ASSERT_VALID( this );
CRect rectClient;
	GetClientRect( &rectClient );
CString sWindowText( _T("") );
	GetWindowText( sWindowText );
bool bDrawFocusRect = (GetFocus() == this) ? true : false;
bool bDefault = GetDefault() ? true : false;
bool bPushed = (m_bPushed || GetCheck()) ? true : false;
bool bEnabled = IsWindowEnabled() ? true : false;
bool bFlat = GetFlat() ? true : false;
bool bHover =
		( m_bMouseOver
		&& !CExtPopupMenuWnd::IsMenuTracking() )
			? true : false;
HICON hIcon = (bHover) ? m_hIconIn : m_hIconOut;
	if( GetParent()->IsKindOf(RUNTIME_CLASS(CControlBar)) )
	{
		bDrawFocusRect = false;
		bFlat = true;
	}
	if(		(! bTransparent )
		&&	(! (bHover || bPushed) )
		&&	g_PaintManager->GetCb2DbTransparentMode(this)
		)
	{
		CExtPaintManager::stat_ExcludeChildAreas(
			dc,
			GetSafeHwnd(),
			CExtPaintManager::stat_DefExcludeChildAreaCallback
			);
		g_PaintManager->PaintDockerBkgnd( dc, this );
		bTransparent = true;
	}

CExtCmdIcon icon( hIcon, false );
CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
		this,										   
		true, rectClient, (LPCTSTR)sWindowText,
		&icon, bFlat, bHover, bPushed,
		false, bEnabled,
		m_bDrawBorder ? true : false,
		bDrawFocusRect, bDefault, m_nAlign,
		NULL, _IsMenuAvail(), 0, bTransparent
		);
	_ppbd.m_bSeparatedDropDown = m_bSeparatedDropDown ? true : false;
	if( _ppbd.m_bSeparatedDropDown
		&& m_nTrackingHT == 2
		&& _ppbd.m_bPushed
		)
	{
		_ppbd.m_bPushed = false;
		_ppbd.m_bPushedDropDown = true;
//		_ppbd.m_bHover = false;
	}
	g_PaintManager->PaintPushButton( dc, _ppbd );
	icon.DetachAsHICON();
}

void CExtButton::_DrawImpl(
	CDC & dc,
	bool bTransparent // = false
	)
{
CRect rectClient;
	GetClientRect( &rectClient );
CExtMemoryDC dcmm( &dc, &rectClient );
	_RenderImpl(dcmm,bTransparent);
}

void CExtButton::OnPaint() 
{
CPaintDC dcPaint( this );
	_DrawImpl( dcPaint );
}

BOOL CExtButton::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
//	_DrawImpl( *pDC );
	return FALSE;
}

void CExtButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
CClientDC dc(this);
	_DrawImpl( dc );
}

CExtColorButton::CExtColorButton()
{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
	m_bUseStdColorDlg = false;
#else
	m_bUseStdColorDlg = true;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
	m_bEnableBtnColorDefault = true;
	m_bEnableBtnColorCustom = true;
	m_clrDefault = (COLORREF)(-1); // unexisting
	m_clrSelected = RGB(0,0,0);
	m_lParamCookie = 0;

	m_sBtnTextColorDefault = _T("");
	m_sBtnTextColorCustom = _T("");
}

CExtColorButton::~CExtColorButton()
{
	_FreeWinObjects();
}

IMPLEMENT_DYNCREATE(CExtColorButton, CExtButton);

BEGIN_MESSAGE_MAP(CExtColorButton, CExtButton)
    //{{AFX_MSG_MAP(CExtColorButton)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChanged,
		OnColorChanged
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)
END_MESSAGE_MAP()

bool CExtColorButton::_IsMenuAvail()
{
	return true;
}

bool CExtColorButton::_OnTrackPopup(
	bool bSelectAny
	)
{

bool bInBar = false;
CWnd * pWnd = GetParent();
	ASSERT_VALID( pWnd );
DWORD dwAlignOpt = 0;
DWORD dwMenuOpt = m_dwMenuOpt;
	while( pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)) )
	{
		bInBar = true;
		if( dwAlignOpt == 0 && ((CControlBar *)pWnd)->m_pDockBar != NULL )
		{
			ASSERT_VALID( ((CControlBar *)pWnd)->m_pDockBar );
			dwMenuOpt &= ~TPMX_ALIGN_MASK;
			UINT nDockBarID = ((CControlBar *)pWnd)->m_pDockBar->GetDlgCtrlID();
			switch( nDockBarID )
			{
			case AFX_IDW_DOCKBAR_TOP:
				dwAlignOpt = TPMX_TOPALIGN;
				break;
			case AFX_IDW_DOCKBAR_BOTTOM:
				dwAlignOpt = TPMX_BOTTOMALIGN;
				break;
			case AFX_IDW_DOCKBAR_LEFT:
				dwAlignOpt = TPMX_LEFTALIGN;
				break;
			case AFX_IDW_DOCKBAR_RIGHT:
				dwAlignOpt = TPMX_RIGHTALIGN;
				break;
			case AFX_IDW_DOCKBAR_FLOAT:
				dwAlignOpt = TPMX_TOPALIGN;
				break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
				break;
#endif // _DEBUG
			} // switch( nDockBarID )
		} // if( dwAlignOpt == 0 && ((CControlBar *)pWnd)->m_pDockBar != NULL )
		pWnd = pWnd->GetParent();
		if( pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		{
			pWnd = pWnd->GetParentFrame();
			ASSERT_VALID( pWnd );
			break;
		}
	}

//	if( ! (	CExtPopupMenuSite ::g_DefPopupMenuSite.IsEmpty()
//			|| CExtPopupMenuSite ::g_DefPopupMenuSite.IsShutdownMode()
//			)
//		)
//	{
//		CExtPopupMenuWnd * pPopupCurr =
//			CExtPopupMenuSite ::g_DefPopupMenuSite.GetInstance();
//		ASSERT_VALID( pPopupCurr );
//		if(		pPopupCurr->GetSafeHwnd() != NULL
//			&&	( ::IsWindow(pPopupCurr->GetSafeHwnd()) )
//			&&	pPopupCurr->GetParent()->GetSafeHwnd() ==
//					( bInBar ? pWnd->GetSafeHwnd() : GetSafeHwnd() )
//			)
//			return true;
//	}

CExtPopupColorMenuWnd * pPopup = new CExtPopupColorMenuWnd;
	pPopup->SetLeftAreaWidth( m_nMenuLeftAreaWidth );
	pPopup->m_lParamCookie = m_lParamCookie;

	VERIFY(
		pPopup->CreatePopupMenu(
			bInBar ? pWnd->GetSafeHwnd() : GetSafeHwnd()
			)
		);
	pPopup->m_bEnableBtnColorDefault =
		m_bEnableBtnColorDefault;
	pPopup->m_bEnableBtnColorCustom =
		m_bEnableBtnColorCustom;
	pPopup->m_clrDefault =
		m_clrDefault;
	pPopup->m_clrInitial =
		m_clrSelected;
	if( !m_sBtnTextColorDefault.IsEmpty() )
		pPopup->m_sBtnTextColorDefault = m_sBtnTextColorDefault;
	if( !m_sBtnTextColorCustom.IsEmpty() )
		pPopup->m_sBtnTextColorCustom = m_sBtnTextColorCustom;

CRect wr;
	GetWindowRect( &wr );
CPoint pt = wr.CenterPoint();
	if( bInBar)
		pWnd->SetFocus();
	else
		SetFocus();
bool bToolMenuTracking = false;
	if( bInBar )
	{
		CExtToolControlBar::_CloseTrackingMenus();
		bToolMenuTracking = true;
	}
	if( !pPopup->TrackPopupMenu(
			dwMenuOpt
				| dwAlignOpt
				| ( (bSelectAny) ? TPMX_SELECT_ANY : 0 )
				//| TPMX_DO_MESSAGE_LOOP
				| TPMX_COMBINE_DEFAULT
				,
			pt.x,
			pt.y,
			&wr,
			this,
			_CbPaintCombinedContent,
			NULL,
			true
			)
		)
	{
		delete pPopup;
		ASSERT( FALSE );
	}
	else
	{
		if( bToolMenuTracking )
			CExtToolControlBar::g_bMenuTracking = true;
	}
	return true;
}

void CExtColorButton::OnGenerateColorBtnIcon(
	HICON * p_hIcon,
	bool * p_bDestroyIcon
	)
{
	ASSERT( p_hIcon != NULL );
	ASSERT( p_bDestroyIcon != NULL );
	*p_bDestroyIcon = false;
	*p_hIcon =
		CExtPaintManager::stat_GenerateColorIconBox(
			m_clrSelected
			);
	ASSERT( *p_hIcon != NULL );
	if( *p_hIcon != NULL )
		*p_bDestroyIcon = true;
}

void CExtColorButton::_RenderImpl( // non-buffered paint
	CDC & dc,
	bool bTransparent // = false
	)
{
HICON hIcon = NULL;
bool bDestroyIcon = false;
	OnGenerateColorBtnIcon( &hIcon, &bDestroyIcon );

CRect rectClient;
	GetClientRect( &rectClient );
CString sWindowText( _T("") );
	GetWindowText( sWindowText );
bool bDrawFocusRect = (GetFocus() == this) ? true : false;
bool bDefault = GetDefault() ? true : false;
bool bPushed = (m_bPushed || GetCheck()) ? true : false;
bool bEnabled = IsWindowEnabled() ? true : false;
bool bFlat = GetFlat() ? true : false;
bool bHover =
		( m_bMouseOver
		&& !CExtPopupMenuWnd::IsMenuTracking() )
			? true : false;
	if( GetParent()->IsKindOf(RUNTIME_CLASS(CControlBar)) )
	{
		bDrawFocusRect = false;
		bFlat = true;
	}
	if(		(! bTransparent )
		&&	(! (bHover || bPushed) )
		&&	g_PaintManager->GetCb2DbTransparentMode(this)
		)
	{
		CExtPaintManager::stat_ExcludeChildAreas(
			dc,
			GetSafeHwnd(),
			CExtPaintManager::stat_DefExcludeChildAreaCallback
			);
		g_PaintManager->PaintDockerBkgnd( dc, this );
		bTransparent = true;
	}
CExtCmdIcon icon( hIcon, false );
CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
		this,
		true, rectClient, (LPCTSTR)sWindowText,
		&icon, bFlat, bHover, bPushed,
		false, bEnabled,
		(m_bDrawBorder && (!CExtPopupMenuWnd::IsMenuTracking()))
			? true : false,
		(bDrawFocusRect && (!CExtPopupMenuWnd::IsMenuTracking())
			&& (!sWindowText.IsEmpty()) )
			? true : false,
		bDefault, m_nAlign,
		NULL, _IsMenuAvail(), 0, bTransparent
		);
	_ppbd.m_bSeparatedDropDown = m_bSeparatedDropDown ? true : false;
	if( _ppbd.m_bSeparatedDropDown
		&& m_nTrackingHT == 2
		&& _ppbd.m_bPushed
		)
	{
		_ppbd.m_bPushed = false;
		_ppbd.m_bPushedDropDown = true;
//		_ppbd.m_bHover = false;
	}
	g_PaintManager->PaintPushButton( dc, _ppbd );
	icon.DetachAsHICON();

	if( hIcon != NULL && bDestroyIcon )
	{
		::DestroyIcon( hIcon );
	}
}

LRESULT CExtColorButton::OnColorChanged(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;

//	m_clrSelected = (COLORREF)wParam;
//	Invalidate();
//	UpdateWindow();
//CExtPopupMenuWnd * pPopup =
//		CExtPopupMenuWnd::GetTrackingMenu();
//	if( pPopup != NULL
//		&& pPopup->GetSafeHwnd() != NULL
//		)
//		pPopup->Invalidate();

	return
		GetParent()->SendMessage(
			CExtPopupColorMenuWnd::g_nMsgNotifyColorChanged,
			wParam,
			lParam
			);
}

LRESULT CExtColorButton::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	lParam;
COLORREF clr = (COLORREF)wParam;
	m_clrSelected = (clr == ((COLORREF)(-1))) ? RGB(0,0,0) : clr;
	Invalidate();
	UpdateWindow();

	return
		GetParent()->SendMessage(
			CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
			wParam,
			lParam
			);
}

LRESULT CExtColorButton::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	if( m_bUseStdColorDlg )
	{
		CColorDialog dlg(m_clrSelected,CC_FULLOPEN,this);
		if( dlg.DoModal() != IDOK )
			return 0;
		m_clrSelected = dlg.m_cc.rgbResult;
	} // if( m_bUseStdColorDlg )
	else
	{
		CExtColorDlg dlg( m_clrSelected,m_clrSelected,this );
		if( dlg.DoModal() != IDOK )
			return 0;
		m_clrSelected = dlg.m_clrNew;
	} // else from if( m_bUseStdColorDlg )
	GetParent()->SendMessage(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		WPARAM( m_clrSelected ),
		lParam
		);
	Invalidate();
	UpdateWindow();
	return 0;
}

LRESULT CExtButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == CExtToolControlBar::g_nMsgContinueTrackMenu )
	{
		_OnTrackPopup( lParam ? true : false );
		return (!0);
	}
	if(		message == WM_NOTIFY
		&&	m_wndToolTip.GetSafeHwnd() != NULL
		&&	IsWindow( m_wndToolTip.GetSafeHwnd() )
		&&	((LPNMHDR)lParam) != NULL
		&&	((LPNMHDR)lParam)->hwndFrom == m_wndToolTip.GetSafeHwnd()
		&&	((LPNMHDR)lParam)->code == TTN_SHOW
		)
		::SetWindowPos(
			m_wndToolTip.GetSafeHwnd(),
			HWND_TOP,
			0,0,0,0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE
			);
	
	if(
			message == CExtPopupMenuWnd::g_nMsgPrepareMenu
		||	message == CExtPopupMenuWnd::g_nMsgPopupNext
		||	message == CExtPopupMenuWnd::g_nMsgPopupPrev
		||	message == CExtPopupMenuWnd::g_nMsgNotifyMenuExpanded
		||	message == CExtPopupMenuWnd::g_nMsgNotifyMenuClosed
		||	message == CExtPopupMenuWnd::g_nMsgPopupDrawItem
		||	message == CExtPopupMenuWnd::g_nMsgPopupDrawLeftArea
		)
	{
		CWnd * pParent = GetParent();
		if( pParent )
			return pParent->SendMessage( message, wParam, lParam );
		else
			return 0;
	}

bool bInvalidate = false;
	switch( message )
	{
	case WM_MOUSEMOVE:
		if( CExtToolControlBar::g_bMenuTracking )
		{
			CWnd * pWndParent = GetParent();
			if(		pWndParent != NULL
				&&	pWndParent->IsKindOf(RUNTIME_CLASS(CExtToolControlBar))
				)
			{
				_OnTrackPopup( false );
				return 0;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
			return 0;
		//message = WM_LBUTTONDOWN;
	//break;
	case BM_SETCHECK:
	case BM_SETSTATE:
	case BM_SETSTYLE:
#if (WINVER >= 0x0400)
	case BM_CLICK:
	case BM_SETIMAGE:
#endif
	case WM_SETTEXT:
	case WM_SIZE:
	case WM_WINDOWPOSCHANGED:
		bInvalidate = true;
	break;
	} // switch( message )

	
LRESULT lResult =
		CButton::WindowProc( message, wParam, lParam );

//	if(
//			CExtPopupColorMenuWnd::g_nMsgNotifyColorChanged
//		||	CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally
//		||	CExtPopupColorMenuWnd::g_nMsgNotifyCustColor
//		)
//	{
//		CWnd * pParent = GetParent();
//		if( pParent )
//			pParent->SendMessage( message, wParam, lParam );
//	}

	if( bInvalidate )
	{
		Invalidate();
		UpdateWindow();
	}

	return lResult;
}

void CExtColorButton::_OnClick(
	bool bSelectAny,
	bool bSeparatedDropDownClicked
	)
{
	if(		(!m_bSeparatedDropDown)
		||	bSeparatedDropDownClicked
		)
	{
		if( _OnTrackPopup( bSelectAny ) )
			return;
	}

CWnd * pWnd = GetParent();
	ASSERT( pWnd != NULL );
	ASSERT_VALID( pWnd );
	pWnd->SendMessage(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		(WPARAM)m_clrSelected,
		m_lParamCookie
		);
}

