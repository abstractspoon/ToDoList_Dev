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

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#include <../src/mfc/AfxImpl.h>
#endif

#if (!defined __EXT_MENUCONTROLBAR_H)
	#include <ExtMenuControlBar.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if( !defined __EXTMINIDOCKFRAMEWND_H)
	#include "ExtMiniDockFrameWnd.h"
#endif

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
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

#define __MENUBAR_RIGHT_ADJUST_GAP 1

bool CExtMenuControlBar::g_bHideMenuBarExpandContentButton = false;
bool CExtMenuControlBar::g_bCancelNearestAltUp = false;
const CSize CExtMenuControlBar::g_sizeDefTBB( 23, 20 );

/////////////////////////////////////////////////////////////////////////////
// CExtMenuControlBar hook sink

bool CExtMenuControlBar::OnHookWndMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	UINT nMessage,
	WPARAM & wParam,
	LPARAM & lParam
	)
{
	if(		GetSafeHwnd() == NULL
		||	(! ::IsWindow(GetSafeHwnd()) )
		||	CWnd::FromHandlePermanent(GetSafeHwnd()) == NULL
		||	_DraggingGetBar() != NULL
		)
		return 
			CExtHookSink::OnHookWndMsg(
				lResult,
				hWndHooked,
				nMessage,
				wParam,
				lParam
				);

	if(		( (!m_bPresubclassDialogMode) && hWndHooked == _GetHwndMainFrame() )
		||	(   m_bPresubclassDialogMode  && hWndHooked == _GetHwndPlacement() )
		)
	{ // if window is a main frame or parent dialog-like
		if( nMessage == WM_PARENTNOTIFY
			&&
			CExtPopupMenuWnd::IsMenuTracking()
			)
		{
			lResult = 0;
			return true;
		}

		if( nMessage == WM_SIZE
			|| nMessage == WM_ACTIVATEAPP
			|| ( nMessage == WM_COMMAND && (((HIWORD(wParam))==0)||((HIWORD(wParam))==1)) )
			|| nMessage == WM_SYSCOMMAND
			)
		{
			HWND hWndCapture = CExtMouseCaptureSink::GetCapture();
			if( hWndCapture == GetSafeHwnd() )
				CExtMouseCaptureSink::ReleaseCapture();
			else if( hWndCapture != NULL )
			{
				CWnd * pWnd = FromHandlePermanent(hWndCapture);
				if( pWnd != NULL
					&& pWnd->GetSafeHwnd() != NULL
					&& ::IsWindow( pWnd->GetSafeHwnd() )
					&& pWnd->IsKindOf(RUNTIME_CLASS(CExtControlBar))
					)
					CExtMouseCaptureSink::ReleaseCapture();
			}

			if( m_bFlatTracking )
			{
				m_bFlatTracking = false;
				_UpdateFlatTracking();
			}
			if( CExtMenuControlBar::g_bMenuTracking )
				CExtMenuControlBar::_CloseTrackingMenus();
			//if(	CExtPopupMenuWnd::IsMenuTracking() )
			//	CExtPopupMenuWnd::CancelMenuTracking();
			return 
				CExtHookSink::OnHookWndMsg(
					lResult,
					hWndHooked,
					nMessage,
					wParam,
					lParam
					);
		}

		if( !m_bPresubclassDialogMode )
		{
			if( (nMessage == WM_NCLBUTTONUP && wParam == HTSYSMENU)
				|| (nMessage == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU))
				)
			{
				CExtToolControlBar::g_bMenuTrackingExpanded = false;
				CPoint * pPoint = NULL, ptCursor;
				if( nMessage == WM_NCRBUTTONUP )
				{
					if( ! ::GetCursorPos(&ptCursor) )
					{
						lResult = 0;
						return true;
					}
					pPoint = &ptCursor;
				}
				if( !_IsDockSiteCustomizeMode() )
				{
					TrackMainFrameSystemMenu(
						pPoint,
						FALSE
						);
				} // if( !_IsDockSiteCustomizeMode() )
				lResult = 0;
				return true;
			}
			if( (nMessage == WM_NCLBUTTONDOWN || nMessage == WM_NCRBUTTONDOWN)
				&& wParam == HTSYSMENU
				)
			{
				lResult = 0;
				return true;
			}
			if( nMessage == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
			{
				if( !_IsDockSiteCustomizeMode() )
				{
					CExtToolControlBar::g_bMenuTrackingExpanded = false;
					CPoint ptCursor;
					if( ::GetCursorPos(&ptCursor) )
						TrackMainFrameSystemMenu(
							&ptCursor,
							FALSE
							);
				} // if( !_IsDockSiteCustomizeMode() )
				lResult = 0;
				return true;
			}
		
			if( nMessage == CExtPopupMenuWnd::g_nMsgNotifyMenuClosed )
			{
				m_bSysMenuTracking = false;
				return true;
			}
		} // if( !m_bPresubclassDialogMode )

		bool bPrintPreviewMode = false;
		if(		(!m_bPresubclassDialogMode)
			&&	m_pDockSite->GetSafeHwnd() != NULL
			&&	FindPrintPreviewMode(m_pDockSite)
			)
			bPrintPreviewMode = true;

		if( nMessage == CExtPopupMenuWnd::g_nMsgPopupNext
			|| nMessage == CExtPopupMenuWnd::g_nMsgPopupPrev
			)
		{ // if menu next/prev popup activation messages
			BOOL bDummyMax = FALSE;
			HWND hWndMdiChild = _GetActiveMdiChildWnd(bDummyMax);

			int iTrackingIndex =
				GetMenuTrackingButton();
			if( (!bPrintPreviewMode) && iTrackingIndex < 0 ) // menu tracked on other bar
			{
				if( m_bSysMenuTracking )
				{
					if( nMessage == CExtPopupMenuWnd::g_nMsgPopupNext
						&& m_bMdiApp
						&& hWndMdiChild != NULL
						&& IsDisplayMdiDocumentButtons()
						)
					{
						ASSERT( !m_bPresubclassDialogMode );
						VERIFY(
							TrackChildFrameSystemMenu(
								NULL,
								TRUE
								)
							);
					} // if( m_bMdiApp && hWndMdiChild != NULL )
					else
					{
						int iNewTrackIndex =
							GetVisibleButton(
								iTrackingIndex,
								(nMessage == CExtPopupMenuWnd::g_nMsgPopupNext) ?
									TRUE : FALSE
								);
						if( iNewTrackIndex == iTrackingIndex )
						{
							lResult = 0; // same as tracking now
							return true;
						}
						TrackButtonMenu(iNewTrackIndex);
					} // else from if( m_bMdiApp && hWndMdiChild != NULL )
				} // if( m_bSysMenuTracking )
				lResult = 0;
				return true;
			}

			if( nMessage == CExtPopupMenuWnd::g_nMsgPopupNext
				&& ( !m_bPresubclassDialogMode )
				&& iTrackingIndex == GetVisibleButton(-1,FALSE)
				)
			{
				if( !_IsDockSiteCustomizeMode() )
					TrackMainFrameSystemMenu(
						NULL,
						TRUE
						);
				lResult = 0;
				return true;
			}

			if( nMessage == CExtPopupMenuWnd::g_nMsgPopupPrev
				&& ( !m_bPresubclassDialogMode )
				&& iTrackingIndex == 0
				&& IsDisplayMdiDocumentButtons()
				)
			{
				if( !_IsDockSiteCustomizeMode() )
				{
					if(		(! (m_bMdiApp && hWndMdiChild != NULL ) )
						||	bPrintPreviewMode
						||	(! TrackChildFrameSystemMenu(
								NULL,
								TRUE
								)
							)
						)
						TrackMainFrameSystemMenu(
							NULL,
							TRUE
							);
				} // if( !_IsDockSiteCustomizeMode() )
				lResult = 0;
				return true;
			}

			if(		nMessage == CExtPopupMenuWnd::g_nMsgPopupNext
				&&	(!m_bPresubclassDialogMode)
				&&	m_bMdiApp
				&&	m_bSysMenuTracking
				&&	iTrackingIndex < 0
				&&	hWndMdiChild != NULL
				&&	IsDisplayMdiDocumentButtons()
				&&	(!bPrintPreviewMode)
				)
			{
				if( !_IsDockSiteCustomizeMode() )
					TrackChildFrameSystemMenu(
						NULL,
						TRUE
						);
				lResult = 0;
				return true;
			}

			int iNewTrackIndex =
				GetVisibleButton(
					iTrackingIndex,
					(nMessage == CExtPopupMenuWnd::g_nMsgPopupNext) ?
						TRUE : FALSE
					);
			if( (!IsDisplayMdiDocumentButtons())
				&& nMessage == CExtPopupMenuWnd::g_nMsgPopupPrev
				&& iTrackingIndex == 0
				&& iNewTrackIndex > 0
				)
			{
				if( !_IsDockSiteCustomizeMode() )
					TrackMainFrameSystemMenu(
						NULL,
						TRUE
						);
				lResult = 0;
				return true;
			}

			if(	bPrintPreviewMode
				|| iNewTrackIndex == iTrackingIndex
				)
			{
				lResult = 0; // same as tracking now
				return true;
			}
			TrackButtonMenu(iNewTrackIndex);
			lResult = 0;
			return true;
		} // if menu next/prev popup activation messages

		if( nMessage == CExtPopupMenuWnd::g_nMsgNotifyMenuExpanded )
		{ // if menu rarely used items was expanded
			CExtToolControlBar::g_bMenuTrackingExpanded = true;
			lResult = 0;
			return true;
		} // if menu rarely used items was expanded

		if( !m_bPresubclassDialogMode )
			_KillFrameMenu();
	} // if window is a main frame or parent dialog-like
	else if( hWndHooked == _GetHwndChildFrame() )
	{

		if( nMessage == WM_PARENTNOTIFY
			&& CExtPopupMenuWnd::IsMenuTracking()
			)
		{
			lResult = 0;
			return true;
		}

		if(		nMessage == WM_SIZE
			||	nMessage == WM_WINDOWPOSCHANGED
			||	nMessage == WM_ACTIVATEAPP
			||	( nMessage == WM_COMMAND && (((HIWORD(wParam))==0)||((HIWORD(wParam))==1)) )
			||	nMessage == WM_SYSCOMMAND
			)
		{
			bool bSizePos = false;
			if( nMessage == WM_SIZE )
				bSizePos = true;
			else if( nMessage == WM_WINDOWPOSCHANGED )
			{
				LPWINDOWPOS lpWP = (LPWINDOWPOS)lParam;
				ASSERT( lpWP != NULL );
				if( (lpWP->flags & SWP_FRAMECHANGED) == 0 )
					bSizePos = true;
			} // else if( nMessage == WM_WINDOWPOSCHANGED )
			
			HWND hWndCapture = CExtMouseCaptureSink::GetCapture();
			if( hWndCapture == GetSafeHwnd() )
				CExtMouseCaptureSink::ReleaseCapture();
			else if( hWndCapture != NULL )
			{
				CWnd * pWnd = FromHandlePermanent(hWndCapture);
				if( pWnd != NULL
					&& pWnd->GetSafeHwnd() != NULL
					&& ::IsWindow( pWnd->GetSafeHwnd() )
					&& pWnd->IsKindOf(RUNTIME_CLASS(CExtControlBar))
					)
					CExtMouseCaptureSink::ReleaseCapture();
			}

			if( m_bFlatTracking )
			{
				m_bFlatTracking = false;
				_UpdateFlatTracking();
			}
			if( CExtMenuControlBar::g_bMenuTracking )
				CExtMenuControlBar::_CloseTrackingMenus();
			if(	CExtPopupMenuWnd::IsMenuTracking() )
				CExtPopupMenuWnd::CancelMenuTracking();
			
			if( !bSizePos )
			{
				_KillFrameMenu();

				if(		(	nMessage == WM_SIZE
						||	nMessage == WM_WINDOWPOSCHANGED
						)
					&&	(! _IsDockSiteCustomizeMode() )
					)
				{
					WINDOWPLACEMENT _wp;
					::memset( &_wp, 0, sizeof(WINDOWPLACEMENT) );
					::GetWindowPlacement( _GetHwndChildFrame(), &_wp );
					if(		_wp.showCmd != SW_SHOWMAXIMIZED
						||	(	_GetButtonsCountImpl() > 0
							&&	(! _GetButtonPtr(0)->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
							&&	( IsDisplayMdiDocumentButtons() )
							) // MDI doc buttons not installed, but should be displayed
						)
						_DelayUpdateMenuBar();
				}
			} // if( !bSizePos )
			
			return 
				CExtHookSink::OnHookWndMsg(
					lResult,
					hWndHooked,
					nMessage,
					wParam,
					lParam
					);
		}

		bool bPrintPreviewMode = false;
		if(		(!m_bPresubclassDialogMode)
			&&	m_pDockSite->GetSafeHwnd() != NULL
			&&	FindPrintPreviewMode(m_pDockSite)
			)
			bPrintPreviewMode = true;

		if( bPrintPreviewMode )
		{
			lResult = 0;
			return true;
		}

		bool bMinFrameNcLbDown = false;
		bool bMinFrameNcLbUp = false;
		if(		nMessage == WM_NCLBUTTONDOWN
			||	nMessage == WM_NCLBUTTONUP
			)
		{
			WINDOWPLACEMENT wp;
			::memset( &wp, 0, sizeof(WINDOWPLACEMENT) );
			::GetWindowPlacement( hWndHooked, &wp );
			if( wp.showCmd == SW_SHOWMINIMIZED )
			{
				if( nMessage == WM_NCLBUTTONDOWN )
					bMinFrameNcLbDown = true;
				else if( nMessage == WM_NCLBUTTONUP )
					bMinFrameNcLbUp = true;
			}
		}

		if( bMinFrameNcLbUp && wParam == HTCAPTION )
			return true;

		if(		( bMinFrameNcLbDown && wParam == HTCAPTION )
			||	(nMessage == WM_NCLBUTTONUP && wParam == HTSYSMENU)
			||	(nMessage == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU))
			)
		{
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			CPoint * pPoint = NULL, ptCursor;
			if( nMessage == WM_NCRBUTTONUP )
			{
				if( ! ::GetCursorPos(&ptCursor) )
				{
					lResult = 0;
					return true;
				}
				pPoint = &ptCursor;
			}
			if( !_IsDockSiteCustomizeMode() )
				TrackChildFrameSystemMenu(
					pPoint,
					FALSE
					);
			lResult = 0;
			return true;
		}
		if( (nMessage == WM_NCLBUTTONDOWN || nMessage == WM_NCRBUTTONDOWN)
			&& wParam == HTSYSMENU
			)
		{
			lResult = 0;
			return true;
		}
		if( nMessage == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
		{
			if( !_IsDockSiteCustomizeMode() )
			{
				CExtToolControlBar::g_bMenuTrackingExpanded = false;
				CPoint ptCursor;
				if( ::GetCursorPos(&ptCursor) )
					TrackChildFrameSystemMenu(
						&ptCursor,
						FALSE
						);
			} // if( !_IsDockSiteCustomizeMode() )
			lResult = 0;
			return true;
		}

		if( nMessage == CExtPopupMenuWnd::g_nMsgNotifyMenuClosed )
		{
			m_bSysMenuTracking = false;
			return true;
		}

		if( nMessage == CExtPopupMenuWnd::g_nMsgPopupNext
			|| nMessage == CExtPopupMenuWnd::g_nMsgPopupPrev
			)
		{ // if menu next/prev popup activation messages
			if( nMessage == CExtPopupMenuWnd::g_nMsgPopupPrev )
			{
				if( !_IsDockSiteCustomizeMode() )
					TrackMainFrameSystemMenu(
						NULL,
						TRUE
						);
				lResult = 0;
				return true;
			}
			int iTrackingIndex =
				GetMenuTrackingButton();
			if( iTrackingIndex < 0 ) // menu tracked on other bar
			{
				if( m_bSysMenuTracking )
				{
					int iNewTrackIndex =
						GetVisibleButton(
							iTrackingIndex,
							(nMessage == CExtPopupMenuWnd::g_nMsgPopupNext) ?
								TRUE : FALSE
							);
					if( iNewTrackIndex == iTrackingIndex )
					{
						lResult = 0; // same as tracking now
						return true;
					}
					TrackButtonMenu(iNewTrackIndex);
				} // if( m_bSysMenuTracking )
				lResult = 0;
				return true;
			}

			int iNewTrackIndex =
				GetVisibleButton(
					iTrackingIndex,
					(nMessage == CExtPopupMenuWnd::g_nMsgPopupNext) ?
						TRUE : FALSE
					);
			if( iNewTrackIndex == iTrackingIndex )
			{
				lResult = 0; // same as tracking now
				return true;
			}
			TrackButtonMenu(iNewTrackIndex);
			lResult = 0;
			return true;
		} // if menu next/prev popup activation messages

	} // else if( hWndHooked == _GetHwndChildFrame() )
	else if( hWndHooked == _GetHwndMdiArea() )
	{
		if( nMessage == WM_PARENTNOTIFY
			&&
			CExtPopupMenuWnd::IsMenuTracking()
			)
		{
			lResult = 0;
			return true;
		}

		if( nMessage == WM_MDISETMENU )
		{
			CFrameWnd * pFrame = _GetDockingFrameImpl();
			ASSERT_VALID( pFrame );
			HWND hWndFrame = pFrame->GetSafeHwnd();
			ASSERT( hWndFrame != NULL );
			ASSERT( ::IsWindow(hWndFrame) );

			bool bOle = IsOleIpObjActive();
			
			HMENU hOldMdiMenu = m_menuDoc.Detach();
			HMENU hNewMdiMenu = (HMENU)wParam;
			m_menuDoc.Attach( hNewMdiMenu );

			if( bOle )
			{
				::SetMenu( hWndFrame, hNewMdiMenu );
			} // if( bOle )
			else
			{
				HMENU hFrameRealMenu = ::GetMenu( hWndFrame );
				if( hFrameRealMenu != NULL )
					::SetMenu( hWndFrame, NULL );
				_DelayUpdateMenuBar();
			} // else from if( bOle )
			CExtHookSink::OnHookWndMsgNextProcCurrent( 0L, 0L );
			lResult = (LRESULT)hOldMdiMenu;
			//_DelaySyncActiveMdiChild(); // (- v.2.23)
			_SyncActiveMdiChild(); // (+ v.2.23)
			m_bDelayedSyncActiveMdiChild = false; // (+ v.2.23)
			return true;
		}
		if( nMessage == WM_MDIREFRESHMENU )
		{
			CExtHookSink::OnHookWndMsgNextProcCurrent( 0L, 0L );
			//_DelaySyncActiveMdiChild(); // (- v.2.23)
			_SyncActiveMdiChild(); // (+ v.2.23)
			m_bDelayedSyncActiveMdiChild = false; // (+ v.2.23)
			lResult = 0;
			return true;
		}

		if( nMessage == WM_CONTEXTMENU )
		{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
			CExtCustomizeSite * pSite =
				CExtCustomizeSite::GetCustomizeSite( m_hWnd );
			if(		pSite != NULL
				&&	pSite->IsCustomizeMode()
				)
			{
				lResult = 0;
				return true;
			}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
			if( !CExtControlBar::FindHelpMode(this) )
			{
				CFrameWnd * pFrame = _GetDockingFrameImpl();
				ASSERT_VALID( pFrame );

				CPoint ptCursor;
				if( ! ::GetCursorPos(&ptCursor) )
				{
					lResult = 0;
					return true;
				}
				HWND hWndTrack = GetOwner()->GetSafeHwnd();
				ASSERT( hWndTrack != NULL && ::IsWindow(hWndTrack) );
				CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
				VERIFY( pPopup->CreatePopupMenu(hWndTrack) );
				if(	! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
						pFrame,
						pPopup,
						CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_MDICLIAREA_CTX,
						this,
						WM_CONTEXTMENU,
						NULL
						)
					)
				{
					delete pPopup;
				}
				else
				{
					if( pPopup->ItemGetCount() == 0 )
					{
						delete pPopup;
					}
					else
					{
						::SetFocus( hWndTrack );
						if( ! pPopup->TrackPopupMenu(
								TPMX_OWNERDRAW_FIXED,
								ptCursor.x,
								ptCursor.y
								)
							)
						{
							delete pPopup;
						}
					}
				}
			} // if( !CExtControlBar::FindHelpMode(this) )
			lResult = 0;
			return true;
		} // if( nMessage == WM_CONTEXTMENU )

		if(		nMessage == WM_MDIACTIVATE
			||	nMessage == WM_MDIDESTROY
			||	nMessage == WM_MDINEXT
			)
			_DelaySyncActiveMdiChild();
	} // else if( hWndHooked == _GetHwndMdiArea() )
	else
	{
		ASSERT( FALSE );
	}

	MSG msg;
	::memset( &msg, 0, sizeof(MSG) );
	msg.hwnd = hWndHooked;
	msg.message = nMessage;
	msg.wParam = wParam;
	msg.lParam = lParam;
	if( TranslateMainFrameMessage(&msg) )
		return true;

	return 
		CExtHookSink::OnHookWndMsg(
			lResult,
			hWndHooked,
			nMessage,
			wParam,
			lParam
			);
}

