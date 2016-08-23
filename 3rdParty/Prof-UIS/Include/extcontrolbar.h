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

#if (!defined __EXT_CONTROLBAR_H)
#define __EXT_CONTROLBAR_H

//
// Many thanks to Paul DiLascia & Cristi Posea, their works
// were a powerful incentive to learn more about pretty
// complicated MFC docking windows mechanism
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// extcontrolbar.h : header file
//

#if (!defined __AFXMT_H__)
	#include<AfxMT.h>
#endif

#if (!defined __AFXTEMPL_H__)
	#include<AfxTempl.h>
#endif

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	#if (!defined __EXTCUSTOMIZE_H)
		#include <ExtCustomize.h>
	#endif
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#define __EXT_TB_BUTTON_SEPARATOR_WIDTH 6
#define __EXT_TB_BUTTON_SEPARATOR_HEIGHT 6
#define __EXT_TB_BUTTON_TEXT_MARGIN 3 // 5
#define __EXT_TB_BUTTON_INNER_MARGIN 0 // 1
#define __EXT_TB_BUTTON_OUTER_MARGIN 3
#define __EXT_TB_ROW_DISTANCE 6

class CExtDockBar;
class CExtDockOuterBar;
class CExtMiniDockFrameWnd;
class CExtPopupMenuWnd;
class CExtControlBar;
class CExtToolControlBar;
class CExtMenuControlBar;
class CExtBarButton;
class CExtBarContentExpandButton;
class CExtBarMdiDocButton;
class CExtControlBar;
class CExtDynControlBar;
class CExtDockDynBar;

#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	class CExtTabMdiWnd;
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	class CExtDockDynTabBar;
	class CExtDynTabWnd;
	class CExtDynTabControlBar;
	class CExtDynAutoHideArea;
	class CExtDynAutoHideSlider;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

typedef
	CArray < CExtControlBar*, CExtControlBar* >
	ExtControlBarVector_t;

typedef
	CArray < CControlBar*, CControlBar* >
	MfcControlBarVector_t;


/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButton

class __PROF_UIS_API CExtBarNcAreaButton : public CObject
{
protected:
	CExtControlBar * m_pBar;
	CRect m_rc;
	bool m_bHover:1, m_bPushed:1, m_bDisabled:1,
		m_bPaintingCombinedArea:1;

public:

	DECLARE_DYNCREATE( CExtBarNcAreaButton )

	CExtBarNcAreaButton(
		CExtControlBar * pBar = NULL
		);
	~CExtBarNcAreaButton();

	virtual void OnNcDestroy();

	operator CSize() const
	{
		ASSERT_VALID( this );
		return m_rc.Size();
	}
	operator const CRect &() const
	{
		ASSERT_VALID( this );
		return m_rc;
	}
	operator CRect &()
	{
		ASSERT_VALID( this );
		return m_rc;
	}

	bool IsBarFixedMode() const;
	bool IsBarVisible() const;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	bool IsBarAutoHideMode(
		bool * p_bDisplayed = NULL,
		bool * p_bActive = NULL
		) const;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	bool IsBarFloated() const;
	bool IsBarDocked() const;
	bool IsBarDockedHorizontally() const;
	bool IsBarDockedVertically() const;
	bool IsBarDockedAtLeft() const;
	bool IsBarDockedAtRight() const;
	bool IsBarDockedAtTop() const;
	bool IsBarDockedAtBottom() const;
	bool GetBarNcAreaInfo( // returns false if no nc area on bar
		LPRECT pRectGripper = NULL, // in window coords
		LPRECT pRectWnd = NULL, // in screen coords
		bool * pbGripperAtTop = NULL,
		bool * pbTextOnGripper = NULL
		) const;
	bool IsBarWndActive() const;
	bool IsBarSingleOnRow() const;
	bool IsBarMinimizedOnRow() const;
	bool IsBarMaximizedOnRow() const;
	bool IsBarFirstOnRow( bool bExtBar ) const;
	bool IsBarLastOnRow( bool bExtBar ) const;
	CFrameWnd * GetDockingFrame();

	CExtControlBar * GetBar()
	{
		ASSERT_VALID( this );
		return m_pBar;
	}
	const CExtControlBar * GetBar() const
	{
		ASSERT_VALID( this );
		return m_pBar;
	}

	virtual void OnNcAreaReposition(
		CExtBarNcAreaButton * pPrevBtn
		);

	virtual bool OnQueryVisibility() const;
	
	void NcDrawDefault(
		CDC & dc,
		int nDockBtnGlyptT
		);

	virtual void OnNcAreaDraw( CDC & dc );

	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual HCURSOR OnNcAreaQueryCursor( CPoint point );

	virtual bool OnNcAreaClicked( CPoint point );

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);

	int DoDefaultReflectForToolHitTest(
		TOOLINFO * pTI,
		UINT nIdResourceTooltipText,
		__EXT_MFC_SAFE_LPCTSTR sTooltipTextDefault
		);
	int DoDefaultReflectForToolHitTest(
		TOOLINFO * pTI,
		__EXT_MFC_SAFE_LPCTSTR sTooltipText
		);

#ifdef _DEBUG
	virtual void AssertValid() const
	{
		ASSERT( m_pBar != NULL );
		CObject::AssertValid();
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CObject::Dump( dc );
	}
#endif

	friend class CExtControlBar;
}; // class CExtBarNcAreaButton

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonExpand

class __PROF_UIS_API CExtBarNcAreaButtonExpand : public CExtBarNcAreaButton
{
public:

	DECLARE_DYNCREATE( CExtBarNcAreaButtonExpand )

	CExtBarNcAreaButtonExpand(
		CExtControlBar * pBar = NULL
		);

	virtual bool OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC & dc );
	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual bool OnNcAreaClicked( CPoint point );

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);

}; // class CExtBarNcAreaButtonExpand

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonMenu

class __PROF_UIS_API CExtBarNcAreaButtonMenu : public CExtBarNcAreaButton
{
public:

	DECLARE_DYNCREATE( CExtBarNcAreaButtonMenu )

	CExtBarNcAreaButtonMenu(
		CExtControlBar * pBar = NULL
		);

	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);

	virtual bool OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC & dc );
	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual bool OnNcAreaClicked( CPoint point );

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);
}; // class CExtBarNcAreaButtonMenu

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonClose

class __PROF_UIS_API CExtBarNcAreaButtonClose : public CExtBarNcAreaButton
{
public:

	DECLARE_DYNCREATE( CExtBarNcAreaButtonClose )

	CExtBarNcAreaButtonClose(
		CExtControlBar * pBar = NULL
		);

	virtual bool OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC & dc );
	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual bool OnNcAreaClicked( CPoint point );

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);

}; // class CExtBarNcAreaButtonClose

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonAutoHide

class __PROF_UIS_API CExtBarNcAreaButtonAutoHide : public CExtBarNcAreaButton
{
public:

	DECLARE_DYNCREATE( CExtBarNcAreaButtonAutoHide )

	CExtBarNcAreaButtonAutoHide(
		CExtControlBar * pBar = NULL
		);

