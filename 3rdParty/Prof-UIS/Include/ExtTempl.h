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

#if (!defined __EXT_TEMPL_H)
#define __EXT_TEMPL_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtWFF template window extension (flicker free window)
//
// removes flicker effect caused by WM_ERASEBKGND or
// non-buffered painting, usefull for new win32 (not old win16)
// common controls such as listview or treeview
//

template <
	class CExtWFFBase = CWnd,
	bool _bExludeChildAreas = true,
	bool _bEat_WM_ERASEBKGND = true,
	bool _bEmulate_WM_ERASEBKGND = true
	>
class CExtWFF : public CExtWFFBase
{
protected:
	virtual LRESULT WindowProc( 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		switch( message )
		{
		case WM_ERASEBKGND:
			if( _bEat_WM_ERASEBKGND )
				return FALSE;
		break;
		case WM_PAINT:
			{
				CPaintDC dcPaint( this );
				if( _bExludeChildAreas )
				{
					CExtPaintManager::stat_ExcludeChildAreas(
						dcPaint.GetSafeHdc(),
						GetSafeHwnd()
						);
				}
				CRect rcClient;
				GetClientRect( &rcClient );
				CExtMemoryDC dc(
					&dcPaint,
					&rcClient
					);
				if( _bEmulate_WM_ERASEBKGND )
					DefWindowProc(
						WM_ERASEBKGND,
						WPARAM( dc.GetSafeHdc() ),
						LPARAM(0)
						);
				DefWindowProc(
					WM_PAINT,
					WPARAM( dc.GetSafeHdc() ),
					LPARAM(0)
					);
			}
			return TRUE;
		};
		return CExtWFFBase::WindowProc( message, wParam, lParam );
	};
}; // class CExtWFF

/////////////////////////////////////////////////////////////////////////////
// CExtWRB template window extension (window in resizable bar)
//
// adds thin nonclient area border according to paint manager;
// useful for windows inserted into resizable control bar
//

template <
	class CExtWRBBase = CWnd
	>
class CExtWRB : public CExtWRBBase
{
protected:
	virtual LRESULT WindowProc( 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		switch( message )
		{
		case WM_NCCALCSIZE:
			{
				NCCALCSIZE_PARAMS * pNCCSP =
					reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
				ASSERT( pNCCSP != NULL );
				
				CRect rcInBarWnd( pNCCSP->rgrc[0] );
				rcInBarWnd.DeflateRect(
					1, 1, 1, 1
					);
				::CopyRect( &(pNCCSP->rgrc[0]), rcInBarWnd );
				
				return 0;
			} // case WM_NCCALCSIZE
		case WM_NCPAINT:
			{
				CRect rcInBarWnd, rcInBarClient;
				GetWindowRect( &rcInBarWnd );
				GetClientRect( &rcInBarClient );
				ClientToScreen( &rcInBarClient );
				if( rcInBarWnd == rcInBarClient )
					return 0;
				CPoint ptDevOffset = -rcInBarWnd.TopLeft();
				rcInBarWnd.OffsetRect( ptDevOffset );
				rcInBarClient.OffsetRect( ptDevOffset );

				CWindowDC dc( this );
				ASSERT( dc.GetSafeHdc() != NULL );
				dc.ExcludeClipRect( &rcInBarClient );

				g_PaintManager->PaintResizableBarChildNcAreaRect(
					dc,
					rcInBarWnd,
					this
					);

				return 0;
			} // case WM_NCPAINT
		}; // switch( message )
		return CExtWRBBase::WindowProc( message, wParam, lParam );
	};
}; // class CExtWRB

/////////////////////////////////////////////////////////////////////////////
// CExtWS template window extension (styled window)
//
// adds current PaintManager's background style,
// load/save window position feature,
// right-bottom resizing gripper (default is off)
// and cool system menu for WS_POPUP windows
//

#ifdef GET_WM_CTLCOLOR_HDC
	#define __EXT_GET_WM_CTLCOLOR_HDC(wp,lp,msg) GET_WM_CTLCOLOR_HDC(wp,lp,msg)
#else
	#define __EXT_GET_WM_CTLCOLOR_HDC(wp,lp,msg) (HDC)(wp)
#endif
#ifdef GET_WM_CTLCOLOR_HWND
	#define __EXT_GET_WM_CTLCOLOR_HWND(wp,lp,msg) GET_WM_CTLCOLOR_HWND(wp,lp,msg)
#else
	#define __EXT_GET_WM_CTLCOLOR_HWND(wp,lp,msg) (HWND)(lp)
#endif
#ifdef GET_WM_CTLCOLOR_TYPE
	#define __EXT_GET_WM_CTLCOLOR_TYPE(wp,lp,msg) GET_WM_CTLCOLOR_TYPE(wp,lp,msg)
#else
	#define __EXT_GET_WM_CTLCOLOR_TYPE(wp,lp,msg) (WORD)(msg - WM_CTLCOLORMSGBOX)
#endif

template <
	class CExtWSBase
	, UINT nIdTimerSysMenuTracking = UINT(301)
	>
class CExtWS : public CExtWSBase
{
protected:
	CExtSafeString m_sSection; // section name and
	CExtSafeString m_sEntry;   // entry for save/restore
	bool m_bStyledWndInitDone : 1;
	bool m_bEnableSaveRestore : 1;
	bool m_bSysMenuTracking : 1;
	bool m_bShowResizingGripper : 1;
	CRect m_rcResizingGripper;
public:
	