/////////////////////////////////////////////////////////////////////////////
// CExtMenuControlBar

IMPLEMENT_DYNCREATE(CExtMenuControlBar, CExtToolControlBar)

CExtMenuControlBar::CExtMenuControlBar()
{
	m_bFlatTracking = false;
	m_bSysMenuTracking = false;
	m_nFlatTrackingIndex = m_nOldTrackingIndex = -1;
	m_bMdiApp = false;

	m_bMultiRowLayout = true;

	m_sMdiWindowPopupName = _T("");
	m_nMdiDocButtonCmdID = 0;
	m_hWndHelper = NULL;

	m_bDelayedSyncActiveMdiChild = false;
	m_bDelayedUpdateMenuBar = false;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_pMenuInfo = NULL;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
}

CExtMenuControlBar::~CExtMenuControlBar()
{
	_RemoveAllButtonsImpl();
CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
int nCountOfBars = g_AllBars.GetSize();
	for( int i=0; i<nCountOfBars; i++ )
	{
		CExtControlBar * pBar = g_AllBars[i];
		ASSERT( pBar != NULL );
		if( pBar == this )
		{
			g_AllBars.RemoveAt( i );
			break;
		}
	}
	_slCsCB.Unlock();
}

BEGIN_MESSAGE_MAP(CExtMenuControlBar, CExtToolControlBar)
    //{{AFX_MSG_MAP(CExtMenuControlBar)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CExtToolControlBar::g_nMsgTrackButtonMenu, OnTrackButtonMenu)
END_MESSAGE_MAP()