	virtual bool OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC & dc );
	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual bool OnNcAreaClicked( CPoint point );

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);

	virtual void OnNcAreaReposition(
		CExtBarNcAreaButton * pPrevBtn
		);
}; // class CExtBarNcAreaButtonAutoHide

#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)


class CExtCustomizeToolbarsListBox;

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar window

class __PROF_UIS_API CExtControlBar
	: public CControlBar
	, public CExtMouseCaptureSink
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	, public CExtCustomizeSite::ICustomizeDropTarget
	, public CExtCustomizeSite::ICustomizeDropSource
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
{
	struct __PROF_UIS_API InternalDockBarInfo_t
	{
		CMap < UINT, UINT, DWORD, DWORD > m_mapDockBarInfo;
		InternalDockBarInfo_t();
		DWORD operator[]( UINT nDockBarID );
		bool ReplaceFrameDockBar(
			UINT nDockBarID,
			DWORD dwDockStyle,
			CFrameWnd * pFrame
			);
	}; // struct InternalDockBarInfo_t

	static InternalDockBarInfo_t g_DockBarInfo;

public:

	class __PROF_UIS_API InternalAffixmentData
	{
		void _AssignFromOther(
			const InternalAffixmentData & other
			);
	public:
		UINT m_nAffixmentWeight;
		CRect m_rcAffixment;
		CSize m_sizeCompressed;
		DWORD m_dwAffixmentFlags;
		enum e_affixment_flags
		{
			__EAFF_FIXED_MODE_INITIAL_RECALC = 0x00000001L
		};
		InternalAffixmentData();
		InternalAffixmentData(
			const InternalAffixmentData & other
			);
		~InternalAffixmentData();
		bool IsEmpty() const;
		void Empty();
		bool operator !()
		{
			return (!IsEmpty());
		}
		InternalAffixmentData & operator =(
			const InternalAffixmentData & other
			)
		{
			_AssignFromOther( other );
			return *this;
		}
		int CompareAffixment(
			const InternalAffixmentData & other
			) const;
		void Serialize( CArchive & ar );
	}; // class InternalAffixmentData

	class __PROF_UIS_API InternalDockStateBar : public CObject
	{
	public:
		UINT m_nBarID, m_nDockBarID, m_nCircleNo, m_nMRUWidth;
		DWORD m_dwBarFlags, m_dwDockStateFlags, m_dwMRUFloatStyle,
			m_dwFloatingFrameCreateStyle;
		LONG m_nDelayedRowUpdateMetric;
		CPoint m_ptFloatPos, m_ptMRUFloatPos;
		CRect m_rcMRUDockPos, m_rcFloatingFrame, m_rcBar;
		CSize m_sizeDockedH, m_sizeDockedV, m_sizeFloated;

		CArray <
			InternalDockStateBar *,
			InternalDockStateBar *
			> m_arrChildStates;

		LONG m_nHelperAciveTabChildIndex; 
		CExtControlBar * m_pHelperBar;

		// m_dwDockStateFlags values
		enum
		{
			__DOCK_AT_NOWHERE				= 0x00000000,
			__DOCK_AT_SIDE					= 0x00000001,
			__DOCK_DYNAMIC_HALF_SPLIT		= 0x00000002,
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			__DOCK_DYNAMIC_TABBED			= 0x00000003,
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			__DOCK_AT_MASK					= 0x0000000F,
			__STATE_VISIBLE					= 0x00000010,
			__STATE_FLOATING				= 0x00000020,
			__STATE_NEW_ROW					= 0x00000040,
			__STATE_DYNAMIC_CHILD			= 0x00000080,
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			__STATE_TAB_CHILD				= 0x00000100,
			__STATE_TAB_ACTIVE				= 0x00000200,
			__STATE_AUTOHIDDEN				= 0x00000400,
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		};

		InternalDockStateBar();
		~InternalDockStateBar();
		void InitMembers();
		virtual void Serialize( CArchive & ar );
		void Clear();
		void StateGet(
			CExtControlBar * pBar,
			InternalDockStateBar * pParentState
			);
		void StateSet(
			CFrameWnd * pDockSite,
			InternalDockStateBar * pParentState
			);
		void PreSyncAttributes(
			CFrameWnd * pDockSite,
			InternalDockStateBar * pParentState
			);
		void PostSyncAttributes(
			CFrameWnd * pDockSite,
			InternalDockStateBar * pParentState
			);
	};

	struct __PROF_UIS_API OuterItemData_t
	{
		UINT m_nBarID;
		CPoint m_ptFloatHelper;
		CSize m_sizeDockedH, m_sizeDockedV, m_sizeFloated;
		InternalAffixmentData m_AffixmentData;
		bool m_bFloating:1;
		OuterItemData_t();
		OuterItemData_t(
			const OuterItemData_t & other
			);
		OuterItemData_t & operator =(
			const OuterItemData_t & other
			);
		void Serialize( CArchive & ar );
	};

	typedef
		CArray < OuterItemData_t, OuterItemData_t & >
		OuterPropsArr_t;

	class __PROF_UIS_API InternalDockStateSite : public CObject
	{
	public:
		
		OuterPropsArr_t m_arrOuterProps;

		CFrameWnd * m_pDockSite;
		WINDOWPLACEMENT m_wp;
		CDockState * m_pOuterState;

		CArray <
			InternalDockStateBar *,
			InternalDockStateBar *
			> m_arrBarStates;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		CArray < UINT, UINT > m_arrAutoHideOrder;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		InternalDockStateSite(
			CFrameWnd * pDockSite
			);
		~InternalDockStateSite();

		virtual void Serialize( CArchive & ar );
		void Clear();
		void StateGet();
		void StateSet( bool bPresetWP ) const;

		InternalDockStateBar * AppendFloatingLayout(
			CExtMiniDockFrameWnd * pMiniFrame
			);
		InternalDockStateBar * AppendDockBarAtSideLayout(
			CExtDockBar * pDockBar
			);
	};

private:
	class __PROF_UIS_API InternalFriendlyFrameWnd : public CFrameWnd
	{
		void SetOuterDockState(
			const CDockState & state,
			const OuterPropsArr_t & arrOuterProps
			);
		void GetOuterDockState(
			CDockState & state,
			OuterPropsArr_t & arrOuterProps
			) const;
	public:
		bool SetupSmilyFrameWnd(void);

		void SetDockState(
			const InternalDockStateSite & state,
			bool bPresetWP
			);
		void GetDockState(
			InternalDockStateSite & state
			) const;

		bool IsRecalcLayoutDelayed()
		{
			return
				( (m_nIdleFlags & CFrameWnd::idleLayout) != NULL )
					? true : false;
		}
		bool IsInRecalcLayout()
		{
			return m_bInRecalcLayout ? true : false;
		}
		BOOL ProcessHelpMsg( MSG & msg, DWORD * pContext )
		{
			return CFrameWnd::ProcessHelpMsg( msg, pContext );
		}
		HWND SetHelpCapture( POINT point, BOOL * pbDescendant )
		{
			return CFrameWnd::SetHelpCapture( point, pbDescendant );
		}

		friend class InternalDockStateSite;
	};

	struct __PROF_UIS_API InternalBarRepositionData_t
	{
		CControlBar * m_pBar;
		CRect m_rcSrc, m_rcDst;
		INT	m_nSpaceAvailByBar,
			m_nSpaceAvailBySideBefore,
			m_nSpaceAvailBySideAfter;
		InternalBarRepositionData_t(
			CControlBar * pBar = NULL,
			bool bSetAdvData = false
			);
		InternalBarRepositionData_t(
			const InternalBarRepositionData_t & other
			);
		void _AssignFromOther(
			const InternalBarRepositionData_t & other
			);
		void SetBar(
			CControlBar * pBar,
			bool bSetAdvData = false
			);
		InternalBarRepositionData_t & operator=( const InternalBarRepositionData_t & other )
		{
			_AssignFromOther( other );
			return *this;
		}
		bool IsRectsEqual() const
		{
			return (m_rcSrc == m_rcDst) ? true : false;
		}
		CExtControlBar * GetExtBar();
		CExtControlBar * GetExtBarFixed();
	}; // struct InternalBarRepositionData_t

// Construction
public:

	struct __PROF_UIS_API QUERY_REPOSITION_CALC_EFFECT_DATA
	{
		enum // Prof-UIS reposition effect flags
		{
			__PRCF_WND_REPOS_DEFAULT	= 0x0000,
			__PRCF_WND_REPOS_QUERY		= 0x0001,
			__PRCF_WND_REPOS_EXTRA		= 0x0002,
			__PRCF_WND_REPOS_MASK		= 0x000F,
			__PRCF_EXCLUDE_FROM_CENTER	= 0x0010,
		};
		SHORT m_nFlags; 
		CWnd * m_pWndToReposChilds;
		UINT m_nIDFirst;
		UINT m_nIDLast;
		UINT m_nIDLeftOver;
		LPRECT m_lpRectParam;
		LPCRECT m_lpcRectClient;
		bool m_bStretch:1;
		QUERY_REPOSITION_CALC_EFFECT_DATA(
			SHORT nFlags,
			CWnd * pWndToReposChilds,
			UINT nIDFirst,
			UINT nIDLast,
			UINT nIDLeftOver,
			LPRECT lpRectParam,
			LPCRECT lpcRectClient,
			bool bStretch
			);
		~QUERY_REPOSITION_CALC_EFFECT_DATA();
		operator WPARAM() const;
		void QueryTypeSet( SHORT nQueryType );
		SHORT QueryTypeGet() const;
		bool IsQueryReposDefault() const;
		bool IsQueryReposQuery() const;
		bool IsQueryReposExtra() const;
		void ExcludeFromCenterSet(
			bool bExclude = true
			);
		bool ExcludeFromCenterGet() const;
	}; // struct QUERY_REPOSITION_CALC_EFFECT_DATA

	struct __PROF_UIS_API POPUP_MENU_EVENT_DATA
	{
		enum // Prof-UIS notification types
		{
			__PMED_DOCKBAR_CTX					=  0, // dockbar context menu
			__PMED_CONTROLBAR_CTX				=  1, // any control bar context menu (client area)
			__PMED_CONTROLBAR_NC_CTX			=  2, // any control bar context menu (non-client area)
			__PMED_STATUSBAR_CTX				=  3, // statusbar context menu
			__PMED_AUTOHIDESLIDER_CTX			=  4, // autohide slider window context menu
			__PMED_MINIFRAME_NC_CTX				=  5, // miniframe context menu (non-client area)
			__PMED_MDICLIAREA_CTX				=  6, // mdi client area context nenu
			__PMED_MDITABS_CTX					=  7, // mdi-tabs window
			__PMED_AUTOHIDETABS_CTX				=  8, // autohide-tabs window
			__PMED_DYNCBCTABS_CTX				=  9, // dynamic control bar container tabs window
			__PMED_CONTROLBAR_NCBTNMENU_TOP		= 10, // control bar nc-area-menu-button - top level
			__PMED_CONTROLBAR_NCBTNMENU_BARS	= 11, // control bar nc-area-menu-button - control bars list 
			__PMED_CTXEXPBTN_TOP				= 12, // content expand button - top level
			__PMED_CTXEXPBTN_APPEND				= 13, // content expand button - append to buttons list
			__PMED_CTXEXPBTN_BARS				= 14, // content expand button - control bars list
		};

		SHORT m_nHelperNotificationType; // notification type
		CExtPopupMenuWnd * m_pPopupMenuWnd; // popup menu to initialize
		CWnd * m_pWndEventSrc; // any control bar or mdi-tabs or ahtohide-tabs
		UINT m_nMsgID; // message handler from where notification sent
		CObject * m_pObjectSrc; // NULL or CExtBarNcAreaButton
		CWnd * m_pWndInitialTarget; // window to notify first
		bool m_bPostNotification:1; // is it post notifictaion?
		CPoint m_ptScreen; // cursor position in screen coords
		
		POPUP_MENU_EVENT_DATA(
			SHORT nHelperNotificationType,
			CExtPopupMenuWnd * pPopupMenuWnd,
			CWnd * pWndEventSrc,
			const POINT & ptScreen,
			UINT nMsgID = WM_NULL,
			CObject * pObjectSrc = NULL
			);
		virtual ~POPUP_MENU_EVENT_DATA();

		virtual bool IsControlBarMenuListNotification() const;

		operator WPARAM() const;
		static POPUP_MENU_EVENT_DATA * FromWParam( WPARAM wParam );
		static POPUP_MENU_EVENT_DATA * FromMSG( MSG * pMSG );
		
		virtual bool NotifyTarget(
			bool bPostNotification
			);
	}; // struct POPUP_MENU_EVENT_DATA

	DECLARE_DYNCREATE( CExtControlBar )

	CExtControlBar();

	virtual bool OnConstructDockSiteControlBarPopupMenu(
		CExtPopupMenuWnd * pPopup
		) const;

// Attributes
public:
	bool m_bGripperStaticallyAtTop:1;
	bool m_bAllowContractExpand:1;
	bool m_bDoNotEraseClientBackground:1;
	bool m_bPresubclassDialogMode:1;
	bool m_bReflectParentSizing:1;
	bool m_bInCustomModeUpdateCmdUI:1;
	bool m_bAppearInDockSiteControlBarPopupMenu:1;
	static const UINT g_nMsgQueryRepositionCalcEffect;
	static const UINT g_nMsgQueryIcon;
	static const UINT g_nMsgPutToPopupMenu;
	static const UINT g_nMsgConstructPopupMenu;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	static const UINT g_nMsgCreateTabbedBar;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	static bool g_bEnableOnIdleCalls;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	static bool g_bAllowAutohideTabsOleDropTarget; // (+ v.2.23)
	static bool g_bAllowDynamicTabsOleDropTarget; // (+ v.2.23)
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

protected:
	bool m_bFixedMode:1;
	INT m_nGripHeightAtTop, m_nGripWidthAtLeft;
	INT m_nSeparatorHeight, m_nSeparatorWidth;
	INT m_nMinHW, m_nMinVH;
	INT m_nTrackerOffset, m_nMetricOffset;

private:
	static CCriticalSection g_csCB;
	static ExtControlBarVector_t g_AllBars;
	// sizes in docked/floated state
	CSize m_sizeDockedH, m_sizeDockedV, m_sizeFloated;
	CRect m_rcGrip,m_rcRowResize,m_rcRowRecalc,m_rcRowRecalcUp;
	// flags
	bool m_bRowResizing:1;
	bool m_bRowRecalcing:1;
	bool m_bDragging:1;
	bool m_bReposSingleChildMode:1;
public:
	const CRect & _GetRectGrip() const
	{
		return m_rcGrip;
	}
protected:
	bool m_bUpdatingChain:1;
private:
	LONG m_nDelayedRowUpdateMetric;
	// floating/dragging helpers
	CPoint m_ptFloatHelper;
	CPoint m_ptHelperDragOffsetFloated;
	CPoint m_ptHelperDragOffsetDockedH;
	CPoint m_ptHelperDragOffsetDockedV;
	bool m_bWindowActive:1;
	bool m_bTopRecalcing:1;
	bool m_bNcAreaBtnTracking:1;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	bool m_bAutoHideMode:1;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	CPoint m_ptDraggingLast;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtControlBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtControlBar();

	void SetInitDesiredSizeVertical(CSize size_data)
	{
		m_sizeDockedV = size_data;
	};
	void SetInitDesiredSizeHorizontal(CSize size_data)
	{
		m_sizeDockedH = size_data;
	};
	void SetInitDesiredSizeFloating(CSize size_data)
	{
		m_sizeFloated = size_data;
	};

	static bool ProfileBarStateLoad(
		CFrameWnd * pFrame,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%
		LPWINDOWPLACEMENT pFrameWp = NULL // need serialize frame's WP
		);
	static bool ProfileBarStateSave(
		CFrameWnd * pFrame,
		__EXT_MFC_SAFE_LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		__EXT_MFC_SAFE_LPCTSTR sSectionNameProfile, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%\%sSectionNameProfile%\%sSectionNameProfile%
		LPWINDOWPLACEMENT pFrameWp = NULL // need serialize frame's WP
		);
	static bool ProfileBarStateSerialize(
		CArchive & ar,
		CFrameWnd * pFrame,
		LPWINDOWPLACEMENT pFrameWp = NULL // need serialize frame's WP
		);

    virtual BOOL Create(
		__EXT_MFC_SAFE_LPCTSTR lpszWindowName,
		CWnd * pParentWnd,
        UINT nID = AFX_IDW_DIALOGBAR,
		DWORD dwStyle =
			WS_CHILD|WS_VISIBLE
			|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS
			|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
			|CBRS_HIDE_INPLACE
		);

	static bool FindPrintPreviewMode(
		CFrameWnd * pFrame
		);
	virtual bool IsDockSiteInPrintPreviewMode();
	static bool IsOleIpObjActive(
		CFrameWnd * pFrameSearch
		);
	bool IsOleIpObjActive() const;
	virtual bool SafeDisplayBar();
	static bool FindHelpMode(
		CWnd * pWndStartSearch
		);

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual void OnCustomizeModeEnter();
	virtual void OnCustomizeModeLeave();
	bool m_bVisibleInCustomizeListBox:1;
	virtual void OnCustomizeListBoxUpdate( CExtCustomizeToolbarsListBox * pWndList );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

protected:
	static bool _FrameEnableDockingImpl(
		CFrameWnd * pFrame,
		DWORD dwDockStyle = CBRS_ALIGN_ANY,
		bool bReplaceFloatingWnd = true
		);
	static bool _SetupFloatingFrameWndImpl(
		CFrameWnd * pFrame
		);
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	static bool _FrameInjectAutoHideAreasImpl(
		CFrameWnd * pFrame
		);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
public:
	static bool FrameEnableDocking(
		CFrameWnd * pFrame,
		DWORD dwDockStyle = CBRS_ALIGN_ANY,
		bool bReplaceFloatingWnd = true
		);
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	static bool FrameInjectAutoHideAreas(
		CFrameWnd * pFrame
		);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	bool IsSingleOnRow(
		bool bOnlyFixed = false,
		bool bOnlyNonFixed = false
		) const;
	void MakeSingleOnRow();

	bool IsMinimizedOnRow() const;
	bool IsMaximizedOnRow() const;
	void MaximizeOnRow();
	void MinimizeOnRow();

	virtual bool IsFixedMode() const;
	virtual bool IsFixedDockStyle() const;

	UINT GetSafeDockBarDlgCtrlID() const;
	bool IsDockedAtTop() const;
	bool IsDockedAtBottom() const;
	bool IsDockedAtLeft() const;
	bool IsDockedAtRight() const;
	bool IsDockedHorizontally() const;
	bool IsDockedVertically() const;
	bool IsDocked() const;
	bool IsFloating() const;
	
	virtual bool IsBarWithGripper(
		bool * pbGripperAtTop = NULL,
		bool * pbTextOnGripper = NULL
		) const;
	
	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz
		);
	virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD nMode
		);

	virtual void DoPaint( CDC* pDC );
	virtual void DoEraseBk( CDC* pDC, LPCRECT pRectBk );
	virtual void OnUpdateCmdUI(
		CFrameWnd * pTarget,
		BOOL bDisableIfNoHndler
		);

	static BOOL DoCustomModePreTranslateMessage(
		CWnd * pWndPretranslateSrc,
		MSG * pMsg
		);
	static void DoCustomModeUpdateControlBars(
		CWnd * pWndParentTarget,
		BOOL bDisableIfNoHndler = TRUE
		);
	bool DoCustomModeUpdateCmdUI(
		BOOL bDisableIfNoHndler = TRUE
		);