	CExtWS()
		: m_bStyledWndInitDone( false )
		, m_bSysMenuTracking( false )
		, m_bEnableSaveRestore( false )
		, m_bShowResizingGripper( false )
		, m_rcResizingGripper( 0, 0, 0, 0 )
	{
	}
	
	CExtWS(
		UINT nIDTemplate,
		CWnd * pParentWnd
		)
		: CExtWSBase( nIDTemplate, pParentWnd )
	{
	}
	
	CExtWS(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd
		)
		: CExtWSBase( lpszTemplateName, pParentWnd )
	{
	}
	
	CExtWS(
			UINT nIDTemplate,
			UINT nIDCaption = 0
			)
		: CExtWSBase( nIDTemplate, nIDCaption )
	{
	}
	
	CExtWS(
			__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
			UINT nIDCaption = 0
			)
		: CExtWSBase( lpszTemplateName, nIDCaption )
	{
	}
	
	CExtWS(
		UINT nIDCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
		: CExtWSBase( nIDCaption, pParentWnd, iSelectPage )
	{
	}
	
	CExtWS(
		__EXT_MFC_SAFE_LPCTSTR pszCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
		: CExtWSBase( pszCaption, pParentWnd, iSelectPage )
	{
	}
	
	virtual void ShowSizeGrip( BOOL bShow = TRUE )
	{
		bool _bShow = bShow ? true : false;
		if( m_bShowResizingGripper != _bShow )
		{
			m_bShowResizingGripper = _bShow;
			InvalidateRect( &m_rcResizingGripper );
		}
	}

	virtual void SaveWindowRect() const
	{
		CExtSafeString strWP;
		WINDOWPLACEMENT WindowPlacement;
		ZeroMemory( &WindowPlacement, sizeof(WINDOWPLACEMENT) );
		WindowPlacement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement( &WindowPlacement );
		RECT & rc = WindowPlacement.rcNormalPosition;
		strWP.Format(
			_T("%d,%d,%d,%d,%d,%d"),
			rc.left, rc.top, rc.right, rc.bottom,
			WindowPlacement.showCmd,
			WindowPlacement.flags
			);
		::AfxGetApp()->WriteProfileString(
			__EXT_MFC_SAFE_LPCTSTR( m_sSection ),
			__EXT_MFC_SAFE_LPCTSTR( m_sEntry ),
			__EXT_MFC_SAFE_LPCTSTR( strWP )
			);
	}

	virtual void LoadWindowRect(
		bool bForceHideWindow = false
		)
	{
		CExtSafeString strWP;
		WINDOWPLACEMENT WindowPlacement;
		strWP =
			::AfxGetApp()->
				GetProfileString(
					__EXT_MFC_SAFE_LPCTSTR( m_sSection ),
					__EXT_MFC_SAFE_LPCTSTR( m_sEntry )
					);
		if( strWP.IsEmpty() )
			return;
		ZeroMemory( &WindowPlacement, sizeof(WINDOWPLACEMENT) );
		WindowPlacement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement( &WindowPlacement );
		RECT & rc = WindowPlacement.rcNormalPosition;
		if( _stscanf(
				__EXT_MFC_SAFE_LPCTSTR( strWP ),
				__EXT_MFC_SAFE_LPCTSTR( _T("%d,%d,%d,%d,%d,%d") ),
				&rc.left, &rc.top, &rc.right, &rc.bottom,
				&WindowPlacement.showCmd,
				&WindowPlacement.flags
				) == 6
			)
		{
			if( bForceHideWindow )
				WindowPlacement.showCmd = SW_HIDE;
			SetWindowPlacement( &WindowPlacement );
		}
		RedrawWindow(
			NULL, NULL,
			RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW
				|RDW_ALLCHILDREN|RDW_FRAME
			);
	}

	virtual void EnableSaveRestore(
			__EXT_MFC_SAFE_LPCTSTR pszSection,
			__EXT_MFC_SAFE_LPCTSTR pszEntry,
			bool bForceHideWindow = false
			)
	{
		m_sSection = pszSection;
		m_sEntry = pszEntry;

		m_bEnableSaveRestore = true;

		HWND hWndOwn = GetSafeHwnd();
		if(		hWndOwn != NULL
			&&	::IsWindow(hWndOwn)
			)
			LoadWindowRect( bForceHideWindow );
	}

	virtual BOOL _TrackWndSystemPopupMenu(
		CPoint * pPoint,
		BOOL bSelectAny
		)
	{
		if( (GetStyle() & WS_POPUP) == 0 )
			return FALSE;
		if(	CExtPopupMenuWnd::IsMenuTracking() )
			CExtPopupMenuWnd::CancelMenuTracking();
		m_bSysMenuTracking = false;

		CMenu * pSysMenu =
			GetSystemMenu( FALSE );
		if( pSysMenu == NULL )
			return FALSE;
		ASSERT( ::IsMenu(pSysMenu->GetSafeHmenu()) );
		CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
		if( !pPopup->CreatePopupMenu( GetSafeHwnd() ) )
		{
			ASSERT( FALSE );
			delete pPopup;
			return FALSE;
		}
		if( !pPopup->UpdateFromMenu(
				GetSafeHwnd(),
				pSysMenu,
				false
				)
			)
		{
			ASSERT( FALSE );
			delete pPopup;
			return FALSE;
		}

		CPoint point;
		if( pPoint != NULL)
			point = *pPoint;
		else
		{
			CRect rcWnd, rcClient, rcHelper;
			GetWindowRect( &rcWnd );
			GetClientRect( &rcClient );
			rcHelper = rcWnd;
			ScreenToClient( &rcHelper );
			ASSERT( rcHelper.top <= rcClient.top );
			int yDiff = rcClient.top - rcHelper.top;
			point = rcWnd.TopLeft();
			point.x += GetSystemMetrics(SM_CXFRAME);
			point.y += yDiff;
		}
		DWORD dwTrackFlags = TPMX_BOTTOMALIGN;
		if( bSelectAny )
			dwTrackFlags |= TPMX_SELECT_ANY;
		HWND hWndFocus = ::GetFocus();
		if(		hWndFocus != NULL
			&&	(!	(	hWndFocus == m_hWnd
					||	::IsChild( m_hWnd, hWndFocus )
				)	)
			)
			SetFocus();
		m_bSysMenuTracking =
			pPopup->TrackPopupMenu(
				dwTrackFlags,
				point.x,point.y,
				NULL
				) ? true : false;

		if( m_bSysMenuTracking )
			SetTimer( nIdTimerSysMenuTracking, 20, NULL );
		else
			delete pPopup;
		return m_bSysMenuTracking; //TRUE;
	}

	virtual BOOL PreTranslateMessage( MSG* pMsg )
	{
		if(		GetSafeHwnd() != NULL
			&&	::IsWindow( GetSafeHwnd() )
			&&	(	pMsg->message == WM_SYSKEYDOWN
				||	pMsg->message == WM_SYSKEYUP
				||	pMsg->message == WM_KEYDOWN
				//||	pMsg->message == WM_KEYUP
				)
			)
		{
			BOOL bSuccessRetVal = TRUE;
			if( ( GetStyle() & WS_POPUP ) == 0 )
			{
				//return FALSE;
				//return CExtWSBase::PreTranslateMessage(pMsg);
				bSuccessRetVal = FALSE;
			}
			bool bKeyUp =
				(	//pMsg->message == WM_KEYUP ||
					pMsg->message == WM_SYSKEYUP)
						? true
						: false
						;
			//BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x80000000;
			//BOOL bShift = GetKeyState(VK_SHIFT) & 0x80000000;
			BOOL bAlt = HIWORD(pMsg->lParam) & KF_ALTDOWN;
			__EXT_MFC_SAFE_TCHAR vkTCHAR = (TCHAR)pMsg->wParam;
			if( bAlt && vkTCHAR == ((TCHAR)VK_SPACE) )
			{
				if( bKeyUp )
					return bSuccessRetVal; // TRUE;
				// allow child popup track it's system menu
				CWnd * pWnd = CWnd::GetActiveWindow();
				if( pWnd == NULL
					|| (! ::IsWindow(pWnd->GetSafeHwnd()))
					)
					return bSuccessRetVal; // TRUE;
				if( pWnd->GetSafeHwnd() != GetSafeHwnd()
					&& (pWnd->GetStyle() & WS_POPUP) != 0
					)
					return bSuccessRetVal; // TRUE;
				// track dialog system menu
				if(		(GetStyle() & WS_POPUP) != 0
					&&	CExtWSBase::GetSystemMenu( FALSE ) != NULL
					)
				{
					VERIFY( _TrackWndSystemPopupMenu( NULL, TRUE ) );
				}
				return bSuccessRetVal; // TRUE;
			} // if( bAlt && vkTCHAR == ((TCHAR)VK_SPACE) )
		}

		return CExtWSBase::PreTranslateMessage(pMsg);
	}

protected:

	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		) 
	{
		if( message == WM_SIZE )
		{
			bool bForceInvalidate =
				(	(GetStyle()&WS_VISIBLE) != 0
				&&	g_PaintManager->GetCb2DbTransparentMode(this)
				&&	CExtPaintManager::stat_GetBPP() > 8
				) ? true : false ;
			if(		m_bShowResizingGripper
				&&	(! m_rcResizingGripper.IsRectEmpty() )
				&&	(! bForceInvalidate)
				)
				InvalidateRect( &m_rcResizingGripper );
			LRESULT lResult = CExtWSBase::WindowProc(message,wParam,lParam);
			if( m_bShowResizingGripper )
			{
				GetClientRect( &m_rcResizingGripper );
				m_rcResizingGripper.left = m_rcResizingGripper.right - ::GetSystemMetrics( SM_CXVSCROLL );
				m_rcResizingGripper.top = m_rcResizingGripper.bottom - ::GetSystemMetrics( SM_CYHSCROLL );
				if(! bForceInvalidate)
					InvalidateRect( &m_rcResizingGripper );
			}
			if( bForceInvalidate )
				Invalidate();
			return lResult;
		} // if( message == WM_SIZE )

		if( message == WM_WINDOWPOSCHANGED )
		{
			LRESULT lResult = CExtWSBase::WindowProc(message,wParam,lParam);
			bool bForceInvalidate =
				(	(GetStyle()&WS_VISIBLE) != 0
				&&	g_PaintManager->GetCb2DbTransparentMode(this)
				&&	CExtPaintManager::stat_GetBPP() > 8
				) ? true : false ;
			if( bForceInvalidate )
				Invalidate();
			return lResult;
		} // if( message == WM_WINDOWPOSCHANGED )

		if( message == WM_NCHITTEST )
		{
			if( m_bShowResizingGripper )
			{
				CPoint ptClient( LOWORD(lParam), HIWORD(lParam) );
				ScreenToClient( &ptClient );
				if( m_rcResizingGripper.PtInRect(ptClient) )
					return HTBOTTOMRIGHT;
			} // if( m_bShowResizingGripper )
			
			return CExtWSBase::WindowProc(message,wParam,lParam);
		} // if( message == WM_NCHITTEST )

		if( message == WM_PAINT )
		{
			CPaintDC dc( this );
			if( g_PaintManager->GetCb2DbTransparentMode(this) )
			{
				CExtPaintManager::stat_ExcludeChildAreas(
					dc,
					GetSafeHwnd(),
					CExtPaintManager::stat_DefExcludeChildAreaCallback
					);
				g_PaintManager->PaintDockerBkgnd( dc, this );
			} // if( g_PaintManager->GetCb2DbTransparentMode(this) )
			if( !m_bShowResizingGripper || IsZoomed() )
				return 0;
			g_PaintManager->PaintResizingGripper(
				dc,
				m_rcResizingGripper,
				this
				);
			g_PaintManager->OnPaintSessionComplete( this );
			return 0;
		} // if( message == WM_PAINT )

		if( message == WM_ERASEBKGND )
		{
			HDC hDC = reinterpret_cast < HDC > (wParam);
			ASSERT( hDC != NULL );
			CExtPaintManager::stat_ExcludeChildAreas(
				hDC,
				GetSafeHwnd(),
				CExtPaintManager::stat_DefExcludeChildAreaCallback
				);
			return CExtWSBase::WindowProc(message,wParam,lParam);
		} // if( message == WM_ERASEBKGND )

		if( message >= WM_CTLCOLORMSGBOX && message <= WM_CTLCOLORSTATIC )
		{
			HBRUSH hBrush = (HBRUSH)
				CExtWSBase::WindowProc(message,wParam,lParam);
			HDC hDC =
				__EXT_GET_WM_CTLCOLOR_HDC(wParam,lParam,message);
#ifdef _DEBUG
			HWND hWnd =
				__EXT_GET_WM_CTLCOLOR_HWND(wParam,lParam,message);
			ASSERT( hWnd != NULL );
#endif // _DEBUG
			INT nCtlColor =
				__EXT_GET_WM_CTLCOLOR_TYPE(wParam,lParam,message);
			if( nCtlColor == CTLCOLOR_DLG
				|| nCtlColor == CTLCOLOR_STATIC
				|| nCtlColor == CTLCOLOR_MSGBOX
				)
			{
				if( nCtlColor == CTLCOLOR_STATIC )
				{
					::SetBkMode( hDC, TRANSPARENT );
					// (+ v.2.22) corrected by the Dmitry Yakovlev's advice
					if(		(GetStyle()&WS_CHILD) != 0
						&&	g_PaintManager->GetCb2DbTransparentMode(this)
						)
						return (LRESULT)::GetStockObject( NULL_BRUSH );
				}
				static CBrush brBackground;
				static COLORREF clrLast = (COLORREF)(-1L);
				COLORREF clrNew =
					g_PaintManager->GetColor(
						CExtPaintManager::CLR_3DFACE_OUT
						);
				if( clrLast != clrNew )
				{
					if( brBackground.GetSafeHandle() != NULL )
						brBackground.DeleteObject();
				}
				if( brBackground.GetSafeHandle() == NULL )
				{
					clrLast = clrNew;
					VERIFY(
						brBackground.CreateSolidBrush( clrLast )
						);
				} // if( brBackground.GetSafeHandle() == NULL )
				return (LRESULT)brBackground.GetSafeHandle();
			}
			return (LRESULT)hBrush;
		} // if( message >= WM_CTLCOLORMSGBOX && message <= WM_CTLCOLORSTATIC )

		if( message == WM_TIMER )
		{
			if( wParam == nIdTimerSysMenuTracking )
			{
				if( !CExtPopupMenuWnd::IsMenuTracking() )
				{
					m_bSysMenuTracking = false;
					KillTimer( nIdTimerSysMenuTracking );
				}
				return 0;
			}
			return CExtWSBase::WindowProc(message,wParam,lParam);
		} // if( message == WM_TIMER )

		if( message == WM_DESTROY )
		{
			if( m_bEnableSaveRestore )
				SaveWindowRect();
			return CExtWSBase::WindowProc(message,wParam,lParam);
		}

		if( (message == WM_NCLBUTTONUP && wParam == HTSYSMENU)
			|| (message == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU))
			)
		{
			if( wParam == HTCAPTION )
			{
				CPoint point;
				if( ::GetCursorPos( &point ) )
					_TrackWndSystemPopupMenu(
						&point,
						FALSE
						);
			} // if( wParam == HTCAPTION )
			else
			{
				_TrackWndSystemPopupMenu(
					NULL,
					FALSE
					);
			} // else if if( wParam == HTCAPTION )
			return 0;
		}
		if( (message == WM_NCLBUTTONDOWN || message == WM_NCRBUTTONDOWN)
			&& wParam == HTSYSMENU
			)
			return 0;
		if( message == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
		{
			CPoint point;
			if( ::GetCursorPos( &point ) )
				_TrackWndSystemPopupMenu(
					&point,
					FALSE
					);
			return 0;
		}
		
		return CExtWSBase::WindowProc(message,wParam,lParam);
	}

	virtual void PostNcDestroy()
	{
		m_bStyledWndInitDone = false;
		CExtWSBase::PostNcDestroy();
	}

}; // class CExtWS

/////////////////////////////////////////////////////////////////////////////
// CExtWA template window extension
//
// adds child anchoring support to any window;
// anchoring idea by Paolo Messina, copyright provided:
//
//
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
//

const SIZE __RDA_NONE	= {	 -1,	 -1 };
const SIZE __RDA_LT		= {	  0,	  0 };
const SIZE __RDA_RT		= {	100,	  0 };
const SIZE __RDA_LB		= {	  0,	100 };
const SIZE __RDA_RB		= {	100,	100 };

const SIZE __RDA_KEEP	= __RDA_LT;
const SIZE __RDA_X		= __RDA_RT;
const SIZE __RDA_Y		= __RDA_LB;
const SIZE __RDA_XY		= __RDA_RB;
const SIZE __RDA_BOTH	= __RDA_RB;


template < class CExtWABase >
class CExtWA : public CExtWABase
{
protected:
	bool m_bAnchoringInitDone : 1;
	bool m_bUseMaxTrack : 1;
	bool m_bUseMinTrack : 1;
	bool m_bUseMaxRect : 1;