// LoadMenuBar() required only for CFrameWnd
// based windows principally without menu
BOOL CExtMenuControlBar::LoadMenuBar(
	UINT nResourceID
	)
{
	if( m_menuDoc.GetSafeHmenu() != NULL )
	{
		VERIFY( m_menuDoc.DestroyMenu() );
	}
	if( m_menuFrame.GetSafeHmenu() != NULL )
	{
		VERIFY( m_menuFrame.DestroyMenu() );
	}
	
	if( m_pDockSite != NULL )
	{
		if( GetSafeHwnd() == NULL
			|| !::IsWindow( GetSafeHwnd() )
			|| m_menuFrame.GetSafeHmenu() != NULL
			|| !IsHookedWindow( _GetHwndMainFrame() )
			)
		{
			ASSERT( FALSE );
			return FALSE;
		}
	}
	
	if( !m_menuFrame.LoadMenu( nResourceID ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( ::IsMenu(m_menuFrame.GetSafeHmenu()) );
	if( !g_CmdManager->UpdateFromMenu(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			m_menuFrame.GetSafeHmenu()
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}

	return _UpdateMenuBar();
}

CMenu * CExtMenuControlBar::_GetMenu()
{
	if( m_menuDoc.GetSafeHmenu() != NULL )
	{
		ASSERT( ::IsMenu(m_menuDoc.GetSafeHmenu()) );
		return &m_menuDoc;
	}
	if( m_menuFrame.GetSafeHmenu() != NULL )
	{
		ASSERT( ::IsMenu(m_menuFrame.GetSafeHmenu()) );
		return &m_menuFrame;
	}
	return NULL;
}

CMenu * CExtMenuControlBar::GetMenu()
{
	return _GetMenu();
}

BOOL CExtMenuControlBar::UpdateMenuBar( // update after menu changed
	BOOL bDoRecalcLayout // = TRUE
	)
{
	return _UpdateMenuBar( bDoRecalcLayout );
}

BOOL CExtMenuControlBar::_UpdateMenuBar(
	BOOL bDoRecalcLayout // = TRUE
	)
{
	SetButtons(); // remove all buttons

	VERIFY(
		g_CmdManager->CmdRemoveByMask(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			(DWORD)CExtCmdItem::STATE_MENUBAR_TMP
			)
		);

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	MenuInfoUpdate();
CExtCustomizeSite::CCmdMenuInfo * pMenuInfo = MenuInfoGet();
	if( pMenuInfo != NULL )
	{
		ASSERT_VALID( pMenuInfo->GetNode() );
		SetButtons( pMenuInfo->GetNode() );
	} // if( pMenuInfo != NULL )
	else
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	{
		CMenu * pMenu = GetMenu();
		if( pMenu != NULL && pMenu->GetSafeHmenu() != NULL )
		{
			ASSERT( ::IsMenu(pMenu->GetSafeHmenu()) );

			UINT nMenuItemCount = pMenu->GetMenuItemCount();
			for( UINT iMenu=0; iMenu<nMenuItemCount; iMenu++ )
			{
				MENUITEMINFO mii;
				::memset(&mii,0,sizeof(MENUITEMINFO));
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask =
					MIIM_CHECKMARKS
					|MIIM_DATA
					|MIIM_ID
					|MIIM_STATE
					|MIIM_SUBMENU
					|MIIM_TYPE
					;
				mii.cch = __MAX_UI_ITEM_TEXT;
				CExtSafeString sText;
				mii.dwTypeData =
					sText.GetBuffer(__MAX_UI_ITEM_TEXT);
				ASSERT( mii.dwTypeData != NULL );
				if( mii.dwTypeData == NULL )
				{
					ASSERT( FALSE );
					return FALSE;
				}
				if( !pMenu->GetMenuItemInfo(
						iMenu,
						&mii,
						TRUE
						)
					)
				{
					sText.ReleaseBuffer();
					ASSERT( FALSE );
					return false;
				}
				sText.ReleaseBuffer();

				BOOL bAppendMdiWindowsMenu = FALSE;
				UINT nCmdID = 0;
				CExtCmdItem * pCmdItem = NULL;
				if( mii.hSubMenu == NULL )
				{
					nCmdID = mii.wID;
					if( nCmdID == ID_SEPARATOR )
					{
						if( !InsertButton(
								iMenu,
								nCmdID,
								FALSE
								)
							)
						{
							ASSERT( FALSE );
							return FALSE;
						}
						continue;
					}
					ASSERT( CExtCmdManager::IsCommand(nCmdID) );
					pCmdItem =
						g_CmdManager->CmdGetPtr(
							g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
							nCmdID
							);
					ASSERT( pCmdItem != NULL );
				} // if( mii.hSubMenu == NULL )
				else
				{
					pCmdItem =
						g_CmdManager->CmdAllocPtr(
							g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() )
							);
					if( pCmdItem == NULL )
					{
						ASSERT( FALSE );
						return FALSE;
					}
					nCmdID = pCmdItem->m_nCmdID;
					ASSERT( CExtCmdManager::IsCommand(nCmdID) );
					pCmdItem->StateSetMenubarTemp();
					pCmdItem->StateSetBasic();

					if( m_bMdiApp && !m_sMdiWindowPopupName.IsEmpty() )
					{
						CExtSafeString _sText(sText);
						_sText.TrimLeft();
						_sText.TrimRight();
						while( _sText.Replace(_T("&"),_T("")) > 0 )
						{
							_sText.TrimLeft();
							_sText.TrimRight();
						}
						if( _sText == m_sMdiWindowPopupName )
							bAppendMdiWindowsMenu = TRUE;
					} // if( m_bMdiApp && !m_sMdiWindowPopupName.IsEmpty() )
				} // else from if( mii.hSubMenu == NULL )
				ASSERT( pCmdItem != NULL );
				if( pCmdItem->m_sToolbarText.IsEmpty() )
					pCmdItem->m_sToolbarText = sText;
				if( pCmdItem->m_sMenuText.IsEmpty() )
					pCmdItem->m_sMenuText = sText;

				if( !InsertButton(
						iMenu,
						nCmdID,
						FALSE
						)
					)
				{
					ASSERT( FALSE );
					return FALSE;
				}

				if( mii.hSubMenu != NULL )
				{
					ASSERT( ::IsMenu(mii.hSubMenu) );
					SetButtonMenu(
						iMenu,
						mii.hSubMenu,
						FALSE,
						FALSE,
						FALSE
						);
				}
				if( bAppendMdiWindowsMenu )
				{
					VERIFY(
						MarkButtonAsMdiWindowsMenu(
							iMenu,
							TRUE
							)
						);
				}

			} // for( UINT iMenu=0; iMenu<nMenuItemCount; iMenu++ )

			ASSERT( m_pRightBtn == NULL );
			m_pRightBtn = OnCreateBarRightBtn();
			if( m_pRightBtn != NULL )
			{
				ASSERT_VALID( m_pRightBtn );
				ASSERT_KINDOF( CExtBarContentExpandButton, m_pRightBtn );
				m_buttons.Add( m_pRightBtn );
			} // if( m_pRightBtn != NULL )
		} // if( pMenu != NULL && pMenu->GetSafeHmenu() != NULL )
	} // else from if( pMenuInfo != NULL )

	if( m_bMdiApp )
	{
		if( !IsOleIpObjActive() )
			if( _InstallMdiDocButtons( FALSE ) )
				bDoRecalcLayout = TRUE;
		VERIFY( _SyncActiveMdiChild() );
	}
	
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
	return TRUE;
}

HWND CExtMenuControlBar::_GetActiveMdiChildWnd(
	BOOL & bMaximized
	)
{
	bMaximized = FALSE;
	if( !m_bMdiApp )
		return NULL;

CMDIFrameWnd * pFrame =
		DYNAMIC_DOWNCAST(
			CMDIFrameWnd,
			_GetDockingFrameImpl()
			);
	ASSERT_VALID( pFrame );
HWND hWndMdiArea = pFrame->m_hWndMDIClient;
	ASSERT( hWndMdiArea != NULL );
	ASSERT( ::IsWindow(hWndMdiArea) );
BOOL bMax = FALSE;
HWND hWnd = (HWND)
		::SendMessage(
			hWndMdiArea,
			WM_MDIGETACTIVE,
			0,
			(LPARAM)&bMax
			);
	ASSERT(
		hWnd == NULL
		|| ::IsWindow( hWnd )
		);
	bMaximized = bMax;
	return hWnd;
}

void CExtMenuControlBar::OnUpdateCmdUI(
	CFrameWnd * pTarget,
	BOOL bDisableIfNoHndler
	)
{
	CExtToolControlBar::OnUpdateCmdUI(
		pTarget,
		bDisableIfNoHndler
		);
}

BOOL CExtMenuControlBar::_SyncActiveMdiChild()
{
	if( m_bPresubclassDialogMode )
		return TRUE;
	if( _DraggingGetBar() != NULL )
		return TRUE;

	if( (GetStyle() & WS_VISIBLE) == 0 )
		return TRUE;

BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( m_hWndHelper == hWndActiveChild )
		return TRUE;
	if( m_hWndHelper != NULL )
		SetupHookWndSink( m_hWndHelper, true );
	m_hWndHelper = hWndActiveChild;
	if( m_hWndHelper != NULL )
		SetupHookWndSink( m_hWndHelper );
	_RecalcLayoutImpl();
	return TRUE;
}

LRESULT CExtMenuControlBar::DoHelpHitTest( CPoint ptHelpHitTest )
{
	ASSERT_VALID( this );
	return CExtToolControlBar::DoHelpHitTest( ptHelpHitTest );
}

bool CExtMenuControlBar::IsDisplayMdiDocumentButtons() const
{
	return true;
}

BOOL CExtMenuControlBar::_InstallMdiDocButtons(
	BOOL bDoRecalcLayout // = TRUE
	)
{
	ASSERT( m_bMdiApp );

	if( !IsDisplayMdiDocumentButtons() )
	{
		if( bDoRecalcLayout )
			_RecalcLayoutImpl();
		return TRUE;
	}

	if( _DraggingGetBar() != NULL )
		return FALSE;

BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
int nCountOfButtons = GetButtonsCount();
	if( nCountOfButtons == 0
		&& hWndActiveChild == NULL
		)
		return FALSE;
CExtBarMdiDocButton * pDocTBB = NULL;
	if( nCountOfButtons > 0 )
		pDocTBB = 
			DYNAMIC_DOWNCAST(
				CExtBarMdiDocButton,
				_GetButtonPtr(0)
				);
int nRightIndex = nCountOfButtons - 1;
CExtBarMdiRightButton * pRightTBB = NULL;
	if( nCountOfButtons > 0 )
		pRightTBB =
			DYNAMIC_DOWNCAST(
				CExtBarMdiRightButton,
				_GetButtonPtr(nRightIndex)
				);
	if( pRightTBB == NULL
		&& nCountOfButtons > 1
		)
	{
		if( !m_bMultiRowLayout )
			nRightIndex--;
		pRightTBB =
			DYNAMIC_DOWNCAST(
				CExtBarMdiRightButton,
				_GetButtonPtr(nRightIndex)
				);
	}
BOOL bRetVal = FALSE;
	if( hWndActiveChild == NULL || (!bMax) )
	{ // if no active MDI doc or not maximized
		if( pRightTBB != NULL )
		{
			ASSERT_VALID( pRightTBB );
			VERIFY( RemoveButton(nRightIndex,FALSE) );
			bRetVal = TRUE;
			pRightTBB = NULL;
		}
		if( pDocTBB != NULL )
		{
			ASSERT_VALID( pDocTBB );
			VERIFY( RemoveButton(0,FALSE) );
			bRetVal = TRUE;
			pDocTBB = NULL;
		}
	} // if no active MDI doc or not maximized
	else
	{ // if acdive MDI doc is maximized
		ASSERT( hWndActiveChild != NULL );
		ASSERT( ::IsWindow(hWndActiveChild) );
		if( m_nMdiDocButtonCmdID == 0 )
		{
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdAllocPtr(
					g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() )
					);
			ASSERT( pCmdItem != NULL );
			if( pCmdItem == NULL )
				return FALSE;
			m_nMdiDocButtonCmdID = pCmdItem->m_nCmdID;
			ASSERT(
				CExtCmdManager::IsCommand(
					m_nMdiDocButtonCmdID
					)
				);
			//pCmdItem->StateSetMenubarTemp();
			pCmdItem->StateSetBasic();
		} // if( m_nMdiDocButtonCmdID == 0 )
		if( pRightTBB == NULL )
		{
			pRightTBB =
				new CExtBarMdiRightButton(
					this,
					m_nMdiDocButtonCmdID
					);
			ASSERT_VALID( pRightTBB );
			m_buttons.InsertAt( nRightIndex + 1, pRightTBB );
			bRetVal = TRUE;
		} // if( pRightTBB == NULL )
		if( pDocTBB == NULL )
		{
			HMENU hDocMenu =
				::GetSystemMenu(hWndActiveChild,FALSE);
			if(		hDocMenu != NULL
				&&	::IsMenu(hDocMenu)
				)
			{
				pDocTBB =
					new CExtBarMdiDocButton(
						this,
						m_nMdiDocButtonCmdID
						);
				ASSERT_VALID( pDocTBB );
				pDocTBB->SetMenu( hDocMenu, FALSE, FALSE );
				m_buttons.InsertAt( 0, pDocTBB );
				bRetVal = TRUE;
			}
		} // if( pDocTBB == NULL )
	} // if acdive MDI doc is maximized

	if( bDoRecalcLayout )
	{
		_RecalcLayoutImpl();
		bRetVal = FALSE;
	}
	return bRetVal;
}

void CExtMenuControlBar::OnDestroy() 
{
	if( m_nMdiDocButtonCmdID != 0 )
	{
		VERIFY(
			g_CmdManager->CmdRemove(
				g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
				m_nMdiDocButtonCmdID
				)
			);
		m_nMdiDocButtonCmdID = 0;
	}
	RemoveAllWndHooks();
	CExtToolControlBar::OnDestroy();
}

BOOL CExtMenuControlBar::TranslateMainFrameMessage(MSG* pMsg) 
{
	if( GetSafeHwnd() == NULL
		|| (!::IsWindow(GetSafeHwnd()))
		)
		return  FALSE;
	ASSERT_VALID(this);
	ASSERT(pMsg);

	if(		pMsg->message == WM_ACTIVATE
		||	pMsg->message == WM_ACTIVATEAPP
		||	pMsg->message == WM_SETFOCUS
		||	pMsg->message == WM_KILLFOCUS
		)
	{
		g_bCancelNearestAltUp = true;
		return FALSE;
	} // if( pMsg->message == WM_ACTIVATE ...

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite = GetCustomizeSite();
	if(		pSite != NULL
		&&	pSite->GetCustomizeDropSource() != NULL
		)
		return FALSE;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

bool bDockSiteCustomizeMode =
		_IsDockSiteCustomizeMode();

	if( m_bMdiApp )
	{
		if(		(	pMsg->message == WM_COMMAND
				&&	(	(LOWORD(pMsg->wParam)) == ID_NEXT_PANE
					||	(LOWORD(pMsg->wParam)) == ID_PREV_PANE
					||	(LOWORD(pMsg->wParam)) == ID_WINDOW_NEW
					||	(LOWORD(pMsg->wParam)) == ID_WINDOW_ARRANGE
					||	(LOWORD(pMsg->wParam)) == ID_WINDOW_CASCADE
					||	(LOWORD(pMsg->wParam)) == ID_WINDOW_TILE_HORZ
					||	(LOWORD(pMsg->wParam)) == ID_WINDOW_TILE_VERT
					||	(LOWORD(pMsg->wParam)) == ID_WINDOW_SPLIT
					)
				)
			||	pMsg->message == WM_MDIACTIVATE
			||	pMsg->message == WM_MDINEXT
			||	pMsg->message == WM_MDIRESTORE
			||	pMsg->message == WM_MDIMAXIMIZE
			||	pMsg->message == WM_MDITILE
			||	pMsg->message == WM_MDICASCADE
			||	pMsg->message == WM_MDIICONARRANGE
			)
		{
			if( bDockSiteCustomizeMode )
				return TRUE;
			if( IsOleIpObjActive() )
				return TRUE;
		}
	} // if( m_bMdiApp )
	

bool bPrintPreviewMode = false;
	if(		(!m_bPresubclassDialogMode)
		&&	(!bDockSiteCustomizeMode)
		&&	m_pDockSite->GetSafeHwnd() != NULL
		&&	FindPrintPreviewMode(m_pDockSite)
		)
		bPrintPreviewMode = true;
bool bComboDropDownTracking = false;
CWnd * pWndCurrFocus = CWnd::GetFocus();
	if( pWndCurrFocus != NULL )
	{
		if( pWndCurrFocus->IsKindOf(RUNTIME_CLASS(CComboBox)) )
		{
			if(		(pWndCurrFocus->GetStyle() & (CBS_DROPDOWN|CBS_DROPDOWNLIST)) != 0
				&&	((CComboBox *)pWndCurrFocus)->GetDroppedState()
				)
				bComboDropDownTracking = true;
		}
		else
		{
			pWndCurrFocus = pWndCurrFocus->GetParent();
			if( pWndCurrFocus != NULL )
			{
				if( pWndCurrFocus->IsKindOf(RUNTIME_CLASS(CComboBox)) )
				{
					if(		(pWndCurrFocus->GetStyle() & (CBS_DROPDOWN|CBS_DROPDOWNLIST)) != 0
						&&	((CComboBox *)pWndCurrFocus)->GetDroppedState()
						)
						bComboDropDownTracking = true;
				}
			}
		}
	}

UINT nMsg = pMsg->message;

	if(		nMsg == WM_COMMAND
		&&	HIWORD( pMsg->wParam ) == 0
		) // (+ v.2.23)
	{
		bool bAlt = ( (::GetAsyncKeyState(VK_MENU)&0x8000) != 0 ) ? true : false;
		g_bCancelNearestAltUp = bAlt;
	}

	if(	   nMsg == WM_SYSKEYDOWN
		|| nMsg == WM_SYSKEYUP
		|| nMsg == WM_KEYDOWN
		|| nMsg == WM_KEYUP
		)
	{
		BOOL bKeyUp =
			(	nMsg == WM_KEYUP ||
				nMsg == WM_SYSKEYUP) ?
					TRUE : FALSE;
		BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x80000000;
		BOOL bShift = GetKeyState(VK_SHIFT) & 0x80000000;
		BOOL bAlt = HIWORD(pMsg->lParam) & KF_ALTDOWN;
		TCHAR vkTCHAR = (TCHAR)pMsg->wParam;

		if( vkTCHAR == VK_MENU && (!bCtrl) && (!bShift) )
		{ // only VK_MENU pressed
			if( bDockSiteCustomizeMode )
				return TRUE;
			if(		bPrintPreviewMode
				||	bComboDropDownTracking
				)
				return FALSE;

			if( m_bFlatTracking ) // (+ v.2.23)
			{
				if( !bKeyUp )
					return TRUE;
				m_bFlatTracking = false;
				_UpdateFlatTracking();
				return TRUE;
			} // if( m_bFlatTracking )
			
			if( !bKeyUp )
			{
				g_bCancelNearestAltUp = false;
				return FALSE; // (+ v.2.23)
			}
			if(		g_bCancelNearestAltUp
				||	g_bMenuTracking
				||	GetButtonsCount() == 0
				)
			{
				if( g_bCancelNearestAltUp ) // (+ v.2.23)
					return FALSE;
				return TRUE;
			}
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			TrackButtonMenu( 0 );
			return TRUE;
		} // only VK_MENU pressed

		if(		(! m_bPresubclassDialogMode )
			&&	bAlt
			&&	(	 vkTCHAR == VK_SPACE
				||	(	m_bMdiApp // (+ v.2.23)
					&&	(	vkTCHAR == VK_SUBTRACT
// disable warning C4310: cast truncates constant value
#pragma warning( push )
#pragma warning( disable : 4310 )
						||	vkTCHAR == ((TCHAR)0xbd) // '-' near '0'
// rollback warning C4310: cast truncates constant value
#pragma warning( pop )
						)
					)
				)
			)
		{
			if( bDockSiteCustomizeMode )
				return TRUE;
			if( bKeyUp )
				return TRUE;
			// allow child popup track it's system menu
			CWnd * pWnd = CWnd::GetActiveWindow();
			if( pWnd == NULL
				|| (!(::IsWindow(pWnd->GetSafeHwnd())))
				)
				return TRUE;
			if( pWnd->GetSafeHwnd() != _GetDockingFrameImpl()->GetSafeHwnd()
				&& (pWnd->GetStyle() & WS_POPUP) != 0
				)
				return TRUE;

			// track frame system menu
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			if( !_IsDockSiteCustomizeMode() )
			{
				if( m_bFlatTracking ) // (+ v.2.23)
				{
					m_bFlatTracking = false;
					_UpdateFlatTracking();
				}
				if( vkTCHAR == VK_SPACE )
				{
					TrackMainFrameSystemMenu( NULL, TRUE );
				} // if( vkTCHAR == VK_SPACE )
				else
				{
					ASSERT( m_bMdiApp ); // (+ v.2.23)
					if(	   nMsg == WM_KEYDOWN
						|| nMsg == WM_KEYUP
						)
						return TRUE;
					BOOL bMax = FALSE;
					HWND hWndActiveChild = _GetActiveMdiChildWnd( bMax );
					if( hWndActiveChild == NULL )
					{
						if( !bKeyUp )
							g_bCancelNearestAltUp = true;
						return TRUE;
					}
					bool bExplicitTrackChildFrameSystemMenu = true;
					if( GetButtonsCount() > 0 )
					{
						CExtBarButton * pTBB = _GetButtonPtr( 0 );
						ASSERT_VALID( pTBB );
						if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
						{
							VERIFY( CExtToolControlBar::TrackButtonMenu( 0 ) );
							bExplicitTrackChildFrameSystemMenu = false;
						} // if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
					} // if( GetButtonsCount() > 0 )
					if( bExplicitTrackChildFrameSystemMenu )
						TrackChildFrameSystemMenu( NULL, TRUE );
				} // else from if( vkTCHAR == VK_SPACE )
			}
			return TRUE;
		} // if( ( !m_bPresubclassDialogMode ) && bAlt && vkTCHAR == VK_SPACE )

		if(		( bPrintPreviewMode || bComboDropDownTracking )
			&&	(! bDockSiteCustomizeMode )
			)
			return FALSE;

		if( bAlt && (!bDockSiteCustomizeMode) )
		{ // if VK_ALT + VK_... pressed
//			if( g_bCancelNearestAltUp ) // (+ v.2.23)
//				return FALSE;
			if( bKeyUp )
			{
				return TRUE;
			}
			if( m_bFlatTracking )
			{
				m_bFlatTracking = false;
				_UpdateFlatTracking();
			}
			BYTE lpKeyState[256];
			::GetKeyboardState( lpKeyState );
			UINT wScanCode = ::MapVirtualKey( vkTCHAR, 0 );
			HKL hKeyboardLayout =
				::GetKeyboardLayout(
					( ::AfxGetThread() ) -> m_nThreadID
					);
#if (defined _UNICODE)
			TCHAR szChar[2] = { _T('\0'), _T('\0') };
			::ToUnicodeEx(
				vkTCHAR,
				wScanCode,
				lpKeyState,
				szChar, 1,
				1,
				hKeyboardLayout
				);
#else
			WORD nMapped = 0;
			::ToAsciiEx(
				vkTCHAR,
				wScanCode,
				lpKeyState,
				&nMapped,
				1,
				hKeyboardLayout
				);
			TCHAR szChar[2] = { (TCHAR)nMapped, _T('\0') };
#endif
			::CharUpper( szChar );

			int nBtnIdx = -1;
			if( szChar[0] != 0 )
			{
				nBtnIdx = GetButtonByAccessKey( szChar[0] );
				if( nBtnIdx < 0 )
				{
					CPtrList _listVisibleToolBars;
					CFrameWnd * pDockSite = m_pDockSite;
					if( pDockSite == NULL )
						pDockSite =
							DYNAMIC_DOWNCAST( CFrameWnd, GetParent() );
					if( pDockSite != NULL )
					{
						ExtControlBarVector_t vBars;
						CExtControlBar::_GetFrameControlBars(
							vBars,
							__GFECB_FIXED|__GFECB_NONFIXED|__GFECB_DOCKED|__GFECB_FLOATED|__GFECB_VIS_ALL
							);
						for( int i = 0; i < vBars.GetSize(); i++ )
						{
							CExtToolControlBar * pToolBar =
								DYNAMIC_DOWNCAST( CExtToolControlBar, vBars[i] );
							if(		pToolBar->GetSafeHwnd() == NULL
								||	pToolBar == this
								)
								continue;
							ASSERT_VALID( pToolBar );
							ASSERT( pToolBar->IsVisible() );
							_listVisibleToolBars.AddTail( pToolBar );
						}
					} // if( pDockSite != NULL )
					else
					{
						for(	CWnd * pWnd = GetParent()->GetWindow( GW_CHILD );
								pWnd != NULL;
								pWnd = pWnd->GetWindow( GW_HWNDNEXT )
								)
						{
							CExtToolControlBar * pToolBar =
								DYNAMIC_DOWNCAST( CExtToolControlBar, pWnd );
							if(		pToolBar->GetSafeHwnd() == NULL
								||	pToolBar == this
								)
								continue;
							ASSERT_VALID( pToolBar );
							if( ! pToolBar->IsVisible() )
								continue;
							_listVisibleToolBars.AddTail( pToolBar );
						}
					} // else from if( pDockSite != NULL )
					for(	POSITION pos = _listVisibleToolBars.GetHeadPosition();
							pos != NULL;
						)
					{
						CExtToolControlBar * pToolBar = (CExtToolControlBar *)
							_listVisibleToolBars.GetNext( pos );
						int nBtnIdx = pToolBar->GetButtonByAccessKey( szChar[0] );
						if( nBtnIdx < 0 )
							continue;
						CExtBarButton * pTBB = pToolBar->GetButton( nBtnIdx );
						ASSERT_VALID( pTBB );
						ASSERT( !pTBB->IsSeparator() );
						if( pTBB->IsAbleToTrackMenu() )
						{
							CExtToolControlBar::g_bMenuTrackingExpanded = false;
							pToolBar->TrackButtonMenu( nBtnIdx, true );
						} // if( pTBB->IsAbleToTrackMenu() )
						else
						{
							g_bCancelNearestAltUp =
								CExtPopupMenuWnd::IsKeyPressed( VK_MENU )
									? true : false;
							pTBB->OnDeliverCmd();
						} // else from if( pTBB->IsAbleToTrackMenu() )
						return TRUE;
					}
					g_bCancelNearestAltUp = true; // (+ v.2.23)
					return FALSE;
				} // if( nBtnIdx < 0 )
				else
				{
					CExtBarButton * pTBB = CExtToolControlBar::GetButton( nBtnIdx );
					ASSERT_VALID( pTBB );
					ASSERT( !pTBB->IsSeparator() );
					if( pTBB->IsAbleToTrackMenu() )
					{
						CExtToolControlBar::g_bMenuTrackingExpanded = false;
						CExtToolControlBar::TrackButtonMenu( nBtnIdx, true );
					} // if( pTBB->IsAbleToTrackMenu() )
					else
					{
						g_bCancelNearestAltUp =
							CExtPopupMenuWnd::IsKeyPressed( VK_MENU )
								? true : false;
						pTBB->OnDeliverCmd();
					} // else from if( pTBB->IsAbleToTrackMenu() )
					return TRUE;
				} // else from if( nBtnIdx < 0 )
			} // if( szChar[0] != 0 )
			g_bCancelNearestAltUp = true; // (+ v.2.23)
			return FALSE;
		} // if( bAlt && (!bDockSiteCustomizeMode) )

		if( m_bFlatTracking && (!bDockSiteCustomizeMode) )
		{
			if( bKeyUp )
				return TRUE;
			bool bUpdateIndex = false;
			switch( vkTCHAR )
			{
			case VK_LEFT:
				bUpdateIndex = true;
				//m_nFlatTrackingIndex--;
				m_nFlatTrackingIndex =
					GetVisibleButton(
						m_nFlatTrackingIndex,
						FALSE
						);
			break;
			case VK_RIGHT:
				bUpdateIndex = true;
				//m_nFlatTrackingIndex++;
				m_nFlatTrackingIndex =
					GetVisibleButton(
						m_nFlatTrackingIndex,
						TRUE
						);
			break;
			case VK_RETURN:
			case VK_UP:
			case VK_DOWN:
			{
				if( CExtPopupMenuWnd::IsMenuTracking()
					|| m_nFlatTrackingIndex < 0
					)
					return TRUE;
				ASSERT( m_nFlatTrackingIndex < GetButtonsCount() );
				CExtBarButton * pTBB =
					_GetButtonPtr(m_nFlatTrackingIndex);
				ASSERT_VALID( pTBB );
				if( pTBB->IsAbleToTrackMenu() )
				{
					int nIndex = m_nFlatTrackingIndex;
					m_bFlatTracking = false;
					_UpdateFlatTracking( FALSE );
					CExtToolControlBar::TrackButtonMenu(
						nIndex,
						true
						);
				}
				else
				{
					if(		vkTCHAR == VK_RETURN
						&&	CExtCmdManager::IsCommand(
								pTBB->GetCmdID(true)
								)
						)
					{
						pTBB->OnClick(
							CPoint(0,0),
							false
							);
					}
				}
				return TRUE;
			}
			break;
			case VK_ESCAPE:
//				if( g_bMenuTracking
//					//&& !m_bFlatTracking
//					)
//				//if( CExtPopupMenuWnd::IsMenuTracking() )
//				{
//					m_nFlatTrackingIndex = GetMenuTrackingButton();
//					_CloseTrackingMenus();
//					if( m_nFlatTrackingIndex >= 0 )
//						m_bFlatTracking = true;
//					else
//						m_bFlatTracking = false;
//				}
//				else
					m_bFlatTracking = false;
				_UpdateFlatTracking();
				return TRUE;
			} // switch( vkTCHAR )
			if( bUpdateIndex )
			{
				int nCountOfButtons = GetButtonsCount();
				ASSERT( nCountOfButtons > 0 );
				if( m_nFlatTrackingIndex < 0 )
					m_nFlatTrackingIndex = nCountOfButtons - 1;
				else if(m_nFlatTrackingIndex >= nCountOfButtons)
					m_nFlatTrackingIndex = 0;
			}
			_UpdateFlatTracking();
			return TRUE;
		} // if( m_bFlatTracking && (!bDockSiteCustomizeMode) )
	}

	if( nMsg == WM_CONTEXTMENU )
	{
		if( g_bMenuTracking || m_bFlatTracking || bDockSiteCustomizeMode )
			return TRUE;
	}

	if( m_bFlatTracking && (!bDockSiteCustomizeMode) )
	{
		if( (nMsg >= WM_MOUSEFIRST && nMsg <= WM_MOUSELAST)
				&& nMsg != WM_MOUSEMOVE
			)
		{
			m_bFlatTracking = false;
			_UpdateFlatTracking();
		}
	} // if( m_bFlatTracking && (!bDockSiteCustomizeMode) )
	
	return FALSE;
}

CRect CExtMenuControlBar::_GetChildFrameSysIconRect()
{
CRect rc;
	rc.SetRectEmpty();
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
		return rc;
	ASSERT( ::IsWindow(hWndActiveChild) );
	if( bMax )
		return rc;
CWnd * pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if( pWnd == NULL )
	{
		ASSERT( FALSE );
		return rc;
	}
	ASSERT_VALID( pWnd );
	ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );

CFrameWnd * pFrame = 
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			pWnd
			);
	ASSERT( pFrame != NULL );
	if( pFrame == NULL )
	{
		ASSERT( FALSE );
		return rc;
	}
	ASSERT_VALID( pFrame );
		