protected:

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		) const;
	
public:
	virtual bool _GetFullRowMode() const;
	virtual INT _CalcDesiredMinHW() const;
	virtual INT _CalcDesiredMinVH() const;
	virtual CSize _CalcDesiredMinFloatedSize() const;
	virtual CSize _CalcDesiredMinOuterSize( BOOL bHorz );
	virtual CExtControlBar::InternalAffixmentData *
		_AffixmentGetOuter();
	virtual void _AffixmentSetOuter(
		CExtControlBar::InternalAffixmentData * pAffixmentData
		);
	static CExtControlBar::InternalAffixmentData *
		_AffixmentGetOuter(
			CControlBar * pBar
			);
	static int _AffixmentCompareOuter(
		CControlBar * pBar1,
		CControlBar * pBar2
		);
	virtual void _AffixmentRecalcOuter();
	virtual void _AffixmentSafeClearOuter();
	virtual bool _AffixmentIsAlignedHW() const;

private:
	
	class FriendlyDockBarHack;

	CSize _CalcDesiredLayout(
		BOOL bHorz,
		BOOL bForceFullRow = FALSE
		);

protected:
	enum eGFECB_t
	{
		// filters by control bar window properties
		__GFECB_FIXED				= 0x00000001L, // fixed sized
		__GFECB_NONFIXED_MIN		= 0x00000002L, // resizable and minimized on row
		__GFECB_NONFIXED_NONMIN		= 0x00000004L, // resizable and not minimized on row
		__GFECB_NONFIXED			= (__GFECB_NONFIXED_MIN|__GFECB_NONFIXED_NONMIN),

		__GFECB_DOCKED_TOP			= 0x00000008L,
		__GFECB_DOCKED_BOTTOM		= 0x00000010L,
		__GFECB_DOCKED_LEFT			= 0x00000020L,
		__GFECB_DOCKED_RIGHT		= 0x00000040L,

		__GFECB_DOCKED_HORZ			= (__GFECB_DOCKED_TOP|__GFECB_DOCKED_BOTTOM),
		__GFECB_DOCKED_VERT			= (__GFECB_DOCKED_LEFT|__GFECB_DOCKED_RIGHT),
		__GFECB_DOCKED				= (__GFECB_DOCKED_HORZ|__GFECB_DOCKED_VERT),
		
		__GFECB_FLOATED				= 0x00000080L,
		
		__GFECB_BAR_VISIBLE			= 0x00000100L,
		__GFECB_BAR_INVISIBLE		= 0x00000200L,
		__GFECB_WND_VISIBLE			= 0x00000400L,
		__GFECB_WND_INVISIBLE		= 0x00000800L,
		
		__GFECB_VIS_BAR_ALL			= (__GFECB_BAR_VISIBLE|__GFECB_BAR_INVISIBLE),
		__GFECB_VIS_WND_ALL			= (__GFECB_WND_VISIBLE|__GFECB_WND_INVISIBLE),
		
		__GFECB_VIS_ALL				= (__GFECB_BAR_VISIBLE|__GFECB_WND_VISIBLE),
		__GFECB_INVIS_ALL			= (__GFECB_BAR_INVISIBLE|__GFECB_WND_INVISIBLE),
//		__GFECB_VIS_ALL				= (__GFECB_VIS_BAR_ALL|__GFECB_VIS_WND_ALL),
		
		__GFECB_ALL_NONRESTRICT		= (__GFECB_FIXED|__GFECB_NONFIXED|__GFECB_DOCKED|__GFECB_FLOATED|__GFECB_VIS_ALL|__GFECB_INVIS_ALL),

		// restriction by point
		__GFECB_BYPT_ONLY_WND		= 0x00008000L, // abondones all other __GFECB_BYPT_ flags
		__GFECB_BYPT_CLIENT			= 0x00010000L,
		__GFECB_BYPT_NCGRIP			= 0x00020000L,

		__GFECB_BYPT_HTLEFT			= 0x00040000L,
		__GFECB_BYPT_HTRIGHT		= 0x00080000L,
		__GFECB_BYPT_HTTOP			= 0x00100000L,
		__GFECB_BYPT_HTBOTTOM		= 0x00200000L,
		__GFECB_BYPT_HTTOPLEFT		= 0x00400000L,
		__GFECB_BYPT_HTTOPRIGHT		= 0x00800000L,
		__GFECB_BYPT_HTBOTTOMLEFT	= 0x01000000L,
		__GFECB_BYPT_HTBOTTOMRIGHT	= 0x02000000L,

		__GFECB_BYPT_HT_CORNER		= (__GFECB_BYPT_HTLEFT|__GFECB_BYPT_HTRIGHT|__GFECB_BYPT_HTTOP|__GFECB_BYPT_HTBOTTOM),
		__GFECB_BYPT_HT_NONCORNER	= (__GFECB_BYPT_HTTOPLEFT|__GFECB_BYPT_HTTOPRIGHT|__GFECB_BYPT_HTBOTTOMLEFT|__GFECB_BYPT_HTBOTTOMRIGHT),

		__GFECB_BYPT_HTNOWHERE		= 0x04000000L,
		__GFECB_BYPT_HTSYSMENU		= 0x06000000L,

		__GFECB_BYPT_HTGROWBOX		= 0x10000000L,
		__GFECB_BYPT_HTMINBUTTON	= 0x20000000L,
		__GFECB_BYPT_HTMAXBUTTON	= 0x40000000L,
		__GFECB_BYPT_HT_BUTTONS		= (__GFECB_BYPT_HTGROWBOX|__GFECB_BYPT_HTMINBUTTON|__GFECB_BYPT_HTMAXBUTTON),

		__GFECB_BYPT_HT_BORDER		= (__GFECB_BYPT_HT_CORNER|__GFECB_BYPT_HT_NONCORNER),
		__GFECB_BYPT_HT_NONBORDER	= (__GFECB_BYPT_HT_BUTTONS|__GFECB_BYPT_HTNOWHERE|__GFECB_BYPT_HTSYSMENU),
		__GFECB_BYPT_HTNONCLIENT	= (__GFECB_BYPT_HT_BORDER|__GFECB_BYPT_HT_NONBORDER),

		__GFECB_BYPT_HTCLIENT		= 0x80000000L,

		__GFECB_BYPT_NC				= (__GFECB_BYPT_NCGRIP|__GFECB_BYPT_HTNONCLIENT|__GFECB_BYPT_HTCLIENT),
		__GFECB_BYPT_ALL			= (__GFECB_BYPT_ONLY_WND|__GFECB_BYPT_NC|__GFECB_BYPT_CLIENT),
	}; // enum eGFECB_t
    
    static void _GetFrameControlBars(
		CFrameWnd * pFrame,
		ExtControlBarVector_t & vBars,
		DWORD dwCallOpt = ((DWORD)__GFECB_ALL_NONRESTRICT),
		CPoint ptRestrict = CPoint( -1000, -1000 )
		);
    void _GetFrameControlBars(
		ExtControlBarVector_t & vBars,
		DWORD dwCallOpt = ((DWORD)__GFECB_ALL_NONRESTRICT),
		CPoint ptRestrict = CPoint( -1000, -1000 )
		) const;

	static CControlBar * _GetNextControlBarInRow(
		CControlBar * pBar,
		bool bNext = true, // next or previous
		bool bExtBar = false,
		bool bNonFixed = false
		);
	static CControlBar * _GetFirstControlBarInRow(
		CControlBar * pBar,
		bool bNext = false, // last or first
		bool bExtBar = false,
		bool bNonFixed = false
		);

    void _GetRowBars(
		MfcControlBarVector_t & vBars,
		CRect * pRcAllBars = NULL,
		INT * p_nOwnPos = NULL,
		bool * p_bRowMaxSized = NULL, // row with resizable bars?
		bool * p_bRowFullSized = NULL // row with menu-like bar?
		) const;
	void _GetRowExtBars(
		ExtControlBarVector_t & vBars,
		bool bNonFixedOnly = true,
		CRect * pRcAllBars = NULL,
		INT * p_nOwnPos = NULL,
		bool * p_bRowMaxSized = NULL, // row with resizable bars?
		bool * p_bRowFullSized = NULL // row with menu-like bar?
		) const;
    void _GetNextRowBars(
		bool bNextRow,
		MfcControlBarVector_t & vBars,
		CRect * pRcAllBars = NULL,
		INT * p_nOwnPos = NULL,
		bool * p_bRowMaxSized = NULL, // row with resizable bars?
		bool * p_bRowFullSized = NULL // row with menu-like bar?
		) const;

	bool _IsFirstBarInRow( bool bExtBar ) const;
	bool _IsLastBarInRow( bool bExtBar ) const;

    static CFrameWnd * _GetDockingFrameImpl(
		CControlBar * pBar
		);
    CFrameWnd * _GetDockingFrameImpl() const;

	virtual void _RecalcLayoutImpl();
	virtual void _RecalcNcArea();