	CPoint m_ptMinTrackSize;	// min tracking size
	CPoint m_ptMaxTrackSize;	// max tracking size
	CPoint m_ptMaxPos;			// maximized position
	CPoint m_ptMaxSize;			// maximized size

	struct RDI_t
	{
		// upper-left corner
		SIZE m_AnchLT;
		SIZE m_MargLT;
		// bottom-right corner
		SIZE m_AnchRB;
		SIZE m_MargRB;
	
		RDI_t(
			SIZE AnchLT = __RDA_LT,
			SIZE MargLT = CSize(0,0), 
			SIZE AnchRB = __RDA_LT,
			SIZE MargRB = CSize(0,0)
			)
			: m_AnchLT( AnchLT )
			, m_MargLT( MargLT )
			, m_AnchRB( AnchRB )
			, m_MargRB( MargRB )
		{
		};
		
		RDI_t(
			const RDI_t & other
			)
			: m_AnchLT( other.m_AnchLT )
			, m_MargLT( other.m_MargLT )
			, m_AnchRB( other.m_AnchRB )
			, m_MargRB( other.m_MargRB )
		{
		};
		
		RDI_t & operator=(
			const RDI_t & other
			)
		{
			m_AnchLT = other.m_AnchLT;
			m_MargLT = other.m_MargLT;
			m_AnchRB = other.m_AnchRB;
			m_MargRB = other.m_MargRB;
			return *this;
		};

	}; // struct RDI_t