CRect rcWnd,rcClient,rcHelper;
	pFrame->GetWindowRect( &rcWnd );
	pFrame->GetClientRect( &rcClient );
	rcHelper = rcWnd;
	pFrame->ScreenToClient( &rcHelper );
	ASSERT( rcHelper.top <= rcClient.top );
	int yDiff = rcClient.top - rcHelper.top;
	rc.SetRect(
		rcWnd.left + GetSystemMetrics(SM_CYFRAME),
		rcWnd.top + GetSystemMetrics(SM_CXFRAME),
		rcWnd.left + yDiff,
		rcWnd.top + yDiff
		);
	return rc;
}

CRect CExtMenuControlBar::_GetMainFrameSysIconRect()
{
CRect rc;
CRect rcWnd,rcClient,rcHelper;
CFrameWnd * pFrame =
		_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	pFrame->GetWindowRect( &rcWnd );
	pFrame->GetClientRect( &rcClient );
	rcHelper = rcWnd;
	pFrame->ScreenToClient( &rcHelper );
	ASSERT( rcHelper.top <= rcClient.top );
	int yDiff = rcClient.top - rcHelper.top;
	rc.SetRect(
		rcWnd.left + GetSystemMetrics(SM_CYFRAME),
		rcWnd.top + GetSystemMetrics(SM_CXFRAME),
		rcWnd.left + yDiff,
		rcWnd.top + yDiff
		);
	return rc;
}