protected:
	virtual bool _IsShowContentWhenDragging() const;
	virtual bool _IsShowContentWhenRowResizing() const;
	virtual bool _IsShowContentWhenRowRecalcing() const;
public:
	virtual void OnRepositionSingleChild(
		int cx = -1, // if negative - get from client area
		int cy = -1,
		bool bRedraw = true
		);

public:
	virtual void _RowResizingStart();
protected:
	virtual void _RowResizingStop( bool bCancel );
	virtual void _RowResizingUpdateState();

public:
	virtual void _RowRecalcingStart();
protected:
	virtual void _RowRecalcingStop( bool bCancel );
	virtual void _RowRecalcingUpdateState();
	
	virtual bool _RowRecalcing_IsBottomEnabled();
	virtual bool _RowRecalcing_IsTopEnabled();
	
	virtual void _DrawResizingTracker();
	virtual void _DrawRecalcingTracker();

	enum e_calc_dock_type_t
	{
		__ECDT_FLOATED				=  0,
		__ECDT_INNER_TOP			=  1,
		__ECDT_INNER_BOTTOM			=  2,
		__ECDT_INNER_LEFT			=  3,
		__ECDT_INNER_RIGHT			=  4,
		__ECDT_DOCKBAR_INNER_TOP	=  5,
		__ECDT_DOCKBAR_INNER_BOTTOM	=  6,
		__ECDT_DOCKBAR_INNER_LEFT	=  7,
		__ECDT_DOCKBAR_INNER_RIGHT	=  8,
		__ECDT_CIRCLE1_OUTER_TOP	=  9,
		__ECDT_CIRCLE1_OUTER_BOTTOM	= 10,
		__ECDT_CIRCLE1_OUTER_LEFT	= 11,
		__ECDT_CIRCLE1_OUTER_RIGHT	= 12,
		__ECDT_OWN_AREA				= 13,
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		__ECDT_TABBED_NEW			= 14,
		__ECDT_TABBED_UPDATE		= 15,
		__ECDT_TABBED_REINDEX		= 16,
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	};

	enum e_calc_fixed_resizing_t
	{
		__ECFR_NO_RESIZING = 0,
		__ECFR_HORZ = 1,
		__ECFR_VERT = 2,
	};

	class __PROF_UIS_API InternalDraggingState_t : public CObject
	{
		bool m_bEmpty:1, m_bDeepHalfSplit:1;
		e_calc_dock_type_t m_eCDT;
		e_calc_fixed_resizing_t m_eCFR;
		UINT m_nCircleNo;
		CRect m_rcTrack;
		CExtControlBar * m_pExtBarSrc;
		CExtControlBar * m_pExtBarDst;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		LONG m_nTabReindex;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		void _AssignFromOther(
			const InternalDraggingState_t & other
			);
		bool _IsEqualToOther(
			const InternalDraggingState_t & other
			) const
		{
			ASSERT_VALID( this );
			ASSERT_VALID( (&other) );
			if( m_bEmpty != other.m_bEmpty )
				return false;
			if( m_eCDT != other.m_eCDT )
				return false;
//			if( m_eCFR != other.m_eCFR )
//				return false;
			if( m_nCircleNo != other.m_nCircleNo )
				return false;
			if( m_rcTrack != other.m_rcTrack )
				return false;
			if( m_pExtBarSrc != other.m_pExtBarSrc )
				return false;
			if( m_pExtBarDst != other.m_pExtBarDst )
				return false;
			if( m_bDeepHalfSplit != other.m_bDeepHalfSplit )
				return false;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			if( m_nTabReindex != other.m_nTabReindex )
				return false;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			return true;
		}
	public:
		InternalDraggingState_t(
			CExtControlBar * pExtBarSrc = NULL
			)
			: m_bEmpty( true )
			, m_bDeepHalfSplit( false )
			, m_eCDT( __ECDT_FLOATED )
			, m_eCFR( __ECFR_NO_RESIZING )
			, m_nCircleNo( 0 )
			, m_rcTrack( 0, 0, 0, 0 )
			, m_pExtBarSrc( pExtBarSrc )
			, m_pExtBarDst( NULL )
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
			, m_nTabReindex( 0 )
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		{
		}
		InternalDraggingState_t(
			const InternalDraggingState_t & other
			)
		{
			_AssignFromOther( other );
		}

		InternalDraggingState_t & operator=(
			const InternalDraggingState_t & other
			)
		{
			_AssignFromOther( other );
			return *this;
		}
		bool operator==(
			const InternalDraggingState_t & other
			) const
		{
			return _IsEqualToOther( other );
		}
		bool operator!=(
			const InternalDraggingState_t & other
			) const
		{
			return !_IsEqualToOther( other );
		}

		e_calc_dock_type_t GetCDT() const
		{
			ASSERT_VALID( this );
			return m_eCDT;
		};
		e_calc_dock_type_t SetCDT( e_calc_dock_type_t eCDT )
		{
			//ASSERT_VALID( this );
			e_calc_dock_type_t eOldCDT = m_eCDT;
			m_eCDT = eCDT;
			return eOldCDT;
		};

		e_calc_fixed_resizing_t GetCFR() const
		{
			ASSERT_VALID( this );
			return m_eCFR;
		};
		e_calc_fixed_resizing_t SetCFR( e_calc_fixed_resizing_t eCFR )
		{
			//ASSERT_VALID( this );
			e_calc_fixed_resizing_t eOldCFR = m_eCFR;
			m_eCFR = eCFR;
			return eOldCFR;
		};

		UINT GetCircleNo() const
		{
			return m_nCircleNo;
		};
		UINT SetCircleNo( UINT nCircleNo )
		{
			UINT nCircleNoOld = m_nCircleNo;
			m_nCircleNo = nCircleNo;
			return nCircleNoOld;
		};

		CRect & TrackRectGet()
		{
			ASSERT_VALID( this );
			return m_rcTrack;
		};
		const CRect & TrackRectGet() const
		{
			ASSERT_VALID( this );
			return m_rcTrack;
		};
		void TrackRectGet( CRect & rcTrack ) const
		{
			ASSERT_VALID( this );
			rcTrack = m_rcTrack;
		};
		void TrackRectSet( const CRect & rcTrack )
		{
			ASSERT_VALID( this );
			m_rcTrack = rcTrack;
		};
		void TrackRectSetEmpty()
		{
			ASSERT_VALID( this );
			m_rcTrack.SetRectEmpty();
		};

		bool IsEmpty() const
		{
			//ASSERT_VALID( this );
			ASSERT( this != NULL );
			return m_bEmpty;
		}
		void SetEmpty(
			bool bEmpty = true
			);
		bool IsDeepHalfSplit() const
		{
			ASSERT_VALID( this );
			return m_bDeepHalfSplit;
		}
		void SetDeepHalfSplit( bool bDeepHalfSplit = true )
		{
			ASSERT_VALID( this );
			m_bDeepHalfSplit = bDeepHalfSplit;
		}

		CExtControlBar * ExtBarSrcGet()
		{
			//ASSERT_VALID( this );
			ASSERT( this != NULL );
			return m_pExtBarSrc;
		}
		CExtControlBar * ExtBarDstGet()
		{
			ASSERT_VALID( this );
			return m_pExtBarDst;
		}
		CExtControlBar * ExtBarSrcSet(
			CExtControlBar * pExtBarSrc
			)
		{
			ASSERT_VALID( this );
			ASSERT( pExtBarSrc != NULL );
			ASSERT_VALID( pExtBarSrc );
			CExtControlBar * pExtBarSrcOld = m_pExtBarSrc;
			m_pExtBarSrc = pExtBarSrc;
			ASSERT_VALID( this );
			return pExtBarSrcOld;
		}
		CExtControlBar * ExtBarDstSet(
			CExtControlBar * pExtBarDst = NULL,
			bool bNoCheckOnShutdown = false
			);

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		LONG ReindexPosGet() const
		{
			ASSERT_VALID( this );
			if( m_eCDT != __ECDT_TABBED_REINDEX )
				return (-1L);
			return m_nTabReindex;
		}

		void ReindexPosCalc( CPoint ptCursor );
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

		void DrawState();
	private:
		void _DoFixed_BasicAdjustment(
			CPoint ptTrack, // in screen coords
			bool bForceFloatMode
			);
		void _DoFixed_FloatingAdjustment(
			CPoint ptTrack // in screen coords
			);
		bool _DoFixed_OuterDockerAdjustment(
			CPoint ptTrack, // in screen coords
			CExtDockOuterBar * pOuterDocker
			);
		static void _DoFixed_StaticOuterRowSwap(
			CControlBar * pBar1,
			CControlBar * pBar2
			);
		static void _DoFixed_StaticMinimalDockSiteRecalc(
			CFrameWnd * pDockSite,
			CExtDockOuterBar * pOuterDocker
			);
	public:
		void CalcStateDynamic(
			CPoint ptTrack, // in screen coords
			bool bForceFloatMode
			);

#ifdef _DEBUG
		virtual void AssertValid() const
		{
			ASSERT( this != NULL );
			if( !m_bEmpty )
			{
				ASSERT( m_pExtBarSrc != NULL );
				ASSERT_VALID( m_pExtBarSrc );
				
				CFrameWnd * pFrameSrc =
					m_pExtBarSrc->_GetDockingFrameImpl();
				ASSERT_VALID( pFrameSrc );
				pFrameSrc;
				
				if( m_pExtBarDst != NULL )
				{
					ASSERT_VALID( m_pExtBarDst );
					
					CFrameWnd * pFrameDst =
						m_pExtBarDst->_GetDockingFrameImpl();
					ASSERT_VALID( pFrameDst );
					ASSERT( pFrameSrc == pFrameDst );
				}
			} // if( !m_bEmpty )
			else
			{
				ASSERT( m_pExtBarDst == NULL );
			} // if( !m_bEmpty )

			CObject::AssertValid();
		}
		
		virtual void Dump( CDumpContext & dc ) const
		{
			CObject::Dump( dc );
		}
#endif

	}; // class InternalDraggingState_t
	
	friend class InternalDraggingState_t;

	static InternalDraggingState_t g_DragStateOld;
	static InternalDraggingState_t g_DragStateNew;

	static bool g_bUpdatingDragState;

	virtual void _DraggingDoDeepHalfSplit(
		CExtControlBar * pExtBar,
		UINT nHalfSplitID // one of AFX_IDW_DOCKBAR_... docked value
		);
	virtual bool _DraggingApplyState(
		CExtControlBar::InternalDraggingState_t & _ds
		);