	// map of repositionable controls
	CMap < HWND, HWND, RDI_t, RDI_t > m_mapRDI;

public:
	CExtWA()
		: m_bAnchoringInitDone( false )
		, m_bUseMinTrack( true )
		, m_bUseMaxTrack( false )
		, m_bUseMaxRect( false )
		, m_ptMinTrackSize( 0, 0 )
		, m_ptMaxTrackSize( 0, 0 )
		, m_ptMaxPos( 0, 0 )
		, m_ptMaxSize( 0, 0 )
	{
	}
	
	CExtWA(
		UINT nIDTemplate,
		CWnd * pParentWnd
		)
		: CExtWABase( nIDTemplate, pParentWnd )
		, m_bAnchoringInitDone( false )
		, m_bUseMinTrack( true )
		, m_bUseMaxTrack( false )
		, m_bUseMaxRect( false )
		, m_ptMinTrackSize( 0, 0 )
		, m_ptMaxTrackSize( 0, 0 )
		, m_ptMaxPos( 0, 0 )
		, m_ptMaxSize( 0, 0 )
	{
	}
	
	CExtWA(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd
		)
		: CExtWABase( lpszTemplateName, pParentWnd )
		, m_bAnchoringInitDone( false )
		, m_bUseMinTrack( true )
		, m_bUseMaxTrack( false )
		, m_bUseMaxRect( false )
		, m_ptMinTrackSize( 0, 0 )
		, m_ptMaxTrackSize( 0, 0 )
		, m_ptMaxPos( 0, 0 )
		, m_ptMaxSize( 0, 0 )
	{
	}
	