BOOL CExtMenuControlBar::_TrackFrameSystemMenu(
	CFrameWnd * pFrame,
	CPoint * pPoint, // = NULL, // NULL means calc meny track area automatically
	BOOL bSelectAny, // = FALSE
	LPCRECT rcExcludeArea, // = NULL
	UINT nTrackFlags, // = (UINT)(-1)
	BOOL bCombinedMode // = FALSE
	)
{
	if( m_bFlatTracking )
	{
		m_bFlatTracking = false;
		_UpdateFlatTracking();
	}

	if( nTrackFlags == (UINT)(-1) )
		nTrackFlags = TPMX_BOTTOMALIGN;
	ASSERT_VALID( pFrame );

bool bPrevTBMT = CExtToolControlBar::g_bMenuTracking;
int nTrackingIdxOld = m_nBtnIdxMenuTracking;

//	if( g_bMenuTracking )
		CExtToolControlBar::_CloseTrackingMenus();
//	if(	CExtPopupMenuWnd::IsMenuTracking() )
//		CExtPopupMenuWnd::CancelMenuTracking();
CMenu * pSysMenu = pFrame->GetSystemMenu( FALSE );
	if( pSysMenu == NULL )
		return FALSE;
	ASSERT( ::IsMenu(pSysMenu->GetSafeHmenu()) );

CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	if( !pPopup->CreatePopupMenu( pFrame->GetSafeHwnd() ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( !pPopup->UpdateFromMenu(
			pFrame->GetSafeHwnd(),
			pSysMenu,
			false
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}

CPoint point;
	if( pPoint != NULL)
	{
		point = *pPoint;
	}
	else
	{
		CRect rcWnd,rcClient,rcHelper;
		pFrame->GetWindowRect( &rcWnd );
		pFrame->GetClientRect( &rcClient );
		rcHelper = rcWnd;
		pFrame->ScreenToClient( &rcHelper );
		ASSERT( rcHelper.top <= rcClient.top );
		int yDiff = rcClient.top - rcHelper.top;
		point = rcWnd.TopLeft();
		point.x += GetSystemMetrics(SM_CXFRAME);
		point.y += yDiff;
	}
	if( bSelectAny )
		nTrackFlags |= TPMX_SELECT_ANY;
	if( bPrevTBMT )
		nTrackFlags |= TPMX_NO_HIDE_RARELY;
	pFrame->SetFocus();

	CExtToolControlBar::g_bMenuTracking =
		bSelectAny ? bPrevTBMT : false; // (+ v.2.23)
	if( nTrackingIdxOld >= 0 )
		_SwitchMenuTrackingIndex( nTrackingIdxOld );

	if(	!pPopup->TrackPopupMenu(
			nTrackFlags|TPMX_COMBINE_DEFAULT|TPMX_OWNERDRAW_FIXED,
			point.x,point.y,
			rcExcludeArea,
			bCombinedMode ?
				this : NULL,
			bCombinedMode ?
				CExtToolControlBar::_CbPaintCombinedContent : NULL,
			NULL,
			bCombinedMode ? true : false
			)
		)
	{
		delete pPopup;
		CExtToolControlBar::_CloseTrackingMenus();
		return FALSE;
	}

	g_bMenuTracking = true;

	return TRUE;
}

BOOL CExtMenuControlBar::TrackChildFrameSystemMenu(
	CPoint * pPoint, // = NULL, // NULL means calc meny track area automatically
	BOOL bSelectAny // = FALSE
	)
{
	if( !m_bMdiApp )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	if( GetButtonsCount() > 0 )
	{
		CExtBarButton * pTBB = _GetButtonPtr( 0 );
		ASSERT_VALID( pTBB );
		if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
		{
			if( !TrackButtonMenu( 0 ) )
			{
				ASSERT( FALSE );
				return FALSE;
			} // if( !TrackButtonMenu( 0 ) )
			return TRUE;
		} // if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
	} // if( GetButtonsCount() > 0 )

BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( ::IsWindow(hWndActiveChild) );
CWnd * pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if( pWnd == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT_VALID( pWnd );
	ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );

CFrameWnd * pFrame = 
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			pWnd
			);
	ASSERT( pFrame != NULL );
	if( pFrame == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT_VALID( pFrame );

	g_bMenuTracking = true;
	_SwitchMenuTrackingIndex();
	m_bSysMenuTracking =
		_TrackFrameSystemMenu(
			pFrame,
			pPoint,
			bSelectAny
			) ? true : false;
	return m_bSysMenuTracking ? TRUE : FALSE;
}

BOOL CExtMenuControlBar::TrackMainFrameSystemMenu(
	CPoint * pPoint, // = NULL // NULL means calc meny track area automatically
	BOOL bSelectAny // = FALSE
	)
{
CFrameWnd * pFrame =
		_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	g_bMenuTracking = true;
	_SwitchMenuTrackingIndex();
	m_bSysMenuTracking =
		_TrackFrameSystemMenu(
			pFrame,
			pPoint,
			bSelectAny
			) ? true : false;
	return m_bSysMenuTracking ? TRUE : FALSE;
}

BOOL CExtMenuControlBar::TrackButtonMenu(
	int nIndex,
	BOOL bSelectAny, // = FALSE
	BOOL bDelayedTracking // = FALSE
	)
{
	bSelectAny;
	bDelayedTracking;

	if( g_bMenuTracking
		&& m_nBtnIdxMenuTracking == nIndex
		)
		return TRUE;

	if( !SafeDisplayBar() )
		return FALSE;

	if( m_bFlatTracking )
	{
		m_bFlatTracking = false;
		_UpdateFlatTracking();
	}

	VERIFY(
		PostMessage(
			g_nMsgTrackButtonMenu,
			(WPARAM)nIndex
			)
		);
	return TRUE;
}

LRESULT CExtMenuControlBar::OnTrackButtonMenu(WPARAM wParam,LPARAM lParam)
{
	lParam;
	if( !(	::IsWindowEnabled(
				m_bPresubclassDialogMode
					? GetOwner()->GetSafeHwnd()
					: _GetDockingFrameImpl()->GetSafeHwnd()
				)
			)
		) 
		return 0;
int nCountOfButtons = GetButtonsCount();
	ASSERT( nCountOfButtons >= 0 );
	if( nCountOfButtons == 0 )
		return 0;
int nIndex = int(wParam);
	if( nIndex < 0 || nIndex>=nCountOfButtons )
	{
		if( m_bFlatTracking )
		{
			m_bFlatTracking = false;
			_UpdateFlatTracking();
		}
		_CloseTrackingMenus();
		return 0;
	}
CExtBarButton * pTBB = _GetButtonPtr(nIndex);
	ASSERT_VALID( pTBB );
BOOL bDroppedMode =
		CExtPopupMenuWnd::IsMenuTracking();
	if( !pTBB->IsAbleToTrackMenu() )
		bDroppedMode = FALSE;
	if( bDroppedMode )
	{
		CExtToolControlBar::TrackButtonMenu( nIndex, true );
		return 0;
	}
	_CloseTrackingMenus();
	if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
		CExtMouseCaptureSink::ReleaseCapture();

	m_nFlatTrackingIndex = nIndex;
	m_bFlatTracking = true;
	_UpdateFlatTracking();

	return 0;
}

void CExtMenuControlBar::_UpdateFlatTracking(
	BOOL bRepaint // = TRUE
	)
{
	if( m_nFlatTrackingIndex < 0
		&& m_nOldTrackingIndex < 0
		)
	{
		m_bFlatTracking = false;
		return;
	}

HWND hWndCurrentFocus = ::GetFocus();
	if( hWndCurrentFocus != NULL )
	{
		CWnd * pWndTest = CWnd::FromHandlePermanent( hWndCurrentFocus );
		if( pWndTest == NULL )
		{
			HWND hWnd = ::GetParent( hWndCurrentFocus );
			if( hWnd != NULL )
				pWndTest = CWnd::FromHandlePermanent( hWnd );
		} // if( pWndTest == NULL )
		if( pWndTest != NULL )
		{
			CComboBox * pComboTest =
				DYNAMIC_DOWNCAST( CComboBox, pWndTest );
			if( pComboTest == NULL )
			{
				pWndTest = pWndTest->GetParent();
				if( pWndTest != NULL )
				{
					pComboTest =
						DYNAMIC_DOWNCAST( CComboBox, pWndTest );
				}
			} // if( pComboTest == NULL )
			if(		pComboTest != NULL
				&&	(pComboTest->GetStyle() & (CBS_DROPDOWN|CBS_DROPDOWNLIST)) != 0
				&&	pComboTest->GetDroppedState()
				)
				pComboTest->ShowDropDown( FALSE );
		} // if( pWndTest != NULL )
	} // if( hWndCurrentFocus != NULL )

int nCountOfButtons = GetButtonsCount();
	if( !m_bFlatTracking )
	{
		if( m_nFlatTrackingIndex >= 0 )
		{
			ASSERT( m_nFlatTrackingIndex < nCountOfButtons );
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nFlatTrackingIndex);
			ASSERT_VALID( pTBB );
			pTBB->ModifyStyle(0,TBBS_PRESSED);
			if( bRepaint )
			{
				_InvalidateButton(m_nFlatTrackingIndex);
				UpdateWindow();
			}
			m_nFlatTrackingIndex = m_nOldTrackingIndex = -1;
		}
//		if( g_bMenuTracking )
//			_CloseTrackingMenus();
		return;
	} // if( !m_bFlatTracking )
	if( m_nFlatTrackingIndex != m_nOldTrackingIndex )
	{
		if( m_nOldTrackingIndex >= 0 )
		{
			ASSERT( m_nOldTrackingIndex < nCountOfButtons );
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nOldTrackingIndex);
			ASSERT_VALID( pTBB );
			pTBB->ModifyStyle(0,TBBS_PRESSED|TBBS_CHECKED);
			if( bRepaint )
				_InvalidateButton(m_nOldTrackingIndex);
		}
		if( m_nFlatTrackingIndex >= 0 )
		{
			ASSERT( m_nFlatTrackingIndex < nCountOfButtons );
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nFlatTrackingIndex);
			ASSERT_VALID( pTBB );
			pTBB->ModifyStyle(TBBS_PRESSED);
			if( bRepaint )
				_InvalidateButton(m_nFlatTrackingIndex);
		}
		m_nOldTrackingIndex = m_nFlatTrackingIndex;
		UpdateWindow();
	} // if( m_nFlatTrackingIndex != m_nOldTrackingIndex )
}

void CExtMenuControlBar::_DelaySyncActiveMdiChild()
{
	ASSERT_VALID( this );
	if(		m_bDelayedSyncActiveMdiChild
		||	m_bDelayedUpdateMenuBar
		||	m_hWnd == NULL
		||	(! ::IsWindow(m_hWnd) )
		)
		return;
	m_bDelayedSyncActiveMdiChild = true;
	PostMessage( WM_TIMER, 13, 0 );
}

void CExtMenuControlBar::_DelayUpdateMenuBar()
{
	ASSERT_VALID( this );
	if(		m_bDelayedUpdateMenuBar
		||	m_hWnd == NULL
		||	(! ::IsWindow(m_hWnd) )
		)
		return;
	m_bDelayedSyncActiveMdiChild = false;
	m_bDelayedUpdateMenuBar = true;
	PostMessage( WM_TIMER, 14, 0 );
}

void CExtMenuControlBar::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == 13 )
	{
		if( m_bDelayedSyncActiveMdiChild )
			_SyncActiveMdiChild();
		m_bDelayedSyncActiveMdiChild = false;
		return;
	}
	if( nIDEvent == 14 )
	{
		if( m_bDelayedUpdateMenuBar )
			VERIFY( UpdateMenuBar( TRUE ) );
		m_bDelayedUpdateMenuBar = false;
		return;
	}
	CExtToolControlBar::OnTimer(nIDEvent);
}

bool CExtMenuControlBar::_OnMouseMoveMsg(UINT nFlags, CPoint point) 
{
	if( !m_bFlatTracking )
		return CExtToolControlBar::_OnMouseMoveMsg(nFlags, point);
int nIndex = HitTest(point);
	if( nIndex == m_nFlatTrackingIndex
		|| nIndex < 0
		)
		return true;
	//m_nFlatTrackingIndex = nIndex;
	//_UpdateFlatTracking();
	TrackButtonMenu( nIndex );
	return true;
}

void CExtMenuControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bFlatTracking )
	{
		int nIndex = HitTest(point);
		if( nIndex >= 0 )
		{
			int nMenuIndex = GetMenuTrackingButton();
			if( nIndex == nMenuIndex )
				return;
		}
	}

	CExtToolControlBar::OnLButtonDown(nFlags, point);
}

void CExtMenuControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
BOOL bRepaint = FALSE;
	if( m_bFlatTracking )
	{
		m_bFlatTracking = false;
		_UpdateFlatTracking( FALSE );
		bRepaint = TRUE;
	}
	CExtToolControlBar::OnLButtonUp(nFlags, point);
	if( bRepaint )
		Invalidate();
}

void CExtMenuControlBar::_RecalcLayoutImpl()
{
	CExtToolControlBar::_RecalcLayoutImpl();
	if(		m_pDockSite != NULL
		&&	(! IsFloating() )
		&&	( ! ((InternalFriendlyFrameWnd*)m_pDockSite)->IsInRecalcLayout() )
		)
		m_pDockSite->RecalcLayout();
}