public:
	static CExtControlBar * _DraggingGetBar(); // return currently dagging controlbar
protected:
	static bool _DraggingCancel(); // returns true if dragging was really canceled
    virtual void _DraggingStart(
		const CPoint & point,
		const CPoint & pointOffset = CPoint( 0, 0 ),
		CSize sizeWaitMouseMove = CSize( 1, 1 )
		);
    virtual void _DraggingStop( bool bCancel );
    virtual void _DraggingUpdateState(
		const CPoint & point,
		bool bForceFloatMode
		);
	virtual void _Dragging_OnStart();
	virtual void _Dragging_OnStop(
		InternalDraggingState_t & _dsOld,
		InternalDraggingState_t & _dsNew,
		bool bCancel
		);

	virtual void _SetCursor(
		const CPoint & point
		);

	bool _IsSingleVisibleInFloatingPalette();

	void _GetDeepNcModeMetrics(
		bool & bEnableResizerLeft,
		bool & bEnableResizerTop,
		bool & bEnableResizerRight,
		bool & bEnableResizerBottom
		);

	void _UpdateVisibilityInChain();
	void _UpdateVisibilityInRow();

	void _ActivateOnClick();

public:

	static CFrameWnd * DockSiteFromWnd(
		CWnd * pWndStartSearch
		);
	static HWND FindHwndNotSafeForMutualPaint(
		CWnd * pWndStartSearch
		);

	static bool _IsDockSiteCustomizeMode(
		CWnd * pWndStartSearch
		);
	virtual bool _IsDockSiteCustomizeMode() const;
	
	UINT GetDockBarCircleNo() const;

