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

// extcontrolbar.cpp : implementation file
//

#include "stdafx.h"

//
// Many thanks to Paul DiLascia & Cristi Posea, their works
// were a powerful incentive to learn more about pretty
// complicated MFC docking windows mechanism
//

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	#if (!defined __EXT_CONTROLBAR_TABBED_FEATURES_H)
		#include "ExtControlBarTabbedFeatures.h"
	#endif // __EXT_CONTROLBAR_TABBED_FEATURES_H
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

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

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif 

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#ifndef __AFXSTATE_H__
		#include <../src/mfc/afxstat_.h>
	#endif
	#include <../src/mfc/AfxImpl.h>
#endif

#ifndef __AFXOLE_H__
	#include <AfxOle.h>
#endif 

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	#if (!defined __EXTCUSTOMIZE_H)
		#include <ExtCustomize.h>
	#endif
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define __DOCKSITE_DCX_FLAGS \
	( DCX_WINDOW \
		|DCX_CACHE \
		|DCX_LOCKWINDOWUPDATE \
		|DCX_CLIPSIBLINGS \
	)

#define __PLACEHODLER_BAR_PTR( __CB__ ) \
	( (HIWORD(__CB__) ) == 0 )

#define __BAR_NC_GAP_X 1 //afxData.cxBorder2
#define __BAR_NC_GAP_Y 1 //afxData.cyBorder2

#define __DOCKING_SIDE_GAP 10

static HCURSOR g_hCursorArrow	= ::LoadCursor(NULL,IDC_ARROW);
static HCURSOR g_hCursorDrag	= ::LoadCursor(NULL,IDC_SIZEALL);
static HCURSOR g_hCursorResizeV	= ::LoadCursor(NULL,IDC_SIZENS);
static HCURSOR g_hCursorResizeH	= ::LoadCursor(NULL,IDC_SIZEWE);

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar::FriendlyDockBarHack

class CExtControlBar::FriendlyDockBarHack
	: CDockBar
{
public:
	friend class CExtControlBar;
	friend class CExtControlBar::InternalDraggingState_t;

	INT GetCalcExtentMax( BOOL bHorz )
	{
		ASSERT( this != NULL );
		ASSERT_VALID( this );
		ASSERT( m_pDockSite != NULL );
		ASSERT_VALID( m_pDockSite );
		ASSERT_KINDOF( CDockBar, this );
		INT nCalcExtentMax = bHorz
			? m_rectLayout.Width()
			: m_rectLayout.Height();
		ASSERT( nCalcExtentMax >= 0 );
		if( nCalcExtentMax == 0
			|| m_rectLayout.IsRectEmpty()
			)
		{
			CRect rcDock;
			GetClientRect( &rcDock );
			//m_pDockSite->GetClientRect( &rcDock );
			nCalcExtentMax = bHorz
				? rcDock.Width()  // + 2
				: rcDock.Height() // - 2
				;
			//ASSERT( nCalcExtentMax >= 0 );
			if( nCalcExtentMax <= 0 )
				return 3; // dummy choice
		} // if( nCalcExtentMax == 0 )
		return nCalcExtentMax;
	}

	void _InsertBar( INT nPos, CControlBar * pBar )
	{
		ASSERT_VALID( this );
		ASSERT( nPos >= 0 && nPos < m_arrBars.GetSize() );
		m_arrBars.InsertAt( nPos, pBar );
		ASSERT_VALID( this );
#ifdef _DEBUG
		if( pBar != NULL )
		{
			ASSERT_BAR_LOCATION_IN_ROW( this, pBar );
		}
#endif // _DEBUG
	}
	
	void _InsertRowSplitter( INT nPos )
	{
		ASSERT_VALID( this );
		_InsertBar( nPos, NULL );
		ASSERT_VALID( this );
	}

}; // class CExtControlBar::FriendlyDockBarHack

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar::POPUP_MENU_EVENT_DATA

CExtControlBar::POPUP_MENU_EVENT_DATA::POPUP_MENU_EVENT_DATA(
	SHORT nHelperNotificationType,
	CExtPopupMenuWnd * pPopupMenuWnd,
	CWnd * pWndEventSrc,
	const POINT & ptScreen,
	UINT nMsgID, // = WM_NULL
	CObject * pObjectSrc // = NULL
	)
	: m_nHelperNotificationType( nHelperNotificationType )
	, m_pPopupMenuWnd( pPopupMenuWnd )
	, m_pWndEventSrc( pWndEventSrc )
	, m_ptScreen( ptScreen )
	, m_nMsgID( nMsgID )
	, m_pObjectSrc( pObjectSrc )
	, m_pWndInitialTarget( NULL )
	, m_bPostNotification( false )
{
	ASSERT_VALID( m_pPopupMenuWnd );
	ASSERT_VALID( m_pWndEventSrc );
	ASSERT( m_pWndEventSrc->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(m_pWndEventSrc->GetSafeHwnd()) );
	if( m_ptScreen.x < 0 || m_ptScreen.y < 0 )
	{
		if( ! ::GetCursorPos( &m_ptScreen ) )
			m_ptScreen.x = m_ptScreen.y = 0;
	}
}

CExtControlBar::POPUP_MENU_EVENT_DATA::~POPUP_MENU_EVENT_DATA()
{
}

bool CExtControlBar::POPUP_MENU_EVENT_DATA::IsControlBarMenuListNotification() const
{
	switch( m_nHelperNotificationType )
	{
	case __PMED_DOCKBAR_CTX:
	case __PMED_CONTROLBAR_CTX:
	case __PMED_CONTROLBAR_NC_CTX:
	case __PMED_STATUSBAR_CTX:
	case __PMED_AUTOHIDESLIDER_CTX:
	case __PMED_MINIFRAME_NC_CTX:
	case __PMED_MDICLIAREA_CTX:
	case __PMED_CONTROLBAR_NCBTNMENU_BARS:
	case __PMED_CTXEXPBTN_BARS:
		return true;
	} // switch( m_nHelperNotificationType )
	return false;
}

CExtControlBar::POPUP_MENU_EVENT_DATA::operator WPARAM() const
{
	return ( (WPARAM) this );
}

CExtControlBar::POPUP_MENU_EVENT_DATA *
	CExtControlBar::POPUP_MENU_EVENT_DATA::FromWParam( WPARAM wParam )
{
	return ( (POPUP_MENU_EVENT_DATA *) wParam );
}

CExtControlBar::POPUP_MENU_EVENT_DATA *
	CExtControlBar::POPUP_MENU_EVENT_DATA::FromMSG( MSG * pMSG )
{
	ASSERT( pMSG != NULL );
	ASSERT( pMSG->message == CExtControlBar::g_nMsgConstructPopupMenu );
	return FromWParam( pMSG->wParam );
}

bool CExtControlBar::POPUP_MENU_EVENT_DATA::NotifyTarget(
	bool bPostNotification
	)
{
	ASSERT_VALID( m_pPopupMenuWnd );
	ASSERT_VALID( m_pWndEventSrc );
	ASSERT( m_pWndEventSrc->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(m_pWndEventSrc->GetSafeHwnd()) );
	m_bPostNotification = bPostNotification;
CControlBar * pBar = DYNAMIC_DOWNCAST( CControlBar, m_pWndEventSrc );
	if( pBar != NULL )
		m_pWndInitialTarget = pBar->m_pDockSite;
	if( m_pWndInitialTarget == NULL )
	{
		m_pWndInitialTarget =  m_pWndEventSrc->GetParentFrame();
		if( m_pWndInitialTarget == NULL )
		{
			m_pWndInitialTarget = m_pWndEventSrc->GetParent();
			ASSERT_VALID( m_pWndInitialTarget );
		}
		else if( m_pWndInitialTarget->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
		{
			m_pWndInitialTarget = m_pWndInitialTarget->GetParentFrame();
			ASSERT_VALID( m_pWndInitialTarget );
			ASSERT( ! m_pWndInitialTarget->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
		}
	}
	ASSERT_VALID( m_pWndInitialTarget );
	ASSERT( m_pWndInitialTarget->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(m_pWndInitialTarget->GetSafeHwnd()) );
LRESULT lResult =
		m_pWndInitialTarget->SendMessage(
			CExtControlBar::g_nMsgConstructPopupMenu,
			*this
			);
	return (lResult != 0) ? true : false;
}


/////////////////////////////////////////////////////////////////////////////
// CExtControlBar

IMPLEMENT_DYNCREATE( CExtControlBar, CControlBar )

CCriticalSection CExtControlBar::g_csCB;
ExtControlBarVector_t CExtControlBar::g_AllBars;
CExtControlBar::InternalDockBarInfo_t CExtControlBar::g_DockBarInfo;
bool CExtControlBar::g_bEnableOnIdleCalls = false;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
bool CExtControlBar::g_bAllowAutohideTabsOleDropTarget = true;
bool CExtControlBar::g_bAllowDynamicTabsOleDropTarget = true;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

const UINT CExtControlBar::g_nMsgQueryRepositionCalcEffect =
	::RegisterWindowMessage(
		_T("CExtControlBar::g_nMsgQueryRepositionCalcEffect")
		);
const UINT CExtControlBar::g_nMsgQueryIcon =
	::RegisterWindowMessage(
		_T("CExtControlBar::g_nMsgQueryIcon")
		);
const UINT CExtControlBar::g_nMsgPutToPopupMenu =
	::RegisterWindowMessage(
		_T("CExtControlBar::g_nMsgPutToPopupMenu")
		);
const UINT CExtControlBar::g_nMsgConstructPopupMenu =
	::RegisterWindowMessage(
		_T("CExtControlBar::g_nMsgConstructPopupMenu")
		);
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
const UINT CExtControlBar::g_nMsgCreateTabbedBar =
	::RegisterWindowMessage(
		_T("CExtControlBar::g_nMsgCreateTabbedBar")
		);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)


static CExtSafeString productsection2regkeypath(
	__EXT_MFC_SAFE_LPCTSTR sProfileName,
	__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	return CExtCmdManager::GetSubSystemRegKeyPath(
		__PROF_UIS_REG_CONTROL_BAR,
		sProfileName,
		sSectionNameCompany,
		sSectionNameProduct
		);
}

CExtControlBar::InternalDockBarInfo_t::InternalDockBarInfo_t()
{
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_TOP,
		CBRS_TOP
		);
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_BOTTOM,
		CBRS_BOTTOM
		);
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_LEFT,
		CBRS_LEFT
		);
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_RIGHT,
		CBRS_RIGHT
		);
}

DWORD CExtControlBar::InternalDockBarInfo_t::operator[]( UINT nDockBarID )
{
	DWORD dwStyle = 0;
	VERIFY( m_mapDockBarInfo.Lookup(nDockBarID,dwStyle) );
	return dwStyle;
}

bool CExtControlBar::InternalDockBarInfo_t::ReplaceFrameDockBar(
	UINT nDockBarID,
	DWORD dwDockStyle,
	CFrameWnd * pFrame
	)
{
	ASSERT_VALID(pFrame);

	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);

DWORD dwDockBarStyle = (*this)[nDockBarID];
	if( (dwDockBarStyle&dwDockStyle&CBRS_ALIGN_ANY) == 0 )
		return false;
	
CDockBar * pDockBar = (CDockBar*)
		pFrame->GetControlBar(nDockBarID);
	ASSERT_VALID( pDockBar );
	ASSERT_KINDOF( CDockBar, pDockBar );
	if( pDockBar->IsKindOf(
			RUNTIME_CLASS(CExtDockOuterBar)
			)
		)
		return true;

BOOL bExplicitDelete = !pDockBar->m_bAutoDelete;
	pDockBar->m_pDockSite->RemoveControlBar( pDockBar );
	pDockBar->m_pDockSite = NULL;
	pDockBar->DestroyWindow();
	if( bExplicitDelete )
		delete pDockBar;
	pDockBar = new CExtDockOuterBar();
	DWORD dwStyle =
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN
		|dwDockBarStyle;
	if(	!pDockBar->Create( pFrame, dwStyle, nDockBarID ) )
	{
		ASSERT( FALSE );
		return false;
	}

	return true;
}

CExtControlBar::CExtControlBar()
	: m_bGripperStaticallyAtTop( true )
	, m_bAllowContractExpand( true )
	, m_bDoNotEraseClientBackground( false )
	, m_bPresubclassDialogMode( false )
	, m_bReflectParentSizing( true )
	, m_bInCustomModeUpdateCmdUI( false )
	, m_bFixedMode( false )
	, m_bAppearInDockSiteControlBarPopupMenu( true )
	, m_nGripHeightAtTop( ::GetSystemMetrics(SM_CYSMCAPTION)+1 )
	, m_nGripWidthAtLeft( ::GetSystemMetrics(SM_CYSMCAPTION)+1 )
	, m_nSeparatorHeight( min( 4, ::GetSystemMetrics(SM_CYSIZEFRAME) ) )
	, m_nSeparatorWidth(  min( 4, ::GetSystemMetrics(SM_CXSIZEFRAME) ) )
	, m_nMinHW( ::GetSystemMetrics(SM_CYSMCAPTION)+1 + m_nSeparatorHeight )
	, m_nMinVH( ::GetSystemMetrics(SM_CYSMCAPTION)+1 + m_nSeparatorHeight )
	, m_nTrackerOffset( 0 )
	, m_nMetricOffset( 0 )
	, m_sizeDockedH( 200, 150 )
	, m_sizeDockedV( 150, 200 )
	, m_sizeFloated( 200, 200 )
	, m_bRowResizing( false )
	, m_bRowRecalcing( false )
	, m_bDragging( false )
	, m_bReposSingleChildMode( true )
	, m_ptHelperDragOffsetFloated( 0, 0 )
	, m_ptHelperDragOffsetDockedH( 0, 0 )
	, m_ptHelperDragOffsetDockedV( 0, 0 )
	, m_ptFloatHelper( 0, 0 )
	, m_bWindowActive( false )
	, m_bTopRecalcing( false )
	, m_bNcAreaBtnTracking( false )
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	, m_bAutoHideMode( false )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	, m_ptDraggingLast( -1, -1 )
	, m_bUpdatingChain( false )
	, m_nDelayedRowUpdateMetric( 0 )
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	, m_bVisibleInCustomizeListBox( false )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
{
	VERIFY( RegisterControlBarClass() );

CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
	g_AllBars.Add( this );
	_slCsCB.Unlock();
}

CExtControlBar::~CExtControlBar()
{
CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
INT nCountOfExtBars = g_AllBars.GetSize();
	for( INT nExtBar = 0; nExtBar < nCountOfExtBars; nExtBar++ )
	{
		CExtControlBar * pBar = g_AllBars[ nExtBar ];
		ASSERT( pBar != NULL );
		if( pBar == this )
		{
			g_AllBars.RemoveAt( nExtBar );
			break;
		}
	} // for( INT nExtBar = 0; nExtBar < nCountOfExtBars; nExtBar++ )
	_slCsCB.Unlock();

	NcButtons_RemoveAll();
}


BEGIN_MESSAGE_MAP(CExtControlBar, CControlBar)
	//{{AFX_MSG_MAP(CExtControlBar)
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
    ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCRBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_SYSCOLORCHANGE()
	__EXT_MFC_SAFE_ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(__ExtMfc_WM_THEMECHANGED, OnThemeChanged)
END_MESSAGE_MAP()


BOOL CExtControlBar::Create(
	__EXT_MFC_SAFE_LPCTSTR lpszWindowName,
	CWnd * pParentWnd,
    UINT nID, // = AFX_IDW_DIALOGBAR
	DWORD dwStyle // =
		//WS_CHILD|WS_VISIBLE
		//|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS
		//|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
		//|CBRS_HIDE_INPLACE
	)
{
    ASSERT_VALID( pParentWnd );
 
	if( !RegisterControlBarClass() )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	
	ASSERT(
		!((dwStyle & CBRS_SIZE_FIXED)
		&&
        (dwStyle & CBRS_SIZE_DYNAMIC))
		);
    m_dwStyle = dwStyle & CBRS_ALL;

CExtSafeString sWindowName(
		(lpszWindowName != NULL)
			? lpszWindowName : _T("")
		);
	if( lpszWindowName == NULL && nID != 0 && nID != UINT(-1) )
	{
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( pParentWnd->GetSafeHwnd() ),
				nID
				);
		if( pCmdItem != NULL )
		{
			sWindowName = pCmdItem->m_sMenuText;
			sWindowName.Replace( _T("&"), _T("") );
		}
		else
		{
			if( !sWindowName.LoadString(nID	) )
				sWindowName = _T("");
		}
	}
	if( !sWindowName.IsEmpty() )
	{
		int nNlPos = sWindowName.Find( _T('\n') );
		if( nNlPos >= 0 )
			sWindowName = sWindowName.Left( nNlPos );
	}
    
	dwStyle &= ~CBRS_ALL;
    dwStyle |= WS_CLIPCHILDREN;
CExtSafeString sWndClassName(
		::AfxRegisterWndClass(
			CS_DBLCLKS,
			g_hCursorArrow,
			g_PaintManager->GetBrush( COLOR_BTNFACE ),
			0
			)
		);

	if(	!CWnd::Create(
			sWndClassName,
			sWindowName.IsEmpty() ? _T("") : sWindowName,
			dwStyle,
			CRect( 0, 0, 0, 0 ),
			pParentWnd,
			nID
			)
		)
	{
		ASSERT( FALSE );
        return FALSE;
	}
    return TRUE;
}

bool CExtControlBar::_SetupFloatingFrameWndImpl(
	CFrameWnd * pFrame
	)	
{
	ASSERT( pFrame != NULL );
	return
		((CExtControlBar::InternalFriendlyFrameWnd *)pFrame)->
			SetupSmilyFrameWnd();
}

bool CExtControlBar::_FrameEnableDockingImpl(
	CFrameWnd * pFrame,
	DWORD dwDockStyle, // = CBRS_ALIGN_ANY
	bool bReplaceFloatingWnd // = true
	) 
{
	// NOTE: this rule is specific for current implementation
	ASSERT( dwDockStyle == CBRS_ALIGN_ANY );

	ASSERT_VALID(pFrame);
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);

	pFrame->EnableDocking( dwDockStyle );
	
	if(	!g_DockBarInfo.ReplaceFrameDockBar(
			AFX_IDW_DOCKBAR_TOP,
			dwDockStyle,
			pFrame
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if(	!g_DockBarInfo.ReplaceFrameDockBar(
			AFX_IDW_DOCKBAR_BOTTOM,
			dwDockStyle,
			pFrame
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if(	!g_DockBarInfo.ReplaceFrameDockBar(
			AFX_IDW_DOCKBAR_LEFT,
			dwDockStyle,
			pFrame
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if(	!g_DockBarInfo.ReplaceFrameDockBar(
			AFX_IDW_DOCKBAR_RIGHT,
			dwDockStyle,
			pFrame
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}

	if( bReplaceFloatingWnd )
	{
		if( !_SetupFloatingFrameWndImpl( pFrame ) )
		{
			ASSERT( FALSE );
			return false;
		}
	}

	return true;
}

bool CExtControlBar::FrameEnableDocking(
	CFrameWnd * pFrame,
	DWORD dwDockStyle, // = CBRS_ALIGN_ANY
	bool bReplaceFloatingWnd // = true
	) 
{
	return
		_FrameEnableDockingImpl(
			pFrame,
			dwDockStyle,
			bReplaceFloatingWnd
			);
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
bool CExtControlBar::FrameInjectAutoHideAreas(
	CFrameWnd * pFrame
	) 
{
	return _FrameInjectAutoHideAreasImpl( pFrame );
}

bool CExtControlBar::_FrameInjectAutoHideAreasImpl(
	CFrameWnd * pFrame
	) 
{
	ASSERT_VALID( pFrame );
	ASSERT( pFrame->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );

CExtDynAutoHideSlider * pWndSlider = new CExtDynAutoHideSlider;
	if(	!pWndSlider->Create( pFrame ) )
	{
		ASSERT( FALSE );
		return false;
	}

CExtDynAutoHideArea * pWndAutoHideArea =
		new CExtDynAutoHideArea();
	if(	!pWndAutoHideArea->Create(
			pFrame,
			pWndSlider,
			AFX_IDW_DOCKBAR_TOP
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}

	pWndAutoHideArea =
		new CExtDynAutoHideArea();
	if(	!pWndAutoHideArea->Create(
			pFrame,
			pWndSlider,
			AFX_IDW_DOCKBAR_BOTTOM
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	
	pWndAutoHideArea =
		new CExtDynAutoHideArea();
	if(	!pWndAutoHideArea->Create(
			pFrame,
			pWndSlider,
			AFX_IDW_DOCKBAR_LEFT
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	
	pWndAutoHideArea =
		new CExtDynAutoHideArea();
	if(	!pWndAutoHideArea->Create(
			pFrame,
			pWndSlider,
			AFX_IDW_DOCKBAR_RIGHT
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}

	pFrame->DelayRecalcLayout();
	return true;
}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

bool CExtControlBar::IsFixedMode() const
{
	return m_bFixedMode;
}

bool CExtControlBar::IsFixedDockStyle() const
{
	return IsFixedMode();
}

bool CExtControlBar::ProfileBarStateLoad(
	CFrameWnd * pFrame,
	__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
	LPWINDOWPLACEMENT pFrameWp // = NULL // need serialize frame's WP
	)
{
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
	ASSERT( sSectionNameProfile != NULL );
	ASSERT_VALID( pFrame );

__EXT_MFC_SAFE_LPCTSTR strProfileName =
		g_CmdManager->ProfileNameFromWnd(
			pFrame->GetSafeHwnd()
			);
	if( pFrame == NULL
		|| strProfileName == NULL
		|| strProfileName[0] == _T('\0')
		)
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT_VALID( pFrame );

//__EXT_MFC_SAFE_LPCTSTR lpszProfileName =
//		AfxGetApp()->m_pszProfileName;
//	ASSERT( lpszProfileName != NULL );
//	ASSERT( lpszProfileName[0] != _T('\0') );

CExtSafeString sRegKeyPath=
		productsection2regkeypath(
			sSectionNameProfile,
			sSectionNameCompany,
			sSectionNameProduct
			);

bool bRetVal = false;
	try
	{
		// prepare memory file and archive,
		// get information from registry
		CMemFile _file;
		if( !CExtCmdManager::FileObjFromRegistry(
				_file,
				sRegKeyPath
				)
			)
		{
			// win xp fix - begin
			WINDOWPLACEMENT _wpf;
			::memset( &_wpf, 0, sizeof(WINDOWPLACEMENT) );
			_wpf.length = sizeof(WINDOWPLACEMENT);
			if( pFrame->GetWindowPlacement(&_wpf) )
			{
				_wpf.ptMinPosition.x = _wpf.ptMinPosition.y = 0;
				_wpf.ptMaxPosition.x = _wpf.ptMaxPosition.y = 0;
				_wpf.showCmd = 
					(pFrame->GetStyle() & WS_VISIBLE)
						? SW_SHOWNA
						: SW_HIDE;
				pFrame->SetWindowPlacement(&_wpf);
			} // if( pFrame->GetWindowPlacement(&_wpf) )
			// win xp fix - end
			return false;
		}
		CArchive ar(
			&_file,
			CArchive::load
			);
		// do serialization
		bRetVal =
			ProfileBarStateSerialize(
				ar,
				pFrame,
				pFrameWp
				);
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	return bRetVal;
}

bool CExtControlBar::ProfileBarStateSave(
	CFrameWnd * pFrame,
	__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%\%sSectionNameProfile%
	LPWINDOWPLACEMENT pFrameWp // = NULL // need serialize frame's WP
	)
{
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
	ASSERT( sSectionNameProfile != NULL );
	ASSERT_VALID( pFrame );

__EXT_MFC_SAFE_LPCTSTR strProfileName =
		g_CmdManager->ProfileNameFromWnd(
			pFrame->GetSafeHwnd()
			);
	if( pFrame == NULL
		|| strProfileName == NULL
		|| strProfileName[0] == _T('\0')
		)
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT_VALID( pFrame );

//__EXT_MFC_SAFE_LPCTSTR lpszProfileName =
//		AfxGetApp()->m_pszProfileName;
//	ASSERT( lpszProfileName != NULL );
//	ASSERT( lpszProfileName[0] != _T('\0') );
CExtSafeString sRegKeyPath =
		productsection2regkeypath(
			sSectionNameProfile,
			sSectionNameCompany,
			sSectionNameProduct
			);

bool bRetVal = false;
	try
	{
		// prepare memory file and archive
		CMemFile _file;
		CArchive ar(
			&_file,
			CArchive::store
			);
		// do serialization
		ProfileBarStateSerialize(
			ar,
			pFrame,
			pFrameWp
			);
		// OK, serialization passed
		ar.Flush();
		ar.Close();
		// put information to registry
		bRetVal =
			CExtCmdManager::FileObjToRegistry(
				_file,
				sRegKeyPath
				);
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )

	return bRetVal;
}

bool CExtControlBar::InternalFriendlyFrameWnd::SetupSmilyFrameWnd(void)
{
	m_pFloatingFrameClass =
		RUNTIME_CLASS(CExtMiniDockFrameWnd);

	return true;
}

void CExtControlBar::InternalFriendlyFrameWnd::SetOuterDockState(
	const CDockState & state,
	const CExtControlBar::OuterPropsArr_t & arrOuterProps
	)
{
	// first pass through barinfo's sets the m_pBar member correctly
	// creating floating frames if necessary
	for( int i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
	{
		CControlBarInfo * pInfo = (CControlBarInfo *)
			state.m_arrBarInfo[i];
		ASSERT( pInfo != NULL );
		if( pInfo->m_bFloating )
		{
			// need to create floating frame to match
			CMiniDockFrameWnd * pDockFrame =
				CreateFloatingFrame(
					pInfo->m_bHorz ?
						CBRS_ALIGN_TOP : CBRS_ALIGN_LEFT
					);
			ASSERT( pDockFrame != NULL) ;
			CRect rcWnd( pInfo->m_pointPos, CSize(10, 10) );
			pDockFrame->CalcWindowRect( &rcWnd );
			pDockFrame->SetWindowPos(
				NULL,
				rcWnd.left, rcWnd.top, 0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE
				);
			CDockBar * pDockBar = (CDockBar *)
				pDockFrame->GetDlgItem( AFX_IDW_DOCKBAR_FLOAT );
			ASSERT( pDockBar != NULL );
			ASSERT_KINDOF( CDockBar, pDockBar );
			pInfo->m_pBar = pDockBar;
		} // if( pInfo->m_bFloating )
		else // regular dock bar or toolbar
		{
			pInfo->m_pBar = GetControlBar( pInfo->m_nBarID );
			if( pInfo->m_pBar == NULL )
				continue;
		} // else from if( pInfo->m_bFloating )
		pInfo->m_pBar->m_nMRUWidth = pInfo->m_nMRUWidth;
	} // for( int i = 0; i < state.m_arrBarInfo.GetSize(); i++ )

	// update new outer props
	for( i = 0; i < arrOuterProps.GetSize(); i++ )
	{
		OuterItemData_t _oid = arrOuterProps[i];
		CExtControlBar * pExtBar =
			STATIC_DOWNCAST(
				CExtControlBar,
				GetControlBar( _oid.m_nBarID )
				);
		pExtBar->m_ptFloatHelper = _oid.m_ptFloatHelper;
		pExtBar->m_sizeDockedH = _oid.m_sizeDockedH;
		pExtBar->m_sizeDockedV = _oid.m_sizeDockedV;
		pExtBar->m_sizeFloated = _oid.m_sizeFloated;
		pExtBar->_AffixmentSetOuter( &_oid.m_AffixmentData );
	} // for( i = 0; i < arrOuterProps.GetSize(); i++ )

	// the second pass will actually dock all of the control bars and
	// set everything correctly
	for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
	{
		CControlBarInfo * pInfo = (CControlBarInfo *)
			state.m_arrBarInfo[i];
		ASSERT( pInfo != NULL );
		if( pInfo->m_pBar == NULL )
			continue;
		if( pInfo->m_pBar->m_pDockContext != NULL
			&& pInfo->m_pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
			)
			((CExtControlBar*)(pInfo->m_pBar))->m_ptFloatHelper
				= pInfo->m_ptMRUFloatPos;
		pInfo->m_pBar->SetBarInfo( pInfo, this );
	} // for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )

	// the third pass will move independent floating bars
	// into valid positions
	for( i = 0; i < arrOuterProps.GetSize(); i++ )
	{
		OuterItemData_t _oid = arrOuterProps[i];
		CExtControlBar * pExtBar =
			STATIC_DOWNCAST(
				CExtControlBar,
				GetControlBar( _oid.m_nBarID )
				);
		if( !_oid.m_bFloating )
			continue;
		CRect rcFrameInitial( _oid.m_ptFloatHelper, _oid.m_sizeFloated );
		CRect rcFrameNew =
			CExtPaintManager::stat_AlignWndRectToMonitor(
				rcFrameInitial
				);
		CExtMiniDockFrameWnd * pMiniFrame =
			DYNAMIC_DOWNCAST(
				CExtMiniDockFrameWnd,
				pExtBar->GetParentFrame()
				);
		if( pMiniFrame != NULL )
		{
			ASSERT_VALID( pMiniFrame );
			CRect rcFrameReal;
			pMiniFrame->GetWindowRect( rcFrameReal );
			if( rcFrameReal == rcFrameNew )
				continue;
			pMiniFrame->MoveWindow( &rcFrameNew, FALSE );
			pMiniFrame->DelayRecalcLayout();
		} // if( pMiniFrame != NULL )
		else
			pExtBar->FloatControlBar( rcFrameNew.TopLeft() );
	} // for( i = 0; i < arrOuterProps.GetSize(); i++ )

	// last pass shows all the floating windows that were previously shown
	for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
	{
		CControlBarInfo * pInfo = (CControlBarInfo *)
			state.m_arrBarInfo[i];
		ASSERT( pInfo != NULL );
		if( pInfo->m_pBar == NULL )
			continue;
		if( !pInfo->m_bFloating )
			continue;
		CFrameWnd * pFrameWnd =
			pInfo->m_pBar->GetParentFrame();
		CDockBar * pDockBar = (CDockBar *) pInfo->m_pBar;
		ASSERT_KINDOF( CDockBar, pDockBar );
		if( pDockBar->GetDockedVisibleCount() > 0 )
		{
			pFrameWnd->RecalcLayout();
			pFrameWnd->ShowWindow( SW_SHOWNA );
			for( INT nBar = 1; nBar < pDockBar->m_arrBars.GetSize(); nBar ++ )
			{
				ASSERT( pDockBar->m_arrBars[0] == NULL );
				CControlBar * pBar = (CControlBar *)
					pDockBar->m_arrBars[nBar];
				if( pBar == NULL )
					continue;
				if( __PLACEHODLER_BAR_PTR(pBar) )
					continue;
				CExtControlBar * pExtBar =
					DYNAMIC_DOWNCAST(
						CExtControlBar,
						pBar
						);
				if( pExtBar == NULL
					|| pExtBar->IsFixedMode()
					)
					continue;
				pExtBar->_RecalcNcArea();
			}
		} // if( pDockBar->GetDockedVisibleCount() > 0 )
	} // for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
}

void CExtControlBar::InternalFriendlyFrameWnd::GetOuterDockState(
	CDockState & state,
	CExtControlBar::OuterPropsArr_t & arrOuterProps
	) const
{
	state.Clear(); //make sure dockstate is empty
	arrOuterProps.RemoveAll();

	for(	POSITION pos = m_listControlBars.GetHeadPosition();
			pos != NULL;
		)
	{ // get state info for each bar
		CControlBar * pBar = (CControlBar *)
			m_listControlBars.GetNext( pos );
		ASSERT( pBar != NULL );

		if(		pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
			||	pBar->IsKindOf(RUNTIME_CLASS(CExtDockDynBar))
			)
			continue;

		if(		pBar->m_pDockBar != NULL
			&&	pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynBar))
			)
			continue;

		if( pBar->IsDockBar() )
		{
			UINT nID = pBar->GetDlgCtrlID();
			ASSERT_DOCKBAR_DLGCTRLID( nID );
			if( nID == AFX_IDW_DOCKBAR_FLOAT )
			{
				CControlBar * pBarChild =
					STATIC_DOWNCAST(
						CControlBar,
						pBar->GetWindow(GW_CHILD)
						);
				if( pBarChild->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
					continue;
			}
			if(		pBar->IsKindOf(RUNTIME_CLASS(CExtDockBar))
				&&	((CExtDockBar *)pBar)->_GetCircleNo() != 0
				)
				continue;
		} // if( pBar->IsDockBar() )
#ifdef _DEBUG
		else
		{
			ASSERT( !pBar->IsKindOf(RUNTIME_CLASS(CDockBar)) );
		} // else from if( pBar->IsDockBar() )
#endif // _DEBUG

		CSize sizeReset( -1, -1 );
		if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
		{
			if( pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
				continue;

			if(		(! ((CExtControlBar*)pBar)->IsFixedMode() )
				&&	(! pBar->IsFloating() )
				)
			{
				if( pBar->m_pDockBar == NULL )
					continue;
				CExtDockBar * pDockBar =
					STATIC_DOWNCAST(
						CExtDockBar,
						pBar->m_pDockBar
						);
				ASSERT( !pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynBar)) );
				if( pDockBar->_GetCircleNo() > 0 )
					continue;
			}
			OuterItemData_t _oid;
			_oid.m_nBarID = pBar->GetDlgCtrlID();
			_oid.m_ptFloatHelper = ((CExtControlBar*)pBar)->m_ptFloatHelper;
			_oid.m_sizeDockedH = ((CExtControlBar*)pBar)->m_sizeDockedH;
			_oid.m_sizeDockedV = ((CExtControlBar*)pBar)->m_sizeDockedV;
			_oid.m_sizeFloated = ((CExtControlBar*)pBar)->m_sizeFloated;
			ASSERT( _oid.m_AffixmentData.IsEmpty() );
			InternalAffixmentData * pAffixmentData =
				((CExtControlBar*)pBar)->_AffixmentGetOuter();
			if( pAffixmentData != NULL )
				_oid.m_AffixmentData = *pAffixmentData;
	
			ASSERT( !_oid.m_bFloating );
			if( pBar->IsFloating() )
			{
				CRect rcBarWnd;
				CRect rcFraweWnd;
				pBar->GetWindowRect( &rcBarWnd );
				CExtMiniDockFrameWnd * pMiniFrame =
					STATIC_DOWNCAST(
						CExtMiniDockFrameWnd,
						pBar->GetParentFrame()
						);
				pMiniFrame->GetWindowRect( &rcFraweWnd );
				CSize sizeBarWnd = rcBarWnd.Size();
				CSize sizeFraweWnd = rcFraweWnd.Size();
				CSize sizeDiff = sizeFraweWnd - sizeBarWnd;
				_oid.m_ptFloatHelper = rcFraweWnd.TopLeft();
				if( ((CExtControlBar*)pBar)->IsFixedMode() )
				{
					_oid.m_sizeFloated += sizeDiff;
					sizeReset = _oid.m_sizeFloated;
				} // if( ((CExtControlBar*)pBar)->IsFixedMode() )
				_oid.m_bFloating = true;
			} // if( pBar->IsFloating() )

			arrOuterProps.Add( _oid );
		} // if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )

		CControlBarInfo* pInfo = new CControlBarInfo;
		pBar->GetBarInfo( pInfo );
		if( pInfo->m_pBar == NULL )
			continue;

		if( sizeReset.cx > 0 && sizeReset.cy > 0 )
			pInfo->m_nMRUWidth = sizeReset.cx;

		state.m_arrBarInfo.Add(pInfo);
	} // get state info for each bar
}

void CExtControlBar::InternalFriendlyFrameWnd::SetDockState(
	const CExtControlBar::InternalDockStateSite & state,
	bool bPresetWP
	)
{
	state.StateSet( bPresetWP );
}

void CExtControlBar::InternalFriendlyFrameWnd::GetDockState(
	CExtControlBar::InternalDockStateSite & state
	) const
{
	state.StateGet();
}

CExtControlBar::InternalDockStateSite::InternalDockStateSite(
	CFrameWnd * pDockSite
	)
	: m_pDockSite( pDockSite )
	, m_pOuterState( NULL )
{
	ASSERT_VALID( m_pDockSite );
	ASSERT_KINDOF( CFrameWnd, m_pDockSite );
	ASSERT( !m_pDockSite->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) );
	::memset( &m_wp, 0, sizeof(WINDOWPLACEMENT) );
	m_pOuterState = new CDockState;
}

CExtControlBar::InternalDockStateSite::~InternalDockStateSite()
{
	Clear();
	if( m_pOuterState != NULL )
		delete m_pOuterState;
}

void CExtControlBar::InternalDockStateSite::Clear()
{
	ASSERT_VALID( this );
	ASSERT( m_pOuterState != NULL );
	::memset( &m_wp, 0, sizeof(WINDOWPLACEMENT) );
	m_wp.length = sizeof(WINDOWPLACEMENT);

	m_pOuterState->Clear();

INT nCount = m_arrBarStates.GetSize();
	for( INT i = 0; i < nCount; i++ )
	{
		InternalDockStateBar * pState =
			m_arrBarStates[i];
		ASSERT_VALID( pState );
		pState->Clear();
		delete pState;
	}
	m_arrBarStates.RemoveAll();

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	m_arrAutoHideOrder.RemoveAll();
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	m_arrOuterProps.RemoveAll();
}

void CExtControlBar::InternalDockStateSite::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );

INT i, nCount;
DWORD dwApiVer0 = g_CmdManager.GetVersionDWORD( false );
DWORD dwApiVer1 = g_CmdManager.GetVersionDWORD( true );
DWORD dwReserved = 0;

	if( ar.IsStoring() )
	{

		// serialize version info
		CExtSafeString sTmpBuffer;
		CExtSafeString sFriendlyVer;
		sFriendlyVer.Format(
			_T("Prof-UIS (v. %s) control bar state"),
			g_CmdManager.GetVersionString( sTmpBuffer )
			);
		ar << sFriendlyVer;
		ar << dwApiVer0;
		ar << dwApiVer1;
		ar << dwReserved;
		ar << dwReserved;
		ar << dwReserved;
		ar << dwReserved;

		// serialize frame position
		CExtSafeString sFrameWp;
		sFrameWp.Format(
			_T("%d,%d,%d,%d,%d,%d"),
			m_wp.rcNormalPosition.left,
			m_wp.rcNormalPosition.top,
			m_wp.rcNormalPosition.right,
			m_wp.rcNormalPosition.bottom,
			m_wp.showCmd,
			m_wp.flags
			);
		ar << sFrameWp;

		m_pOuterState->Serialize( ar );

		nCount = m_arrOuterProps.GetSize();
		ar << nCount;
		for( i = 0; i < nCount; i++ )
			m_arrOuterProps.ElementAt(i).Serialize( ar );

		nCount = m_arrBarStates.GetSize();
		ar << nCount;
		for( i = 0; i < nCount; i++ )
		{
			InternalDockStateBar * pState =
				m_arrBarStates[i];
			ASSERT_VALID( pState );
			pState->Serialize( ar );
		}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		nCount = m_arrAutoHideOrder.GetSize();
		ar << nCount;
		for( i = 0; i < nCount; i++ )
		{
			UINT nBarID = m_arrAutoHideOrder[ i ];
			ar << nBarID;
		}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		
	} // if( ar.IsStoring() )
	else
	{
		Clear();

		// serialize version info
		DWORD dwApiVer0a = 0, dwApiVer1a = 0;
		CExtSafeString sFriendlyVer;
		ar >> sFriendlyVer;
		ar >> dwApiVer0a;
		ar >> dwApiVer1a;
		ar >> dwReserved;
		ar >> dwReserved;
		ar >> dwReserved;
		ar >> dwReserved;
		if( dwApiVer1 != dwApiVer1a )
		{
			ASSERT( FALSE );
			::AfxThrowArchiveException(
				CArchiveException::generic,
				NULL
				);
		}

		CExtSafeString sFrameWp;
		ar >> sFrameWp;
		ASSERT( !sFrameWp.IsEmpty() );
		::memset( &m_wp, 0, sizeof(WINDOWPLACEMENT) );
		m_wp.length = sizeof(WINDOWPLACEMENT);
		VERIFY( m_pDockSite->GetWindowPlacement(&m_wp)  );
		m_wp.ptMinPosition.x = m_wp.ptMinPosition.y = 0;
		m_wp.ptMaxPosition.x = m_wp.ptMaxPosition.y = 0;

		if(	_stscanf(
				sFrameWp,
				_T("%d,%d,%d,%d,%d,%d"),
				&m_wp.rcNormalPosition.left,
				&m_wp.rcNormalPosition.top,
				&m_wp.rcNormalPosition.right,
				&m_wp.rcNormalPosition.bottom,
				&m_wp.showCmd,
				&m_wp.flags
				) != 6
			)
		{
			ASSERT( FALSE );
			::memset( &m_wp, 0, sizeof(WINDOWPLACEMENT) );
			m_wp.length = sizeof(WINDOWPLACEMENT);
			m_wp.showCmd = SW_HIDE;
			//return false;
			::AfxThrowArchiveException(
				CArchiveException::generic,
				NULL
				);
		}

		m_pOuterState->Serialize( ar );

		ar >> nCount;
		for( i = 0; i < nCount; i++ )
		{
			OuterItemData_t _oid;
			_oid.Serialize( ar );
			m_arrOuterProps.Add( _oid );
		}

		ar >> nCount;
		for( i = 0; i < nCount; i++ )
		{
			InternalDockStateBar * pState =
				new InternalDockStateBar;
			pState->Serialize( ar );
			ASSERT_VALID( pState );
			m_arrBarStates.Add( pState );
		}
		
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		ar >> nCount;
		for( i = 0; i < nCount; i++ )
		{
			UINT nBarID;
			ar >> nBarID;
			m_arrAutoHideOrder.Add( nBarID );
		}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	} // else from if( ar.IsStoring() )
}

void CExtControlBar::InternalDockStateSite::StateGet()
{
	ASSERT_VALID( this );
	ASSERT( m_pOuterState != NULL );
	ASSERT_VALID( m_pDockSite );

	Clear();

	CExtDockBar::_OptimizeCircles( m_pDockSite );

	VERIFY( m_pDockSite->GetWindowPlacement(&m_wp) );

POSITION pos = m_pDockSite->m_listControlBars.GetHeadPosition();
	for( ; pos != NULL; )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockSite->m_listControlBars.GetNext( pos );
		ASSERT_VALID( pBar );
		
		if( pBar->IsFloating() )
		{
			if( pBar->IsDockBar() )
				continue;
			if( pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
			{
				CExtMiniDockFrameWnd * pFrame =
					STATIC_DOWNCAST(
						CExtMiniDockFrameWnd,
						pBar->GetParentFrame()
						);
				ASSERT_VALID( pBar->m_pDockBar );
				ASSERT_KINDOF( CDockBar, pBar->m_pDockBar );
				ASSERT( !pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) );
				ASSERT( pBar->m_pDockBar->GetDlgCtrlID() == AFX_IDW_DOCKBAR_FLOAT );
				ASSERT( pBar->m_pDockBar->GetParent() == pFrame );
				AppendFloatingLayout(
					STATIC_DOWNCAST(
						CExtMiniDockFrameWnd,
						pFrame
						)
					);
			} // if( pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
			continue;
		} // if( pBar->IsFloating() )
		
		if( !pBar->IsDockBar() )
			continue;

		ASSERT_KINDOF( CDockBar, pBar );
		if( !pBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) )
			continue;

		CFrameWnd * pFrame = pBar->GetParentFrame();
		ASSERT_VALID( pFrame );
		if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		{
			ASSERT_KINDOF( CExtMiniDockFrameWnd, pFrame );
			continue;
		}

		ASSERT(
			!pBar->GetParentFrame()->IsKindOf(
				RUNTIME_CLASS( CMiniFrameWnd )
				)
			);
		
		CExtDockBar * pDockBar =
			STATIC_DOWNCAST(
				CExtDockBar,
				pBar
				);

		UINT nCircleNo = pDockBar->_GetCircleNo();
		if( nCircleNo == 0 )
			continue;

		AppendDockBarAtSideLayout( pDockBar );
	}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
CExtDynAutoHideArea * arrAutoHiders[4] =
{
	NULL, NULL, NULL, NULL
};
static const UINT arrSideMap[4] =
{
	AFX_IDW_DOCKBAR_TOP,
	AFX_IDW_DOCKBAR_BOTTOM,
	AFX_IDW_DOCKBAR_LEFT,
	AFX_IDW_DOCKBAR_RIGHT,
};
	for( INT nSide = 0; nSide < 4; nSide++ )
	{
		UINT nDockBarID = arrSideMap[ nSide ];
		CExtDockBar * pDockBar = (CExtDockBar *)
			m_pDockSite->GetControlBar( nDockBarID );
		ASSERT_VALID( pDockBar );
		ASSERT_KINDOF( CExtDockBar, pDockBar );
		ASSERT( pDockBar->_GetCircleNo() == 0 );
		arrAutoHiders[ nSide ] = pDockBar->_GetAutoHideArea();
		if( arrAutoHiders[ nSide ] == NULL )
		{
			ASSERT( nSide == 0 );
			break;
		}
		ASSERT_VALID( arrAutoHiders[ nSide ] );
		ASSERT(
			UINT( arrAutoHiders[ nSide ]->GetDlgCtrlID() )
				== arrSideMap[nSide]
				);
	} // for( INT nSide = 0; nSide < 4; nSide++ )

	if( arrAutoHiders[0] != NULL )
	{
		for( nSide = 0; nSide < 4; nSide++ )
		{
			ASSERT_VALID( arrAutoHiders[nSide] );
			INT nCount = arrAutoHiders[nSide]->ItemGetCount();
			for( INT nItem = 0; nItem < nCount; nItem++ )
			{
				CExtTabWnd::TAB_ITEM_INFO * pTII =
					arrAutoHiders[nSide]->ItemGet( nItem );
				ASSERT_VALID( pTII );
				if( (pTII->GetItemStyle() & __ETWI_IN_GROUP_ACTIVE) == 0 )
					continue;
				CExtControlBar * pBar =
					reinterpret_cast < CExtControlBar * >
						( pTII->LParamGet() );
				ASSERT_VALID( pBar );
				ASSERT_KINDOF( CExtControlBar, pBar );
				ASSERT( !pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) );
				ASSERT( !pBar->IsFixedMode() );
				ASSERT( m_pDockSite->m_listControlBars.Find(pBar) != NULL );
				ASSERT( pBar->AutoHideModeGet() );
				UINT nBarID = (UINT) pBar->GetDlgCtrlID();
				m_arrAutoHideOrder.Add( nBarID );
			} // for( INT nItem = 0; nItem < nCount; nItem++ )
		} // for( nSide = 0; nSide < 4; nSide++ )
	} // if( arrAutoHiders[0] != NULL )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	((InternalFriendlyFrameWnd *)m_pDockSite)->GetOuterDockState(
		*m_pOuterState,
		m_arrOuterProps
		);
}

CExtControlBar::InternalAffixmentData::InternalAffixmentData()
	: m_nAffixmentWeight( 0 )
	, m_rcAffixment( 0, 0, 0, 0 )
	, m_sizeCompressed( 0, 0 )
	, m_dwAffixmentFlags( __EAFF_FIXED_MODE_INITIAL_RECALC )
{
}

CExtControlBar::InternalAffixmentData::InternalAffixmentData(
	const CExtControlBar::InternalAffixmentData & other
	)
{
	_AssignFromOther( other );
}

CExtControlBar::InternalAffixmentData::~InternalAffixmentData()
{
}

void CExtControlBar::InternalAffixmentData::_AssignFromOther(
	const CExtControlBar::InternalAffixmentData & other
	)
{
	m_nAffixmentWeight = other.m_nAffixmentWeight;
	m_rcAffixment = other.m_rcAffixment;
	m_sizeCompressed = other.m_sizeCompressed;
	m_dwAffixmentFlags = other.m_dwAffixmentFlags;
}

bool CExtControlBar::InternalAffixmentData::IsEmpty() const
{
	return (m_nAffixmentWeight==0) ? true : false;
}

void CExtControlBar::InternalAffixmentData::Empty()
{
	m_nAffixmentWeight = 0;
	m_rcAffixment.SetRectEmpty();
	ASSERT(
			m_rcAffixment.left == 0
		&&	m_rcAffixment.right == 0
		&&	m_rcAffixment.top == 0
		&&	m_rcAffixment.bottom == 0
		);
	m_sizeCompressed.cx = m_sizeCompressed.cy = 0;
	m_dwAffixmentFlags = __EAFF_FIXED_MODE_INITIAL_RECALC;
}

int CExtControlBar::InternalAffixmentData::CompareAffixment(
	const CExtControlBar::InternalAffixmentData & other
	) const
{
	if( !IsEmpty() )
	{
		if( other.IsEmpty() )
			return 1;
		if( m_nAffixmentWeight > other.m_nAffixmentWeight )
			return 1;
		if( m_nAffixmentWeight < other.m_nAffixmentWeight )
			return -1;
	} // if( !IsEmpty() )
	else
	{
		if( !other.IsEmpty() )
			return -1;
	} // else from if( !IsEmpty() )
	return 0;
}

void CExtControlBar::InternalAffixmentData::Serialize( CArchive & ar )
{
	ASSERT( sizeof(m_nAffixmentWeight) == (sizeof(BYTE)*4) );
SHORT nHelperFlags = 0x0000;
	if( ar.IsStoring() )
	{
		if( IsEmpty() )
			nHelperFlags |= 0x0001;
		ar.Write( &nHelperFlags, sizeof(nHelperFlags) );
		if( nHelperFlags & 0x0001 )
			return;
		ar << m_nAffixmentWeight;
		ar << m_rcAffixment;
		ar << m_sizeCompressed;
		ar << m_dwAffixmentFlags;
	} // if( ar.IsStoring() )
	else
	{
		Empty();
		ar.Read( &nHelperFlags, sizeof(nHelperFlags) );
		if( nHelperFlags & 0x0001 )
			return;
		ar >> m_nAffixmentWeight;
		ar >> m_rcAffixment;
		ar >> m_sizeCompressed;
		ar >> m_dwAffixmentFlags;
	} // else from if( ar.IsStoring() )
}

CExtControlBar::InternalDockStateBar *
	CExtControlBar::InternalDockStateSite::AppendFloatingLayout(
		CExtMiniDockFrameWnd * pMiniFrame
		)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pMiniFrame );
	ASSERT_KINDOF( CExtMiniDockFrameWnd, pMiniFrame );

CDockBar * pFloatDocker =
		STATIC_DOWNCAST(
			CDockBar,
			pMiniFrame->GetWindow( GW_CHILD )
			);
	ASSERT_VALID( pFloatDocker );
	ASSERT( pFloatDocker->IsDockBar() );
	ASSERT_KINDOF( CDockBar, pFloatDocker );
	ASSERT( !pFloatDocker->IsKindOf(RUNTIME_CLASS(CExtDockBar)) );
	ASSERT( pFloatDocker->GetDlgCtrlID() == AFX_IDW_DOCKBAR_FLOAT );
CExtDynControlBar * pDynDocker =
		STATIC_DOWNCAST(
			CExtDynControlBar,
			pFloatDocker->GetWindow( GW_CHILD )
			);
	ASSERT_VALID( pDynDocker );

InternalDockStateBar * _idsb =
		new InternalDockStateBar;
	_idsb->StateGet( pDynDocker, NULL );
	ASSERT( _idsb->m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING );
	m_arrBarStates.Add( _idsb );

	return _idsb;
}

CExtControlBar::InternalDockStateBar *
	CExtControlBar::InternalDockStateSite::AppendDockBarAtSideLayout(
		CExtDockBar * pDockBar
		)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDockBar );
	ASSERT_KINDOF( CExtDockBar, pDockBar );
	ASSERT( !pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynBar)) );
	ASSERT( pDockBar->_GetCircleNo() > 0 );

	ASSERT( pDockBar->m_arrBars[0] == NULL );
bool bNewRow = false;
INT nCount = pDockBar->m_arrBars.GetSize();
InternalDockStateBar * pLastState = NULL;
	for( INT nBar = 1; nBar < nCount; nBar ++ )
	{
		CExtControlBar * pBar = (CExtControlBar *)
			pDockBar->m_arrBars[nBar];
		if( pBar == NULL )
		{
			bNewRow = true;
			continue;
		}
		if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_KINDOF( CExtControlBar, pBar );
		ASSERT( !pBar->IsFixedMode() );

//		if( pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
//		{
//			if( ((CExtDynControlBar*)pBar)->m_pWndDynDocker->GetDockedCount() == 0 )
//				continue;
//		}

		InternalDockStateBar * _idsb =
			new InternalDockStateBar;
		_idsb->StateGet( pBar, NULL );
		if( bNewRow )
			_idsb->m_dwDockStateFlags |= InternalDockStateBar::__STATE_NEW_ROW;
		m_arrBarStates.Add( _idsb );
		bNewRow = false;
		pLastState = _idsb;
	}
	return pLastState;
}

void CExtControlBar::InternalDockStateSite::StateSet( bool bPresetWP ) const
{
	ASSERT_VALID( this );
	ASSERT( m_pOuterState != NULL );
	ASSERT_VALID( m_pDockSite );

	if( bPresetWP )
	{
		//WINDOWPLACEMENT _wp;
		//::memcpy( &_wp, &m_wp, sizeof(WINDOWPLACEMENT) );
		//VERIFY( m_pDockSite->SetWindowPlacement( &_wp ) );

		CRect rcDockSiteWnd;
		if(		m_wp.showCmd == SW_SHOWMAXIMIZED
			||	m_wp.showCmd == SW_SHOWMINIMIZED
			||	m_wp.showCmd == SW_SHOWMINNOACTIVE
			||	m_wp.showCmd == SW_HIDE
			||	m_wp.showCmd == SW_FORCEMINIMIZE
			)
		{
			CExtPaintManager::monitor_parms_t _mp;
			CExtPaintManager::stat_GetMonitorParms( _mp, m_wp.rcNormalPosition );
			rcDockSiteWnd = _mp.m_rcWorkArea;
		}
		else
		{
			rcDockSiteWnd =  m_wp.rcNormalPosition;
		}
		m_pDockSite->SetWindowPos(
			NULL,
			rcDockSiteWnd.left, rcDockSiteWnd.top,
			rcDockSiteWnd.Width(), rcDockSiteWnd.Height(),
			SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE
				|SWP_NOREDRAW|SWP_NOSENDCHANGING 
			);
	} // if( bPresetWP )

POSITION pos;
INT i, nCount;

ExtControlBarVector_t vRedockBars;
	for(	pos =
				m_pDockSite->m_listControlBars.GetHeadPosition();
			pos != NULL;
			)
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockSite->m_listControlBars.GetNext( pos );
		ASSERT_VALID( pBar );
		
		if( pBar->GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		{
			if(		pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
				&&	!pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
				)
				vRedockBars.Add( (CExtControlBar *)pBar );
			continue;
		}

		if( pBar->IsDockBar() )
			continue;
		if(		pBar->m_pDockSite == NULL
			||	pBar->m_pDockContext == NULL
			)
			continue;
		if( pBar->m_pDockBar == NULL )
		{
			m_pDockSite->DockControlBar( pBar );
			m_pDockSite->ShowControlBar( pBar, FALSE, TRUE );
			continue;
		}
		if(		!pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
			||	pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
			||	((CExtControlBar *)pBar)->IsFixedMode()
			)
			continue;

		ASSERT_KINDOF( CExtDockBar, pBar->m_pDockBar );
		if( ((CExtDockBar*)pBar->m_pDockBar)->_GetCircleNo() == 0
			&& (! pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynBar)) )
			)
			continue;
		
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		if( ((CExtControlBar *)pBar)->AutoHideModeGet() )
			((CExtControlBar *)pBar)->AutoHideModeSet(
				false, false, false, true );
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		m_pDockSite->ShowControlBar( pBar, FALSE, TRUE );
	}

	nCount = vRedockBars.GetSize();
	for( i = 0; i < nCount; i++ )
	{
		CExtControlBar * pBar = vRedockBars[i];
		DWORD dwBarStyle = pBar->GetBarStyle();
		UINT nBarDockerID = AFX_IDW_DOCKBAR_TOP;
		if( dwBarStyle & CBRS_ALIGN_BOTTOM )
			nBarDockerID = AFX_IDW_DOCKBAR_BOTTOM;
		else if( dwBarStyle & CBRS_ALIGN_LEFT )
			nBarDockerID = AFX_IDW_DOCKBAR_LEFT;
		else if( dwBarStyle & CBRS_ALIGN_RIGHT )
			nBarDockerID = AFX_IDW_DOCKBAR_RIGHT;
		if( pBar->IsFixedMode() )
		{
			bool bFloating = pBar->IsFloating();
			if( bFloating )
				m_pDockSite->ShowControlBar( pBar, FALSE, FALSE );
			m_pDockSite->DockControlBar(
				pBar,
				nBarDockerID
				);
			m_pDockSite->ShowControlBar( pBar, FALSE, TRUE );
		}
		else
		{
			((CExtControlBar *)pBar)->DockControlBar(
				nBarDockerID,
				1,
				m_pDockSite,
				false
				);
			m_pDockSite->ShowControlBar( pBar, FALSE, TRUE );
		}
	}

	CExtDockBar::_OptimizeCircles( m_pDockSite );
	m_pDockSite->RecalcLayout();

	nCount = m_arrBarStates.GetSize();
	for( i = 0; i < nCount; i++ )
	{
		InternalDockStateBar * pState =
			m_arrBarStates[i];
		ASSERT_VALID( pState );
		pState->StateSet( m_pDockSite, NULL );
	}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	nCount = m_arrAutoHideOrder.GetSize();
	for( i = 0; i < nCount; i++ )
	{
		UINT nBarID = m_arrAutoHideOrder[ i ];
		CExtControlBar * pBar =
			STATIC_DOWNCAST(
				CExtControlBar,
				m_pDockSite->GetControlBar( nBarID )
				);
		ASSERT_VALID( pBar );
		ASSERT_KINDOF( CExtControlBar, pBar );
		ASSERT( !pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) );
		ASSERT( !pBar->IsFixedMode() );
		ASSERT( !pBar->AutoHideModeGet() );
		ASSERT( !pBar->IsFloating() );
		ASSERT( !pBar->GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) );
		pBar->AutoHideModeSet( true, false, false, true );
		
		ASSERT_KINDOF( CExtDockBar, pBar->m_pDockBar );
		
		CWnd * pWndParent = pBar->m_pDockBar;
		for( ; true ;  )
		{
			ASSERT_VALID( pWndParent );
			if( pWndParent->IsKindOf(RUNTIME_CLASS(CExtDockBar)) )
			{
				if( !pWndParent->IsKindOf(RUNTIME_CLASS(CExtDockDynBar)) )
				{
					ASSERT( pWndParent->GetParent() == m_pDockSite );
					break;
				}
			}
			pWndParent = pWndParent->GetParent();
		}
		ASSERT_KINDOF( CExtDockBar, pWndParent );
		ASSERT( ((CExtDockBar*)pWndParent)->_GetCircleNo() > 0 );
		UINT nTopDockBarID = pWndParent->GetDlgCtrlID();
		ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nTopDockBarID );
		CExtDockBar * pOuterDocker =
			STATIC_DOWNCAST(
				CExtDockBar,
				m_pDockSite->GetControlBar( nTopDockBarID )
				);
		ASSERT_VALID( pOuterDocker );
		ASSERT( UINT(pOuterDocker->GetDlgCtrlID()) == nTopDockBarID );
		
		CExtDynAutoHideArea * pAutoHideArea =
			pOuterDocker->_GetAutoHideArea();
		ASSERT_VALID( pAutoHideArea );
		ASSERT( UINT(pAutoHideArea->GetDlgCtrlID()) == nTopDockBarID );
		LONG nPos = pAutoHideArea->FindControlBar( pBar );
		ASSERT( nPos >= 0 );
		
		CExtTabWnd::TAB_ITEM_INFO * pTII =
			pAutoHideArea->ItemGet( nPos );
		pTII->ModifyItemStyle( 0, __ETWI_IN_GROUP_ACTIVE );

	} // for( i = 0; i < nCount; i++ )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	((InternalFriendlyFrameWnd *)m_pDockSite)->SetOuterDockState(
		*m_pOuterState,
		m_arrOuterProps
		);

	nCount = m_arrBarStates.GetSize();
	for( i = 0; i< nCount; i++ )
	{
		InternalDockStateBar * pState =
			m_arrBarStates[i];
		ASSERT_VALID( pState );
		pState->PreSyncAttributes( m_pDockSite, NULL );
	}

#ifdef _DEBUG
	for(	pos =
				m_pDockSite->m_listControlBars.GetHeadPosition();
			pos != NULL;
			)
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockSite->m_listControlBars.GetNext( pos );
		ASSERT_VALID( pBar );
		CExtDockBar * pExtDocker =
			DYNAMIC_DOWNCAST( CExtDockBar, pBar );
		if( pExtDocker != NULL )
		{
			ASSERT( !pExtDocker->m_bLockedOptimize );
		}
	}
#endif // _DEBUG

	m_pDockSite->RecalcLayout();

	nCount = m_arrBarStates.GetSize();
	for( i = 0; i < nCount; i++ )
	{
		InternalDockStateBar * pState =
			m_arrBarStates[i];
		ASSERT_VALID( pState );
		pState->PostSyncAttributes( m_pDockSite, NULL );
		if( pState->m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING )
		{
			ASSERT_VALID( pState->m_pHelperBar );
			CExtMiniDockFrameWnd * pMiniFrame =
				STATIC_DOWNCAST(
					CExtMiniDockFrameWnd,
					pState->m_pHelperBar->GetParentFrame()
					);
			pState->m_pHelperBar->_RecalcNcArea();
			pState->m_pHelperBar->OnRepositionSingleChild();
			pMiniFrame->RecalcLayout();
		}
	}

	for(	pos =
				m_pDockSite->m_listControlBars.GetHeadPosition();
			pos != NULL;
			)
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockSite->m_listControlBars.GetNext( pos );
		ASSERT_VALID( pBar );
		if( pBar->IsDockBar() )
			continue;
		if(		(!pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)))
			||	pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
			)
			continue;
		if( !((CExtControlBar *)pBar)->IsFixedMode() )
			continue;
		((CExtControlBar *)pBar)->_RecalcNcArea();
		if( !pBar->IsFloating() )
			continue;
		ASSERT_KINDOF(
			CMiniDockFrameWnd,
			pBar->GetParentFrame()
			);
		((CExtControlBar *)pBar)->_RecalcLayoutImpl();
	}

/*
bool bRecalcLayout = false;
	for(	pos =
				m_pDockSite->m_listControlBars.GetHeadPosition();
			pos != NULL;
			)
	{
		CControlBar * pBar = (CControlBar *)m_pDockSite->m_listControlBars.GetNext(pos);
		ASSERT_VALID( pBar );
		ASSERT_KINDOF( CControlBar, pBar );
		ASSERT( pBar->GetSafeHwnd() != NULL );
		ASSERT( ::IsWindow( pBar->GetSafeHwnd() ) );
		if(		pBar->IsDockBar()
			||	(!pBar->IsVisible())
			||	pBar->GetParentFrame() != m_pDockSite
			)
			continue;
		bRecalcLayout = true;
		pBar->SetWindowPos(
			NULL, 0,0,0,0,
			SWP_NOMOVE|SWP_NOSIZE
				|SWP_NOZORDER|SWP_NOOWNERZORDER
				|SWP_FRAMECHANGED
			);
	}
	if( bRecalcLayout )
		m_pDockSite->RecalcLayout();
*/
	m_pDockSite->SendMessageToDescendants(
		WM_IDLEUPDATECMDUI,
		(WPARAM)TRUE,
		0
		);
	m_pDockSite->SendMessageToDescendants(
		WM_IDLEUPDATECMDUI,
		(WPARAM)TRUE,
		0,
		TRUE,
		TRUE
		);
	m_pDockSite->SendMessage(
		WM_SETMESSAGESTRING,
		AFX_IDS_IDLEMESSAGE
		);
}

CExtControlBar::InternalDockStateBar::InternalDockStateBar()
{
	InitMembers();
}

CExtControlBar::InternalDockStateBar::~InternalDockStateBar()
{
	Clear();
}

void CExtControlBar::InternalDockStateBar::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );

INT i, nCount;

	if( ar.IsStoring() )
	{
		ar << m_nBarID;
		ar << m_nDockBarID;
		ar << m_nCircleNo;
		ar << m_nMRUWidth;
		ar << m_dwBarFlags;
		ar << m_dwDockStateFlags;
		ar << m_dwMRUFloatStyle;
		ar << m_ptFloatPos;
		ar << m_ptMRUFloatPos;
		ar << m_rcMRUDockPos;
		ar << m_rcFloatingFrame;
		ar << m_dwFloatingFrameCreateStyle;
		ar << m_nDelayedRowUpdateMetric;
		ar << m_rcBar;
		ar << m_sizeDockedH;
		ar << m_sizeDockedV;
		ar << m_sizeFloated;

		nCount = m_arrChildStates.GetSize();
		ar << nCount;
		for( i = 0; i < nCount; i++ )
		{
			InternalDockStateBar * pState = m_arrChildStates[i];
			ASSERT_VALID( pState );
			pState->Serialize( ar );
		}

	} // if( ar.IsStoring() )
	else
	{
		Clear();
		
		ar >> m_nBarID;
		ar >> m_nDockBarID;
		ar >> m_nCircleNo;
		ar >> m_nMRUWidth;
		ar >> m_dwBarFlags;
		ar >> m_dwDockStateFlags;
		ar >> m_dwMRUFloatStyle;
		ar >> m_ptFloatPos;
		ar >> m_ptMRUFloatPos;
		ar >> m_rcMRUDockPos;
		ar >> m_rcFloatingFrame;
		ar >> m_dwFloatingFrameCreateStyle;
		ar >> m_nDelayedRowUpdateMetric;
		ar >> m_rcBar;
		ar >> m_sizeDockedH;
		ar >> m_sizeDockedV;
		ar >> m_sizeFloated;

		ar >> nCount;
		for( i = 0; i < nCount; i++ )
		{
			InternalDockStateBar * pState = new InternalDockStateBar;
			pState->Serialize( ar );
			ASSERT_VALID( pState );
			m_arrChildStates.Add( pState );
		}

	} // else from if( ar.IsStoring() )
}

void CExtControlBar::InternalDockStateBar::Clear()
{
	ASSERT_VALID( this );
	InitMembers();

INT i, nCount;

	nCount = m_arrChildStates.GetSize();
	for( i = 0; i < nCount; i++ )
	{
		InternalDockStateBar * pState = m_arrChildStates[i];
		ASSERT_VALID( pState );
		pState->Clear();
		delete pState;
	}
	m_arrChildStates.RemoveAll();

}

void CExtControlBar::InternalDockStateBar::InitMembers()
{
	ASSERT( this != NULL );
	m_nBarID = m_nDockBarID = AFX_IDW_DOCKBAR_FLOAT;
	m_nCircleNo = 0;
	m_nMRUWidth = 50;
	m_ptFloatPos.x = m_ptFloatPos.y = 0;
	m_rcMRUDockPos.SetRectEmpty();
	m_rcFloatingFrame.SetRect( 0, 0, 200, 200 );
	m_dwFloatingFrameCreateStyle = WS_CHILD;
	m_nDelayedRowUpdateMetric = 0;
	m_rcBar.SetRect( 0, 0, 200, 200 );
	m_dwMRUFloatStyle = 0;
	m_ptMRUFloatPos.x = m_ptMRUFloatPos.y;
	m_sizeDockedH.cx = m_sizeDockedH.cy = 200;
	m_sizeDockedV.cx = m_sizeDockedV.cy = 200;
	m_sizeFloated.cx = m_sizeFloated.cy = 200;

	m_dwDockStateFlags = 0;

	m_nHelperAciveTabChildIndex = -1;
	m_pHelperBar = NULL;
}

void CExtControlBar::InternalDockStateBar::StateGet(
	CExtControlBar * pBar,
	CExtControlBar::InternalDockStateBar * pParentState
	)
{
	pParentState;
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( !pBar->IsFixedMode() );
	ASSERT_VALID( pBar->m_pDockBar );
	ASSERT( pBar->m_pDockContext != NULL );
#ifdef _DEBUG
	if( pBar->IsFloating() )
	{
		ASSERT_KINDOF( CDockBar, pBar->m_pDockBar );
		ASSERT( !pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) );
	}
	else
	{
		ASSERT_KINDOF( CExtDockBar, pBar->m_pDockBar );
	}
#endif // _DEBUG

	Clear();

	m_nBarID = pBar->GetDlgCtrlID();
	m_nDockBarID = pBar->m_pDockBar->GetDlgCtrlID();
	m_nCircleNo = 0;
	if( !pBar->IsFloating() )
	{

		ASSERT_KINDOF( CExtDockBar, pBar->m_pDockBar );
		m_nCircleNo = ((CExtDockBar *)pBar->m_pDockBar)->_GetCircleNo();

#ifdef _DEBUG
		CFrameWnd * pParentFrame = pBar->GetParentFrame();
		if( pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		{
			ASSERT_KINDOF( CExtMiniDockFrameWnd, pParentFrame );
			ASSERT( m_nCircleNo == 0 );
		}
		else
		{
			//ASSERT( m_nCircleNo > 0 );
			ASSERT( pParentFrame == pBar->m_pDockSite );
		}
#endif // _DEBUG

	}

	if( pBar->IsVisible() )
		m_dwDockStateFlags |= __STATE_VISIBLE;
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( pBar->AutoHideModeGet() )
	{
		CExtDockDynTabBar * pTabbedDocker =
			DYNAMIC_DOWNCAST(
				CExtDockDynTabBar,
				pBar->m_pDockBar
				);
		if( pTabbedDocker == NULL )
		{
			if( !pBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar)) )
				m_dwDockStateFlags |= __STATE_AUTOHIDDEN;
		}
		else
		{
			ASSERT_VALID( pParentState );
			ASSERT_KINDOF( CExtDynTabControlBar, pTabbedDocker->GetParent() );
			pParentState->m_dwDockStateFlags |= __STATE_AUTOHIDDEN;
		} // else from if( pTabbedDocker == NULL )
	} // if( pBar->AutoHideModeGet() )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	
	m_nMRUWidth = pBar->m_nMRUWidth;

CRect m_rcBar;
	pBar->GetWindowRect( &m_rcBar );
//CRect rcBarClient;
//	pBar->GetClientRect( &rcBarClient );
//CSize sizeBarWnd = m_rcBar.Size();
//CSize sizeBarClient = rcBarClient.Size();
//CSize sizeNcArea = sizeBarWnd - sizeBarClient;
	pBar->m_pDockBar->ScreenToClient( &m_rcBar );

	m_ptFloatPos = pBar->m_ptFloatHelper;
	m_sizeDockedH = pBar->m_sizeDockedH; // + sizeNcArea;
	m_sizeDockedV = pBar->m_sizeDockedV; // + sizeNcArea;
	m_sizeFloated = pBar->m_sizeFloated;

	m_nDelayedRowUpdateMetric = pBar->m_nDelayedRowUpdateMetric;

	m_rcMRUDockPos = pBar->m_pDockContext->m_rectMRUDockPos;
	m_dwMRUFloatStyle = pBar->m_pDockContext->m_dwMRUFloatStyle;
	m_ptMRUFloatPos = pBar->m_pDockContext->m_ptMRUFloatPos;

	m_dwBarFlags = pBar->m_dwStyle;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( pBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar)) )
	{ // if tabbed container
		m_dwDockStateFlags |= InternalDockStateBar::__DOCK_DYNAMIC_TABBED;

		CExtDynTabControlBar * pTabbedBar =
			STATIC_DOWNCAST(
				CExtDynTabControlBar,
				pBar
				);
		
		CExtDockDynTabBar * pTabbedDocker =
			STATIC_DOWNCAST(
				CExtDockDynTabBar,
				pTabbedBar->m_pWndDynDocker //  GetWindow( GW_CHILD )
				);

		ASSERT( pTabbedDocker->m_arrBars.GetSize() > 1 );
		ASSERT( pTabbedDocker->m_arrBars[0] == NULL );

		LONG nSel = -1;
		if( pBar->GetStyle() & WS_VISIBLE )
			nSel = pTabbedBar->GetSwitcherSelection();

		ExtControlBarVector_t vBars;
		pTabbedDocker->CalcOrderedVector( vBars );
		LONG nCount = vBars.GetSize();
		//ASSERT( nCount > 1 );
#ifdef _DEBUG
		INT nRealDockedCount = pTabbedDocker->GetDockedCount();
		ASSERT( nCount == nRealDockedCount );
#endif // _DEBUG
		for( LONG nBar = 0; nBar < nCount; nBar++ )
		{
			CExtControlBar * pChildBar = vBars[ nBar ];
			ASSERT_VALID( pChildBar );
			ASSERT_KINDOF( CExtControlBar, pChildBar );
			ASSERT( !pChildBar->IsFixedMode() );

			InternalDockStateBar * _idsb =
				new InternalDockStateBar;
			_idsb->StateGet( pChildBar, this );
			_idsb->m_dwDockStateFlags |= InternalDockStateBar::__STATE_DYNAMIC_CHILD;
			_idsb->m_dwDockStateFlags |= InternalDockStateBar::__STATE_TAB_CHILD;
			if( nSel == nBar )
				_idsb->m_dwDockStateFlags |= InternalDockStateBar::__STATE_TAB_ACTIVE;
			m_arrChildStates.Add( _idsb );
		}
		ASSERT( m_arrChildStates.GetSize() == nRealDockedCount );

		if( pBar->IsFloating() )
		{
			m_dwDockStateFlags |= InternalDockStateBar::__STATE_FLOATING;
			CExtMiniDockFrameWnd * pMiniFrame =
				STATIC_DOWNCAST(
					CExtMiniDockFrameWnd,
					pBar->GetParentFrame()
					);
			pMiniFrame->GetWindowRect( &m_rcFloatingFrame );
			m_dwFloatingFrameCreateStyle = pMiniFrame->GetStyle();
		}

	} // if tabbed container
	else
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		if( pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
	{ // if dynamic half-split container
		m_dwDockStateFlags |= InternalDockStateBar::__DOCK_DYNAMIC_HALF_SPLIT;
		
		CExtDockDynBar * pDynDocker =
			STATIC_DOWNCAST(
				CExtDockDynBar,
				pBar->GetWindow( GW_CHILD )
				);

		INT nCount = pDynDocker->m_arrBars.GetSize();
		ASSERT( nCount > 1 );
		ASSERT( pDynDocker->m_arrBars[0] == NULL );

		bool bNewRow = false;
		for( INT nBar = 1; nBar < nCount; nBar++ )
		{
			CExtControlBar * pChildBar = (CExtControlBar *)
				pDynDocker->m_arrBars[nBar];
			if( pChildBar == NULL )
			{
				bNewRow = true;
				continue;
			}
			if( __PLACEHODLER_BAR_PTR(pChildBar) )
				continue;
			ASSERT_KINDOF( CExtControlBar, pChildBar );
			ASSERT( !pChildBar->IsFixedMode() );

//			if( pChildBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
//			{
//				if( ((CExtDynControlBar*)pChildBar)->m_pWndDynDocker->GetDockedCount() == 0 )
//					continue;
//			}

			InternalDockStateBar * _idsb =
				new InternalDockStateBar;
			_idsb->StateGet( pChildBar, this );
			if( bNewRow )
				_idsb->m_dwDockStateFlags |= InternalDockStateBar::__STATE_NEW_ROW;
			_idsb->m_dwDockStateFlags |= InternalDockStateBar::__STATE_DYNAMIC_CHILD;
			m_arrChildStates.Add( _idsb );

			bNewRow = false;
		}
#ifdef _DEBUG
		INT nRealDockedCount = pDynDocker->GetDockedCount();
		ASSERT( nRealDockedCount > 0 );
		ASSERT( m_arrChildStates.GetSize() == nRealDockedCount );
#endif // _DEBUG

		if( pBar->IsFloating() )
		{
			m_dwDockStateFlags |= InternalDockStateBar::__STATE_FLOATING;
			CExtMiniDockFrameWnd * pMiniFrame =
				STATIC_DOWNCAST(
					CExtMiniDockFrameWnd,
					pBar->GetParentFrame()
					);
			pMiniFrame->GetWindowRect( &m_rcFloatingFrame );
			m_dwFloatingFrameCreateStyle = pMiniFrame->GetStyle();
		}

	} // if dynamic half-split container
	else
	{ // if bar at side
		m_dwDockStateFlags |= InternalDockStateBar::__DOCK_AT_SIDE;
		ASSERT( !pBar->IsFloating() );
	} // if bar at side

}

void CExtControlBar::InternalDockStateBar::StateSet(
	CFrameWnd * pDockSite,
	CExtControlBar::InternalDockStateBar * pParentState
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDockSite );

	m_pHelperBar = NULL;

	switch( (m_dwDockStateFlags & __DOCK_AT_MASK) )
	{
	case __DOCK_AT_NOWHERE:
	break; // case __DOCK_AT_NOWHERE
	case __DOCK_AT_SIDE:
	{
		ASSERT_DOCKBAR_DLGCTRLID_DOCKED( m_nDockBarID );
		ASSERT( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) == 0 );
		
		m_pHelperBar = (CExtControlBar *)
			pDockSite->GetControlBar( m_nBarID );
		ASSERT_VALID( m_pHelperBar );
		ASSERT_KINDOF( CExtControlBar, m_pHelperBar );
		ASSERT( !m_pHelperBar->IsFixedMode() );

		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )
		{
			ASSERT_VALID( pParentState );
			ASSERT_VALID( pParentState->m_pHelperBar );
			ASSERT_KINDOF( CExtDynControlBar, pParentState->m_pHelperBar );

			CExtDockDynBar * pDockBar =
				STATIC_DOWNCAST(
					CExtDockDynBar,
					(	STATIC_DOWNCAST(
							CExtDynControlBar,
							pParentState->m_pHelperBar
							)
						)->m_pWndDynDocker
					);

			pDockBar->_LockSequenceOptimization( true );
			
			m_pHelperBar->m_bUpdatingChain = true;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_TAB_CHILD) != 0 )
			{
				ASSERT_KINDOF( CExtDynTabControlBar, pParentState->m_pHelperBar );
				((CExtDynTabControlBar*)pParentState->m_pHelperBar)
					-> InsertBar(
						m_pHelperBar,
						-1,
						false
						);
			} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_TAB_CHILD) != 0 )
			else
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			{
				pDockBar->DockControlBar( m_pHelperBar );
			} // else from if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_TAB_CHILD) != 0 )
			
			m_pHelperBar->MoveWindow( &m_rcBar, FALSE );
			m_pHelperBar->m_bUpdatingChain = false;
			
			pDockBar->_LockSequenceOptimization( false );

			INT nBarPos = 
				pDockBar->FindBar( m_pHelperBar );
			ASSERT( nBarPos >= 1 );
			if( nBarPos > 1 )
			{
				if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				{
					if( pDockBar->m_arrBars[nBarPos-1] != NULL  )
						pDockBar->m_arrBars.InsertAt(
							nBarPos,
							(CControlBar *)NULL
							);
				} // if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				else
				{
					if( pDockBar->m_arrBars[nBarPos-1] == NULL  )
						pDockBar->m_arrBars.RemoveAt( nBarPos-1 );
				} // else from if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
			} // if( nBarPos > 1 )
		} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )
		else
		{
			ASSERT( m_nCircleNo > 0 );

			ASSERT_VALID( m_pHelperBar->m_pDockBar );
			ASSERT( m_pHelperBar->m_pDockContext != NULL );
			ASSERT_KINDOF( CExtDockBar, m_pHelperBar->m_pDockBar );

			CExtDockBar * pDockBar =
				STATIC_DOWNCAST(
					CExtDockBar,
					pDockSite->GetControlBar( m_nDockBarID )
					);
			pDockBar =
				pDockBar->_GetBarByCircleNo( m_nCircleNo );

			pDockBar->_LockSequenceOptimization( true );
			pDockBar->DockControlBar( m_pHelperBar );
			pDockBar->_LockSequenceOptimization( false );

			INT nBarPos = 
				pDockBar->FindBar( m_pHelperBar );
			ASSERT( nBarPos >= 1 );
			if( nBarPos > 1 )
			{
				if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				{
					if( pDockBar->m_arrBars[nBarPos-1] != NULL  )
						pDockBar->m_arrBars.InsertAt(
							nBarPos,
							(CControlBar *)NULL
							);
				} // if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				else
				{
					if( pDockBar->m_arrBars[nBarPos-1] == NULL  )
						pDockBar->m_arrBars.RemoveAt( nBarPos-1 );
				} // else from if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
			} // if( nBarPos > 1 )
		} // else from if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )
	}
	break; // case __DOCK_AT_SIDE
	case __DOCK_DYNAMIC_HALF_SPLIT:
	{
		CFrameWnd * pParentFrame = pDockSite;
		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CExtMiniDockFrameWnd * pMiniFrame =
				new CExtMiniDockFrameWnd;
			if( !pMiniFrame->Create(
					pDockSite,
					m_dwFloatingFrameCreateStyle
					)
				)
			{
				ASSERT( FALSE );
				AfxThrowResourceException();
			}
			pParentFrame = pMiniFrame;
			CRect rcFrameAdjusted =
				CExtPaintManager::stat_AlignWndRectToMonitor(
					m_rcFloatingFrame
					);
			pMiniFrame->MoveWindow( &rcFrameAdjusted, FALSE );
			pMiniFrame->DelayRecalcLayout();
		} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )

		m_pHelperBar = new CExtDynControlBar;

		DWORD dwDynStyle =
				WS_CHILD
				| (m_dwDockStateFlags & CBRS_ALIGN_ANY)
				;
		dwDynStyle |= CBRS_BORDER_ANY;
		dwDynStyle &= ~( CBRS_FLOATING | CBRS_GRIPPER );
		dwDynStyle |= CBRS_TOOLTIPS;

		m_pHelperBar->SetInitDesiredSizeHorizontal( m_sizeDockedH );
		m_pHelperBar->SetInitDesiredSizeVertical( m_sizeDockedV );
		m_pHelperBar->SetInitDesiredSizeFloating( m_sizeFloated );
		if(	!m_pHelperBar->Create(
				NULL,
				pDockSite, // pParentFrame
				m_nBarID,
				dwDynStyle
				)
			)
		{
			ASSERT( FALSE );
			return;
		}
		ASSERT( ((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker != NULL );
		ASSERT( ((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->GetSafeHwnd() != NULL );
		ASSERT( ::IsWindow(((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->GetSafeHwnd()) );
		m_pHelperBar->m_pDockSite = pDockSite;
		((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_pDockSite = pDockSite;
		m_pHelperBar->m_hWndOwner = pDockSite->GetSafeHwnd();
		m_pHelperBar->EnableDocking( CBRS_ALIGN_ANY );
		ASSERT( m_pHelperBar->m_pDockContext != NULL );
		ASSERT( m_pHelperBar->m_pDockContext->m_pDockSite == pDockSite );
		ASSERT_VALID( m_pHelperBar );

		if( pDockSite->m_listControlBars.Find(m_pHelperBar) == NULL )
			pDockSite->m_listControlBars.AddTail( m_pHelperBar );

		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CExtMiniDockFrameWnd * pMiniFrame =
				STATIC_DOWNCAST(
					CExtMiniDockFrameWnd,
					pParentFrame
					);
			CDockBar * pFloatingDocker =
				STATIC_DOWNCAST(
					CDockBar,
					pMiniFrame->GetWindow(GW_CHILD)
					);
			ASSERT( !pFloatingDocker->IsKindOf(RUNTIME_CLASS(CExtDockBar)) );
			ASSERT( pFloatingDocker->GetDlgCtrlID() == AFX_IDW_DOCKBAR_FLOAT );
			m_pHelperBar->SetParent( pFloatingDocker );
		} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )

		if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )
		{
			POSITION posToRemove =
				pDockSite->m_listControlBars.Find(
					((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker
					);
			if( posToRemove != NULL )
			{
				pDockSite->m_listControlBars.RemoveAt( posToRemove );
				((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified = true;
			} // if( posToRemove != NULL )
		} // if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )

		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CDockBar * pDockBar =
				STATIC_DOWNCAST(
					CDockBar,
					pParentFrame->GetWindow( GW_CHILD )
					);
			ASSERT_KINDOF( CDockBar, pDockBar );
			ASSERT( !pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) );
			ASSERT( pDockBar->GetDlgCtrlID() ==	AFX_IDW_DOCKBAR_FLOAT );
			((CExtDockBar*)pDockBar)->DockControlBar( m_pHelperBar );
			ASSERT( pDockBar->IsFloating() );
			ASSERT( m_pHelperBar->IsFloating() );
			
			m_pHelperBar->m_bUpdatingChain = true;
			m_pHelperBar->MoveWindow( &m_rcBar, FALSE );
			m_pHelperBar->m_bUpdatingChain = false;
		} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		else if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )
		{
			ASSERT_VALID( pParentState );
			ASSERT_VALID( pParentState->m_pHelperBar );
			ASSERT_KINDOF( CExtDynControlBar, m_pHelperBar );

			CExtDockBar * pDockBar =
				STATIC_DOWNCAST(
					CExtDockBar,
					(	STATIC_DOWNCAST(
							CExtDynControlBar,
							pParentState->m_pHelperBar
							)
						)->m_pWndDynDocker
					);

			pDockBar->_LockSequenceOptimization( true );

			pDockBar->DockControlBar( m_pHelperBar );
			
			m_pHelperBar->m_bUpdatingChain = true;
			m_pHelperBar->MoveWindow( &m_rcBar, FALSE );
			m_pHelperBar->m_bUpdatingChain = false;
			
			pDockBar->_LockSequenceOptimization( false );

			INT nBarPos = 
				pDockBar->FindBar( m_pHelperBar );
			ASSERT( nBarPos >= 1 );
			if( nBarPos > 1 )
			{
				if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				{
					if( pDockBar->m_arrBars[nBarPos-1] != NULL  )
						pDockBar->m_arrBars.InsertAt(
							nBarPos,
							(CControlBar *)NULL
							);
				} // if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				else
				{
					if( pDockBar->m_arrBars[nBarPos-1] == NULL  )
						pDockBar->m_arrBars.RemoveAt( nBarPos-1 );
				} // else from if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
			} // if( nBarPos > 1 )
		} // else if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )
		else
		{
			ASSERT( m_nCircleNo > 0 );
			ASSERT( m_pHelperBar->m_pDockContext != NULL );

			CExtDockBar * pDockBar =
				STATIC_DOWNCAST(
					CExtDockBar,
					pDockSite->GetControlBar( m_nDockBarID )
					);
			pDockBar =
				pDockBar->_GetBarByCircleNo( m_nCircleNo );

			pDockBar->_LockSequenceOptimization( true );
			
			pDockBar->DockControlBar( m_pHelperBar );
			
			m_pHelperBar->m_bUpdatingChain = true;
			m_pHelperBar->MoveWindow( &m_rcBar, FALSE );
			m_pHelperBar->m_bUpdatingChain = false;
			
			pDockBar->_LockSequenceOptimization( false );

			INT nBarPos = 
				pDockBar->FindBar( m_pHelperBar );
			ASSERT( nBarPos >= 1 );
			if( nBarPos > 1 )
			{
				if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				{
					if( pDockBar->m_arrBars[nBarPos-1] != NULL  )
						pDockBar->m_arrBars.InsertAt(
							nBarPos,
							(CControlBar *)NULL
							);
				} // if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				else
				{
					if( pDockBar->m_arrBars[nBarPos-1] == NULL  )
						pDockBar->m_arrBars.RemoveAt( nBarPos-1 );
				} // else from if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
			} // if( nBarPos > 1 )
		} // else from if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )

		INT nCount = m_arrChildStates.GetSize();
		for( INT nBar = 0; nBar < nCount; nBar ++ )
		{
			InternalDockStateBar * pState = m_arrChildStates[nBar];
			ASSERT_VALID( pState );
			pState->StateSet( pDockSite, this );
		}
	
#ifdef _DEBUG
		INT nRealDockedCount =
			((CExtDynControlBar *)m_pHelperBar)
				-> m_pWndDynDocker
				-> GetDockedCount();
		ASSERT( nRealDockedCount == nCount );
#endif // _DEBUG

	}
	break; // case __DOCK_DYNAMIC_HALF_SPLIT
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	case __DOCK_DYNAMIC_TABBED:
	{
		CFrameWnd * pParentFrame = pDockSite;
		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CExtMiniDockFrameWnd * pMiniFrame =
				new CExtMiniDockFrameWnd;
			if( !pMiniFrame->Create(
					pDockSite,
					m_dwFloatingFrameCreateStyle
					)
				)
			{
				ASSERT( FALSE );
				AfxThrowResourceException();
			}
			CRect rcFrameAdjusted =
				CExtPaintManager::stat_AlignWndRectToMonitor(
					m_rcFloatingFrame
					);
			pMiniFrame->MoveWindow( &rcFrameAdjusted, FALSE );
			pMiniFrame->DelayRecalcLayout();
			pParentFrame = pMiniFrame;
		} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )

		DWORD dwDynStyle =
				WS_CHILD
				| (m_dwDockStateFlags & CBRS_ALIGN_ANY)
				;
		dwDynStyle |= CBRS_BORDER_ANY;
		dwDynStyle &= ~( CBRS_FLOATING | CBRS_GRIPPER );
		dwDynStyle |= CBRS_TOOLTIPS;

		m_pHelperBar = NULL;
		pDockSite->SendMessage(
			g_nMsgCreateTabbedBar,
			WPARAM( &m_pHelperBar )
			);
#ifdef _DEBUG
		if( m_pHelperBar != NULL )
		{
			ASSERT_VALID( m_pHelperBar );
			ASSERT_KINDOF( CExtDynTabControlBar, m_pHelperBar );
		} // if( m_pHelperBar != NULL )
#endif // _DEBUG
		if( m_pHelperBar == NULL )
			m_pHelperBar = new CExtDynTabControlBar;
		m_pHelperBar->SetInitDesiredSizeHorizontal( m_sizeDockedH );
		m_pHelperBar->SetInitDesiredSizeVertical( m_sizeDockedV );
		m_pHelperBar->SetInitDesiredSizeFloating( m_sizeFloated );
		if(	!m_pHelperBar->Create(
				NULL,
				pDockSite, // pParentFrame
				m_nBarID,
				dwDynStyle
				)
			)
		{
			ASSERT( FALSE );
			return;
		}
		ASSERT( ((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker != NULL );
		ASSERT( ((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->GetSafeHwnd() != NULL );
		ASSERT( ::IsWindow(((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->GetSafeHwnd()) );
		m_pHelperBar->m_pDockSite = pDockSite;
		((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_pDockSite = pDockSite;
		m_pHelperBar->m_hWndOwner = pDockSite->GetSafeHwnd();
		m_pHelperBar->EnableDocking( CBRS_ALIGN_ANY );
		ASSERT( m_pHelperBar->m_pDockContext != NULL );
		ASSERT( m_pHelperBar->m_pDockContext->m_pDockSite == pDockSite );
		ASSERT_VALID( m_pHelperBar );

		if( pDockSite->m_listControlBars.Find(m_pHelperBar) == NULL )
			pDockSite->m_listControlBars.AddTail( m_pHelperBar );

		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CExtMiniDockFrameWnd * pMiniFrame =
				STATIC_DOWNCAST(
					CExtMiniDockFrameWnd,
					pParentFrame
					);
			CDockBar * pFloatingDocker =
				STATIC_DOWNCAST(
					CDockBar,
					pMiniFrame->GetWindow(GW_CHILD)
					);
			ASSERT( !pFloatingDocker->IsKindOf(RUNTIME_CLASS(CExtDockBar)) );
			ASSERT( pFloatingDocker->GetDlgCtrlID() == AFX_IDW_DOCKBAR_FLOAT );
			m_pHelperBar->SetParent( pFloatingDocker );
		} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )

		if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )
		{
			POSITION posToRemove =
				pDockSite->m_listControlBars.Find(
					((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker
					);
			if( posToRemove != NULL )
			{
				pDockSite->m_listControlBars.RemoveAt( posToRemove );
				((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified = true;
			} // if( posToRemove != NULL )
		} // if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )

		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CDockBar * pDockBar =
				STATIC_DOWNCAST(
					CDockBar,
					pParentFrame->GetWindow( GW_CHILD )
					);
			ASSERT_KINDOF( CDockBar, pDockBar );
			ASSERT( !pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) );
			ASSERT( pDockBar->GetDlgCtrlID() ==	AFX_IDW_DOCKBAR_FLOAT );
			((CExtDockBar*)pDockBar)->DockControlBar( m_pHelperBar );
			ASSERT( pDockBar->IsFloating() );
			ASSERT( m_pHelperBar->IsFloating() );
			
			m_pHelperBar->m_bUpdatingChain = true;
			m_pHelperBar->MoveWindow( &m_rcBar, FALSE );
			m_pHelperBar->m_bUpdatingChain = false;
		} // if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		else if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )
		{
			ASSERT_VALID( pParentState );
			ASSERT_VALID( pParentState->m_pHelperBar );
			ASSERT_KINDOF( CExtDynControlBar, m_pHelperBar );

			CExtDockBar * pDockBar =
				STATIC_DOWNCAST(
					CExtDockBar,
					(	STATIC_DOWNCAST(
							CExtDynControlBar,
							pParentState->m_pHelperBar
							)
						)->m_pWndDynDocker
					);

			pDockBar->_LockSequenceOptimization( true );
			
			pDockBar->DockControlBar( m_pHelperBar );
			
			m_pHelperBar->m_bUpdatingChain = true;
			m_pHelperBar->MoveWindow( &m_rcBar, FALSE );
			m_pHelperBar->m_bUpdatingChain = false;
			
			pDockBar->_LockSequenceOptimization( false );

			INT nBarPos = 
				pDockBar->FindBar( m_pHelperBar );
			ASSERT( nBarPos >= 1 );
			if( nBarPos > 1 )
			{
				if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				{
					if( pDockBar->m_arrBars[nBarPos-1] != NULL  )
						pDockBar->m_arrBars.InsertAt(
							nBarPos,
							(CControlBar *)NULL
							);
				} // if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				else
				{
					if( pDockBar->m_arrBars[nBarPos-1] == NULL  )
						pDockBar->m_arrBars.RemoveAt( nBarPos-1 );
				} // else from if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
			} // if( nBarPos > 1 )
		} // else if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )
		else
		{
			ASSERT( m_nCircleNo > 0 );
			ASSERT( m_pHelperBar->m_pDockContext != NULL );

			CExtDockBar * pDockBar =
				STATIC_DOWNCAST(
					CExtDockBar,
					pDockSite->GetControlBar( m_nDockBarID )
					);
			pDockBar =
				pDockBar->_GetBarByCircleNo( m_nCircleNo );

			pDockBar->_LockSequenceOptimization( true );
			
			pDockBar->DockControlBar( m_pHelperBar );
			
			m_pHelperBar->m_bUpdatingChain = true;
			m_pHelperBar->MoveWindow( &m_rcBar, FALSE );
			m_pHelperBar->m_bUpdatingChain = false;
			
			pDockBar->_LockSequenceOptimization( false );

			INT nBarPos = 
				pDockBar->FindBar( m_pHelperBar );
			ASSERT( nBarPos >= 1 );
			if( nBarPos > 1 )
			{
				if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				{
					if( pDockBar->m_arrBars[nBarPos-1] != NULL  )
						pDockBar->m_arrBars.InsertAt(
							nBarPos,
							(CControlBar *)NULL
							);
				} // if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
				else
				{
					if( pDockBar->m_arrBars[nBarPos-1] == NULL  )
						pDockBar->m_arrBars.RemoveAt( nBarPos-1 );
				} // else from if( (m_dwDockStateFlags & __STATE_NEW_ROW) != 0 )
			} // if( nBarPos > 1 )
		} // else from if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_DYNAMIC_CHILD) != 0 )

		INT nCount = m_arrChildStates.GetSize();
		//ASSERT( nCount > 1 );
		m_nHelperAciveTabChildIndex = -1;
		for( INT nBar = 0; nBar < nCount; nBar ++ )
		{
			InternalDockStateBar * pState = m_arrChildStates[nBar];
			ASSERT_VALID( pState );
			if( (pState->m_dwDockStateFlags & InternalDockStateBar::__STATE_TAB_ACTIVE) != 0 )
			{
				if( (pState->m_dwDockStateFlags & InternalDockStateBar::__STATE_VISIBLE) != 0 )
				{
					ASSERT( m_nHelperAciveTabChildIndex == -1 );
					m_nHelperAciveTabChildIndex = nBar;
				}
			}
			pState->StateSet( pDockSite, this );
		}

#ifdef _DEBUG
		INT nRealDockedCount =
			((CExtDynTabControlBar *)m_pHelperBar)
				-> m_pWndDynDocker
				-> GetDockedCount();
		ASSERT( nRealDockedCount == nCount );
#endif // _DEBUG

	}
	break; // case __DOCK_DYNAMIC_TABBED
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

#ifdef _DEBUG
	default:
	{
		ASSERT( FALSE );
	}
	break;
#endif // _DEBUG
	} // switch( (m_dwDockStateFlags & __DOCK_AT_MASK) )

	ASSERT_VALID( m_pHelperBar );

//bool bBarWndVisible = ( ( m_pHelperBar->GetStyle() & WS_VISIBLE ) != 0 ) ? true : false;
//	if( (m_dwDockStateFlags & __STATE_VISIBLE) != 0 )
//	{
//		if( !bBarWndVisible )
//			pDockSite->ShowControlBar( m_pHelperBar, TRUE, TRUE );
//	} // if( (m_dwDockStateFlags & __STATE_VISIBLE) != 0 )
//	else
//	{
//		if( bBarWndVisible )
//			pDockSite->ShowControlBar( m_pHelperBar, FALSE, TRUE );
//	} // else from if( (m_dwDockStateFlags & __STATE_VISIBLE) != 0 )

	pDockSite->ShowControlBar(
		m_pHelperBar,
		(m_dwDockStateFlags & __STATE_VISIBLE) ? TRUE : FALSE,
		TRUE
		);

CExtMiniDockFrameWnd * pMiniFrame =
		DYNAMIC_DOWNCAST(
			CExtMiniDockFrameWnd,
			m_pHelperBar->GetParentFrame()
			);
	if( pMiniFrame != NULL )
	{
		ASSERT_VALID( pMiniFrame );
		INT nCount = pMiniFrame->m_listControlBars.GetCount();
		if( nCount != 0 )
		{
			ASSERT( nCount == 1 );
			pMiniFrame->m_listControlBars.RemoveAll();
		}

		CRect rcFrameInitial;
		pMiniFrame->GetWindowRect( &rcFrameInitial );
		CRect rcFrameAdjusted =
			CExtPaintManager::stat_AlignWndRectToMonitor(
				rcFrameInitial
				);
		if( rcFrameAdjusted != rcFrameInitial )
		{
			pMiniFrame->MoveWindow( &rcFrameAdjusted, FALSE );
			pMiniFrame->DelayRecalcLayout();
		} // if( rcFrameAdjusted != rcFrameInitial )
	} // if( pMiniFrame != NULL )


	m_pHelperBar->m_dwStyle = m_dwBarFlags;

	m_pHelperBar->m_ptFloatHelper = m_ptFloatPos;

	m_pHelperBar->m_sizeDockedH = m_sizeDockedH;
	m_pHelperBar->m_sizeDockedV = m_sizeDockedV;
	m_pHelperBar->m_sizeFloated = m_sizeFloated;

	m_pHelperBar->m_nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;

	m_pHelperBar->m_nMRUWidth = m_nMRUWidth;

	m_pHelperBar->m_pDockContext->m_rectMRUDockPos = m_rcMRUDockPos;
	m_pHelperBar->m_pDockContext->m_dwMRUFloatStyle = m_dwMRUFloatStyle;
	m_pHelperBar->m_pDockContext->m_ptMRUFloatPos = m_ptMRUFloatPos;

//	if( m_dwDockStateFlags & __STATE_AUTOHIDDEN
//		&& !m_pHelperBar->AutoHideModeGet()
//		)
//	{
//		VERIFY(
//			m_pHelperBar->AutoHideModeSet(
//				true,
//				false,
//				false,
//				false
//				)
//			);
//	}
}

void CExtControlBar::InternalDockStateBar::PreSyncAttributes(
	CFrameWnd * pDockSite,
	CExtControlBar::InternalDockStateBar * pParentState
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pHelperBar );
	pParentState;

	m_pHelperBar->m_ptFloatHelper = m_ptFloatPos;

	m_pHelperBar->m_sizeDockedH = m_sizeDockedH;
	m_pHelperBar->m_sizeDockedV = m_sizeDockedV;
	m_pHelperBar->m_sizeFloated = m_sizeFloated;

	m_pHelperBar->m_nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;

	m_pHelperBar->m_nMRUWidth = m_nMRUWidth;

	m_pHelperBar->m_pDockContext->m_rectMRUDockPos = m_rcMRUDockPos;
	m_pHelperBar->m_pDockContext->m_dwMRUFloatStyle = m_dwMRUFloatStyle;
	m_pHelperBar->m_pDockContext->m_ptMRUFloatPos = m_ptMRUFloatPos;

	INT nCount = m_arrChildStates.GetSize();
	for( INT nBar = 0; nBar < nCount; nBar++ )
	{
		InternalDockStateBar * pState = m_arrChildStates[nBar];
		ASSERT_VALID( pState );
		pState->PreSyncAttributes( pDockSite, this );
	}
}

void CExtControlBar::InternalDockStateBar::PostSyncAttributes(
	CFrameWnd * pDockSite,
	CExtControlBar::InternalDockStateBar * pParentState
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pHelperBar );
	pParentState;

bool bWalkChild = false;
	switch( (m_dwDockStateFlags & __DOCK_AT_MASK) )
	{
	case __DOCK_AT_NOWHERE:
	{
	}
	break; // case __DOCK_AT_NOWHERE
	case __DOCK_AT_SIDE:
	{
	}
	break; // case __DOCK_AT_SIDE
	case __DOCK_DYNAMIC_HALF_SPLIT:
	{
		bWalkChild = true;
		ASSERT_KINDOF( CExtDynControlBar, m_pHelperBar );
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		ASSERT( !m_pHelperBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar)) );
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )
		{
			POSITION posToRemove =
				pDockSite->m_listControlBars.Find(
					((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker
					);
			if( posToRemove != NULL )
			{
				pDockSite->m_listControlBars.RemoveAt( posToRemove );
				((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified = true;
			} // if( posToRemove != NULL )
		} // if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )

		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CExtMiniDockFrameWnd * pParentFrame =
				STATIC_DOWNCAST(
					CExtMiniDockFrameWnd,
					m_pHelperBar->GetParentFrame()
					);
			pParentFrame->RecalcLayout();
		}

#ifdef _DEBUG
		INT nChildsCount = m_arrChildStates.GetSize();
		INT nRealDockedCount =
			((CExtDynControlBar *)m_pHelperBar)
				-> m_pWndDynDocker
				-> GetDockedCount();
		ASSERT( nRealDockedCount == nChildsCount );
#endif // _DEBUG
	}
	break; // case __DOCK_DYNAMIC_HALF_SPLIT
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	case __DOCK_DYNAMIC_TABBED:
	{
		bWalkChild = true;
		ASSERT_KINDOF( CExtDynTabControlBar, m_pHelperBar );

		if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )
		{
			POSITION posToRemove =
				pDockSite->m_listControlBars.Find(
					((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker
					);
			if( posToRemove != NULL )
			{
				pDockSite->m_listControlBars.RemoveAt( posToRemove );
				((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified = true;
			} // if( posToRemove != NULL )
		} // if( !((CExtDynControlBar *)m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified )

		if( (m_dwDockStateFlags & InternalDockStateBar::__STATE_FLOATING) != 0 )
		{
			CExtMiniDockFrameWnd * pParentFrame =
				STATIC_DOWNCAST(
					CExtMiniDockFrameWnd,
					m_pHelperBar->GetParentFrame()
					);
			pParentFrame->RecalcLayout();
		}

		if( m_nHelperAciveTabChildIndex >= 0 )
		{
			//ASSERT( m_pHelperBar->GetStyle() & WS_VISIBLE );
			if( (m_pHelperBar->GetStyle() & WS_VISIBLE) == 0 )
				m_pHelperBar->ShowWindow( SW_SHOWNA );

			CExtDynTabControlBar * pTabbedBar =
				STATIC_DOWNCAST(
					CExtDynTabControlBar,
					m_pHelperBar
					);
			LONG nSel = 
				pTabbedBar->GetSwitcherSelection();
			if(		nSel != m_nHelperAciveTabChildIndex
				&&	m_nHelperAciveTabChildIndex >= 0
				&&	pTabbedBar->GetBarAt(m_nHelperAciveTabChildIndex,true)->IsVisible()
				)
				pTabbedBar->SetSwitcherSelection(
					m_nHelperAciveTabChildIndex,
					true,
					true
					);
		}
	
#ifdef _DEBUG
		INT nChildsCount = m_arrChildStates.GetSize();
		INT nRealDockedCount =
			((CExtDynTabControlBar *)m_pHelperBar)
				-> m_pWndDynDocker
				-> GetDockedCount();
		ASSERT( nRealDockedCount == nChildsCount );
#endif // _DEBUG
	}
	break; // case __DOCK_DYNAMIC_TABBED
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

#ifdef _DEBUG
	default:
	{
		ASSERT( FALSE );
	}
	break;
#endif // _DEBUG
	} // switch( (m_dwDockStateFlags & __DOCK_AT_MASK) )

	if( bWalkChild )
	{
		INT nCount = m_arrChildStates.GetSize();
		ASSERT( nCount > 0 );
		for( INT nBar = 0; nBar < nCount; nBar++ )
		{
			InternalDockStateBar * pState = m_arrChildStates[nBar];
			ASSERT_VALID( pState );
			pState->PostSyncAttributes( pDockSite, this );
		}
	}





	m_pHelperBar->m_ptFloatHelper = m_ptFloatPos;

	m_pHelperBar->m_sizeDockedH = m_sizeDockedH;
	m_pHelperBar->m_sizeDockedV = m_sizeDockedV;
	m_pHelperBar->m_sizeFloated = m_sizeFloated;

	m_pHelperBar->m_nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;

	m_pHelperBar->m_nMRUWidth = m_nMRUWidth;

	m_pHelperBar->m_pDockContext->m_rectMRUDockPos = m_rcMRUDockPos;
	m_pHelperBar->m_pDockContext->m_dwMRUFloatStyle = m_dwMRUFloatStyle;
	m_pHelperBar->m_pDockContext->m_ptMRUFloatPos = m_ptMRUFloatPos;
}

bool CExtControlBar::ProfileBarStateSerialize(
	CArchive & ar,
	CFrameWnd * pFrame,
	LPWINDOWPLACEMENT pFrameWp // = NULL // need serialize frame's WP
	)
{
	ASSERT( pFrame != NULL );
	ASSERT(
		::IsWindow( pFrame->GetSafeHwnd() )
		);
	ASSERT_VALID( pFrame );

bool bRetVal = false;
	try
	{
		InternalDockStateSite state( pFrame );

		if( ar.IsStoring() )
		{
			((InternalFriendlyFrameWnd *)pFrame)->
				GetDockState(
					state
					);
			state.Serialize( ar );
		} // if( ar.IsStoring() )
		else
		{
			// win xp fix - begin
			WINDOWPLACEMENT _wpf;
			::memset( &_wpf, 0, sizeof(WINDOWPLACEMENT) );
			_wpf.length = sizeof(WINDOWPLACEMENT);
			if( pFrame->GetWindowPlacement(&_wpf) )
			{
				_wpf.ptMinPosition.x = _wpf.ptMinPosition.y = 0;
				_wpf.ptMaxPosition.x = _wpf.ptMaxPosition.y = 0;
				_wpf.showCmd = 
					(pFrame->GetStyle() & WS_VISIBLE)
						? SW_SHOWNA
						: SW_HIDE;
				pFrame->SetWindowPlacement(&_wpf);
			} // if( pFrame->GetWindowPlacement(&_wpf) )
			// win xp fix - end
			bool bPresetWP =
				( pFrameWp != NULL ) ? true : false;
			state.Serialize( ar );
			((InternalFriendlyFrameWnd *)pFrame)->SetDockState(
				state,
				bPresetWP
				);
		} // else from if( ar.IsStoring() )

		if( pFrameWp != NULL )
			::memcpy(
				pFrameWp,
				&state.m_wp,
				sizeof(WINDOWPLACEMENT)
				);
		// almost OK
		bRetVal = true;
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )

	return bRetVal;
}

LRESULT CExtControlBar::OnSetText(WPARAM wParam, LPARAM lParam)
{
    wParam;
LRESULT lResult = CWnd::Default();
	if(	IsFloating() )
	{
		CFrameWnd * pFrame = GetParentFrame();
		if(		pFrame != NULL
			&&	pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd))
			)
		{
			m_pDockBar->SetWindowText( (LPTSTR) lParam );
			pFrame->SetWindowText( (LPTSTR) lParam );
			pFrame->DelayRecalcLayout();
			pFrame->SendMessage( WM_NCPAINT );
		}
	}

bool bTextOnGripper = false;
	if( IsBarWithGripper( NULL, &bTextOnGripper ) )
	{
		if( bTextOnGripper )
			SendMessage( WM_NCPAINT );
	}
	return lResult;
}

static void stat_AddUnique(
	ExtControlBarVector_t & vBars,
	CExtControlBar * pBar
	)
{
	ASSERT( pBar != NULL );
INT nCount = vBars.GetSize();
	for( INT nBar = 0; nBar < nCount; nBar++ )
	{
		CExtControlBar * pAddedBar = vBars[ nBar ];
		ASSERT( pAddedBar != NULL );
		if( pAddedBar == pBar )
			return;
	}
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( pBar->m_pDockBar != NULL )
	{
		if( pBar->m_pDockBar->IsKindOf( RUNTIME_CLASS(CExtDockDynTabBar) ) )
		{
			CExtDynTabControlBar * pTabbedBar =
				STATIC_DOWNCAST(
					CExtDynTabControlBar,
					pBar->m_pDockBar->GetParent()
					);
			stat_AddUnique( vBars, pTabbedBar );
			return;
		}
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	vBars.Add( pBar );
}

void CExtControlBar::_GetFrameControlBars(
	CFrameWnd * pFrame,
	ExtControlBarVector_t & vBars,
	DWORD dwCallOpt, // = ((DWORD)__GFECB_ALL_NONRESTRICT)
	CPoint ptRestrict // = CPoint( -1000, -1000 )
	)
{
	ASSERT( pFrame != NULL );
	ASSERT( pFrame->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( pFrame->GetSafeHwnd() ) );
	ASSERT_VALID( pFrame );
	vBars.RemoveAll();
	if( ( dwCallOpt & 
			( DWORD(__GFECB_FIXED) | DWORD(__GFECB_NONFIXED) )
			) == 0
		)
		return; // empty query
	if( ( dwCallOpt & 
			( DWORD(__GFECB_BAR_VISIBLE) | DWORD(__GFECB_BAR_INVISIBLE) )
			) == 0
		)
		return; // empty query
	if( ( dwCallOpt & 
			( DWORD(__GFECB_WND_VISIBLE) | DWORD(__GFECB_WND_INVISIBLE) )
			) == 0
		)
		return; // empty query
CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
int nAllBarsCount = g_AllBars.GetSize();
	for( INT nBar = 0; nBar < nAllBarsCount; nBar++ )
	{
		CExtControlBar * pBar = g_AllBars[nBar];
		ASSERT_KINDOF( CExtControlBar, pBar );
		if( pBar->GetSafeHwnd() == NULL )
			continue;
		if( !(::IsWindow(pBar->GetSafeHwnd())) )
			continue;
		if( pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			&& ( !pBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar)) )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			)
			continue;
		if( pBar->m_bPresubclassDialogMode )
			continue;
		CFrameWnd * pBarFrame =
			_GetDockingFrameImpl( pBar );
		ASSERT( pBarFrame != NULL );
		ASSERT( pBarFrame->GetSafeHwnd() != NULL );
		ASSERT( ::IsWindow( pBarFrame->GetSafeHwnd() ) );
		ASSERT_VALID( pBarFrame );
		if( pBarFrame != pFrame )
			continue;
		if( dwCallOpt == ((DWORD)__GFECB_ALL_NONRESTRICT) )
		{
			//stat_AddUnique( vBars, pBar );
			vBars.Add( pBar );
			continue;
		}
		if( (dwCallOpt&((DWORD)__GFECB_FIXED)) == 0 )
		{
			if( pBar->IsFixedMode() )
				continue;
		}
		if( !pBar->IsFixedMode() )
		{
			if( (dwCallOpt&((DWORD)__GFECB_NONFIXED)) == 0 )
				continue;
			bool bMinOnRow = pBar->IsMinimizedOnRow();
			if( (dwCallOpt&((DWORD)__GFECB_NONFIXED_MIN)) == 0 )
			{
				if( bMinOnRow )
					continue;
			}
			if( (dwCallOpt&((DWORD)__GFECB_NONFIXED_NONMIN)) == 0 )
			{
				if( !bMinOnRow )
					continue;
			}
		}
		if( pBar->IsVisible() )
		{
			if( (dwCallOpt&((DWORD)__GFECB_BAR_VISIBLE)) == 0 )
				continue;
		}
		else
		{
			if( (dwCallOpt&((DWORD)__GFECB_BAR_INVISIBLE)) == 0 )
				continue;
		}
		if( pBar->IsWindowVisible() )
		{
			if( (dwCallOpt&((DWORD)__GFECB_WND_VISIBLE)) == 0 )
				continue;
		}
		else
		{
			if( (dwCallOpt&((DWORD)__GFECB_WND_INVISIBLE)) == 0 )
				continue;
		}

		if( pBar->IsFloating() )
		{
			if( (dwCallOpt&((DWORD)__GFECB_FLOATED)) == 0 )
				continue;
		} // if( pBar->IsFloating() )
		else
		{
			if( (dwCallOpt&((DWORD)__GFECB_DOCKED)) == 0 )
				continue;
			if( (dwCallOpt&((DWORD)__GFECB_DOCKED)) !=
					((DWORD)__GFECB_DOCKED)
				)
			{ // if not all docked bars
				UINT nDockBarID = pBar->GetSafeDockBarDlgCtrlID();
				switch( nDockBarID )
				{
				case AFX_IDW_DOCKBAR_TOP:
					if( (dwCallOpt&((DWORD)__GFECB_DOCKED_TOP)) == 0 )
						continue;
				break;
				case AFX_IDW_DOCKBAR_BOTTOM:
					if( (dwCallOpt&((DWORD)__GFECB_DOCKED_BOTTOM)) == 0 )
						continue;
				break;
				case AFX_IDW_DOCKBAR_LEFT:
					if( (dwCallOpt&((DWORD)__GFECB_DOCKED_LEFT)) == 0 )
						continue;
				break;
				case AFX_IDW_DOCKBAR_RIGHT:
					if( (dwCallOpt&((DWORD)__GFECB_DOCKED_RIGHT)) == 0 )
						continue;
				break;
#ifdef _DEBUG
				default:
					ASSERT( FALSE );
				break;
#endif // _DEBUG
				} // switch( nDockBarID )
			} // if not all docked bars
		} // else from if( pBar->IsFloating() )

		// restriction by point
		if( (dwCallOpt&((DWORD)__GFECB_BYPT_ALL)) != 0 )
		{ // if restriction by point
			CRect rcWnd;
			CWnd * pWndNcTest = pBar;
			if( pBar->IsFloating() )
			{
				pWndNcTest = pBar->GetDockingFrame();
				ASSERT( pWndNcTest != NULL );
				ASSERT( pWndNcTest->GetSafeHwnd() != NULL );
				ASSERT( ::IsWindow( pWndNcTest->GetSafeHwnd() ) );
				ASSERT_VALID( pWndNcTest );
				ASSERT_KINDOF( CMiniDockFrameWnd, pWndNcTest );
			}
			pWndNcTest->GetWindowRect( &rcWnd );
			BOOL bInsideWndArea =
				rcWnd.PtInRect( ptRestrict );
			if( !bInsideWndArea )
				continue;
			
			if( (dwCallOpt&((DWORD)__GFECB_BYPT_ONLY_WND)) != 0 )
			{
				stat_AddUnique( vBars, pBar );
				//continue;
				_slCsCB.Unlock();
				return;
			}
			
			if( (dwCallOpt&((DWORD)__GFECB_BYPT_NCGRIP)) != 0 )
			{
				if( pBar->m_rcGrip.IsRectEmpty() )
					continue;
				CPoint ptWnd( ptRestrict );
				ptWnd -= rcWnd.TopLeft();
				if( !pBar->m_rcGrip.PtInRect(ptWnd) )
					continue;
			}

			if( (dwCallOpt&((DWORD)__GFECB_BYPT_CLIENT)) != 0 )
			{
				CRect rcClient;
				pBar->GetClientRect( &rcClient );
				pBar->ClientToScreen( &rcClient );
				if( !rcClient.PtInRect( ptRestrict ) )
					continue;
			}

			if( (dwCallOpt&((DWORD)__GFECB_BYPT_NC)) != 0 )
			{ // if any nc hit test restriction
				UINT nHitTest =
					pWndNcTest->SendMessage(
						WM_NCHITTEST,
						WPARAM(0),
						MAKELPARAM(ptRestrict.x,ptRestrict.y)
						);
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTLEFT)) != 0 )
				{
					if( nHitTest != HTLEFT )
						continue;
				}
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTRIGHT)) != 0 )
				{
					if( nHitTest != HTRIGHT )
						continue;
				}
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTTOP)) != 0 )
				{
					if( nHitTest != HTTOP )
						continue;
				}
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTBOTTOM)) != 0 )
				{
					if( nHitTest != HTBOTTOM )
						continue;
				}

				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTTOPLEFT)) != 0 )
				{
					if( nHitTest != HTTOPLEFT )
						continue;
				}
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTTOPRIGHT)) != 0 )
				{
					if( nHitTest != HTTOPRIGHT )
						continue;
				}
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTBOTTOMLEFT)) != 0 )
				{
					if( nHitTest != HTBOTTOMLEFT )
						continue;
				}
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTBOTTOMRIGHT)) != 0 )
				{
					if( nHitTest != HTBOTTOMRIGHT )
						continue;
				}

				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTNOWHERE)) != 0 )
				{
					if( nHitTest != HTNOWHERE )
						continue;
				}
				
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTSYSMENU)) != 0 )
				{
					if( nHitTest != HTSYSMENU )
						continue;
				}
				
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTGROWBOX)) != 0 )
				{
					if( nHitTest != HTGROWBOX )
						continue;
				}
				
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTMINBUTTON)) != 0 )
				{
					if( nHitTest != HTMINBUTTON )
						continue;
				}
				
				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTMAXBUTTON)) != 0 )
				{
					if( nHitTest != HTMAXBUTTON )
						continue;
				}

				if( (dwCallOpt&((DWORD)__GFECB_BYPT_HTCLIENT)) != 0 )
				{
					if( nHitTest != HTCLIENT )
						continue;
				}
				
			} // if any nc hit test restriction
		} // if restriction by point

		stat_AddUnique( vBars, pBar );
	} // for( INT nBar = 0; nBar < nAllBarsCount; nBar++ )

	_slCsCB.Unlock();
}

void CExtControlBar::_GetFrameControlBars(
	ExtControlBarVector_t & vBars,
	DWORD dwCallOpt, // = ((DWORD)__GFECB_ALL_NONRESTRICT)
	CPoint ptRestrict // = CPoint( -1000, -1000 )
	) const
{
CFrameWnd * pFrame = _GetDockingFrameImpl();
	_GetFrameControlBars(
		pFrame,
		vBars,
		dwCallOpt,
		ptRestrict
		);
}

CControlBar * CExtControlBar::_GetNextControlBarInRow(
	CControlBar * pBar,
	bool bNext, // = true // next or previous
	bool bExtBar, // = false
	bool bNonFixed // = false
	)
{
	ASSERT( pBar != NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	ASSERT_VALID( pBar );
	ASSERT( !pBar->IsFloating() );
	ASSERT_VALID( pBar->m_pDockBar );
INT nPos = pBar->m_pDockBar->FindBar( pBar );
	ASSERT( nPos >= 0 );
	if( nPos == 0 && !bNext )
		return NULL; // first in first row
INT nCountOfSideBars = pBar->m_pDockBar->m_arrBars.GetSize();
	ASSERT( nPos < nCountOfSideBars );
	if( nPos == (nCountOfSideBars-1) && bNext )
		return NULL; // last in last row
	if( bNext )
	{
		for( nPos++; nPos < nCountOfSideBars; nPos++ )
		{
			CControlBar * pExamineBar = (CControlBar *)
				pBar->m_pDockBar->m_arrBars[ nPos ];
			ASSERT( pExamineBar != pBar );
			if( pExamineBar == NULL )
				return NULL; // row separator
			if( __PLACEHODLER_BAR_PTR(pExamineBar) )
				continue;
			if( bExtBar )
			{
				if( !pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
					continue;
				if( bNonFixed )
				{
					if( ((CExtControlBar *)pBar)->IsFixedMode() )
						continue;
				}
			}
			return pExamineBar;
		} // for( nPos++; nPos < nCountOfSideBars; nPos++ )
	} // if( bNext )
	else
	{
		for( nPos--; nPos >= 0; nPos-- )
		{
			CControlBar * pExamineBar = (CControlBar *)
				pBar->m_pDockBar->m_arrBars[ nPos ];
			ASSERT( pExamineBar != pBar );
			if( pExamineBar == NULL )
				return NULL; // row separator
			if( __PLACEHODLER_BAR_PTR(pExamineBar) )
				continue;
			if( bExtBar )
			{
				if( !pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
					continue;
				if( bNonFixed )
				{
					if( ((CExtControlBar *)pBar)->IsFixedMode() )
						continue;
				}
			}
			return pExamineBar;
		} // for( nPos--; nPos >= 0; nPos-- )
	} // else from if( bNext )
	return NULL;
}

CControlBar * CExtControlBar::_GetFirstControlBarInRow(
	CControlBar * pBar,
	bool bNext, // = false // last or first
	bool bExtBar, // = false
	bool bNonFixed // = false
	)
{
	ASSERT( pBar != NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	ASSERT_VALID( pBar );
	ASSERT( !pBar->IsFloating() );
	ASSERT_VALID( pBar->m_pDockBar );
INT nPos = pBar->m_pDockBar->FindBar( pBar );
	ASSERT( nPos > 0 );
	if( nPos == 1 && !bNext )
	{
		// first in first row
		if( bExtBar )
		{
			if( !pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			{
				CControlBar * pBar2 =
					_GetNextControlBarInRow(
						pBar,true,bExtBar,bNonFixed );
				return ( pBar2 == NULL ) ? pBar : pBar2;
			}
			if( bNonFixed
				&& ((CExtControlBar *)pBar)->IsFixedMode()
				)
			{
				CControlBar * pBar2 =
					_GetNextControlBarInRow(
						pBar,true,bExtBar,bNonFixed );
				return ( pBar2 == NULL ) ? pBar : pBar2;
			}
		}
		return pBar;
	} // if( nPos == 1 && !bNext )
INT nCountOfSideBars = pBar->m_pDockBar->m_arrBars.GetSize();
	ASSERT( nPos <= nCountOfSideBars );
	if( nPos == (nCountOfSideBars-1) && bNext )
	{
		// last in last row
		if( bExtBar )
		{
			if( !pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			{
				CControlBar * pBar2 =
					_GetNextControlBarInRow(
						pBar,false,bExtBar,bNonFixed );
				return ( pBar2 == NULL ) ? pBar : pBar2;
			}
			if( bNonFixed
				&& ((CExtControlBar *)pBar)->IsFixedMode()
				)
			{
				CControlBar * pBar2 =
					_GetNextControlBarInRow(
						pBar,false,bExtBar,bNonFixed );
				return ( pBar2 == NULL ) ? pBar : pBar2;
			}
		}
		return pBar;
	}
CControlBar * pPrevBar = pBar;
	if( bNext )
	{
		for( nPos++; nPos < nCountOfSideBars; nPos++ )
		{
			CControlBar * pExamineBar = (CControlBar *)
				pBar->m_pDockBar->m_arrBars[ nPos ];
			ASSERT( pExamineBar != pBar );
			if( pExamineBar == NULL )
				return pPrevBar; // row separator
			if( __PLACEHODLER_BAR_PTR(pExamineBar) )
				continue;
			if( bExtBar )
			{
				if( !pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
					continue;
				if( bNonFixed )
				{
					if( ((CExtControlBar *)pBar)->IsFixedMode() )
						continue;
				}
			}
			pPrevBar = pExamineBar;
		} // for( nPos++; nPos < nCountOfSideBars; nPos++ )
	} // if( bNext )
	else
	{
		for( nPos--; nPos >= 0; nPos-- )
		{
			CControlBar * pExamineBar = (CControlBar *)
				pBar->m_pDockBar->m_arrBars[ nPos ];
			ASSERT( pExamineBar != pBar );
			if( pExamineBar == NULL )
				return pPrevBar; // row separator
			if( __PLACEHODLER_BAR_PTR(pExamineBar) )
				continue;
			if( bExtBar )
			{
				if( !pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
					continue;
				if( bNonFixed )
				{
					if( ((CExtControlBar *)pBar)->IsFixedMode() )
						continue;
				}
			}
			pPrevBar = pExamineBar;
		} // for( nPos--; nPos >= 0; nPos-- )
	} // else from if( bNext )
	return pPrevBar;
}

void CExtControlBar::_GetNextRowBars(
	bool bNextRow,
	MfcControlBarVector_t & vBars,
	CRect * pRcAllBars, // = NULL
	INT * p_nOwnPos, // = NULL
	bool * p_bRowMaxSized, // = NULL // row with resizable bars?
	bool * p_bRowFullSized // = NULL // row with menu-like bar?
	) const
{
	ASSERT_VALID( this );
	if( pRcAllBars != NULL )
		pRcAllBars->SetRect( 0, 0, 0, 0 );
	if( p_nOwnPos != NULL )
		*p_nOwnPos = -1;
	if( p_bRowMaxSized != NULL )
		*p_bRowMaxSized = false;
	if( p_bRowFullSized != NULL )
		*p_bRowFullSized = false;
    vBars.RemoveAll();
	if( m_pDockBar == NULL )
		return;
	ASSERT_VALID( m_pDockBar );
INT nOwnIdx = m_pDockBar->FindBar( (CControlBar *)this );
	ASSERT( nOwnIdx >= 0 );
	if( nOwnIdx < 0 )
		return;
	if( p_nOwnPos != NULL )
		*p_nOwnPos = nOwnIdx;
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
INT nFirstToReview = -1;
INT nIncrement = bNextRow ? 1 : -1;
	for( INT nBar = nOwnIdx+nIncrement; true; nBar += nIncrement )
	{
		if( nBar == 0 || nBar == nTotalDockCount )
			return;
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			nFirstToReview = nBar + nIncrement;
			if( nFirstToReview >= nTotalDockCount )
				return;
			break;
		}
#ifdef _DEBUG
		if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT_KINDOF( CControlBar, pBar );
#endif // _DEBUG
	} // for( INT nBar = nOwnIdx+nIncrement; true; nBar += nIncrement )
	
	if( nFirstToReview <= 0 )
		return;
//	ASSERT( 0 < nFirstToReview && nFirstToReview < nTotalDockCount );
	ASSERT( 0 <= nFirstToReview && nFirstToReview < nTotalDockCount );

INT nBarWidth = 0;
bool bHorz = IsDockedHorizontally();

	for( nBar = nFirstToReview; true; nBar += nIncrement )
	{
		if( nBar < 0 || nBar >= nTotalDockCount )
			break;
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			if( vBars.GetSize() == 0 )
			{
				nBarWidth = 0;
				if( pRcAllBars != NULL )
					pRcAllBars->SetRect( 0, 0, 0, 0 );
				if( p_bRowMaxSized != NULL )
					*p_bRowMaxSized = false;
				if( p_bRowFullSized != NULL )
					*p_bRowFullSized = false;
				continue; // skip invisible or placeholder's row
			}
			break;
		}
		if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT_KINDOF( CControlBar, pBar );
		ASSERT( pBar != (CControlBar *)this );

		if( !pBar->IsVisible() )
			continue;
		
		if( pRcAllBars != NULL )
		{
			if( vBars.GetSize() == 0 )
				pBar->GetWindowRect( pRcAllBars );
			else
			{
				CRect rcWndBar;
				pBar->GetWindowRect( &rcWndBar );
				pRcAllBars->left =
					min( pRcAllBars->left, rcWndBar.left );
				pRcAllBars->top =
					min( pRcAllBars->top, rcWndBar.top );
				pRcAllBars->right =
					max( pRcAllBars->right, rcWndBar.right );
				pRcAllBars->bottom =
					max( pRcAllBars->bottom, rcWndBar.bottom );
			}
		} // if( pRcAllBars != NULL )

		if( (p_bRowMaxSized != NULL || p_bRowFullSized != NULL)
			&& pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
			)
		{
			if( p_bRowMaxSized != NULL
				&& ( ! (*p_bRowMaxSized) )
				&& !((CExtControlBar*)pBar)->IsFixedMode()
				)
				*p_bRowMaxSized = true;
			if(	p_bRowFullSized != NULL
				&& ( ! (*p_bRowFullSized) )
				&& pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
				&& ((CExtControlBar *)pBar)->_GetFullRowMode()
				)
				*p_bRowFullSized = true;
			if( p_bRowFullSized != NULL
				&& ( ! (*p_bRowFullSized) )
				)
			{
				CRect rcBarWnd;
				pBar->GetWindowRect( &rcBarWnd );
				nBarWidth +=
					bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
			}
		}
		vBars.Add( pBar );
	} // for( nBar = nFirstToReview; true; nBar += nIncrement )

	if( p_bRowFullSized != NULL
		&& ( ! (*p_bRowFullSized) )
		)
	{
		ASSERT_VALID( m_pDockBar );
		CRect rcDockBarWnd;
		m_pDockBar->GetWindowRect( &rcDockBarWnd );
		INT nDockBarWidth = bHorz ? rcDockBarWnd.Width() : rcDockBarWnd.Height();
		nDockBarWidth --;
		if( nBarWidth >= nDockBarWidth )
			*p_bRowFullSized = true;
	}
}

void CExtControlBar::_GetRowBars(
	MfcControlBarVector_t & vBars,
	CRect * pRcAllBars, // = NULL
	INT * p_nOwnPos, // = NULL
	bool * p_bRowMaxSized, // = NULL // row with resizable bars?
	bool * p_bRowFullSized // = NULL // row with menu-like bar?
	) const
{
	ASSERT_VALID( this );
	if( pRcAllBars != NULL )
		pRcAllBars->SetRect( 0, 0, 0, 0 );
	if( p_nOwnPos != NULL )
		*p_nOwnPos = -1;
	if( p_bRowMaxSized != NULL )
		*p_bRowMaxSized = false;
	if( p_bRowFullSized != NULL )
		*p_bRowFullSized = false;
    vBars.RemoveAll();
bool bEmptyRowCalc = true;
	if( m_pDockBar == NULL )
		return;
	ASSERT_VALID( m_pDockBar );
INT nBarWidth = 0;
bool bHorz = IsDockedHorizontally();
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
bool bThisFound = false;
	for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			if( bThisFound )
				break;
			vBars.RemoveAll();
			bEmptyRowCalc = true;
			nBarWidth = 0;
			if( pRcAllBars != NULL )
				pRcAllBars->SetRect( 0, 0, 0, 0 );
			if( p_bRowMaxSized != NULL )
				*p_bRowMaxSized = false;
			if( p_bRowFullSized != NULL )
				*p_bRowFullSized = false;
			continue;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_KINDOF( CControlBar, pBar );
		
		if( pBar == (CControlBar *)this )
		{
			bThisFound = true;
			if( p_nOwnPos != NULL )
				*p_nOwnPos = vBars.GetSize();
		}

		if( !pBar->IsVisible() )
			continue;
		
		if( pRcAllBars != NULL )
		{
			if( bEmptyRowCalc )
			{
				bEmptyRowCalc = false;
				pBar->GetWindowRect( pRcAllBars );
			} // if( bEmptyRowCalc )
			else
			{
				CRect rcWndBar;
				pBar->GetWindowRect( &rcWndBar );
				pRcAllBars->left =
					min( pRcAllBars->left, rcWndBar.left );
				pRcAllBars->top =
					min( pRcAllBars->top, rcWndBar.top );
				pRcAllBars->right =
					max( pRcAllBars->right, rcWndBar.right );
				pRcAllBars->bottom =
					max( pRcAllBars->bottom, rcWndBar.bottom );
			} // else from if( bEmptyRowCalc )
		} // if( pRcAllBars != NULL )

		if( (p_bRowMaxSized != NULL || p_bRowFullSized != NULL)
			&& pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
			)
		{
			if( p_bRowMaxSized != NULL
				&& ( ! (*p_bRowMaxSized) )
				&& !((CExtControlBar*)pBar)->IsFixedMode()
				)
				*p_bRowMaxSized = true;
			if( p_bRowFullSized != NULL
				&& ( ! (*p_bRowFullSized) )
				&& pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
				&& ((CExtControlBar *)pBar)->_GetFullRowMode()
				)
				*p_bRowFullSized = true;
			if( p_bRowFullSized != NULL
				&& ( ! (*p_bRowFullSized) )
				)
			{
				CRect rcBarWnd;
				pBar->GetWindowRect( &rcBarWnd );
				nBarWidth +=
					bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
			}
		}

		vBars.Add( pBar );
	} // for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )

	if( p_bRowFullSized != NULL
		&& ( ! (*p_bRowFullSized) )
		)
	{
		ASSERT_VALID( m_pDockBar );
		CRect rcDockBarWnd;
		m_pDockBar->GetWindowRect( &rcDockBarWnd );
		INT nDockBarWidth = bHorz ? rcDockBarWnd.Width() : rcDockBarWnd.Height();
		nDockBarWidth --;
		if( nBarWidth >= nDockBarWidth )
			*p_bRowFullSized = true;
	}

#ifdef _DEBUG
	if( p_nOwnPos != NULL )
	{
		INT nCount = vBars.GetSize();
		ASSERT( nCount > 0 );
		ASSERT( 0 <= (*p_nOwnPos) && (*p_nOwnPos) < nCount );
		for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
		{
			CControlBar * pBar = (CControlBar *)
				m_pDockBar->m_arrBars[ nBar ];
			pBar;
		}
	}
#endif // _DEBUG
}

void CExtControlBar::_GetRowExtBars(
	ExtControlBarVector_t & vBars,
	bool bNonFixedOnly, // = true
	CRect * pRcAllBars, // = NULL
	INT * p_nOwnPos, // = NULL
	bool * p_bRowMaxSized, // = NULL // row with resizable bars?
	bool * p_bRowFullSized // = NULL // row with menu-like bar?
	) const
{
	ASSERT_VALID( this );
	if( pRcAllBars != NULL )
		pRcAllBars->SetRect( 0, 0, 0, 0 );
	if( p_nOwnPos != NULL )
		*p_nOwnPos = -1;
	if( p_bRowMaxSized != NULL )
		*p_bRowMaxSized = false;
	if( p_bRowFullSized != NULL )
		*p_bRowFullSized = false;
    vBars.RemoveAll();
bool bEmptyRowCalc = true;
	if( m_pDockBar == NULL )
		return;
	ASSERT_VALID( m_pDockBar );
INT nBarWidth = 0;
bool bHorz = IsDockedHorizontally();
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
bool bThisFound = false;
	for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			if( bThisFound )
				break;
			vBars.RemoveAll();
			bEmptyRowCalc = true;
			nBarWidth = 0;
			if( pRcAllBars != NULL )
				pRcAllBars->SetRect( 0, 0, 0, 0 );
			if( p_bRowMaxSized != NULL )
				*p_bRowMaxSized = false;
			if( p_bRowFullSized != NULL )
				*p_bRowFullSized = false;
			continue;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_KINDOF( CControlBar, pBar );

		if( pBar == (CControlBar *)this )
		{
			bThisFound = true;
			if( p_nOwnPos != NULL )
				*p_nOwnPos = vBars.GetSize();
		}

		if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
		{
			if( ( !((CExtControlBar *)pBar)->IsFixedMode() )
				|| (! bNonFixedOnly )
				)
				vBars.Add( (CExtControlBar *)pBar );
		
			if( p_bRowMaxSized != NULL || p_bRowFullSized != NULL )
			{
				if( p_bRowMaxSized != NULL
					&& ( ! (*p_bRowMaxSized) )
					&& !((CExtControlBar*)pBar)->IsFixedMode()
					)
					*p_bRowMaxSized = true;
				if( p_bRowFullSized != NULL
					&& ( ! (*p_bRowFullSized) )
					&& pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
					&& ((CExtControlBar *)pBar)->_GetFullRowMode()
					)
					*p_bRowFullSized = true;
			}
		}
		if( p_bRowFullSized != NULL
			&& ( ! (*p_bRowFullSized) )
			)
		{
			CRect rcBarWnd;
			pBar->GetWindowRect( &rcBarWnd );
			nBarWidth +=
				bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
		}

		if( pRcAllBars != NULL )
		{
			if( bEmptyRowCalc )
			{
				bEmptyRowCalc = false;
				pBar->GetWindowRect( pRcAllBars );
			} // if( bEmptyRowCalc )
			else
			{
				CRect rcWndBar;
				pBar->GetWindowRect( &rcWndBar );
				pRcAllBars->left =
					min( pRcAllBars->left, rcWndBar.left );
				pRcAllBars->top =
					min( pRcAllBars->top, rcWndBar.top );
				pRcAllBars->right =
					max( pRcAllBars->right, rcWndBar.right );
				pRcAllBars->bottom =
					max( pRcAllBars->bottom, rcWndBar.bottom );
			} // if( bEmptyRowCalc )
		} // if( pRcAllBars != NULL )
	} // for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )

	if( p_bRowFullSized != NULL
		&& ( ! (*p_bRowFullSized) )
		)
	{
		ASSERT_VALID( m_pDockBar );
		CRect rcDockBarWnd;
		m_pDockBar->GetWindowRect( &rcDockBarWnd );
		INT nDockBarWidth = bHorz ? rcDockBarWnd.Width() : rcDockBarWnd.Height();
		nDockBarWidth --;
		if( nBarWidth >= nDockBarWidth )
			*p_bRowFullSized = true;
	}

#ifdef _DEBUG
	if( p_nOwnPos != NULL )
	{
		INT nCount = vBars.GetSize();
		ASSERT( nCount > 0 );
		ASSERT( 0 <= (*p_nOwnPos) && (*p_nOwnPos) < nCount );
	}
#endif // _DEBUG
}

bool CExtControlBar::_IsFirstBarInRow( bool bExtBar ) const
{
	if( m_pDockBar == NULL )
		return false;
	ASSERT_VALID( m_pDockBar );
INT nRowCount = 0;
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
	for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			nRowCount = 0;
			continue;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_KINDOF( CControlBar, pBar );
		if( bExtBar )
		{
			CExtControlBar * pExtBar = 
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar == NULL )
				return false;
			if( pExtBar->IsFixedMode() )
				return false;
		}
		if( pBar == (CControlBar *)this )
		{
			return (nRowCount == 0) ? true : false;
		}
		nRowCount++;
	} // for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	ASSERT( FALSE );
	return false;
}

bool CExtControlBar::_IsLastBarInRow( bool bExtBar ) const
{
	ASSERT( !IsFloating() );
	if( bExtBar )
	{
		ExtControlBarVector_t vBars;
		_GetRowExtBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		ASSERT( nCountOfBars > 0 );
		if( vBars[nCountOfBars-1] == (CExtControlBar *)this )
			return true;
		return false;
	}
MfcControlBarVector_t vBars;
    _GetRowBars( vBars );
INT nCountOfBars = vBars.GetSize();
	ASSERT( nCountOfBars > 0 );
	if( vBars[nCountOfBars-1] == (CControlBar *)this )
		return true;
	return false;
}

CFrameWnd * CExtControlBar::_GetDockingFrameImpl(
	CControlBar * pBar
	)
{
	ASSERT_VALID( pBar );
	ASSERT( pBar->GetSafeHwnd() != NULL );

	if( pBar->m_pDockSite != NULL )
	{
		ASSERT_VALID( pBar->m_pDockSite );
		ASSERT( pBar->m_pDockSite->GetSafeHwnd() != NULL );
		ASSERT_KINDOF( CFrameWnd, pBar->m_pDockSite );
		return pBar->m_pDockSite;
	}

CFrameWnd * pFrame = pBar->GetParentFrame();
	if( pFrame == NULL )
		return NULL;

	ASSERT_VALID( pFrame );
	ASSERT_KINDOF( CFrameWnd, pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		ASSERT_VALID( pFrame );
		ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
	}
	return pFrame;
}

CFrameWnd * CExtControlBar::_GetDockingFrameImpl() const
{
	return _GetDockingFrameImpl( (CControlBar *)this );
}

UINT CExtControlBar::GetSafeDockBarDlgCtrlID() const
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( GetSafeHwnd() ) );

CWnd * pParentWnd =
		(m_pDockBar != NULL) ? m_pDockBar : GetParent();
	if( pParentWnd->GetSafeHwnd() == NULL
		|| !::IsWindow( pParentWnd->GetSafeHwnd() )
		|| !pParentWnd->IsKindOf( RUNTIME_CLASS(CDockBar) )
		)
		return AFX_IDW_DOCKBAR_FLOAT; // IDC_STATIC

UINT nDlgCtrlID = pParentWnd->GetDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID( nDlgCtrlID );
	return nDlgCtrlID;
}

bool CExtControlBar::IsDockedAtTop() const
{
	if( m_pDockSite == NULL )
		return (m_dwStyle & CBRS_TOP) ? true : false;
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_TOP;
}

bool CExtControlBar::IsDockedAtBottom() const
{
	if( m_pDockSite == NULL )
		return (m_dwStyle & CBRS_BOTTOM) ? true : false;
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_BOTTOM;
}

bool CExtControlBar::IsDockedAtLeft() const
{
	if( m_pDockSite == NULL )
		return (m_dwStyle & CBRS_LEFT) ? true : false;
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_LEFT;
}

bool CExtControlBar::IsDockedAtRight() const
{
	if( m_pDockSite == NULL )
		return (m_dwStyle & CBRS_RIGHT) ? true : false;
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_RIGHT;
}

bool CExtControlBar::IsDockedHorizontally() const
{
	if( m_pDockSite == NULL )
		return (m_dwStyle & (CBRS_TOP|CBRS_BOTTOM)) ? true : false;
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM;
}

bool CExtControlBar::IsDockedVertically() const
{
	if( m_pDockSite == NULL )
		return (m_dwStyle & (CBRS_LEFT|CBRS_RIGHT)) ? true : false;
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT;
}

bool CExtControlBar::IsDocked() const
{
	if( m_pDockSite == NULL )
		return true;
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT;
}

bool CExtControlBar::IsFloating() const
{
	if( !IsDocked() )
	{
		if( GetParentFrame() != NULL )
			return true;
	}
	return false;
}

bool CExtControlBar::_IsSingleVisibleInFloatingPalette()
{
	if(		m_pDockSite == NULL
		||	!IsVisible()
		||	IsFixedMode()
//		||	m_bUpdatingChain
		)
		return false;
CFrameWnd * pFrame = GetParentFrame();
	if( pFrame == NULL )
		return false;
	if( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		return false;
	ASSERT_KINDOF( CExtMiniDockFrameWnd, pFrame );
	ASSERT( m_pDockSite != pFrame );
POSITION pos = m_pDockSite->m_listControlBars.GetHeadPosition();
	for( ; pos != NULL; )
	{
		CExtControlBar * pBar =
			DYNAMIC_DOWNCAST(
				CExtControlBar,
				((CControlBar *)(m_pDockSite->m_listControlBars.GetNext(pos)))
				);
		if(		pBar == NULL
			||	pBar == this
			||	pBar->m_pDockSite == NULL
			||	pBar->m_pDockBar == NULL
			||	pBar->IsFixedMode()
			||	(	pBar->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
					&& !pBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar))
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
				)
			||	pBar->GetParentFrame() != pFrame
			)
			continue;
		HWND hWndDockBar = pBar->m_pDockBar->GetSafeHwnd();
		if(		hWndDockBar == NULL // by Guillaume Provost advice
			||	(! ::IsWindow(hWndDockBar) )
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
		if( pBar->IsVisible() )
			return false;
	}
	return true;
}

bool CExtControlBar::IsBarWithGripper(
	bool * pbGripperAtTop, // = NULL
	bool * pbTextOnGripper // = NULL
	) const
{
	ASSERT_VALID( this );
	if( pbGripperAtTop != NULL )
		*pbGripperAtTop = false;
	if( pbTextOnGripper != NULL )
		*pbTextOnGripper = false;
	if( ! ::IsWindow(GetSafeHwnd()) )
		return false;
	if( (m_dwStyle & CBRS_GRIPPER) == 0
		|| IsFloating()
		)
		return false;

CFrameWnd * pParentFrame = GetParentFrame();
	if(		pParentFrame != NULL
		&&	pParentFrame->IsKindOf(RUNTIME_CLASS(CExtMiniDockFrameWnd))
		)
	{
		if( ((CExtControlBar *)this)->_IsSingleVisibleInFloatingPalette() )
			return false;
	}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( m_pDockBar != NULL
		&& !AutoHideModeGet()
		)
	{
		if( m_pDockBar->IsKindOf( RUNTIME_CLASS(CExtDockDynTabBar) ) )
		{
			if( pbGripperAtTop != NULL )
				*pbGripperAtTop = false;
			if( pbTextOnGripper != NULL )
				*pbTextOnGripper = false;
			return false;
		}
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	if( pbGripperAtTop != NULL
		&&	( m_bGripperStaticallyAtTop || IsDockedVertically() )
		)
		*pbGripperAtTop = true;
	if( pbTextOnGripper != NULL )
		*pbTextOnGripper = true;
    return true;
}

CSize CExtControlBar::CalcFixedLayout(
	BOOL bStretch,
	BOOL bHorz
	)
{
	bStretch;
	return _CalcDesiredLayout( bHorz );
}

CSize CExtControlBar::CalcDynamicLayout(
	int nLength,
	DWORD nMode
	)
{
	// MSDN for LM_HORZDOCK and LM_VERTDOCK: ... return
	// horizontal/vertical docked dimensions ...

	if( ((nMode & (LM_HORZDOCK | LM_VERTDOCK)) == 0)
		|| IsFloating()
		)
	{ // if going to floated state or already floated
		
		if( nLength > 0 )
		{
			// MSDN for nLength: ... The requested dimension of the
			// control bar, either horizontal or vertical,
			// depending on dwMode ...

			// MSDN for LM_LENGTHY: ... Set if nLength indicates
			// height (Y-direction) instead of width.
			if( nMode & LM_LENGTHY ) 
				m_sizeFloated.cy = nLength;
			else
				m_sizeFloated.cx = nLength;
		} // if( nLength > 0 )

		// MSDN for LM_MRUWIDTH: ... Most Recently Used Dynamic Width.
		// Ignores nLength parameter and uses the remembered most
		// recently used width.

		// MSDN for LM_COMMIT: ... Resets LM_MRUWIDTH to current
		// width of floating control bar.
		if( nMode & ( LM_MRUWIDTH | LM_COMMIT ) )
			return m_sizeFloated;

		return _CalcDesiredLayout( nMode & LM_HORZ );

	} // if going to floated state or already floated

	if( nLength >= 0 )
		return _CalcDesiredLayout( nMode & LM_HORZ );

	ASSERT_VALID( m_pDockBar );
	if( ((CExtDockBar *)m_pDockBar)->m_bLayoutQuery )
		return _CalcDesiredLayout( nMode & LM_HORZ );

    ASSERT_VALID( m_pDockBar );
INT nCalcExtentMax = 
		((FriendlyDockBarHack*)m_pDockBar)->
			GetCalcExtentMax( nMode & LM_HORZ );
	ASSERT( nCalcExtentMax >= 0 );
	if( nCalcExtentMax == 0 )
		return _CalcDesiredLayout( nMode & LM_HORZ );
INT nCalcExtentDiff = 0;
INT nCalcExtentMin =
		(nMode & LM_HORZ) ? afxData.cyBorder2 : afxData.cxBorder2;
INT nCalcExtentReal = 0, nMaxWidth = 0;
INT nCountOfBarsAtSide = m_pDockBar->m_arrBars.GetSize();
	ASSERT( nCountOfBarsAtSide > 0 );
INT nThisIdx = -1, nReviewStart = -1, nReviewStop = -1;
INT nCountInRowReal = 0, nCountExtBarsInRowReal = 0;
INT nCountOfMinSizedBars = 0;
	for( INT nBarAtSide = 0; nBarAtSide < nCountOfBarsAtSide ; nBarAtSide++ )
	{
		CControlBar * pBar =  (CControlBar *)
			m_pDockBar->m_arrBars[ nBarAtSide ];
		if( pBar == NULL )
		{
			if( nThisIdx >= 0 )
			{
				nReviewStop = nBarAtSide - 1;
				break;
			}
			nCountInRowReal = 0;
			nCountExtBarsInRowReal = 0;
			nMaxWidth = 0;
			nReviewStop = -1;
			nReviewStart = nBarAtSide + 1;
			continue;
		} // if( pBar == NULL )
		if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow( pBar->GetSafeHwnd() )
			);
		if( pBar == this )
		{
			if( nCountExtBarsInRowReal > 0 )
				//goto __Align_Mutually_Row__;
				return _CalcDesiredLayout( nMode & LM_HORZ );
			nThisIdx = nBarAtSide;
			//nReviewStart = nThisIdx;
		}
		CRect rcBarWnd;
		pBar->GetWindowRect( &rcBarWnd );
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar!=NULL && pExtBar->IsFixedMode() )
			pExtBar = NULL;
//		INT nCalcExtentStep =
//			(nMode & LM_HORZ) ? rcBarWnd.Width() : rcBarWnd.Height();
		if( pExtBar != NULL )
		{
			nCountExtBarsInRowReal++;
			nMaxWidth =
				max(
					nMaxWidth,
					(nMode & LM_HORZ)
						? pExtBar->m_sizeDockedH.cy
						: pExtBar->m_sizeDockedV.cx
					);
		} // if( pExtBar != NULL )
		nCountInRowReal++;
	} // for( int nBarAtSide = 0; nBarAtSide < nCountOfBarsAtSide ; nBarAtSide++ )

	ASSERT( nReviewStart <= nReviewStop );
	ASSERT( nReviewStart <= nThisIdx && nThisIdx <= nReviewStop );

	if( nCountInRowReal == 1 )
	{
        if( nMode & LM_HORZ )
            m_sizeDockedH.cx =
				max( _CalcDesiredMinHW(), nCalcExtentMax );
        else
            m_sizeDockedV.cy =
				max( _CalcDesiredMinVH(), nCalcExtentMax );
		goto __Align_Mutually_Row__;
	}

    for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
    {
        CControlBar * pBar = (CControlBar*)
			m_pDockBar->m_arrBars[ nBarAtSide ];
        if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar!=NULL && pExtBar->IsFixedMode() )
			pExtBar = NULL;

        INT nCalcExtentBar = 0;
        if( pExtBar == NULL )
        {
            CRect rcBar;
            pBar->GetWindowRect( &rcBar );
            nCalcExtentBar =
				(nMode & LM_HORZ) ?
					rcBar.Width() : rcBar.Height();
        } // if( pExtBar == NULL )
        else
		{
			nCalcExtentBar =
				(nMode & LM_HORZ)
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH()
					;
			LONG & ref_nCalcExtentBar =
				(nMode & LM_HORZ)
					? pExtBar->m_sizeDockedH.cx
					: pExtBar->m_sizeDockedV.cy
					;
			if( ref_nCalcExtentBar < nCalcExtentBar )
				ref_nCalcExtentBar = nCalcExtentBar;
		}
        nCalcExtentMin += nCalcExtentBar;
		if( nCalcExtentMin <= nCalcExtentMax )
		{
			if( pExtBar == NULL )
			{
				nCalcExtentMax -= nCalcExtentBar;
				continue;
			}
			nCalcExtentReal +=
				(nMode & LM_HORZ)
					? pExtBar->m_sizeDockedH.cx
					: pExtBar->m_sizeDockedV.cy ;
			continue;
		} // if( nCalcExtentMin <= nCalcExtentMax )
        
        if( nBarAtSide < nThisIdx )
        {
			ASSERT( pBar != this );
			((FriendlyDockBarHack*)m_pDockBar)->
				_InsertRowSplitter( nBarAtSide + 1 );
			return _CalcDesiredLayout( nMode & LM_HORZ );
        }
        if( nBarAtSide == nThisIdx )
        {
			ASSERT( pBar == this );
			if( nMode & LM_HORZ )
				m_sizeDockedH.cx =
					max( _CalcDesiredMinHW(), nCalcExtentMax );
			else
				m_sizeDockedV.cy =
					max( _CalcDesiredMinVH(), nCalcExtentMax );
			break;
        }
		ASSERT( pBar != this );
		((FriendlyDockBarHack*)m_pDockBar)->
			_InsertRowSplitter( nBarAtSide );
        nReviewStop = nBarAtSide - 1;
		ASSERT( nReviewStart <= nReviewStop );
		ASSERT( nReviewStart <= nThisIdx && nThisIdx <= nReviewStop );
        break;
    } // for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )

	nCountOfMinSizedBars = 0;
	for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
	{
        CControlBar * pBar = (CControlBar*)
			m_pDockBar->m_arrBars[ nBarAtSide ];
        if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar!=NULL && pExtBar->IsFixedMode() )
			pExtBar = NULL;
		if( pExtBar == NULL )
			continue;
        LONG nMinMetric =
			(nMode & LM_HORZ)
				? pExtBar->_CalcDesiredMinHW()
				: pExtBar->_CalcDesiredMinVH()
				;
        LONG nCalcExtentBar =
			(nMode & LM_HORZ)
				? pExtBar->m_sizeDockedH.cx
				: pExtBar->m_sizeDockedV.cy
				;
		ASSERT( nCalcExtentBar >= nMinMetric );
		//
		if( nMinMetric == nCalcExtentBar )
			nCountOfMinSizedBars++;
	} // for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
	ASSERT( nCountOfMinSizedBars <= nCountExtBarsInRowReal );

	nCalcExtentDiff = nCalcExtentMax - nCalcExtentReal;
	if( nCountOfMinSizedBars != nCountExtBarsInRowReal )
	{
//		INT nCalcStep =
//			nCalcExtentDiff /
//				(nCountExtBarsInRowReal - nCountOfMinSizedBars);
		INT nCalcStep = (nCalcExtentDiff > 0) ? 1 : -1;
		INT nPass = 0;
		INT nPassMax = abs(nCalcExtentDiff)*2;
		for(	nBarAtSide = nReviewStart;
				// nBarAtSide < nReviewStop &&
				nCalcExtentDiff != 0
				&& nCountOfMinSizedBars != nCountExtBarsInRowReal		
				&& nPass < nPassMax
				;
				)
		{
			ASSERT( nBarAtSide <= nReviewStop+1 );
			if( nBarAtSide == nReviewStop+1 )
			{
				nPass++;
				nBarAtSide = nReviewStart;
			}
			
			CControlBar * pBar = (CControlBar*)
				m_pDockBar->m_arrBars[ nBarAtSide ];
			if( pBar == NULL )
			{
				nBarAtSide++;
				continue;
			}
			if( __PLACEHODLER_BAR_PTR(pBar) )
			{
				nBarAtSide++;
				continue;
			}
			ASSERT_VALID( pBar );
			ASSERT(
				pBar->GetSafeHwnd() != NULL
				&& ::IsWindow(pBar->GetSafeHwnd())
				);
			if( !pBar->IsVisible() )
			{
				nBarAtSide++;
				continue;
			}
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar == NULL
				|| pExtBar->IsFixedMode()
				)
			{
				nBarAtSide++;
				continue;
			}

			if( nMode & LM_HORZ )
				pExtBar->m_sizeDockedH.cy = nMaxWidth;
			else
				pExtBar->m_sizeDockedV.cx = nMaxWidth;
			
			ASSERT( nCalcExtentDiff != 0 );

			LONG nMinMetric =
				(nMode & LM_HORZ)
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH()
					;
			LONG & ref_nCalcExtentBar =
				(nMode & LM_HORZ)
					? pExtBar->m_sizeDockedH.cx
					: pExtBar->m_sizeDockedV.cy
					;
			ASSERT( ref_nCalcExtentBar >= nMinMetric );
			if( ref_nCalcExtentBar == nMinMetric )
			{
				nBarAtSide++;
				continue;
			}

			ASSERT( ref_nCalcExtentBar > nMinMetric );
			ref_nCalcExtentBar += nCalcStep;
			nCalcExtentDiff -= nCalcStep;
			if( ref_nCalcExtentBar == nMinMetric )
				nCountOfMinSizedBars++;
			
			ASSERT( ref_nCalcExtentBar >= nMinMetric );
			nBarAtSide++;
		} // for( nBarAtSide = nReviewStart; ...

		ASSERT( nCalcExtentDiff == 0 || nPass == nPassMax );

	} // if( nCountOfMinSizedBars != nCountExtBarsInRowReal )
	else
	{
		CExtControlBar * pLastBar = (CExtControlBar *)
			_GetFirstControlBarInRow(
				this, true, true, true
				);
		ASSERT( pLastBar != NULL );
		while( !pLastBar->IsVisible() )
		{
			CExtControlBar * pLastBar2 = (CExtControlBar *)
				_GetNextControlBarInRow(
					pLastBar, false, true, true
					);
			if( pLastBar2 == NULL )
				break;
			pLastBar = pLastBar2;
		}
		ASSERT( pLastBar != NULL );
		LONG & ref_nCalcExtentBar =
			(nMode & LM_HORZ) ?
				pLastBar->m_sizeDockedH.cx : pLastBar->m_sizeDockedV.cy;
		ref_nCalcExtentBar += nCalcExtentDiff;
		nCalcExtentReal = nCalcExtentMax;
	} // else from if( nCountOfMinSizedBars != nCountExtBarsInRowReal )

__Align_Mutually_Row__:

	ASSERT_VALID( m_pDockBar );
//	if( ((CExtDockBar *)m_pDockBar)->m_bLayoutQuery )
//		return _CalcDesiredLayout( nMode & LM_HORZ );

//bool bAtTop = IsDockedAtTop();
CRect rcWndDockBar;
	m_pDockBar->GetWindowRect( &rcWndDockBar );
CPoint ptDevOffset = -rcWndDockBar.TopLeft();
INT nCalcBarOffset = 0;	

INT nHdwpCount = nReviewStop - nReviewStart + 1;
	ASSERT( nHdwpCount > 0 );
HANDLE hDWP = ::BeginDeferWindowPos( nHdwpCount );
	ASSERT( hDWP != NULL );
static const UINT nDwpFlags =
		SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE
		//|SWP_NOSENDCHANGING|SWP_NOREDRAW
		;

	for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
	{
		CControlBar * pBar = (CControlBar*)
			m_pDockBar->m_arrBars[ nBarAtSide ];
		if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;

		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar != NULL && pExtBar->IsFixedMode() )
			pExtBar = NULL;
		CRect rcWndBarOrg;
		pBar->GetWindowRect( &rcWndBarOrg );
		CRect rcWndBar( rcWndBarOrg );
		rcWndBar.OffsetRect( ptDevOffset );
		
		if( pExtBar != NULL )
            rcWndBar =
				CRect(
					rcWndBar.TopLeft(),
					(nMode & LM_HORZ)
						? pExtBar->m_sizeDockedH
						: pExtBar->m_sizeDockedV
					);
		INT nBarRealOffset =
			(nMode & LM_HORZ) ? rcWndBar.left : rcWndBar.top;
        if( nBarRealOffset != nCalcBarOffset )
        {
            if( !(nMode & LM_HORZ) )
                rcWndBar.OffsetRect(
					0,
					nCalcBarOffset - rcWndBar.top
					);
//            else if( bAtTop )
//                rcWndBar.OffsetRect(
//					nCalcBarOffset - rcWndBar.left,
//					-2
//					);
            else
            	rcWndBar.OffsetRect(
					nCalcBarOffset - rcWndBar.left,
					0
					);
			
		} // if( nBarRealOffset != nCalcBarOffset )
		
		if( rcWndBar != rcWndBarOrg )
		{
			hDWP = ::DeferWindowPos(
				hDWP,
				pBar->GetSafeHwnd(),
				NULL,
				rcWndBar.left, rcWndBar.top,
				rcWndBar.Width(), rcWndBar.Height(),
				nDwpFlags
				);
			ASSERT( hDWP != NULL );
		} // if( rcWndBar != rcWndBarOrg )
			

        nCalcBarOffset += (nMode & LM_HORZ)
			? rcWndBar.Width() : rcWndBar.Height();
	} // for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )

	VERIFY( ::EndDeferWindowPos( hDWP ) );

	return _CalcDesiredLayout( nMode & LM_HORZ );
}

bool CExtControlBar::_GetFullRowMode() const
{
	ASSERT_VALID( this );
	return false;
}

INT CExtControlBar::_CalcDesiredMinHW() const
{
INT nCalcVal = m_nMinHW + m_rcRowRecalcUp.Width();
	if( m_bGripperStaticallyAtTop )
	{
//		int nResizers =
//			m_rcRowRecalc.Width()
//			+ m_rcRowRecalcUp.Width();
//		int nMinMetric =
//			1
//			+ ::GetSystemMetrics(SM_CYSMCAPTION)+2
//			+ nResizers;
//		if( IsBarWithGripper() && !m_rcGrip.IsRectEmpty() )
//		{
//			int nMinFriendly = m_rcGrip.Height() + nResizers;
//			if( nMinMetric < nMinFriendly )
//				nMinMetric = nMinFriendly;
//		}
//		if( nCalcVal < nMinMetric  )
//			nCalcVal = nMinMetric;
		
		nCalcVal = max( nCalcVal, 10 );
	}
	return nCalcVal;
}

INT CExtControlBar::_CalcDesiredMinVH() const
{
INT nCalcVal = m_nMinVH + m_rcRowRecalcUp.Height();
	return nCalcVal;
}

CSize CExtControlBar::_CalcDesiredMinOuterSize( BOOL bHorz )
{
	bHorz;
	return CSize( _CalcDesiredMinHW(), _CalcDesiredMinVH() );
}

CSize CExtControlBar::_CalcDesiredMinFloatedSize() const
{
	// for nonfixed bars only
	return CSize( 80, 60 );
}

CSize CExtControlBar::_CalcDesiredLayout(
	BOOL bHorz,
	BOOL bForceFullRow // = FALSE
	)
{
	if( IsFloating() )
		return m_sizeFloated;
	
	if( bHorz )
	{
		if( bForceFullRow )
			return CSize( MAXSHORT, m_sizeDockedH.cy );
		if( m_sizeDockedH.cy < _CalcDesiredMinHW() )
			m_sizeDockedH.cy = _CalcDesiredMinHW();
		return m_sizeDockedH;
	} // if( bHorz )
	else
	{
		if( bForceFullRow )
			return CSize( m_sizeDockedH.cx, MAXSHORT );
		if( m_sizeDockedV.cx < _CalcDesiredMinVH() )
			m_sizeDockedV.cx = _CalcDesiredMinVH();
		return m_sizeDockedV;
	} // else from if( bHorz )
}

CExtControlBar::InternalAffixmentData *
	CExtControlBar::_AffixmentGetOuter()
{
	ASSERT_VALID( this );
	return NULL;
}

void CExtControlBar::_AffixmentSetOuter(
	CExtControlBar::InternalAffixmentData * pAffixmentData
	)
{
	ASSERT_VALID( this );
	ASSERT( pAffixmentData != NULL );
	pAffixmentData;
}

CExtControlBar::InternalAffixmentData *
	CExtControlBar::_AffixmentGetOuter(
		CControlBar * pBar
		)
{
	ASSERT_VALID( pBar );
CExtControlBar * pExtBar =
		DYNAMIC_DOWNCAST( CExtControlBar, pBar );
	if( pExtBar == NULL )
		return NULL;
	return pExtBar->_AffixmentGetOuter();
}

int CExtControlBar::_AffixmentCompareOuter(
	CControlBar * pBar1,
	CControlBar * pBar2
	)
{
	ASSERT_VALID( pBar1 );
	ASSERT_VALID( pBar2 );
InternalAffixmentData * pAffixmentData1 =
		_AffixmentGetOuter( pBar1 );
InternalAffixmentData * pAffixmentData2 =
		_AffixmentGetOuter( pBar2 );
	if( pAffixmentData1 != NULL )
	{
		if( pAffixmentData2 != NULL )
			return pAffixmentData1->CompareAffixment( *pAffixmentData2 );
	} // if( pAffixmentData1 != NULL )
	else
	{
		if( pAffixmentData2 != NULL )
			return -1;
	} // else from if( pAffixmentData1 != NULL )
	return 0;
}

void CExtControlBar::_AffixmentRecalcOuter()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockBar );
	ASSERT_KINDOF( CExtDockBar, m_pDockBar );
	ASSERT_VALID( m_pDockSite );
	ASSERT( !IsFloating() );
	ASSERT( GetParentFrame() == m_pDockSite );
	ASSERT( (! GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) ) );
	ASSERT( GetSafeDockBarDlgCtrlID() != AFX_IDW_DOCKBAR_FLOAT );
InternalAffixmentData * pAffixmentData =
		_AffixmentGetOuter();
	if( pAffixmentData == NULL )
		return;
CExtDockOuterBar * pDockBar =
		DYNAMIC_DOWNCAST(CExtDockOuterBar, m_pDockBar );
	if( pDockBar == NULL )
		return;
	pDockBar->_AffixmentBringToTop( this );
}

void CExtControlBar::_AffixmentSafeClearOuter()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );
InternalAffixmentData * pAffixmentData =
		_AffixmentGetOuter();
	if( pAffixmentData == NULL )
		return;
	pAffixmentData->Empty();
}

bool CExtControlBar::_AffixmentIsAlignedHW() const
{
	ASSERT_VALID( this );
	return false;
}

void CExtControlBar::DoPaint( CDC* pDC )
{
#if (defined __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__)

//	CControlBar::DoPaint( pDC );
	ASSERT( pDC->GetSafeHdc() != NULL );
CRect rcBarClient;
	GetClientRect( &rcBarClient );
	pDC->FillSolidRect( &rcBarClient, g_PaintManager->GetColor(COLOR_INFOBK) );

CRect rcDummy( rcBarClient );
	rcDummy.DeflateRect( 4, 4 );
	pDC->DrawFocusRect( &rcDummy );

#else // __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__

	ASSERT( pDC->GetSafeHdc() != NULL );
CRect rcClient;
	GetClientRect( &rcClient );
	if( rcClient.IsRectEmpty()
		|| !pDC->RectVisible( &rcClient )
		)
		return;
	CExtPaintManager::stat_ExcludeChildAreas(
		*pDC,
		*this
		);
CExtMemoryDC dc( pDC, &rcClient );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( dc.GetSafeHdc() != NULL )
		pDC = &dc;
	DoEraseBk( pDC, &rcClient );

	g_PaintManager->OnPaintSessionComplete( this );
#endif // ! __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
}

void CExtControlBar::DoEraseBk( CDC* pDC, LPCRECT pRectBk )
{
	ASSERT( pDC->GetSafeHdc() != NULL );
	ASSERT( pRectBk != NULL );
	g_PaintManager->PaintControlBarClientArea(
		*pDC,
		*pRectBk,
		this
		);
}

BOOL CExtControlBar::DoCustomModePreTranslateMessage(
	CWnd * pWndPretranslateSrc,
	MSG * pMsg
	)
{
	ASSERT( pWndPretranslateSrc != NULL );
	ASSERT( pMsg != NULL );

HWND hWndPretranslateSrc = pWndPretranslateSrc->GetSafeHwnd();
	if(	hWndPretranslateSrc == NULL
		|| ( ! ::IsWindow(hWndPretranslateSrc) )
		)
		return FALSE;

	for(	HWND hWndChild = ::GetWindow( hWndPretranslateSrc, GW_CHILD );
			hWndChild != NULL && ::IsWindow( hWndPretranslateSrc );
			hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT )
		)
	{
		CWnd * pWndChild = CWnd::FromHandlePermanent( hWndChild );
		if( pWndChild == NULL )
			continue;
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pWndChild );
		if( pExtBar == NULL )
			continue;
		if( pExtBar->PreTranslateMessage(pMsg) )
			return TRUE;
	}
	return FALSE;
}

void CExtControlBar::DoCustomModeUpdateControlBars(
	CWnd * pWndParentTarget,
	BOOL bDisableIfNoHndler // = TRUE
	)
{
	ASSERT_VALID( pWndParentTarget );

HWND hWndUpdateTarget = pWndParentTarget->GetSafeHwnd();
	if( hWndUpdateTarget == NULL )
		return;

	ASSERT( ::IsWindow( hWndUpdateTarget ) );
	for(	HWND hWndChild = ::GetWindow( hWndUpdateTarget, GW_CHILD );
			hWndChild != NULL && ::IsWindow( hWndUpdateTarget );
			hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT )
		)
	{
		CWnd * pWndChild = CWnd::FromHandlePermanent( hWndChild );
		if( pWndChild == NULL )
			continue;
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pWndChild );
		if( pExtBar == NULL )
			continue;
		pExtBar->DoCustomModeUpdateCmdUI(
			bDisableIfNoHndler
			);
	}
}

bool CExtControlBar::DoCustomModeUpdateCmdUI(
	BOOL bDisableIfNoHndler // = TRUE
	)
{
	ASSERT_VALID( this );

	if( m_bInCustomModeUpdateCmdUI )
		return true;

HWND hWndOwn = GetSafeHwnd();
	if( hWndOwn == NULL
		|| ( ! ::IsWindow( hWndOwn ) )
		)
		return false;

	m_bInCustomModeUpdateCmdUI = true;

CFrameWnd * pWndUpdateTarget = NULL;
	
	if( !m_bPresubclassDialogMode )
	{
		pWndUpdateTarget = _GetDockingFrameImpl();
		ASSERT_VALID( pWndUpdateTarget );
		ASSERT_KINDOF( CFrameWnd, pWndUpdateTarget );
	}
	
	OnUpdateCmdUI(
		pWndUpdateTarget,
		bDisableIfNoHndler
		);
	
	m_bInCustomModeUpdateCmdUI = false;

	return true;
}

void CExtControlBar::OnUpdateCmdUI(
	CFrameWnd * pTarget,
	BOOL bDisableIfNoHndler
	)
{
	if(	m_bPresubclassDialogMode
		|| m_bDragging || m_bRowRecalcing || m_bRowResizing
		|| (CExtControlBar::_DraggingGetBar() != NULL)
		)
		return;

	pTarget;
	bDisableIfNoHndler;

	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;

CPoint ptScreen;
	if( ! ::GetCursorPos( &ptScreen ) )
		return;
bool bNeedRepaint = false;

UINT nHT = NcButtons_HitTest( ptScreen );
	if( nHT == HTCLIENT )
	{
		if( m_bNcAreaBtnTracking )
			bNeedRepaint = true;
		m_bNcAreaBtnTracking = false;
	} // if( nHT == HTCLIENT )
	else
	{
		if( !m_bNcAreaBtnTracking )
			bNeedRepaint = true;
		m_bNcAreaBtnTracking = true;
	} // else from if( nHT == HTCLIENT )

HWND hTmpWndFocus = ::GetFocus();
bool bWindowActiveOldVal = m_bWindowActive;
	m_bWindowActive =
		( hTmpWndFocus != NULL
		&& (  GetSafeHwnd() == hTmpWndFocus
			  || ::IsChild( GetSafeHwnd(), hTmpWndFocus )
			)
		);
	if( bWindowActiveOldVal != m_bWindowActive )
		bNeedRepaint = true;

	if( bNeedRepaint )
	{
		SendMessage( WM_NCPAINT );
		GetParent()->SendMessage( WM_NCPAINT );
	}
}

void CExtControlBar::_RecalcNcArea()
{
	if( !IsVisible() )
		return;
	SetWindowPos(
		NULL, 0, 0, 0, 0,
		SWP_NOSIZE|SWP_NOMOVE
			|SWP_NOZORDER|SWP_NOOWNERZORDER
			|SWP_NOREPOSITION
			|SWP_FRAMECHANGED
		);
}

void CExtControlBar::_RecalcLayoutImpl()
{
	if( !m_bReflectParentSizing )
		return;
	if( m_bPresubclassDialogMode )
		return;
CWnd * pWndParent = GetParentFrame();
	if( pWndParent == NULL ||
		pWndParent->GetSafeHwnd() == NULL
		)
		return;
WINDOWPLACEMENT wp;
	ZeroMemory( &wp, sizeof(WINDOWPLACEMENT) );
	wp.length = sizeof(WINDOWPLACEMENT);
	pWndParent->GetWindowPlacement( &wp );
	if( wp.showCmd == SW_SHOWMINIMIZED )
		return;
InternalFriendlyFrameWnd * pFrame = (InternalFriendlyFrameWnd *)
		( DYNAMIC_DOWNCAST( CFrameWnd, pWndParent ) );
	if(		pFrame == NULL
		||	pFrame->IsInRecalcLayout()
		)
		return;
	if( IsFixedMode() && IsFloating() )
		pFrame->RecalcLayout();
	else
		pFrame->DelayRecalcLayout();
}

LRESULT CExtControlBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch( message )
	{
	case WM_CLOSE:

		// emulate for disabling update in user non-frame mode
		m_bInCustomModeUpdateCmdUI = true;

		return 0;

	case WM_DESTROY:
		
		// emulate for disabling update in user non-frame mode
		m_bInCustomModeUpdateCmdUI = true;
		
		break;
	case WM_NCDESTROY:

		// emulate for disabling update in user non-frame mode
		m_bInCustomModeUpdateCmdUI = true;

		NcButtons_RemoveAll();
		break;
	case WM_SETFOCUS:
		{
			LRESULT lResult =
				CControlBar::WindowProc(message, wParam, lParam);
			if( IsMinimizedOnRow() )
				MaximizeOnRow();
			if( !IsFixedMode() )
			{
				CWnd* pWnd = GetWindow(GW_CHILD);
				if(pWnd != NULL)
				{
					pWnd->SetFocus();
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
					ASSERT(
						pWnd->GetWindow(GW_HWNDNEXT) == NULL
						|| IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar))
						);
#else
					ASSERT(
						pWnd->GetWindow(GW_HWNDNEXT) == NULL
						);
#endif // else from (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
				}
			} // if( !IsFixedMode() )
			return lResult;
		}
	case WM_ERASEBKGND:
		if( !m_bDoNotEraseClientBackground )
			return !0;
		CExtPaintManager::stat_ExcludeChildAreas(
			(HDC)wParam,
			*this
			);
		return CControlBar::WindowProc(message, wParam, lParam);;
	case WM_CREATE:
		{
			if( (!m_bPresubclassDialogMode) && (!IsFixedMode()) )
			{
				ASSERT( GetSafeHwnd() != NULL );
				ASSERT( ::IsWindow(GetSafeHwnd()) );
				HWND hWndParent = ::GetParent( GetSafeHwnd() );
				CFrameWnd * pFrame = NULL;
				do
				{
					ASSERT( hWndParent != NULL );
					ASSERT( ::IsWindow(hWndParent) );
					CWnd * pWnd = CWnd::FromHandle( hWndParent );
					if( pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
					{
						pFrame = (CFrameWnd *)pWnd;
						break;
					}
				} while( pFrame == NULL );
				ASSERT( pFrame != NULL );
				ASSERT_VALID( pFrame );
				if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
				{
					pFrame = pFrame->GetParentFrame();
					ASSERT( pFrame != NULL );
					ASSERT_VALID( pFrame );
					ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) );
				}
				VERIFY( _FrameEnableDockingImpl( pFrame ) );
			} // if( (!m_bPresubclassDialogMode) && (!IsFixedMode()) )
		}
		break;
	case WM_WINDOWPOSCHANGED:
		{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
			CExtCustomizeSite * pSite =
				CExtCustomizeSite::GetCustomizeSite( m_hWnd );
			if( pSite != NULL )
				pSite->OnBarStateChanged( this );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
			
			if( m_pDockBar == NULL && (!m_bPresubclassDialogMode) )
				break; // on destruction

			LPWINDOWPOS lpWindowPos = 
				reinterpret_cast < LPWINDOWPOS > (lParam);
			ASSERT( lpWindowPos != NULL );

			if( !IsFixedMode() )
				_UpdateVisibilityInChain();
			
			if( (lpWindowPos->flags & SWP_FRAMECHANGED) == 0 )
			{
				_RecalcNcArea();
			}
			else
			{
				//NcButtons_RemoveAll();
				//OnNcAreaButtonsReinitialize();
				OnNcAreaButtonsReposition();
//				CFrameWnd * pFrame = GetParentFrame();
//				if( pFrame != NULL
//					&& pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd))
//					)
//					pFrame->SendMessage( WM_NCPAINT );
			}

			if(		(	_IsShowContentWhenDragging()
					||	_IsShowContentWhenRowRecalcing()
					||	_IsShowContentWhenRowResizing()
					)
				&&	(GetStyle() & WS_VISIBLE) != 0
				&&	g_PaintManager->GetCb2DbTransparentMode(this)
				)
			{
				SendMessage( WM_NCPAINT );
				RedrawWindow(
					NULL, NULL,
					RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW
						|RDW_ALLCHILDREN
						|RDW_NOFRAME
					);
				if(		m_pDockBar != NULL
					&&	m_pDockBar->IsKindOf( RUNTIME_CLASS(CExtDockBar) )
					)
				{
					m_pDockBar->Invalidate();
					if( _DraggingGetBar() == this )
						m_pDockBar->UpdateWindow();
				}
			}

			break;
		}
	case WM_WINDOWPOSCHANGING:
		{
			if( m_pDockBar == NULL && (!m_bPresubclassDialogMode) )
				break; // on destruction

			LPWINDOWPOS lpWindowPos = 
				reinterpret_cast < LPWINDOWPOS > (lParam);
			ASSERT( lpWindowPos != NULL );
			lpWindowPos->flags |= SWP_FRAMECHANGED;
			break;
		}
	} // switch( message )
	
	return CControlBar::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar message handlers

void CExtControlBar::NcButtons_Paint( CDC & dc )
{
	ASSERT( dc.GetSafeHdc() != NULL );
INT nCountOfNcButtons = NcButtons_GetCount();
	for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
	{
		CExtBarNcAreaButton * pBtn =
			NcButtons_GetAt( nBtn );
		if( !pBtn->OnQueryVisibility() )
			continue;
		pBtn->OnNcAreaDraw( dc );
	} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
}

CRect & CExtControlBar::NcButtons_CalcGripperTextLocation(
	CRect & rcPreCalcText
	)
{
INT nCountOfNcButtons = NcButtons_GetCount();
	if( nCountOfNcButtons == 0 )
		return rcPreCalcText; 

bool bHorz = false;
bool bGripperAtTop = true;
	if( !IsFloating() )
	{
		if( !IsBarWithGripper( &bGripperAtTop ) )
			return rcPreCalcText;
		bHorz = IsDockedHorizontally();
	}


	// find last visible at right or at bottom
CExtBarNcAreaButton * pBtn = NULL;
	for( INT nBtnIdx = nCountOfNcButtons; nBtnIdx > 0 ; nBtnIdx-- )
	{
		CExtBarNcAreaButton * pBtnExamine =
				NcButtons_GetAt( nBtnIdx - 1 );
		ASSERT_VALID( pBtnExamine );
		if( pBtnExamine->OnQueryVisibility() )
		{
			pBtn = pBtnExamine;
			break;
		}
	} // for( INT nBtnIdx = nCountOfNcButtons; nBtnIdx > 0 ; nBtnIdx-- )
	if( pBtn == NULL )
		return rcPreCalcText;

	// adjust caption text rect
const CRect & rcBtn = *pBtn;
	if( bHorz && !bGripperAtTop )
	{
		rcPreCalcText.top = rcBtn.bottom + 1;
		if( rcPreCalcText.top > rcPreCalcText.bottom )
			rcPreCalcText.top = rcPreCalcText.bottom;
	}
	else
	{
		rcPreCalcText.right = rcBtn.left - 1;
		if( rcPreCalcText.left > rcPreCalcText.right )
			rcPreCalcText.left = rcPreCalcText.right;
	}

	return rcPreCalcText;
}


UINT CExtControlBar::NcButtons_HitTest(
	CPoint point,
	CExtBarNcAreaButton ** ppBtn // = NULL
	)
{
	if( ppBtn != NULL )
		*ppBtn = NULL;

bool bNeedRepaint = false;

CWnd * pWndNcRepaint = this;
CRect rcBarWnd;
	if( IsFloating() )
	{
		CExtMiniDockFrameWnd * pFrame =
			DYNAMIC_DOWNCAST(
				CExtMiniDockFrameWnd,
				GetParentFrame()
				);
		if( pFrame == NULL )
			return HTCLIENT;
		pFrame->GetWindowRect( &rcBarWnd );
		pWndNcRepaint = pFrame;
	} // if( IsFloating() )
	else
	{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		if( AutoHideModeGet() )
		{
			ASSERT_VALID( m_pDockBar );
			ASSERT_KINDOF( CExtDockBar, m_pDockBar );
			CExtDynAutoHideArea * pWndAutoHideArea =
				((CExtDockBar*)m_pDockBar)->_GetAutoHideArea();
			ASSERT_VALID( pWndAutoHideArea );
			ASSERT_VALID( pWndAutoHideArea->m_pWndSlider );
			if( pWndAutoHideArea->m_pWndSlider->m_pBar != this
				|| ((pWndAutoHideArea->m_pWndSlider->GetStyle() & WS_VISIBLE) == 0)
				)
				return HTCLIENT;
			pWndAutoHideArea->m_pWndSlider->
				GetWindowRect( &rcBarWnd );
		}
		else
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			GetWindowRect( &rcBarWnd );
	} // else from if( IsFloating() )
CPoint ptDevOffset = -rcBarWnd.TopLeft();
CPoint ptWnd( point + ptDevOffset );

bool bDragging = m_bDragging;
	if( !bDragging )
		bDragging = (_DraggingGetBar() != NULL) ? true : false;

UINT nFirstHT = HTCLIENT;
	if(		_DraggingGetBar() == NULL
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		)
	{
		INT nCountOfNcButtons = NcButtons_GetCount();
		for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		{
			CExtBarNcAreaButton * pBtn =
				NcButtons_GetAt( nBtn );
			if( !pBtn->OnQueryVisibility() )
				continue;
			
			if( (pBtn->m_bHover || pBtn->m_bPushed)
				&& ppBtn == NULL
				)
			{
				bNeedRepaint = true;
				if( bDragging || m_bRowRecalcing || m_bRowResizing )
				{
					pBtn->m_bHover = false;
					pBtn->m_bPushed = false;
					continue;
				}
			}
			UINT nHT =
				pBtn->OnNcAreaHitTest( ptWnd );
			if( nHT != HTCLIENT )
			{
				if( ppBtn != NULL )
				{
					*ppBtn = pBtn;
					return nHT;
				}
				// TO FIX: ASSERT( nFirstHT == HTCLIENT );
				nFirstHT = nHT;
				bNeedRepaint = true;
			}
		} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
	}

	if( bNeedRepaint )
		pWndNcRepaint->SendMessage( WM_NCPAINT );
	
	return nFirstHT;
}

UINT CExtControlBar::OnNcHitTest(CPoint point) 
{
UINT nHT = NcButtons_HitTest( point );
	if( nHT != HTCLIENT )
	{
		m_bNcAreaBtnTracking = true;
		return nHT;
	}
	else
	{
		if( m_bNcAreaBtnTracking )
			PostMessage( WM_NCPAINT );
		m_bNcAreaBtnTracking = false;
	}

	if( IsFixedMode() || IsFloating() )
		return HTCLIENT;

CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
CRect rcBarClient;
	GetClientRect( &rcBarClient );
	ClientToScreen( &rcBarClient );
	if( rcBarWnd == rcBarClient )
		return HTCLIENT;
//	rcBarWnd.OffsetRect( ptDevOffset );
//	rcBarClient.OffsetRect( ptDevOffset );

	point += ptDevOffset;

bool bHorz = IsDockedHorizontally();
	if( !m_rcRowResize.IsRectEmpty()
		&& m_rcRowResize.PtInRect( point )
		)
	{
		return bHorz ? HTBOTTOM : HTRIGHT;
	}	
	if( !m_rcRowRecalc.IsRectEmpty()
		&& m_rcRowRecalc.PtInRect( point )
		)
	{
		if( _IsLastBarInRow( true ) )
			return HTCLIENT;
		if( !_RowRecalcing_IsBottomEnabled() )
			return HTCLIENT;
		return bHorz ? HTRIGHT : HTBOTTOM;
	}	
	if( !m_rcRowRecalcUp.IsRectEmpty()
		&& m_rcRowRecalcUp.PtInRect( point )
		)
	{
		if( _IsFirstBarInRow( true ) )
			return HTCLIENT;
		if( !_RowRecalcing_IsTopEnabled() )
			return HTCLIENT;
		return bHorz ? HTLEFT : HTTOP;
	}	
	return HTCLIENT;
}

void CExtControlBar::_GetDeepNcModeMetrics(
	bool & bEnableResizerLeft,
	bool & bEnableResizerTop,
	bool & bEnableResizerRight,
	bool & bEnableResizerBottom
	)
{
	bEnableResizerLeft = true;
	bEnableResizerTop = true;
	bEnableResizerRight = true;
	bEnableResizerBottom = true;

	if( IsFixedMode() )
		return;

	if( !IsVisible() )
		return;

	if( m_pDockSite == NULL || m_pDockBar == NULL )
		return;
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );

	if( !m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) )
		return;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar)) )
	{
		bEnableResizerLeft = false;
		bEnableResizerTop = false;
		bEnableResizerRight = false;
		bEnableResizerBottom = false;
		return;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

CExtDockBar * pDockBar = (CExtDockBar *) m_pDockBar;
UINT nDockBarID = pDockBar->GetDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );

BOOL bHorz = ( nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM );

INT nCount = pDockBar->m_arrBars.GetSize();
	ASSERT( nCount >= 1 );
	if( nCount == 1 )
		return;

INT nOwnIdx = pDockBar->FindBar( this );
	if( nOwnIdx == -1 )
		return; // destruction
	ASSERT( nOwnIdx > 0 && nOwnIdx < nCount );

bool bPrevRowExist = false;
bool bFirstVisibleOnRow = true;
	for( INT nBar = nOwnIdx-1; nBar > 0; nBar-- )
	{
		CControlBar * pBar = (CControlBar *) pDockBar->m_arrBars[nBar];
		if( pBar == NULL )
		{
			if( nBar > 0 )
				bPrevRowExist = true;
			break;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT_KINDOF( CControlBar, pBar );
		if( pBar->IsVisible() )
			bFirstVisibleOnRow = false;
	}
bool bPrevVisibleRowExist = false;
	if( bPrevRowExist )
	{
		for( nBar--; nBar > 0; nBar-- )
		{
			CControlBar * pBar = (CControlBar *) pDockBar->m_arrBars[nBar];
			if( pBar == NULL )
				continue;
			if( __PLACEHODLER_BAR_PTR( pBar ) )
				continue;
			ASSERT_VALID( pBar );
			ASSERT_KINDOF( CControlBar, pBar );
			if( pBar->IsVisible() )
			{
				bPrevVisibleRowExist = true;
				break;
			}
		}
	}

bool bNextRowExist = false;
bool bLastVisibleOnRow = true;
	for( nBar = nOwnIdx+1; nBar < nCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *) pDockBar->m_arrBars[nBar];
		if( pBar == NULL )
		{
			if( nBar < (nCount-1) )
				bNextRowExist = true;
			break;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT_KINDOF( CControlBar, pBar );
		if( pBar->IsVisible() )
			bLastVisibleOnRow = false;
	}
bool bNextVisibleRowExist = false;
	if( bNextRowExist )
	{
		for( nBar++; nBar < nCount; nBar++ )
		{
			CControlBar * pBar = (CControlBar *) pDockBar->m_arrBars[nBar];
			if( pBar == NULL )
				continue;
			if( __PLACEHODLER_BAR_PTR( pBar ) )
				continue;
			ASSERT_VALID( pBar );
			ASSERT_KINDOF( CControlBar, pBar );
			if( pBar->IsVisible() )
			{
				bNextVisibleRowExist = true;
				break;
			}
		}
	}

	if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynBar)) )
	{ // if dockbar is dynamic docker
		if( bHorz )
		{
			if( !bPrevVisibleRowExist )
				bEnableResizerTop = false;
/**/		if( !bNextVisibleRowExist )
/**/			bEnableResizerBottom = false;
			if( bFirstVisibleOnRow )
				bEnableResizerLeft = false;
			if( bLastVisibleOnRow )
				bEnableResizerRight = false;
		} // if( bHorz )
		else
		{
			if( !bPrevVisibleRowExist )
				bEnableResizerLeft = false;
/**/		if( !bNextVisibleRowExist )
/**/			bEnableResizerRight = false;
			if( bFirstVisibleOnRow )
				bEnableResizerTop = false;
			if( bLastVisibleOnRow )
				bEnableResizerBottom = false;
		} // else from if( bHorz )

	} // if dockbar is dynamic docker
	else
	{ // if dockbar is static docker
		
		INT nOwnCircleNo = GetDockBarCircleNo();
		if( nOwnCircleNo > 0 )
		{
			switch( nDockBarID )
			{
			case AFX_IDW_DOCKBAR_TOP:
				if( !bPrevVisibleRowExist )
					bEnableResizerTop = false;
//				if( !bNextVisibleRowExist )
//					bEnableResizerBottom = false;
				if( bFirstVisibleOnRow )
					bEnableResizerLeft = false;
				if( bLastVisibleOnRow )
					bEnableResizerRight = false;
				break;
			case AFX_IDW_DOCKBAR_BOTTOM:
//				if( !bPrevVisibleRowExist )
//					bEnableResizerTop = false;
				if( !bNextVisibleRowExist )
					bEnableResizerBottom = false;
				if( bFirstVisibleOnRow )
					bEnableResizerLeft = false;
				if( bLastVisibleOnRow )
					bEnableResizerRight = false;
				break;
			case AFX_IDW_DOCKBAR_LEFT:
				if( !bPrevVisibleRowExist )
					bEnableResizerLeft = false;
//				if( !bNextVisibleRowExist )
//					bEnableResizerRight = false;
				if( bFirstVisibleOnRow )
					bEnableResizerTop = false;
				if( bLastVisibleOnRow )
					bEnableResizerBottom = false;
				break;
			case AFX_IDW_DOCKBAR_RIGHT:
//				if( !bPrevVisibleRowExist )
//					bEnableResizerLeft = false;
				if( !bNextVisibleRowExist )
					bEnableResizerRight = false;
				if( bFirstVisibleOnRow )
					bEnableResizerTop = false;
				if( bLastVisibleOnRow )
					bEnableResizerBottom = false;
				break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
				break;
#endif // _DEBUG
			} // switch( nDockBarID )
		} // if( nOwnCircleNo > 0 )

	} // if dockbar is static docker

}

void CExtControlBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	bCalcValidRects;

	if( m_pDockBar == NULL && (!m_bPresubclassDialogMode) )
		return; // on destruction

	if( IsFloating() ) // (+ v.2.22)
	{
		if( !IsFixedMode() ) // (+ v.2.23)
		{
			CRect & rcClientLoc =
				reinterpret_cast < CRect & > ( lpncsp->rgrc[0] );
			rcClientLoc.DeflateRect( 1, 1 );
			return;
		}
		CRect & rcClientLoc =
			reinterpret_cast < CRect & > ( lpncsp->rgrc[0] );
		rcClientLoc.OffsetRect( -rcClientLoc.TopLeft() );
		return;
	}

bool bEnableResizerLeft = true;
bool bEnableResizerTop = true;
bool bEnableResizerRight = true;
bool bEnableResizerBottom = true;
	_GetDeepNcModeMetrics(
		bEnableResizerLeft,
		bEnableResizerTop,
		bEnableResizerRight,
		bEnableResizerBottom
		);

CRect & rcClientLoc =
		reinterpret_cast < CRect & > ( lpncsp->rgrc[0] );
CRect rcDevOrg( rcClientLoc );

	m_rcGrip.SetRectEmpty();
	m_rcRowResize.SetRectEmpty();
	m_rcRowRecalc.SetRectEmpty();
	m_rcRowRecalcUp.SetRectEmpty();

	m_nMinHW = m_nSeparatorWidth;
	m_nMinVH = m_nSeparatorHeight;

bool bGripperAtTop = false;
bool bGripperExists = IsBarWithGripper( &bGripperAtTop );

#ifdef _DEBUG
	if( IsKindOf( RUNTIME_CLASS( CExtDynControlBar ) ) )
	{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		if( !IsKindOf( RUNTIME_CLASS( CExtDynTabControlBar ) ) )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		{
			ASSERT( !bGripperExists );
		}
	}
#endif // _DEBUG
	
CPoint ptGripOffset( 0, 0 );
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();

	if( !IsFixedMode() )
	{
		MfcControlBarVector_t vBars;
		_GetRowBars( vBars );
		int nCountOfnBars = vBars.GetSize();
		INT nLastFixedIdx = -1;
		CExtControlBar * pLastExtBar = NULL;
		bool bAddTopRecalc = false;
		for( INT nBar = 0; nBar < nCountOfnBars ; nBar++ )
		{
			CControlBar * pBar = vBars[ nBar ];
			ASSERT_VALID( pBar );
			if( !pBar->IsVisible() )
				continue;
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar != NULL && pExtBar->IsFixedMode() )
				pExtBar = NULL;
			if( pExtBar != NULL )
			{
				if( pExtBar == this
					&& pLastExtBar != NULL
					&& nLastFixedIdx == (nBar-1)
					)
				{
					bAddTopRecalc = true;
					break;
				}
				pLastExtBar = pExtBar;
			}
			else
				nLastFixedIdx = nBar;
		} // for( INT nBar = 0; nBar < nCountOfnBars ; nBar++ )
		switch( nDockBarDlgCtrlID )
		{
		case AFX_IDW_DOCKBAR_TOP:
			if( bEnableResizerRight )
			{
				m_rcRowRecalc = rcClientLoc;
				m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalc.left = m_rcRowRecalc.right - m_nSeparatorWidth;
				rcClientLoc.right -= m_nSeparatorWidth;
			}
			
			if( bEnableResizerBottom )
			{
				rcClientLoc.DeflateRect( 0, 0, 0, m_nSeparatorHeight );
				m_rcRowResize = rcClientLoc;
				m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowResize.top = m_rcRowResize.bottom;
				m_rcRowResize.bottom += m_nSeparatorHeight;
				m_rcRowResize.right = m_rcRowResize.left + rcDevOrg.Width();
			}

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.right = m_rcRowRecalcUp.left + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.x += m_nSeparatorWidth;
				rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		case AFX_IDW_DOCKBAR_BOTTOM:
			if( bEnableResizerRight )
			{
				m_rcRowRecalc = rcClientLoc;
				m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalc.left = m_rcRowRecalc.right - m_nSeparatorWidth;
				rcClientLoc.right -= m_nSeparatorWidth;
			}
			
			if( bEnableResizerTop )
			{
				rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0 );
				m_rcRowResize = rcClientLoc;
				m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowResize.bottom = m_rcRowResize.top + m_nSeparatorHeight;
				m_rcRowResize.right = m_rcRowResize.left + rcDevOrg.Width();
				if( bGripperExists )
					ptGripOffset.y += m_nSeparatorHeight;
			}

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.right = m_rcRowRecalcUp.left + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.x += m_nSeparatorWidth;
				rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		case AFX_IDW_DOCKBAR_LEFT:
			if( bEnableResizerBottom )
			{
				m_rcRowRecalc = rcClientLoc;
				m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalc.top = m_rcRowRecalc.bottom - m_nSeparatorHeight;
rcClientLoc.DeflateRect( 0, 0, 0, m_nSeparatorHeight );
			}
			
			if( bEnableResizerRight )
			{
				//rcClientLoc.DeflateRect( 0, 0, m_nSeparatorWidth, m_nSeparatorHeight );
				rcClientLoc.DeflateRect( 0, 0, m_nSeparatorHeight, 0 );
				m_rcRowResize = rcClientLoc;
				m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowResize.left = m_rcRowResize.right;
				m_rcRowResize.right += m_nSeparatorWidth;
				m_rcRowResize.bottom = m_rcRowResize.top + rcDevOrg.Height();
			}

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.bottom = m_rcRowRecalcUp.top + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.y += m_nSeparatorHeight;
				rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		case AFX_IDW_DOCKBAR_RIGHT:
			if( bEnableResizerBottom )
			{
				m_rcRowRecalc = rcClientLoc;
				m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalc.top = m_rcRowRecalc.bottom - m_nSeparatorHeight;
rcClientLoc.DeflateRect( 0, 0, 0, m_nSeparatorHeight );
			}
			
			if( bEnableResizerLeft )
			{
				//rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, m_nSeparatorHeight );
				rcClientLoc.DeflateRect( m_nSeparatorHeight, 0, 0, 0 );
				m_rcRowResize = rcClientLoc;
				m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowResize.right = m_rcRowResize.left + m_nSeparatorWidth;
				m_rcRowResize.bottom = m_rcRowResize.top + rcDevOrg.Height();
				if( bGripperExists )
					ptGripOffset.x += m_nSeparatorWidth;
			}

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.bottom = m_rcRowRecalcUp.top + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.y += m_nSeparatorHeight;
				rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		} // switch( nDockBarDlgCtrlID )
	} // if( !IsFixedMode() )
	else
	{
		g_PaintManager->FixedBar_AdjustClientRect(
			this,
			rcClientLoc
			);
	} // else from if( !IsFixedMode() )

	if( bGripperExists )
	{
		m_rcGrip = rcClientLoc;
		m_rcGrip.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
		if( bGripperAtTop )
		{
			m_rcGrip.bottom = m_rcGrip.top + m_nGripHeightAtTop;
			m_nMinVH += m_rcGrip.Height();
			if( IsFixedMode() )
				m_rcGrip.DeflateRect( __BAR_NC_GAP_X, 0 );
		} // if( bGripperAtTop )
		else
		{
			m_rcGrip.right = m_rcGrip.left + m_nGripHeightAtTop;
			m_nMinHW += m_rcGrip.Width();
			if( IsFixedMode() )
				m_rcGrip.DeflateRect( 0, __BAR_NC_GAP_Y );
		} // else from if( bGripperAtTop )
		m_rcGrip.OffsetRect( ptGripOffset );

		if( IsFixedMode() )
			m_rcGrip.DeflateRect(
				(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT)   ? __BAR_NC_GAP_X : 0,
				(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP)    ? __BAR_NC_GAP_X : 0,
				(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT)  ? __BAR_NC_GAP_X : 0,
				(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM) ? __BAR_NC_GAP_X : 0
				);
	} // if( bGripperExists )

	OnNcAreaButtonsReinitialize();
	OnNcAreaButtonsReposition();

	if( bGripperExists )
		rcClientLoc.DeflateRect(
			bGripperAtTop
				? 0
				: m_nGripWidthAtLeft  + __BAR_NC_GAP_X
				,
			bGripperAtTop
				? m_nGripHeightAtTop  + __BAR_NC_GAP_Y
				: 0
				,
			0,
			0
			);

//	if( !IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
//		rcClientLoc.DeflateRect( __BAR_NC_GAP_X, __BAR_NC_GAP_Y );

	if( rcClientLoc.bottom < rcClientLoc.top )
		rcClientLoc.bottom = rcClientLoc.top;
	if( rcClientLoc.right < rcClientLoc.left )
		rcClientLoc.right = rcClientLoc.left;
}

void CExtControlBar::OnNcPaint() 
{
	if( m_pDockBar == NULL && (!m_bPresubclassDialogMode) )
		return; // on destruction

#if (defined __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__)

CRect rcBarWnd, rcBarClient;
	GetWindowRect( &rcBarWnd );
	GetClientRect( &rcBarClient );
	ClientToScreen( &rcBarClient );
	if( rcBarWnd == rcBarClient )
		return;
CPoint ptDevOffset = -rcBarWnd.TopLeft();
	rcBarWnd.OffsetRect( ptDevOffset );
	rcBarClient.OffsetRect( ptDevOffset );

CWindowDC dcBarWnd( this );
	ASSERT( dcBarWnd.GetSafeHdc() != NULL );
	dcBarWnd.ExcludeClipRect( &rcBarClient );

CDC & dc = dcBarWnd;
	dc.FillSolidRect( &rcBarWnd, g_PaintManager->GetColor(COLOR_3DFACE) );

bool bGripperAtTop = false;
bool bGripperExists = IsBarWithGripper( &bGripperAtTop );

	if( bGripperExists )
	{
		ASSERT( !m_rcGrip.IsRectEmpty() );
		CString sWndText;
		GetWindowText( sWndText );
		dc.FillSolidRect( &m_rcGrip, g_PaintManager->GetColor(COLOR_HIGHLIGHT) );
		if( !sWndText.IsEmpty() )
		{
			CRect rcGripText( m_rcGrip );
			rcGripText.DeflateRect( 2, 2 );
			HFONT hOldFont = (HFONT)
				::SelectObject(
					dc.GetSafeHdc(),
					::GetStockObject(DEFAULT_GUI_FONT)
					);
			COLORREF clrTextOld =
				dc.SetTextColor( g_PaintManager->GetColor(COLOR_HIGHLIGHTTEXT) );
			int nOldBkMode = dc.SetBkMode( TRANSPARENT );
			dc.DrawText(
				sWndText,
				&rcGripText,
				DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS
				);
			dc.SetBkMode( nOldBkMode );
			dc.SetTextColor( clrTextOld );
			::SelectObject(
				dc.GetSafeHdc(),
				hOldFont
				);
		}
	} // if( bGripperExists )

	if( !m_rcRowResize.IsRectEmpty() )
		dc.FillSolidRect( &m_rcRowResize, RGB(255,0,0) );
	if( !m_rcRowRecalc.IsRectEmpty() )
		dc.FillSolidRect( &m_rcRowRecalc, RGB(0,255,0) );
	if( !m_rcRowRecalcUp.IsRectEmpty() )
		dc.FillSolidRect( &m_rcRowRecalcUp, RGB(0,0,255) );

	if( IsBarWithGripper() && !IsFloating() )
		NcButtons_Paint( dc );

#else // __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__

CWindowDC dc( this );
CRect rcClient, rcBar;
	GetClientRect( rcClient );
	ClientToScreen( rcClient );
	GetWindowRect( rcBar );
	rcClient.OffsetRect( -rcBar.TopLeft() );
	rcBar.OffsetRect( -rcBar.TopLeft() );
	dc.ExcludeClipRect(rcClient);
CExtMemoryDC mdc( &dc, &rcBar );
CExtPaintManager::PAINTDOCKINGFRAMEDATA _pdfd(
		this,
		rcBar,
		rcClient,
		IsFloating() ? true : false,
		!IsFixedMode(),
		!IsFixedMode()
		);
	g_PaintManager->PaintDockingFrame( mdc, _pdfd );
bool bGripperAtTop = false;
	if( IsBarWithGripper(&bGripperAtTop) && !IsFloating() )
	{
		CRect rcGrip( m_rcGrip );
		bool bHorz = IsDockedHorizontally();
		CRect rcText( rcGrip );
		if( bHorz && !bGripperAtTop )
		{
			rcText.DeflateRect( 0, 2 );
			rcText.OffsetRect( -2, 0 );
		}
		else
			rcText.DeflateRect( 2, 0 );
		NcButtons_CalcGripperTextLocation( rcText );
		CString sCaption;
		GetWindowText( sCaption.GetBuffer(1024), 1024 );
		sCaption.ReleaseBuffer();
		CRgn rgnExclude;
		if( rgnExclude.CreateRectRgnIndirect( &rcGrip ) )
			mdc.SelectClipRgn( &rgnExclude );
		CExtPaintManager::PAINTGRIPPERDATA _pgd(
			this,
			rcGrip,
			rcText,
			m_bWindowActive,
			false,
			bHorz && !bGripperAtTop,
			!IsFixedMode(),
			sCaption.IsEmpty() ? LPCTSTR( NULL ) : sCaption
			);
		g_PaintManager->PaintGripper( mdc, _pgd );
		NcButtons_Paint( mdc );
		mdc.SelectClipRgn( NULL );
	} // if( IsBarWithGripper(&bGripperAtTop) && !IsFloating() )
	mdc.__Flush();
	ReleaseDC( &dc );

	g_PaintManager->OnPaintSessionComplete( this );

#endif // ! __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
}

void CExtControlBar::OnSysColorChange() 
{
	CControlBar::OnSysColorChange();
	g_PaintManager.OnSysColorChange( this );
	g_CmdManager.OnSysColorChange( this );
	_RecalcLayoutImpl();
}

void CExtControlBar::OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection) 
{
    CControlBar::OnSettingChange(uFlags, lpszSection);
//	m_nGripHeightAtTop = ::GetSystemMetrics(SM_CYSMCAPTION)+1;
//	m_nGripWidthAtLeft = ::GetSystemMetrics(SM_CYSMCAPTION)+1;
//	m_nSeparatorHeight = min( 4, ::GetSystemMetrics(SM_CYSIZEFRAME) );
//	m_nSeparatorWidth = min( 4, ::GetSystemMetrics(SM_CXSIZEFRAME) );
//	m_nMinHW = ::GetSystemMetrics(SM_CYSMCAPTION)+1 + m_nSeparatorHeight;
//	m_nMinVH = ::GetSystemMetrics(SM_CYSMCAPTION)+1 + m_nSeparatorHeight;
	g_PaintManager.OnSettingChange( this, uFlags, lpszSection );
	g_CmdManager.OnSettingChange( this, uFlags, lpszSection );
	_RecalcLayoutImpl();
}

LRESULT CExtControlBar::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = CControlBar::OnDisplayChange( wParam, lParam );
	g_PaintManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	g_CmdManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	return lResult;
}

LRESULT CExtControlBar::OnThemeChanged( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = Default();
	g_PaintManager.OnThemeChanged( this, wParam, lParam );
	g_CmdManager.OnThemeChanged( this, wParam, lParam );
	return lResult;
}

void CExtControlBar::OnCancelMode() 
{
	if( m_bRowResizing )
		_RowResizingStop( true );
	if( m_bRowRecalcing )
		_RowRecalcingStop( true );
	if( m_bDragging )
		_DraggingStop( true );
	CExtPopupMenuWnd::CancelMenuTracking();
	CWnd::CancelToolTips();
//	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );
	if( m_bNcAreaBtnTracking )
	{
		m_bNcAreaBtnTracking = false;
		PostMessage( WM_NCPAINT );
	}
	CControlBar::OnCancelMode();
}

void CExtControlBar::OnCaptureChanged(CWnd *pWnd) 
{
	if( m_bRowResizing )
		_RowResizingStop( true );
	if( m_bRowRecalcing )
		_RowRecalcingStop( true );
//	if( m_bDragging )
//		_DraggingStop( true );
	CControlBar::OnCaptureChanged(pWnd);
}

bool CExtControlBar::_OnMouseMoveMsg(UINT nFlags, CPoint point)
{
	nFlags;
	_SetCursor( point );
	if( m_bNcAreaBtnTracking )
		return true;
	if( m_bRowResizing )
	{
		_RowResizingUpdateState();
		return true;
	}
	if( m_bRowRecalcing )
	{
		_RowRecalcingUpdateState();
		return true;
	}
	if( m_bDragging )
	{
		CPoint ptScreen( point );
		ClientToScreen( &ptScreen );
		_DraggingUpdateState( ptScreen, false );
		return true;
	}
	if( IsFloating()
		&&
		CExtMiniDockFrameWnd::g_bAutoMakeTopmostOnMouseEvent
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		)
	{
		CFrameWnd * pParentFrame = GetParentFrame();
		if( pParentFrame != NULL )
			pParentFrame->BringWindowToTop();
	}
	return false;
}

void CExtControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( _OnMouseMoveMsg(nFlags,point) )
		return;
	CControlBar::OnMouseMove(nFlags,point);
}

bool CExtControlBar::_RowRecalcing_IsBottomEnabled()
{
	// prev or next extbar should not be minimized
ExtControlBarVector_t vBars;
	_GetRowExtBars( vBars );
INT nCountOfBars = vBars.GetSize();
bool bThisPassed = false;
CExtControlBar * pBarPrev = NULL, * pBarNext = NULL;
	for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT_VALID( pBar );
		if( !pBar->IsVisible() )
			continue;
		if( !bThisPassed )
			pBarPrev = pBar;
		else
		{
			pBarNext = pBar;
			break;
		}
		if( pBar == this )
		{
			bThisPassed = true;
			continue;
		}
	} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
bool bMinimizedPrev = true, bMinimizedNext = true;
	if( pBarPrev != NULL )
		bMinimizedPrev = pBarPrev->IsMinimizedOnRow();
	if( pBarNext != NULL )
		bMinimizedNext = pBarNext->IsMinimizedOnRow();
	if( bMinimizedPrev && bMinimizedNext )
		return false;
	return true;
}

bool CExtControlBar::_RowRecalcing_IsTopEnabled()
{
	// this or next extbar should not be minimized
ExtControlBarVector_t vBars;
	_GetRowExtBars( vBars );
INT nCountOfBars = vBars.GetSize();
CExtControlBar * pBarNext = NULL;
	for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nCountOfBars - nBar - 1 ];
		ASSERT_VALID( pBar );
		if( !pBar->IsVisible() )
			continue;
		if( pBar == this )
			break;
		pBarNext = pBar;
	} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
bool bMinimizedNext = true;
	if( pBarNext != NULL )
		bMinimizedNext = pBarNext->IsMinimizedOnRow();
	if( bMinimizedNext && IsMinimizedOnRow() )
		return false;
	return true;
}

void CExtControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		pSite->DropTargetPopupCancel();
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if( IsFixedMode() || IsFloating() )
		return;

	switch( nHitTest )
	{
	case HTLEFT:
	case HTBOTTOM:
		break; // case HTLEFT & HTBOTTOM
	default:
		if(		( !IsFixedMode() )
			&&	( !IsMinimizedOnRow() )
			)
		{
			CPoint ptScreen( point );
			//ClientToScreen( &ptScreen );
			CExtBarNcAreaButton * pBtn = NULL;
			( const_cast < CExtControlBar * > ( this ) ) ->
				NcButtons_HitTest( ptScreen, &pBtn );
			if( pBtn == NULL ) // (+ v.2.23)
				SetFocus();
		}
		//return;
		break;
	} // switch( nHitTest )

	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );

CRect rcBarWnd, rcBarClient;
	GetWindowRect( &rcBarWnd );
	GetClientRect( &rcBarClient );
	ClientToScreen( &rcBarClient );
	if( rcBarWnd == rcBarClient )
		return;
CPoint ptDevOffset = -rcBarWnd.TopLeft();
//	rcBarWnd.OffsetRect( ptDevOffset );
//	rcBarClient.OffsetRect( ptDevOffset );

	point += ptDevOffset;

bool bHorz = IsDockedHorizontally();
	if( !m_rcRowResize.IsRectEmpty()
		&& m_rcRowResize.PtInRect( point )
		)
	{
		ASSERT( (INT)nHitTest == (bHorz ? HTBOTTOM : HTRIGHT) );
		_RowResizingStart();
		return;
	}	
	if( !m_rcRowRecalc.IsRectEmpty()
		&& m_rcRowRecalc.PtInRect( point )
		)
	{
		ASSERT( !_IsLastBarInRow( true ) );
		ASSERT( (INT)nHitTest == (bHorz ? HTRIGHT : HTBOTTOM) );
		m_bTopRecalcing = false;
		
		if( !_RowRecalcing_IsBottomEnabled() )
			return;
		
		_RowRecalcingStart();
		return;
	}	
	if( !m_rcRowRecalcUp.IsRectEmpty()
		&& m_rcRowRecalcUp.PtInRect( point )
		)
	{
		ASSERT( !_IsFirstBarInRow( true ) );
		ASSERT( (INT)nHitTest == (bHorz ? HTLEFT : HTTOP) );

		if( !_RowRecalcing_IsTopEnabled() )
			return;
		
		m_bTopRecalcing = true;
		_RowRecalcingStart();
		return;
	}
}

void CExtControlBar::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if( m_bNcAreaBtnTracking )
	{
		CRect rcBarWnd;
		GetWindowRect( &rcBarWnd );
		CPoint ptDevOffset = -rcBarWnd.TopLeft();
		CPoint ptWnd( point + ptDevOffset );
		INT nCountOfNcButtons = NcButtons_GetCount();
		for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		{
			CExtBarNcAreaButton * pBtn =
				NcButtons_GetAt( nBtn );
			if( !pBtn->OnQueryVisibility() )
				continue;
			if( pBtn->OnNcAreaClicked( ptWnd ) )
				return;
		} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		return;
	}

	if( m_bRowResizing )
	{
		_RowResizingStop( false );
		return;
	}
	
	if( m_bRowRecalcing )
	{
		_RowRecalcingStop( false );
		return;
	}
	
	if( m_bDragging )
	{
		//_DraggingStop( false );
		return;
	}

	CControlBar::OnNcLButtonUp(nHitTest, point);
}

void CExtControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if( m_pDockBar == NULL || CExtPopupMenuWnd::IsMenuTracking() )
	{
        CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	// start dragging
	ASSERT_VALID( m_pDockBar );
    ASSERT( m_pDockContext != NULL );

    ClientToScreen( &point );
	
bool bExtendedDragging = _IsShowContentWhenDragging();
	if( !bExtendedDragging && IsFixedMode() )
	{
		m_pDockContext->StartDrag( point );
		return;
	}

	if( m_pDockBar != NULL && (!IsFloating()) )
	{
		if( !m_pDockBar->IsKindOf(
				RUNTIME_CLASS(CExtDockBar)
				)
			)
			bExtendedDragging = false;
	}

	if( IsFloating() )
	{
		CMiniDockFrameWnd * pMiniFrame =
			DYNAMIC_DOWNCAST(
				CMiniDockFrameWnd,
				GetDockingFrame()
				);
		ASSERT_VALID( pMiniFrame );

		CExtMiniDockFrameWnd * pExtMiniFrame =
			DYNAMIC_DOWNCAST(
				CExtMiniDockFrameWnd,
				GetDockingFrame()
				);
		if( pExtMiniFrame != NULL )
		{
			pExtMiniFrame->OnNcLButtonDown(
				HTCAPTION,
				point
				);
			return;
		}
	} // if( IsFloating() )

	if( !IsFixedMode() )
		SetFocus();

CPoint ptOffset( point );
CRect rcWnd;
	GetWindowRect( &rcWnd );
	ptOffset -= rcWnd.TopLeft();

	_DraggingStart( point, ptOffset );
}

void CExtControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bRowResizing )
	{
		_RowResizingStop( false );
		return;
	}
	if( m_bRowRecalcing )
	{
		_RowRecalcingStop( false );
		return;
	}
	if( m_bDragging )
	{
		_DraggingStop( false );
		return;
	}
	CControlBar::OnLButtonUp(nFlags, point);
}

void CExtControlBar::OnRButtonUp(UINT nFlags, CPoint point) 
{
	nFlags;

//	if( m_bRowResizing )
//		_RowResizingStop();
//	if( m_bRowRecalcing )
//		_RowRecalcingStop();
//	if( m_bDragging )
//		_DraggingStop( false );
//	CControlBar::OnRButtonUp(nFlags, point);

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		return;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if(		m_bPresubclassDialogMode
		||	_GetDockingFrameImpl() == NULL
		||	CExtControlBar::FindHelpMode( this )
		)
	{
		CControlBar::OnRButtonUp( nFlags, point );
		return;
	}

CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	if( ! ::GetCursorPos(&point) )
		return;
HWND hWndTrack = GetOwner()->GetSafeHwnd();
	ASSERT( hWndTrack != NULL && ::IsWindow(hWndTrack) );
CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu(hWndTrack) );
	if(	! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
			pFrame,
			pPopup,
			CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_CONTROLBAR_CTX,
			this,
			WM_RBUTTONUP,
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

void CExtControlBar::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		return;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if(		m_bPresubclassDialogMode
		||	_GetDockingFrameImpl() == NULL
		||	CExtControlBar::FindHelpMode( this )
		)
	{
		CControlBar::OnNcRButtonUp( nHitTest, point );
		return;
	}

CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	if( ! ::GetCursorPos(&point) )
		return;
HWND hWndTrack = GetOwner()->GetSafeHwnd();
	ASSERT( hWndTrack != NULL && ::IsWindow(hWndTrack) );
CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu(hWndTrack) );
	if(	! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
			pFrame,
			pPopup,
			CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_CONTROLBAR_NC_CTX,
			this,
			WM_RBUTTONUP,
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

void CExtControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if( m_pDockBar == NULL )
	{
        CWnd::OnLButtonDblClk(nFlags, point);
		return;
	}
	ToggleDocking();
}

void CExtControlBar::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
    if( m_pDockBar == NULL || nHitTest != HTCAPTION )
	{
        CWnd::OnNcLButtonDblClk(nHitTest, point);
		return;
	}
	ToggleDocking();
}

bool CExtControlBar::_IsShowContentWhenDragging() const
{
//BOOL bDragShowContent = FALSE;
//    ::SystemParametersInfo(
//		SPI_GETDRAGFULLWINDOWS,
//		0,
//        &bDragShowContent,
//		0
//		);
//	return bDragShowContent ? true : false;

	return false;
}

bool CExtControlBar::_IsShowContentWhenRowResizing() const
{
	return _IsShowContentWhenDragging();
}

bool CExtControlBar::_IsShowContentWhenRowRecalcing() const
{
	return _IsShowContentWhenDragging();
}

void CExtControlBar::_RowResizingStart()
{
	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	CWnd::CancelToolTips();
	m_bRowResizing = true;
	CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
	ASSERT_VALID( m_pDockSite );
	ASSERT( !IsFloating() );
	m_nMetricOffset = IsDockedHorizontally() ?
		m_sizeDockedH.cy : m_sizeDockedV.cx;
	if( !_IsShowContentWhenRowResizing() )
	{
		CPoint ptCursorScreen( 0, 0 );
		::GetCursorPos( &ptCursorScreen );
		m_nTrackerOffset =
			IsDockedHorizontally()
				? ptCursorScreen.y : ptCursorScreen.x;
		_DrawResizingTracker();
	} // if( !_IsShowContentWhenRowResizing() )

	while( m_bRowResizing )
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
			case WM_CANCELMODE:
//			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONUP:
//			case WM_NCLBUTTONDOWN:
			case WM_NCRBUTTONUP:
			case WM_NCRBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_NCMBUTTONDOWN:
			case WM_NCMBUTTONUP:
			case WM_CONTEXTMENU:
				bStop = true;
			break;
			default:
				if(	WM_KEYFIRST <= msg.message
					&& msg.message <= WM_KEYLAST
					)
					bStop = true;
//				else
//				{
//					if( CExtMouseCaptureSink::GetCapture() != GetSafeHwnd() )
//						bStop = true;
//					else
//					{
//						if(	(!CExtPopupMenuWnd::
//								IsKeyPressed(VK_LBUTTON))
//							|| CExtPopupMenuWnd::
//								IsKeyPressed(VK_RBUTTON)
//							|| CExtPopupMenuWnd::
//								IsKeyPressed(VK_MBUTTON)
//							)
//							bStop = true;
//					}
//				}
			break;
			} // switch( msg.message )
			if( bStop )
			{
				_RowResizingStop( true );
				break;
			}
			if( !AfxGetThread()->PumpMessage() )
			{
				PostQuitMessage(0);
				break; // Signal WM_QUIT received
			} // if( !AfxGetThread()->PumpMessage() )
		} // while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		if( m_bRowResizing && CExtControlBar::g_bEnableOnIdleCalls )
		{
			for(	LONG nIdleCounter = 0L;
					::AfxGetThread()->OnIdle(nIdleCounter);
					nIdleCounter ++
					);
		}
	} // while( m_bRowResizing )
}

void CExtControlBar::_RowResizingStop( bool bCancel )
{
	m_bRowResizing = false;
	if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
		CExtMouseCaptureSink::ReleaseCapture();
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT( !IsFloating() );

	if( !_IsShowContentWhenRowResizing() )
	{
		_DrawResizingTracker();

		if( !bCancel )
		{
			bool bHorz = IsDockedHorizontally();
			if(	m_nMetricOffset !=
					(bHorz ? m_sizeDockedH.cy : m_sizeDockedV.cx)
				)
			{
				ExtControlBarVector_t vBars;
				_GetRowExtBars( vBars );
				INT nCountOfBars = vBars.GetSize();
				ASSERT( nCountOfBars > 0 );
				LONG nDiff = 0;
				for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
				{
					CExtControlBar * pBar = vBars[ nBar ];
					ASSERT_VALID( pBar );
//					if( !pBar->IsVisible() )
//						continue;
					if( pBar->IsFixedMode() )
						continue;
					if( bHorz )
					{
						if( nDiff == 0 )
							nDiff = pBar->m_sizeDockedH.cy - m_nMetricOffset;
						pBar->m_sizeDockedH.cy = m_nMetricOffset;
					}
					else
					{
						if( nDiff == 0 )
							nDiff = pBar->m_sizeDockedV.cx - m_nMetricOffset;
						pBar->m_sizeDockedV.cx = m_nMetricOffset;
					}
				} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )

				if( nDiff != 0 )
				{
					UINT nDockBarID = GetSafeDockBarDlgCtrlID();
					ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
					MfcControlBarVector_t vNextRow;
					if( nDockBarID == AFX_IDW_DOCKBAR_TOP
						|| nDockBarID == AFX_IDW_DOCKBAR_LEFT
						)
						_GetNextRowBars( true, vNextRow );
					else
						_GetNextRowBars( false, vNextRow );
					INT nCountOfBars = vNextRow.GetSize();
					for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
					{
						CExtControlBar * pBar = (CExtControlBar *)vNextRow[ nBar ];
						ASSERT_VALID( pBar );
						if( !pBar->IsKindOf( RUNTIME_CLASS(CExtControlBar) ) )
							continue;
						if( pBar->IsFixedMode() )
							continue;
						if( bHorz )
							pBar->m_sizeDockedH.cy += nDiff;
						else
							pBar->m_sizeDockedV.cx += nDiff;
					} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
				} // if( nDiff != 0 )
			}

//xx//
			//m_pDockSite->RecalcLayout();
			GetParentFrame()->RecalcLayout();
		} // if( !bCancel )

		ASSERT_VALID( m_pDockBar );
		m_pDockBar->RedrawWindow(
			NULL, NULL,
			RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW
				|RDW_ALLCHILDREN
				|RDW_NOFRAME
			);

	} // if( !_IsShowContentWhenRowResizing() )
}

void CExtControlBar::_RowResizingUpdateState()
{
	ASSERT( m_bRowResizing );
	ASSERT_VALID( m_pDockSite );

CRect rcDockSite;
	m_pDockSite->GetWindowRect( &rcDockSite );
CSize sizeDockMax =
		rcDockSite.Size()
		- CSize(
			_CalcDesiredMinHW(),
			_CalcDesiredMinVH()
			);
	if( sizeDockMax.cx <= 0 || sizeDockMax.cy <= 0 )
		return;

CPoint ptCursorScreen( 0, 0 );
	::GetCursorPos( &ptCursorScreen );
CPoint ptCursor( ptCursorScreen );

	//ScreenToClient( &ptCursor );
CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
	ptCursor += ptDevOffset;

bool bHorz = IsDockedHorizontally();
	if( bHorz )
	{
		bool bTop = IsDockedAtTop();
		m_nMetricOffset =
			max(
				_CalcDesiredMinVH()*2,
				bTop
					? (ptCursor.y + m_rcRowResize.Height()/2)
					: (m_sizeDockedH.cy - ptCursor.y + m_rcRowResize.Height()/2)
				);
		if( m_nMetricOffset > sizeDockMax.cy )
			m_nMetricOffset = sizeDockMax.cy;
		if( m_nMetricOffset == m_sizeDockedH.cy )
			return;
	} // if( bHorz )
	else
	{
		bool bLeft = IsDockedAtLeft();
		m_nMetricOffset =
			max(
				_CalcDesiredMinHW()*2,
				bLeft
					? (ptCursor.x + m_rcRowResize.Width()/2)
					: (m_sizeDockedV.cx - ptCursor.x + m_rcRowResize.Width()/2 )
				);
		if( m_nMetricOffset > sizeDockMax.cx )
			m_nMetricOffset = sizeDockMax.cx;
		if( m_nMetricOffset == m_sizeDockedV.cx )
			return;
	} // else from if( bHorz )

bool bShowContent = _IsShowContentWhenRowResizing();
	if( bShowContent )
	{

/*
		ExtControlBarVector_t vBars;
		_GetRowExtBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		ASSERT( nCountOfBars > 0 );
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CExtControlBar * pBar = vBars[ nBar ];
			ASSERT_VALID( pBar );
			if( bHorz )
				pBar->m_sizeDockedH.cy = m_nMetricOffset;
			else
				pBar->m_sizeDockedV.cx = m_nMetricOffset;
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
*/

		ExtControlBarVector_t vBars;
		_GetRowExtBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		ASSERT( nCountOfBars > 0 );
		LONG nDiff = 0;
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CExtControlBar * pBar = vBars[ nBar ];
			ASSERT_VALID( pBar );
//					if( !pBar->IsVisible() )
//						continue;
			if( pBar->IsFixedMode() )
				continue;
			if( bHorz )
			{
				if( nDiff == 0 )
					nDiff = pBar->m_sizeDockedH.cy - m_nMetricOffset;
				pBar->m_sizeDockedH.cy = m_nMetricOffset;
			}
			else
			{
				if( nDiff == 0 )
					nDiff = pBar->m_sizeDockedV.cx - m_nMetricOffset;
				pBar->m_sizeDockedV.cx = m_nMetricOffset;
			}
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )

/*
		if( nDiff != 0 )
		{
			UINT nDockBarID = GetSafeDockBarDlgCtrlID();
			ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
			MfcControlBarVector_t vNextRow;
			if( nDockBarID == AFX_IDW_DOCKBAR_TOP
				|| nDockBarID == AFX_IDW_DOCKBAR_LEFT
				)
				_GetNextRowBars( true, vNextRow );
			else
				_GetNextRowBars( false, vNextRow );
			INT nCountOfBars = vNextRow.GetSize();
			for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
			{
				CExtControlBar * pBar = (CExtControlBar *)vNextRow[ nBar ];
				ASSERT_VALID( pBar );
				if( !pBar->IsKindOf( RUNTIME_CLASS(CExtControlBar) ) )
					continue;
				if( pBar->IsFixedMode() )
					continue;
				if( bHorz )
					pBar->m_sizeDockedH.cy += nDiff;
				else
					pBar->m_sizeDockedV.cx += nDiff;
			} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		} // if( nDiff != 0 )
*/

		m_pDockSite->RecalcLayout();

		ASSERT_VALID( m_pDockBar );
		if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) )
			((CExtDockBar *)m_pDockBar)->OnDynamicLayoutUpdate();

		return;
	} // if( bShowContent )

	_DrawResizingTracker();

	m_nTrackerOffset =
		bHorz ? ptCursorScreen.y : ptCursorScreen.x;
	_DrawResizingTracker();
}

void CExtControlBar::_RowRecalcingStart()
{
	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	CWnd::CancelToolTips();
	m_bRowRecalcing = true;
    CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
	ASSERT_VALID( m_pDockSite );
	if( !_IsShowContentWhenRowRecalcing() )
	{
		CPoint ptCursorScreen( 0, 0 );
		::GetCursorPos( &ptCursorScreen );
		m_nTrackerOffset =
			IsDockedHorizontally()
				? ptCursorScreen.x
				: ptCursorScreen.y;
		m_nMetricOffset = 0;
		_DrawRecalcingTracker();
	} // if( !_IsShowContentWhenRowRecalcing() )

	while( m_bRowRecalcing )
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
			case WM_CANCELMODE:
//			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
//			case WM_NCLBUTTONUP:
			case WM_NCLBUTTONDOWN:
			case WM_NCRBUTTONUP:
			case WM_NCRBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_NCMBUTTONDOWN:
			case WM_NCMBUTTONUP:
			case WM_CONTEXTMENU:
				bStop = true;
			break;
			default:
				if(	WM_KEYFIRST <= msg.message
					&& msg.message <= WM_KEYLAST
					)
					bStop = true;
//				else
//				{
//					if( CExtMouseCaptureSink::GetCapture() != GetSafeHwnd() )
//						bStop = true;
//					else
//					{
//						if(	(!CExtPopupMenuWnd::
//								IsKeyPressed(VK_LBUTTON))
//							|| CExtPopupMenuWnd::
//								IsKeyPressed(VK_RBUTTON)
//							|| CExtPopupMenuWnd::
//								IsKeyPressed(VK_MBUTTON)
//							)
//							bStop = true;
//					}
//				}
			break;
			} // switch( msg.message )
			if( bStop )
			{
				_RowRecalcingStop( true );
				break;
			}
			if( !AfxGetThread()->PumpMessage() )
			{
				PostQuitMessage(0);
				break; // Signal WM_QUIT received
			} // if( !AfxGetThread()->PumpMessage() )
		} // while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		if( m_bRowRecalcing && CExtControlBar::g_bEnableOnIdleCalls )
		{
			for(	LONG nIdleCounter = 0L;
					::AfxGetThread()->OnIdle(nIdleCounter);
					nIdleCounter ++
					);
		}
	} // while( m_bRowRecalcing )

}

void CExtControlBar::_RowRecalcingStop( bool bCancel )
{
	m_bRowRecalcing = false;
	if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
		CExtMouseCaptureSink::ReleaseCapture();
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT( !IsFloating() );

	if( !_IsShowContentWhenRowRecalcing() )
	{
		_DrawRecalcingTracker();

		if( !bCancel )
		{
			if( m_nMetricOffset != 0 )
			{
				bool bHorz = IsDockedHorizontally();
				ExtControlBarVector_t vBars;
				_GetRowExtBars( vBars );
				INT nCountOfBars = vBars.GetSize();
				ASSERT( nCountOfBars > 1 );
				bool bThisPassed = false;
				for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
				{
					CExtControlBar * pBar =
						vBars[
							m_bTopRecalcing
								? (nCountOfBars - nBar - 1)
								: nBar
							];
					ASSERT_VALID( pBar );
					if( pBar == this )
					{
						bThisPassed = true;
						continue;
					}
					if( !bThisPassed )
						continue;
					if( bHorz )
					{
						pBar->m_sizeDockedH.cx += m_nMetricOffset;
						if( pBar->m_sizeDockedH.cx < pBar->_CalcDesiredMinHW() )
						{
							m_sizeDockedH.cx -=
								pBar->_CalcDesiredMinHW() - pBar->m_sizeDockedH.cx;
							pBar->m_sizeDockedH.cx = pBar->_CalcDesiredMinHW();
						}
					} // if( bHorz )
					else
					{
						pBar->m_sizeDockedV.cy += m_nMetricOffset;
						if( pBar->m_sizeDockedV.cy < pBar->_CalcDesiredMinVH() )
						{
							m_sizeDockedV.cy -=
								pBar->_CalcDesiredMinVH() - pBar->m_sizeDockedV.cy;
							pBar->m_sizeDockedV.cy = pBar->_CalcDesiredMinVH();
						}
					} // else from if( bHorz )
					break;
				} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )

				if( bHorz )
				{
					m_sizeDockedH.cx -= m_nMetricOffset;
					if( m_sizeDockedH.cx < _CalcDesiredMinHW() )
					{
						m_sizeDockedH.cx +=
							_CalcDesiredMinHW() - m_sizeDockedH.cx;
						m_sizeDockedH.cx = _CalcDesiredMinHW();
					}
				} // if( bHorz )
				else
				{
					m_sizeDockedV.cy -= m_nMetricOffset;
					if( m_sizeDockedV.cy < _CalcDesiredMinVH() )
					{
						m_sizeDockedV.cy +=
							_CalcDesiredMinVH() - m_sizeDockedV.cy;
						m_sizeDockedV.cy = _CalcDesiredMinVH();
					}
				} // else from if( bHorz )

//xx//
				//m_pDockSite->RecalcLayout();
				GetParentFrame()->RecalcLayout();
			} // if( m_nMetricOffset != 0 )
		} // if( !bCancel )

		ASSERT_VALID( m_pDockBar );
		m_pDockBar->RedrawWindow(
			NULL, NULL,
			RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW
				|RDW_ALLCHILDREN
				|RDW_NOFRAME
			);
	
	} // if( !_IsShowContentWhenRowRecalcing() )
}

void CExtControlBar::_RowRecalcingUpdateState()
{
	ASSERT( m_bRowRecalcing );
	ASSERT_VALID( m_pDockBar );
	ASSERT_VALID( m_pDockSite );

CPoint ptCursorScreen( 0, 0 );
	::GetCursorPos( &ptCursorScreen );
CPoint ptCursor( ptCursorScreen );

	//ScreenToClient( &ptCursor );
CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
	ptCursor += ptDevOffset;

bool bSysDragShowContent = _IsShowContentWhenRowRecalcing();
bool bHorz = IsDockedHorizontally();
	if( bHorz )
	{
		if( m_bTopRecalcing )
		{
			INT nMetric =
				max(
					_CalcDesiredMinHW(),
					(m_sizeDockedH.cx - ptCursor.x + m_rcRowRecalcUp.Width()/2)
					);
			m_nMetricOffset = m_sizeDockedH.cx - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedH.cx = nMetric;
		} // if( m_bTopRecalcing )
		else
		{
			INT nMetric =
				max(
					_CalcDesiredMinHW(),
					(ptCursor.x + m_rcRowRecalc.Width()/2)
					);
			m_nMetricOffset = m_sizeDockedH.cx - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedH.cx = nMetric;
		} // else from if( bTopRecalcing )
	} // if( bHorz )
	else
	{
		if( m_bTopRecalcing )
		{
			INT nMetric =
				max(
					_CalcDesiredMinVH(),
					(m_sizeDockedV.cy - ptCursor.y + m_rcRowRecalcUp.Height()/2)
					);
			m_nMetricOffset = m_sizeDockedV.cy - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedV.cy = nMetric;
		} // if( m_bTopRecalcing )
		else
		{
			INT nMetric =
				max(
					_CalcDesiredMinVH(),
					(ptCursor.y + m_rcRowRecalc.Height()/2)
					);
			m_nMetricOffset = m_sizeDockedV.cy - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedV.cy = nMetric;
		} // else from if( m_bTopRecalcing )
	} // else from if( bHorz )
	ASSERT( m_nMetricOffset != 0 );

	if( bSysDragShowContent )
	{
		ExtControlBarVector_t vBars;
		_GetRowExtBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		ASSERT( nCountOfBars > 1 );
		bool bThisPassed = false;
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CExtControlBar * pBar =
				vBars[
					m_bTopRecalcing
						? (nCountOfBars - nBar - 1)
						: nBar
					];
			ASSERT_VALID( pBar );
			if( pBar == this )
			{
				bThisPassed = true;
				continue;
			}
			if( !bThisPassed )
				continue;
			if( bHorz )
			{
				pBar->m_sizeDockedH.cx += m_nMetricOffset;
				if( pBar->m_sizeDockedH.cx < pBar->_CalcDesiredMinHW() )
				{
					m_sizeDockedH.cx -=
						pBar->_CalcDesiredMinHW() - pBar->m_sizeDockedH.cx;
					pBar->m_sizeDockedH.cx = pBar->_CalcDesiredMinHW();
				}
			} // if( bHorz )
			else
			{
				pBar->m_sizeDockedV.cy += m_nMetricOffset;
				if( pBar->m_sizeDockedV.cy < pBar->_CalcDesiredMinVH() )
				{
					m_sizeDockedV.cy -=
						pBar->_CalcDesiredMinVH() - pBar->m_sizeDockedV.cy;
					pBar->m_sizeDockedV.cy = pBar->_CalcDesiredMinVH();
				}
			} // else from if( bHorz )
			break;
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )

		m_pDockSite->RecalcLayout();

		ASSERT_VALID( m_pDockBar );
		if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) )
			((CExtDockBar *)m_pDockBar)->OnDynamicLayoutUpdate();
		
		return;
	} // if( bSysDragShowContent )

	ASSERT_VALID( m_pDockBar );
CRect rcDockBarWnd;
	m_pDockBar->GetWindowRect( &rcDockBarWnd );

INT nMinTrackerOffset = bHorz ?
		rcDockBarWnd.left : rcDockBarWnd.top;

	if( m_bTopRecalcing )
	{
		MfcControlBarVector_t vBars;
		_GetRowBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CControlBar * pBar = vBars[ nBar ];
			ASSERT_VALID( pBar );
			if( !pBar->IsVisible() )
				continue;
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar != NULL && pExtBar->IsFixedMode() )
				pExtBar = NULL;
			if( pExtBar == NULL )
			{
				CRect rcBarWnd;
				pBar->GetWindowRect( &rcBarWnd );
				nMinTrackerOffset += bHorz ?
					rcBarWnd.Width() : rcBarWnd.Height();
			} // if( pExtBar == NULL )
			else
			{
				nMinTrackerOffset += bHorz
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH();
			} // else from if( pExtBar == NULL )
			if( pBar == this )
				break;
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	} // if( m_bTopRecalcing )
	else
	{
		nMinTrackerOffset = bHorz
			? rcBarWnd.left + _CalcDesiredMinHW()
			: rcBarWnd.top  + _CalcDesiredMinVH()
			;
	} // else from if( m_bTopRecalcing )

INT nMaxTrackerOffset = 0;

	if( m_bTopRecalcing )
	{
		nMaxTrackerOffset = bHorz
			? rcBarWnd.right  - _CalcDesiredMinHW()
			: rcBarWnd.bottom - _CalcDesiredMinVH()
			;
	} // if( m_bTopRecalcing )
	else
	{
		nMaxTrackerOffset = bHorz ?
			rcDockBarWnd.right : rcDockBarWnd.bottom;
		MfcControlBarVector_t vBars;
			_GetRowBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CControlBar * pBar = vBars[ nCountOfBars - nBar - 1 ];
			ASSERT_VALID( pBar );
			if( !pBar->IsVisible() )
				continue;
			if( pBar == this )
				break;
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar != NULL && pExtBar->IsFixedMode() )
				pExtBar = NULL;
			if( pExtBar == NULL )
			{
				CRect rcBarWnd;
				pBar->GetWindowRect( &rcBarWnd );
				nMaxTrackerOffset -= bHorz ?
					rcBarWnd.Width() : rcBarWnd.Height();
			} // if( pExtBar == NULL )
			else
			{
				nMaxTrackerOffset -= bHorz
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH();
			} // else from if( pExtBar == NULL )
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	} // else from if( m_bTopRecalcing )

	_DrawRecalcingTracker();
	m_nTrackerOffset = bHorz
		? ptCursorScreen.x
		: ptCursorScreen.y;
	ASSERT( nMinTrackerOffset <= nMaxTrackerOffset );
	if( m_nTrackerOffset < nMinTrackerOffset )
		m_nTrackerOffset = nMinTrackerOffset;
	if( m_nTrackerOffset > nMaxTrackerOffset )
		m_nTrackerOffset = nMaxTrackerOffset;
	_DrawRecalcingTracker();
}

void CExtControlBar::_DrawResizingTracker()
{
	ASSERT( !IsFloating() );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );

CRect rcDockBar;
	m_pDockBar->GetWindowRect( &rcDockBar );
	if( rcDockBar.IsRectEmpty() )
		return;

CRect rcTracker( rcDockBar );
	if( IsDockedHorizontally() )
	{
		rcTracker.top =
			m_nTrackerOffset - m_rcRowResize.Height()/2;
		rcTracker.bottom =
			m_nTrackerOffset + m_rcRowResize.Height()/2;
	} // if( IsDockedHorizontally() )
	else
	{
		rcTracker.left =
			m_nTrackerOffset - m_rcRowResize.Width()/2;
		rcTracker.right =
			m_nTrackerOffset + m_rcRowResize.Width()/2;
	} // else from if( IsDockedHorizontally() )

CFrameWnd * pFrame = GetDockingFrame(); // m_pDockSite;
	ASSERT_VALID( pFrame );

CRect rcDockingFrame;
	pFrame->GetWindowRect( &rcDockingFrame );
	rcTracker.OffsetRect( -rcDockingFrame.TopLeft() );

CDC * pDC = pFrame->GetDCEx( NULL, __DOCKSITE_DCX_FLAGS );
CBrush * pBrushOld =
		pDC->SelectObject( CDC::GetHalftoneBrush() );
    pDC->PatBlt(
		rcTracker.left, rcTracker.top,
		rcTracker.Width(), rcTracker.Height(),
		PATINVERT
		);
    pDC->SelectObject(pBrushOld);
    pFrame->ReleaseDC(pDC);
}

void CExtControlBar::_DrawRecalcingTracker()
{
	ASSERT( !IsFloating() );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );

CRect rcTracker;
	GetWindowRect( &rcTracker );

	if( IsDockedHorizontally() )
	{
		INT nMetric =
			m_bTopRecalcing
				? m_rcRowRecalcUp.Width()/2
				: m_rcRowRecalc.Width()/2;
		rcTracker.left =
			m_nTrackerOffset - nMetric;
		rcTracker.right =
			m_nTrackerOffset + nMetric;
	} // if( IsDockedHorizontally() )
	else
	{
		INT nMetric =
			m_bTopRecalcing
				? m_rcRowRecalcUp.Height()/2
				: m_rcRowRecalc.Height()/2;
		rcTracker.top =
			m_nTrackerOffset - nMetric;
		rcTracker.bottom =
			m_nTrackerOffset + nMetric;
	} // else from if( IsDockedHorizontally() )

CFrameWnd * pFrame = GetDockingFrame(); // m_pDockSite;
	ASSERT_VALID( pFrame );

CRect rcDockingFrame;
	pFrame->GetWindowRect( &rcDockingFrame );
	rcTracker.OffsetRect( -rcDockingFrame.TopLeft() );

CDC * pDC = pFrame->GetDCEx( NULL, __DOCKSITE_DCX_FLAGS );
CBrush * pBrushOld =
		pDC->SelectObject( CDC::GetHalftoneBrush() );
    pDC->PatBlt(
		rcTracker.left, rcTracker.top,
		rcTracker.Width(), rcTracker.Height(),
		PATINVERT
		);
    pDC->SelectObject(pBrushOld);
    pFrame->ReleaseDC(pDC);
}

bool CExtControlBar::IsOleIpObjActive(
	CFrameWnd * pFrameSearch
	)
{
	if( FindPrintPreviewMode( pFrameSearch ) )
		return false;
	if( pFrameSearch == NULL )
	{
		pFrameSearch =
			DYNAMIC_DOWNCAST( CFrameWnd, AfxGetMainWnd() );
		if( pFrameSearch == NULL )
			return false;
	}
	ASSERT_VALID( pFrameSearch );
	if( pFrameSearch->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
	{
		pFrameSearch = pFrameSearch->GetParentFrame();
		ASSERT_VALID( pFrameSearch );
		ASSERT( !pFrameSearch->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
	}
	if( pFrameSearch->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) )
	{
		pFrameSearch = ((CMDIFrameWnd*)pFrameSearch)->GetActiveFrame();
		if( pFrameSearch == NULL )
			return false;
	}
bool bOleInplaceItemActivated = false;
CView * pActiveView = pFrameSearch->GetActiveView();
	if(		pActiveView != NULL
		&&	(! pActiveView->IsKindOf(RUNTIME_CLASS(CPreviewView)) )
		)
	{
		ASSERT_VALID( pActiveView );
		CDocument * pActiveDoc = pActiveView->GetDocument();
		if( pActiveDoc != NULL )
		{
			ASSERT_VALID( pActiveDoc );
			COleDocument * pActiveOleDoc = 
				DYNAMIC_DOWNCAST(COleDocument,pActiveDoc);
			if( pActiveOleDoc != NULL )
			{
				ASSERT_VALID( pActiveOleDoc );
				COleClientItem * pItem =
					pActiveOleDoc->GetInPlaceActiveItem(pActiveView);
				if( pItem != NULL )
					bOleInplaceItemActivated = true;
			} // if( pActiveOleDoc != NULL )
		} // if( pActiveDoc != NULL )
	} // if( pActiveView != NULL ...
	return bOleInplaceItemActivated;
}

bool CExtControlBar::IsOleIpObjActive() const
{
	ASSERT_VALID( this );
	return IsOleIpObjActive( _GetDockingFrameImpl() );
}

bool CExtControlBar::SafeDisplayBar()
{
	if( m_bPresubclassDialogMode )
	{
		if( GetStyle() & WS_VISIBLE )
			return true;
		return false;
	}
	if( IsVisible() )
		return true;
	if( m_nStateFlags &
			//(CControlBar::delayHide | CControlBar::tempHide)
			CControlBar::tempHide
		)
		return false;
CFrameWnd * pFrame =
		_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
bool bOleInplaceItemActivated =
		//CExtControlBar::IsOleIpObjActive( pFrame );
		IsOleIpObjActive();
	if( bOleInplaceItemActivated &&
			(m_dwStyle & CBRS_HIDE_INPLACE)
		)
		return false;
	pFrame->ShowControlBar( this, TRUE, TRUE );
	pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
	pFrame->RecalcLayout();
HWND hWndThis = GetSafeHwnd();
	CExtPopupMenuWnd::PassMsgLoop(false);
	if( ! ::IsWindow(hWndThis) )
		return false;
	if( !IsVisible() )
	{
		ASSERT( FALSE );
		return false;
	}
	return true;
}

bool CExtControlBar::IsSingleOnRow(
	bool bOnlyFixed, // = false
	bool bOnlyNonFixed // = false
	) const
{
	ASSERT_VALID( this );
	ASSERT( !IsFloating() );
	ASSERT_VALID( m_pDockBar );
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
bool bThisFound = false;
INT nCountInRow = 0;
	for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			if( bThisFound )
				break;
			nCountInRow = 0;
			continue;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_KINDOF( CControlBar, pBar );

		if( pBar == (CControlBar *)this )
			bThisFound = true;

		if( !pBar->IsVisible() )
			continue;

		if( bOnlyFixed || bOnlyNonFixed )
		{
			bool bFixed = true;
			if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
				&& !((CExtControlBar *)pBar)->IsFixedMode()
				)
				bFixed = false;
			if( bOnlyFixed && !bFixed )
				continue;
			if( !bOnlyFixed && bFixed )
				continue;
		} // if( bOnlyFixed || bOnlyNonFixed )

		nCountInRow++;

		if( bThisFound && nCountInRow > 1 )
			return false;
	} // for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )

	ASSERT( nCountInRow >= 1 );
	return (nCountInRow == 1) ? true : false;
}

void CExtControlBar::MakeSingleOnRow()
{
CFrameWnd * pFrame = GetDockingFrame();
	ASSERT_VALID( pFrame );
	pFrame->RecalcLayout();
	ASSERT_VALID( m_pDockBar );
int nPos = m_pDockBar->FindBar( this );
	ASSERT( 1 <= nPos && nPos < m_pDockBar->m_arrBars.GetSize() );
	if( nPos < m_pDockBar->m_arrBars.GetSize()
		&&	m_pDockBar->m_arrBars[nPos+1] != NULL
		)
		m_pDockBar->m_arrBars.InsertAt(
			nPos+1, ((CControlBar*)NULL) );
	if( m_pDockBar->m_arrBars[nPos-1] != NULL )
		m_pDockBar->m_arrBars.InsertAt(
			nPos, ((CControlBar*)NULL) );
	if( IsDockedHorizontally() )
		m_sizeDockedH = CSize( MAXSHORT, m_sizeDockedH.cy );
	else
		m_sizeDockedV = CSize( m_sizeDockedV.cx, MAXSHORT );
	pFrame->RecalcLayout();
}

bool CExtControlBar::IsMinimizedOnRow() const
{
	if(	IsDockedVertically()
		&& m_sizeDockedV.cy == _CalcDesiredMinVH()
		)
		return true;
	if(	IsDockedHorizontally()
		&& m_sizeDockedH.cx == _CalcDesiredMinHW()
		)
		return true;
	return false;
}

bool CExtControlBar::IsMaximizedOnRow() const
{
	if( IsFloating() )
		return false;
	if(IsMinimizedOnRow())
		return false;
ExtControlBarVector_t vBars;
	(const_cast<CExtControlBar *>(this))->
		_GetRowExtBars(
			vBars
			);
INT nCountOfBars = vBars.GetSize();
	for( int nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT( pBar != NULL );
		if( pBar == this )
			continue;
		if( !(pBar->IsMinimizedOnRow()) )
			return false;
	}
	return true;
}

void CExtControlBar::MinimizeOnRow()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( GetSafeHwnd() ) != NULL );

	if( IsFloating () || IsSingleOnRow() )
		return;

ExtControlBarVector_t vBars;
    _GetRowExtBars(vBars);
INT nCountOfBars = vBars.GetSize();
	if( nCountOfBars <= 1 )
		return;
CExtDockBar * pDockBar =
		(CExtDockBar*) m_pDockBar;
	ASSERT( pDockBar != NULL );
BOOL bHorz = IsDockedHorizontally();

INT nCalcExtentMax = 0;
	if( bHorz )
	{
		INT nMin = _CalcDesiredMinHW();
		nCalcExtentMax = m_sizeDockedH.cx - nMin;
		m_sizeDockedH.cx = nMin;
		ASSERT( nCalcExtentMax >= 0 );
	}
	else
	{
		INT nMin = _CalcDesiredMinVH();
		nCalcExtentMax = m_sizeDockedV.cy - nMin;
		m_sizeDockedV.cy = nMin;
		ASSERT( nCalcExtentMax >= 0 );
	}

	for( INT nBar = 0; nCalcExtentMax > 0; nBar++ )
	{
		if( nBar == nCountOfBars )
			nBar = 0;
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT_VALID( pBar );
		if( pBar != this )
		{
			if( bHorz )
				pBar->m_sizeDockedH.cx++;
			else
				pBar->m_sizeDockedV.cy++;
			nCalcExtentMax--;
		} // if( pBar != this )
	} // for( nBar = 0; nCalcExtentMax > 0; nBar++ )


//	m_pDockSite->RecalcLayout();
	GetParentFrame()->RecalcLayout();
	
	_GetDockingFrameImpl()->SetFocus();
}

void CExtControlBar::MaximizeOnRow()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( GetSafeHwnd() ) != NULL );

	if( IsFloating () || IsSingleOnRow() )
		return;

ExtControlBarVector_t vBars;
    _GetRowExtBars(vBars);
INT nCountOfBars = vBars.GetSize();
	if( nCountOfBars <= 1 )
		return;
CExtDockBar * pDockBar =
		(CExtDockBar*) m_pDockBar;
	ASSERT( pDockBar != NULL );
BOOL bHorz = IsDockedHorizontally();
INT nMetric = 0;
	for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT_VALID( pBar );
		if( pBar == this )
			continue;
		if( bHorz )
		{
			INT nMin = pBar->_CalcDesiredMinHW();
			nMetric += pBar->m_sizeDockedH.cx - nMin;
			pBar->m_sizeDockedH.cx = nMin;
		}
		else
		{
			INT nMin = pBar->_CalcDesiredMinVH();
			nMetric += pBar->m_sizeDockedV.cy - nMin;
			pBar->m_sizeDockedV.cy = nMin;
		}
	} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	if( bHorz )
		m_sizeDockedH.cx += nMetric;
	else
		m_sizeDockedV.cy += nMetric;

//	m_pDockSite->RecalcLayout();
	GetParentFrame()->RecalcLayout();
}

void CExtControlBar::OnRepositionSingleChild(
	int cx, // = -1, // if negative - get from client area
	int cy, // = -1
	bool bRedraw // = TRUE
	)
{
	if( !m_bReposSingleChildMode )
		return;

    // automatic child resizing - only one child is allowed
CWnd * pWnd = GetWindow( GW_CHILD );
    if( pWnd != NULL
		&& ::IsWindow( pWnd->GetSafeHwnd() )
		)
	{
		CRect rcClient;
		if( cx < 0 || cy < 0 )
			GetClientRect( &rcClient );
		else
			rcClient.SetRect( 0, 0, cx, cy );
		if( IsFloating() )
		{
			CSize sizeFrame(
				::GetSystemMetrics(SM_CXSIZEFRAME), // SM_CXFIXEDFRAME
				::GetSystemMetrics(SM_CYSIZEFRAME)  // SM_CYFIXEDFRAME
				);
			rcClient.DeflateRect( sizeFrame );
		} // if( IsFloating() )
		pWnd->MoveWindow( &rcClient, FALSE);
		ASSERT( pWnd->GetWindow(GW_HWNDNEXT) == NULL );
		//pWnd->ShowWindow( IsMinimizedOnRow() ? SW_HIDE : SW_SHOW );
	}

	if( !bRedraw )
		return;

	//pWnd->
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE // |RDW_UPDATENOW
			|RDW_ERASE // |RDW_ERASENOW
			//|RDW_INTERNALPAINT
			|RDW_ALLCHILDREN
			|RDW_NOFRAME
		);
}

void CExtControlBar::OnSize(UINT nType, int cx, int cy) 
{
	//CControlBar::OnSize(nType, cx, cy);
	nType;
	if( m_bUpdatingChain )
		return;
    
	OnRepositionSingleChild( cx, cy, true );

CFrameWnd * pFrame = GetParentFrame();
	if( pFrame != NULL
		&& pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd))
		)
	{
		OnNcAreaButtonsReposition();
		pFrame->SendMessage( WM_NCPAINT );
	}
}

void CExtControlBar::_Dragging_OnStart()
{
	CWnd::CancelToolTips();
	CExtPopupMenuWnd::CancelMenuTracking();

	ASSERT_VALID( m_pDockBar );
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
CExtDockDynTabBar * pTabbedDockBar =
		DYNAMIC_DOWNCAST(
			CExtDockDynTabBar,
			m_pDockBar
			);
	if( pTabbedDockBar != NULL )
	{
		CExtDynTabControlBar * pTabbedBar =
			STATIC_DOWNCAST(
				CExtDynTabControlBar,
				pTabbedDockBar->GetParent()
				);
		ASSERT_VALID( pTabbedBar );
		pTabbedBar->SaveIndex( this );
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	GetOwner()->
		SendMessage(
			WM_SETMESSAGESTRING,
			IDS_EXTSTATUS_PERVENT_DOCKING
			);
}

void CExtControlBar::_Dragging_OnStop(
	InternalDraggingState_t & _dsOld,
	InternalDraggingState_t & _dsNew,
	bool bCancel
	)
{
	_dsOld, _dsNew, bCancel;

	GetOwner()->
		SendMessage(
			WM_SETMESSAGESTRING,
			AFX_IDS_IDLEMESSAGE
			);

	ASSERT_VALID( this );
	if( IsFixedMode() )
		return;

	ASSERT_VALID( (&_dsOld) );
	ASSERT_VALID( (&_dsNew) );
	ASSERT( _dsNew.ExtBarSrcGet() == this );

	ASSERT_VALID( m_pDockBar );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

CExtDockDynTabBar * pTabbedDockBar =
		DYNAMIC_DOWNCAST(
			CExtDockDynTabBar,
			m_pDockBar
			);
	if( pTabbedDockBar != NULL )
	{
		CExtDynTabControlBar * pTabbedBar =
			STATIC_DOWNCAST(
				CExtDynTabControlBar,
				pTabbedDockBar->GetParent()
				);
		ASSERT_VALID( pTabbedBar );
		pTabbedBar->RemoveTemporaryItems();
		if( bCancel )
		{
			pTabbedBar->RestoreIndex( this );
			pTabbedBar->SelectControlBar( this );
		} // if( bCancel )
	} // if( pTabbedDockBar != NULL )

	if( bCancel && _dsNew.ExtBarDstGet() != NULL )
	{
		CExtDynTabControlBar * pTabbedBar =
			DYNAMIC_DOWNCAST(
				CExtDynTabControlBar,
				_dsNew.ExtBarDstGet()
				);
		if( pTabbedBar != NULL )
		{
			pTabbedBar->RemoveTemporaryItems( true );
		}
	} // if( bCancel && _dsNew.ExtBarDstGet() != NULL )

#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

}

void CExtControlBar::_SetCursor(
	const CPoint & point
	)
{
bool bCustomizeMode = false;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if( pSite != NULL )
	{
		if( pSite->DraggedNodeGet() != NULL )
			return;
		bCustomizeMode = pSite->IsCustomizeMode();
	} // if( pSite != NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if( !bCustomizeMode )
	{
		if( CExtControlBar::FindHelpMode(this) )
			return;
	} // if( !bCustomizeMode )
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	else
	{
		ASSERT( !CExtControlBar::FindHelpMode(this) );
	} // else from if( !bCustomizeMode )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

HCURSOR hCursor = NULL;
	if( !m_bPresubclassDialogMode )
	{
		if( m_bDragging || m_bRowResizing || m_bRowRecalcing )
		{ // if can analyze resize cursors
			if( m_bDragging )
			{
				ASSERT( !(m_bRowResizing || m_bRowRecalcing) );
				if( IsFixedMode() )
				{
					e_calc_fixed_resizing_t eCFR =
						g_DragStateOld.GetCFR();
					if( eCFR == __ECFR_HORZ )
						hCursor = g_hCursorResizeH;
					else if( eCFR == __ECFR_VERT )
						hCursor = g_hCursorResizeV;
					else
						hCursor = g_hCursorDrag;
					ASSERT( hCursor != NULL );
				}
			} // if( m_bDragging )
			else
			{
				ASSERT( !m_bDragging );
				ASSERT( !IsFloating() );
				if( IsDockedVertically() )
				{
					if( m_bRowRecalcing )
					{
						ASSERT( !m_bRowResizing );
						hCursor = g_hCursorResizeV;
						ASSERT( hCursor != NULL );
					} // if( m_bRowRecalcing )
					else
					{
						ASSERT( !m_bRowRecalcing );
						hCursor = g_hCursorResizeH;
						ASSERT( hCursor != NULL );
					} // else from if( m_bRowRecalcing )
				} // if( IsDockedVertically() )
				else
				{
					if( m_bRowRecalcing )
					{
						ASSERT( !m_bRowResizing );
						hCursor = g_hCursorResizeH;
						ASSERT( hCursor != NULL );
					} // if( m_bRowRecalcing )
					else
					{
						ASSERT( !m_bRowRecalcing );
						hCursor = g_hCursorResizeV;
						ASSERT( hCursor != NULL );
					} // else if( m_bRowRecalcing )
				} // else from if( IsDockedVertically() )
			} // else from if( m_bDragging )
		} // if can analyze resize cursors
		else
		{ // if can not analyze resize cursors

			if( !IsFloating() )
			{
				CRect rcBarWnd;
				GetWindowRect( &rcBarWnd );
				CPoint ptDevOffset = -rcBarWnd.TopLeft();
				CPoint ptWnd( point );
				ClientToScreen( &ptWnd );
				ptWnd += ptDevOffset;
				if( m_rcGrip.PtInRect(ptWnd) )
				{
					INT nCountOfNcButtons = NcButtons_GetCount();
					for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
					{
						CExtBarNcAreaButton * pBtn =
							NcButtons_GetAt( nBtn );
						hCursor =
							pBtn->OnNcAreaQueryCursor( ptWnd );
						if( hCursor != NULL )
							break;
					} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
					if( hCursor == NULL )
					{
						hCursor = g_hCursorDrag;
						ASSERT( hCursor != NULL );
					} // if( hCursor == NULL )
				} // if( m_rcGrip.PtInRect(ptWnd) )
			} // if( !IsFloating() )
		
		} // if can not analyze resize cursors
	} // if( !m_bPresubclassDialogMode )

	if( hCursor == NULL )
	{
		hCursor = (HCURSOR)
			::GetClassLong( m_hWnd, GCL_HCURSOR );
		ASSERT( hCursor != NULL );
	} // if( hCursor == NULL )
	
	if( GetCursor() != hCursor )
		::SetCursor( hCursor );
}

CExtControlBar * CExtControlBar::_DraggingGetBar() // return currently dagging controlbar
{
	if( g_DragStateOld.IsEmpty() )
		return NULL;
	return g_DragStateOld.ExtBarSrcGet();
}

bool CExtControlBar::_DraggingCancel() // returns true if dragging was really canceled
{
CExtControlBar * pBar = _DraggingGetBar();
	if( pBar == NULL )
		return false;
	pBar->_DraggingStop( true );
	return true;
}

void CExtControlBar::_DraggingStart(
	const CPoint & point,
	const CPoint & pointOffset, // = CPoint( 0, 0 );
	CSize sizeWaitMouseMove // = CSize( 1, 1 )
	)
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	ASSERT( !m_bDragging );

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		pSite->DropTargetPopupCancelEx( -2 );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	// (+ v.2.23)
	// detect valid dragging start event
HWND hWndOwn = GetSafeHwnd();
	ASSERT( hWndOwn != NULL && ::IsWindow(hWndOwn) );
CExtMouseCaptureSink::SetCapture( hWndOwn );
//sizeWaitMouseMove.cx = sizeWaitMouseMove.cy = 5; // debug
	if( sizeWaitMouseMove.cx > 0 && sizeWaitMouseMove.cy > 0 )
	{
		::WaitMessage();
		CPoint ptMouseStart( 0, 0 ), ptMouseCurrent( 0, 0 );
		::GetCursorPos( &ptMouseStart );
		bool bEnableStartDragging = true, bMouseShiftDetected = false;
		for( ; bEnableStartDragging && (!bMouseShiftDetected); )
		{
			for( 	MSG msg;
					::IsWindow(hWndOwn)
					&&	PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE );
					)
			{ // wait any mouse movement look
				if( ::IsWindow(hWndOwn) )
				{
					if( msg.hwnd == hWndOwn )
					{
						if( msg.message == WM_MOUSEMOVE )
						{
							::GetCursorPos( &ptMouseCurrent );
							CSize sizeMouseShift(
								abs( (ptMouseCurrent.x - ptMouseStart.x) ),
								abs( (ptMouseCurrent.y - ptMouseStart.y) )
								);
							if(		sizeMouseShift.cx >= sizeWaitMouseMove.cx
								||	sizeMouseShift.cy >= sizeWaitMouseMove.cy
								)
							{
								bMouseShiftDetected = true;
								break; // start dragging
							}
							PeekMessage( &msg, hWndOwn, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE );
							continue;
						} // if( msg.message == WM_MOUSEMOVE )
						switch( msg.message )
						{
						case WM_CAPTURECHANGED:
							if( ((HWND)msg.lParam) != hWndOwn )
							{
								bEnableStartDragging = false;
								break;
							} // if( ((HWND)msg.lParam) != hWndOwn )
							continue;
						case WM_CANCELMODE:
						case WM_ACTIVATEAPP:
						case WM_CONTEXTMENU:
							bEnableStartDragging = false;
							break;
						default:
							if(		(	WM_KEYFIRST <= msg.message
									&&	msg.message <= WM_KEYLAST
									)
								||	(	WM_MOUSEFIRST <= msg.message
									&&	msg.message <= WM_MOUSELAST
									)
								)
								bEnableStartDragging = false;
							break;
						} // switch( msg.message )
					} // if( msg.hwnd == hWndOwn )
					else
					{
						switch( msg.message )
						{
						case WM_CANCELMODE:
						case WM_ACTIVATEAPP:
						case WM_CONTEXTMENU:
							bEnableStartDragging = false;
							break;
						default:
							if(		(	WM_KEYFIRST <= msg.message
									&&	msg.message <= WM_KEYLAST
									)
								||	(	WM_MOUSEFIRST <= msg.message
									&&	msg.message <= WM_MOUSELAST
									)
								)
								bEnableStartDragging = false;
							break;
						} // switch( msg.message )
					} // else from if( msg.hwnd == hWndOwn )
				} // if( ::IsWindow(hWndOwn) )
				else
					bEnableStartDragging = false;
				if( ! bEnableStartDragging )
					break;
				if( !AfxGetThread()->PumpMessage() )
				{
					PostQuitMessage(0);
					break; // Signal WM_QUIT received
				} // if( !AfxGetThread()->PumpMessage() )
				if( ! ( ::IsWindow(hWndOwn) ) )
				{
					bEnableStartDragging = false;
					break;
				}
				if( CExtControlBar::g_bEnableOnIdleCalls )
				{
					for(	LONG nIdleCounter = 0L;
							::AfxGetThread()->OnIdle(nIdleCounter);
							nIdleCounter ++
							);
				} // if( CExtControlBar::g_bEnableOnIdleCalls )
				if( ! ( ::IsWindow(hWndOwn) ) )
				{
					bEnableStartDragging = false;
					break;
				}
				::WaitMessage();
			} // wait any mouse movement look
		} // for( ; bEnableStartDragging && (!bMouseShiftDetected); )
		if( ! bMouseShiftDetected )
			bEnableStartDragging = false;
		if( ! bEnableStartDragging )
		{
			if(		::IsWindow(hWndOwn)
				&&	CExtMouseCaptureSink::GetCapture() == hWndOwn
				)
				CExtMouseCaptureSink::ReleaseCapture();
			return;
		} // if( ! bEnableStartDragging )
	} // if( sizeWaitMouseMove.cx > 0 && sizeWaitMouseMove.cy > 0 )
	ASSERT( CExtMouseCaptureSink::GetCapture() == hWndOwn );

	ASSERT( g_DragStateOld.IsEmpty() );
	ASSERT( g_DragStateNew.IsEmpty() );
	g_DragStateOld.ExtBarSrcSet( this );
	g_DragStateNew.ExtBarSrcSet( this );
	g_DragStateNew.SetEmpty( false );

    ASSERT( m_pDockBar != NULL );
    ASSERT( m_pDockContext != NULL );

	_Dragging_OnStart();
///	CExtMouseCaptureSink::SetCapture( GetSafeHwnd() ); // (- v.2.23)
	m_bDragging = TRUE;

INT nCyGripper = min( m_rcGrip.Width(), m_rcGrip.Height() );
CPoint ptHelperDragOffsetDefault(
		( nCyGripper > 2 )
			? nCyGripper
			: ( ::GetSystemMetrics(SM_CYSMCAPTION) / 2 )
			,
		( nCyGripper > 2 )
			? ( nCyGripper / 2 )
			: ( ::GetSystemMetrics(SM_CYSMCAPTION) / 2 )
		);
	m_ptHelperDragOffsetFloated = ptHelperDragOffsetDefault;
	m_ptHelperDragOffsetDockedH = ptHelperDragOffsetDefault;
	m_ptHelperDragOffsetDockedV = ptHelperDragOffsetDefault;
	if( pointOffset.x != 0 || pointOffset.y != 0 )
	{
		if( IsFloating() )
			m_ptHelperDragOffsetFloated = pointOffset;
		else if( IsDockedHorizontally() )
			m_ptHelperDragOffsetDockedH = pointOffset;
		else
			m_ptHelperDragOffsetDockedV = pointOffset;
	} // if( pointOffset.x != 0 || pointOffset.y != 0 )

	_DraggingUpdateState(
		point,
		CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) ? true : false
		);
	ASSERT( hWndOwn != NULL && ::IsWindow(hWndOwn) );
	while( m_bDragging && ::IsWindow(hWndOwn) )
	{
		::WaitMessage();
		MSG msg;
		// Process all the messages in the message queue
		while( ::IsWindow(hWndOwn) && PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		{
			bool bStop = false;
			switch( msg.message )
			{
			case WM_ACTIVATEAPP:
			case WM_CANCELMODE:
//			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONUP:
//			case WM_NCLBUTTONDOWN:
			case WM_NCRBUTTONUP:
			case WM_NCRBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_NCMBUTTONDOWN:
			case WM_NCMBUTTONUP:
			case WM_CONTEXTMENU:
				bStop = true;
			break;
			case WM_MOUSEMOVE:
				{
					CPoint pt( (DWORD)msg.lParam );
					if( msg.hwnd != m_hWnd )
					{
						::ClientToScreen( msg.hwnd, &pt );
						::ScreenToClient( m_hWnd, &pt );
					}
					if( _OnMouseMoveMsg((UINT)msg.wParam,pt) )
					{
						PeekMessage(&msg,NULL,msg.message,msg.message,PM_REMOVE);
						continue;
					} // if( pTestBar->_OnMouseMoveMsg((UINT)msg.wParam,pt) )
				}
			default:
				if(	WM_KEYFIRST <= msg.message
					&& msg.message <= WM_KEYLAST
					)
				{
					if( g_bUpdatingDragState )
						break;
					if(		(	msg.message == WM_KEYDOWN
							||	msg.message == WM_KEYUP )
						&&	msg.wParam == VK_CONTROL
						)
					{
						CPoint ptCursor( 0, 0 );
						::GetCursorPos( &ptCursor );
						if( m_ptDraggingLast == ptCursor )
							m_ptDraggingLast.x --;
//						_DraggingUpdateState(
//							ptCursor,
//							(msg.message == WM_KEYDOWN) ? true : false
//							);
						ScreenToClient( &ptCursor );
						PostMessage(
							WM_MOUSEMOVE,
							(msg.message == WM_KEYDOWN) ? (VK_CONTROL) : 0L,
							MAKELPARAM( ptCursor.x, ptCursor.y )
							);
						break;
					}
					bStop = true;
				}
				else
				{
					if( CExtMouseCaptureSink::GetCapture() != GetSafeHwnd() )
						bStop = true;
					else
					{
						if(	(!CExtPopupMenuWnd::
								IsKeyPressed(VK_LBUTTON))
							|| CExtPopupMenuWnd::
								IsKeyPressed(VK_RBUTTON)
							|| CExtPopupMenuWnd::
								IsKeyPressed(VK_MBUTTON)
							)
							bStop = true;
					}
				}
			break;
			} // switch( msg.message )
			if( ! ( ::IsWindow(hWndOwn) ) )
				break;
			if( bStop )
			{
				_DraggingStop( true );
				break;
			}
			if( !AfxGetThread()->PumpMessage() )
			{
				PostQuitMessage(0);
				break; // Signal WM_QUIT received
			} // if( !AfxGetThread()->PumpMessage() )
		} // while( ::IsWindow(hWndOwn) && PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		if( ! ( ::IsWindow(hWndOwn) ) )
			break;
		if( m_bDragging && CExtControlBar::g_bEnableOnIdleCalls )
		{
			for(	LONG nIdleCounter = 0L;
					::AfxGetThread()->OnIdle(nIdleCounter);
					nIdleCounter ++
					);
		}
	} // while( m_bDragging && ::IsWindow(hWndOwn) )
}

void CExtControlBar::_DraggingStop( bool bCancel )
{
	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	//ASSERT( m_bDragging );
	if( !m_bDragging )
		return;

HWND hWndOwn = GetSafeHwnd();
	ASSERT( hWndOwn != NULL );
	ASSERT( ::IsWindow(hWndOwn) );

	m_bDragging = false;
	m_ptDraggingLast.x = m_ptDraggingLast.y = -1;

bool bEmptyOld = g_DragStateOld.IsEmpty();
bool bOldEqualNew =
		(g_DragStateOld == g_DragStateNew)
			? true : false;
	
//	g_DragStateOld.ExtBarDstSet( NULL );
//HWND hWndDstBar = g_DragStateNew.ExtBarDstGet()->GetSafeHwnd();

	if( !g_DragStateNew.IsEmpty() )
	{
		g_DragStateNew.DrawState();

		_Dragging_OnStop(
			g_DragStateOld,
			g_DragStateNew,
			bCancel
			);
		
		if( !bCancel )
		{
			if( _IsShowContentWhenDragging() )
			{
				if( (!bEmptyOld) && (!bOldEqualNew) )
					_DraggingApplyState( g_DragStateNew );
			} // if( _IsShowContentWhenDragging() )
			else
			{
///					g_DragStateOld.SetEmpty();
					_DraggingApplyState( g_DragStateNew );
			} // else from if( _IsShowContentWhenDragging() )
		} // if( !bCancel )
	}

//	g_DragStateNew.ExtBarDstSet(
//		NULL,
//		( ::IsWindow(hWndDstBar) ) ? false : true
//		);

	g_DragStateOld.ExtBarDstSet( NULL, true );
	g_DragStateNew.ExtBarDstSet( NULL, true );

	g_DragStateOld.SetEmpty();
	g_DragStateNew.SetEmpty();

	if( ::IsWindow(hWndOwn) )
	{
		ASSERT_VALID( this );

		if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
			CExtMouseCaptureSink::ReleaseCapture();

		if(		IsFloating()
			&&	( ! _IsShowContentWhenDragging() )
			)
			RedrawWindow(
				NULL, NULL,
				RDW_INVALIDATE //|RDW_UPDATENOW
					|RDW_ERASE //|RDW_ERASENOW
					|RDW_ALLCHILDREN
				);

		if( bCancel )
			m_pDockSite->DelayRecalcLayout();
	} // if( ::IsWindow(hWndOwn) )

}

CExtControlBar::InternalDraggingState_t
	CExtControlBar::g_DragStateOld;
CExtControlBar::InternalDraggingState_t
	CExtControlBar::g_DragStateNew;

bool CExtControlBar::g_bUpdatingDragState = false;

void CExtControlBar::_DraggingUpdateState(
	const CPoint & point,
	bool bForceFloatMode
	)
{
	if( g_bUpdatingDragState || m_ptDraggingLast == point )
		return;
//	if( !IsFloating() )
//	{
//		bool bHorz = IsDockedHorizontally();
//		if( bHorz
//			&& abs(m_ptDraggingLast.y-point.y) < 3
//			&& m_ptDraggingLast.x == point.x
//			)
//			return;
//		if( abs(m_ptDraggingLast.x-point.x) < 3
//			&& m_ptDraggingLast.y == point.y
//			)
//			return;
//	}
	m_ptDraggingLast = point;
	g_bUpdatingDragState = true;

	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	ASSERT( m_bDragging );
	ASSERT( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() );

	g_DragStateNew.CalcStateDynamic( point, bForceFloatMode );
	if( g_DragStateNew != g_DragStateOld )
	{
		
		bool bShowContent = _IsShowContentWhenDragging();
		bool bEmptyOld = g_DragStateOld.IsEmpty();
		bool bEmptyNew = g_DragStateNew.IsEmpty();

		if( !bEmptyOld )
		{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			if( ! (!bEmptyNew
					&& g_DragStateOld.GetCDT() == __ECDT_TABBED_REINDEX
					&& g_DragStateOld.ExtBarDstGet() == g_DragStateNew.ExtBarDstGet()
					)
				)
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
				g_DragStateOld.DrawState();
//			else
//				bPassPaintMessages = false;
		}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		CFrameWnd * pFrame = g_DragStateOld.ExtBarSrcGet()->m_pDockSite;
		ASSERT_VALID( pFrame );
		POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
		for( ; pos != NULL; )
		{
			CControlBar * pBar = (CControlBar *)
				pFrame->m_listControlBars.GetNext( pos );
			ASSERT_VALID( pBar );
			if( pBar->GetSafeHwnd() == NULL
				|| ( ! ::IsWindow( pBar->GetSafeHwnd() ) )
				|| ( ! pBar->IsVisible() )
				)
				continue;
			CExtDynTabControlBar * pTabbedBar =
				DYNAMIC_DOWNCAST(
					CExtDynTabControlBar,
					pBar
					);
			if( pTabbedBar == NULL )
				continue;
			pTabbedBar->InvalidateSwitcher();
		}

		if( (!bEmptyOld) && (!bEmptyNew) )
		{
			e_calc_dock_type_t eCDT = g_DragStateOld.GetCDT();
			if( (		eCDT == __ECDT_TABBED_UPDATE
					||	eCDT == __ECDT_TABBED_REINDEX
				)
				//&& g_DragStateOld.ExtBarDstGet() != g_DragStateNew.ExtBarDstGet()
				)
			{
				g_DragStateOld.ExtBarDstSet( NULL );
			}
		} // if( (!bEmptyOld) && (!bEmptyNew) )

#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		
		if( !bEmptyNew )
			g_DragStateNew.DrawState();

		if( bShowContent )
		{
			if( !g_DragStateOld.IsEmpty()
				&& g_DragStateOld != g_DragStateNew
				)
				_DraggingApplyState( g_DragStateNew );
		} // if( bShowContent )

		g_DragStateOld = g_DragStateNew;
	
	} // if( g_DragStateNew != g_DragStateOld )

	if( CExtMouseCaptureSink::GetCapture() != GetSafeHwnd() )
		CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
	g_bUpdatingDragState = false;
}

void CExtControlBar::OnNcAreaButtonsReinitialize()
{
INT nCountOfNcButtons = NcButtons_GetCount();
	if( nCountOfNcButtons > 0 )
		return;
	NcButtons_Add( new CExtBarNcAreaButtonClose(this) );
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	NcButtons_Add( new CExtBarNcAreaButtonAutoHide(this) );
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
//	NcButtons_Add( new CExtBarNcAreaButtonExpand(this) );
	NcButtons_Add( new CExtBarNcAreaButtonMenu(this) );
}

void CExtControlBar::OnNcAreaButtonsReposition()
{

INT nCountOfNcButtons = NcButtons_GetCount();
CExtBarNcAreaButton * pPrevBtn = NULL;
	for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
	{
		CExtBarNcAreaButton * pBtn =
			NcButtons_GetAt( nBtn );
		pBtn->OnNcAreaReposition( pPrevBtn );
		if( !pBtn->OnQueryVisibility() )
			continue;
		pPrevBtn = pBtn;
	} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButton

IMPLEMENT_DYNCREATE( CExtBarNcAreaButton, CObject )

CExtBarNcAreaButton::CExtBarNcAreaButton(
	CExtControlBar * pBar
	)
	: m_pBar( pBar )
	, m_rc( 0, 0, 0, 0 )
	, m_bHover( false )
	, m_bPushed( false )
	, m_bDisabled( false )
	, m_bPaintingCombinedArea( false )
{
	ASSERT_VALID( this );
}

CExtBarNcAreaButton::~CExtBarNcAreaButton()
{
	ASSERT_VALID( this );
}

void CExtBarNcAreaButton::OnNcDestroy()
{
	ASSERT_VALID( this );
	delete this;
}

bool CExtBarNcAreaButton::IsBarFixedMode() const
{
	ASSERT_VALID( this );
	return m_pBar->IsFixedMode();
}

bool CExtBarNcAreaButton::IsBarVisible() const
{
	ASSERT_VALID( this );
	return m_pBar->IsVisible() ? true : false;
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
bool CExtBarNcAreaButton::IsBarAutoHideMode(
	bool * p_bDisplayed, // = NULL
	bool * p_bActive // = NULL
	) const
{
	ASSERT_VALID( this );
	if( p_bDisplayed != NULL )
		*p_bDisplayed = false;
	if( p_bActive != NULL )
		*p_bActive = false;
bool bAutohide = m_pBar->AutoHideModeGet();
	if( !bAutohide )
		return false;
	if( p_bDisplayed != NULL || p_bActive != NULL )
	{
		ASSERT_VALID( m_pBar->m_pDockBar );
		ASSERT_KINDOF( CExtDockBar, m_pBar->m_pDockBar );
		CExtDynAutoHideArea * pWndAutoHideArea =
			((CExtDockBar*)m_pBar->m_pDockBar)->_GetAutoHideArea();
		ASSERT_VALID( pWndAutoHideArea );
		if( pWndAutoHideArea->m_pWndSlider != NULL )
		{
			ASSERT_VALID( pWndAutoHideArea->m_pWndSlider );
			if( pWndAutoHideArea->m_pWndSlider->m_pBar == m_pBar )
			{
				if( p_bDisplayed != NULL )
					*p_bDisplayed = true;
				if( p_bActive != NULL )
					*p_bActive = pWndAutoHideArea->m_pWndSlider->m_bWndActive;
			}
		}
	}
	return true;
}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

bool CExtBarNcAreaButton::IsBarFloated() const
{
	ASSERT_VALID( this );
	return m_pBar->IsFloating();
}

bool CExtBarNcAreaButton::IsBarDocked() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDocked();
}

bool CExtBarNcAreaButton::IsBarDockedHorizontally() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedHorizontally();
}

bool CExtBarNcAreaButton::IsBarDockedVertically() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedVertically();
}

bool CExtBarNcAreaButton::IsBarDockedAtLeft() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtLeft();
}

bool CExtBarNcAreaButton::IsBarDockedAtRight() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtRight();
}

bool CExtBarNcAreaButton::IsBarDockedAtTop() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtTop();
}

bool CExtBarNcAreaButton::IsBarDockedAtBottom() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtBottom();
}

bool CExtBarNcAreaButton::GetBarNcAreaInfo( // returns false if no nc area on bar
	LPRECT pRectGripper, //  = NULL // in window coords
	LPRECT pRectWnd, // = NULL // in screen coords
	bool * pbGripperAtTop, //  = NULL
	bool * pbTextOnGripper //  = NULL
	) const
{
	ASSERT_VALID( this );
	if( pRectGripper != NULL )
		::SetRectEmpty( pRectGripper );
	if( pRectWnd != NULL )
		::SetRectEmpty( pRectWnd );
	
	if( IsBarFloated() )
	{
		if( pbGripperAtTop != NULL )
			*pbGripperAtTop = false;
		if( pbTextOnGripper != NULL )
			*pbTextOnGripper = false;
		
		CFrameWnd * pMiniFrame =
			m_pBar->GetDockingFrame();
		if( pRectWnd != NULL )
			pMiniFrame->GetWindowRect( pRectWnd );
		CExtMiniDockFrameWnd * pExtMiniFrame =
			DYNAMIC_DOWNCAST( CExtMiniDockFrameWnd, pMiniFrame );
		if( pExtMiniFrame == NULL )
			return false;
		
		if( pRectGripper != NULL )
		{
			CRect rcGrip( pExtMiniFrame->m_rcFrameCaption );
			rcGrip.InflateRect( 2, 2, 1, 2 );
			*pRectGripper = rcGrip;
		}
		if( pbGripperAtTop != NULL )
			*pbGripperAtTop = true;
		if( pbTextOnGripper != NULL )
			*pbTextOnGripper = true;
		return true;
	} // if( IsBarFloated() )

	if( !m_pBar->IsFixedMode() )
	{
		CFrameWnd * pParentFrame = m_pBar->GetParentFrame();
		if(		pParentFrame != NULL
			&&	pParentFrame->IsKindOf(RUNTIME_CLASS(CExtMiniDockFrameWnd))
			)
		{
			if( m_pBar->_IsSingleVisibleInFloatingPalette() )
			{
				if( pbGripperAtTop != NULL )
					*pbGripperAtTop = false;
				if( pbTextOnGripper != NULL )
					*pbTextOnGripper = false;
				
				if( pRectWnd != NULL )
					pParentFrame->GetWindowRect( pRectWnd );
				CExtMiniDockFrameWnd * pExtMiniFrame =
					STATIC_DOWNCAST( CExtMiniDockFrameWnd, pParentFrame );
				
				if( pRectGripper != NULL )
				{
					CRect rcGrip( pExtMiniFrame->m_rcFrameCaption );
					rcGrip.InflateRect( 2, 2, 1, 2 );
					*pRectGripper = rcGrip;
				}
				if( pbGripperAtTop != NULL )
					*pbGripperAtTop = true;
				if( pbTextOnGripper != NULL )
					*pbTextOnGripper = true;
				return true;
			}
		}
	}


	if( pRectWnd != NULL )
		m_pBar->GetWindowRect( pRectWnd );
bool bGrip =
		m_pBar->IsBarWithGripper(
			pbGripperAtTop,
			pbTextOnGripper
			);
	if( !bGrip )
		return false;
	if( pRectGripper != NULL )
	{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		if( m_pBar->AutoHideModeGet() )
		{
			ASSERT_VALID( m_pBar->m_pDockBar );
			ASSERT_KINDOF( CExtDockBar, m_pBar->m_pDockBar );
			CExtDynAutoHideArea * pAutoHideArea =
				((CExtDockBar*)m_pBar->m_pDockBar)->_GetAutoHideArea();
			ASSERT_VALID( pAutoHideArea );
			ASSERT_VALID( pAutoHideArea->m_pWndSlider );
			pAutoHideArea->m_pWndSlider->
				CalcInnerNcRects(
					NULL,
					NULL,
					pRectGripper
					);
		}
		else
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			*pRectGripper = m_pBar->m_rcGrip;
	}
	return true;
}

bool CExtBarNcAreaButton::IsBarWndActive() const
{
	ASSERT_VALID( this );
	
	if( IsBarFloated() )
	{
		CFrameWnd * pMiniFrame =
			m_pBar->GetDockingFrame();
		CExtMiniDockFrameWnd * pExtMiniFrame =
			DYNAMIC_DOWNCAST( CExtMiniDockFrameWnd, pMiniFrame );
		if( pExtMiniFrame == NULL )
			return false;

		HWND hTmpWndFocus = ::GetFocus();
		if( hTmpWndFocus != NULL
			&& (  pExtMiniFrame->GetSafeHwnd() == hTmpWndFocus
				  || ::IsChild( pExtMiniFrame->GetSafeHwnd(), hTmpWndFocus )
				)
			)
			return true;
		return  false;
	} // if( IsBarFloated() )

	return m_pBar->m_bWindowActive;
}

bool CExtBarNcAreaButton::IsBarSingleOnRow() const
{
	ASSERT_VALID( this );
	return m_pBar->IsSingleOnRow();
}

bool CExtBarNcAreaButton::IsBarMinimizedOnRow() const
{
	ASSERT_VALID( this );
	return m_pBar->IsMinimizedOnRow();
}

bool CExtBarNcAreaButton::IsBarMaximizedOnRow() const
{
	ASSERT_VALID( this );
	return m_pBar->IsMaximizedOnRow();
}

bool CExtBarNcAreaButton::IsBarFirstOnRow( bool bExtBar ) const
{
	ASSERT_VALID( this );
	return m_pBar->_IsFirstBarInRow( bExtBar );
}

bool CExtBarNcAreaButton::IsBarLastOnRow( bool bExtBar ) const
{
	ASSERT_VALID( this );
	return m_pBar->_IsLastBarInRow( bExtBar );
}

CFrameWnd * CExtBarNcAreaButton::GetDockingFrame()
{
	ASSERT_VALID( this );
	return m_pBar->_GetDockingFrameImpl();
}

void CExtBarNcAreaButton::OnNcAreaReposition(
	CExtBarNcAreaButton * pPrevBtn
	)
{
	ASSERT_VALID( this );

	//m_bHover = m_bPushed = false;

bool bGripperAtTop = false;
	if(	!GetBarNcAreaInfo( &m_rc, NULL, &bGripperAtTop ) )
		return;
const int _nGapToBordersH = 2;
const int _nGapToBordersV = 3;
const int _nGapBetweenButtons = 3;
	if( bGripperAtTop )
	{
		m_rc.DeflateRect(
			_nGapToBordersH,
			_nGapToBordersV
			);
		m_rc.left = m_rc.right - m_rc.Height();
		m_rc.OffsetRect( -1, 0 );
		if( pPrevBtn != NULL )
		{
			const CRect & rcPrev = *pPrevBtn;
			m_rc.OffsetRect(
				rcPrev.left - m_rc.right -
					_nGapBetweenButtons,
				0
				);
		} // if( pPrevBtn != NULL )
	} // if( bGripperAtTop )
	else
	{
		m_rc.DeflateRect(
			_nGapToBordersV,
			_nGapToBordersH
			);
		m_rc.bottom = m_rc.top + m_rc.Width();
		m_rc.OffsetRect( 0, 1 );
		if( pPrevBtn != NULL )
		{
			const CRect & rcPrev = *pPrevBtn;
			m_rc.OffsetRect(
				0,
				- rcPrev.top + m_rc.bottom +
					_nGapBetweenButtons
				);
		} // if( pPrevBtn != NULL )
	} // else from if( bGripperAtTop )
}

bool CExtBarNcAreaButton::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !IsBarVisible() )
	{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		bool bDisplayed = false;
		if( IsBarAutoHideMode( &bDisplayed ) )
		{
			if( bDisplayed )
				return true;
		}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		return false;
	}
CRect rcGrip;
	if(	!GetBarNcAreaInfo( &rcGrip ) )
		return false;
	if( rcGrip.IsRectEmpty() )
		return false;
	return true;
}

void CExtBarNcAreaButton::NcDrawDefault(
	CDC & dc,
	int nDockBtnGlyptT
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;
bool bMenuTracking =
		CExtPopupMenuWnd::IsMenuTracking() ? true : false;

//bool bDrawActive = IsBarWndActive();
//	if( !bDrawActive )
//		IsBarAutoHideMode( NULL, &bDrawActive );
bool bDrawActive = false;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( !IsBarAutoHideMode( NULL, &bDrawActive ) )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		bDrawActive = IsBarWndActive();

CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA _pdcbd(
		this,
		m_rc,
		(CExtPaintManager::e_docking_caption_button_t)nDockBtnGlyptT,
		!m_bDisabled && m_bHover && !bMenuTracking && !m_bPaintingCombinedArea,
		!m_bDisabled && m_bPushed && !bMenuTracking && !m_bPaintingCombinedArea,
		!m_bDisabled,
		!IsBarFixedMode() || m_bPaintingCombinedArea,
		bDrawActive,
		IsBarFloated()
		);
	g_PaintManager->PaintDockingCaptionButton( dc, _pdcbd );
}

void CExtBarNcAreaButton::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__)
	dc.FillSolidRect( &m_rc, RGB(255,128,128) );
#else // __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
	dc;
#endif // !__DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
}

UINT CExtBarNcAreaButton::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
	{
		m_bPushed =
			(
				CExtPopupMenuWnd::IsKeyPressed( VK_LBUTTON )
				&& !CExtPopupMenuWnd::IsKeyPressed( VK_MBUTTON )
				&& !CExtPopupMenuWnd::IsKeyPressed( VK_RBUTTON )
			)
			? true : false;
		m_bHover = true;
		return HTNOWHERE;
	}
	m_bHover = m_bPushed = false;
	return HTCLIENT; // not inside nc-button
}

HCURSOR CExtBarNcAreaButton::OnNcAreaQueryCursor( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
		return g_hCursorArrow;
	return NULL; // not inside nc-button
}

bool CExtBarNcAreaButton::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
		return true;
	return false; // continue asking nc-buttons
}

int CExtBarNcAreaButton::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	
	point; // shuld be in button area
	pTI;

	return -1;
}

int CExtBarNcAreaButton::DoDefaultReflectForToolHitTest(
	TOOLINFO * pTI,
	UINT nIdResourceTooltipText,
	__EXT_MFC_SAFE_LPCTSTR sTooltipTextDefault
	)
{
	ASSERT_VALID( this );
	ASSERT( sTooltipTextDefault != NULL );

#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

CExtSafeString sTooltipText;
	if( !sTooltipText.LoadString(nIdResourceTooltipText) )
	{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		sTooltipText = sTooltipTextDefault;
	}
	
	return
		DoDefaultReflectForToolHitTest(
			pTI,
			sTooltipText
			);
}

int CExtBarNcAreaButton::DoDefaultReflectForToolHitTest(
	TOOLINFO * pTI,
	__EXT_MFC_SAFE_LPCTSTR sTooltipText
	)
{
	ASSERT_VALID( this );
	ASSERT( sTooltipText != NULL );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( IsBarAutoHideMode() )
		return false;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	if( pTI != NULL )
	{
		CExtControlBar * pBar = GetBar();
		ASSERT_VALID( pBar );

		CPoint ptDevOffset( 0, 0 );
		if( pBar->IsFloating() )
		{
			CRect rcBarWnd, rcBarClient;
			CFrameWnd * pFrame = pBar->GetParentFrame();
			ASSERT_VALID( pFrame );
			pFrame->GetWindowRect( &rcBarWnd );
			pFrame->GetClientRect( &rcBarClient );
			pFrame->ClientToScreen( &rcBarClient );
			ptDevOffset =
				rcBarClient.TopLeft()
				- rcBarWnd.TopLeft();
/*
			//pTI->uFlags |= TTF_TRACK|TTF_ALWAYSTIP;
			//pFrame->m_nFlags |= WF_TRACKINGTOOLTIPS;
			//pTI->uFlags = TTF_ALWAYSTIP;
			//AfxGetApp()->m_pActiveWnd = pFrame;

			pTI->uFlags |= TTF_TRACK|TTF_ALWAYSTIP|TTF_NOTBUTTON;
			pFrame->m_nFlags |= WF_TRACKINGTOOLTIPS;

			// not implemented yet
			return -1;
*/
		} // if( pBar->IsFloating() )
		else
		{
			CRect rcBarWnd, rcBarClient;
			pBar->GetWindowRect( &rcBarWnd );
			pBar->GetClientRect( &rcBarClient );
			pBar->ClientToScreen( &rcBarClient );
			ptDevOffset =
				rcBarClient.TopLeft()
				- rcBarWnd.TopLeft();
		} // else from if( pBar->IsFloating() )

		CRect rcArea = *this;
		rcArea.OffsetRect( -ptDevOffset );
		
		::CopyRect(
			&(pTI->rect),
			&rcArea
			);

		int nCmdID = (int)pBar->GetDlgCtrlID();
		
		pTI->uId = (UINT)nCmdID;
		pTI->hwnd = pBar->GetSafeHwnd();

		pTI->lpszText = (LPTSTR)
			::calloc(
				(_tcslen(sTooltipText) + 1),
				sizeof(TCHAR)
				);
		if( pTI->lpszText != NULL )
			_tcscpy(
				pTI->lpszText,
				sTooltipText
				);
		else
			pTI->lpszText = LPSTR_TEXTCALLBACK;

		return nCmdID;
	} // if( pTI != NULL )
	
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonExpand

IMPLEMENT_DYNCREATE( CExtBarNcAreaButtonExpand, CExtBarNcAreaButton )

CExtBarNcAreaButtonExpand::CExtBarNcAreaButtonExpand(
	CExtControlBar * pBar
	)
	: CExtBarNcAreaButton( pBar )
{
}

bool CExtBarNcAreaButtonExpand::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !CExtBarNcAreaButton::OnQueryVisibility() )
		return false;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( IsBarAutoHideMode() )
		return false;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( IsBarFixedMode() || IsBarFloated() )
		return false;
	return true;
}

void CExtBarNcAreaButtonExpand::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__)

	dc.FillSolidRect( &m_rc, RGB(128,255,128) );

#else // __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	m_bDisabled = IsBarAutoHideMode();
	if( !m_bDisabled )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		m_bDisabled =
			IsBarSingleOnRow()
			;
bool bMax = IsBarMaximizedOnRow();
bool bHorz = IsBarDockedHorizontally();
	NcDrawDefault(
		dc,
		bHorz
			? ( bMax
					? CExtPaintManager::__DCBT_ARROW_RIGHT
					: CExtPaintManager::__DCBT_ARROW_LEFT
				)
			: ( bMax
					? CExtPaintManager::__DCBT_ARROW_DOWN
					: CExtPaintManager::__DCBT_ARROW_UP
					)
		);

#endif // !__DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
}

UINT CExtBarNcAreaButtonExpand::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
	if( nHT == HTCLIENT )
		return HTCLIENT; // not inside nc-button

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( IsBarAutoHideMode() )
		return HTCLIENT;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( IsBarSingleOnRow() )
		return HTCLIENT;
//	if( IsBarMaximizedOnRow() )
		return HTMINBUTTON;
//	return HTMAXBUTTON;
}

bool CExtBarNcAreaButtonExpand::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( !m_rc.PtInRect(point) )
		return false; // continue asking nc-buttons

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( IsBarAutoHideMode() )
		return false;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	if( IsBarSingleOnRow() )
	{
		if( !IsBarWndActive() )
			m_pBar->SetFocus();
		return true;
	}

	if( IsBarMaximizedOnRow() )
		m_pBar->MinimizeOnRow();
	else
	{
		m_pBar->MaximizeOnRow();
		m_pBar->SetFocus();
	}
	
	return true;
}

int CExtBarNcAreaButtonExpand::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	
	point; // shuld be in button area

bool bBarMaximizedOnRow = IsBarMaximizedOnRow();
	return
		DoDefaultReflectForToolHitTest(
			pTI,
			bBarMaximizedOnRow ? IDS_CBNCAREA_CONTRACT : IDS_CBNCAREA_EXPAND,
			bBarMaximizedOnRow ? _T("Contract") : _T("Expand")
			);
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonClose

IMPLEMENT_DYNCREATE( CExtBarNcAreaButtonClose, CExtBarNcAreaButton )

CExtBarNcAreaButtonClose::CExtBarNcAreaButtonClose(
	CExtControlBar * pBar
	)
	: CExtBarNcAreaButton( pBar )
{
}

bool CExtBarNcAreaButtonClose::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !CExtBarNcAreaButton::OnQueryVisibility() )
		return false;
	if( !IsBarFixedMode()  )
		return true;
	return IsBarFloated();
}

void CExtBarNcAreaButtonClose::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__)

	dc.FillSolidRect( &m_rc, RGB(128,128,255) );

#else // __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__

	NcDrawDefault(
		dc,
		CExtPaintManager::__DCBT_CLOSE
		);

#endif // !__DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
}

UINT CExtBarNcAreaButtonClose::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
	if( nHT == HTCLIENT )
		return HTCLIENT; // not inside nc-button
	//return HTCLOSE;
	return HTMINBUTTON;
}

bool CExtBarNcAreaButtonClose::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
	{

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		if( !m_pBar->IsFixedMode()
			&& m_pBar->AutoHideModeGet()
			)
		{
			m_pBar->AutoHideModeSet( false, false, true, true );
// 2.21 fix
///			return true;
		}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		CFrameWnd * pFrame = GetDockingFrame();
// 2.21 fix
///		ASSERT( IsBarVisible() );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		if( m_pBar->IsFloating() )
		{
			pFrame->ShowControlBar( m_pBar, FALSE, FALSE );
			return true;
		}
		CExtDynTabControlBar * pTabbedBar =
			DYNAMIC_DOWNCAST(
				CExtDynTabControlBar,
				m_pBar
				);
		if( pTabbedBar != NULL )
		{
			ASSERT( pTabbedBar->GetSwitcherItemCount() > 0 );
			LONG nCurrent = pTabbedBar->GetSwitcherSelection();
			if( nCurrent >= 0 )
			{
				ASSERT( nCurrent < pTabbedBar->GetSwitcherItemCount() );
				CExtControlBar * pBar =
					pTabbedBar->GetBarAt( nCurrent, true );
				ASSERT_VALID( pBar );
				pFrame->ShowControlBar( pBar, FALSE, FALSE );
				return true;
			} // if( nCurrent >= 0 )
		} // if( pTabbedBar != NULL )
		pFrame->ShowControlBar( m_pBar, FALSE, FALSE );
		return true;

#else // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		pFrame->ShowControlBar( m_pBar, FALSE, FALSE );
		return true;

#endif // else from (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	}
	return false; // continue asking nc-buttons
}

int CExtBarNcAreaButtonClose::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	
	point; // shuld be in button area
	return
		DoDefaultReflectForToolHitTest(
			pTI,
			IDS_CBNCAREA_CLOSE,
			_T("Hide")
			);
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonAutoHide

IMPLEMENT_DYNCREATE( CExtBarNcAreaButtonAutoHide, CExtBarNcAreaButton )

CExtBarNcAreaButtonAutoHide::CExtBarNcAreaButtonAutoHide(
	CExtControlBar * pBar
	)
	: CExtBarNcAreaButton( pBar )
{
}

bool CExtBarNcAreaButtonAutoHide::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !CExtBarNcAreaButton::OnQueryVisibility() )
		return false;
	if( IsBarFixedMode()  )
		return false;
	if( IsBarFloated() )
		return false;
const CExtControlBar * pExtBar = GetBar();
	ASSERT( pExtBar );
	if( pExtBar->m_pDockBar == NULL )
		return false;
CExtDockBar * pDockBar =
		DYNAMIC_DOWNCAST( CExtDockBar, pExtBar->m_pDockBar );
	if( pDockBar == NULL )
		return false;
	if( pDockBar->_GetAutoHideArea() == NULL )
		return false;
CFrameWnd * pFrame = pExtBar->GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		return false;
	return true;
}

void CExtBarNcAreaButtonAutoHide::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__)

	dc.FillSolidRect( &m_rc, RGB(255,128,255) );

#else // __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__

	NcDrawDefault(
		dc,
		IsBarAutoHideMode()
			? CExtPaintManager::__DCBT_WND_UNKEEP
			: CExtPaintManager::__DCBT_WND_KEEP
		);

#endif // !__DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
}

UINT CExtBarNcAreaButtonAutoHide::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
	if( nHT == HTCLIENT )
		return HTCLIENT; // not inside nc-button
	//return HTCLOSE;
	return HTMINBUTTON;
}

bool CExtBarNcAreaButtonAutoHide::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
	{
		m_bPushed = m_bHover = false;
		CExtControlBar * pExtBar = GetBar();
		ASSERT_VALID( pExtBar );
		bool bAutoHideMode = pExtBar->AutoHideModeGet();
		pExtBar->AutoHideModeSet( !bAutoHideMode, false, true, false );
		return true;
	}
	return false; // continue asking nc-buttons
}

void CExtBarNcAreaButtonAutoHide::OnNcAreaReposition(
	CExtBarNcAreaButton * pPrevBtn
	)
{
	ASSERT_VALID( this );
CExtControlBar * pExtBar = GetBar();
	ASSERT_VALID( pExtBar );
bool bAutoHideMode = pExtBar->AutoHideModeGet();
	if( !bAutoHideMode )
		m_bHover = m_bPushed = false;
	CExtBarNcAreaButton::OnNcAreaReposition( pPrevBtn );
}

int CExtBarNcAreaButtonAutoHide::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	
	point; // shuld be in button area
	return
		DoDefaultReflectForToolHitTest(
			pTI,
			IDS_EXTTABWND_BTN_AUTOHIDE,
			_T("Auto Hide")
			);
}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonMenu

IMPLEMENT_DYNCREATE( CExtBarNcAreaButtonMenu, CExtBarNcAreaButton )

CExtBarNcAreaButtonMenu::CExtBarNcAreaButtonMenu(
	CExtControlBar * pBar
	)
	: CExtBarNcAreaButton( pBar )
{
}

bool CExtBarNcAreaButtonMenu::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !CExtBarNcAreaButton::OnQueryVisibility() )
		return false;
	if( !IsBarFixedMode() || !IsBarFloated() )
		return false;
	return true;
}

void CExtBarNcAreaButtonMenu::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( GetBar()->GetSafeHwnd() );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		m_bHover = m_bPushed = false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#if (defined __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__)

	dc.FillSolidRect( &m_rc, RGB(255,255,128) );

#else // __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__

	NcDrawDefault(
		dc,
		CExtPaintManager::__DCBT_ARROW_DOWN
		);

#endif // !__DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__
}

UINT CExtBarNcAreaButtonMenu::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( GetBar()->GetSafeHwnd() );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		return HTCLIENT;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
	if( nHT == HTCLIENT )
		return HTCLIENT; // not inside nc-button

	return HTMINBUTTON;
}

bool CExtBarNcAreaButtonMenu::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( GetBar()->GetSafeHwnd() );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		return true;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if( CExtControlBar::FindHelpMode( GetBar() ) )
		return true;

	if( !m_rc.PtInRect(point) )
		return false; // continue asking nc-buttons

CFrameWnd * pFrame = GetDockingFrame();
	ASSERT_VALID( pFrame );

HWND hWndTrack = pFrame->GetSafeHwnd();
	ASSERT(
		hWndTrack != NULL
		&& ::IsWindow(hWndTrack)
		);

CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	if( !pPopup->CreatePopupMenu(hWndTrack) )
	{
		ASSERT( FALSE );
		return false;
	}

CExtControlBar::POPUP_MENU_EVENT_DATA _pmed(
		CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_CONTROLBAR_NCBTNMENU_TOP,
		pPopup,
		m_pBar,
		CPoint( -1, -1 ),
		WM_NCLBUTTONDOWN,
		this
		);
	if( !_pmed.NotifyTarget( false ) )
	{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

		CExtSafeString sShowHidePanels;
		if( !sShowHidePanels.LoadString(IDS_SHOW_HIDE_PANELS) )
		{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			sShowHidePanels = _T("&Show/hide panels");
		}
		VERIFY(
			pPopup->ItemInsert(
				(UINT)CExtPopupMenuWnd::TYPE_POPUP,
				-1,
				sShowHidePanels
				)
			);
		CExtPopupMenuWnd::MENUITEMDATA & mi = pPopup->ItemGetInfo( pPopup->ItemGetCount() - 1 );
		mi.SetChildCombine();
		mi.SetToolButton();
		CExtPopupMenuWnd * pPopupBars = mi.GetPopup();
		ASSERT( pPopupBars != NULL );

		if(	! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
				pFrame,
				pPopupBars,
				CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_CONTROLBAR_NCBTNMENU_BARS,
				m_pBar,
				WM_NCLBUTTONDOWN,
				this
				)
			)
		{
			VERIFY(
				pPopup->ItemRemove(
					pPopup->ItemGetCount() - 1
					)
				);
		}
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		if(		m_pBar->IsKindOf( RUNTIME_CLASS(CExtToolControlBar) )
			&&	((CExtToolControlBar*)m_pBar)->m_bCustomizationAllowed
			&&	((CExtToolControlBar*)m_pBar)->GetCustomizeSite() != NULL
			)
		{ // if customization allowed
			VERIFY(
				CExtDockBar::_AppendAddRemoveButtonsCustomizeMenu(
					pPopup,
					m_pBar
					)
				);
		} // if customization allowed
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	} // if( !_pmed.NotifyTarget( false ) )
	_pmed.NotifyTarget( true );

	if( pPopup->ItemGetCount() == 0 )
	{
		delete pPopup;
		return false;
	}

CRect rcBarWnd;
	VERIFY(
		GetBarNcAreaInfo( NULL, &rcBarWnd )
		);
CRect rcBtnScreen( m_rc );
	rcBtnScreen.OffsetRect( rcBarWnd.TopLeft() );
	rcBtnScreen.InflateRect( 1, 1 );

CPoint ptCursor = rcBtnScreen.CenterPoint();
	::SetFocus( hWndTrack );
	if(	! pPopup->TrackPopupMenu(
			TPMX_TOPALIGN|TPMX_COMBINE_DEFAULT|TPMX_OWNERDRAW_FIXED,
			ptCursor.x,  ptCursor.y,
			&rcBtnScreen,
			this,
			_CbPaintCombinedContent,
			NULL,
			true
			)
		)
	{
		delete pPopup;
		return false;
	}
	
	return true;
}

int CExtBarNcAreaButtonMenu::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	
	point; // shuld be in button area
	return
		DoDefaultReflectForToolHitTest(
			pTI,
			IDS_CBNCAREA_MENU,
			_T("Options")
			);
}

void CExtBarNcAreaButtonMenu::_CbPaintCombinedContent(
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

CExtBarNcAreaButtonMenu * pBtn = (CExtBarNcAreaButtonMenu *)pCookie;
	ASSERT( pBtn != NULL );
	ASSERT_VALID( pBtn );
	ASSERT( pBtn->IsKindOf(RUNTIME_CLASS(CExtBarNcAreaButtonMenu)) );

	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );

	if( rcExcludeArea.IsRectEmpty() )
		return;

CRect rcBarWnd;
	VERIFY(
		pBtn->GetBarNcAreaInfo( NULL, &rcBarWnd )
		);
CRect rcExcludeAreaX;
	refWndMenu.GetClientRect( &rcExcludeAreaX );
	refWndMenu.ClientToScreen( &rcExcludeAreaX );
CPoint ptOffset =
		rcBarWnd.TopLeft()
		- rcExcludeAreaX.TopLeft()
		;
CPoint ptViewportOrg = dc.GetViewportOrg();
	dc.SetViewportOrg( ptOffset );
	pBtn->m_bPaintingCombinedArea = true;
	pBtn->OnNcAreaDraw( dc );
	pBtn->m_bPaintingCombinedArea = false;
	dc.SetViewportOrg( ptViewportOrg );
}

LRESULT CExtControlBar::OnSizeParent(WPARAM wParam, LPARAM lParam)
{
	if( !m_bReflectParentSizing )
		return (LRESULT)0;

//	if( CExtControlBar::g_bUpdatingDragState )
//		return 0;

LRESULT lResult =
		CControlBar::OnSizeParent(wParam,lParam);
	return lResult;
}

CExtControlBar * CExtControlBar::InternalDraggingState_t::ExtBarDstSet(
	CExtControlBar * pExtBarDst, // = NULL
	bool bNoCheckOnShutdown // = false
	)
{
	bNoCheckOnShutdown;
	ASSERT( this != NULL );
//	ASSERT_VALID( this );
CExtControlBar * pExtBarDstOld = m_pExtBarDst;
	m_pExtBarDst = pExtBarDst;
//	ASSERT_VALID( this );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	if(	(!bNoCheckOnShutdown)
		&& pExtBarDstOld != NULL
		&& pExtBarDstOld != m_pExtBarDst )
	{
		CExtDynTabControlBar * pTabbedBar =
			DYNAMIC_DOWNCAST(
				CExtDynTabControlBar,
				pExtBarDstOld
				);
		if( pTabbedBar != NULL )
			pTabbedBar->RemoveTemporaryItems( true );
	} // if( pExtBarDstOld != NULL && pExtBarDstOld != m_pExtBarDst )

	m_nTabReindex = 0;

#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	return pExtBarDstOld;
}

void CExtControlBar::InternalDraggingState_t::_AssignFromOther(
	const CExtControlBar::InternalDraggingState_t & other
	)
{
	ASSERT( this != NULL );
	ASSERT_VALID( (&other) );

	if( m_pExtBarDst != NULL
		&& m_pExtBarDst != other.m_pExtBarDst
		)
	{
		ExtBarDstSet( other.m_pExtBarDst );
	} // if( m_pExtBarDst ....

	m_bEmpty = other.m_bEmpty;
	m_bDeepHalfSplit = other.m_bDeepHalfSplit;
	m_eCDT = other.m_eCDT;
	m_eCFR = other.m_eCFR;
	m_nCircleNo = other.m_nCircleNo;
	m_rcTrack = other.m_rcTrack;
	m_pExtBarSrc = other.m_pExtBarSrc;
	m_pExtBarDst = other.m_pExtBarDst;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	m_nTabReindex = other.m_nTabReindex;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	ASSERT_VALID( this );
}

void CExtControlBar::InternalDraggingState_t::SetEmpty(
	bool bEmpty // = true
	)
{
//	ASSERT_VALID( this );
	ASSERT( this != NULL );

	m_eCFR = __ECFR_NO_RESIZING;
	if( bEmpty )
		ExtBarDstSet( NULL );
	
	m_bEmpty = bEmpty;
	if( m_bEmpty )
	{
		m_rcTrack.SetRectEmpty();
//		m_pExtBarDst = NULL;
	}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	m_nTabReindex = 0;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
void CExtControlBar::InternalDraggingState_t::ReindexPosCalc( CPoint ptCursor )
{
	ASSERT_VALID( this );
	ASSERT( !IsEmpty() );
	ASSERT( m_eCDT == __ECDT_TABBED_REINDEX );
	ASSERT_VALID( m_pExtBarDst );
CExtDynTabControlBar * pTabbedBar =
		STATIC_DOWNCAST(
			CExtDynTabControlBar,
			m_pExtBarDst
			);
	m_nTabReindex = pTabbedBar->ReindexPosCalc( ptCursor );
}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

void CExtControlBar::InternalDraggingState_t::DrawState()
{
	ASSERT_VALID( this );
	ASSERT( !IsEmpty() );
	ASSERT_VALID( m_pExtBarSrc );
	ASSERT_VALID( m_pExtBarSrc->m_pDockSite );
	ASSERT_VALID( m_pExtBarSrc->m_pDockBar );

	if( m_eCDT == __ECDT_OWN_AREA )
		return;

	if( m_pExtBarSrc->IsFixedMode() )
		return;
	
	if( m_pExtBarSrc->_IsShowContentWhenDragging() )
		return;

//	if( m_eCDT == __ECDT_OWN_AREA )
//		return;

	if( m_rcTrack.IsRectEmpty() )
		return;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( m_eCDT == __ECDT_TABBED_UPDATE
		|| m_eCDT == __ECDT_TABBED_REINDEX
		)
	{
		ASSERT_VALID( m_pExtBarDst );
		ASSERT_VALID( m_pExtBarDst->m_pDockSite );
		ASSERT_VALID( m_pExtBarDst->m_pDockBar );
		ASSERT( !m_pExtBarDst->IsFixedMode() );

		ASSERT_KINDOF( CExtControlBar, m_pExtBarSrc );
		ASSERT_KINDOF( CExtDynTabControlBar, m_pExtBarDst );

		if( m_eCDT == __ECDT_TABBED_REINDEX )
			((CExtDynTabControlBar*)m_pExtBarDst) ->
				RemoveTemporaryItems( false );
		((CExtDynTabControlBar*)m_pExtBarDst) ->
			InsertTemporaryItemsFrom(
				m_pExtBarSrc,
				ReindexPosGet(),
				true
				);
		return;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

CRect rcTrackerOuter( m_rcTrack ), rcDockingFrame, rcDockBarWnd;
	m_pExtBarSrc->m_pDockSite->GetWindowRect( &rcDockingFrame );

	if( m_pExtBarDst != NULL )
	{
		ASSERT_VALID( m_pExtBarDst );
		ASSERT_VALID( m_pExtBarDst->m_pDockSite );
		ASSERT_VALID( m_pExtBarDst->m_pDockBar );
		ASSERT( !m_pExtBarDst->IsFixedMode() );

		m_pExtBarDst->m_pDockBar->GetWindowRect( &rcDockBarWnd );
		if( rcDockBarWnd.IsRectEmpty() )
			return;
		if( rcTrackerOuter.left < rcDockBarWnd.left )
			rcTrackerOuter.left = rcDockBarWnd.left;
		if( rcTrackerOuter.top < rcDockBarWnd.top )
			rcTrackerOuter.top = rcDockBarWnd.top;
		if( rcTrackerOuter.right > rcDockBarWnd.right )
			rcTrackerOuter.right = rcDockBarWnd.right;
		if( rcTrackerOuter.bottom > rcDockBarWnd.bottom )
			rcTrackerOuter.bottom = rcDockBarWnd.bottom;

		if( rcTrackerOuter.IsRectEmpty() )
			return;
	} // if( m_pExtBarDst != NULL )

	rcTrackerOuter.OffsetRect( -rcDockingFrame.TopLeft() );

INT nTrackerBorderH = ::GetSystemMetrics( SM_CXSIZEFRAME );
INT nTrackerMinWidth = nTrackerBorderH*2+1;
	if( rcTrackerOuter.Width() < nTrackerMinWidth )
		return;
INT nTrackerBorderV = ::GetSystemMetrics( SM_CYSIZEFRAME );
INT nTrackerMinHeight = nTrackerBorderV*2+1;
	if( rcTrackerOuter.Height() < nTrackerMinHeight )
		return;

CWnd * pWndDrawTracker = NULL;
	if( m_pExtBarDst != NULL
		|| m_eCDT == __ECDT_DOCKBAR_INNER_TOP
		|| m_eCDT == __ECDT_DOCKBAR_INNER_BOTTOM
		|| m_eCDT == __ECDT_DOCKBAR_INNER_LEFT
		|| m_eCDT == __ECDT_DOCKBAR_INNER_RIGHT
		|| m_eCDT == __ECDT_CIRCLE1_OUTER_TOP
		|| m_eCDT == __ECDT_CIRCLE1_OUTER_BOTTOM
		|| m_eCDT == __ECDT_CIRCLE1_OUTER_LEFT
		|| m_eCDT == __ECDT_CIRCLE1_OUTER_RIGHT
		)
	{
		if( m_pExtBarDst != NULL )
		{
			pWndDrawTracker = m_pExtBarDst->GetDockingFrame();
			if( m_pExtBarSrc->m_pDockSite != pWndDrawTracker )
			{
				m_pExtBarSrc->m_pDockSite->ClientToScreen( &rcTrackerOuter );
				pWndDrawTracker->ScreenToClient( &rcTrackerOuter );
				rcTrackerOuter.OffsetRect(-1,-1);
			}
		}
		else
			pWndDrawTracker = m_pExtBarSrc->m_pDockSite;
	} // if( m_pExtBarDst != NULL )
	else
		pWndDrawTracker = CWnd::GetDesktopWindow();
	ASSERT( pWndDrawTracker != NULL );
	ASSERT( pWndDrawTracker->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( pWndDrawTracker->GetSafeHwnd() ) );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
bool bTabShape =
		(m_eCDT == __ECDT_TABBED_NEW)
			? true : false;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

CDC * pDcDrawTracker =
		pWndDrawTracker->GetDCEx( NULL, __DOCKSITE_DCX_FLAGS );
	ASSERT_VALID( pDcDrawTracker );
	ASSERT( pDcDrawTracker->GetSafeHdc() != NULL );
	if( pDcDrawTracker->RectVisible(&rcTrackerOuter) )
	{
		CRect rcTrackerInner( rcTrackerOuter );
		rcTrackerInner.DeflateRect(
			nTrackerBorderH,
			nTrackerBorderV
			);

		bool bRgnCreated = false;
		CRgn
			rgnPaint,
			rgnInner, rgnOuter,
			rgnTabMainArea,
			rgnTabBottomLeftArea,
			rgnTabBottomMiddleArea,
			rgnTabBottomRightArea;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		if( bTabShape )
		{
			const int nMaxTabHeight = 18;
			const int nTabBottomLeftAreaWidth = 8;
			const int nTabBottomMiddleAreaWidth = 36;
			int nTabHeight = nMaxTabHeight;
			int nTrackerHeight = rcTrackerOuter.Height();
			if( nTrackerHeight < (nMaxTabHeight * 2) )
				nTabHeight = nTrackerHeight / 2;
			//if( nTabHeight >= (nTrackerBorderV * 3 + 3) )
			//{
				CRect rcTabMainArea( rcTrackerInner );
				rcTabMainArea.bottom -= nTabHeight;
				ASSERT( rcTabMainArea.bottom > rcTabMainArea.top );
				
				CRect rcTabBottomLeftArea( rcTrackerOuter );
				rcTabBottomLeftArea.right =
					rcTabBottomLeftArea.left + nTabBottomLeftAreaWidth;
				rcTabBottomLeftArea.top =
					rcTabBottomLeftArea.bottom - nTabHeight;

				CRect rcTabBottomMiddleArea( rcTabBottomLeftArea );
				rcTabBottomMiddleArea.OffsetRect(
					nTrackerBorderH + rcTabBottomLeftArea.Width(),
					-nTrackerBorderV
					);
				rcTabBottomMiddleArea.right =
					rcTabBottomMiddleArea.left + nTabBottomMiddleAreaWidth;
				if( rcTabBottomMiddleArea.right > rcTrackerInner.right )
					rcTabBottomMiddleArea.right = 
						rcTabBottomMiddleArea.left
						+ (rcTrackerInner.right - rcTabBottomMiddleArea.left) / 2
						;
				
				CRect rcTabBottomRightArea( rcTabBottomLeftArea );
				rcTabBottomRightArea.right = rcTrackerOuter.right;
				rcTabBottomRightArea.left =
					rcTabBottomMiddleArea.right
					+ nTrackerBorderH;

				if(		rgnOuter.CreateRectRgnIndirect(
							&rcTrackerOuter
							)
					&&	rgnTabMainArea.CreateRectRgnIndirect(
							&rcTabMainArea
							)
					&&	rgnTabBottomLeftArea.CreateRectRgnIndirect(
							&rcTabBottomLeftArea
							)
					&&	rgnTabBottomMiddleArea.CreateRectRgnIndirect(
							&rcTabBottomMiddleArea
							)
					&&	rgnTabBottomRightArea.CreateRectRgnIndirect(
							&rcTabBottomRightArea
							)
					&&	rgnPaint.CreateRectRgnIndirect(
							&rcTrackerOuter
							)
					)
				{ // if regions created successfully
					int nRgnPaintCombineResult = 
						rgnTabMainArea.CombineRgn(
							&rgnTabMainArea,
							&rgnTabBottomLeftArea,
							RGN_OR
							);
					if( nRgnPaintCombineResult != ERROR
						&& nRgnPaintCombineResult != NULLREGION
						)
					{
						nRgnPaintCombineResult = 
							rgnTabMainArea.CombineRgn(
								&rgnTabMainArea,
								&rgnTabBottomMiddleArea,
								RGN_OR
								);
						if( nRgnPaintCombineResult != ERROR
							&& nRgnPaintCombineResult != NULLREGION
							)
						{
							nRgnPaintCombineResult = 
								rgnTabMainArea.CombineRgn(
									&rgnTabMainArea,
									&rgnTabBottomRightArea,
									RGN_OR
									);
							if( nRgnPaintCombineResult != ERROR
								&& nRgnPaintCombineResult != NULLREGION
								)
							{
								nRgnPaintCombineResult = 
									rgnPaint.CombineRgn(
										&rgnPaint,
										&rgnTabMainArea,
										RGN_DIFF
										);
								if( nRgnPaintCombineResult != ERROR
									&& nRgnPaintCombineResult != NULLREGION
									)
								{ // if paint region created successfully & non-empty
									bRgnCreated = true;
								} // if paint region created successfully & non-empty
							}
						}
					}
				} // if regions created successfully
			//} // if( nTabHeight >= (nTrackerBorderV * 3  + 3) )
		} // if( bTabShape )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		
		if( !bRgnCreated )
		{ // create non-tabbed shape
			if(		rgnOuter.CreateRectRgnIndirect(
						&rcTrackerOuter
						)
				&&	rgnInner.CreateRectRgnIndirect(
						&rcTrackerInner
						)
				&&	rgnPaint.CreateRectRgn(0,0,0,0)
				)
			{ // if regions created successfully
				int nRgnPaintCombineResult =
					rgnPaint.CombineRgn(
						&rgnOuter,
						&rgnInner,
						RGN_DIFF
						);
				if( nRgnPaintCombineResult != ERROR
					&& nRgnPaintCombineResult != NULLREGION
					)
				{ // if paint region created successfully & non-empty
					bRgnCreated = true;
				} // if paint region created successfully & non-empty
			} // if regions created successfully
		} // create non-tabbed shape

		if( bRgnCreated )
		{ // if we have sense to paint pattern
				pDcDrawTracker->SelectClipRgn( &rgnPaint );
				
				CBrush * pBrushOld =
					pDcDrawTracker->SelectObject(
						CDC::GetHalftoneBrush()
						);
				pDcDrawTracker->PatBlt(
					rcTrackerOuter.left,
					rcTrackerOuter.top,
					rcTrackerOuter.Width(),
					rcTrackerOuter.Height(),
					PATINVERT
					);
				pDcDrawTracker->SelectObject(
					pBrushOld
					);

				pDcDrawTracker->SelectClipRgn( NULL );
		} // if we have sense to paint pattern

	} // if( pDcDrawTracker->RectVisible(&rcTrackerOuter) )

    pWndDrawTracker->ReleaseDC( pDcDrawTracker );
}

CExtControlBar::OuterItemData_t::OuterItemData_t()
	: m_nBarID( AFX_IDW_DOCKBAR_FLOAT )
	, m_ptFloatHelper( 0, 0 )
	, m_sizeDockedH( 200, 200 )
	, m_sizeDockedV( 200, 200 )
	, m_sizeFloated( 200, 200 )
	, m_bFloating( false )
{
	ASSERT( m_AffixmentData.IsEmpty() );
}

CExtControlBar::OuterItemData_t::OuterItemData_t(
	const CExtControlBar::OuterItemData_t & other
	)
{
	m_nBarID = other.m_nBarID;
	m_ptFloatHelper = other.m_ptFloatHelper;
	m_sizeDockedH = other.m_sizeDockedH;
	m_sizeDockedV = other.m_sizeDockedV;
	m_sizeFloated = other.m_sizeFloated;
	m_AffixmentData = other.m_AffixmentData;
	m_bFloating = other.m_bFloating;
}

CExtControlBar::OuterItemData_t &
	CExtControlBar::OuterItemData_t::operator=(
		const CExtControlBar::OuterItemData_t & other
	)
{
	m_nBarID = other.m_nBarID;
	m_ptFloatHelper = other.m_ptFloatHelper;
	m_sizeDockedH = other.m_sizeDockedH;
	m_sizeDockedV = other.m_sizeDockedV;
	m_sizeFloated = other.m_sizeFloated;
	m_AffixmentData = other.m_AffixmentData;
	m_bFloating = other.m_bFloating;
	return (*this);
}

void CExtControlBar::OuterItemData_t::Serialize( CArchive & ar )
{
	ASSERT( sizeof(m_nBarID) == (sizeof(BYTE)*4) );
SHORT nHelperFlags = 0x0000;
	if( ar.IsStoring() )
	{
		if( m_bFloating )
			nHelperFlags |= 0x0001;
		ar.Write( &nHelperFlags, sizeof(nHelperFlags) );
		ar << m_nBarID;
		ar << m_ptFloatHelper;
		ar << m_sizeDockedH;
		ar << m_sizeDockedV;
		ar << m_sizeFloated;
	} // if( ar.IsStoring() )
	else
	{
		m_nBarID = 0;
		m_ptFloatHelper.x = m_ptFloatHelper.y = 0;
		m_sizeDockedH.cx = m_sizeDockedH.cy
			= m_sizeDockedV.cx = m_sizeDockedV.cy
			= m_sizeFloated.cx = m_sizeFloated.cy
			= 0;
		m_AffixmentData.Empty();
		ar.Read( &nHelperFlags, sizeof(nHelperFlags) );
		m_bFloating = false;
		if( (nHelperFlags & 0x0001) != 0 )
			m_bFloating = true;
		ar >> m_nBarID;
		ar >> m_ptFloatHelper;
		ar >> m_sizeDockedH;
		ar >> m_sizeDockedV;
		ar >> m_sizeFloated;
	} // else from if( ar.IsStoring() )
	m_AffixmentData.Serialize( ar );
}

CExtControlBar::InternalBarRepositionData_t::InternalBarRepositionData_t(
	CControlBar * pBar, // = NULL
	bool bSetAdvData // = false
	)
	: m_pBar( NULL )
	, m_rcSrc( 0, 0, 0, 0 )
	, m_rcDst( 0, 0, 0, 0 )
	, m_nSpaceAvailByBar( 0 )
	, m_nSpaceAvailBySideBefore( 0 )
	, m_nSpaceAvailBySideAfter( 0 )
{
	if( pBar != NULL )
		SetBar( pBar, bSetAdvData );
}

CExtControlBar::InternalBarRepositionData_t::InternalBarRepositionData_t(
	const InternalBarRepositionData_t & other
	)
{
	_AssignFromOther( other );
}

void CExtControlBar::InternalBarRepositionData_t::_AssignFromOther(
	const CExtControlBar::InternalBarRepositionData_t & other
	)
{
	m_pBar = other.m_pBar;
	m_rcSrc = other.m_rcSrc;
	m_rcDst = other.m_rcDst;
	m_nSpaceAvailByBar = other.m_nSpaceAvailByBar;
	m_nSpaceAvailBySideBefore = other.m_nSpaceAvailBySideBefore;
	m_nSpaceAvailBySideAfter = other.m_nSpaceAvailBySideAfter;
}

void CExtControlBar::InternalBarRepositionData_t::SetBar(
	CControlBar * pBar,
	bool bSetAdvData // = false
	)
{
	ASSERT_VALID( pBar );
	m_pBar = pBar;
	m_pBar->GetWindowRect( m_rcSrc );
	m_rcDst = m_rcSrc;
	m_nSpaceAvailByBar = 0;
	m_nSpaceAvailBySideBefore = 0;
	m_nSpaceAvailBySideAfter = 0;

	if( !bSetAdvData )
		return;

CExtControlBar * pExtBar = GetExtBarFixed();
	if( pExtBar == NULL )
		return;
	if( pExtBar->IsFloating() )
		return;
bool bHorz = pExtBar->IsDockedHorizontally();

CRect rcClient;
	pExtBar->GetClientRect( &rcClient );
	m_nSpaceAvailByBar = bHorz
		? m_rcSrc.Width() - rcClient.Width()
		: m_rcSrc.Height() - rcClient.Height();
	ASSERT( m_nSpaceAvailByBar > 0 );

//	pExtBar->ClientToScreen( &rcClient );
//CRect rcResetFixedSize(
//		m_rcSrc.TopLeft(),
//		bHorz ? pExtBar->m_sizeDockedH : pExtBar->m_sizeDockedV
//		);
//	rcResetFixedSize.left += rcClient.left - m_rcSrc.left;
//	rcResetFixedSize.top += rcClient.top - m_rcSrc.top;
//	rcResetFixedSize.right += m_rcSrc.right - rcClient.right;
//	rcResetFixedSize.bottom += m_rcSrc.bottom - rcClient.bottom;
//	m_rcSrc = rcResetFixedSize;

	ASSERT_VALID( pExtBar->m_pDockBar );
	ASSERT( pExtBar->m_pDockBar->IsWindowVisible() );
CRect wrDockBar;
	pExtBar->m_pDockBar->GetWindowRect( &wrDockBar );

CControlBar * pBarPrev =
		pExtBar->_GetNextControlBarInRow( pExtBar, false );
	while( !pBarPrev->IsVisible() )
	{
		CControlBar * pLastBar2 =
			_GetNextControlBarInRow( pBarPrev, false );
		if( pLastBar2 == NULL )
			break;
		pBarPrev = pLastBar2;
	}
	if( pBarPrev != NULL )
	{
		CRect wrPrevBar;
		pBarPrev->GetWindowRect( &wrPrevBar );
		INT m_nSpaceAvailBySideBefore = bHorz
			? m_rcSrc.left - wrPrevBar.right
			: m_rcSrc.top - wrPrevBar.bottom;
		ASSERT( m_nSpaceAvailBySideBefore >= 0 );
	} // if( pBarPrev != NULL )
	else
	{
		INT m_nSpaceAvailBySideBefore = bHorz
			? m_rcSrc.left - wrDockBar.left
			: m_rcSrc.top - wrDockBar.top;
		ASSERT( m_nSpaceAvailBySideBefore >= 0 );
	} // else from if( pBarPrev != NULL )

CControlBar * pBarNext =
		pExtBar->_GetNextControlBarInRow( pExtBar, true );
	while( !pBarNext->IsVisible() )
	{
		CControlBar * pLastBar2 =
			_GetNextControlBarInRow( pBarNext, true );
		if( pLastBar2 == NULL )
			break;
		pBarNext = pLastBar2;
	}
	if( pBarNext != NULL )
	{
		CRect wrNextBar;
		pBarNext->GetWindowRect( &wrNextBar );
		INT m_nSpaceAvailBySideAfter = bHorz
			? m_rcSrc.left - wrNextBar.right
			: m_rcSrc.top - wrNextBar.bottom;
		ASSERT( m_nSpaceAvailBySideAfter >= 0 );
	} // if( pBarNext != NULL )
	else
	{
		INT m_nSpaceAvailBySideBefore = bHorz
			? wrDockBar.right - m_rcSrc.right
			: wrDockBar.bottom - m_rcSrc.bottom;
		ASSERT( m_nSpaceAvailBySideBefore >= 0 );
	} // else from if( pBarNext != NULL )
}

CExtControlBar * CExtControlBar::InternalBarRepositionData_t::GetExtBar()
{
	if( m_pBar == NULL )
		return NULL;
CExtControlBar * pExtBar =
	DYNAMIC_DOWNCAST( CExtControlBar, m_pBar );
	return pExtBar;
}

CExtControlBar * CExtControlBar::InternalBarRepositionData_t::GetExtBarFixed()
{
CExtControlBar * pExtBar = GetExtBar();
	if( pExtBar == NULL )
		return NULL;
	if( pExtBar->IsFixedMode() )
		return NULL;
	return pExtBar;
}

void CExtControlBar::InternalDraggingState_t::_DoFixed_FloatingAdjustment(
	CPoint ptTrack // in screen coords
	)
{
	ASSERT_VALID( m_pExtBarSrc );

	g_DragStateOld.SetCFR( __ECFR_NO_RESIZING );

INT nCyGripper =
	min(
		m_pExtBarSrc->m_rcGrip.Width(),
		m_pExtBarSrc->m_rcGrip.Height()
		);
CSize sizeOffset( nCyGripper, nCyGripper/2 );

bool bAlreadyFloat = m_pExtBarSrc->IsFloating();
	if( !bAlreadyFloat )
	{
		m_pExtBarSrc->m_ptFloatHelper =
			ptTrack - sizeOffset;

		if( ((CExtMouseCaptureSink *)m_pExtBarSrc)->GetCapture() != m_pExtBarSrc->GetSafeHwnd() )
			((CExtMouseCaptureSink *)m_pExtBarSrc)->SetCapture( m_pExtBarSrc->GetSafeHwnd() );
		m_pExtBarSrc->FloatControlBar();

		ASSERT( m_pExtBarSrc->m_pDockContext != NULL );
		m_pExtBarSrc->m_pDockContext->m_ptMRUFloatPos =
			m_pExtBarSrc->m_ptFloatHelper;

		if( ((CExtMouseCaptureSink *)m_pExtBarSrc)->GetCapture() != m_pExtBarSrc->GetSafeHwnd() )
			((CExtMouseCaptureSink *)m_pExtBarSrc)->SetCapture( m_pExtBarSrc->GetSafeHwnd() );
		return;
	} // if( !bAlreadyFloat )

CFrameWnd * pFloatingFrame = m_pExtBarSrc->GetParentFrame();
	ASSERT_VALID( pFloatingFrame );
	ASSERT_KINDOF( CMiniDockFrameWnd, pFloatingFrame );
CRect wrFloatingFrame;
	pFloatingFrame->GetWindowRect( &wrFloatingFrame );
	wrFloatingFrame.OffsetRect(
		-wrFloatingFrame.TopLeft() + (ptTrack-sizeOffset)
		);
	wrFloatingFrame.OffsetRect(
		- m_pExtBarSrc->m_ptHelperDragOffsetFloated
		);
//	
//CExtPaintManager::monitor_parms_t _mp;
//	CExtPaintManager::stat_GetMonitorParms( _mp, m_pExtBarSrc );
//	if( wrFloatingFrame.top < _mp.m_rcWorkArea.top )
//		wrFloatingFrame.OffsetRect(
//			0,
//			_mp.m_rcWorkArea.top - wrFloatingFrame.top
//			);
//	if( wrFloatingFrame.left < _mp.m_rcWorkArea.left )
//		wrFloatingFrame.OffsetRect(
//			_mp.m_rcWorkArea.left - wrFloatingFrame.left,
//			0
//			);
//	if( wrFloatingFrame.top + afxData.cyBorder2*2 + nCyGripper > _mp.m_rcWorkArea.bottom )
//		wrFloatingFrame.OffsetRect(
//			0,
//			_mp.m_rcWorkArea.bottom - (wrFloatingFrame.top + afxData.cyBorder2*2 + nCyGripper)
//			);
//	if( ptTrack.x > _mp.m_rcWorkArea.right )
//		wrFloatingFrame.OffsetRect(
//			_mp.m_rcWorkArea.right - ptTrack.x,
//			0
//			);
	pFloatingFrame->MoveWindow( &wrFloatingFrame );
	if( !m_pExtBarSrc->_IsDockSiteCustomizeMode() )
		pFloatingFrame->BringWindowToTop();
	pFloatingFrame->DelayRecalcLayout();

	ASSERT( m_pExtBarSrc->m_pDockContext != NULL );
	m_pExtBarSrc->m_pDockContext->m_ptMRUFloatPos =
		wrFloatingFrame.TopLeft();
	
	if( ((CExtMouseCaptureSink *)m_pExtBarSrc)->GetCapture() != m_pExtBarSrc->GetSafeHwnd() )
		((CExtMouseCaptureSink *)m_pExtBarSrc)->SetCapture( m_pExtBarSrc->GetSafeHwnd() );
}

bool CExtControlBar::InternalDraggingState_t::_DoFixed_OuterDockerAdjustment(
	CPoint ptTrack, // in screen coords
	CExtDockOuterBar * pOuterDocker
	)
{
	ASSERT_VALID( m_pExtBarSrc );
	ASSERT_VALID( m_pExtBarSrc->m_pDockSite );
	ASSERT_VALID( pOuterDocker );

	g_DragStateOld.SetCFR( __ECFR_NO_RESIZING );

	if(	(		pOuterDocker->m_dwStyle 
			&	m_pExtBarSrc->m_dwDockStyle
			&	CBRS_ALIGN_ANY
			) == 0
		)
		return false;

INT nCyGripper =
		min(
			m_pExtBarSrc->m_rcGrip.Width(),
			m_pExtBarSrc->m_rcGrip.Height()
			);
CSize sizeOffset( nCyGripper, nCyGripper/2 );

UINT nDockBarID = pOuterDocker->GetDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
BOOL bHorzDockBar = 
		(	nDockBarID == AFX_IDW_DOCKBAR_TOP
		||	nDockBarID == AFX_IDW_DOCKBAR_BOTTOM )
		? TRUE : FALSE;

	if( bHorzDockBar )
	{
		sizeOffset.cx = m_pExtBarSrc->m_ptHelperDragOffsetDockedH.x;
		sizeOffset.cy = m_pExtBarSrc->m_ptHelperDragOffsetDockedH.y;
	}
	else
	{
		sizeOffset.cx = m_pExtBarSrc->m_ptHelperDragOffsetDockedV.x;
		sizeOffset.cy = m_pExtBarSrc->m_ptHelperDragOffsetDockedV.y;
	}

	if( pOuterDocker != m_pExtBarSrc->m_pDockBar )
	{
		CSize sizeDocked =
			m_pExtBarSrc->CalcFixedLayout(
				FALSE,
				bHorzDockBar
				);
		ASSERT_VALID( m_pExtBarSrc->m_pDockBar );
		CRect rcInsert( ptTrack - sizeOffset, sizeDocked );
		m_pExtBarSrc->_AffixmentSafeClearOuter();
		pOuterDocker->DockControlBar(
			m_pExtBarSrc,
			rcInsert,
			true
			);
		if(		m_pExtBarSrc->_IsDockSiteCustomizeMode()
			&&	(!m_pExtBarSrc->IsSingleOnRow())
			)
			m_pExtBarSrc->MakeSingleOnRow();
		m_pExtBarSrc->_AffixmentRecalcOuter();
		m_pExtBarSrc->m_pDockSite->RecalcLayout();
		return true;
	} // if( pOuterDocker != m_pExtBarSrc->m_pDockBar )

bool bSwap = false;
CRect rcAllRow;
MfcControlBarVector_t vRow;
	m_pExtBarSrc->_GetRowBars(
		vRow,
		&rcAllRow,
		NULL,
		NULL,
		&bSwap
		);
	ASSERT( vRow.GetSize() > 0 );

int nMoveUpDownDirection = 0;
int nAdjustPixelsLT = 0, nAdjustPixelsRB = 0;
	if(		nDockBarID == AFX_IDW_DOCKBAR_RIGHT
		||	nDockBarID == AFX_IDW_DOCKBAR_BOTTOM
		)
		nAdjustPixelsRB = -2;
	else
		nAdjustPixelsLT = 2;
CRect rcUpDownAnalyze( rcAllRow );
	if( bHorzDockBar )
	{
		rcUpDownAnalyze.InflateRect( 0, 1 );
		if(		ptTrack.y <= (rcUpDownAnalyze.top + nAdjustPixelsLT)
			||	ptTrack.y >= (rcUpDownAnalyze.bottom + nAdjustPixelsRB)
			)
		{
			if( ptTrack.y <= (rcUpDownAnalyze.top + nAdjustPixelsLT) )
				nMoveUpDownDirection = -1;
			else
				nMoveUpDownDirection = 1;
		}
	} // if( bHorzDockBar )
	else
	{
		rcUpDownAnalyze.InflateRect( 1, 0 );
		if(		ptTrack.x <= (rcUpDownAnalyze.left + nAdjustPixelsLT)
			||	ptTrack.x >= (rcUpDownAnalyze.right + nAdjustPixelsRB)
			)
		{
			if( ptTrack.x <= (rcUpDownAnalyze.left + nAdjustPixelsLT) )
				nMoveUpDownDirection = -1;
			else
				nMoveUpDownDirection = 1;
		}
	} // else from if( bHorzDockBar )
	if( nMoveUpDownDirection != 0 )
	{
		if( vRow.GetSize() != 1 )
		{
			m_pExtBarSrc->_AffixmentSafeClearOuter();
			pOuterDocker->_NewRowDockControlBar(
				m_pExtBarSrc,
				m_pExtBarSrc,
				(nMoveUpDownDirection < 0) ? true : false
				);
			m_pExtBarSrc->_AffixmentRecalcOuter();

			m_pExtBarSrc->_AffixmentRecalcOuter();
			_DoFixed_StaticMinimalDockSiteRecalc(
				m_pExtBarSrc->m_pDockSite,
				pOuterDocker
				);
			return true;
		} // if( vRow.GetSize() != 1 )

		bool bSwapXtra = false;
		CRect rcAllRow2;
		MfcControlBarVector_t vRow2;
		m_pExtBarSrc->_GetNextRowBars(
			(nMoveUpDownDirection < 0) ? false : true,
			vRow2,
			&rcAllRow2,
			NULL,
			NULL,
			&bSwapXtra
			);
		if( bSwapXtra && (!bSwap) && vRow2.GetSize() > 0 )
		{
			CRect rcOuterDockerClient;
			pOuterDocker->GetClientRect( &rcOuterDockerClient );
			int nOuterDockerExtent = bHorzDockBar
				? rcOuterDockerClient.Width()
				: rcOuterDockerClient.Height()
				;
			CSize sizeDragMin = m_pExtBarSrc->_CalcDesiredMinOuterSize( bHorzDockBar );
			int nExtentDragMin = bHorzDockBar
				? sizeDragMin.cx
				: sizeDragMin.cy
				;
			int nExtentOtherOuterMin = 0;
			for( int nBar = 0; nBar < vRow2.GetSize(); nBar++ )
			{
				CControlBar * pOtherBar = vRow2[nBar];
				ASSERT_VALID( pOtherBar );
				ASSERT_KINDOF( CControlBar, pOtherBar );
				CExtControlBar * pExtBar =
					DYNAMIC_DOWNCAST( CExtControlBar, pOtherBar );
				CRect rcBarWnd;
				pOtherBar->GetWindowRect( &rcBarWnd );
				nExtentOtherOuterMin += bHorzDockBar
					? rcBarWnd.Width()
					: rcBarWnd.Height()
					;
				if(		pExtBar != NULL
					&&	pExtBar->IsFixedMode()
					&&	(! pExtBar->_GetFullRowMode() )
					)
				{
					CRect rcBarClient;
					pOtherBar->GetClientRect( &rcBarClient );
					CSize sizeOtherOuterMin = pExtBar->_CalcDesiredMinOuterSize( bHorzDockBar );
					nExtentOtherOuterMin += bHorzDockBar
						? (sizeOtherOuterMin.cx - rcBarClient.Width())
						: (sizeOtherOuterMin.cy - rcBarClient.Height())
						;
				}
			} // for( int nBar = 0; nBar < vRow2.GetSize(); nBar++ )
			if( (nExtentDragMin + nExtentOtherOuterMin) < nOuterDockerExtent )
				bSwapXtra = false;
		} // if( bSwapXtra && (!bSwap) && vRow2.GetSize() > 0 )
		
		if(		m_pExtBarSrc->_IsDockSiteCustomizeMode()
			&&	m_pExtBarSrc->IsSingleOnRow()
			)
			bSwap = true;

		if( bSwap || bSwapXtra )
		{
			if( vRow2.GetSize() > 0 )
			{
				CRect wrDockBar;
				pOuterDocker->GetWindowRect( &wrDockBar );
				CRect rcAnalyze( rcAllRow2 );
				if( bHorzDockBar )
				{
					rcAnalyze.left = wrDockBar.left;
					rcAnalyze.right = wrDockBar.right;
				} // if( bHorzDockBar )
				else
				{
					rcAnalyze.top = wrDockBar.top;
					rcAnalyze.bottom = wrDockBar.bottom;
				} // else from if( bHorzDockBar )
				if( nMoveUpDownDirection < 0 )
				{
					rcAnalyze.OffsetRect(
						bHorzDockBar ? 0 : (rcAllRow.right - rcAllRow2.right),
						bHorzDockBar ? (rcAllRow.bottom - rcAllRow2.bottom) : 0
						);
				} // if( nMoveUpDownDirection < 0 )
				else
				{
					rcAnalyze.OffsetRect(
						bHorzDockBar ? 0 : (rcAllRow2.right - rcAllRow.right),
						bHorzDockBar ? (rcAllRow2.bottom - rcAllRow.bottom) : 0
						);
				} // else from if( nMoveUpDownDirection < 0 )
				if( !rcAnalyze.PtInRect(ptTrack) )
				{
					_DoFixed_StaticOuterRowSwap(
						vRow2[0],
						vRow[0]
						);
					_DoFixed_StaticMinimalDockSiteRecalc(
						m_pExtBarSrc->m_pDockSite,
						pOuterDocker
						);
					return true;
				} // if( !rcAnalyze.PtInRect(ptTrack) )
			} // if( vRow2.GetSize() > 0 )
		} // if( bSwap || bSwapXtra )
		else
		{
			ASSERT( !bSwap );
			ASSERT( !bSwapXtra );
			if( vRow2.GetSize() != 0 )
			{
				// do redock to existing row
				CRect rcBar;
				m_pExtBarSrc->GetWindowRect( &rcBar );
				pOuterDocker->ScreenToClient( &rcBar );
				int nOffsetInRowActual = bHorzDockBar
						? rcBar.left
						: rcBar.top
						;
				CPoint ptShifted( ptTrack - sizeOffset );
				pOuterDocker->ScreenToClient( &ptShifted );
				int nOffsetInRowDesired = bHorzDockBar
						? ptShifted.x
						: ptShifted.y
						;
				int nShiftDesired = nOffsetInRowDesired - nOffsetInRowActual;
				
				//m_pExtBarSrc->_AffixmentSafeClearOuter();
				if( pOuterDocker->_ReDockToNewRow(
						m_pExtBarSrc,
						vRow2,
						nShiftDesired
						)
					)
				{ // if redock to new row is successful
					m_pExtBarSrc->_AffixmentRecalcOuter();
					_DoFixed_StaticMinimalDockSiteRecalc(
						m_pExtBarSrc->m_pDockSite,
						pOuterDocker
						);
					return true;
				} // if redock to new row is successful
				
				// if redock to new row is failed
				if( vRow2.GetSize() > 0 )
				{ // try swapping - 2nd attempt
					CRect wrDockBar;
					pOuterDocker->GetWindowRect( &wrDockBar );
					CRect rcAnalyze( rcAllRow2 );
					if( bHorzDockBar )
					{
						rcAnalyze.left = wrDockBar.left;
						rcAnalyze.right = wrDockBar.right;
					} // if( bHorzDockBar )
					else
					{
						rcAnalyze.top = wrDockBar.top;
						rcAnalyze.bottom = wrDockBar.bottom;
					} // else from if( bHorzDockBar )
					if( nMoveUpDownDirection < 0 )
					{
						rcAnalyze.OffsetRect(
							bHorzDockBar ? 0 : (rcAllRow.right - rcAllRow2.right),
							bHorzDockBar ? (rcAllRow.bottom - rcAllRow2.bottom) : 0
							);
					} // if( nMoveUpDownDirection < 0 )
					else
					{
						rcAnalyze.OffsetRect(
							bHorzDockBar ? 0 : (rcAllRow2.right - rcAllRow.right),
							bHorzDockBar ? (rcAllRow2.bottom - rcAllRow.bottom) : 0
							);
					} // else from if( nMoveUpDownDirection < 0 )
					if( !rcAnalyze.PtInRect(ptTrack) )
					{
m_pExtBarSrc->_AffixmentRecalcOuter();
						_DoFixed_StaticOuterRowSwap(
							vRow2[0],
							vRow[0]
							);
						_DoFixed_StaticMinimalDockSiteRecalc(
							m_pExtBarSrc->m_pDockSite,
							pOuterDocker
							);
						return true;
					} // if( !rcAnalyze.PtInRect(ptTrack) )
				} // try swapping - 2nd attempt
			} // if( vRow2.GetSize() != 0 )
		} // else from if( bSwap || bSwapXtra )
	} // if( nMoveUpDownDirection != 0 )
//	if( bSwap )
//		return true;

	// do slide
CRect rcBar;
	m_pExtBarSrc->GetWindowRect( &rcBar );
	pOuterDocker->ScreenToClient( &rcBar );
int nOffsetInRowActual = bHorzDockBar
		? rcBar.left
		: rcBar.top
		;
CPoint ptShifted( ptTrack - sizeOffset );
	pOuterDocker->ScreenToClient( &ptShifted );
int nOffsetInRowDesired = bHorzDockBar
		? ptShifted.x
		: ptShifted.y
		;
int nShiftDesired = nOffsetInRowDesired - nOffsetInRowActual;

BOOL bLayoutQueryOld = pOuterDocker->m_bLayoutQuery;
CRect rectLayoutOld( pOuterDocker->m_rectLayout );
	pOuterDocker->m_bLayoutQuery = FALSE;
	pOuterDocker->GetWindowRect( &pOuterDocker->m_rectLayout );
	pOuterDocker->m_rectLayout.OffsetRect(
		- pOuterDocker->m_rectLayout.TopLeft()
		);
bool bInplaceResizing;
	if( m_pExtBarSrc->IsFixedMode() ) // (+ v.2.22)
	{
		InternalAffixmentData * pAffixmentData =
			m_pExtBarSrc->_AffixmentGetOuter();
		ASSERT( pAffixmentData != NULL );
		if(		(! pAffixmentData->IsEmpty() )
			&&	(! m_pExtBarSrc->_GetFullRowMode() )
			)
		{
			bool bInitialRecalcAffixment = false;
			if( (pAffixmentData->m_dwAffixmentFlags & InternalAffixmentData::__EAFF_FIXED_MODE_INITIAL_RECALC) != 0 )
				bInitialRecalcAffixment = true;
			else
			{
				CRect rcOuterDocker;
				pOuterDocker->GetClientRect( &rcOuterDocker );
				ASSERT( rcOuterDocker.left == 0 && rcOuterDocker.top == 0 );
				if(		(bHorzDockBar && pAffixmentData->m_rcAffixment.Width() >= rcOuterDocker.right)
					||	(bHorzDockBar && pAffixmentData->m_rcAffixment.Height() >= rcOuterDocker.bottom)
					)
					bInitialRecalcAffixment = true;
			} // else from if( (pAffixmentData->m_dwAffixmentFlags & InternalAffixmentData::__EAFF_FIXED_MODE_INITIAL_RECALC) != 0 )
			if( bInitialRecalcAffixment )
			{
				pAffixmentData->Empty(); // initial recalc
				pAffixmentData->m_dwAffixmentFlags &=
					~(InternalAffixmentData::__EAFF_FIXED_MODE_INITIAL_RECALC);
			} // bInitialRecalcAffixment
		}
		if( pAffixmentData->IsEmpty() )
			pOuterDocker->_AffixmentBringToTop( m_pExtBarSrc );
	} // if( m_pExtBarSrc->IsFixedMode() ) // (+ v.2.22)
	pOuterDocker->_RedockInRow(
		m_pExtBarSrc,
		nShiftDesired,
		&bInplaceResizing
		);
	pOuterDocker->m_bLayoutQuery = bLayoutQueryOld;
	pOuterDocker->m_rectLayout = rectLayoutOld;

	if( bInplaceResizing )
	{
		g_DragStateOld.SetCFR(
			bHorzDockBar ? __ECFR_HORZ : __ECFR_VERT
			);
	} // if( bInplaceResizing )

	return true;
}

void CExtControlBar::InternalDraggingState_t::_DoFixed_StaticMinimalDockSiteRecalc(
	CFrameWnd * pDockSite,
	CExtDockOuterBar * pOuterDocker
	)
{
	ASSERT_VALID( pDockSite ); 
	ASSERT_VALID( pOuterDocker ); 
	ASSERT( pOuterDocker->GetParent() == pDockSite );

	if( (pOuterDocker->GetStyle() & WS_VISIBLE) == 0 )
	{
		pDockSite->RecalcLayout();
		return;
	}

UINT nDockBarID = pOuterDocker->GetDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
BOOL bHorzDockBar = 
		(	nDockBarID == AFX_IDW_DOCKBAR_TOP
		||	nDockBarID == AFX_IDW_DOCKBAR_BOTTOM )
		? TRUE : FALSE;

BOOL bLayoutQueryOld = pOuterDocker->m_bLayoutQuery;
CRect rectLayoutOld( pOuterDocker->m_rectLayout );

CRect rcDockerWnd;
	pOuterDocker->GetWindowRect( &rcDockerWnd );
CSize sizeDockBarBefore = rcDockerWnd.Size();

	pOuterDocker->m_rectLayout = rcDockerWnd;
	pOuterDocker->m_rectLayout.OffsetRect(
		- pOuterDocker->m_rectLayout.TopLeft()
		);
	pOuterDocker->m_bLayoutQuery = TRUE;
CSize sizeDockBarAfter =
		pOuterDocker->CalcFixedLayout( FALSE, bHorzDockBar );

bool bSizesAreEqual = false;
	if( bHorzDockBar )
	{
		if( sizeDockBarAfter.cy == sizeDockBarBefore.cy )
			bSizesAreEqual = true;
	} // if( bHorzDockBar )
	else
	{
		if( sizeDockBarAfter.cx == sizeDockBarBefore.cx )
			bSizesAreEqual = true;
	} // else from if( bHorzDockBar )

	if( bSizesAreEqual )
	{
		pOuterDocker->m_bLayoutQuery = FALSE;
		pOuterDocker->CalcFixedLayout( FALSE, bHorzDockBar );
	}

	pOuterDocker->m_bLayoutQuery = bLayoutQueryOld;
	pOuterDocker->m_rectLayout = rectLayoutOld;

	if( !bSizesAreEqual )
		pDockSite->RecalcLayout();
}

void CExtControlBar::InternalDraggingState_t::_DoFixed_StaticOuterRowSwap(
	CControlBar * pBar1,
	CControlBar * pBar2
	)
{
	ASSERT_VALID( pBar1 );
	ASSERT_VALID( pBar2 );
	ASSERT_VALID( pBar1->m_pDockBar );
	ASSERT_VALID( pBar2->m_pDockBar );
	ASSERT_VALID( pBar1->m_pDockSite );
	ASSERT_VALID( pBar2->m_pDockSite );
	ASSERT( pBar1->m_pDockBar == pBar2->m_pDockBar );
	ASSERT( pBar1->m_pDockSite == pBar2->m_pDockSite );
	ASSERT( pBar1 != pBar2 );
CDockBar * pDockBar = pBar1->m_pDockBar;
int nPos1 = pDockBar->FindBar( pBar1 );
int	nPos2 = pDockBar->FindBar( pBar2 );
	ASSERT( nPos1 != nPos2 );
bool bWalkUp = (nPos1 < nPos2) ? true : false;

	ASSERT( pDockBar->m_arrBars[0] == NULL );

MfcControlBarVector_t vPush;
int nPosRemovedStart = nPos1, nPosRemovedEnd = nPos1;
	for( int nPos = nPos1; nPos > 0; nPos-- )
	{
		CControlBar * pBar = (CControlBar *)pDockBar->m_arrBars[nPos];
		if( pBar == NULL )
			break;
		vPush.InsertAt( 0, pBar );
		nPosRemovedStart = nPos;
	}
int nCount = pDockBar->m_arrBars.GetSize();
	for( nPos = nPos1+1; nPos < nCount; nPos++ )
	{
		CControlBar * pBar = (CControlBar *)pDockBar->m_arrBars[nPos];
		if( pBar == NULL )
			break;
		vPush.Add( pBar );
		nPosRemovedEnd = nPos;
	}
	if( nPosRemovedEnd < nCount-1 )
	{
		CControlBar * pBar = (CControlBar *)pDockBar->m_arrBars[nPosRemovedEnd+1];
		if( pBar == NULL )
			nPosRemovedEnd++;
	}
int nCountToRemove = nPosRemovedEnd - nPosRemovedStart + 1;
	pDockBar->m_arrBars.RemoveAt( nPosRemovedStart, nCountToRemove );
	nPos2 = pDockBar->FindBar( pBar2 );
	nCount = pDockBar->m_arrBars.GetSize();
	if( bWalkUp )
	{
		for( ; nPos2 < nCount; nPos2++ )
		{
			CControlBar * pBar = (CControlBar *)pDockBar->m_arrBars[nPos2];
			if( pBar == NULL )
			{
				nPos2++;
				break;
			}
			if( nPos2 == nCount-1 )
			{
				pDockBar->m_arrBars.Add( (CControlBar *)NULL );
				nPos2++;
				break;
			}
		}
	} // if( bWalkUp )
	else
	{
		ASSERT( nPos2 > 0 );
		for( ; nPos2 > 0; nPos2-- )
		{
			CControlBar * pBar = (CControlBar *)pDockBar->m_arrBars[nPos2];
			if( pBar == NULL )
			{
				nPos2++;
				break;
			}
		}
		if( nPos2 == 0 )
			nPos2++;
	} // else from if( bWalkUp )
	nCount = vPush.GetSize();
	for( nPos = 0; nPos < nCount; nPos++, nPos2++ )
		pDockBar->m_arrBars.InsertAt( nPos2, vPush[nPos] );
	pDockBar->m_arrBars.InsertAt( nPos2, (CControlBar *)NULL );
	ASSERT_VALID( pDockBar );
}

void CExtControlBar::InternalDraggingState_t::_DoFixed_BasicAdjustment(
	CPoint ptTrack, // in screen coords
	bool bForceFloatMode
	)
{
	ASSERT_VALID( this );
	ASSERT( !IsEmpty() );
	ASSERT_VALID( m_pExtBarSrc );
	ASSERT( m_pExtBarSrc->IsFixedDockStyle() );
	
	ASSERT( m_pExtBarSrc->_IsShowContentWhenDragging() );
	
	TrackRectSetEmpty();

BOOL bForceFloat =
		(	bForceFloatMode ||
			CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL)
		) ? TRUE : FALSE;
	if( bForceFloat )
	{
		_DoFixed_FloatingAdjustment( ptTrack );
		return;
	}

CFrameWnd * pFrame = m_pExtBarSrc->_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );

CRect wrFrame;
	pFrame->GetWindowRect( &wrFrame );
	wrFrame.InflateRect( afxData.cxBorder2, afxData.cyBorder2 );
	if( !wrFrame.PtInRect(ptTrack) )
	{
		_DoFixed_FloatingAdjustment( ptTrack );
		return;
	}

struct
{
	UINT m_nDockBarID;
	bool m_bForceFirstReview:1, m_bVisible:1, m_bForceExclude:1;
	RECT m_rcDockBarAnalyze;
	CExtDockOuterBar * m_pOuterDocker;
} arrDockBarAnalyze[4] =
{
	{ AFX_IDW_DOCKBAR_TOP,		false, false, false, {0,0,0,0}, NULL },
	{ AFX_IDW_DOCKBAR_BOTTOM,	false, false, false, {0,0,0,0}, NULL },
	{ AFX_IDW_DOCKBAR_LEFT,		false, false, false, {0,0,0,0}, NULL },
	{ AFX_IDW_DOCKBAR_RIGHT,	false, false, false, {0,0,0,0}, NULL },
};
UINT nOwnDockBarID = m_pExtBarSrc->GetSafeDockBarDlgCtrlID();
int nIdxForceFirstReview = -1;
	for( int nDockBarIdx = 0; nDockBarIdx < 4; nDockBarIdx++ )
	{
		if( nOwnDockBarID == arrDockBarAnalyze[nDockBarIdx].m_nDockBarID )
		{
			ASSERT( nIdxForceFirstReview < 0 );
			nIdxForceFirstReview = nDockBarIdx;
			arrDockBarAnalyze[nDockBarIdx].m_bForceFirstReview = true;
		}
		arrDockBarAnalyze[nDockBarIdx].m_pOuterDocker =
			STATIC_DOWNCAST(
				CExtDockOuterBar,
				pFrame->GetControlBar(
					arrDockBarAnalyze[nDockBarIdx].m_nDockBarID
					)
				);
		ASSERT_VALID( arrDockBarAnalyze[nDockBarIdx].m_pOuterDocker );
//		const int nRdpExtraInnerDx = 6; // (- v.2.23)
//		const int nRdpExtraInnerDy = 6; // (- v.2.23)
//		const int nRdpExtraOuterDx = nRdpExtraInnerDx*2; // (- v.2.23)
//		const int nRdpExtraOuterDy = nRdpExtraInnerDy*2; // (- v.2.23)
		const int nRdpExtraInnerDx = 12; // (+ v.2.23)
		const int nRdpExtraInnerDy = 12; // (+ v.2.23)
		const int nRdpExtraOuterDx = ::MulDiv( nRdpExtraInnerDx, 3, 2 ); // (+ v.2.23)
		const int nRdpExtraOuterDy = ::MulDiv( nRdpExtraInnerDy, 3, 2 ); // (+ v.2.23)

		arrDockBarAnalyze[nDockBarIdx].m_bVisible =
			(arrDockBarAnalyze[nDockBarIdx].m_pOuterDocker->GetStyle() & WS_VISIBLE)
				? true : false;
		if( arrDockBarAnalyze[nDockBarIdx].m_bVisible )
		{
			arrDockBarAnalyze[nDockBarIdx].m_pOuterDocker->GetWindowRect(
				&( arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze )
				);
		} // if( arrDockBarAnalyze[nDockBarIdx].m_bVisible )
		else
		{
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze = wrFrame;
			switch( arrDockBarAnalyze[nDockBarIdx].m_nDockBarID )
			{
			case AFX_IDW_DOCKBAR_TOP:
				arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.bottom =
					arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.top;
			break;
			case AFX_IDW_DOCKBAR_BOTTOM:
				arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.top =
					arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.bottom;
			break;
			case AFX_IDW_DOCKBAR_LEFT:
				arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.right =
					arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.left;
			break;
			case AFX_IDW_DOCKBAR_RIGHT:
				arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.left =
					arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.right;
			break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
			break;
#endif // _DEBUG
			} // switch( arrDockBarAnalyze[nDockBarIdx].m_nDockBarID )
		} // else from if( arrDockBarAnalyze[nDockBarIdx].m_bVisible )

		switch( arrDockBarAnalyze[nDockBarIdx].m_nDockBarID )
		{
		case AFX_IDW_DOCKBAR_TOP:
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.bottom +=
				nRdpExtraInnerDy;
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.top =
				wrFrame.top;
		break;
		case AFX_IDW_DOCKBAR_BOTTOM:
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.top -=
				nRdpExtraInnerDy;
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.bottom =
				wrFrame.bottom;
		break;
		case AFX_IDW_DOCKBAR_LEFT:
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.right +=
				nRdpExtraInnerDx;
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.left =
				wrFrame.left;
		break;
		case AFX_IDW_DOCKBAR_RIGHT:
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.left -=
				nRdpExtraInnerDx;
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.right =
				wrFrame.right;
		break;
#ifdef _DEBUG
		default:
			ASSERT( FALSE );
		break;
#endif // _DEBUG
		} // switch( arrDockBarAnalyze[nDockBarIdx].m_nDockBarID )

		if( arrDockBarAnalyze[nDockBarIdx].m_nDockBarID ==
				AFX_IDW_DOCKBAR_LEFT
			||
			arrDockBarAnalyze[nDockBarIdx].m_nDockBarID ==
				AFX_IDW_DOCKBAR_RIGHT
			)
		{
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.top =
				arrDockBarAnalyze[0].m_rcDockBarAnalyze.bottom
				- nRdpExtraInnerDy;
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze.bottom =
				arrDockBarAnalyze[1].m_rcDockBarAnalyze.top
				+ nRdpExtraInnerDy;
		}

		CRect rcReDockAnalyze(
			arrDockBarAnalyze[nDockBarIdx].m_rcDockBarAnalyze
			);
		rcReDockAnalyze.InflateRect(
			nRdpExtraOuterDx,
			nRdpExtraOuterDy
			);
		if( !rcReDockAnalyze.PtInRect( ptTrack ) )
			arrDockBarAnalyze[nDockBarIdx].m_bForceExclude = true;
	} // for( int nDockBarIdx = 0; nDockBarIdx < 4; nDockBarIdx++ )

	if( nIdxForceFirstReview >= 0 )
	{
		if( arrDockBarAnalyze[nIdxForceFirstReview].m_bForceExclude )
			arrDockBarAnalyze[nIdxForceFirstReview].m_bForceFirstReview = false;
		else
		{
			if( _DoFixed_OuterDockerAdjustment(
					ptTrack,
					arrDockBarAnalyze[nIdxForceFirstReview].m_pOuterDocker
					)
				)
				return;
		} // else from if( arrDockBarAnalyze[nDockBarIdx].m_bForceExclude )
	} // if( nIdxForceFirstReview >= 0 )

	for( nDockBarIdx = 0; nDockBarIdx < 4; nDockBarIdx++ )
	{
		ASSERT_VALID( arrDockBarAnalyze[nDockBarIdx].m_pOuterDocker );
		if(		arrDockBarAnalyze[nDockBarIdx].m_bForceFirstReview
			||	(! arrDockBarAnalyze[nDockBarIdx].m_bVisible )
			||	arrDockBarAnalyze[nDockBarIdx].m_bForceExclude
			)
			continue;
		
		if( _DoFixed_OuterDockerAdjustment(
				ptTrack,
				arrDockBarAnalyze[nDockBarIdx].m_pOuterDocker
				)
			)
			return;
	} // for( nDockBarIdx = 0; nDockBarIdx < 4; nDockBarIdx++ )

	for( nDockBarIdx = 0; nDockBarIdx < 4; nDockBarIdx++ )
	{
		if(		arrDockBarAnalyze[nDockBarIdx].m_bForceFirstReview
			||	arrDockBarAnalyze[nDockBarIdx].m_bVisible
			||	arrDockBarAnalyze[nDockBarIdx].m_bForceExclude
			)
			continue;
		if( _DoFixed_OuterDockerAdjustment(
				ptTrack,
				arrDockBarAnalyze[nDockBarIdx].m_pOuterDocker
				)
			)
			return;
	} // for( nDockBarIdx = 0; nDockBarIdx < 4; nDockBarIdx++ )

	_DoFixed_FloatingAdjustment( ptTrack );
}

void CExtControlBar::InternalDraggingState_t::CalcStateDynamic(
	CPoint ptTrack, // in screen coords
	bool bForceFloatMode
	)
{
	ASSERT_VALID( this );
	ASSERT( !IsEmpty() );
	ASSERT_VALID( m_pExtBarSrc );

	if( m_pExtBarSrc->IsFixedDockStyle() )
	{
		ASSERT( m_pExtBarSrc->_IsShowContentWhenDragging() );
		_DoFixed_BasicAdjustment( ptTrack, bForceFloatMode );
		return;
	}

bool bShowContent = m_pExtBarSrc->_IsShowContentWhenDragging();
	if( bShowContent && m_pExtBarSrc->IsDocked() )
	{
		ASSERT_VALID( m_pExtBarSrc->m_pDockBar );
		CRect rcDockBar;
		m_pExtBarSrc->m_pDockBar->GetWindowRect( &rcDockBar );
		if( rcDockBar.PtInRect(ptTrack) )
			return;
	}
	
	TrackRectSetEmpty();

static const DWORD _dwDockBarMap[4][2] =
{
	{ AFX_IDW_DOCKBAR_TOP,      CBRS_TOP    },
	{ AFX_IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
	{ AFX_IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
	{ AFX_IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
};

// reasonable dockbar parts
static const INT nRdpOuter = 16;
static const INT nRdpInner = nRdpOuter*2;
static const INT nRdpMostInner = 12;
// reasonable controlbar part
static const INT nRcpAnyHalfMin = 8;

	m_eCDT = __ECDT_FLOATED;
	m_pExtBarDst = NULL;
	//ExtBarDstSet( NULL );

CRect rcBarWnd;

INT nSrcMinHW = m_pExtBarSrc->_CalcDesiredMinHW();
INT nSrcMinVH = m_pExtBarSrc->_CalcDesiredMinVH();
	if( nSrcMinHW < nRcpAnyHalfMin )
		nSrcMinHW = nRcpAnyHalfMin;
	if( nSrcMinVH < nRcpAnyHalfMin )
		nSrcMinVH = nRcpAnyHalfMin;
INT nDstMinHW = nRcpAnyHalfMin; // some reasonable min width when horz docked
INT nDstMinVH = nRcpAnyHalfMin; // some reasonable min height when vert docked

CFrameWnd * pFrame = m_pExtBarSrc->_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
CRect rcFrameWnd;
	pFrame->GetWindowRect( &rcFrameWnd );

bool bForceFloat = bForceFloatMode;
	if(		( !bForceFloat )
		&&	m_pExtBarSrc->IsFloating()
		&& (	m_pExtBarSrc->IsKindOf(RUNTIME_CLASS(CExtDynControlBar))
				|| !bShowContent
			)
		)
	{
		CFrameWnd * pFrameExcludeTest = m_pExtBarSrc->GetParentFrame();
		if( pFrameExcludeTest->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		{
			CRect rcFrameExcludeText;
			pFrameExcludeTest->GetWindowRect( &rcFrameExcludeText );
			if( rcFrameExcludeText.PtInRect(ptTrack) )
				bForceFloat = true;
		}
	}

	if( !CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL)
		&& !bForceFloat
		)
	{
		ASSERT( m_eCDT == __ECDT_FLOATED );

		bool bCanDockToInnerCircles =
			m_pExtBarSrc->_CanDockToInnerCircles();

		if( bCanDockToInnerCircles && ! bShowContent )
		{ // try to analyze docking into floating bars
			ExtControlBarVector_t vBars;
			m_pExtBarSrc->_GetFrameControlBars(
				vBars,
				(DWORD)(
					__GFECB_NONFIXED_NONMIN //|__GFECB_FIXED
						|__GFECB_FLOATED
						|__GFECB_VIS_ALL
						|__GFECB_BYPT_ONLY_WND
					),
				ptTrack
				);

			CExtControlBar * pDesiredBar = NULL;
			INT nCountOfBars = vBars.GetSize();
			for( INT nBarIdx = 0; nBarIdx < nCountOfBars; nBarIdx++ )
			{
				CExtControlBar * pTestBar = vBars[nBarIdx];
				ASSERT_VALID( pTestBar );
				ASSERT( !pTestBar->IsFixedMode() );
				if( pTestBar == m_pExtBarSrc )
					continue;
				pDesiredBar = pTestBar;
				break;
			} // for( INT nBarIdx = 0; nBarIdx < nCountOfBars; nBarIdx++ )

			if( pDesiredBar != NULL )
			{
				ASSERT_VALID( pDesiredBar );
				//m_pExtBarDst = pDesiredBar;
				ExtBarDstSet( pDesiredBar );

				m_pExtBarDst->GetParentFrame()->GetWindowRect( &rcBarWnd );

				nDstMinHW = m_pExtBarDst->_CalcDesiredMinHW();
				nDstMinVH = m_pExtBarDst->_CalcDesiredMinVH();

				INT nWidth3   = nRdpInner; // rcBarWnd.Width();
				INT nHeight3  = nRdpInner; // rcBarWnd.Height();
//				INT nWidth2  = nWidth  / 2;
//				INT nHeight2 = nHeight / 2;
//				INT nWidth3  = nWidth  / 4;
//				INT nHeight3 = nHeight / 4;

//				UINT nDockBarID =
//					m_pExtBarDst->GetSafeDockBarDlgCtrlID();
//				ASSERT_DOCKBAR_DLGCTRLID( nDockBarID );
//				bool bHorz = 
//					(	nDockBarID == AFX_IDW_DOCKBAR_TOP
//					||	nDockBarID == AFX_IDW_DOCKBAR_BOTTOM
//					||	nDockBarID == AFX_IDW_DOCKBAR_FLOAT
//					) ? true : false;
				ASSERT( m_pExtBarDst->m_pDockBar != NULL );
				CDockBar * pDockBar =
					STATIC_DOWNCAST( CDockBar, m_pExtBarDst->m_pDockBar );
				ASSERT_VALID( pDockBar );

				bool bEnableInnerDock = true;
//				if( nDockBarID == AFX_IDW_DOCKBAR_LEFT
//					&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_LEFT) == 0
//					)
//					bEnableInnerDock = false;
//				if( nDockBarID == AFX_IDW_DOCKBAR_RIGHT
//					&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_RIGHT) == 0
//					)
//					bEnableInnerDock = false;
//				if( nDockBarID == AFX_IDW_DOCKBAR_TOP
//					&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_TOP) == 0
//					)
//					bEnableInnerDock = false;
//				if( nDockBarID == AFX_IDW_DOCKBAR_BOTTOM
//					&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_BOTTOM) == 0
//					)
//					bEnableInnerDock = false;
				
				if( bEnableInnerDock && (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_ANY) != 0 )
				{
					m_bDeepHalfSplit = false;

					if( !m_pExtBarDst->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar))
						|| m_pExtBarDst->IsFloating()
						)
						m_bDeepHalfSplit = true;
					
					CRect rcWndDockBar;
					pDockBar->GetWindowRect( &rcWndDockBar );
					
					CRect rcPart( rcBarWnd );
					do
					{ // SINGLE LOOP

						INT nHalfHeight = rcBarWnd.Height()/2;
						if(	rcBarWnd.Width() >= nSrcMinHW
							&& nHalfHeight >= nSrcMinVH
							&& nHalfHeight >= nDstMinVH
							)
						{ // if can do half splitting

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
							BOOL bTabbedDest =
								m_pExtBarDst->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar))
								&& ( ((CExtDynTabControlBar*)m_pExtBarDst)->IsSwitcherVisible() )
								;
							INT nTabbedShift = 0;
							bool bCanDockToTabbedContainers =
								(	m_pExtBarSrc->_CanDockToTabbedContainers( m_pExtBarDst )
									&& m_pExtBarDst->_CanDockToTabbedContainers( m_pExtBarSrc )
									) ? true : false;
							if( (!bShowContent) && bCanDockToTabbedContainers )
							{
								rcPart = rcBarWnd;
								rcPart.bottom = rcPart.top + nHeight3;
								if(	rcPart.PtInRect(ptTrack) )
								{
									if( bTabbedDest )
										m_eCDT = __ECDT_TABBED_UPDATE;
									else
										m_eCDT = __ECDT_TABBED_NEW;
									rcPart = rcBarWnd;
									break;
								}
								nTabbedShift = nHeight3;
								rcPart.OffsetRect( 0, nTabbedShift );
							}
							else
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
							{
								rcPart = rcBarWnd;
								rcPart.bottom = rcPart.top + nHeight3;
							}
							
							
							if( rcPart.PtInRect(ptTrack) )
							{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
								rcPart.OffsetRect( 0, -nTabbedShift );
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
								m_bDeepHalfSplit = true;
								m_eCDT = __ECDT_INNER_TOP;
								rcPart.bottom = rcPart.top + rcBarWnd.Height()/2; // + nHeight2;
								rcPart.left = min( rcPart.left, rcWndDockBar.left );
								rcPart.right = max( rcPart.right, rcWndDockBar.right );
								break;
							} // if( rcPart.PtInRect(ptTrack) )

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
							rcPart = rcBarWnd;
							INT nBottomShift = 0;
							if( bCanDockToTabbedContainers && (!bShowContent)
								&& bTabbedDest
								)
							{
								CExtDynTabControlBar * pTabbedBar =
									(CExtDynTabControlBar *)m_pExtBarDst;
								CRect rcTabSwitcher;
								pTabbedBar->GetSwitcherWindowRect( rcTabSwitcher );
								if( rcTabSwitcher.PtInRect(ptTrack) )
								{
									m_eCDT = __ECDT_TABBED_REINDEX;
									ReindexPosCalc( ptTrack );
									break;
								}
								nBottomShift = rcTabSwitcher.Size().cy;
							}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

							rcPart = rcBarWnd;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
							rcPart.bottom -= nBottomShift;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
							rcPart.top = rcPart.bottom - nHeight3;
							if( rcPart.PtInRect(ptTrack) )
							{
								m_bDeepHalfSplit = true;
								m_eCDT = __ECDT_INNER_BOTTOM;
								rcPart.top = rcPart.bottom - rcBarWnd.Height()/2; // - nHeight2;
								rcPart.left = min( rcPart.left, rcWndDockBar.left );
								rcPart.right = max( rcPart.right, rcWndDockBar.right );
								break;
							} // if( rcPart.PtInRect(ptTrack) )
						} // if can do half splitting

						
						INT nHalfWidth = rcBarWnd.Width()/2;
						if( rcBarWnd.Height() >= nSrcMinVH
							&& nHalfWidth >= nSrcMinHW
							&& nHalfWidth >= nDstMinHW
							)
						{ // if can do half splitting
							rcPart = rcBarWnd;
							rcPart.right = rcPart.left + nWidth3;
							if( rcPart.PtInRect(ptTrack) )
							{
								m_bDeepHalfSplit = true;
								m_eCDT = __ECDT_INNER_LEFT;
								rcPart.right = rcPart.left + rcBarWnd.Width()/2; // + nWidth2;
								rcPart.top = min( rcPart.top, rcWndDockBar.top );
								rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom );
								break;
							} // if( rcPart.PtInRect(ptTrack) )

							rcPart = rcBarWnd;
							rcPart.left = rcPart.right - nWidth3;
							if( rcPart.PtInRect(ptTrack) )
							{
								m_bDeepHalfSplit = true;
								m_eCDT = __ECDT_INNER_RIGHT;
								rcPart.left = rcPart.right - rcBarWnd.Width()/2; // - nWidth2;
								rcPart.top = min( rcPart.top, rcWndDockBar.top );
								rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom );
								break;
							} // if( rcPart.PtInRect(ptTrack) )
						} // if can do half splitting

					} // SINGLE LOOP
					while( false );

//					if( m_bDeepHalfSplit )
//					{
//						if( m_pExtBarDst->IsSingleOnRow() )
//							m_bDeepHalfSplit = false;
//					} // if( m_bDeepHalfSplit )

					if( m_eCDT != __ECDT_FLOATED )
					{

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
						if(	m_bDeepHalfSplit
							&& (	m_eCDT == __ECDT_TABBED_NEW
									|| m_eCDT == __ECDT_TABBED_UPDATE
									|| m_eCDT == __ECDT_TABBED_REINDEX
									)
							)
							m_bDeepHalfSplit = false;

						ASSERT(
								(	m_bDeepHalfSplit
									&& m_eCDT != __ECDT_TABBED_NEW
									&& m_eCDT != __ECDT_TABBED_UPDATE
									&& m_eCDT != __ECDT_TABBED_REINDEX
									)
								||
								(	(!m_bDeepHalfSplit)
									&& (	m_eCDT == __ECDT_TABBED_NEW
											|| m_eCDT == __ECDT_TABBED_UPDATE
											|| m_eCDT == __ECDT_TABBED_REINDEX
											)
									)
							);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

//						if( bHorz )
//						{
							if( rcPart.Width() < nSrcMinHW )
								m_eCDT = __ECDT_FLOATED;
//						} // if( bHorz )
//						else
//						{
							if( rcPart.Height() < nSrcMinVH )
								m_eCDT = __ECDT_FLOATED;
//						} // else from if( bHorz )
						if( m_eCDT != __ECDT_FLOATED )
							rcBarWnd = rcPart;
					} // if( m_eCDT != __ECDT_FLOATED )
				
				} // if( bEnableInnerDock && (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_ANY) != 0 )




			} // if( pDesiredBar != NULL )
		} // try to analyze docking into floating bars

		if( bCanDockToInnerCircles && m_eCDT == __ECDT_FLOATED )
		{
			// first try calc by circle 0 inner part
			CRect rcCircle0( 0, 0, 0, 0 );

			CExtDockBar * vDockBars0[4];
			for( INT nDockSide = 0; nDockSide < 4;  nDockSide++ )
			{
				UINT nDockBarID =
					_dwDockBarMap[ nDockSide ] [ 0 ];
				ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
				vDockBars0[ nDockSide ] = (CExtDockBar *)
					pFrame->GetControlBar( nDockBarID );
				ASSERT_VALID( vDockBars0[ nDockSide ] );
				ASSERT_KINDOF( CExtDockBar, vDockBars0[ nDockSide ] );
				ASSERT( vDockBars0[ nDockSide ] -> _GetCircleNo() == 0 );

				CRect rcDockBar;
				vDockBars0[ nDockSide ]->_GetPreCalcWindowRect( rcDockBar );
				switch( nDockBarID )
				{
				case AFX_IDW_DOCKBAR_TOP:
					rcCircle0.top = rcDockBar.bottom;
					break;
				case AFX_IDW_DOCKBAR_BOTTOM:
					rcCircle0.bottom = rcDockBar.top;
					break;
				case AFX_IDW_DOCKBAR_LEFT:
					rcCircle0.left = rcDockBar.right;
					break;
				case AFX_IDW_DOCKBAR_RIGHT:
					rcCircle0.right = rcDockBar.left;
					break;
#ifdef _DEBUG
				default:
					ASSERT( FALSE );
					break;
#endif // _DEBUG
				} // switch( nDockBarID )
			} // for( INT nDockSide = 0; nDockSide < 4;  nDockSide++ )

//			ASSERT( rcFrameWnd.left <= rcCircle0.left );
//			ASSERT( rcFrameWnd.top <= rcCircle0.top );
//			ASSERT( rcFrameWnd.right >= rcCircle0.right );
//			ASSERT( rcFrameWnd.bottom >= rcCircle0.bottom );
			CRect rcTestCircle0( rcCircle0 );
			//rcTestCircle0.InflateRect( nRdpOuter, nRdpOuter );
			rcTestCircle0.InflateRect( nRdpMostInner, nRdpMostInner );
			if( rcCircle0.left < rcCircle0.right
				&& rcCircle0.top < rcCircle0.bottom
				&& rcTestCircle0.PtInRect( ptTrack )
				)
			{ // if we have sense analyze docking into circle 0 inner part/circle 1 outer part
				for( nDockSide = 0; nDockSide < 4;  nDockSide++ )
				{
					DWORD dwDockStyle =
						_dwDockBarMap[ nDockSide ] [ 1 ];
					if( (m_pExtBarSrc->m_dwDockStyle & dwDockStyle) == 0 )
						continue; // can not dock at this side

					UINT nDockBarID =
						_dwDockBarMap[ nDockSide ] [ 0 ];
					ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );

					CRect rcFrameAnalyze( rcTestCircle0 );

					switch( nDockBarID )
					{
					case AFX_IDW_DOCKBAR_TOP:
					{
						//rcFrameAnalyze.top -= nRdpMostInner;
						rcFrameAnalyze.bottom =
							rcFrameAnalyze.top + nRdpOuter;
						if( rcFrameAnalyze.PtInRect(ptTrack) )
						{
							m_eCDT = __ECDT_CIRCLE1_OUTER_TOP;
							m_nCircleNo = 1;

							rcBarWnd.top = rcCircle0.top;
							rcBarWnd.bottom =
								rcBarWnd.top +
								m_pExtBarSrc->m_sizeDockedH.cy;
							rcBarWnd.left = rcCircle0.left;
							rcBarWnd.right = rcCircle0.right;
						} // if( rcFrameAnalyze.PtInRect(ptTrack) )
					}
					break; // case AFX_IDW_DOCKBAR_TOP
					case AFX_IDW_DOCKBAR_BOTTOM:
					{
						//rcFrameAnalyze.bottom += nRdpMostInner;
						rcFrameAnalyze.top =
							rcFrameAnalyze.bottom - nRdpOuter;
						if( rcFrameAnalyze.PtInRect(ptTrack) )
						{
							m_eCDT = __ECDT_CIRCLE1_OUTER_BOTTOM;
							m_nCircleNo = 1;

							rcBarWnd.bottom = rcCircle0.bottom;
							rcBarWnd.top =
								rcBarWnd.bottom -
								m_pExtBarSrc->m_sizeDockedH.cy;
							rcBarWnd.left = rcCircle0.left;
							rcBarWnd.right = rcCircle0.right;
						} // if( rcFrameAnalyze.PtInRect(ptTrack) )
					}
					break; // case AFX_IDW_DOCKBAR_BOTTOM
					case AFX_IDW_DOCKBAR_LEFT:
					{
						//rcFrameAnalyze.left -= nRdpMostInner;
						rcFrameAnalyze.right =
							rcFrameAnalyze.left + nRdpOuter;
						if( rcFrameAnalyze.PtInRect(ptTrack) )
						{
							m_eCDT = __ECDT_CIRCLE1_OUTER_LEFT;
							m_nCircleNo = 1;
							
							rcBarWnd.left = rcCircle0.left;
							rcBarWnd.right =
								rcBarWnd.left +
								m_pExtBarSrc->m_sizeDockedV.cx;
							rcBarWnd.top = rcCircle0.top;
							rcBarWnd.bottom = rcCircle0.bottom;
						} // if( rcFrameAnalyze.PtInRect(ptTrack) )
					}
					break; // case AFX_IDW_DOCKBAR_LEFT
					case AFX_IDW_DOCKBAR_RIGHT:
					{
						//rcFrameAnalyze.right += nRdpMostInner;
						rcFrameAnalyze.left =
							rcFrameAnalyze.right - nRdpOuter;
						if( rcFrameAnalyze.PtInRect(ptTrack) )
						{
							m_eCDT = __ECDT_CIRCLE1_OUTER_RIGHT;
							m_nCircleNo = 1;
							
							rcBarWnd.right = rcCircle0.right;
							rcBarWnd.left =
								rcBarWnd.right -
								m_pExtBarSrc->m_sizeDockedV.cx;
							rcBarWnd.top = rcCircle0.top;
							rcBarWnd.bottom = rcCircle0.bottom;
						} // if( rcFrameAnalyze.PtInRect(ptTrack) )
					} // case AFX_IDW_DOCKBAR_RIGHT
					break;
#ifdef _DEBUG
					default:
						ASSERT( FALSE );
					break;
#endif // _DEBUG
					} // switch( nDockBarID )

					if( m_eCDT != __ECDT_FLOATED )
						break;
				} // for( nDockSide = 0; nDockSide < 4;  nDockSide++ )
			} // if we have sense analyze docking into circle 0 inner part/circle 1 outer part

			if( m_eCDT == __ECDT_FLOATED )
			{ // try calc by most inner dockbars

				CRect rcFrameTest( rcFrameWnd );
				rcFrameTest.InflateRect( nRdpInner, nRdpInner );
				if( rcFrameTest.PtInRect( ptTrack ) )
				{
					
					CRect rcFrameInner;
					pFrame->GetClientRect( &rcFrameInner );
					RepositionBarsEx(
						pFrame,
						0,
						0xffff,
						AFX_IDW_PANE_FIRST,
						CWnd::reposQuery,
						rcFrameInner,
						rcFrameInner
						);
//					pFrame->RepositionBars(
//						0,
//						0xffff,
//						AFX_IDW_PANE_FIRST,
//						CWnd::reposQuery,
//						rcFrameInner,
//						rcFrameInner
//						);
					pFrame->ClientToScreen( &rcFrameInner );

					if( rcFrameInner.left < rcFrameInner.right
						&& rcFrameInner.top < rcFrameInner.bottom
						&& rcFrameInner.PtInRect( ptTrack )
						)
					{ // if we have any space to dock into inner circle
						ASSERT( rcFrameWnd.left <= rcFrameInner.left );
						ASSERT( rcFrameWnd.top <= rcFrameInner.top );
						ASSERT( rcFrameWnd.right >= rcFrameInner.right );
						ASSERT( rcFrameWnd.bottom >= rcFrameInner.bottom );

						CSize _sizeMaxInner = rcFrameInner.Size();
						_sizeMaxInner.cx = ::MulDiv( _sizeMaxInner.cx, 3, 4 );
						_sizeMaxInner.cy = ::MulDiv( _sizeMaxInner.cy, 3, 4 );

						UINT nSrcBarCircleNo =
							((CExtDockBar *)m_pExtBarSrc->m_pDockBar)->_GetCircleNo();

						for( nDockSide = 0; nDockSide < 4;  nDockSide++ )
						{
							DWORD dwDockStyle =
								_dwDockBarMap[ nDockSide ] [ 1 ];
							if( (m_pExtBarSrc->m_dwDockStyle & dwDockStyle) == 0 )
								continue; // can not dock at this side
							UINT nDockBarID =
								_dwDockBarMap[ nDockSide ] [ 0 ];
							ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );

							CExtDockBar * pDockBar = vDockBars0[ nDockSide ];
							ASSERT_VALID( pDockBar );
							ASSERT_KINDOF( CExtDockBar, pDockBar );
							ASSERT( pDockBar->_GetCircleNo() == 0 );

							if( bShowContent && m_pExtBarSrc->IsDocked() )
							{
								ASSERT( m_pExtBarSrc->m_pDockBar != NULL );
								ASSERT_VALID( m_pExtBarSrc->m_pDockBar );
								ASSERT_KINDOF( CExtDockBar, m_pExtBarSrc->m_pDockBar );
								if( pDockBar == m_pExtBarSrc->m_pDockBar )
								{
									CRect rcDockBar;
									pDockBar->GetWindowRect( &rcDockBar );
									if( rcDockBar.PtInRect(ptTrack) )
									{
										m_eCDT = __ECDT_OWN_AREA;
										nDstMinHW = nSrcMinHW;
										nDstMinVH = nSrcMinVH;
										return;
									}
									continue;
								} // if( pDockBar == m_pExtBarSrc->m_pDockBar )
							} // if( bShowContent && m_pExtBarSrc->IsDocked() )

							pDockBar = pDockBar->_GetDockBarInnerTop();
							ASSERT_VALID( pDockBar );
							ASSERT_KINDOF( CExtDockBar, pDockBar );
							
							CRect rcFrameAnalyze( rcFrameInner );

							switch( nDockBarID )
							{
							case AFX_IDW_DOCKBAR_TOP:
							{
								rcFrameAnalyze.bottom =
									rcFrameAnalyze.top + nRdpMostInner;
								if( rcFrameAnalyze.PtInRect(ptTrack) )
								{
									m_eCDT = __ECDT_DOCKBAR_INNER_TOP;
									m_nCircleNo = pDockBar->_GetCircleNo();

									bool bDeepCircleTarget = false;
									if(	m_nCircleNo <= (nSrcBarCircleNo + 2) )
										bDeepCircleTarget = true;
									if( !bDeepCircleTarget )
									{
										CExtDockBar * pBarLeft =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_LEFT );
										ASSERT_VALID( pBarLeft );
										CExtDockBar * pBarRight =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_RIGHT );
										ASSERT_VALID( pBarRight );
										if( !pBarLeft->_CanBeSafeOptimized()
											|| !pBarLeft->_CanBeSafeOptimized()
											)
											bDeepCircleTarget = true;
									}
									if( bDeepCircleTarget )
										m_nCircleNo++;

									rcBarWnd.top = rcFrameInner.top;
									rcBarWnd.bottom =
										rcBarWnd.top
										+
										min(
											m_pExtBarSrc->m_sizeDockedH.cy,
											_sizeMaxInner.cy
											);
									rcBarWnd.left = rcFrameInner.left;
									rcBarWnd.right = rcFrameInner.right;
								} // if( rcFrameAnalyze.PtInRect(ptTrack) )
							}
							break; // case AFX_IDW_DOCKBAR_TOP
							case AFX_IDW_DOCKBAR_BOTTOM:
							{
								rcFrameAnalyze.top =
									rcFrameAnalyze.bottom - nRdpMostInner;
								if( rcFrameAnalyze.PtInRect(ptTrack) )
								{
									m_eCDT = __ECDT_DOCKBAR_INNER_BOTTOM;
									m_nCircleNo = pDockBar->_GetCircleNo();

									bool bDeepCircleTarget = false;
									if(	m_nCircleNo <= (nSrcBarCircleNo + 2) )
										bDeepCircleTarget = true;
									if( !bDeepCircleTarget )
									{
										CExtDockBar * pBarLeft =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_LEFT );
										ASSERT_VALID( pBarLeft );
										CExtDockBar * pBarRight =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_RIGHT );
										ASSERT_VALID( pBarRight );
										if( !pBarLeft->_CanBeSafeOptimized()
											|| !pBarLeft->_CanBeSafeOptimized()
											)
											bDeepCircleTarget = true;
									}
									if( bDeepCircleTarget )
										m_nCircleNo++;

									rcBarWnd.bottom = rcFrameInner.bottom;
									rcBarWnd.top =
										rcBarWnd.bottom
										-
										min(
											m_pExtBarSrc->m_sizeDockedH.cy,
											_sizeMaxInner.cy
											);
									rcBarWnd.left = rcFrameInner.left;
									rcBarWnd.right = rcFrameInner.right;
								} // if( rcFrameAnalyze.PtInRect(ptTrack) )
							}
							break; // case AFX_IDW_DOCKBAR_BOTTOM
							case AFX_IDW_DOCKBAR_LEFT:
							{
								rcFrameAnalyze.right =
									rcFrameAnalyze.left + nRdpMostInner;
								if( rcFrameAnalyze.PtInRect(ptTrack) )
								{
									m_eCDT = __ECDT_DOCKBAR_INNER_LEFT;
									m_nCircleNo = pDockBar->_GetCircleNo();

									bool bDeepCircleTarget = false;
									if(	m_nCircleNo <= (nSrcBarCircleNo + 2) )
										bDeepCircleTarget = true;
									if( !bDeepCircleTarget )
									{
										CExtDockBar * pBarTop =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_TOP );
										ASSERT_VALID( pBarTop );
										CExtDockBar * pBarBottom =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_BOTTOM );
										ASSERT_VALID( pBarBottom );
										if( !pBarTop->_CanBeSafeOptimized()
											|| !pBarBottom->_CanBeSafeOptimized()
											)
											bDeepCircleTarget = true;
									}
									if( bDeepCircleTarget )
										m_nCircleNo++;
									
									rcBarWnd.left = rcFrameInner.left;
									rcBarWnd.right =
										rcBarWnd.left
										+
										min(
											m_pExtBarSrc->m_sizeDockedV.cx,
											_sizeMaxInner.cx
											);
									rcBarWnd.top = rcFrameInner.top;
									rcBarWnd.bottom = rcFrameInner.bottom;
								} // if( rcFrameAnalyze.PtInRect(ptTrack) )
							}
							break; // case AFX_IDW_DOCKBAR_LEFT
							case AFX_IDW_DOCKBAR_RIGHT:
							{
								rcFrameAnalyze.left =
									rcFrameAnalyze.right - nRdpMostInner;
								if( rcFrameAnalyze.PtInRect(ptTrack) )
								{
									m_eCDT = __ECDT_DOCKBAR_INNER_RIGHT;
									m_nCircleNo = pDockBar->_GetCircleNo();

									bool bDeepCircleTarget = false;
									if(	m_nCircleNo <= (nSrcBarCircleNo + 2) )
										bDeepCircleTarget = true;
									if( !bDeepCircleTarget )
									{
										CExtDockBar * pBarTop =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_TOP );
										ASSERT_VALID( pBarTop );
										CExtDockBar * pBarBottom =
											pDockBar->_GetInCircle( AFX_IDW_DOCKBAR_BOTTOM );
										ASSERT_VALID( pBarBottom );
										if( !pBarTop->_CanBeSafeOptimized()
											|| !pBarBottom->_CanBeSafeOptimized()
											)
											bDeepCircleTarget = true;
									}
									if( bDeepCircleTarget )
										m_nCircleNo++;
									
									rcBarWnd.right = rcFrameInner.right;
									rcBarWnd.left =
										rcBarWnd.right
										-
										min(
											m_pExtBarSrc->m_sizeDockedV.cx,
											_sizeMaxInner.cx
											);
									rcBarWnd.top = rcFrameInner.top;
									rcBarWnd.bottom = rcFrameInner.bottom;
								} // if( rcFrameAnalyze.PtInRect(ptTrack) )
							} // case AFX_IDW_DOCKBAR_RIGHT
							break;
#ifdef _DEBUG
							default:
								ASSERT( FALSE );
							break;
#endif // _DEBUG
							} // switch( nDockBarID )

							if( m_eCDT != __ECDT_FLOATED )
								break;
						} // for( nDockSide = 0; nDockSide < 4;  nDockSide++ )
					} // if we have any space to dock into inner circle
				} // if( rcFrameTest.PtInRect( ptTrack ) )
			} // try calc by most inner dockbars
		} // if( bCanDockToInnerCircles && m_eCDT == __ECDT_FLOATED )

		if( m_eCDT == __ECDT_FLOATED )
		{ // try calc by other bar
		
			ExtControlBarVector_t vBars;
			m_pExtBarSrc->_GetFrameControlBars(
				vBars,
				(DWORD)(
					__GFECB_NONFIXED_NONMIN //|__GFECB_FIXED
						|__GFECB_DOCKED
						|__GFECB_VIS_ALL
						|__GFECB_BYPT_ONLY_WND
					),
				ptTrack
				);
			INT nCountOfBars = vBars.GetSize();
			ASSERT( nCountOfBars >= 0 && nCountOfBars <= 1 );

			if( nCountOfBars == 1 )
			{
				m_pExtBarDst = vBars[0];
				//ExtBarDstSet( vBars[0] );
				ASSERT_VALID( m_pExtBarDst );
				m_pExtBarDst->GetWindowRect( &rcBarWnd );
				if( m_pExtBarDst == m_pExtBarSrc )
				{
					m_eCDT = __ECDT_OWN_AREA;
					
					nDstMinHW = nSrcMinHW;
					nDstMinVH = nSrcMinVH;
				} // if( m_pExtBarDst == m_pExtBarSrc )
				else if( !bShowContent )
				{
					m_eCDT = __ECDT_FLOATED;

					bool bCanAnalyzeInnerParts = true;
					if( !bCanDockToInnerCircles )
					{
						ASSERT_VALID( m_pExtBarDst->m_pDockBar );
						CExtDockBar * pDockBarDst =
							STATIC_DOWNCAST( CExtDockBar, m_pExtBarDst->m_pDockBar );
						ASSERT_KINDOF( CExtDockBar, pDockBarDst );
						if( pDockBarDst->_GetCircleNo() > 0 )
							bCanAnalyzeInnerParts = false;
					}

					if( bCanAnalyzeInnerParts && m_pExtBarDst != NULL )
					{
						nDstMinHW = m_pExtBarDst->_CalcDesiredMinHW();
						nDstMinVH = m_pExtBarDst->_CalcDesiredMinVH();
						INT nWidth3   = nRdpInner; // rcBarWnd.Width();
						INT nHeight3  = nRdpInner; // rcBarWnd.Height();
//						INT nWidth2  = nWidth  / 2;
//						INT nHeight2 = nHeight / 2;
//						INT nWidth3  = nWidth  / 4;
//						INT nHeight3 = nHeight / 4;

						ASSERT_VALID( m_pExtBarDst );
						UINT nDockBarID =
							m_pExtBarDst->GetSafeDockBarDlgCtrlID();
						if( nDockBarID == AFX_IDW_DOCKBAR_FLOAT )
							nDockBarID = AFX_IDW_DOCKBAR_TOP;
						ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
						bool bHorz = 
							(	nDockBarID == AFX_IDW_DOCKBAR_TOP
							||	nDockBarID == AFX_IDW_DOCKBAR_BOTTOM
							) ? true : false;
						ASSERT( m_pExtBarDst->m_pDockBar != NULL );

//						CExtDockBar * pDockBar =
//							STATIC_DOWNCAST( CExtDockBar, m_pExtBarDst->m_pDockBar );
						CDockBar * pDockBar = m_pExtBarDst->m_pDockBar;

						ASSERT_VALID( pDockBar );

//						if( bShowContent && m_pExtBarSrc->IsDocked() )
//						{
//							ASSERT( m_pExtBarDst->IsDocked() );
//							ASSERT_VALID( m_pExtBarSrc->m_pDockBar );
//							if( m_pExtBarSrc->m_pDockBar == pDockBar )
//							{
//								CRect rcWndTargetBar, rcWndSrcBar;
//								m_pExtBarDst->GetWindowRect( &rcWndTargetBar );
//								m_pExtBarSrc->GetWindowRect( &rcWndSrcBar );
//								rcWndTargetBar.InflateRect( 2, 2 );
//								if( bHorz )
//								{
//									if(		abs(rcWndSrcBar.top-rcWndTargetBar.bottom) <= 2
//										||	abs(rcWndSrcBar.bottom-rcWndTargetBar.top) <= 2
//										)
//									{
//										rcWndSrcBar.InflateRect( 2, 0 );
//										if(		rcWndSrcBar.left < rcWndTargetBar.right
//												&& rcWndTargetBar.right < rcWndSrcBar.right
//											||
//												rcWndSrcBar.left < rcWndTargetBar.left
//												&& rcWndTargetBar.left < rcWndSrcBar.right
//											)
//										{
//											m_eCDT = __ECDT_OWN_AREA;
//											nDstMinHW = nSrcMinHW;
//											nDstMinVH = nSrcMinVH;
//											return;
//										}
//									}
//								} // if( bHorz )
//								else
//								{
//									if(		abs(rcWndSrcBar.left-rcWndTargetBar.right) <= 2
//										||	abs(rcWndSrcBar.right-rcWndTargetBar.left) <= 2
//										)
//									{
//										rcWndSrcBar.InflateRect( 0, 2 );
//										if(		rcWndSrcBar.top < rcWndTargetBar.bottom
//												&& rcWndTargetBar.bottom < rcWndSrcBar.bottom
//											||
//												rcWndSrcBar.top < rcWndTargetBar.top
//												&& rcWndTargetBar.top < rcWndSrcBar.bottom
//											)
//										{
//											m_eCDT = __ECDT_OWN_AREA;
//											nDstMinHW = nSrcMinHW;
//											nDstMinVH = nSrcMinVH;
//											return;
//										}
//									}
//								} // else from if( bHorz )
//							} // if( m_pExtBarSrc->m_pDockBar == pDockBar )
//						} // if( bShowContent && m_pExtBarSrc->IsDocked() )

						bool bEnableInnerDock = true;
						if( nDockBarID == AFX_IDW_DOCKBAR_LEFT
							&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_LEFT) == 0
							)
							bEnableInnerDock = false;
						if( nDockBarID == AFX_IDW_DOCKBAR_RIGHT
							&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_RIGHT) == 0
							)
							bEnableInnerDock = false;
						if( nDockBarID == AFX_IDW_DOCKBAR_TOP
							&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_TOP) == 0
							)
							bEnableInnerDock = false;
						if( nDockBarID == AFX_IDW_DOCKBAR_BOTTOM
							&& (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_BOTTOM) == 0
							)
							bEnableInnerDock = false;
						
						if( bEnableInnerDock && (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_ANY) != 0 )
						{
							m_bDeepHalfSplit = false;
							if( !m_pExtBarDst->m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar))
								|| m_pExtBarDst->IsFloating()
								)
								m_bDeepHalfSplit = true;
							
							CRect rcWndDockBar;
							pDockBar->GetWindowRect( &rcWndDockBar );
							
							CRect rcPart( rcBarWnd );
							do
							{ // SINGLE LOOP
								
								INT nHalfHeight = nRdpInner*2; // rcBarWnd.Height()/2;
								if(	rcBarWnd.Width() >= nSrcMinHW
									&& nHalfHeight >= nSrcMinVH
									&& nHalfHeight >= nDstMinVH
									)
								{ // if can do half splitting
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
									BOOL bTabbedDest =
										m_pExtBarDst->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar))
										&& ( ((CExtDynTabControlBar*)m_pExtBarDst)->IsSwitcherVisible() )
										;

									INT nTabbedShift = 0;
									bool bCanDockToTabbedContainers =
										(	m_pExtBarSrc->_CanDockToTabbedContainers( m_pExtBarDst )
											&& m_pExtBarDst->_CanDockToTabbedContainers( m_pExtBarSrc )
											) ? true : false;
									if( (!bShowContent) && bCanDockToTabbedContainers )
									{
										rcPart = rcBarWnd;
										rcPart.bottom = rcPart.top + nHeight3;
										if(	rcPart.PtInRect(ptTrack) )
										{
											if( bTabbedDest )
												m_eCDT = __ECDT_TABBED_UPDATE;
											else
												m_eCDT = __ECDT_TABBED_NEW;
											rcPart = rcBarWnd;
											break;
										}
										nTabbedShift = nHeight3;
										rcPart.OffsetRect( 0, nTabbedShift );
									}
									else
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
									{
										rcPart = rcBarWnd;
										rcPart.bottom = rcPart.top + nHeight3;
									}
									
									if( rcPart.PtInRect(ptTrack) )
									{
										if( !bShowContent || !bHorz )
										{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
											rcPart.OffsetRect( 0, -nTabbedShift );
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
											CRect rcDeep(
												rcPart.left,
												rcPart.top+rcPart.Height()/2,
												rcPart.right,
												rcPart.bottom
												);
											m_bDeepHalfSplit =
												rcDeep.PtInRect(ptTrack) ? true : false;

											m_eCDT = __ECDT_INNER_TOP;
											rcPart.bottom = rcPart.top + rcBarWnd.Height()/2; // + nHeight2;
											
											if( bHorz && !m_bDeepHalfSplit )
											{
												rcPart.left = min( rcPart.left, rcWndDockBar.left );
												rcPart.right = max( rcPart.right, rcWndDockBar.right );
											}
											break;
										} // if( !bShowContent || !bHorz )
									} // if( rcPart.PtInRect(ptTrack) )

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
									rcPart = rcBarWnd;
									INT nBottomShift = 0;
									if( bCanDockToTabbedContainers && (!bShowContent)
										&& bTabbedDest
										)
									{
										CExtDynTabControlBar * pTabbedBar =
											(CExtDynTabControlBar *)m_pExtBarDst;
										CRect rcTabSwitcher;
										pTabbedBar->GetSwitcherWindowRect( rcTabSwitcher );
										if( rcTabSwitcher.PtInRect(ptTrack) )
										{
											m_eCDT = __ECDT_TABBED_REINDEX;
											ReindexPosCalc( ptTrack );
											break;
										}
										nBottomShift = rcTabSwitcher.Size().cy;
									}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

									rcPart = rcBarWnd;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
									rcPart.bottom -= nBottomShift;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
									rcPart.top = rcPart.bottom - nHeight3;
									if( rcPart.PtInRect(ptTrack) )
									{
										if( !bShowContent || !bHorz )
										{
											CRect rcDeep(
												rcPart.left,
												rcPart.top,
												rcPart.right,
												rcPart.bottom-rcPart.Height()/2
												);
											m_bDeepHalfSplit =
												rcDeep.PtInRect(ptTrack) ? true : false;

											m_eCDT = __ECDT_INNER_BOTTOM;
											rcPart.top = rcPart.bottom - rcBarWnd.Height()/2;// - nHeight2;
											
											if( bHorz && !m_bDeepHalfSplit )
											{
												rcPart.left = min( rcPart.left, rcWndDockBar.left );
												rcPart.right = max( rcPart.right, rcWndDockBar.right );
											}
											break;
										} // if( !bShowContent || !bHorz )
									} // if( rcPart.PtInRect(ptTrack) )
								} // if can do half splitting
								
								INT nHalfWidth = nRdpInner*2; // rcBarWnd.Width()/2;
								if( rcBarWnd.Height() >= nSrcMinVH
									&& nHalfWidth >= nSrcMinHW
									&& nHalfWidth >= nDstMinHW
									)
								{ // if can do half splitting
									rcPart = rcBarWnd;
									rcPart.right = rcPart.left + nWidth3;
									if( rcPart.PtInRect(ptTrack) )
									{
										if( !bShowContent || bHorz )
										{
											CRect rcDeep(
												rcPart.left+rcPart.Width()/2,
												rcPart.top,
												rcPart.right,
												rcPart.bottom
												);
											m_bDeepHalfSplit =
												rcDeep.PtInRect(ptTrack) ? true : false;

											m_eCDT = __ECDT_INNER_LEFT;
											rcPart.right = rcPart.left + rcBarWnd.Width()/2; // + nWidth2;
											
											if( !bHorz && !m_bDeepHalfSplit )
											{
												rcPart.top = min( rcPart.top, rcWndDockBar.top );
												rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom );
											}
											break;
										} // if( !bShowContent || bHorz )
									} // if( rcPart.PtInRect(ptTrack) )

									rcPart = rcBarWnd;
									rcPart.left = rcPart.right - nWidth3;
									if( rcPart.PtInRect(ptTrack) )
									{
										if( !bShowContent || bHorz )
										{
											CRect rcDeep(
												rcPart.left,
												rcPart.top,
												rcPart.right-rcPart.Width()/2,
												rcPart.bottom
												);
											m_bDeepHalfSplit =
												rcDeep.PtInRect(ptTrack) ? true : false;

											m_eCDT = __ECDT_INNER_RIGHT;
											rcPart.left = rcPart.right - rcBarWnd.Width()/2; // - nWidth2;
											
											if( !bHorz && !m_bDeepHalfSplit )
											{
												rcPart.top = min( rcPart.top, rcWndDockBar.top );
												rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom );
											}
											break;
										} // if( !bShowContent || bHorz )
									} // if( rcPart.PtInRect(ptTrack) )
								} // if can do half splitting

							} // SINGLE LOOP
							while( false );

							if( m_bDeepHalfSplit )
							{
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
								if(	m_bDeepHalfSplit
									&& (	m_eCDT == __ECDT_TABBED_NEW
											|| m_eCDT == __ECDT_TABBED_UPDATE
											|| m_eCDT == __ECDT_TABBED_REINDEX
											)
									)
									m_bDeepHalfSplit = false;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

//								if(	m_pExtBarDst->IsFloating()
//									|| m_pExtBarDst->IsSingleOnRow()
//									)
//									m_bDeepHalfSplit = false;
								if(	!m_pExtBarDst->IsFloating()
									&& m_pExtBarDst->IsSingleOnRow()
									)
									m_bDeepHalfSplit = false;
							} // if( m_bDeepHalfSplit )

							if( m_eCDT != __ECDT_FLOATED )
							{
								if( bHorz )
								{
									if( rcPart.Width() < nSrcMinHW )
										m_eCDT = __ECDT_FLOATED;
								} // if( bHorz )
								else
								{
									if( rcPart.Height() < nSrcMinVH )
										m_eCDT = __ECDT_FLOATED;
								} // else from if( bHorz )
								if( m_eCDT != __ECDT_FLOATED )
									rcBarWnd = rcPart;
							} // if( m_eCDT != __ECDT_FLOATED )
						
						} // if( bEnableInnerDock && (m_pExtBarSrc->m_dwDockStyle&CBRS_ALIGN_ANY) != 0 )
					} // if( bCanAnalyzeInnerParts && m_pExtBarDst != NULL )

				} // else if( !bShowContent ) -> else from if( m_pExtBarDst == m_pExtBarSrc )
			} // if( nCountOfBars == 1 )
		} // try calc by other bar

	} // if( !CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) ....

	if( m_eCDT == __ECDT_FLOATED )
	{
		//m_pExtBarDst = NULL;
		ExtBarDstSet( NULL );
		if( m_pExtBarSrc->IsFloating() )
		{
			CMiniDockFrameWnd * pMiniFrame = 
				DYNAMIC_DOWNCAST(
					CMiniDockFrameWnd,
					m_pExtBarSrc->GetDockingFrame()
					);
			ASSERT_VALID( pMiniFrame );
			pMiniFrame->GetWindowRect( &rcBarWnd );
			rcBarWnd.OffsetRect( -rcBarWnd.TopLeft() );

			rcBarWnd.OffsetRect(
				ptTrack - m_pExtBarSrc->m_ptHelperDragOffsetFloated
				);

			INT nCyGripper =
				min(
					m_pExtBarSrc->m_rcGrip.Width(),
					m_pExtBarSrc->m_rcGrip.Height()
					);
			rcBarWnd.OffsetRect( -nCyGripper, -nCyGripper/2 );

		} // if( m_pExtBarSrc->IsFloating() )
		else
		{
			rcBarWnd.SetRect(
				ptTrack,
				ptTrack + m_pExtBarSrc->m_sizeFloated
				);
			rcBarWnd.OffsetRect(
				- m_pExtBarSrc->m_ptHelperDragOffsetFloated
				);
		} // else from if( m_pExtBarSrc->IsFloating() )

		rcBarWnd.OffsetRect( rcFrameWnd.TopLeft() );
	} // if( m_eCDT == __ECDT_FLOATED )

	TrackRectSet( &rcBarWnd );
}

/*
#define __RECALC_FRAME_STEPS( __FRAME_PTR__ ) \
	(__FRAME_PTR__)->RecalcLayout(); \
	(__FRAME_PTR__)->SetWindowPos( \
		NULL, 0, 0, 0, 0, \
		SWP_NOSIZE|SWP_NOMOVE \
			|SWP_NOZORDER|SWP_NOACTIVATE \
			|SWP_FRAMECHANGED \
		); \
	(__FRAME_PTR__)->SendMessage( WM_NCPAINT )
*/

#define __RECALC_FRAME_STEPS( __FRAME_PTR__ ) \
	(__FRAME_PTR__)->RecalcLayout(); \
	(__FRAME_PTR__)->RedrawWindow( \
		NULL, NULL, \
		RDW_VALIDATE \
			|RDW_INVALIDATE|RDW_UPDATENOW \
			|RDW_ERASE|RDW_ERASENOW \
			|RDW_FRAME \
			|RDW_ALLCHILDREN \
		); 

void CExtControlBar::_DraggingDoDeepHalfSplit(
	CExtControlBar * pExtBar,
	UINT nHalfSplitID // one of AFX_IDW_DOCKBAR_... docked value
	)
{
	ASSERT_VALID( this );
	ASSERT_KINDOF( CExtControlBar, this );
	ASSERT( !IsFixedMode() );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT_KINDOF( CDockBar, m_pDockBar );
	
	ASSERT_VALID( pExtBar );
	ASSERT_KINDOF( CExtControlBar, pExtBar );
	ASSERT( !pExtBar->IsFixedMode() );

#ifdef _DEBUG
	if( pExtBar->m_pDockBar != NULL )	
	{
		ASSERT_VALID( pExtBar->m_pDockSite );
		ASSERT_VALID( pExtBar->m_pDockBar );
		ASSERT_KINDOF( CDockBar, pExtBar->m_pDockBar );
		ASSERT( pExtBar->m_pDockSite == m_pDockSite );
	}
#endif // _DEBUG
	
	ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nHalfSplitID );

CDockBar * pDockBarIn = m_pDockBar;
//		STATIC_DOWNCAST( CExtDockBar, m_pDockBar );
CDockBar * pDockBarOut = pExtBar->m_pDockBar;
//		STATIC_DOWNCAST( CExtDockBar, pExtBar->m_pDockBar );

CRect rcDyn;
	GetWindowRect( &rcDyn );
CRect rcIn( rcDyn );
CRect rcOut( rcDyn );
	pDockBarIn->ScreenToClient( &rcDyn );
UINT nNewID = AFX_IDW_DOCKBAR_TOP;
DWORD nBarStyle = CBRS_ALIGN_TOP;
	switch( nHalfSplitID )
	{
	case AFX_IDW_DOCKBAR_TOP:
		nNewID = AFX_IDW_DOCKBAR_LEFT;
		nBarStyle = CBRS_ALIGN_LEFT;
		rcIn.top += rcIn.Height()/2;
		rcOut.bottom = rcIn.top;
		break;
	case AFX_IDW_DOCKBAR_BOTTOM:
		nNewID = AFX_IDW_DOCKBAR_LEFT;
		nBarStyle = CBRS_ALIGN_LEFT;
		rcIn.bottom -= rcIn.Height()/2;
		rcOut.top = rcIn.bottom;
		break;
	case AFX_IDW_DOCKBAR_LEFT:
		rcIn.left += rcIn.Width()/2;
		rcOut.right = rcIn.left;
		break;
	case AFX_IDW_DOCKBAR_RIGHT:
		rcIn.right -= rcIn.Width()/2;
		rcOut.left = rcIn.right;
		break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		break;
#endif // _DEBUG
	} // switch( nHalfSplitID )

DWORD dwDynStyle =
		GetStyle()
		| (GetBarStyle() & CBRS_ALIGN_ANY)
		;
	dwDynStyle |= CBRS_BORDER_ANY;
	dwDynStyle &= ~( CBRS_FLOATING | CBRS_GRIPPER );
	dwDynStyle |= CBRS_TOOLTIPS;

CExtDynControlBar * pDynBar = new CExtDynControlBar;
	pDynBar->SetInitDesiredSizeHorizontal( m_sizeDockedH );
	pDynBar->SetInitDesiredSizeVertical( m_sizeDockedV );
	if(	!pDynBar->Create(
			NULL,
			m_pDockSite,
			nNewID,
			dwDynStyle
			)
		)
	{
		ASSERT( FALSE );
		return;
	}
	ASSERT( pDynBar->m_pWndDynDocker != NULL );
	ASSERT( pDynBar->m_pWndDynDocker->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(pDynBar->m_pWndDynDocker->GetSafeHwnd()) );
	pDynBar->m_pDockSite = m_pDockSite;
	pDynBar->m_pDockBar = m_pDockBar;
	pDynBar->EnableDocking( CBRS_ALIGN_ANY );
	ASSERT( pDynBar->m_pDockContext != NULL );
	ASSERT( pDynBar->m_pDockContext->m_pDockSite == m_pDockSite );

#ifdef _DEBUG
	if( pDockBarOut != NULL )
	{
		INT nPosOut = pDockBarOut->FindBar( pExtBar );
		ASSERT( nPosOut > 0 && nPosOut < pDockBarOut->m_arrBars.GetSize() );
	}
#endif // _DEBUG

INT nPosIn = pDockBarIn->FindBar( this );
	ASSERT( nPosIn > 0 && nPosIn < pDockBarIn->m_arrBars.GetSize() );
	pDockBarIn->m_arrBars[ nPosIn ] = pDynBar;

	pDynBar->m_pDockBar->m_bFloating = m_pDockBar->m_bFloating;
	m_pDockBar = pDynBar->m_pWndDynDocker;
	pExtBar->m_pDockBar = pDynBar->m_pWndDynDocker;

	m_pDockBar->m_bFloating = FALSE;
	
	if( pDynBar->m_pDockBar->m_bFloating )
		pDynBar->m_pDockBar->SetBarStyle(
			pDynBar->m_pDockBar->GetBarStyle()|CBRS_FLOATING
			);
	m_pDockBar->SetBarStyle(
		m_pDockBar->GetBarStyle() & (~CBRS_FLOATING)
		);

	ASSERT( !m_bUpdatingChain );
	ASSERT( !((CExtControlBar *)pDynBar)->m_bUpdatingChain );
	ASSERT( !pExtBar->m_bUpdatingChain );
	m_bUpdatingChain = true;
	((CExtControlBar *)pDynBar)->m_bUpdatingChain = true;
	pExtBar->m_bUpdatingChain = true;

	SetBarStyle( GetBarStyle() & (~CBRS_FLOATING) );
	pExtBar->SetBarStyle( pExtBar->GetBarStyle() & (~CBRS_FLOATING) );
	
	pDynBar->SetParent( pDockBarIn );
	SetParent( pDynBar->m_pWndDynDocker );
	pExtBar->SetParent( pDynBar->m_pWndDynDocker );

//bool bOptimizeDockBarLayout = false;
//HWND hWndDockBarOut = pDockBarOut->GetSafeHwnd();
	if( pDockBarOut != NULL )
	{
		ASSERT( pDockBarOut->FindBar( pExtBar ) >= 0 );

		if( !pDockBarOut->m_bFloating
			&& pDockBarOut->IsKindOf(RUNTIME_CLASS(CExtDockBar))
			)
		{
			//bOptimizeDockBarLayout = true;
			VERIFY(
				((CExtDockBar *)pDockBarOut)->
					RemoveControlBar( pExtBar, -1, 0, false )
				);
		}
		else
		{
			VERIFY( pDockBarOut->RemoveControlBar(pExtBar) );
		}

	}

	if(		nHalfSplitID == AFX_IDW_DOCKBAR_TOP
		||	nHalfSplitID == AFX_IDW_DOCKBAR_LEFT
		)
	{
		pDynBar->m_pWndDynDocker->m_arrBars.Add( pExtBar );
		pDynBar->m_pWndDynDocker->m_arrBars.Add( this );
	}
	else
	{
		pDynBar->m_pWndDynDocker->m_arrBars.Add( this );
		pDynBar->m_pWndDynDocker->m_arrBars.Add( pExtBar );
	}
	pDynBar->m_pWndDynDocker->m_arrBars.Add( NULL );

	SetInitDesiredSizeHorizontal( rcIn.Size() );
	SetInitDesiredSizeVertical( rcIn.Size() );
	pExtBar->SetInitDesiredSizeHorizontal( rcOut.Size() );
	pExtBar->SetInitDesiredSizeVertical( rcOut.Size() );

//	if( bOptimizeDockBarLayout
//		&& ::IsWindow( hWndDockBarOut )
//		)
//	{
//		ASSERT_VALID( pDockBarOut );
//		ASSERT(
//			CWnd::FromHandlePermanent(hWndDockBarOut)
//				== pDockBarOut
//			);
//		((CExtDockBar *)pDockBarOut)->OnDynamicLayoutOptimize();
//	}

	pDynBar->ScreenToClient( &rcIn );
	pDynBar->ScreenToClient( &rcOut );
	MoveWindow( &rcIn, FALSE );
	pExtBar->MoveWindow( &rcOut, FALSE );

	pDynBar->SetInitDesiredSizeFloating( rcDyn.Size() );
	pDynBar->MoveWindow( &rcDyn, FALSE );

	ASSERT_VALID( pDynBar );
	ASSERT_VALID( pDynBar->m_pWndDynDocker );

	m_bUpdatingChain = false;
	((CExtControlBar *)pDynBar)->m_bUpdatingChain = false;
	pExtBar->m_bUpdatingChain = false;

//pDynBar->m_pWndDynDocker->OnDynamicLayoutUpdate();
	CExtDockBar::_OptimizeCircles( m_pDockSite );

//	pDynBar-> /*m_pDockBar->*/ SetWindowPos(
//		NULL, 0,0,0,0,
//			SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE
//			|SWP_NOZORDER|SWP_NOOWNERZORDER
//			|SWP_FRAMECHANGED
//		);

CFrameWnd * pFrame = pDynBar->GetParentFrame();
	if( pFrame != NULL
//		&&
//		pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd))
		)
	{
		ASSERT_VALID( pFrame );
		__RECALC_FRAME_STEPS(pFrame);
		pDynBar->OnRepositionSingleChild();
		pDynBar->OnNcAreaButtonsReinitialize();
		pDynBar->OnNcAreaButtonsReposition();
		
		// TO FIX:
		if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
			pFrame->DelayRecalcLayout();
	}
}

bool CExtControlBar::_DraggingApplyState(
	CExtControlBar::InternalDraggingState_t & _ds
	)
{
	if( _ds.IsEmpty() )
		return false;

CExtControlBar * pExtBarSrc = _ds.ExtBarSrcGet();
	ASSERT_VALID( pExtBarSrc );

	if( pExtBarSrc->IsFixedDockStyle() )
		return false;

CFrameWnd * pFrame =
		pExtBarSrc->_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
CFrameWnd * pOldFrame =
		pExtBarSrc->GetParentFrame();
	ASSERT_VALID( pOldFrame );
HWND hWndOldFrame = pOldFrame->GetSafeHwnd();

CExtControlBar * pExtBarDst = _ds.ExtBarDstGet();
ExtControlBarVector_t vBarsDstRow;
INT nCountOfDstRowBars = 0;
UINT nDstDockBarID = AFX_IDW_DOCKBAR_FLOAT;
CDockBar * pDstDockBar = NULL;
bool bDstHorzDockBar = true;
bool bDstFloatBar = false;
INT nDstBarIdx = -1;
	if( pExtBarDst != NULL )
	{
		ASSERT_VALID( pExtBarDst );
		pExtBarDst->_GetRowExtBars( vBarsDstRow );
		nCountOfDstRowBars = vBarsDstRow.GetSize();
		ASSERT( nCountOfDstRowBars >= 1 );
		nDstDockBarID = pExtBarDst->GetSafeDockBarDlgCtrlID();
		ASSERT_DOCKBAR_DLGCTRLID( nDstDockBarID );
		ASSERT( pExtBarDst->m_pDockBar != NULL );
		pDstDockBar = pExtBarDst->m_pDockBar;
//			STATIC_DOWNCAST(
//				CExtDockBar,
//				pExtBarDst->m_pDockBar
//				);
		ASSERT( pDstDockBar != NULL );
		ASSERT_VALID( pDstDockBar );
		if( nDstDockBarID == AFX_IDW_DOCKBAR_LEFT
			|| nDstDockBarID == AFX_IDW_DOCKBAR_RIGHT
			)
			bDstHorzDockBar = false;
		if( nDstDockBarID == AFX_IDW_DOCKBAR_FLOAT )
			bDstFloatBar = true;
		nDstBarIdx = pDstDockBar->FindBar( pExtBarDst );
		ASSERT( nDstBarIdx >= 0 );
	} // if( pExtBarDst != NULL )

bool bInitialMode = false;
	if( pExtBarSrc->m_pDockBar == NULL )
		bInitialMode = true;

CRect rcTrack( _ds.TrackRectGet() );
e_calc_dock_type_t eCDT = _ds.GetCDT();
	switch( eCDT )
	{
	case __ECDT_OWN_AREA:
		m_pDockSite->DelayRecalcLayout();
		return true;
	break;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	case __ECDT_TABBED_NEW:
	case __ECDT_TABBED_UPDATE:
	case __ECDT_TABBED_REINDEX:
	{
		if( pExtBarDst == NULL )
			return TRUE;
		ASSERT_VALID( pExtBarDst );
		ASSERT_VALID( pDstDockBar );
		VERIFY(
			pExtBarDst->DockControlBarIntoTabbedContainer(
				pExtBarSrc,
				_ds.ReindexPosGet(), //-1,
				NULL,
				true
				)
			);
		return true;
	}
	break;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	case __ECDT_FLOATED:
	{
		bool bWasFloating = true;
		if( !pExtBarSrc->IsFloating() )
		{
			bWasFloating = false;
			CExtControlBar * pUpSizeBar = (CExtControlBar *)
				pExtBarSrc->_GetNextControlBarInRow(
					pExtBarSrc, true, true, true
					);
			if( pUpSizeBar != NULL )
			{
				while( !pUpSizeBar->IsVisible() )
				{
					CExtControlBar * pUpSizeBar2 = (CExtControlBar *)
						_GetNextControlBarInRow(
							pUpSizeBar, true, true, true
							);
					if( pUpSizeBar2 == NULL )
						break;
					pUpSizeBar = pUpSizeBar2;
				}
			} // if( pUpSizeBar != NULL )
			if( pUpSizeBar == NULL )
			{
				pUpSizeBar = (CExtControlBar *)
					pExtBarSrc->_GetNextControlBarInRow(
						pExtBarSrc, false, true, true
						);
				if( pUpSizeBar != NULL )
				{
					while( !pUpSizeBar->IsVisible() )
					{
						CExtControlBar * pUpSizeBar2 = (CExtControlBar *)
							_GetNextControlBarInRow(
								pUpSizeBar, false, true, true
								);
						if( pUpSizeBar2 == NULL )
							break;
						pUpSizeBar = pUpSizeBar2;
					}
				} // if( pUpSizeBar != NULL )
			} // if( pUpSizeBar == NULL )
			if( pUpSizeBar != NULL )
			{
				CRect rcBarWnd;
				pExtBarSrc->GetWindowRect( &rcBarWnd );
				ASSERT( !rcBarWnd.IsRectEmpty() );
				bool bHorzUpsizing = pExtBarSrc->IsDockedHorizontally();
				LONG nUpSize = bHorzUpsizing
					? rcBarWnd.Width()
					: rcBarWnd.Height()
					;
				LONG & ref_nMetric = bHorzUpsizing
					? pUpSizeBar->m_sizeDockedH.cx
					: pUpSizeBar->m_sizeDockedV.cy;
				ref_nMetric += nUpSize;
			} // if( pUpSizeBar != NULL )
		} // if( !pExtBarSrc->IsFloating() )
		CRect rcFrame;
		pFrame->GetWindowRect( &rcFrame );
		CPoint ptFloat( -rcFrame.TopLeft() );
		ptFloat += rcTrack.TopLeft();
		pExtBarSrc->FloatControlBar( ptFloat );
//		if( !bWasFloating && !bInitialMode )
//			pFrame->RecalcLayout();

		ASSERT( m_pDockContext != NULL );
		m_pDockContext->m_ptMRUFloatPos = ptFloat;

		if( pExtBarSrc->_IsShowContentWhenDragging() )
			pExtBarSrc->RedrawWindow(
				NULL, NULL,
				RDW_INVALIDATE | RDW_UPDATENOW
					| RDW_ERASE | RDW_ERASENOW
					| RDW_ALLCHILDREN
				);
		if( !pExtBarSrc->_IsShowContentWhenDragging() )
		{
			ASSERT_VALID( pExtBarSrc->m_pDockSite );
			pExtBarSrc->m_pDockSite->RecalcLayout();
		}
		return true;
	} // case __ECDT_FLOATED
	case __ECDT_INNER_TOP:
	{
		if( pExtBarDst == NULL )
			return TRUE;
		ASSERT_VALID( pExtBarDst );
		ASSERT_VALID( pDstDockBar );
		ASSERT( nDstBarIdx >= 0 );
		if( bDstHorzDockBar || bDstFloatBar
			|| _ds.IsDeepHalfSplit()
			)
		{ // if full row or deep half split
			if( _ds.IsDeepHalfSplit() )
			{
				pExtBarDst->_DraggingDoDeepHalfSplit(
					pExtBarSrc,
					AFX_IDW_DOCKBAR_TOP
					);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // if( _ds.IsDeepHalfSplit() )
			else
			{
				for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				{
					CExtControlBar * pExtBar = vBarsDstRow[nBarIdx];
					ASSERT_VALID( pExtBar );
					if( pExtBar == pExtBarSrc )
						continue;
					if( pExtBar->IsFixedMode() )
						continue;
					pExtBar->m_sizeDockedH.cy = rcTrack.Height();
				} // for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				pExtBarSrc->m_sizeDockedH.cx = rcTrack.Width();
				pExtBarSrc->m_sizeDockedH.cy = rcTrack.Height();
				ASSERT_KINDOF( CExtDockBar, pDstDockBar );
				((CExtDockBar *)pDstDockBar)->
					_NewRowDockControlBar(
						pExtBarDst,
						pExtBarSrc,
						true
						);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // else from if( _ds.IsDeepHalfSplit() )
		} // if full row or deep half split
		else
		{ // if sizing bar half split
			pExtBarDst->m_sizeDockedV.cy = rcTrack.Height();
			pExtBarSrc->m_sizeDockedV.cy = rcTrack.Height();
			pExtBarSrc->m_sizeDockedV.cx = pExtBarDst->m_sizeDockedV.cx;
			ASSERT_KINDOF( CExtDockBar, pDstDockBar );
			((CExtDockBar *)pDstDockBar)->DockControlBar( pExtBarSrc, &rcTrack );
			ASSERT_VALID( pDstDockBar );
			if( !bInitialMode )
			{
				pFrame->RecalcLayout();
				if( pOldFrame != pFrame
					&& ::IsWindow( hWndOldFrame )
					)
				{
					ASSERT_VALID( pOldFrame );
					__RECALC_FRAME_STEPS( pOldFrame );
				}
				pExtBarSrc->SetWindowPos(NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
						|SWP_FRAMECHANGED
					);
			}
		} // if sizing bar half split
		return true;
	} // case __ECDT_INNER_TOP
	case __ECDT_INNER_BOTTOM:
	{
		if( pExtBarDst == NULL )
			return TRUE;
		ASSERT_VALID( pExtBarDst );
		ASSERT_VALID( pDstDockBar );
		ASSERT( nDstBarIdx >= 0 );
		if( bDstHorzDockBar || bDstFloatBar
			|| _ds.IsDeepHalfSplit()
			)
		{ // if full row or deep half split
			if( _ds.IsDeepHalfSplit() )
			{
				pExtBarDst->_DraggingDoDeepHalfSplit(
					pExtBarSrc,
					AFX_IDW_DOCKBAR_BOTTOM
					);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // if( _ds.IsDeepHalfSplit() )
			else
			{
				for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				{
					CExtControlBar * pExtBar = vBarsDstRow[nBarIdx];
					ASSERT_VALID( pExtBar );
					if( pExtBar == pExtBarSrc )
						continue;
					if( pExtBar->IsFixedMode() )
						continue;
					pExtBar->m_sizeDockedH.cy = rcTrack.Height();
				} // for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				pExtBarSrc->m_sizeDockedH.cx = rcTrack.Width();
				pExtBarSrc->m_sizeDockedH.cy = rcTrack.Height();
				ASSERT_KINDOF( CExtDockBar, pDstDockBar );
				((CExtDockBar *)pDstDockBar)->
					_NewRowDockControlBar(
						pExtBarDst,
						pExtBarSrc,
						false
						);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // else from if( _ds.IsDeepHalfSplit() )
		} // if full row or deep half split
		else
		{ // if sizing bar half split
			pExtBarDst->m_sizeDockedV.cy = rcTrack.Height();
			pExtBarSrc->m_sizeDockedV.cy = rcTrack.Height();
			pExtBarSrc->m_sizeDockedV.cx = pExtBarDst->m_sizeDockedV.cx;
			ASSERT_KINDOF( CExtDockBar, pDstDockBar );
			((CExtDockBar *)pDstDockBar)->DockControlBar( pExtBarSrc, &rcTrack );
			ASSERT_VALID( pDstDockBar );
			if( !bInitialMode )
			{
				pFrame->RecalcLayout();
				if( pOldFrame != pFrame
					&& ::IsWindow( hWndOldFrame )
					)
				{
					ASSERT_VALID( pOldFrame );
					__RECALC_FRAME_STEPS( pOldFrame );
				}
				pExtBarSrc->SetWindowPos(NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
						|SWP_FRAMECHANGED
					);
			}
		} // if sizing bar half split
		return true;
	} // case __ECDT_INNER_BOTTOM
	case __ECDT_INNER_LEFT:
	{
		if( pExtBarDst == NULL )
			return TRUE;
		ASSERT_VALID( pExtBarDst );
		ASSERT_VALID( pDstDockBar );
		ASSERT( nDstBarIdx >= 0 );
		if( (!bDstHorzDockBar) || bDstFloatBar
			|| _ds.IsDeepHalfSplit()
			)
		{ // if full row or deep half split
			if( _ds.IsDeepHalfSplit() )
			{
				pExtBarDst->_DraggingDoDeepHalfSplit(
					pExtBarSrc,
					AFX_IDW_DOCKBAR_LEFT
					);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // if( _ds.IsDeepHalfSplit() )
			else
			{
				for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				{
					CExtControlBar * pExtBar = vBarsDstRow[nBarIdx];
					ASSERT_VALID( pExtBar );
					if( pExtBar == pExtBarSrc )
						continue;
					if( pExtBar->IsFixedMode() )
						continue;
					pExtBar->m_sizeDockedV.cx = rcTrack.Width();
//if( pExtBar->IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar)) )
//	((CExtDockDynTabBar*)((CExtDynTabControlBar*)pExtBar)->m_pWndDynDocker)->_SyncTabbedChilds( true, false );
				} // for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				pExtBarSrc->m_sizeDockedV.cx = rcTrack.Width();
				pExtBarSrc->m_sizeDockedV.cy = rcTrack.Height();
				ASSERT_KINDOF( CExtDockBar, pDstDockBar );
				((CExtDockBar *)pDstDockBar)->
					_NewRowDockControlBar(
						pExtBarDst,
						pExtBarSrc,
						true
						);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // else from if( _ds.IsDeepHalfSplit() )
		} // if full row or deep half split
		else
		{ // if sizing bar half split
			pExtBarDst->m_sizeDockedH.cx = rcTrack.Width();
			pExtBarSrc->m_sizeDockedH.cx = rcTrack.Width();
			pExtBarSrc->m_sizeDockedH.cy = pExtBarDst->m_sizeDockedH.cy;
			ASSERT_KINDOF( CExtDockBar, pDstDockBar );
			((CExtDockBar *)pDstDockBar)->DockControlBar( pExtBarSrc, &rcTrack );
			ASSERT_VALID( pDstDockBar );
			if( !bInitialMode )
			{
				pFrame->RecalcLayout();
				if( pOldFrame != pFrame
					&& ::IsWindow( hWndOldFrame )
					)
				{
					ASSERT_VALID( pOldFrame );
					__RECALC_FRAME_STEPS( pOldFrame );
				}
				pExtBarSrc->SetWindowPos(NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
						|SWP_FRAMECHANGED
					);
			}
		} // if sizing bar half split
		return true;
	} // case __ECDT_INNER_LEFT
	case __ECDT_INNER_RIGHT:
	{
		if( pExtBarDst == NULL )
			return TRUE;
		ASSERT_VALID( pExtBarDst );
		ASSERT_VALID( pDstDockBar );
		ASSERT( nDstBarIdx >= 0 );
		if( (!bDstHorzDockBar) || bDstFloatBar
			|| _ds.IsDeepHalfSplit()
			)
		{ // if full row or deep half split
			if( _ds.IsDeepHalfSplit() )
			{
				pExtBarDst->_DraggingDoDeepHalfSplit(
					pExtBarSrc,
					AFX_IDW_DOCKBAR_RIGHT
					);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // if( _ds.IsDeepHalfSplit() )
			else
			{
				for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				{
					CExtControlBar * pExtBar = vBarsDstRow[nBarIdx];
					ASSERT_VALID( pExtBar );
					if( pExtBar == pExtBarSrc )
						continue;
					if( pExtBar->IsFixedMode() )
						continue;
					pExtBar->m_sizeDockedV.cx = rcTrack.Width();
				} // for( INT nBarIdx = 0; nBarIdx < nCountOfDstRowBars; nBarIdx++ )
				pExtBarSrc->m_sizeDockedV.cx = rcTrack.Width();
				pExtBarSrc->m_sizeDockedV.cy = rcTrack.Height();
				ASSERT_KINDOF( CExtDockBar, pDstDockBar );
				((CExtDockBar *)pDstDockBar)->
					_NewRowDockControlBar(
						pExtBarDst,
						pExtBarSrc,
						false
						);
				if( !bInitialMode )
				{
					pFrame->RecalcLayout();
					if( pOldFrame != pFrame
						&& ::IsWindow( hWndOldFrame )
						)
					{
						ASSERT_VALID( pOldFrame );
						__RECALC_FRAME_STEPS( pOldFrame );
					}
				}
			} // else from if( _ds.IsDeepHalfSplit() )
		} // if full row or deep half split
		else
		{ // if sizing bar half split
			pExtBarDst->m_sizeDockedH.cx = rcTrack.Width();
			pExtBarSrc->m_sizeDockedH.cx = rcTrack.Width();
			pExtBarSrc->m_sizeDockedH.cy = pExtBarDst->m_sizeDockedH.cy;
			ASSERT_KINDOF( CExtDockBar, pDstDockBar );
			((CExtDockBar *)pDstDockBar)->DockControlBar( pExtBarSrc, &rcTrack );
			ASSERT_VALID( pDstDockBar );
			if( !bInitialMode )
			{
				pFrame->RecalcLayout();
				if( pOldFrame != pFrame
					&& ::IsWindow( hWndOldFrame )
					)
				{
					ASSERT_VALID( pOldFrame );
					__RECALC_FRAME_STEPS( pOldFrame );
				}
				pExtBarSrc->SetWindowPos(NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
						|SWP_FRAMECHANGED
					);
			}
		} // if sizing bar half split
		return true;
	} // case __ECDT_INNER_RIGHT
	case __ECDT_DOCKBAR_INNER_TOP:
	case __ECDT_DOCKBAR_INNER_BOTTOM:
	case __ECDT_DOCKBAR_INNER_LEFT:
	case __ECDT_DOCKBAR_INNER_RIGHT:
		{
			ASSERT_VALID( pExtBarSrc );

//			pExtBarSrc->m_sizeDockedV.cy = MAXSHORT;

			if(		eCDT == __ECDT_DOCKBAR_INNER_TOP
				||	eCDT == __ECDT_DOCKBAR_INNER_BOTTOM
				)
			{
				pExtBarSrc->m_sizeDockedH.cy = rcTrack.Height();
				pExtBarSrc->m_sizeDockedH.cy = rcTrack.Height();
			}
			else
			{
				pExtBarSrc->m_sizeDockedV.cx = rcTrack.Width();
				pExtBarSrc->m_sizeDockedV.cx = rcTrack.Width();
			}


			UINT nDockBarID = AFX_IDW_DOCKBAR_TOP;
			if( eCDT == __ECDT_DOCKBAR_INNER_BOTTOM )
				nDockBarID = AFX_IDW_DOCKBAR_BOTTOM;
			else if( eCDT == __ECDT_DOCKBAR_INNER_LEFT )
				nDockBarID = AFX_IDW_DOCKBAR_LEFT;
			else if( eCDT == __ECDT_DOCKBAR_INNER_RIGHT )
				nDockBarID = AFX_IDW_DOCKBAR_RIGHT;
			CExtDockBar * pDockBar = (CExtDockBar *)
				pFrame->GetControlBar( nDockBarID );
			ASSERT_VALID( pDockBar );
			ASSERT_KINDOF( CExtDockBar, pDockBar );
			ASSERT( pDockBar->_GetCircleNo() == 0 );
			UINT nCircleNoRequired = _ds.GetCircleNo();
			if( nCircleNoRequired > 0 )
			{
				pDockBar = pDockBar->_GetBarByCircleNo( nCircleNoRequired );
				ASSERT_VALID( pDockBar );
				ASSERT_KINDOF( CExtDockBar, pDockBar );
				ASSERT( pDockBar->_GetCircleNo() == nCircleNoRequired );
				ASSERT( ((UINT)pDockBar->GetDlgCtrlID()) == nDockBarID );
			}
		
			HWND hWndSrc = pExtBarSrc->GetSafeHwnd();

			pDockBar->_InnerOuterDockControlBar( pExtBarSrc, true );
			ASSERT_VALID( pDockBar );
			if( !bInitialMode )
			{
				pFrame->RecalcLayout();
				if( pOldFrame != pFrame
					&& ::IsWindow( hWndOldFrame )
					)
				{
					ASSERT_VALID( pOldFrame );
					__RECALC_FRAME_STEPS( pOldFrame );
				}
			}

			if( ::IsWindow( hWndSrc ) )
			{
				ASSERT_VALID( pExtBarSrc );
				ASSERT(
					CWnd::FromHandlePermanent(hWndSrc)
						== pExtBarSrc
					);
				pExtBarSrc->SetWindowPos(
					NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
						|SWP_FRAMECHANGED
					);
			}

			return true;
		}
	case __ECDT_CIRCLE1_OUTER_TOP:
	case __ECDT_CIRCLE1_OUTER_BOTTOM:
	case __ECDT_CIRCLE1_OUTER_LEFT:
	case __ECDT_CIRCLE1_OUTER_RIGHT:
		{
			UINT nDockBarID = 0;
			bool bInjectCircle = false;
			switch( eCDT )
			{
			case __ECDT_CIRCLE1_OUTER_TOP:
				nDockBarID = AFX_IDW_DOCKBAR_TOP;
				break;
			case __ECDT_CIRCLE1_OUTER_BOTTOM:
				nDockBarID = AFX_IDW_DOCKBAR_BOTTOM;
				break;
			case __ECDT_CIRCLE1_OUTER_LEFT:
				nDockBarID = AFX_IDW_DOCKBAR_LEFT;
				bInjectCircle = true;
				break;
			case __ECDT_CIRCLE1_OUTER_RIGHT:
				nDockBarID = AFX_IDW_DOCKBAR_RIGHT;
				bInjectCircle = true;
				break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
				break;
#endif // _DEBUG
			} // switch( eCDT )

			CExtDockBar * pDockBar = (CExtDockBar *)
				pFrame->GetControlBar( nDockBarID );
			ASSERT_VALID( pDockBar );
			ASSERT_KINDOF( CExtDockBar, pDockBar );
			ASSERT( pDockBar->_GetCircleNo() == 0 );
			if( bInjectCircle )
				pDockBar->_InjectInnerCircle();
			pDockBar = pDockBar->_GetBarByCircleNo( 1 );
			ASSERT_VALID( pDockBar );
			ASSERT_KINDOF( CExtDockBar, pDockBar );
			ASSERT( pDockBar->_GetCircleNo() == 1 );

			HWND hWndSrc = pExtBarSrc->GetSafeHwnd();
			
			pDockBar->_InnerOuterDockControlBar( pExtBarSrc, false );
			ASSERT_VALID( pDockBar );
			if( !bInitialMode )
			{
				pFrame->RecalcLayout();
				if( pOldFrame != pFrame
					&& ::IsWindow( hWndOldFrame )
					)
				{
					ASSERT_VALID( pOldFrame );
					__RECALC_FRAME_STEPS( pOldFrame );
				}
			}

			if( ::IsWindow( hWndSrc ) )
			{
				ASSERT_VALID( pExtBarSrc );
				ASSERT(
					CWnd::FromHandlePermanent(hWndSrc)
						== pExtBarSrc
					);
				pExtBarSrc->SetWindowPos(
					NULL, 0, 0, 0, 0,
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
						|SWP_FRAMECHANGED
					);
			}
			
		}
		return true; // case __ECDT_CIRCLE1_OUTER_....
	} // switch( eCDT )
	ASSERT( FALSE );
	return false;
}

UINT CExtControlBar::GetDockBarCircleNo() const
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );

	ASSERT_VALID( m_pDockBar );
	ASSERT_KINDOF( CExtDockBar, m_pDockBar );
	
	return ((CExtDockBar *)m_pDockBar)->_GetCircleNo();
}

bool CExtControlBar::_CanDockToInnerCircles() const
{
	return true;
}

bool CExtControlBar::_CanDockToTabbedContainers(
	CExtControlBar * pDestBar
	) const
{
	ASSERT_VALID( this );
	pDestBar;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( _IsShowContentWhenDragging() )
		return false;
	return true;
#else
	return false;
#endif // else from (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
CExtDynTabControlBar * CExtControlBar::_GetNearestTabbedContainer()
{
	ASSERT_VALID( this );
	if( m_hWnd == NULL || (! ::IsWindow(m_hWnd) ) )
		return NULL;
CWnd * pWndParent = GetParent();
	if( !pWndParent->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar)) )
		return NULL;
CExtDynTabControlBar * pTabbedContainer =
		STATIC_DOWNCAST( CExtDynTabControlBar, pWndParent->GetParent() );
	return pTabbedContainer;
}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

// toggle docking state
void CExtControlBar::ToggleDocking()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );

	ASSERT_VALID( m_pDockBar );
#ifdef _DEBUG
	if( !IsFloating() )
	{
		ASSERT_KINDOF( CExtDockBar, m_pDockBar );
	}
#endif // _DEBUG
	ASSERT( m_pDockContext != NULL );

	ASSERT( m_pDockContext != NULL );
	
	if( IsFixedDockStyle() )
	{
		m_pDockContext->ToggleDocking();
		CFrameWnd * pFrame = _GetDockingFrameImpl();
		ASSERT_VALID( pFrame );
		pFrame->DelayRecalcLayout();
		_RecalcNcArea();
		return;
	}

	ASSERT( !IsFixedMode() );
	
//	m_pDockContext->ToggleDocking();
//	
//CFrameWnd * pFrame = _GetDockingFrameImpl();
//	ASSERT_VALID( pFrame );
//	pFrame->RecalcLayout();

UINT nDockBarID = GetSafeDockBarDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID( nDockBarID );
	if( nDockBarID == AFX_IDW_DOCKBAR_FLOAT )
	{
	} // if( nDockBarID == AFX_IDW_DOCKBAR_FLOAT )
	else
	{
		ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
		if( !GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
			FloatControlBar();
	} // else from if( nDockBarID == AFX_IDW_DOCKBAR_FLOAT )
}

CExtMiniDockFrameWnd * CExtControlBar::OnCreateFloatingFrame(
	DWORD dwStyle
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );
CExtMiniDockFrameWnd * pMiniFrame = new CExtMiniDockFrameWnd;
	if( !pMiniFrame->Create(
			m_pDockSite,
			dwStyle
			)
		)
	{
		ASSERT( FALSE );
		AfxThrowResourceException();
	}
	return pMiniFrame;
}

// float control bar into recent floated position
void CExtControlBar::FloatControlBar(
	DWORD dwStyle // = CBRS_ALIGN_TOP
	)
{
	FloatControlBar( m_ptFloatHelper, dwStyle );
}

// float control bar
void CExtControlBar::FloatControlBar(
	CPoint ptFloat,
	DWORD dwStyle // = CBRS_ALIGN_TOP
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );

#ifdef _DEBUG
bool bWasFloating = IsFloating();
#endif // _DEBUG

	// if the bar is already floating and the dock bar only contains this
	// bar and same orientation then move the window rather than recreating
	// the frame
	if( m_pDockBar != NULL )
	{
		CDockBar * pDockBar = m_pDockBar;
		ASSERT_KINDOF( CDockBar, pDockBar );
		if( pDockBar->m_bFloating
			&& pDockBar->GetDockedCount() == 1
//			&& (dwStyle & pDockBar->m_dwStyle & CBRS_ALIGN_ANY) != 0
			)
		{
			ASSERT( bWasFloating );

			CExtMiniDockFrameWnd * pDockFrame =
				(CExtMiniDockFrameWnd*) pDockBar->GetParent();
			ASSERT( pDockFrame != NULL );
			ASSERT_KINDOF( CExtMiniDockFrameWnd, pDockFrame );
			pDockFrame->SetWindowPos(
				NULL, ptFloat.x, ptFloat.y, 0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE
				);
			pDockFrame->RecalcLayout( TRUE );
			pDockFrame->UpdateWindow();

			m_ptFloatHelper = ptFloat;
			
			return;
		}
	} // if( m_pDockBar != NULL )

	ASSERT(
		(!bWasFloating)
		|| m_pDockBar == NULL // initial mode
		);

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
CExtDockDynTabBar * pTabbedDocker =
		DYNAMIC_DOWNCAST(
			CExtDockDynTabBar,
			GetParent()
			);
	if( pTabbedDocker != NULL )
	{
		CExtDynTabControlBar * pTabbedBar =
			STATIC_DOWNCAST(
				CExtDynTabControlBar,
				pTabbedDocker->GetParent()
				);
		pTabbedBar->RemoveSelFromSwitcher();
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

bool bShowMiniFrame =
		( (GetStyle() & WS_VISIBLE) != 0 ) ? true : false;

	if( m_dwStyle & CBRS_SIZE_DYNAMIC )
	{
		dwStyle |= CBRS_SIZE_DYNAMIC;
		if (dwStyle & CBRS_ORIENT_VERT)
		{
			dwStyle &= ~CBRS_ALIGN_ANY;
			dwStyle |= CBRS_ALIGN_TOP;
		}
	}

CExtDockBar * pDockBarOut = (CExtDockBar *)m_pDockBar;
bool bOptimizeDockBarLayout = false;
	if(		pDockBarOut != NULL
		&&	!pDockBarOut->m_bFloating
		&&	pDockBarOut->IsKindOf(RUNTIME_CLASS(CExtDockBar))
		)
		bOptimizeDockBarLayout = true;
HWND hWndDockBarOuter =
		(pDockBarOut != NULL)
			? pDockBarOut->GetSafeHwnd()
			: NULL
			;

CFrameWnd * pOldFrame = GetParentFrame();
HWND hWndOldFrame = NULL;
	if( pOldFrame != NULL )
	{
		ASSERT_VALID( pOldFrame );
		hWndOldFrame = pOldFrame->GetSafeHwnd();
	}

CExtMiniDockFrameWnd * pDockFrame =
		OnCreateFloatingFrame( dwStyle );
	ASSERT( pDockFrame != NULL );
	ASSERT_KINDOF( CExtMiniDockFrameWnd, pDockFrame );
	pDockFrame->SetWindowPos(
		NULL, ptFloat.x, ptFloat.y, 0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE
		);
	if( pDockFrame->m_hWndOwner == NULL )
		pDockFrame->m_hWndOwner = m_pDockSite->m_hWnd;

CDockBar * pDockBar = (CDockBar *)
		pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
	ASSERT( pDockBar != NULL );
	ASSERT_KINDOF( CDockBar, pDockBar );

	ASSERT( !m_bUpdatingChain );
	m_bUpdatingChain = true;

/*
	if( pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockBar)) )
*/
		((CExtDockBar *)pDockBar)->DockControlBar( this );
/*	else
	{
		CDockBar * pDockBarSrc = m_pDockBar;
		bool bOptimizeDockBarLayout = false;
		if( !((CExtDockBar *)pDockBarSrc)->m_bFloating )
			bOptimizeDockBarLayout = true;
		pDockBar->DockControlBar( this );
		if( bOptimizeDockBarLayout )
			((CExtDockBar *)pDockBarSrc)->OnDynamicLayoutOptimize();
	}
*/

	m_bUpdatingChain = false;

	pDockFrame->RecalcLayout( TRUE );
	if( bShowMiniFrame )
	{
		pDockFrame->ShowWindow( SW_SHOWNA );
		pDockFrame->UpdateWindow();
	}

	m_ptFloatHelper = ptFloat;

	if(	bOptimizeDockBarLayout
		&& ::IsWindow( hWndDockBarOuter )
		)
	{
		ASSERT_VALID( pDockBarOut );
		((CExtDockBar*)pDockBarOut)->OnDynamicLayoutOptimize();
	}

	if( pOldFrame != m_pDockSite
		&& ::IsWindow( hWndOldFrame )
		)
	{
		ASSERT_VALID( pOldFrame );
		__RECALC_FRAME_STEPS( pOldFrame );
	}

// temp
CExtDockBar::_RemovePlaceHolder( m_pDockSite, this );

	CExtDockBar::_OptimizeCircles( m_pDockSite );
	m_pDockSite->RecalcLayout();
	OnRepositionSingleChild();

}

// dock control bar to new row
bool CExtControlBar::DockControlBar(
	UINT nDockBarID,
	UINT nCircleNo,
	CFrameWnd * pDockSite, // = NULL // can be NULL only if bar already was at once docked
	bool bRecalcLayout // = true
	)
{
	ASSERT_VALID( this );
	ASSERT_KINDOF( CExtControlBar, this );
	if( pDockSite == NULL )
		pDockSite = m_pDockSite;
	ASSERT_VALID( pDockSite );
	ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );

CExtDockBar * pDockBar = 
		STATIC_DOWNCAST(
			CExtDockBar,
			pDockSite->GetControlBar( nDockBarID )
			);
	ASSERT_VALID( pDockBar );
	ASSERT( pDockBar->_GetCircleNo() == 0 );
	if( nCircleNo == 0 )
	{
		pDockBar->_InnerOuterDockControlBar(
			this,
			true
			);
		if( bRecalcLayout )
			pDockSite->RecalcLayout();
		return true;
	}

	ASSERT( !IsFixedMode() );
	pDockBar = pDockBar->_GetBarByCircleNo( nCircleNo );
	ASSERT_VALID( pDockBar );
	ASSERT( pDockBar->_GetCircleNo() == nCircleNo );
	pDockBar->_InnerOuterDockControlBar(
		this,
		true
		);
	if( bRecalcLayout )
		pDockSite->RecalcLayout();
	return true;
}

// dock control bar relatively to other control bar
bool CExtControlBar::DockControlBar(
	CExtControlBar * pBar,
	bool bHorzBarHalfSplit,
	bool bInnerHalfSplit, // = false
	CFrameWnd * pDockSite, // = NULL // can be NULL only if bar already was at once docked
	bool bRecalcLayout // = true
	)
{
	ASSERT_VALID( this );
	ASSERT_KINDOF( CExtControlBar, this );
	if( pDockSite == NULL )
		pDockSite = m_pDockSite;
	ASSERT_VALID( pDockSite );
	ASSERT_VALID( pBar );
	ASSERT(
		pBar->m_pDockSite == m_pDockSite
		|| pBar->m_pDockSite == NULL
		);

	ASSERT_VALID( m_pDockBar );
	ASSERT_KINDOF( CDockBar, m_pDockBar );
	ASSERT( !IsFixedMode() );
	ASSERT( !pBar->IsFixedMode() );

#ifdef _DEBUG
	if( pBar->m_pDockBar != NULL )
	{
		ASSERT_VALID( pBar->m_pDockBar );
		ASSERT_KINDOF( CExtDockBar, pBar->m_pDockBar );
	}
#endif // _DEBUG

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar)) )
	{
		CExtControlBar * pParentBar =
			STATIC_DOWNCAST(
				CExtControlBar,
				m_pDockBar->GetParent()
				);
		ASSERT_VALID( pParentBar );
		return
			pParentBar->
				DockControlBar(
					pBar,
					bHorzBarHalfSplit,
					bInnerHalfSplit,
					pDockSite,
					bRecalcLayout
					);
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

CRect rc;
	GetWindowRect( &rc );
//	rc.bottom = rc.top + rc.Height()/2;
//	rc.right = rc.left + rc.Width()/2;

InternalDraggingState_t _ds;
	_ds.ExtBarSrcSet( pBar );
	_ds.ExtBarDstSet( this );
	_ds.SetEmpty( false );
	_ds.TrackRectSet( rc );
//	pBar->m_sizeDockedH = m_sizeDockedH;
//	pBar->m_sizeDockedV = m_sizeDockedV;

UINT nOwnDockBarID = GetSafeDockBarDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID( nOwnDockBarID );
	switch( nOwnDockBarID )
	{
	case AFX_IDW_DOCKBAR_FLOAT:
		_ds.SetCDT(
			bHorzBarHalfSplit
				? (bInnerHalfSplit ? __ECDT_INNER_TOP : __ECDT_INNER_BOTTOM)
				: (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT)
				);
		_ds.SetDeepHalfSplit( true );
		break;
	case AFX_IDW_DOCKBAR_LEFT:
		_ds.SetCDT(
			bHorzBarHalfSplit
				? (bInnerHalfSplit ? __ECDT_INNER_TOP : __ECDT_INNER_BOTTOM)
				: (bInnerHalfSplit ? __ECDT_INNER_RIGHT : __ECDT_INNER_LEFT)
				);
		_ds.SetDeepHalfSplit( !bHorzBarHalfSplit );
		break;
	case AFX_IDW_DOCKBAR_RIGHT:
		_ds.SetCDT(
			bHorzBarHalfSplit
				? (bInnerHalfSplit ? __ECDT_INNER_TOP : __ECDT_INNER_BOTTOM)
				: (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT)
				);
		_ds.SetDeepHalfSplit( !bHorzBarHalfSplit );
		break;
	case AFX_IDW_DOCKBAR_TOP:
		_ds.SetCDT(
			bHorzBarHalfSplit
				? (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT)
				: (bInnerHalfSplit ? __ECDT_INNER_BOTTOM : __ECDT_INNER_TOP)
				);
		_ds.SetDeepHalfSplit( bHorzBarHalfSplit );
		break;
	case AFX_IDW_DOCKBAR_BOTTOM:
		_ds.SetCDT(
			bHorzBarHalfSplit
				? (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT)
				: (bInnerHalfSplit ? __ECDT_INNER_TOP: __ECDT_INNER_BOTTOM)
				);
		_ds.SetDeepHalfSplit( bHorzBarHalfSplit );
		break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		return false;
#endif // _DEBUG
	} // switch( nOwnDockBarID )

// tmp
//_ds.SetDeepHalfSplit( true );

bool bRetVal = _DraggingApplyState( _ds );
	
	if( bRecalcLayout )
		pDockSite->RecalcLayout();
	else
		pDockSite->DelayRecalcLayout();
	return bRetVal;
}

// dock control bar into tabbed container
bool CExtControlBar::DockControlBarIntoTabbedContainer(
	CExtControlBar * pBar,
	INT nIndex, // = -1, // append
	CFrameWnd * pDockSite, // = NULL // can be NULL only if bar already was at once docked
	bool bRecalcLayout // = true
	)
{

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	ASSERT_VALID( this );
	ASSERT_KINDOF( CExtControlBar, this );
	if( pDockSite == NULL )
		pDockSite = m_pDockSite;
	ASSERT_VALID( pDockSite );
	ASSERT_VALID( pBar );
	ASSERT(
		pBar->m_pDockSite == m_pDockSite
		|| pBar->m_pDockSite == NULL
		);
	ASSERT( pBar != this );

	ASSERT_VALID( m_pDockBar );
	ASSERT_KINDOF( CDockBar, m_pDockBar );

#ifdef _DEBUG
	if( pBar->m_pDockBar != NULL )
	{
		ASSERT( pBar->m_pDockSite == pDockSite );
		ASSERT_VALID( pBar->m_pDockBar );
		ASSERT_KINDOF( CDockBar, pBar->m_pDockBar );
	}
#endif // _DEBUG

	if( ( ! _CanDockToTabbedContainers( pBar ) )
		|| (! pBar->_CanDockToTabbedContainers( this ) )
		|| IsFixedMode()
		|| pBar->IsFixedMode()
		)
		return false;

	if( IsKindOf(RUNTIME_CLASS(CExtDynTabControlBar)) )
	{
		ASSERT( !m_bUpdatingChain );
		ASSERT( !pBar->m_bUpdatingChain );
		m_bUpdatingChain = true;
		pBar->m_bUpdatingChain = true;

		if( pBar->m_pDockBar != NULL )
			pBar->GetParentFrame()->DelayRecalcLayout();

		BOOL bInsertingOtherTabs =
			pBar->IsKindOf(
				RUNTIME_CLASS(CExtDynTabControlBar)
				);

		((CExtDynTabControlBar *)(this)) ->
			InsertBar( pBar, nIndex, false );

		if( bRecalcLayout )
		{
			GetParentFrame()->RecalcLayout();
			ASSERT_VALID( m_pDockBar );
			m_pDockBar->CalcFixedLayout( TRUE, TRUE );
		}
		else
			GetParentFrame()->DelayRecalcLayout();

		if( nIndex < 0 )
			nIndex = 
				((CExtDynTabControlBar *)(this)) ->
					GetSwitcherItemCount() - 1;

		m_pDockSite->ShowControlBar(
			((CExtDynTabControlBar *)(this)) ->
				GetBarAt( nIndex, true ) ,
			TRUE,
			TRUE
			);

		m_bUpdatingChain = false;
		pBar->m_bUpdatingChain = false;

		((CExtDynTabControlBar *)(this)) ->
			SetSwitcherSelection( nIndex, true, true );
		((CExtDynTabControlBar *)(this)) ->
			OnRepositionSingleChild();

		if( !bInsertingOtherTabs )
		{
			ASSERT_VALID( pBar );
			pBar->OnRepositionSingleChild();
		}

		if( bRecalcLayout )
			CExtDockBar::_OptimizeCircles( m_pDockSite );
		return true;
	}

	if( m_pDockBar != NULL )
	{
		ASSERT_VALID( m_pDockBar );
		if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar)) )
		{
			CExtDynTabControlBar * pDynTabbed =
				STATIC_DOWNCAST(
					CExtDynTabControlBar,
					m_pDockBar->GetParent()
					);
			ASSERT_VALID( pDynTabbed );
			return
				pDynTabbed->DockControlBarIntoTabbedContainer(
					pBar,
					nIndex,
					pDockSite,
					bRecalcLayout
					);
		}
	}


	ASSERT_VALID( this );
	ASSERT_KINDOF( CExtControlBar, this );
	ASSERT( !IsFixedMode() );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT_KINDOF( CDockBar, m_pDockBar );
	
CDockBar * pDockBarIn = m_pDockBar;
			//		STATIC_DOWNCAST( CExtDockBar, m_pDockBar );
CDockBar * pDockBarOut = pBar->m_pDockBar;
			//		STATIC_DOWNCAST( CExtDockBar, pBar->m_pDockBar );
bool bOptimizeOuterDockBarLayout = false;
HWND hWndDockBarOuter = NULL;
	if( pDockBarOut != NULL )
	{
		if( pDockBarOut->IsKindOf(RUNTIME_CLASS(CExtDockBar)) )
			bOptimizeOuterDockBarLayout = true;
		hWndDockBarOuter = pDockBarOut->GetSafeHwnd(); 
	}

CFrameWnd * pOldFrame = pBar->GetParentFrame();
HWND hWndOldFrame = pOldFrame->GetSafeHwnd();

CRect rcDyn;
	GetWindowRect( &rcDyn );
	m_pDockBar->ScreenToClient( &rcDyn );


DWORD dwDynStyle =
		GetStyle()
		| (GetBarStyle() & CBRS_ALIGN_ANY)
		;
	dwDynStyle |= CBRS_BORDER_ANY | CBRS_GRIPPER;
	dwDynStyle &= ~( CBRS_FLOATING );
	dwDynStyle |= CBRS_TOOLTIPS;

UINT nNewID = GetSafeDockBarDlgCtrlID();

CExtDynTabControlBar * pDynBar = NULL;
	pDockSite->SendMessage(
		g_nMsgCreateTabbedBar,
		WPARAM( &pDynBar )
		);
#ifdef _DEBUG
	if( pDynBar != NULL )
	{
		ASSERT_VALID( pDynBar );
		ASSERT_KINDOF( CExtDynTabControlBar, pDynBar );
	} // if( pDynBar != NULL )
#endif // _DEBUG
	if( pDynBar == NULL )
		pDynBar = new CExtDynTabControlBar;

//	pDynBar->SetInitDesiredSizeHorizontal( m_sizeDockedH );
//	pDynBar->SetInitDesiredSizeVertical( m_sizeDockedV );
	if(	!pDynBar->Create(
			NULL,
			m_pDockSite,
			nNewID,
			dwDynStyle
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT( pDynBar->m_pWndDynDocker != NULL );
	ASSERT( pDynBar->m_pWndDynDocker->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(pDynBar->m_pWndDynDocker->GetSafeHwnd()) );
	pDynBar->m_pDockSite = m_pDockSite;
	pDynBar->m_pDockBar = m_pDockBar;
	pDynBar->EnableDocking( CBRS_ALIGN_ANY );
	ASSERT( pDynBar->m_pDockContext != NULL );
	ASSERT( pDynBar->m_pDockContext->m_pDockSite == m_pDockSite );

//#ifdef _DEBUG
//INT nPosOut = pDockBarOut->FindBar( pBar );
//	ASSERT( nPosOut > 0 && nPosOut < pDockBarOut->m_arrBars.GetSize() );
//#endif // _DEBUG

INT nPosIn = pDockBarIn->FindBar( this );
	ASSERT( nPosIn > 0 && nPosIn < pDockBarIn->m_arrBars.GetSize() );
	pDockBarIn->m_arrBars[ nPosIn ] = pDynBar;

	pDynBar->m_pDockBar->m_bFloating = m_pDockBar->m_bFloating;
	m_pDockBar = pDynBar->m_pWndDynDocker;
//	pBar->m_pDockBar = pDynBar->m_pWndDynDocker;

	m_pDockBar->m_bFloating = FALSE;
	
	if( pDynBar->m_pDockBar->m_bFloating )
		pDynBar->m_pDockBar->SetBarStyle(
			pDynBar->m_pDockBar->GetBarStyle()|CBRS_FLOATING
			);
	m_pDockBar->SetBarStyle(
		m_pDockBar->GetBarStyle() & (~CBRS_FLOATING)
		);

	SetBarStyle( GetBarStyle() & (~CBRS_FLOATING) );

	ASSERT( !m_bUpdatingChain );
	ASSERT( !((CExtControlBar *)pDynBar)->m_bUpdatingChain );
	ASSERT( !pBar->m_bUpdatingChain );
	m_bUpdatingChain = true;
	((CExtControlBar *)pDynBar)->m_bUpdatingChain = true;
	pBar->m_bUpdatingChain = true;
	
	pDynBar->SetParent( pDockBarIn );
	SetParent( pDynBar->m_pWndDynDocker );

	pDynBar->m_pWndDynDocker->m_arrBars.Add( this );
	pDynBar->m_pWndDynDocker->m_arrBars.Add( NULL );

//	SetInitDesiredSizeHorizontal( rcDyn.Size() );
//	SetInitDesiredSizeVertical( rcDyn.Size() );
////	pBar->SetInitDesiredSizeHorizontal( rcDyn.Size() );
////	pBar->SetInitDesiredSizeVertical( rcDyn.Size() );

	((CExtControlBar *)pDynBar)->m_sizeDockedH = m_sizeDockedH;
	((CExtControlBar *)pDynBar)->m_sizeDockedV = m_sizeDockedV;
	((CExtControlBar *)pDynBar)->m_sizeFloated = m_sizeFloated;


CRect rcNew( rcDyn );
	rcNew.OffsetRect( -rcNew.TopLeft() );
	MoveWindow( &rcNew, FALSE );
//	pBar->MoveWindow( &rcNew, FALSE );

//	pDynBar->SetInitDesiredSizeFloating( rcDyn.Size() );
	pDynBar->MoveWindow( &rcDyn, FALSE );

	ASSERT_VALID( pDynBar );
	ASSERT_VALID( pDynBar->m_pWndDynDocker );

	pDynBar->InsertControlBar( this );
//	pDynBar->m_wndTabSwitcher.InsertControlBar( pBar );

	pDynBar->InsertBar( pBar, -1, true );

	m_bUpdatingChain = false;
	((CExtControlBar *)pDynBar)->m_bUpdatingChain = false;
	pBar->m_bUpdatingChain = false;

//pDynBar->m_pWndDynDocker->OnDynamicLayoutUpdate();
//	CExtDockBar::_OptimizeCircles( m_pDockSite );

CFrameWnd * pFrame = pDynBar->GetParentFrame();
	if( pFrame != NULL )
	{
		BOOL bMiniFrame =
			pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd));
		if(	bMiniFrame || bRecalcLayout )
		{
			pDynBar->OnNcAreaButtonsReinitialize();
			pDynBar->OnNcAreaButtonsReposition();
			__RECALC_FRAME_STEPS(pFrame);
			if( bMiniFrame )
			{
				__RECALC_FRAME_STEPS( m_pDockSite );
			}
			if( bRecalcLayout )
				pFrame->RecalcLayout();
		}
	}

	if( bRecalcLayout && ::IsWindow( hWndOldFrame ) )
	{
		ASSERT_VALID( pOldFrame );
		ASSERT(
			CWnd::FromHandlePermanent( hWndOldFrame )
				== pOldFrame
			);
		if( pOldFrame != pFrame )
			pOldFrame->RecalcLayout();
	}

	if(	bOptimizeOuterDockBarLayout
		&& ::IsWindow( hWndDockBarOuter )
		)
	{
		ASSERT_VALID( pDockBarOut );
		((CExtDockBar*)pDockBarOut)->OnDynamicLayoutOptimize();
	}

// temp
CExtDockBar::_RemovePlaceHolder( m_pDockSite, this );
CExtDockBar::_RemovePlaceHolder( m_pDockSite, pBar );


	CExtDockBar::_OptimizeCircles( m_pDockSite );

	pDynBar-> /*m_pDockBar->*/ SetWindowPos(
		NULL, 0,0,0,0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE
			|SWP_NOZORDER|SWP_NOOWNERZORDER
			|SWP_FRAMECHANGED
		);
	pDynBar->SetSwitcherSelection( 0, true, true );
	((CExtDockDynTabBar *)pDynBar->m_pWndDynDocker)->_RepositionTabbedChilds( true );

	return true;
#else

	pBar;
	nIndex;
	pDockSite;
	bRecalcLayout;
	ASSERT( FALSE ); // no implementation
	return false;

#endif // else from (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

// is bar in autohide mode
bool CExtControlBar::AutoHideModeGet() const
{
	ASSERT_VALID( this );

#ifdef _DEBUG
	if( m_bAutoHideMode )
	{
		ASSERT( !IsFixedMode() );
		ASSERT( !IsFixedDockStyle() );
	}
#endif // _DEBUG

	return m_bAutoHideMode;
}

// switch to/restore from autohide mode
bool CExtControlBar::AutoHideModeSet(
	bool bAutoHideMode,
	bool bAnimate,
	bool bRecalcLayout,
	bool bForceNoShow
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT_KINDOF( CExtDockBar, m_pDockBar );

	if( bAutoHideMode || m_bAutoHideMode )
	{
		if( IsFixedMode() || IsFixedDockStyle() )
		{
			ASSERT( FALSE );
			return false;
		}
	}

	if( IsFloating() )
	{
		ASSERT( FALSE );
		return false;
	}

	if( bAutoHideMode == m_bAutoHideMode )
		return true;

CExtDockBar * pDockBar =
		STATIC_DOWNCAST(
			CExtDockBar,
			m_pDockBar
			);
	if( pDockBar->IsKindOf( RUNTIME_CLASS(CExtDockDynTabBar) ) )
	{
		CExtDynTabControlBar * pTabbedBar =
			STATIC_DOWNCAST(
				CExtDynTabControlBar,
				m_pDockBar->GetParent()
				);
		bool bRetVal =
			pTabbedBar->AutoHideModeSet(
				bAutoHideMode,
				bAnimate,
				bRecalcLayout,
				bAutoHideMode ? true : bForceNoShow
				);
		ASSERT( bAutoHideMode == m_bAutoHideMode );
		return bRetVal;
	}
CExtDynAutoHideArea * pWndAutoHideArea =
		pDockBar->_GetAutoHideArea();
	if( pWndAutoHideArea == NULL )
	{
		// FrameInjectAutoHideAreas was not called
		ASSERT( FALSE );
		return false;
	}
	ASSERT_VALID( pWndAutoHideArea );

	if( bAutoHideMode )
	{
		if( IsVisible() )
		{
			m_pDockSite->ShowControlBar( this, FALSE, TRUE );
			ASSERT( !IsVisible() );
			m_pDockSite->DelayRecalcLayout();
		}
		pWndAutoHideArea->AppendControlBar(
			this,
			bForceNoShow
			);
	} // if( bAutoHideMode )
	else
	{
		ASSERT( !IsVisible() );
		pWndAutoHideArea->RemoveControlBar(
			this,
			bForceNoShow
			);
	} // else from if( bAutoHideMode )
	
	m_bAutoHideMode = bAutoHideMode;

	if( bRecalcLayout )
	{
		m_pDockSite->RecalcLayout();
		SetWindowPos(
			NULL, 0, 0, 0, 0,
			SWP_NOMOVE|SWP_NOSIZE
				|SWP_NOOWNERZORDER|SWP_NOZORDER
				|SWP_NOACTIVATE
				|SWP_FRAMECHANGED
			);
	} // if( bRecalcLayout )

	return true;
}

#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

void CExtControlBar::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CControlBar::OnShowWindow(bShow, nStatus);
	
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if( pSite != NULL )
		pSite->OnBarStateChanged( this );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if( nStatus != 0 )
		return;

	_UpdateVisibilityInChain();
}

bool CExtControlBar::FindHelpMode(
	CWnd * pWndStartSearch
	)
{
	ASSERT_VALID( pWndStartSearch );
	ASSERT( pWndStartSearch->GetSafeHwnd() != NULL && ::IsWindow(pWndStartSearch->GetSafeHwnd()) );
CFrameWnd * pFrame = DYNAMIC_DOWNCAST( CFrameWnd, pWndStartSearch );
	if( pFrame == NULL )
		pFrame = pWndStartSearch->GetParentFrame();
	for( ;	pFrame != NULL;
			pFrame = pFrame->GetParentFrame()
		)
	{
		if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
			continue;
		if( pFrame->m_bHelpMode )
			return true;
	}
	return false;
}

bool CExtControlBar::FindPrintPreviewMode(
	CFrameWnd * pFrame
	)
{
	ASSERT_VALID( pFrame );
CView * pView = pFrame->GetActiveView();
	if( pView != NULL
		&& pView->IsKindOf( RUNTIME_CLASS(CPreviewView) )
		)
		return true;
	return false;
}

bool CExtControlBar::IsDockSiteInPrintPreviewMode()
{
	ASSERT_VALID( this );
	if( m_pDockSite == NULL
		|| m_bPresubclassDialogMode
		)
		return false;
	return FindPrintPreviewMode( m_pDockSite );
}

void CExtControlBar::_UpdateVisibilityInChain()
{
	if( m_pDockBar == NULL && (!m_bPresubclassDialogMode) )
		return; // on destruction

	if( _DraggingGetBar() != NULL )
		return;

	if( IsFixedMode() || IsFixedDockStyle()
		|| m_pDockSite == NULL
		|| m_pDockBar == NULL
		|| IsDockSiteInPrintPreviewMode()
//		|| (	m_pDockBar->GetSafeHwnd() != NULL
//			&&	::IsWindow( m_pDockBar->GetSafeHwnd() )
//			&&	(!m_pDockBar->IsVisible())
//			)
		)
		return;

	ASSERT_VALID( m_pDockSite );

	if( m_bUpdatingChain )
		return;
	m_bUpdatingChain = true;

	_UpdateVisibilityInRow();

BOOL bVisible = IsVisible();
CExtDockDynBar * pDynDocker = (CExtDockDynBar *)m_pDockBar;
	if( pDynDocker != NULL
		&& pDynDocker->IsKindOf(RUNTIME_CLASS(CExtDockDynBar))
		)
	{ // update parent dynamic docker with new visiblity options
		ASSERT_KINDOF( CExtDockDynBar, pDynDocker );

		CExtDynControlBar * pDynBar =
			STATIC_DOWNCAST(
				CExtDynControlBar,
				pDynDocker->GetParent()
				);
		ASSERT_VALID( pDynBar );
		
		if( bVisible )
		{
			m_pDockSite->ShowControlBar( pDynBar, TRUE, TRUE );
//			pDynDocker->OnDynamicLayoutUpdate();
		} // if( bVisible )
		else
		{
			INT nDockedVisibleCount =  pDynDocker->GetDockedVisibleCount();
			if( nDockedVisibleCount == 0 )
			{
				m_pDockSite->ShowControlBar( pDynBar, FALSE, TRUE );
			} // if( nDockedVisibleCount == 0 )
		} // else from if( bVisible )
	} // update parent dynamic docker with new visiblity options

	if( bVisible )
	{
//		CFrameWnd * pFrameWnd = GetParentFrame();
//		if( pFrameWnd != NULL
//			&&
//			pFrameWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd))
//			)
//		{
//			pFrameWnd->RecalcLayout();
//		}
	} // if( bVisible )
	else
	{ // else from if( bVisible ) - update child invisibility options
		if( IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
		{
			CExtDockDynBar * pInnerDynDocker =
				((CExtDynControlBar *) this)->m_pWndDynDocker;
			if( pInnerDynDocker != NULL )
			{
//				pInnerDynDocker->ShowAll( FALSE );

				for( INT nPos = 0; nPos < pInnerDynDocker->m_arrBars.GetSize(); nPos++ )
				{
					CControlBar * pBar = pInnerDynDocker->GetDockedControlBar( nPos );
					if( pBar == NULL )
						continue;
					
					//CFrameWnd* pFrameWnd = pBar->GetDockingFrame();
					//pFrameWnd->ShowControlBar( pBar, FALSE, TRUE );
					pBar->DelayShow( FALSE );
					
					pBar->SetWindowPos(
						NULL, 0, 0, 0, 0,
						SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE
							|SWP_NOACTIVATE
							|SWP_HIDEWINDOW
							|SWP_NOSENDCHANGING
						);
				} // for( INT nPos = 0; nPos < pInnerDynDocker->m_arrBars.GetSize(); nPos++ )
			} // if( pInnerDynDocker != NULL )
		}
	} // else from if( bVisible ) - update child invisibility options

CFrameWnd * pParentFrame = GetParentFrame();
	ASSERT_VALID( pParentFrame );
	if( pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		pParentFrame->RecalcLayout();

	m_bUpdatingChain = false;
}

void CExtControlBar::_UpdateVisibilityInRow()
{
	if( m_pDockBar == NULL && (!m_bPresubclassDialogMode) )
		return; // on destruction

	if( IsFixedMode() || IsFixedDockStyle()
		|| m_pDockSite == NULL
		|| m_pDockBar == NULL
		)
		return;
	ASSERT_VALID( m_pDockSite );

	if( IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
		return;

	ASSERT_VALID( m_pDockBar );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
CExtDynTabControlBar * pTabbedBar = NULL;
	if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar)) )
	{
		pTabbedBar =
			STATIC_DOWNCAST(
				CExtDynTabControlBar,
				m_pDockBar->GetParent()
				);
		ASSERT_VALID( pTabbedBar );
		pTabbedBar->UpdateBarVisibility( this );
	} // if( m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar)) )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

BOOL bVisible = IsVisible();

	// recalc desired size in row after ShowWindow(SW_SHOW)
UINT nDockBarID = GetSafeDockBarDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID( nDockBarID );
	if( nDockBarID == AFX_IDW_DOCKBAR_FLOAT )
		return;
	ASSERT_DOCKBAR_DLGCTRLID_DOCKED( nDockBarID );
bool bHorz =
		(	nDockBarID == AFX_IDW_DOCKBAR_TOP
		||	nDockBarID == AFX_IDW_DOCKBAR_BOTTOM
		) ? true : false;

	if( !bVisible )
	{
		m_nDelayedRowUpdateMetric = bHorz
			? m_sizeDockedH.cx
			: m_sizeDockedV.cy
			;
		return;
	}
	if( m_nDelayedRowUpdateMetric == 0 )
		return;

long nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;
	m_nDelayedRowUpdateMetric = 0;


ExtControlBarVector_t vRow;
	_GetRowExtBars( vRow );
INT nCountInRow = vRow.GetSize();
	ASSERT( nCountInRow > 0 );
	if( nCountInRow == 1 )
		return;

	for( INT nBar = 0; nBar < nCountInRow; )
	{
		CExtControlBar * pBar = vRow[ nBar ];
		ASSERT_VALID( pBar );
		BOOL bBarIsVisible = pBar->IsVisible();
		bool bFixedBar = pBar->IsFixedMode();
		if( pBar == this
			|| (!bBarIsVisible)
			|| bFixedBar
			)
		{
			nCountInRow--;
			vRow.RemoveAt( nBar );
			continue;
		}

		LONG nRealBarExtent = bHorz
			? pBar->m_sizeDockedH.cx
			: pBar->m_sizeDockedV.cy
			;
		LONG nMinBarExtent = bHorz
			? pBar->_CalcDesiredMinHW()
			: pBar->_CalcDesiredMinVH()
			;
		if( nRealBarExtent <= nMinBarExtent )
		{
			nCountInRow--;
			vRow.RemoveAt( nBar );
			continue;
		}

		nBar ++;
	} // for( INT nBar = 0; nBar < nCountInRow; )
	if( nCountInRow == 0 )
		return;

LONG nExtentDesizred = nDelayedRowUpdateMetric;
	for( ; nExtentDesizred > 0 && nCountInRow > 0; )
	{
		for( nBar = 0; nBar < nCountInRow; )
		{
			CExtControlBar * pBar = vRow[ nBar ];
			ASSERT_VALID( pBar );

			LONG & nRealBarExtentRef = bHorz
				? pBar->m_sizeDockedH.cx
				: pBar->m_sizeDockedV.cy
				;
			LONG nMinBarExtent = bHorz
				? pBar->_CalcDesiredMinHW()
				: pBar->_CalcDesiredMinVH()
				;

			ASSERT( nRealBarExtentRef > nMinBarExtent );

			nRealBarExtentRef --;
			nExtentDesizred --;
			if( nExtentDesizred == 0 )
				break;
			if( nRealBarExtentRef == nMinBarExtent )
			{
				nCountInRow--;
				vRow.RemoveAt( nBar );
				continue;
			}
			nBar ++;
		} // for( nBar = 0; nBar < nCountInRow; )
	} // for( ; nExtentDesizred > 0 && nCountInRow > 0; )
}

bool CExtControlBar::g_bControlBarClassRegistered = false;

bool CExtControlBar::RegisterControlBarClass()
{
	if( g_bControlBarClassRegistered )
		return true;

WNDCLASS wndclass;
HINSTANCE hInst = AfxGetInstanceHandle();
	if( ! ::GetClassInfo(
			hInst,
			__EXT_CONTROLBAR_CLASS_NAME,
			&wndclass
			)
		)
	{
		// otherwise we need to register a new class
		wndclass.style = CS_GLOBALCLASS|CS_DBLCLKS;
		wndclass.lpfnWndProc = ::DefWindowProc;
		wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = ::LoadCursor(
			NULL, //hInst,
			IDC_ARROW
			);
		ASSERT( wndclass.hCursor != NULL );
		wndclass.hbrBackground = NULL; 
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = __EXT_CONTROLBAR_CLASS_NAME;
		if( !::AfxRegisterClass( &wndclass ) )
		{
			ASSERT( FALSE );
			//AfxThrowResourceException();
			return false;
		}
	}

	g_bControlBarClassRegistered = true;
	return true;
}

BOOL CExtControlBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ( !RegisterControlBarClass() )
		|| ( !CControlBar::PreCreateWindow(cs) )
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
	
	cs.lpszClass = __EXT_CONTROLBAR_CLASS_NAME;

	return TRUE;
}

void CExtControlBar::PreSubclassWindow() 
{
	CControlBar::PreSubclassWindow();

	// emulate for enabling update in user non-frame mode
	m_bInCustomModeUpdateCmdUI = false;

	if( !m_bPresubclassDialogMode )
	{ // if m_bPresubclassDialogMode was not set manually
		HWND hWndParent = ::GetParent( m_hWnd );
		if( hWndParent != NULL )
		{
			CWnd * pWndParent = CWnd::FromHandlePermanent( hWndParent );
			if( pWndParent != NULL )
			{
				if(		( ! pWndParent->IsKindOf(RUNTIME_CLASS(CControlBar)) )
					&&	( ! pWndParent->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
					)
				{
					// non-frame mode
					// may be we are inside dialog
					DWORD dwWndStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );
					m_dwStyle = dwWndStyle & CBRS_ALL;
					m_bPresubclassDialogMode = true;
				}
			} // if( pWndParent != NULL )
		} // if( hWndParent != NULL )
	} // if m_bPresubclassDialogMode was not set manually

	if( m_dwStyle & CBRS_TOOLTIPS )
		EnableToolTips();

}

#define ID_TIMER_WAIT   0xE000  // timer while waiting to show status
#define ID_TIMER_CHECK  0xE001  // timer to check for removal of status

BOOL CExtControlBar::PreTranslateMessage(MSG* pMsg) 
{
//	CControlBar::PreTranslateMessage(MSG* pMsg) 

	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	// allow tooltip messages to be filtered
	if( CWnd::PreTranslateMessage(pMsg) )
		return TRUE;

	CWnd * pOwner = GetOwner();

#if _MFC_VER < 0x700
UINT message = pMsg->message;
	// handle CBRS_FLYBY style (status bar flyby help)
	if(	(	(		m_dwStyle & CBRS_FLYBY)
				||	message == WM_LBUTTONDOWN
				||	message == WM_LBUTTONUP
				)
			&&
			(	(		message >= WM_MOUSEFIRST
					&&	message <= WM_MOUSELAST
					) ||
					(	message >= WM_NCMOUSEFIRST
					&& message <= WM_NCMOUSELAST
				)
			)
			&&
			(	(!		CExtToolControlBar::g_bMenuTracking
					||	CExtPopupMenuWnd::IsMenuTracking()
				)
			)
		 )
	{
//#if _MFC_VER < 0x700
		_AFX_THREAD_STATE * pThreadState =
			::AfxGetThreadState();
//#else
//		AFX_MODULE_THREAD_STATE * pThreadState =
//			//::AfxGetModuleThreadState();
//			//AfxGetModuleState()->m_thread.GetData();
//			::AfxGetThreadState()->m_pModuleState->m_thread.GetData();
//#endif

		// gather information about current mouse position
		CPoint point = pMsg->pt;
		ScreenToClient(&point);
		TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));
		ti.cbSize = sizeof(AFX_OLDTOOLINFO);
		int nHit = OnToolHitTest(point, &ti);
		if (ti.lpszText != LPSTR_TEXTCALLBACK)
			free(ti.lpszText);
		BOOL bNotButton =
			message == WM_LBUTTONDOWN && (ti.uFlags & TTF_NOTBUTTON);
		if (message != WM_LBUTTONDOWN && GetKeyState(VK_LBUTTON) < 0)
			nHit = pThreadState->m_nLastStatus;

		// update state of status bar
		if (nHit < 0 || bNotButton)
		{
			if (GetKeyState(VK_LBUTTON) >= 0 || bNotButton)
			{
				SetStatusText(-1);
				KillTimer(ID_TIMER_CHECK);
			}
		}
		else
		{
			if (message == WM_LBUTTONUP)
			{
				SetStatusText(-1);
				ResetTimer(ID_TIMER_CHECK, 200);
			}
			else
			{
				if ((m_nStateFlags & statusSet) || GetKeyState(VK_LBUTTON) < 0)
					SetStatusText(nHit);

				else if (nHit != pThreadState->m_nLastStatus)
					ResetTimer(ID_TIMER_WAIT, 300);
			}
		}
		pThreadState->m_nLastStatus = nHit;
	}
#endif

	// don't translate dialog messages when in Shift+F1 help mode
CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if( pFrameWnd != NULL && pFrameWnd->m_bHelpMode )
		return FALSE;

	if( !m_bPresubclassDialogMode )
	{

		// since 'IsDialogMessage' will eat frame window accelerators,
		//   we call all frame windows' PreTranslateMessage first
		while( pOwner != NULL )
		{
			HWND hWndOwner = pOwner->GetSafeHwnd();
			ASSERT( hWndOwner != NULL );
			if( !::IsWindow(hWndOwner) )
				break;
			if( CWnd::FromHandlePermanent(hWndOwner) == NULL )
				break;

			// allow owner & frames to translate before IsDialogMessage does
			if (pOwner->PreTranslateMessage(pMsg))
				return TRUE;

			// try parent frames until there are no parent frames
			pOwner = pOwner->GetParentFrame();
		}
	
	} // if( !m_bPresubclassDialogMode )

	// filter both messages to dialog and from children
	return PreTranslateInput(pMsg);

}

int CExtControlBar::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	) const
{
	ASSERT_VALID( this );

	if(		CExtToolControlBar::g_bMenuTracking
		||	CExtPopupMenuWnd::IsMenuTracking()
		)
		return -1;

CPoint ptScreen( point );
	ClientToScreen( &ptScreen );
CExtBarNcAreaButton * pBtn = NULL;
	( const_cast < CExtControlBar * > ( this ) ) ->
		NcButtons_HitTest( ptScreen, &pBtn );
	if( pBtn != NULL )
	{
		ASSERT_VALID( pBtn );
		return pBtn->OnToolHitTest( point, pTI );
	}

HWND hWndChild = ::GetWindow( m_hWnd, GW_CHILD );
	if( hWndChild != NULL )
	{
		CWnd * pWndChild = CWnd::FromHandlePermanent(hWndChild);
		if( pWndChild != NULL )
			return pWndChild->OnToolHitTest( point, pTI );
	}

	return -1; //CControlBar::OnToolHitTest( point, pTI );
}

HWND CExtControlBar::FindHwndNotSafeForMutualPaint(
	CWnd * pWndStartSearch
	)
{
	ASSERT_VALID( pWndStartSearch );

	if( ( pWndStartSearch->GetStyle() & WS_VISIBLE ) == 0 )
		return NULL;

CFrameWnd * pFrame = DockSiteFromWnd( pWndStartSearch );
	if( pFrame != NULL )
	{
		ASSERT_VALID( pFrame );
		if( ( pFrame->GetStyle() & WS_VISIBLE ) == 0 )
			return NULL;
//#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
//		for(	HWND hWnd =
//					::GetWindow(
//						pFrame->GetSafeHwnd(),
//						GW_CHILD
//						);
//				hWnd != NULL;
//				hWnd =
//					::GetWindow(
//						hWnd,
//						GW_HWNDNEXT
//						)
//				)
//		{
//			CWnd * pWnd = CWnd::FromHandlePermanent( hWnd );
//			if( pWnd == NULL )
//				continue;
//			CExtDynAutoHideSlider * pSlider =
//				DYNAMIC_DOWNCAST( CExtDynAutoHideSlider, pWnd );
//			if( pSlider != NULL )
//			{
//				if( (pSlider->GetStyle() & WS_VISIBLE) == 0 )
//					break;
//				if( pSlider->m_nResizingMode != HTCLIENT )
//					return hWnd;
//			}
//		}
//#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	} // if( pFrame != NULL )


HWND hWndRetVal = NULL;
CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
CExtControlBar * pBar = _DraggingGetBar();
	if( pBar != NULL )
	{
		if( !pBar->_IsShowContentWhenDragging() )
			hWndRetVal = pBar->GetSafeHwnd();
	} // if( pBar != NULL )
	else
	{
		int nCountOfBars = g_AllBars.GetSize();
		for( int nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			pBar = g_AllBars[nBar];
			ASSERT_KINDOF( CExtControlBar, pBar );
			if( pBar->m_bRowRecalcing
				&& ( !pBar->_IsShowContentWhenRowRecalcing() )
				)
			{
				hWndRetVal = pBar->GetSafeHwnd();
				break;
			}
			if( pBar->m_bRowResizing
				&& ( !pBar->_IsShowContentWhenRowResizing() )
				)
			{
				hWndRetVal = pBar->GetSafeHwnd();
				break;
			}
		} // for( int nBar = 0; nBar < nCountOfBars; nBar++ )
	} // else from if( pBar != NULL )
	_slCsCB.Unlock();

	return hWndRetVal;
}

CFrameWnd * CExtControlBar::DockSiteFromWnd(
	CWnd * pWndStartSearch
	)
{
	ASSERT_VALID( pWndStartSearch );
CFrameWnd * pFrame =
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			pWndStartSearch
			);
	if( pFrame != NULL )
		return pFrame;
	pFrame = pWndStartSearch->GetParentFrame();
	if( pFrame == NULL )
		return NULL;
	ASSERT_VALID( pFrame );
	if( ! pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		return pFrame;
	pFrame = pFrame->GetParentFrame();
	if( pFrame == NULL )
		return NULL;
	ASSERT_VALID( pFrame );
	ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) );
	return pFrame;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)

void CExtControlBar::OnCustomizeModeEnter()
{
	ASSERT_VALID( this );
	if( IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
		return;
CWnd * pWndChild = GetWindow( GW_CHILD );
	if( pWndChild->GetSafeHwnd() != NULL )
		pWndChild->EnableWindow( FALSE );
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	else if( AutoHideModeGet() )
	{
		ASSERT_VALID( m_pDockBar );
		ASSERT_KINDOF( CExtDockBar, m_pDockBar );
		CExtDynAutoHideArea * pWndAutoHideArea =
			((CExtDockBar*)m_pDockBar)->_GetAutoHideArea();
		if(		pWndAutoHideArea != NULL
			&&	pWndAutoHideArea->m_pWndSlider != NULL
			)
		{
			ASSERT_VALID( pWndAutoHideArea );
			ASSERT_VALID( pWndAutoHideArea->m_pWndSlider );
			pWndChild = pWndAutoHideArea->m_pWndSlider->GetWindow( GW_CHILD );
			if( pWndChild->GetSafeHwnd() != NULL )
				pWndChild->EnableWindow( FALSE );
		} // if( pWndAutoHideArea != NULL ...
	} // else if( AutoHideModeGet() )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
}

void CExtControlBar::OnCustomizeModeLeave()
{
	ASSERT_VALID( this );
	if( IsKindOf(RUNTIME_CLASS(CExtDynControlBar)) )
		return;
CWnd * pWndChild = GetWindow( GW_CHILD );
	if( pWndChild->GetSafeHwnd() != NULL )
		pWndChild->EnableWindow( TRUE );
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	else if( AutoHideModeGet() )
	{
		ASSERT_VALID( m_pDockBar );
		ASSERT_KINDOF( CExtDockBar, m_pDockBar );
		CExtDynAutoHideArea * pWndAutoHideArea =
			((CExtDockBar*)m_pDockBar)->_GetAutoHideArea();
		if(		pWndAutoHideArea != NULL
			&&	pWndAutoHideArea->m_pWndSlider != NULL
			)
		{
			ASSERT_VALID( pWndAutoHideArea );
			ASSERT_VALID( pWndAutoHideArea->m_pWndSlider );
			pWndChild = pWndAutoHideArea->m_pWndSlider->GetWindow( GW_CHILD );
			if( pWndChild->GetSafeHwnd() != NULL )
				pWndChild->EnableWindow( TRUE );
		} // if( pWndAutoHideArea != NULL ...
	} // else if( AutoHideModeGet() )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
}

void CExtControlBar::OnCustomizeListBoxUpdate( CExtCustomizeToolbarsListBox * pWndList )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pWndList );
	if( !m_bVisibleInCustomizeListBox )
		return;
	pWndList->AddBar( this );
}

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

bool CExtControlBar::_IsDockSiteCustomizeMode(
	CWnd * pWndStartSearch
	)
{
	ASSERT_VALID( pWndStartSearch );
	ASSERT( pWndStartSearch->GetSafeHwnd() != NULL && ::IsWindow(pWndStartSearch->GetSafeHwnd()) );
	pWndStartSearch;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite(
			pWndStartSearch->GetSafeHwnd()
			);
	if( pSite != NULL )
		return pSite->IsCustomizeMode();
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	return false;
}

bool CExtControlBar::_IsDockSiteCustomizeMode() const
{
	ASSERT_VALID( this );
	return
		_IsDockSiteCustomizeMode( (CWnd *)this );
}

void CExtControlBar::_ActivateOnClick()
{
	ASSERT_VALID( this );
HWND hWndOwn = GetSafeHwnd();
	if(		hWndOwn == NULL
		||	( ! ::IsWindow(hWndOwn) )
		)
		return;
HWND hWndActivate = NULL;
	if( m_bPresubclassDialogMode )
		hWndActivate = ::GetParent( hWndOwn );
	else
		hWndActivate = _GetDockingFrameImpl()->GetSafeHwnd();
	if( hWndActivate != NULL )
	{
		HWND hWndFocus = ::GetFocus();
		if(	hWndFocus == NULL )
			::SetFocus( hWndActivate );
		else if(	hWndActivate != hWndFocus
					|| ( !::IsChild(hWndActivate, hWndFocus) )
					)
		{
			if( ! CExtPopupMenuWnd::TestHoverEnabledFromActiveHWND(hWndOwn) )
				::SetFocus( hWndActivate );
		}
	}
	else
		::SetFocus( hWndOwn );
}

CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	QUERY_REPOSITION_CALC_EFFECT_DATA(
		SHORT nFlags,
		CWnd * pWndToReposChilds,
		UINT nIDFirst,
		UINT nIDLast,
		UINT nIDLeftOver,
		LPRECT lpRectParam,
		LPCRECT lpcRectClient,
		bool bStretch
		)
	: m_nFlags( nFlags )
	, m_pWndToReposChilds( pWndToReposChilds )
	, m_nIDFirst( nIDFirst )
	, m_nIDLast( nIDLast )
	, m_nIDLeftOver( nIDLeftOver )
	, m_lpRectParam( lpRectParam )
	, m_lpcRectClient( lpcRectClient )
	, m_bStretch( bStretch )
{
	ASSERT_VALID( m_pWndToReposChilds );
	ASSERT( m_pWndToReposChilds->GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( m_pWndToReposChilds->GetSafeHwnd() ) );

	ASSERT( ((int)__PRCF_WND_REPOS_DEFAULT) == ((int)CWnd::reposDefault) );
	ASSERT( ((int)__PRCF_WND_REPOS_QUERY) == ((int)CWnd::reposQuery) );
	ASSERT( ((int)__PRCF_WND_REPOS_EXTRA) == ((int)CWnd::reposExtra) );
}

CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	~QUERY_REPOSITION_CALC_EFFECT_DATA()
{
}

void CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	QueryTypeSet( SHORT nQueryType )
{
const SHORT nTest = SHORT(__PRCF_WND_REPOS_MASK);
	nQueryType &= nTest;
	m_nFlags &= ~nTest;
	m_nFlags |= nQueryType;
}

SHORT CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	QueryTypeGet() const
{
const SHORT nTest = SHORT(__PRCF_WND_REPOS_MASK);
SHORT nRetVal = SHORT( m_nFlags & nTest );
	return nRetVal;
}

bool CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	IsQueryReposDefault() const
{
bool bRetVal =
		( ((int)QueryTypeGet()) == ((int)CWnd::reposDefault) )
			? true : false;
	return bRetVal;
}

bool CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	IsQueryReposQuery() const
{
bool bRetVal =
		( ((int)QueryTypeGet()) == ((int)CWnd::reposQuery) )
			? true : false;
	return bRetVal;
}

bool CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	IsQueryReposExtra() const
{
bool bRetVal =
		( ((int)QueryTypeGet()) == ((int)CWnd::reposExtra) )
			? true : false;
	return bRetVal;
}

CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	operator WPARAM() const
{
	return ( (WPARAM) this );
}

void CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	ExcludeFromCenterSet(
		bool bExclude // = true
		)
{
const SHORT nTest = SHORT(__PRCF_EXCLUDE_FROM_CENTER);
	if( bExclude )
		m_nFlags |= nTest;
	else
		m_nFlags &= ~nTest;
}

bool CExtControlBar::QUERY_REPOSITION_CALC_EFFECT_DATA::
	ExcludeFromCenterGet() const
{
const SHORT nTest = SHORT(__PRCF_EXCLUDE_FROM_CENTER);
bool bRetVal = ( (m_nFlags&nTest) != 0 ) ? true : false;
	return bRetVal;
}

void CExtControlBar::RepositionBarsEx(
	CWnd * pWndToReposChilds,
	UINT nIDFirst,
	UINT nIDLast,
	UINT nIDLeftOver,
	UINT nFlag, // = reposDefault
	LPRECT lpRectParam, // = NULL
	LPCRECT lpRectClient, // = NULL
	BOOL bStretch // = TRUE
	)
{
	ASSERT_VALID( pWndToReposChilds );

	ASSERT( nFlag == 0 || nFlag == reposQuery || nFlag == reposExtra );

AFX_SIZEPARENTPARAMS layout;
HWND hWndLeftOver = NULL;

	layout.bStretch = bStretch;
	layout.sizeTotal.cx = layout.sizeTotal.cy = 0;
	if( lpRectClient != NULL )
		layout.rect = *lpRectClient;    // starting rect comes from parameter
	else
		pWndToReposChilds->GetClientRect(
			&layout.rect // starting rect comes from client rect
			);

	if( nFlag != 1 )
		layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
	else
		layout.hDWP = NULL; // not actually doing layout

	for(	HWND hWndChild = ::GetTopWindow( pWndToReposChilds->m_hWnd );
			hWndChild != NULL;
			hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT )
			)
	{
		UINT nIDC = _AfxGetDlgCtrlID(hWndChild);
		CWnd * pWnd = CWnd::FromHandlePermanent( hWndChild );
		if( nIDC == nIDLeftOver )
			hWndLeftOver = hWndChild;
		else if( pWnd != NULL )
		{
// (- v.2.23)
//#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
//			if( nFlag == 1
//				&& pWnd->IsKindOf( RUNTIME_CLASS(CExtTabMdiWnd) )
//				)
//				continue;
//#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
// (+ v.2.23)
			QUERY_REPOSITION_CALC_EFFECT_DATA _qrced(
				(SHORT)nFlag,
				pWndToReposChilds,
				nIDFirst,
				nIDLast,
				nIDLeftOver,
				lpRectParam,
				lpRectClient,
				bStretch ? true : false
				);
			pWnd->SendMessage(
				g_nMsgQueryRepositionCalcEffect,
				_qrced,
				0L
				);
			if( nFlag == 1 && _qrced.ExcludeFromCenterGet() )
			{
				ASSERT( _qrced.IsQueryReposQuery() );
				continue;
			}
			if(
				nIDC >= nIDFirst
				&& nIDC <= nIDLast
				)
				::SendMessage(
					hWndChild,
					WM_SIZEPARENT,
					0,
					(LPARAM)&layout
					);
		}
	}

	// if just getting the available rectangle, return it now...
	if( nFlag == 1 )
	{
		ASSERT(lpRectParam != NULL);
		if( bStretch )
			::CopyRect( lpRectParam, &layout.rect );
		else
		{
			lpRectParam->left = lpRectParam->top = 0;
			lpRectParam->right = layout.sizeTotal.cx;
			lpRectParam->bottom = layout.sizeTotal.cy;
		}
		return;
	}

	// the rest is the client size of the left-over pane
	if( nIDLeftOver != 0 && hWndLeftOver != NULL )
	{
		CWnd* pLeftOver = CWnd::FromHandle(hWndLeftOver);
		// allow extra space as specified by lpRectBorder
		if( nFlag == 2 )
		{
			ASSERT(lpRectParam != NULL);
			layout.rect.left += lpRectParam->left;
			layout.rect.top += lpRectParam->top;
			layout.rect.right -= lpRectParam->right;
			layout.rect.bottom -= lpRectParam->bottom;
		}
		// reposition the window
		pLeftOver->CalcWindowRect( &layout.rect );
		AfxRepositionWindow( &layout, hWndLeftOver, &layout.rect );
	}

	// move and resize all the windows at once!
	if( layout.hDWP == NULL
		|| !::EndDeferWindowPos( layout.hDWP )
		)
		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
}

void CExtControlBar::_PreSyncRowMetrics()
{
	ASSERT_VALID( this );
	if( IsVisible()
		|| IsFloating()
		)
		return;

	//m_nDelayedRowUpdateMetric = 0;

ExtControlBarVector_t vRow;
	_GetRowExtBars( vRow );
INT nCount = vRow.GetSize();
	for( INT i = 0; i < nCount; )
	{
		CExtControlBar * pBar = vRow[ i ];
		ASSERT_VALID( pBar );
		if( !pBar->IsVisible()
			|| pBar->IsFixedMode()
			|| pBar->IsFixedDockStyle()
			|| pBar == this
			)
		{
			vRow.RemoveAt( i );
			nCount--;
		}
		else
			i++;
	}
UINT nDockbarID = GetSafeDockBarDlgCtrlID();
	ASSERT_DOCKBAR_DLGCTRLID( nDockbarID );
bool bHorz =
		(nDockbarID == AFX_IDW_DOCKBAR_TOP || nDockbarID == AFX_IDW_DOCKBAR_BOTTOM)
			? true : false;
LONG nOwnMetric = bHorz ? m_sizeDockedH.cx : m_sizeDockedV.cy;
	if( nOwnMetric == 0 )
	{
		nOwnMetric = bHorz ? _CalcDesiredMinHW() : _CalcDesiredMinVH();
		(bHorz ? m_sizeDockedH.cx : m_sizeDockedV.cy) = nOwnMetric;
	}

LONG & nCrossMetricOwnRef = bHorz ? m_sizeDockedH.cy : m_sizeDockedV.cx;
LONG nCrossMetricOther = -1;

	for( i = 0; i < nCount && nOwnMetric > 0; )
	{
		CExtControlBar * pBar = vRow[ i ];
		//pBar->m_nDelayedRowUpdateMetric = 0;
		LONG & nRefBarMetric = bHorz ? pBar->m_sizeDockedH.cx : pBar->m_sizeDockedV.cy;
		LONG nBarMinMetric = bHorz ? pBar->_CalcDesiredMinHW() : pBar->_CalcDesiredMinVH();
		if( nCrossMetricOther <= 0 )
			nCrossMetricOther = bHorz ? pBar->m_sizeDockedH.cy : pBar->m_sizeDockedV.cx;
		if( nRefBarMetric <= nBarMinMetric )
		{
			vRow.RemoveAt( i );
			nCount--;
			if( nCount == 0 )
				break;
			continue;
		}
		nRefBarMetric --;
		nOwnMetric --;
		ASSERT( nOwnMetric >= 0 );
		if( nOwnMetric == 0 )
			break;
		i++;
		ASSERT( i <= nCount );
		if( i == nCount )
			i = 0;
	}

	if( nCrossMetricOther > 0 )
		nCrossMetricOwnRef = nCrossMetricOther;
}

void CExtControlBar::OnFrameBarCheckCmd(
	bool bResizableCheckmarks // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );

	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
	ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);

//	if(		bResizableCheckmarks // (- v.2.23)
//		||	IsFixedDockStyle()
//		)
bool bVisible = ((GetStyle() & WS_VISIBLE) != 0) ? true : false;
	if(		IsFixedDockStyle()
		||	( bResizableCheckmarks && bVisible ) // (+ v.2.23)
		)
	{
		m_pDockSite->ShowControlBar(
			this,
			bVisible ? FALSE : TRUE,
			FALSE
			);
		return;
	}

	ASSERT( !IsFixedDockStyle() );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( AutoHideModeGet() )
	{
		ASSERT_VALID( m_pDockBar );
		ASSERT_KINDOF( CExtDockBar, m_pDockBar );
		CExtDynAutoHideArea * pWndAutoHideArea =
			((CExtDockBar*)m_pDockBar)->_GetAutoHideArea();
		ASSERT_VALID( pWndAutoHideArea );
		ASSERT_VALID( pWndAutoHideArea->m_pWndSlider );
		if( pWndAutoHideArea->m_pWndSlider->m_bActiveState
			&& pWndAutoHideArea->m_pWndSlider->m_pBar == this
			)
			return;
		LONG nIndex = pWndAutoHideArea->FindControlBar( this );
		if( nIndex < 0 )
		{
			ASSERT( FALSE );
			return;
		}
		pWndAutoHideArea->SelectionSet( nIndex, true, true );
		ASSERT( pWndAutoHideArea->m_pWndSlider->m_bActiveState );
		ASSERT( pWndAutoHideArea->m_pWndSlider->m_pBar == this );
		ASSERT( pWndAutoHideArea->m_pWndSlider->GetStyle() & WS_VISIBLE );
		pWndAutoHideArea->m_pWndSlider->SetFocus();
		return;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	if( !bVisible )
	{
		m_pDockSite->ShowControlBar(this, TRUE, FALSE );
		CExtMiniDockFrameWnd * pMiniFrame =
			DYNAMIC_DOWNCAST(
				CExtMiniDockFrameWnd,
				GetParentFrame()
				);
		if( pMiniFrame != NULL )
		{
			_UpdateVisibilityInChain();
			pMiniFrame->RecalcLayout();
		}
	} // if( !bVisible )

	ASSERT_VALID( m_pDockBar );
//	ASSERT_KINDOF( CExtDockBar, m_pDockBar );

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( ((CExtDockBar*)m_pDockBar)->IsKindOf(RUNTIME_CLASS(CExtDockDynTabBar)) )
	{
		CExtDynTabControlBar * pTabbed = 
			STATIC_DOWNCAST(
				CExtDynTabControlBar,
				m_pDockBar->GetParent()
				);
		pTabbed->SelectControlBar( this );
		SetFocus();
		return;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

CFrameWnd * pParentFrame = GetParentFrame();
	if( pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		pParentFrame->BringWindowToTop();
	SetFocus();
}

void CExtControlBar::OnFrameBarCheckUpdate(
	CCmdUI * pCmdUI,
	bool bResizableCheckmarks // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );

	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
	ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);

	pCmdUI->Enable();

	if(		bResizableCheckmarks
		||	IsFixedDockStyle()
		)
	{
		pCmdUI->SetCheck(
			(GetStyle() & WS_VISIBLE) != 0
			);
		return;
	}
}

BOOL CExtControlBar::DoFrameBarCheckCmd(
	CFrameWnd * pFrame,
	UINT nBarID,
	bool bResizableCheckmarks // = false
	)
{
	ASSERT_VALID( pFrame );

CControlBar * pBar = pFrame->GetControlBar( nBarID );
	if( pBar != NULL )
	{
		if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			((CExtControlBar*)pBar)->
				OnFrameBarCheckCmd( bResizableCheckmarks );
		else
			pFrame->ShowControlBar(
				pBar,
				(pBar->GetStyle() & WS_VISIBLE) == 0,
				FALSE
				);
		return TRUE;
	}
	return FALSE;
}

void CExtControlBar::DoFrameBarCheckUpdate(
	CFrameWnd * pFrame,
	CCmdUI * pCmdUI,
	bool bResizableCheckmarks // = false
	)
{
	ASSERT_VALID( pFrame );

CControlBar * pBar = pFrame->GetControlBar( pCmdUI->m_nID );
	if( pBar != NULL )
	{
		if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			((CExtControlBar*)pBar)->
				OnFrameBarCheckUpdate( pCmdUI, bResizableCheckmarks );
		else
			pCmdUI->SetCheck(
				(pBar->GetStyle() & WS_VISIBLE) != 0
				);
		return;
	}

	pCmdUI->ContinueRouting();
}

void CExtControlBar::OnSetFocus(CWnd* pOldWnd) 
{
	CControlBar::OnSetFocus(pOldWnd);

	if( IsFixedMode() )
		return;
	
HWND hWndChild = ::GetWindow( GetSafeHwnd(), GW_CHILD );
	if( hWndChild != NULL )
		::SetFocus( hWndChild );
}

bool CExtControlBar::OnConstructDockSiteControlBarPopupMenu(
	CExtPopupMenuWnd * pPopup
	) const
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(GetSafeHwnd()) );
	ASSERT_VALID( pPopup );

	if( !m_bAppearInDockSiteControlBarPopupMenu )
		return false;
UINT nBarID = (UINT)GetDlgCtrlID();
	VERIFY( pPopup->ItemInsert( nBarID ) );
	
	return true;
}

LRESULT CExtControlBar::OnHelpHitTest(WPARAM wParam, LPARAM lParam)
{
HWND hWndCapture = ::GetCapture();	
	if( hWndCapture != NULL && hWndCapture == GetSafeHwnd() )
		SendMessage( WM_CANCELMODE );
LRESULT lResult = DoHelpHitTest( CPoint(DWORD(lParam)) );
	if( lResult == -3L )
		return 0L;
	if( lResult == -2L )
		return -1L;
	if( lResult != -1L )
		return HID_BASE_COMMAND+lResult;
	return CControlBar::OnHelpHitTest(wParam,lParam);
}

LRESULT CExtControlBar::DoHelpHitTest( CPoint ptHelpHitTest )
{
	ASSERT_VALID( this );
	ptHelpHitTest;
	return -1L;
}

bool CExtControlBar::g_bUseCmdManagerForSetMessageString = true;
void CExtControlBar::stat_SetMessageString(
	CWnd * pWnd,
	UINT nCmdID // = AFX_IDS_IDLEMESSAGE
	)
{
	if(		pWnd->GetSafeHwnd() == NULL
		||	(! ::IsWindow(pWnd->GetSafeHwnd()) )
		)
		return;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( pWnd->m_hWnd );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		nCmdID = AFX_IDS_IDLEMESSAGE;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

CFrameWnd * pFrame = DYNAMIC_DOWNCAST( CFrameWnd, pWnd );
	if( pFrame == NULL && (pWnd->GetStyle() & WS_CHILD) != 0 )
		pFrame = pWnd->GetParentFrame();
	if(		pFrame != NULL
		&&	pFrame->IsKindOf(RUNTIME_CLASS(CExtMiniDockFrameWnd))
		)
		pFrame = pFrame->GetParentFrame();
	if( pFrame != NULL )
	{
		if(		g_bUseCmdManagerForSetMessageString
			&&	nCmdID != AFX_IDS_IDLEMESSAGE
			)
		{
			__EXT_MFC_SAFE_LPCTSTR strProfile = g_CmdManager->ProfileNameFromWnd( pFrame->m_hWnd );
			if( strProfile != NULL )
			{
				CExtCmdItem * pCmdItem = g_CmdManager->CmdGetPtr( strProfile, nCmdID );
				if( pCmdItem != NULL )
				{
					CControlBar * pTempBar =
						pFrame->GetControlBar( AFX_IDW_STATUS_BAR );
					if( pTempBar != NULL )
					{
						CStatusBar * pStatusBar =
							DYNAMIC_DOWNCAST( CStatusBar, pTempBar );
						if( pStatusBar != NULL )
						{
							pStatusBar->SetPaneText(
								0,
								pCmdItem->m_sTipStatus.IsEmpty()
									? _T("")
									: pCmdItem->m_sTipStatus
								);
							return;
						} // if( pStatusBar != NULL )
					} // if( pTempBar != NULL )
				} // if( pCmdItem != NULL )
			} // if( strProfile != NULL )
		} // if( g_bUseCmdManagerForSetMessageString ...
		pFrame->SendMessage( WM_SETMESSAGESTRING, nCmdID );
	} // if( pFrame != NULL )
	else
	{
		if(		g_bUseCmdManagerForSetMessageString
			&&	nCmdID != AFX_IDS_IDLEMESSAGE
			)
		{
			__EXT_MFC_SAFE_LPCTSTR strProfile = g_CmdManager->ProfileNameFromWnd( pWnd->m_hWnd );
			if( strProfile != NULL )
			{
				CExtCmdItem * pCmdItem = g_CmdManager->CmdGetPtr( strProfile, nCmdID );
				if( pCmdItem != NULL )
				{
					CWnd * pTempBar =
						pWnd->GetDlgItem( AFX_IDW_STATUS_BAR );
					if( pTempBar != NULL )
					{
						CStatusBar * pStatusBar =
							DYNAMIC_DOWNCAST( CStatusBar, pTempBar );
						if( pStatusBar != NULL )
						{
							pStatusBar->SetPaneText(
								0,
								pCmdItem->m_sTipStatus.IsEmpty()
									? _T("")
									: pCmdItem->m_sTipStatus
								);
							return;
						} // if( pStatusBar != NULL )
					} // if( pTempBar != NULL )
				} // if( pCmdItem != NULL )
			} // if( strProfile != NULL )
		} // if( g_bUseCmdManagerForSetMessageString ...
		pWnd->SendMessage( WM_SETMESSAGESTRING, nCmdID );
	} // else from if( pFrame != NULL )
}

void CExtControlBar::stat_RedrawFloatingFrames( CFrameWnd * pDockSite )
{
	ASSERT_VALID( pDockSite );
CMap
	<	CMiniFrameWnd *,
		CMiniFrameWnd *,
		char,
		char
		>
	setMiniFrames;
POSITION pos = pDockSite->m_listControlBars.GetHeadPosition();
	for( ; pos != NULL ; )
	{
		CControlBar * pControlBar = (CControlBar *)
			pDockSite->m_listControlBars.GetNext( pos );
		if( pControlBar->m_pDockSite == NULL )
			continue;
		ASSERT_VALID( pControlBar );
		ASSERT_KINDOF( CControlBar, pControlBar );
		if( pControlBar->IsDockBar() )
			continue;
		CFrameWnd * pBarParentFrame =
			pControlBar->GetParentFrame();
		ASSERT_VALID( pBarParentFrame );
		if( pBarParentFrame == pDockSite )
			continue;
		CMiniFrameWnd * pFloatingFrame =
			DYNAMIC_DOWNCAST(
				CMiniFrameWnd,
				pBarParentFrame
				);
		if( pFloatingFrame == NULL )
			continue;
		setMiniFrames.SetAt( pFloatingFrame, 0 );
	}
	pos = setMiniFrames.GetStartPosition();
	for( ; pos != NULL ; )
	{
		CMiniFrameWnd * pFloatingFrame = NULL;
		char nDummy;
		setMiniFrames.GetNextAssoc(pos,pFloatingFrame,nDummy);
		pFloatingFrame->RecalcLayout();
		pFloatingFrame->RedrawWindow(
			NULL,
			NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
				| RDW_FRAME | RDW_ALLCHILDREN
			);
	}
}

void CExtControlBar::stat_RecalcBarMetrics( CFrameWnd * pDockSite )
{
	ASSERT_VALID( pDockSite );
	for(	POSITION pos = pDockSite->m_listControlBars.GetHeadPosition();
			pos != NULL;
			)
	{
		CControlBar * pControlBar = (CControlBar *)
			pDockSite->m_listControlBars.GetNext( pos );
		if( pControlBar->m_pDockSite == NULL )
			continue;
		ASSERT_VALID( pControlBar );
		ASSERT_KINDOF( CControlBar, pControlBar );
		if( pControlBar->IsDockBar() )
			continue;
		pControlBar->SetWindowPos(
			NULL, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER
				|SWP_NOREDRAW|SWP_FRAMECHANGED
			);
	}
}