void CExtMenuControlBar::_RecalcPositionsImpl()
{
	CExtToolControlBar::_RecalcPositionsImpl();

	if( g_bHideMenuBarExpandContentButton
		&& m_pRightBtn != NULL
		)
	{
		ASSERT_VALID( m_pRightBtn );
		if( m_pRightBtn->GetButtons().GetSize() == 0 )
			m_pRightBtn->Show( FALSE );
	}

CWnd * pWndParent = GetParent();
	ASSERT_VALID( pWndParent );
BOOL bInDockBar = pWndParent->IsKindOf(RUNTIME_CLASS(CDockBar));
	if( IsFloating() && bInDockBar )
		return;
int nCountOfButtons = GetButtonsCount();
	if( nCountOfButtons <= 1 )
		return;
int nRightIndex = nCountOfButtons - 1;
CExtBarMdiRightButton * pRightTBB =
		DYNAMIC_DOWNCAST(
			CExtBarMdiRightButton,
			_GetButtonPtr(nRightIndex)
			);
	if( pRightTBB == NULL && nCountOfButtons > 1 )
	{
		nRightIndex--;
		pRightTBB =
			DYNAMIC_DOWNCAST(
				CExtBarMdiRightButton,
				_GetButtonPtr(nRightIndex)
				);
	} // if( pRightTBB == NULL && nCountOfButtons > 1 )
	if( pRightTBB == NULL )
		return;
	if( !pRightTBB->IsVisible() )
		return;
	ASSERT( nRightIndex >= 1 );

#ifdef _DEBUG
CExtBarButton * pPrevTBB =
		_GetButtonPtr( nRightIndex - 1 );
	ASSERT_VALID( pPrevTBB );
///	ASSERT( pPrevTBB->IsVisible() );
#endif // _DEBUG

CExtBarButton * pExpandTBB = NULL;
CRect rcBtnExpand(0,0,0,0);
int nAlignMargin = 0;
BOOL bHorz = 
	bInDockBar
		? IsDockedHorizontally()
		: (	(m_dwStyle & (CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM))
				? true
				: false
			)
		;
CRect rcClient;
	GetClientRect( &rcClient );
	
	if( nRightIndex < (nCountOfButtons-1) )
	{
		pExpandTBB = _GetButtonPtr( nRightIndex + 1 );
		ASSERT_VALID( pExpandTBB );
		if( !pExpandTBB->IsKindOf( RUNTIME_CLASS(CExtBarContentExpandButton)) )
			pExpandTBB = NULL;
	} // if( nRightIndex < (nCountOfButtons-1) )
	
	if( pExpandTBB != NULL )
	{ // if content-expand button exists
		if( pExpandTBB->IsVisible() )
		{
			rcBtnExpand = pExpandTBB->Rect();
			nAlignMargin =
				bHorz ? rcBtnExpand.left : rcBtnExpand.top;
		} // if( pExpandTBB->IsVisible() )
		else
		{
			pExpandTBB = NULL;
			nAlignMargin =
				bHorz ? rcClient.right : rcClient.bottom;
		} // else from if( pExpandTBB->IsVisible() )
		nAlignMargin -= __MENUBAR_RIGHT_ADJUST_GAP;
	} // if content-expand button exists
	else
	{  // if content-expand button DOES NOT exists
		nAlignMargin =
			bHorz ? rcClient.right : rcClient.bottom;
		nAlignMargin -= __MENUBAR_RIGHT_ADJUST_GAP;
	}  // if content-expand button DOES NOT exists
CRect rcBtnRight = pRightTBB->Rect();
	if( bHorz && rcBtnRight.right >= nAlignMargin )
		return;
	if( (!bHorz) && rcBtnRight.bottom >= nAlignMargin )
		return;
	if( bHorz )
	{
		rcBtnRight.OffsetRect(
			nAlignMargin - rcBtnRight.right,
			0
			);
	} // if( bHorz )
	else
	{
		rcBtnRight.OffsetRect(
			0,
			nAlignMargin - rcBtnRight.bottom
			);
	} // else from if( bHorz )

	if( pExpandTBB == NULL )
	{
		pRightTBB->SetRect(rcBtnRight);
		return;
	}

	ASSERT( !rcBtnExpand.IsRectEmpty() );
CRect rcBtnExpand2(
		bHorz ? rcBtnRight.left : rcBtnExpand.left,
		bHorz ? rcBtnExpand.top : rcBtnRight.top,
		bHorz ?
			rcBtnRight.left + rcBtnExpand.Width()
			:
			rcBtnExpand.left + rcBtnExpand.Width()
			,
		bHorz ?
			rcBtnExpand.top + rcBtnExpand.Height()
			:
			rcBtnRight.top + rcBtnExpand.Height()
		);
	rcBtnRight.OffsetRect(
		bHorz ? rcBtnExpand.right - rcBtnRight.right : 0,
		bHorz ? 0 : rcBtnExpand.bottom - rcBtnRight.bottom
		);
	pRightTBB->SetRect(rcBtnRight);
	pExpandTBB->SetRect(rcBtnExpand2);

	if( g_bHideMenuBarExpandContentButton )
		pExpandTBB->Show( FALSE );
}

bool CExtMenuControlBar::_GetFullRowMode() const
{
	ASSERT_VALID( this );
	return true;
}

bool CExtMenuControlBar::OnQueryMultiRowLayout() const
{
	ASSERT_VALID( this );
	if(		m_bPresubclassDialogMode
//		||	m_pDockSite == NULL
		)
		return false;
	if( m_pDockBar != NULL )
	{
		if( IsFloating() )
			return false;
		if( _IsDockSiteCustomizeMode() )
			return true;
	} // if( m_pDockBar != NULL )
	return m_bMultiRowLayout;
}

CExtBarContentExpandButton * CExtMenuControlBar::OnCreateBarRightBtn()
{
	ASSERT_VALID( this );
	if( m_bMultiRowLayout )
		return NULL;
	return CExtToolControlBar::OnCreateBarRightBtn();
}

void CExtMenuControlBar::_Dragging_OnStart()
{
	if( m_bFlatTracking )
	{
		m_bFlatTracking = false;
		_UpdateFlatTracking();
	}
	if( g_bMenuTracking )
		_CloseTrackingMenus();
	CExtToolControlBar::_Dragging_OnStart();
}

void CExtMenuControlBar::_Dragging_OnStop(
	CExtControlBar::InternalDraggingState_t & _dsOld,
	CExtControlBar::InternalDraggingState_t & _dsNew,
	bool bCancel
	)
{
	CExtToolControlBar::_Dragging_OnStop(
		_dsOld,
		_dsNew,
		bCancel
		);
	if( m_bMdiApp )
	{
		_InstallMdiDocButtons();
		_RecalcPositionsImpl();
	}
}

HWND CExtMenuControlBar::_GetHwndMainFrame()
{
	if( m_bPresubclassDialogMode )
		return NULL;

CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );
	return pFrame->GetSafeHwnd();
}

HWND CExtMenuControlBar::_GetHwndPlacement()
{
	if( !m_bPresubclassDialogMode )
		return NULL;
	return m_hWndHelper;
}

HWND CExtMenuControlBar::_GetHwndChildFrame()
{
	if( m_bPresubclassDialogMode )
		return NULL;
	return m_hWndHelper;
}

HWND CExtMenuControlBar::_GetHwndMdiArea()
{
	if( m_bPresubclassDialogMode )
		return NULL;

	ASSERT( m_bMdiApp );

CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );

	ASSERT_KINDOF( CMDIFrameWnd, pFrame );

HWND hWndMdiArea = ((CMDIFrameWnd*)pFrame)->m_hWndMDIClient;
	ASSERT( hWndMdiArea != NULL );
	ASSERT( ::IsWindow(hWndMdiArea) );
	
	return hWndMdiArea;
}

int CExtMenuControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExtToolControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

CFrameWnd * pFrame = _GetDockingFrameImpl();
	if( pFrame == NULL )
	{
		m_bPresubclassDialogMode = true;
		if( m_hWndHelper == NULL )
		{
			m_hWndHelper = ::GetParent( m_hWnd );
			ASSERT( m_hWndHelper != NULL );
			ASSERT( ::IsWindow( m_hWndHelper ) );
			if( !IsHookedWindow( m_hWndHelper ) )
				SetupHookWndSink( m_hWndHelper );
		} // if( m_hWndHelper == NULL )
		return 0;
	}

	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );

	ASSERT( m_menuFrame.GetSafeHmenu() == NULL );
	ASSERT( m_menuDoc.GetSafeHmenu() == NULL );