protected:
	virtual bool _CanDockToInnerCircles() const;
	virtual bool _CanDockToTabbedContainers(
		CExtControlBar * pDestBar
		) const;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	virtual CExtDynTabControlBar * _GetNearestTabbedContainer();
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	void _PreSyncRowMetrics();

	virtual void OnFrameBarCheckCmd(
		bool bResizableCheckmarks = false
		);
	virtual void OnFrameBarCheckUpdate(
		CCmdUI* pCmdUI,
		bool bResizableCheckmarks = false
		);

public:
	static void stat_RedrawFloatingFrames( CFrameWnd * pDockSite );
	static void stat_RecalcBarMetrics( CFrameWnd * pDockSite );

	static bool g_bUseCmdManagerForSetMessageString;
	static void stat_SetMessageString(
		CWnd * pWnd,
		UINT nCmdID = AFX_IDS_IDLEMESSAGE
		);

	static BOOL DoFrameBarCheckCmd(
		CFrameWnd * pFrame,
		UINT nBarID,
		bool bResizableCheckmarks = false
		);
	static void DoFrameBarCheckUpdate(
		CFrameWnd * pFrame,
		CCmdUI* pCmdUI,
		bool bResizableCheckmarks = false
		);

	virtual CExtMiniDockFrameWnd * OnCreateFloatingFrame(
		DWORD dwStyle
		);

	// toggle docking state
	virtual void ToggleDocking();

	// float control bar into recent floated position
	void FloatControlBar(
		DWORD dwStyle = CBRS_ALIGN_TOP
		);
	// float control bar
	virtual void FloatControlBar(
		CPoint ptFloat,
		DWORD dwStyle = CBRS_ALIGN_TOP
		);

	// dock control bar to new row
	virtual bool DockControlBar(
		UINT nDockBarID,
		UINT nCircleNo,
		CFrameWnd * pDockSite = NULL, // can be NULL only if bar already was at once docked
		bool bRecalcLayout = true
		);

	// dock control bar relatively to other control bar
	virtual bool DockControlBar(
		CExtControlBar * pBar,
		bool bHorzBarHalfSplit,
		bool bInnerHalfSplit = false,
		CFrameWnd * pDockSite = NULL, // can be NULL only if bar already was at once docked
		bool bRecalcLayout = true
		);

	// dock control bar into tabbed container
	virtual bool DockControlBarIntoTabbedContainer(
		CExtControlBar * pBar,
		INT nIndex = -1, // append
		CFrameWnd * pDockSite = NULL, // can be NULL only if bar already was at once docked
		bool bRecalcLayout = true
		);

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	// is bar in autohide mode
	bool AutoHideModeGet() const;
	// switch to/restore from autohide mode
	bool AutoHideModeSet(
		bool bAutoHideMode,
		bool bAnimate,
		bool bRecalcLayout,
		bool bForceNoShow
		);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