	CExtWA(
			UINT nIDTemplate,
			UINT nIDCaption = 0
			)
		: CExtWABase( nIDTemplate, nIDCaption )
		, m_bAnchoringInitDone( false )
		, m_bUseMinTrack( true )
		, m_bUseMaxTrack( false )
		, m_bUseMaxRect( false )
		, m_ptMinTrackSize( 0, 0 )
		, m_ptMaxTrackSize( 0, 0 )
		, m_ptMaxPos( 0, 0 )
		, m_ptMaxSize( 0, 0 )
	{
	}
	
	CExtWA(
			__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
			UINT nIDCaption = 0
			)
		: CExtWABase( lpszTemplateName, nIDCaption )
		, m_bAnchoringInitDone( false )
		, m_bUseMinTrack( true )
		, m_bUseMaxTrack( false )
		, m_bUseMaxRect( false )
		, m_ptMinTrackSize( 0, 0 )
		, m_ptMaxTrackSize( 0, 0 )
		, m_ptMaxPos( 0, 0 )
		, m_ptMaxSize( 0, 0 )
	{
	}
	
	CExtWA(
		UINT nIDCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
		: CExtWABase( nIDCaption, pParentWnd, iSelectPage )
	{
	}
	
	CExtWA(
		__EXT_MFC_SAFE_LPCTSTR pszCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
		: CExtWABase( pszCaption, pParentWnd, iSelectPage )
	{
	}

	bool AddAnchor(
		UINT nDlgCtrlID,
		const CRect & rcAnch
		)
	{
		if(	GetSafeHwnd() == NULL
			|| (! ::IsWindow(GetSafeHwnd()) )
			)
		{
			ASSERT( FALSE );
			return false;
		}
		HWND hWnd = ::GetDlgItem( GetSafeHwnd(), nDlgCtrlID );
		if( hWnd == NULL || (! ::IsWindow(hWnd) ) )
		{
			ASSERT( FALSE );
			return false;
		}
		return AddAnchor( hWnd, rcAnch );
	}

	bool AddAnchor(
		HWND hWnd,
		const CRect & rcAnch
		)
	{
		if(	GetSafeHwnd() == NULL
			|| (! ::IsWindow( GetSafeHwnd() ) )
			)
		{
			ASSERT( FALSE );
			return false;
		}
		return
			AddAnchor(
				hWnd,
				CSize( rcAnch.left, rcAnch.top ),
				CSize( rcAnch.right, rcAnch.bottom )
				);
	}

	bool AddAnchor(
		UINT nDlgCtrlID,
		CSize AnchLT,
		CSize AnchRB = __RDA_NONE
		)
	{
		if( AnchLT == __RDA_NONE
			|| GetSafeHwnd() == NULL
			|| (! ::IsWindow( GetSafeHwnd() ) )
			)
		{
			ASSERT( FALSE );
			return false;
		}
		HWND hWnd = ::GetDlgItem( GetSafeHwnd(), nDlgCtrlID );
		if( hWnd == NULL || (! ::IsWindow(hWnd) ) )
		{
			ASSERT( FALSE );
			return false;
		}
		return AddAnchor( hWnd, AnchLT, AnchRB );
	}

	virtual bool AddAnchor(
		HWND hWnd,
		CSize AnchLT,
		CSize AnchRB = __RDA_NONE
		)
	{
		if( AnchLT == __RDA_NONE
			|| GetSafeHwnd() == NULL
			|| (! ::IsWindow( GetSafeHwnd() ) )
			|| (! ::IsChild( GetSafeHwnd(), hWnd ) )
			)
		{
			ASSERT( FALSE );
			return false;
		}

		CRect rcClient, rcItem;
		GetClientRect( &rcClient );
		int cx = rcClient.Width();
		int cy = rcClient.Height();
		::GetWindowRect( hWnd, &rcItem );
		ScreenToClient( &rcItem );
		
		CSize MargLT, MargRB;
		if( AnchRB == __RDA_NONE )
			AnchRB = AnchLT;
		
		// calculate margin for the top-left corner
		MargLT.cx = rcItem.left - cx*AnchLT.cx/100;
		MargLT.cy = rcItem.top - cy*AnchLT.cy/100;
		
		// calculate margin for the bottom-right corner
		MargRB.cx = rcItem.right - cx*AnchRB.cx/100;
		MargRB.cy = rcItem.bottom - cy*AnchRB.cy/100;

		// add to the list
		m_mapRDI.SetAt(
			hWnd,
			RDI_t( AnchLT, MargLT, AnchRB, MargRB )
			);

		return true;
	}

	virtual void ArrangeLayout(
		int cx = -1,
		int cy = -1
		)
	{
		if( cx < 0 || cy < 0 )
		{
			CRect rcClient;
			GetClientRect( &rcClient );
			cx = rcClient.Width();
			cy = rcClient.Height();
		} // if( cx < 0 || cy < 0 )
		HDWP _hDWP = ::BeginDeferWindowPos( (int)m_mapRDI.GetCount() );
		ASSERT( _hDWP != NULL );
		CList < HWND, HWND > listInvalidHWNDs;
		for( POSITION pos = m_mapRDI.GetStartPosition(); pos != NULL; )
		{
			HWND _hWnd;
			RDI_t _rdi;
			m_mapRDI.GetNextAssoc( pos, _hWnd, _rdi );
			ASSERT( ::IsWindow( _hWnd ) );
			if( ! ::IsWindow( _hWnd ) )
			{
				listInvalidHWNDs.AddTail(_hWnd);
				continue;
			}
			int x  = _rdi.m_MargLT.cx + ::MulDiv(cx,_rdi.m_AnchLT.cx,100);
			int y  = _rdi.m_MargLT.cy + ::MulDiv(cy,_rdi.m_AnchLT.cy,100);
			int dx = _rdi.m_MargRB.cx + ::MulDiv(cx,_rdi.m_AnchRB.cx,100) - x;
			int dy = _rdi.m_MargRB.cy + ::MulDiv(cy,_rdi.m_AnchRB.cy,100) - y;
			if( CExtPaintManager::stat_DefIsHwndNeedsDirectRepaint(_hWnd) )
			{
				::MoveWindow(
					_hWnd,
					x,
					y,
					dx,
					dy,
					TRUE
					);
				CRect rcChild(
					0,  // x,
					0,  // y,
					dx, // x + dx,
					dy  // y + dy
					);
				::RedrawWindow(
					_hWnd, // GetSafeHwnd()
					&rcChild,
					NULL,
					RDW_INVALIDATE|RDW_UPDATENOW
						|RDW_ERASE|RDW_ERASENOW
						|RDW_ALLCHILDREN
						|RDW_NOFRAME //|RDW_FRAME
						|RDW_VALIDATE
					);
				continue;
			}
			_hDWP =
				::DeferWindowPos(
					_hDWP, _hWnd, NULL, x, y, dx, dy,
					SWP_NOZORDER|SWP_NOACTIVATE
					);
			ASSERT( _hDWP != NULL );
		} // for( POSITION pos = m_mapRDI.GetStartPosition(); pos != NULL; )
		VERIFY( ::EndDeferWindowPos( _hDWP ) );
		if( listInvalidHWNDs.GetCount() > 0 )
		{
			for( pos = listInvalidHWNDs.GetHeadPosition(); pos != NULL; )
			{
				HWND hWnd = listInvalidHWNDs.GetNext(pos);
				m_mapRDI.RemoveKey( hWnd );
			}
		}
	}

	void SetMaximizedRect(const CRect& rc)
	{
		m_bUseMaxRect = true;
		m_ptMaxPos = rc.TopLeft();
		m_ptMaxSize.x = rc.Width();
		m_ptMaxSize.y = rc.Height();
	}

	void ResetMaximizedRect()
	{
		m_bUseMaxRect = false;
	}

	void SetMinTrackSize(const CSize& size)
	{
		m_bUseMinTrack = true;
		m_ptMinTrackSize.x = size.cx;
		m_ptMinTrackSize.y = size.cy;
	}

	void ResetMinTrackSize()
	{
		m_bUseMinTrack = false;
	}

	void SetMaxTrackSize(const CSize& size)
	{
		m_bUseMaxTrack = true;
		m_ptMaxTrackSize.x = size.cx;
		m_ptMaxTrackSize.y = size.cy;
	}

	void ResetMaxTrackSize()
	{
		m_bUseMaxTrack = false;
	}

protected:

	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		) 
	{

		if( message == WM_GETMINMAXINFO )
		{
			if( !m_bAnchoringInitDone )
				return 0;
			LPMINMAXINFO lpMMI =
				reinterpret_cast < LPMINMAXINFO > ( lParam );
			if( m_bUseMinTrack )
				lpMMI->ptMinTrackSize = m_ptMinTrackSize;
			if( m_bUseMaxTrack )
				lpMMI->ptMaxTrackSize = m_ptMaxTrackSize;
			if( m_bUseMaxRect )
			{
				lpMMI->ptMaxPosition = m_ptMaxPos;
				lpMMI->ptMaxSize = m_ptMaxSize;
			}
			return 0;
		} // if( message == WM_GETMINMAXINFO )

		if( message == WM_SIZE )
		{
			LRESULT lResult = CExtWABase::WindowProc(message,wParam,lParam);

			if( !m_bAnchoringInitDone )
			{
				// gets the template size as the min track size
				CRect rc;
				GetWindowRect( &rc );
				m_ptMinTrackSize.x = rc.Width();
				m_ptMinTrackSize.y = rc.Height();

				m_bAnchoringInitDone = true;

				ArrangeLayout();
				return lResult;
			} // if( !m_bAnchoringInitDone )
			
			
			if( wParam == SIZE_MAXHIDE || wParam == SIZE_MAXSHOW )
				return lResult; // arrangement not needed

			if( m_bAnchoringInitDone )
				ArrangeLayout( LOWORD(lParam), HIWORD(lParam) );
			return lResult;
		} // if( message == WM_SIZE )
		
		
		return CExtWABase::WindowProc(message, wParam, lParam);
	}

}; // class CExtWAWnd

/////////////////////////////////////////////////////////////////////////////
// CExtADLG template adaptor for CDialog

template < class CExtADLGBase >
class CExtADLG : public CExtADLGBase
{
public:
	CExtADLG()
	{
	}
	