HMENU hFrameMenu = pFrame->GetMenu()->GetSafeHmenu();
	if( hFrameMenu != NULL )
	{
		m_menuFrame.Attach( hFrameMenu );
		_UpdateMenuBar( FALSE );
		pFrame->SetMenu( NULL );
	}

	if(	!SetupHookWndSink( _GetHwndMainFrame() ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	if( pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) )
	{
		m_bMdiApp = true;
		if(	!SetupHookWndSink( _GetHwndMdiArea() ) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
			
		::SetWindowPos(
			_GetHwndMdiArea(),
			NULL,
			0,0,0,0,
			SWP_FRAMECHANGED
				|SWP_NOOWNERZORDER|SWP_NOCOPYBITS
				|SWP_NOREPOSITION
				|SWP_NOZORDER|SWP_NOACTIVATE
				|SWP_NOMOVE|SWP_NOSIZE
			);

		VERIFY( _SyncActiveMdiChild() );

	} // if( pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) )

	// pass message loop for smooth MDI client area update reasons
	CExtPopupMenuWnd::PassMsgLoop( CExtControlBar::g_bEnableOnIdleCalls );

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiDocButton

IMPLEMENT_DYNCREATE(CExtBarMdiDocButton,CExtBarButton)

CSize CExtBarMdiDocButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( GetBar()->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT_VALID( (&dc) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );
//	CExtBarButton::CalculateLayout( dc, sizePreCalc, bHorz );
//const int nExtraMargin =
//		  __EXT_TB_BUTTON_OUTER_MARGIN * 2
//		+ __EXT_TB_BUTTON_INNER_MARGIN * 2
//		+ __EXT_TB_BUTTON_TEXT_MARGIN * 2
//		;
//	if( m_ActiveSize.cx > nExtraMargin )
//		m_ActiveSize.cx -= nExtraMargin;
//	if( m_ActiveSize.cy > nExtraMargin )
//		m_ActiveSize.cy -= nExtraMargin;
//	return m_ActiveSize;
	return CExtBarButton::CalculateLayout( dc, sizePreCalc, bHorz );
}

bool CExtBarMdiDocButton::CanStartCommandDrag()
{
	ASSERT_VALID( this );
	return false;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeCmdTreeNode * CExtBarMdiDocButton::GetCmdNode(
	bool bInitial // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	bInitial;
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	return NULL;
}
void CExtBarMdiDocButton::SetBasicCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	ASSERT( FALSE ); // should be never called
	pNode;
}
void CExtBarMdiDocButton::SetCustomizedCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	ASSERT( FALSE ); // should be never called
	pNode;
}
CExtCustomizeCmdTreeNode * CExtBarMdiDocButton::OnCustomizeNodeInit(
	CExtCustomizeSite * pSite,
	CExtCustomizeCmdTreeNode * pParentNode // toolbar node
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( pSite != NULL );
	ASSERT_VALID( pParentNode );
	pSite;
	pParentNode;
	return NULL; // no customization support
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

BOOL CExtBarMdiDocButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT( pPopup != NULL );
	ASSERT( pPopup->GetSafeHwnd() == NULL );

	ASSERT_VALID( GetBar() );
	ASSERT( GetBar()->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

BOOL bDummyMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bDummyMax );
//	if( hWndActiveChild == NULL )
//		return;
	ASSERT( ::IsWindow( hWndActiveChild ) );

CFrameWnd * pFrame =
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			CWnd::FromHandle(hWndActiveChild)
			);
CMenu * pSysMenu =
		pFrame->GetSystemMenu( FALSE );
	if( pSysMenu == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( ::IsMenu(pSysMenu->GetSafeHmenu()) );
CExtCmdIcon * pIcon = GetIconPtr();

#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

CExtSafeString sActiveDocument;
	if( !sActiveDocument.LoadString(IDS_ACTIVE_DOCUMENT) )
	{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		sActiveDocument = _T("Active document");
	}
	if( !pPopup->ItemInsert(
			(UINT)CExtPopupMenuWnd::TYPE_POPUP,
			-1,
			sActiveDocument,
			(pIcon == NULL) ? NULL : pIcon->GetIcon()
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
CExtPopupMenuWnd * pChildPopup =
		pPopup->ItemGetPopup(
			pPopup->ItemGetCount() - 1
			);
	ASSERT( pChildPopup != NULL );
	if(	!pChildPopup->UpdateFromMenu(
			GetBar()->GetSafeHwnd(),
			pSysMenu,
			false,
			false
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
	pChildPopup->
		SetCmdTargetToAllItems(
			hWndActiveChild
			);

	VERIFY(
		pPopup->ItemInsert(
			CExtPopupMenuWnd::TYPE_SEPARATOR,
			-1
			)
		);

	return TRUE;
}

HWND CExtBarMdiDocButton::_GetActiveMdiChildWnd(
	BOOL & bMaximized
	)
{
	bMaximized = FALSE;
CMDIFrameWnd * pFrame =
		DYNAMIC_DOWNCAST(
			CMDIFrameWnd,
			GetBar()->_GetDockingFrameImpl()
			);
	if( pFrame == NULL )
		return NULL;
HWND hWndMdiArea = pFrame->m_hWndMDIClient;
	ASSERT( hWndMdiArea != NULL );
	ASSERT( ::IsWindow(hWndMdiArea) );
//BOOL bMax = FALSE;
HWND hWndActiveChild = (HWND)
		::SendMessage(
			hWndMdiArea,
			WM_MDIGETACTIVE,
			0,
			(LPARAM)&bMaximized
			);
	return hWndActiveChild;
}

CExtCmdIcon * CExtBarMdiDocButton::GetIconPtr()
{
	ASSERT_VALID( GetBar() );
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
		return NULL;
	ASSERT( ::IsWindow(hWndActiveChild) );

HICON hIcon = (HICON)
	hIcon = (HICON)
		::SendMessage(
			hWndActiveChild,
			CExtControlBar::g_nMsgQueryIcon,
			0,
			0
			);
	if( hIcon == NULL )
		hIcon = (HICON)
			::SendMessage(
				hWndActiveChild,
				WM_GETICON,
				ICON_SMALL,
				0
				);
	if( hIcon == NULL )
		hIcon = (HICON)
			::SendMessage(
				hWndActiveChild,
				WM_GETICON,
				ICON_BIG,
				0
				);
	if(hIcon == NULL)
		hIcon = (HICON)
			::GetClassLong(
				hWndActiveChild,
				GCL_HICONSM
				);

	if(hIcon == NULL)
		return NULL;
	m_iconCache.AssignFromHICON( hIcon, true );
	return &m_iconCache;
}

CWnd * CExtBarMdiDocButton::GetCmdTargetWnd()
{
	ASSERT_VALID( GetBar() );
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
	{
		return NULL;
	};
	ASSERT( ::IsWindow(hWndActiveChild) );
CWnd * pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if( pWnd == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	ASSERT_VALID( pWnd );
	ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );
	return pWnd;
}

UINT CExtBarMdiDocButton::OnTrackPopup(
	CPoint point,
	bool bSelectAny
	)
{
	ASSERT_VALID( GetBar() );

bool bDockSiteCustomizeMode =
		GetBar()->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return UINT(-1L);

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			GetBar()
			);
CWnd * pWnd = CExtBarMdiDocButton::GetCmdTargetWnd();
CFrameWnd * pFrame = 
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			pWnd
			);
	ASSERT( pFrame != NULL );
	if( pBar == NULL || pFrame == NULL )
	{
		CExtBarButton::OnTrackPopup( point, bSelectAny );
		return UINT(-1L);
	}

bool bPrevTBMT = CExtToolControlBar::g_bMenuTracking;
	if( CExtToolControlBar::g_bMenuTracking
		//&& CExtPopupMenuWnd::IsMenuTracking()
		&& pBar->_GetIndexOf(this) ==
			pBar->m_nBtnIdxMenuTracking
		)
		return UINT(-1L);

	CExtToolControlBar::_CloseTrackingMenus();

	if( pBar->IsFloating() )
	{
		pBar->ActivateTopParent();
		CFrameWnd * pFrame =
			pBar->GetDockingFrame();
		ASSERT_VALID( pFrame );
		pFrame->BringWindowToTop();
	}

UINT nTrackFlags = TPMX_TOPALIGN;
CRect rcBtn = Rect();
//	point.x = rcBtn.left;
//	point.y = rcBtn.bottom;
	switch( GetBar()->GetSafeDockBarDlgCtrlID() )
	{
	case AFX_IDW_DOCKBAR_BOTTOM:
		nTrackFlags = TPMX_BOTTOMALIGN;
//		point.x = rcBtn.left;
//		point.y = rcBtn.bottom;
	break;
	case AFX_IDW_DOCKBAR_LEFT:
		nTrackFlags = TPMX_LEFTALIGN;
//		point.x = rcBtn.right;
//		point.y = rcBtn.top;
	break;
	case AFX_IDW_DOCKBAR_RIGHT:
		nTrackFlags = TPMX_RIGHTALIGN;
//		point.x = rcBtn.left;
//		point.y = rcBtn.top;
	break;
	} // switch( GetBar()->GetSafeDockBarDlgCtrlID() )
	pBar->ClientToScreen( &rcBtn );
	//	pBar->ClientToScreen( &point );
	point = rcBtn.CenterPoint();

	CExtToolControlBar::g_bMenuTracking = bPrevTBMT;

	GetBar()->_SwitchMenuTrackingIndex(
		GetBar()->_GetIndexOf( this )
		);

	pBar->m_bSysMenuTracking =
		pBar->
		_TrackFrameSystemMenu(
			pFrame,
			&point, // NULL,
			TRUE,
			&rcBtn,
			nTrackFlags,
			TRUE
			) ? true : false;

	CExtToolControlBar::g_bMenuTracking = true;

	return UINT(-1L);
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiRightButton

IMPLEMENT_DYNCREATE(CExtBarMdiRightButton,CExtBarMdiDocButton)

BOOL CExtBarMdiRightButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT( pPopup != NULL );
	ASSERT( pPopup->GetSafeHwnd() == NULL );

	ASSERT_VALID( GetBar() );
	ASSERT( GetBar()->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			GetBar()
			);
	ASSERT_VALID( pBar );
BOOL bHorz = !pBar->IsDockedVertically();
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );
	if( !_info.m_bInitialized )
		return FALSE;

BOOL bDummyMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bDummyMax );
//	if( hWndActiveChild == NULL )
//		return;
	ASSERT( ::IsWindow( hWndActiveChild ) );

int nCount = pPopup->ItemGetCount();
	if( nCount > 0 )
	{
		if( pPopup->ItemGetCmdID(nCount-1) !=
				CExtPopupMenuWnd::TYPE_SEPARATOR
				)
		{
			VERIFY(
				pPopup->ItemInsert(
					CExtPopupMenuWnd::TYPE_SEPARATOR,
					-1
					)
				);
		}
	}

	if( _info.m_bBtnHelp )
	{
		VERIFY(
			pPopup->ItemInsert(
				SC_CONTEXTHELP,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}
	if( _info.m_bBtnMinimize )
	{
		VERIFY(
			pPopup->ItemInsert(
				SC_MINIMIZE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
		VERIFY(
			pPopup->ItemInsert(
				SC_RESTORE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}
//	if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
//	{
//		VERIFY(
//			pPopup->ItemInsert(
//				SC_MAXIMIZE,
//				-1,NULL,NULL,hWndActiveChild
//				)
//			);
//	}
	if( _info.m_bBtnMaximize )
	{
		VERIFY(
			pPopup->ItemInsert(
				_info.m_bBtnMinimize ? SC_MAXIMIZE : SC_RESTORE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}
	if( _info.m_bBtnClose )
	{
		VERIFY(
			pPopup->ItemInsert(
				SC_CLOSE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}

	return TRUE;
}

CExtCmdIcon * CExtBarMdiRightButton::GetIconPtr()
{
	return NULL;
}

void CExtMenuControlBar::_GetMdiMenuBarRightButtonsInfo(
	CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t & _info,
	CExtBarMdiRightButton * pTBB,
	BOOL bHorz
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTBB );
	_info.m_pWndMdiChildFrame =
		pTBB->GetCmdTargetWnd();
	if( _info.m_pWndMdiChildFrame == NULL )
		return;
	ASSERT( _info.m_pWndMdiChildFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );
	ASSERT_VALID( _info.m_pWndMdiChildFrame );
	_info.m_pBar = this;
DWORD dwFrameStyle = _info.m_pWndMdiChildFrame->GetStyle();
	_info.m_bBtnClose = false;
CMenu * pSysMenu = _info.m_pWndMdiChildFrame->GetSystemMenu( FALSE );
	if( pSysMenu != NULL )
	{
		INT nCount = pSysMenu->GetMenuItemCount();
		for( INT nItem = 0; nItem < nCount; nItem++ )
		{
			UINT nID = pSysMenu->GetMenuItemID( nItem );
			if( nID != SC_CLOSE )
				continue;
			MENUITEMINFO mii;
			::memset(&mii,0,sizeof(MENUITEMINFO));
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_STATE;
			if( !pSysMenu->GetMenuItemInfo(
					nItem,
					&mii,
					TRUE
					)
				)
				break;
			if( ( mii.fState & (MFS_DISABLED|MFS_GRAYED) ) == 0 )
				_info.m_bBtnClose = true;
			break;
		} // for( INT nItem = 0; nItem < nCount; nItem++ )
	} // if( pSysMenu != NULL )
	_info.m_bBtnMaximize =
		(dwFrameStyle & WS_MAXIMIZEBOX) ?
			true : false;
	_info.m_bBtnMinimize =
		(dwFrameStyle & WS_MINIMIZEBOX) ?
			true : false;
	_info.m_bBtnHelp =
		(_info.m_bBtnMaximize && (_info.m_pWndMdiChildFrame->GetExStyle()&WS_EX_CONTEXTHELP)) ?
			true : false;
	if( bHorz )
	{
		_info.m_calcSize.cx = __GAP_BETWEEN_WND_RIGHT_BUTTONS;
		_info.m_calcSize.cy = _info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnMinimize )
			_info.m_calcSize.cx +=
				(_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS)*2;
		if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
			_info.m_calcSize.cx +=
				_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnHelp )
			_info.m_calcSize.cx +=
				_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnClose )
			_info.m_calcSize.cx +=
				_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // if( bHorz )
	else
	{
		_info.m_calcSize.cx = _info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		_info.m_calcSize.cy = __GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnMinimize )
			_info.m_calcSize.cy +=
				(_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS)*2;
		if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
			_info.m_calcSize.cy +=
				_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnHelp )
			_info.m_calcSize.cy +=
				_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnClose )
			_info.m_calcSize.cy +=
				_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // else from if( bHorz )

	_info.m_bInitialized = true;
}

CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t::
	MdiMenuBarRightButtonsInfo_t()
{
	m_bInitialized
		= m_bBtnClose
		= m_bBtnMaximize
		= m_bBtnMinimize
		= m_bBtnHelp
		= false;
	m_pBar = NULL;
	m_pWndMdiChildFrame = NULL;
	if( g_PaintManager.m_bIsWinXPorLater )
	{
		CSize _sizeTmp( // (+ v.2.22)
				::GetSystemMetrics(SM_CXSMSIZE),
				::GetSystemMetrics(SM_CYSMSIZE)
				);
		int nMetric =
			max( _sizeTmp.cx, _sizeTmp.cy ) + 1;
		m_cxIcon = max( nMetric, 18 ); // 16->18 (+ v.2.23)
		m_cyIcon = max( nMetric, 18 ); // 16->18 (+ v.2.23)
	} // if( g_PaintManager.m_bIsWinXPorLater )
	else
	{
		m_cxIcon = ::GetSystemMetrics(SM_CXSIZE);
		m_cyIcon = ::GetSystemMetrics(SM_CYSIZE);
	} // else from if( g_PaintManager.m_bIsWinXPorLater )
	ASSERT(
		m_cxIcon > __GAP_BETWEEN_WND_RIGHT_BUTTONS
		&&
		m_cyIcon > __GAP_BETWEEN_WND_RIGHT_BUTTONS
		);
	m_cxIcon -= __GAP_BETWEEN_WND_RIGHT_BUTTONS;
	m_cyIcon -= __GAP_BETWEEN_WND_RIGHT_BUTTONS;
	m_calcSize.cx = m_calcSize.cy = 0;
	EmptyLocations();
}

void CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t::
	EmptyLocations()
{
	m_rcBtnClose.SetRectEmpty();
	m_rcBtnMaximize.SetRectEmpty();
	m_rcBtnRestore.SetRectEmpty();
	m_rcBtnMinimize.SetRectEmpty();
	m_rcBtnHelp.SetRectEmpty();
}

void CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t::
	InitLocations(
		const CRect & rcBtn,
		BOOL bHorz
		)
{
	ASSERT( m_bInitialized );
	EmptyLocations();
	if( rcBtn.IsRectEmpty() )
		return;
CRect rcStart(0,0,0,0);
CSize sizeOffset(0,0);
	if( bHorz )
	{
		int nOffset = (rcBtn.Height() - m_cyIcon) / 2;
		rcStart.SetRect(
			rcBtn.left + __GAP_BETWEEN_WND_RIGHT_BUTTONS,
			rcBtn.top + nOffset,
			rcBtn.left + __GAP_BETWEEN_WND_RIGHT_BUTTONS
				+ m_cxIcon,
			rcBtn.top + nOffset + m_cyIcon
			);
		sizeOffset.cx = m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // if( bHorz )
	else
	{
		int nOffset = (rcBtn.Width() - m_cxIcon) / 2;
		rcStart.SetRect(
			rcBtn.left + nOffset,
			rcBtn.top + __GAP_BETWEEN_WND_RIGHT_BUTTONS,
			rcBtn.left + nOffset + m_cxIcon,
			rcBtn.top + __GAP_BETWEEN_WND_RIGHT_BUTTONS
				+ m_cxIcon
			);
		sizeOffset.cy = m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // else from if( bHorz )
	if( m_bBtnHelp )
	{
		m_rcBtnHelp = rcStart;
		rcStart.OffsetRect(sizeOffset);
	}
	if( m_bBtnMinimize )
	{
		m_rcBtnMinimize = rcStart;
		rcStart.OffsetRect(sizeOffset);
		m_rcBtnRestore = rcStart;
		rcStart.OffsetRect(sizeOffset);
	}
	if( m_bBtnMaximize && !m_bBtnMinimize )
	{
		m_rcBtnMaximize = rcStart;
		rcStart.OffsetRect(sizeOffset);
	}
	if( m_bBtnClose )
		m_rcBtnClose = rcStart;
}

CSize CExtBarMdiRightButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	ASSERT_VALID( this );
	dc;
	ASSERT_VALID( GetBar() );
	ASSERT( GetBar()->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT_VALID( (&dc) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		STATIC_DOWNCAST(
			CExtMenuControlBar,
			GetBar()
			);
	ASSERT_VALID( pBar );
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );

	//ASSERT( _info.m_bInitialized );
	if( !_info.m_bInitialized )
		return CSize( 1, 1 );

CSize _size( _info.m_calcSize );
	if( bHorz )
	{
		if( _size.cy < sizePreCalc.cy )
			_size.cy = sizePreCalc.cy;
	}
	else
	{
		if( _size.cx < sizePreCalc.cx )
			_size.cx = sizePreCalc.cx;
	}
	
	m_ActiveSize = _size;

	return _size;
}

void CExtBarMdiRightButton::Paint(
	CDC & dc,
	bool bHorz
	)
{
	ASSERT_VALID( GetBar() );
	ASSERT( GetBar()->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT_VALID( (&dc) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			GetBar()
			);
	ASSERT_VALID( pBar );
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );
	if( !_info.m_bInitialized )
		return;
	_info.InitLocations( Rect(), bHorz );

CPoint ptCursor( 0, 0 );
	::GetCursorPos(&ptCursor);
	pBar->ScreenToClient( &ptCursor );
bool bFlat = true;
bool bDrawBorder = true;
int eAlign =
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT;
bool bEnabled = IsEnabled() ? true : false;
	if( _info.m_bBtnHelp )
	{
		bool bHover =
			(IsHover() && _info.m_rcBtnHelp.PtInRect(ptCursor))
				? true : false;
		bool bPushed =
			(IsPressed() && _info.m_rcBtnHelp.PtInRect(ptCursor))
				? true : false;
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbdCtxHelp(
			this,
			bHorz ? true : false,
			_info.m_rcBtnHelp,_T(""),NULL,bFlat,
			bHover, bPushed, false, bEnabled,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_CONTEXTHELP,
			(!bEnabled) || ( bEnabled && (!bHover) && (!bPushed) )
			);
		g_PaintManager->PaintPushButton( dc, _ppbdCtxHelp );
	}
	if( _info.m_bBtnMinimize )
	{
		bool bHover =
			(IsHover() && _info.m_rcBtnRestore.PtInRect(ptCursor))
				? true : false;
		bool bPushed =
			(IsPressed() && _info.m_rcBtnRestore.PtInRect(ptCursor))
				? true : false;
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbdRestore(
			this,
			bHorz ? true : false,
			_info.m_rcBtnRestore,_T(""),NULL,bFlat,
			bHover, bPushed, false, bEnabled,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_RESTORE,
			(!bEnabled) || ( bEnabled && (!bHover) && (!bPushed) )
			);
		g_PaintManager->PaintPushButton( dc, _ppbdRestore );
		bHover =
			(IsHover() && _info.m_rcBtnMinimize.PtInRect(ptCursor))
				? true : false;
		bPushed =
			(IsPressed() && _info.m_rcBtnMinimize.PtInRect(ptCursor))
				? true : false;
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbdMinimize(
			this,
			bHorz ? true : false,
			_info.m_rcBtnMinimize,_T(""),NULL,bFlat,
			bHover, bPushed, false, bEnabled,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_MINIMIZE,
			(!bEnabled) || ( bEnabled && (!bHover) && (!bPushed) )
			);
		g_PaintManager->PaintPushButton( dc, _ppbdMinimize );
	}
	if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
	{
		bool bHover =
			(IsHover() && _info.m_rcBtnMaximize.PtInRect(ptCursor))
				? true : false;
		bool bPushed =
			(IsPressed() && _info.m_rcBtnMaximize.PtInRect(ptCursor))
				? true : false;
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbdMaximize(
			this,
			bHorz ? true : false,
			_info.m_rcBtnMaximize,_T(""),NULL,bFlat,
			bHover, bPushed, false, bEnabled,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_MAXIMIZE,
			(!bEnabled) || ( bEnabled && (!bHover) && (!bPushed) )
			);
		g_PaintManager->PaintPushButton( dc, _ppbdMaximize );
	}
	if( _info.m_bBtnClose )
	{
		bool bHover =
			(IsHover() && _info.m_rcBtnClose.PtInRect(ptCursor))
				? true : false;
		bool bPushed =
			(IsPressed() && _info.m_rcBtnClose.PtInRect(ptCursor))
				? true : false;
		CExtPaintManager::PAINTPUSHBUTTONDATA _ppbdClose(
			this,
			bHorz ? true : false,
			_info.m_rcBtnClose,_T(""),NULL,bFlat,
			bHover, bPushed, false, bEnabled,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_CLOSE,
			(!bEnabled) || ( bEnabled && (!bHover) && (!bPushed) )
			);
		g_PaintManager->PaintPushButton( dc, _ppbdClose );
	}
}

UINT CExtBarMdiRightButton::OnTrackPopup(
	CPoint point,
	bool bSelectAny
	)
{
	point;
	bSelectAny;
	return UINT( -1L );
}

void CExtBarMdiRightButton::OnClick(
	CPoint point,
	bool bDown
	)
{
bool bDockSiteCustomizeMode =
		GetBar()->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return;

	if( bDown )
		return;

//	CExtMouseCaptureSink::ReleaseCapture();

	ASSERT_VALID( GetBar() );
	ASSERT( GetBar()->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			GetBar()
			);
	ASSERT_VALID( pBar );
BOOL bHorz = !pBar->IsDockedVertically();
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );
	if( !_info.m_bInitialized )
		return;
	_info.InitLocations( Rect(), bHorz );

bool bInstallHideFlag = false;
UINT nHelperSysCmdID = 0;
	if( _info.m_bBtnHelp
		&& _info.m_rcBtnHelp.PtInRect(point)
		)
		nHelperSysCmdID = SC_CONTEXTHELP;
	else if( _info.m_bBtnMinimize
		&& _info.m_rcBtnRestore.PtInRect(point)
		)
	{
		nHelperSysCmdID = SC_RESTORE;
		bInstallHideFlag = true;
	}
	else if( _info.m_bBtnMinimize
		&& _info.m_rcBtnMinimize.PtInRect(point)
		)
	{
		nHelperSysCmdID = SC_MINIMIZE;
		bInstallHideFlag = true;
	}
	else if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize
		&& _info.m_rcBtnMaximize.PtInRect(point)
		)
		nHelperSysCmdID = SC_MAXIMIZE;
	else if( _info.m_bBtnClose
		&& _info.m_rcBtnClose.PtInRect(point)
		)
		nHelperSysCmdID = SC_CLOSE;

	if( nHelperSysCmdID == 0 )
		return;


BOOL bDummyMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bDummyMax );
	if( hWndActiveChild == NULL )
		return;
	ASSERT( ::IsWindow( hWndActiveChild ) );
	if( nHelperSysCmdID == SC_MAXIMIZE )
	{
		HWND hWndMdiClientArea =
			::GetParent( hWndActiveChild );
		ASSERT( hWndMdiClientArea != NULL );
		ASSERT( ::IsWindow( hWndMdiClientArea ) );
		::PostMessage(
			hWndMdiClientArea,
			WM_MDIRESTORE,
			(WPARAM)hWndActiveChild,
			0
			);
	} // if( nHelperSysCmdID == SC_MAXIMIZE )
	else
	{
		pBar->ClientToScreen( &point );
		::PostMessage(
			hWndActiveChild,
			WM_SYSCOMMAND,
			nHelperSysCmdID,
			MAKELONG( point.x, point.y )
			);
	} // else

	if( bInstallHideFlag )
	{
		Show( FALSE );
		ASSERT( GetBar()->GetButtonsCount() > 0 );
		CExtBarButton * pTBB = GetBar()->GetButton( 0 );
		ASSERT_VALID( pTBB );
		if( pTBB->IsVisible()
			&& pTBB->IsKindOf( RUNTIME_CLASS(CExtBarMdiDocButton) )
			)
		{
			pTBB->Show( FALSE );
		}
//		GetBar()->Invalidate();
//		GetBar()->UpdateWindow();
//		GetBar()->GetParentFrame()->DelayRecalcLayout();
	}
}

int CExtBarMdiRightButton::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( GetBar()->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			GetBar()
			);
	ASSERT_VALID( pBar );
BOOL bHorz = !pBar->IsDockedVertically();
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );
	if( !_info.m_bInitialized )
		return -1;
	_info.InitLocations( Rect(), bHorz );

	if(		_info.m_bBtnHelp
		&&	_info.m_rcBtnHelp.PtInRect(point)
		)
	{
		if( pTI != NULL )
		{
			::CopyRect(
				&(pTI->rect),
				&_info.m_rcBtnHelp
				);
			CExtSafeString str;
			if( str.LoadString( ID_CONTEXT_HELP ) )
				pTI->lpszText = (LPTSTR)
					::calloc( (str.GetLength() + 1), sizeof(TCHAR) );
			if( pTI->lpszText != NULL )
				_tcscpy( pTI->lpszText, str );
			else
				pTI->lpszText = LPSTR_TEXTCALLBACK;
		}
		return ((int)ID_CONTEXT_HELP);
	}
	if(		_info.m_bBtnMinimize
		&&	_info.m_rcBtnRestore.PtInRect(point)
		)
	{
		if( pTI != NULL )
		{
			::CopyRect(
				&(pTI->rect),
				&_info.m_rcBtnRestore
				);
			CExtSafeString str;
			if( str.LoadString( AFX_IDS_SCRESTORE ) )
				pTI->lpszText = (LPTSTR)
					::calloc( (str.GetLength() + 1), sizeof(TCHAR) );
			if( pTI->lpszText != NULL )
				_tcscpy( pTI->lpszText, str );
			else
				pTI->lpszText = LPSTR_TEXTCALLBACK;
		}
		return ((int)AFX_IDS_SCRESTORE);
	}
	if(		_info.m_bBtnMinimize
		&&	_info.m_rcBtnMinimize.PtInRect(point)
		)
	{
		if( pTI != NULL )
		{
			::CopyRect(
				&(pTI->rect),
				&_info.m_rcBtnMinimize
				);
			CExtSafeString str;
			if( str.LoadString( AFX_IDS_SCMINIMIZE ) )
				pTI->lpszText = (LPTSTR)
					::calloc( (str.GetLength() + 1), sizeof(TCHAR) );
			if( pTI->lpszText != NULL )
				_tcscpy( pTI->lpszText, str );
			else
				pTI->lpszText = LPSTR_TEXTCALLBACK;
		}
		return ((int)AFX_IDS_SCMINIMIZE);
	}
	if(		_info.m_bBtnMaximize && !_info.m_bBtnMinimize
		&&	_info.m_rcBtnMaximize.PtInRect(point)
		)
	{
		if( pTI != NULL )
		{
			::CopyRect(
				&(pTI->rect),
				&_info.m_rcBtnMaximize
				);
			CExtSafeString str;
			if( str.LoadString( AFX_IDS_SCMAXIMIZE ) )
				pTI->lpszText = (LPTSTR)
					::calloc( (str.GetLength() + 1), sizeof(TCHAR) );
			if( pTI->lpszText != NULL )
				_tcscpy( pTI->lpszText, str );
			else
				pTI->lpszText = LPSTR_TEXTCALLBACK;
		}
		return ((int)AFX_IDS_SCMAXIMIZE);
	}
	if(		_info.m_bBtnClose
		&&	_info.m_rcBtnClose.PtInRect(point)
		)
	{
		if( pTI != NULL )
		{
			::CopyRect(
				&(pTI->rect),
				&_info.m_rcBtnMaximize
				);
			CExtSafeString str;
			if( str.LoadString( AFX_IDS_SCCLOSE ) )
				pTI->lpszText = (LPTSTR)
					::calloc( (str.GetLength() + 1), sizeof(TCHAR) );
			if( pTI->lpszText != NULL )
				_tcscpy( pTI->lpszText, str );
			else
				pTI->lpszText = LPSTR_TEXTCALLBACK;
		}
		return ((int)AFX_IDS_SCCLOSE);
	}
	return -1;
}

bool CExtBarMdiRightButton::CanStartCommandDrag()
{
	ASSERT_VALID( this );
	return false;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeCmdTreeNode * CExtBarMdiRightButton::GetCmdNode(
	bool bInitial // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	bInitial;
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	return NULL;
}
void CExtBarMdiRightButton::SetBasicCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	ASSERT( FALSE ); // should be never called
	pNode;
}
void CExtBarMdiRightButton::SetCustomizedCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	ASSERT( FALSE ); // should be never called
	pNode;
}
CExtCustomizeCmdTreeNode * CExtBarMdiRightButton::OnCustomizeNodeInit(
	CExtCustomizeSite * pSite,
	CExtCustomizeCmdTreeNode * pParentNode // toolbar node
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( GetBar() );
	ASSERT( pSite != NULL );
	ASSERT_VALID( pParentNode );
	pSite;
	pParentNode;
	return NULL; // no customization support
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

LRESULT CExtBarMdiRightButton::OnHelpHitTest(
	CPoint point
	)
{
	ASSERT_VALID( this );
	return (LRESULT)OnToolHitTest(point,NULL);
}

void CExtMenuControlBar::_KillFrameMenu()
{
	if( m_bPresubclassDialogMode )
		return;
	if( IsOleIpObjActive() )
		return;
CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
HWND hWndFrame = pFrame->GetSafeHwnd();
	ASSERT( hWndFrame != NULL );
	ASSERT( ::IsWindow(hWndFrame) );
HMENU hFrameRealMenu = ::GetMenu( hWndFrame );
	if( hFrameRealMenu == NULL )
		return;
	::SetMenu( hWndFrame, NULL );
	_DelayUpdateMenuBar();
}

void CExtMenuControlBar::PreSubclassWindow() 
{
	CExtToolControlBar::PreSubclassWindow();
	if( m_bPresubclassDialogMode && m_hWndHelper == NULL )
	{
		m_hWndHelper = ::GetParent( m_hWnd );
		ASSERT( m_hWndHelper != NULL );
		ASSERT( ::IsWindow( m_hWndHelper ) );
		if( !IsHookedWindow( m_hWndHelper ) )
			SetupHookWndSink( m_hWndHelper );
	} // if( m_bPresubclassDialogMode && m_hWndHelper == NULL )
}

BOOL CExtMenuControlBar::PreTranslateMessage(MSG* pMsg) 
{
	if( m_bPresubclassDialogMode )
	{
		if( TranslateMainFrameMessage(pMsg) )
			return TRUE;
	}
	
	return CExtToolControlBar::PreTranslateMessage(pMsg);
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)

CExtCustomizeSite::CCmdMenuInfo * CExtMenuControlBar::MenuInfoGet()
{
	ASSERT_VALID( this );
	return m_pMenuInfo;
}

CExtCustomizeSite::CCmdMenuInfo * CExtMenuControlBar::MenuInfoDetach()
{
	ASSERT_VALID( this );
CExtCustomizeSite::CCmdMenuInfo * pMenuInfo = m_pMenuInfo;
	m_pMenuInfo = NULL;
	return pMenuInfo;
}

CExtCustomizeSite::CCmdMenuInfo * CExtMenuControlBar::MenuInfoAttach(
	CExtCustomizeSite::CCmdMenuInfo * pMenuInfo
	)
{
	ASSERT_VALID( this );
CExtCustomizeSite::CCmdMenuInfo * pMenuInfoOld = m_pMenuInfo;
	m_pMenuInfo = pMenuInfo;
	return pMenuInfoOld;
}

void CExtMenuControlBar::MenuInfoUpdate()
{
	ASSERT_VALID( this );
CExtCustomizeSite * pSite =
		GetCustomizeSite();
	if( pSite == NULL )
		return;
CExtCustomizeSite::CCmdMenuInfo * pMenuInfo =
		pSite->MenuInfoFindForMenuBar();
	if( ((LPVOID)pMenuInfo) != ((LPVOID)MenuInfoGet()) )
		MenuInfoAttach( pMenuInfo );
}

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

void CExtMenuControlBar::SetMdiWindowPopupName(
	__EXT_MFC_SAFE_LPCTSTR sMdiWindowPopupName // = NULL
	)
{
	ASSERT_VALID( this );

	m_sMdiWindowPopupName =
		( sMdiWindowPopupName != NULL )
			? sMdiWindowPopupName
			: _T("")
			;
	m_sMdiWindowPopupName.TrimLeft();
	m_sMdiWindowPopupName.TrimRight();
	while( m_sMdiWindowPopupName.Replace( _T("&"), _T("") ) > 0 )
	{
		m_sMdiWindowPopupName.TrimLeft();
		m_sMdiWindowPopupName.TrimRight();
	}
}