private:
	typedef
		CArray < CExtBarNcAreaButton *, CExtBarNcAreaButton * >
		NcButtonsArr_t;
	NcButtonsArr_t m_vNcButtons;

public:
	virtual CRect & CExtControlBar::NcButtons_CalcGripperTextLocation(
		CRect & rcPreCalcText
		);
	UINT NcButtons_HitTest(
		CPoint point,
		CExtBarNcAreaButton ** ppBtn = NULL
		);
	void NcButtons_Paint( CDC & dc );

	INT NcButtons_GetCount() const
	{
		ASSERT( this != NULL );
		return (INT)m_vNcButtons.GetSize();
	}

	CExtBarNcAreaButton * NcButtons_GetAt( INT nIndex )
	{
		ASSERT( this != NULL );
		ASSERT( nIndex >= 0 && nIndex < m_vNcButtons.GetSize() );
		CExtBarNcAreaButton * pBtn = 
			m_vNcButtons.GetAt( nIndex );
		ASSERT_VALID( pBtn );
		return pBtn;
	}
	
	const CExtBarNcAreaButton * NcButtons_GetAt( INT nIndex ) const
	{
		ASSERT( this != NULL );
		ASSERT( nIndex >= 0 && nIndex < m_vNcButtons.GetSize() );
		const CExtBarNcAreaButton * pBtn = 
			m_vNcButtons.GetAt( nIndex );
		ASSERT_VALID( pBtn );
		return pBtn;
	}
	
	void NcButtons_SetAt(
		INT nIndex,
		CExtBarNcAreaButton * pBtn
		)
	{
		ASSERT( this != NULL && pBtn != NULL );
		if( nIndex < 0 )
		{
			m_vNcButtons.Add( pBtn );
			return;
		}
		ASSERT_VALID( pBtn );
		ASSERT( nIndex >= 0 && nIndex <= m_vNcButtons.GetSize() );
		if( nIndex != m_vNcButtons.GetSize() )
		{
			CExtBarNcAreaButton * pBtnOld =
				m_vNcButtons.GetAt( nIndex );
			ASSERT_VALID( pBtnOld );
			if( pBtnOld != pBtn )
			{
				pBtnOld->OnNcDestroy();
				m_vNcButtons.SetAt( nIndex, pBtn );
			}
			return;
		} // if( nIndex != m_vNcButtons.GetSize() )
		m_vNcButtons.Add( pBtn );
	}
	
	void NcButtons_Add( CExtBarNcAreaButton * pBtn )
	{
		NcButtons_SetAt( -1, pBtn );
	}
	
	void NcButtons_RemoveAt( INT nIndex )
	{
		ASSERT( this != NULL );
		ASSERT( nIndex >= 0 && nIndex < m_vNcButtons.GetSize() );
		CExtBarNcAreaButton * pBtnOld =
			m_vNcButtons.GetAt( nIndex );
		ASSERT_VALID( pBtnOld );
		pBtnOld->OnNcDestroy();
	}
	
	void NcButtons_RemoveAll()
	{
		INT nCount = (INT)m_vNcButtons.GetSize();
		for( INT nIndex = 0; nIndex < nCount; nIndex++ )
		{
			CExtBarNcAreaButton * pBtnOld =
				m_vNcButtons.GetAt( nIndex );
			ASSERT_VALID( pBtnOld );
			pBtnOld->OnNcDestroy();
		}
		m_vNcButtons.RemoveAll();
		m_vNcButtons.FreeExtra();
	}

	virtual void OnNcAreaButtonsReinitialize();
	virtual void OnNcAreaButtonsReposition();


	static bool g_bControlBarClassRegistered;
	static bool RegisterControlBarClass();

	static void RepositionBarsEx(
		CWnd * pWndToReposChilds,
		UINT nIDFirst,
		UINT nIDLast,
		UINT nIDLeftOver,
		UINT nFlag = 0,
		LPRECT lpRectParam = NULL,
		LPCRECT lpRectClient = NULL,
		BOOL bStretch = TRUE
		);
	
	virtual LRESULT DoHelpHitTest( CPoint ptHelpHitTest );

	virtual	bool _OnMouseMoveMsg(UINT nFlags, CPoint point);

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtControlBar)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	friend class CExtBarButton;
	friend class CExtBarContentExpandButton;
	friend class CExtBarMdiDocButton;
	friend class CExtDockBar;
	friend class CExtDockOuterBar;
	friend class CExtToolControlBar;
	friend class CExtMenuControlBar;
	friend class CExtMiniDockFrameWnd;
	friend class CExtBarNcAreaButton;
	friend struct InternalBarRepositionData_t;
	friend class CExtDynControlBar;
	friend class CExtDockDynBar;
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	friend class CExtTabMdiWnd;
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	friend class CExtDockDynTabBar;
	friend class CExtDynTabWnd;
	friend class CExtDynTabControlBar;
	friend class CExtDynAutoHideArea;
	friend class CExtDynAutoHideSlider;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	friend class InternalDockStateBar;
	friend class InternalDockStateSite;
	friend class InternalFriendlyFrameWnd;
};

#define __EXT_CONTROLBAR_CLASS_NAME  _T("ProfUIS-ControlBar")


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_CONTROLBAR_H