	CExtADLG(
		UINT nIDTemplate,
		CWnd * pParentWnd
		)
		: CExtADLGBase( nIDTemplate, pParentWnd )
	{
	}
	
	CExtADLG(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd
		)
		: CExtADLGBase( lpszTemplateName, pParentWnd )
	{
	}
	
	CExtADLG(
			UINT nIDTemplate,
			UINT nIDCaption = 0
			)
	{
		// no such constructor in CDialog
		nIDTemplate;
		nIDCaption;
		ASSERT( FALSE );
	}
	
	CExtADLG(
			__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
			UINT nIDCaption = 0
			)
	{
		// no such constructor in CDialog
		lpszTemplateName;
		nIDCaption;
		ASSERT( FALSE );
	}
	
	CExtADLG(
		UINT nIDCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
	{
		// no such constructor in CDialog
		nIDCaption;
		pParentWnd;
		iSelectPage;
		ASSERT( FALSE );
	}
	
	CExtADLG(
		__EXT_MFC_SAFE_LPCTSTR pszCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
	{
		// no such constructor in CDialog
		pszCaption;
		pParentWnd;
		iSelectPage;
		ASSERT( FALSE );
	}
}; // class CExtADLG

/////////////////////////////////////////////////////////////////////////////
// CExtAPSH template adaptor for CPropertySheet

template < class CExtAPSHBase >
class CExtAPSH : public CExtAPSHBase
{
public:
	CExtAPSH()
	{
	}
	
	CExtAPSH(
		UINT nIDTemplate,
		CWnd * pParentWnd
		)
	{
		// no such constructor in CPropertySheet
		nIDTemplate;
		pParentWnd;
		ASSERT( FALSE );
	}
	
	CExtAPSH(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd
		)
	{
		// no such constructor in CPropertySheet
		lpszTemplateName;
		pParentWnd;
		ASSERT( FALSE );
	}
	
	CExtAPSH(
			UINT nIDTemplate,
			UINT nIDCaption = 0
			)
	{
		// no such constructor in CPropertySheet
		nIDTemplate;
		nIDCaption;
		ASSERT( FALSE );
	}
	
	CExtAPSH(
			__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
			UINT nIDCaption = 0
			)
	{
		// no such constructor in CPropertySheet
		lpszTemplateName;
		nIDCaption;
		ASSERT( FALSE );
	}
	
	CExtAPSH(
		UINT nIDCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
		: CExtAPSHBase( nIDCaption, pParentWnd, iSelectPage )
	{
	}
	
	CExtAPSH(
		__EXT_MFC_SAFE_LPCTSTR pszCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
		: CExtAPSHBase( pszCaption, pParentWnd, iSelectPage )
	{
	}
}; // class CExtAPSH

/////////////////////////////////////////////////////////////////////////////
// CExtAPPG template adaptor for CPropertyPage

template < class CExtAPPGBase >
class CExtAPPG : public CExtAPPGBase
{
public:
	CExtAPPG()
	{
	}
	
	CExtAPPG(
		UINT nIDTemplate,
		CWnd * pParentWnd
		)
	{
		// no such constructor in CPropertyPage
		nIDTemplate;
		pParentWnd;
		ASSERT( FALSE );
	}
	
	CExtAPPG(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd
		)
	{
		// no such constructor in CPropertyPage
		lpszTemplateName;
		pParentWnd;
		ASSERT( FALSE );
	}
	
	CExtAPPG(
			UINT nIDTemplate,
			UINT nIDCaption = 0
			)
		: CExtAPPGBase( nIDTemplate, nIDCaption )
	{
	}
	
	CExtAPPG(
			__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
			UINT nIDCaption = 0
			)
		: CExtAPPGBase( lpszTemplateName, nIDCaption )
	{
	}
	
	CExtAPPG(
		UINT nIDCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
	{
		// no such constructor in CPropertyPage
		nIDCaption;
		pParentWnd;
		iSelectPage;
		ASSERT( FALSE );
	}
	
	CExtAPPG(
		__EXT_MFC_SAFE_LPCTSTR pszCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
	{
		// no such constructor in CPropertyPage
		pszCaption;
		pParentWnd;
		iSelectPage;
		ASSERT( FALSE );
	}
}; // class CExtAPPG

/////////////////////////////////////////////////////////////////////////////
// CExtAGW template adaptor for generic window classes

template < class CExtAGWBase >
class CExtAGW : public CExtAGWBase
{
public:
	CExtAGW()
	{
	}
	
	CExtAGW(
		UINT nIDTemplate,
		CWnd * pParentWnd
		)
	{
		// should not have such constructor
		nIDTemplate;
		pParentWnd;
		ASSERT( FALSE );
	}
	
	CExtAGW(
		__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
		CWnd * pParentWnd
		)
	{
		// should not have such constructor
		lpszTemplateName;
		pParentWnd;
		ASSERT( FALSE );
	}
	
	CExtAGW(
			UINT nIDTemplate,
			UINT nIDCaption = 0
			)
		: CExtAGWBase( nIDTemplate, nIDCaption )
	{
		// should not have such constructor
		nIDTemplate;
		nIDCaption;
		ASSERT( FALSE );
	}
	
	CExtAGW(
			__EXT_MFC_SAFE_LPCTSTR lpszTemplateName,
			UINT nIDCaption = 0
			)
		: CExtAGWBase( lpszTemplateName, nIDCaption )
	{
		// should not have such constructor
		lpszTemplateName;
		nIDCaption;
		ASSERT( FALSE );
	}
	
	CExtAGW(
		UINT nIDCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
	{
		// should not have such constructor
		nIDCaption;
		pParentWnd;
		iSelectPage;
		ASSERT( FALSE );
	}
	
	CExtAGW(
		__EXT_MFC_SAFE_LPCTSTR pszCaption,
		CWnd *pParentWnd,
		UINT iSelectPage
		)
	{
		// should not have such constructor
		pszCaption;
		pParentWnd;
		iSelectPage;
		ASSERT( FALSE );
	}
}; // class CExtAGW

#endif // __EXT_TEMPL_H

