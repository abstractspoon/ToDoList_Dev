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

#if (!defined __EXT_TOOLCONTROLBAR_H)
#define __EXT_TOOLCONTROLBAR_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif // __EXT_CMD_ICON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CExtCmdItem;

/////////////////////////////////////////////////////////////////////////////
// CExtBarButton

class __PROF_UIS_API CExtBarButton : public CObject
{
public:
	DECLARE_DYNCREATE(CExtBarButton);

protected:
	CExtToolControlBar * m_pBar;
	UINT	m_nButtonCmdID; // Command ID or ID_SEPARATOR
	UINT	m_nEffectiveCmdID;
	UINT	m_nStyle; // button styles
	
	CSize	m_ActiveSize;
	CRect	m_ActiveRect;
	
	bool	m_bWrapH:1, m_bWrapV:1, m_bWrapF:1,
			m_bVisible:1,
			m_bHover:1,
			m_bCtrlAutoDestroyed:1,
			m_bVertDocked:1,
			m_bPopupMenu:1,
			m_bAutoDestroyMenu:1,
			m_bAppendMdiWindowsMenu:1,
			m_bSeparatedDropDown:1,
			m_bAutoChangeID:1,
			m_bDropDownHT:1,
			m_bNoRotateVerticalLayout:1,
			m_bCtrlVisibleVert:1,
			m_bPressedTracking:1
			;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	CExtCustomizeCmdTreeNode * m_pCmdNodeI, * m_pCmdNodeC;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	CWnd *	m_pCtrl;
	HMENU m_hMenu;

	void _InitMembers()
	{
		m_pBar = NULL;
		m_nButtonCmdID = m_nEffectiveCmdID = ID_SEPARATOR;
		m_pCtrl = NULL;
		m_bCtrlVisibleVert = false;
		m_bCtrlAutoDestroyed = false;
		m_nStyle = 0;
		m_ActiveSize.cx = m_ActiveSize.cy = 0;
		m_ActiveRect.SetRectEmpty();
		m_bWrapH = m_bWrapV = m_bWrapF = m_bHover = false;
		m_bVisible = true;
		m_bVertDocked = false;
		m_hMenu = NULL;
		m_bPopupMenu = false;
		m_bAutoDestroyMenu = false;
		m_bAppendMdiWindowsMenu = false;
		m_bSeparatedDropDown = false;
		m_bAutoChangeID = false;
		m_bDropDownHT = false;
		m_bNoRotateVerticalLayout = false;
		m_bPressedTracking = false;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		m_pCmdNodeI = m_pCmdNodeC = NULL;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	}

	virtual void _UpdateCtrl()
	{
		if( m_pCtrl == NULL )
			return;
		ASSERT_VALID( m_pCtrl );
		m_pCtrl->ShowWindow(
			(m_bVisible
			&& (!m_bVertDocked || GetCtrlVisibleVertically())
			&& (m_nStyle & TBBS_HIDDEN) == 0
			) ? SW_SHOW : SW_HIDE
			);
		m_pCtrl->EnableWindow( IsEnabled() );
		if( m_pCtrl->IsKindOf(RUNTIME_CLASS(CButton)) )
		{
			int nCheck = 0;
			if( IsPressed() )
				nCheck = 1;
			else if( IsIndeterminate() )
				nCheck = 2;
			((CButton *)(m_pCtrl))->SetCheck( nCheck );
		}
//		if( m_bVisible && m_bHover )
//			m_pCtrl->SetFocus();
	}
	void _UpdateCmdIdFromStyle()
	{
		if( m_nStyle & TBBS_SEPARATOR )
		{
			m_nStyle &= ~(TBBS_BUTTON);
			m_nButtonCmdID = m_nEffectiveCmdID = ID_SEPARATOR;
		}
		else
		{
			m_nStyle |= TBBS_BUTTON;
		}
	}

	void _DestroyMenu()
	{
		if( m_hMenu == NULL )
			return;
		if( m_bAutoDestroyMenu )
		{
			ASSERT( ::IsMenu(m_hMenu) );
			VERIFY( ::DestroyMenu(m_hMenu) );
		}
		m_hMenu = NULL;
	}

public:
	CExtBarButton(
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		)
	{
		_InitMembers();
		//ASSERT( pBar != NULL );
		m_pBar = pBar;
		m_nStyle = nStyle;
		SetCmdID( nCmdID );
	}
	~CExtBarButton()
	{
		_DestroyMenu();
	}

	CExtToolControlBar * GetSafeBar()
	{
		ASSERT( m_pBar != NULL );
		return m_pBar;
	}
	const CExtToolControlBar * GetSafeBar() const
	{
		ASSERT( m_pBar != NULL );
		return m_pBar;
	}
	CExtToolControlBar * GetBar()
	{
		return m_pBar;
	}
	const CExtToolControlBar * GetBar() const
	{
		return m_pBar;
	}
	void SetBar( CExtToolControlBar * pBar )
	{
		m_pBar = pBar;
	}
	virtual bool IsLargeIcon() const;
	virtual bool IsDisplayScreenTip() const;
	virtual bool IsDisplayShortcutKeysOnScreenTip() const;

	virtual UINT GetCmdID(
		bool bEffective = true
		) const;
	virtual void SetCmdID(
		UINT nCmdID,
		bool bEffectiveOnly = false,
		bool bEffectiveRecalcLayout = false
		);

	virtual bool GetSeparatedDropDown() const;
	virtual void SetSeparatedDropDown(
		bool bSeparatedDropDown = true
		);
	virtual bool GetAutoChangeID() const;
	virtual void SetAutoChangeID(
		bool bAutoChangeID = true
		);
	virtual bool GetNoRotateVerticalLayout() const;
	virtual void SetNoRotateVerticalLayout(
		bool bNoRotateVerticalLayout = true
		);
	virtual bool GetCtrlVisibleVertically() const;
	virtual void SetCtrlVisibleVertically( bool bVisible );

	virtual CWnd * CtrlGet()
	{
		return m_pCtrl;
	}
	virtual void CtrlSet(
		CWnd * pCtrl,
		bool bCtrlAutoDestroyed
		)
	{
		if( m_pCtrl != NULL )
		{
			if( ::IsWindow(m_pCtrl->GetSafeHwnd()) )
				m_pCtrl->DestroyWindow();
			if( !m_bCtrlAutoDestroyed )
				delete m_pCtrl;
		}
		m_pCtrl = pCtrl;
		m_bCtrlAutoDestroyed = bCtrlAutoDestroyed;
#ifdef _DEBUG
		if( m_pCtrl != NULL )
		{
			ASSERT_VALID( m_pCtrl );
		}
#endif // _DEBUG
	}

	virtual UINT GetStyle() const;
	virtual void SetStyle( UINT nStyle );
	virtual void ModifyStyle(
		UINT nAdd,
		UINT nRemove = 0
		);
	virtual void OnPressedTrackingStart()
	{
		m_bPressedTracking = true;
	}
	virtual void OnPressedTrackingStop()
	{
		m_bPressedTracking = false;
	}
	virtual bool IsPressedTracking() const
	{
		return m_bPressedTracking;
	}

	virtual void SetVertDocked( bool bVertDocked )
	{
		m_bVertDocked = bVertDocked;
	}

	enum e_wrap_t
	{
		__EVT_HORZ		= 0,
		__EVT_VERT		= 1,
		__EVT_FLOAT		= 2,
		__EVT_MIN_VALUE = 0,
		__EVT_MAX_VALUE = 2,
	};
	virtual bool IsWrap(
		CExtBarButton::e_wrap_t _evt
		) const;
	virtual void SetWrap(
		CExtBarButton::e_wrap_t _evt,
		bool bWrap = true
		);

	virtual bool IsSeparator() const
	{
		bool bSeparator =
			(GetStyle() & TBBS_SEPARATOR) ?
				true : false;
		return bSeparator;
	}
	virtual bool IsDisabled() const
	{
		bool bDisabled =
			(GetStyle() & TBBS_DISABLED) ?
				true : false;
		return bDisabled;
	}
	bool IsEnabled() const
	{
		return !( IsDisabled() );
	}
	virtual bool IsIndeterminate() const
	{
		bool bIndeterminate =
			(GetStyle() & TBBS_INDETERMINATE) ?
				true : false;
		return bIndeterminate;
	}
	virtual bool IsPressed() const
	{
		bool bPressed =
			(GetStyle() & (TBBS_PRESSED|TBBS_CHECKED)) ?
				true : false;
		return bPressed;
	}

	virtual void Show( bool bShow = true )
	{
		m_bVisible = bShow;
		_UpdateCtrl();
	}
	virtual bool IsVisible() const
	{
		return m_bVisible; // && ( (m_nStyle & TBBS_HIDDEN) == 0 );
	}

	virtual void SetHover( bool bHover = true )
	{
		m_bHover = bHover;
		_UpdateCtrl();
	}
	virtual bool IsHover() const
	{
		return m_bHover;
	}

	virtual CExtSafeString GetText() const;
	
	virtual void SetRect( const RECT & rectButton )
	{
		m_ActiveRect = rectButton;
		if( (m_pCtrl != NULL) && (!m_bVertDocked || GetCtrlVisibleVertically()) )
		{
			ASSERT_VALID( m_pCtrl );
			//m_pCtrl->MoveWindow( &m_ActiveRect,FALSE );
			CRect rcCtrl;
			m_pCtrl->GetWindowRect( &rcCtrl );
			CRect rcMove(
				m_ActiveRect.TopLeft(),
				rcCtrl.Size()
				);
			m_pCtrl->MoveWindow( &rcMove,FALSE );
		} // if( (m_pCtrl != NULL) && (!m_bVertDocked || GetCtrlVisibleVertically()) )
		_UpdateCtrl();
	}
	virtual CRect Rect() const;
	virtual CRect RectDropDown() const;
	operator CRect() const
	{
		return Rect();
	}

	virtual CSize Size() const
	{
		return m_ActiveSize;
	}
	operator CSize() const
	{
		return m_ActiveSize;
	}

	virtual void SetMenu(
		HMENU hMenu,
		bool bPopupMenu,
		bool bAutoDestroyMenu
		);
	virtual HMENU GetMenu()
	{
		return m_hMenu;
	}
	operator HMENU()
	{
		return GetMenu();
	}
	virtual void SetAppendMdiWindowsMenu(
		bool bAppendMdiWindowsMenu
		)
	{
		m_bAppendMdiWindowsMenu = bAppendMdiWindowsMenu;
	}
	virtual bool IsAppendMdiWindowsMenu()
	{
		return m_bAppendMdiWindowsMenu;
	}

	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		);
	virtual bool IsContainerOfPopupLikeMenu();

	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual CExtCmdIcon * GetIconPtr();

	virtual CWnd * GetCmdTargetWnd();

	virtual void Paint(
		CDC & dc,
		bool bHorz
		);

	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny
		);
	virtual void OnHover(
		CPoint point,
		bool bOn
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);
	virtual void OnDeliverCmd();
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);
	virtual LRESULT OnHelpHitTest(
		CPoint point
		);
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);

	virtual bool CanStartCommandDrag();
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
protected:
	virtual void _UpdateCmdNodeFromStyle();
public:
	virtual bool IsCustomizeActiveItem();
	virtual void SetCustomizeActiveItem();
	virtual int CalculateDropOverEffect(
		CPoint point,
		CExtCustomizeSite::CCmdDragInfo & _dragInfo,
		bool bAllowTrackNodePopup = true
		);
	virtual CExtCustomizeCmdTreeNode * GetCmdNode(
		bool bInitial = false
		);
	virtual void OnCustomizeUpdateProps(
		CExtCustomizeCmdTreeNode * pNode = NULL // update from basic node
		);
	virtual void SetBasicCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual void SetCustomizedCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CExtCustomizeCmdTreeNode * OnCustomizeNodeInit(
		CExtCustomizeSite * pSite,
		CExtCustomizeCmdTreeNode * pParentNode // toolbar node
		);
#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
	virtual int GetInitialResizingStateH( // -1 - left side resizing, 1 - right side resizing, 0 - no resizing at specified point (in bar's client coord)
		CPoint point,
		int * p_nWidthMin = NULL,
		int * p_nWidthMax = NULL
		);
#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	virtual void RedrawButton(
		bool bUpdateNow = false
		);
	virtual UINT OnGetTrackPopupFlags();
	virtual bool OnSetCursor( CPoint point );

}; // class CExtBarButton

/////////////////////////////////////////////////////////////////////////////
// CExtBarColorButton

class __PROF_UIS_API CExtBarColorButton : public CExtBarButton
{
	COLORREF m_clrDefault, m_clrSelected;
	LPARAM m_lParamCookie;
	bool m_bEnableBtnColorDefault:1, m_bEnableBtnColorCustom:1;
	CExtSafeString m_sBtnTextColorDefault, m_sBtnTextColorCustom;

public:
	enum e_def_icon_type_t
	{
		__DIT_BOX	= 0,
		__DIT_FRAME	= 1,
		__DIT_CHAR	= 2,
	};

private:
	e_def_icon_type_t m_eDIT;

public:
	virtual void OnSyncIcon();

public:
	DECLARE_DYNCREATE(CExtBarColorButton);

	CExtBarColorButton(
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0,
		COLORREF clrDefault = RGB(0,0,0),
		COLORREF clrSelected = RGB(0,0,0),
		LPARAM lParamCookie = 0L,
		bool bEnableBtnColorDefault = true,
		bool bEnableBtnColorCustom = true,
		__EXT_MFC_SAFE_LPCTSTR sBtnTextColorDefault = NULL,
		__EXT_MFC_SAFE_LPCTSTR sBtnTextColorCustom = NULL,
		e_def_icon_type_t eDIT = __DIT_BOX
		)
		: CExtBarButton( pBar, nCmdID, nStyle )
		, m_clrDefault( clrDefault )
		, m_clrSelected( clrSelected )
		, m_lParamCookie( lParamCookie )
		, m_bEnableBtnColorDefault( bEnableBtnColorDefault )
		, m_bEnableBtnColorCustom( bEnableBtnColorCustom )
		, m_sBtnTextColorDefault( (sBtnTextColorDefault==NULL) ? _T("") : sBtnTextColorDefault )
		, m_sBtnTextColorCustom( (sBtnTextColorCustom==NULL) ? _T("") : sBtnTextColorCustom )
		, m_eDIT( eDIT )
	{
		OnSyncIcon();
	}

	virtual COLORREF ColorGet( bool bSelected ) const;
	virtual void ColorSet( COLORREF clr, bool bSelected );
	virtual LPARAM LParamCookieGet() const;
	virtual void LParamCookieSet(
		LPARAM lParamCookie = 0L
		);
	virtual bool IsEnabledBtnColorDefault() const;
	virtual void EnableBtnColorDefault(
		bool bEnable = true
		);
	virtual bool IsEnabledBtnColorCustom() const;
	virtual void EnableBtnColorCustom(
		bool bEnable = true
		);
	virtual __EXT_MFC_SAFE_LPCTSTR BtnTextDefaultGet() const;
	virtual void BtnTextDefaultSet(
		__EXT_MFC_SAFE_LPCTSTR sText = NULL
		);
	virtual __EXT_MFC_SAFE_LPCTSTR BtnTextCustomGet() const;
	virtual void BtnTextCustomSet(
		__EXT_MFC_SAFE_LPCTSTR sText = NULL
		);
	e_def_icon_type_t IconTypeGet() const
	{
		ASSERT_VALID( this );
		return m_eDIT;
	}
	void IconTypeSet( e_def_icon_type_t eDIT = __DIT_BOX )
	{
		ASSERT_VALID( this );
		if( m_eDIT != eDIT )
		{
			m_eDIT = eDIT;
			OnSyncIcon();
		}
	}

	virtual void OnGenerateColorIcon( HICON * p_hIcon );

	virtual CExtCmdIcon * GetIconPtr();
	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		);
	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny
		);
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);
	virtual LRESULT OnHelpHitTest(
		CPoint point
		);
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	
	virtual bool CanStartCommandDrag();
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual int CalculateDropOverEffect(
		CPoint point,
		CExtCustomizeSite::CCmdDragInfo & _dragInfo,
		bool bAllowTrackNodePopup = true
		);
	virtual CExtCustomizeCmdTreeNode * GetCmdNode(
		bool bInitial = false
		);
	virtual void SetBasicCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual void SetCustomizedCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CExtCustomizeCmdTreeNode * OnCustomizeNodeInit(
		CExtCustomizeSite * pSite,
		CExtCustomizeCmdTreeNode * pParentNode // toolbar node
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	virtual void OnDeliverCmd();

}; // class CExtBarColorButton


/////////////////////////////////////////////////////////////////////////////
// CExtBarContentExpandButton

typedef
	CArray < CExtBarButton *, CExtBarButton * >
	_tb_buttons_container_t;

#define __ID_TOOLBAR_RIGHT_BUTTON__ ((UINT)-66)
#define __RIGHT_BUTTON_HORZ_DX__ 11 // 14
#define __RIGHT_BUTTON_VERT_DY__ 11 // 12

class CExtPopupBaseWnd;
class CExtPopupMenuWnd;
class CExtPopupMenuSite;
class CExtMiniDockFrameWnd;

class __PROF_UIS_API CExtBarContentExpandButton : public CExtBarButton
{
public:
	DECLARE_DYNCREATE(CExtBarContentExpandButton);

protected:
	bool m_bClearContent:1;

	_tb_buttons_container_t m_buttons;

public:
	CExtBarContentExpandButton(
		CExtToolControlBar * pBar = NULL
		) :
		CExtBarButton(pBar)
	{
		SetCmdID( __ID_TOOLBAR_RIGHT_BUTTON__ );
	}
	~CExtBarContentExpandButton()
	{
		CExtBarButton::~CExtBarButton();
	}

	bool IsContentClear()
	{
		ASSERT_VALID( this );
		return m_bClearContent;
	}
	void ClearContent( bool bClearContent = true )
	{
		if( this == NULL )
			return;
		ASSERT_VALID( this );
		m_bClearContent = bClearContent;
	}

	const _tb_buttons_container_t & GetButtons() const
	{
		return m_buttons;
	}
	_tb_buttons_container_t & GetButtons()
	{
		return m_buttons;
	}

	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual void Paint(
		CDC & dc,
		bool bHorz
		);

	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny
		);
	virtual void OnHover(
		CPoint point,
		bool bOn
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);
	virtual LRESULT OnHelpHitTest(
		CPoint point
		);

	virtual bool CanStartCommandDrag();
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual CExtCustomizeCmdTreeNode * GetCmdNode(
		bool bInitial = false
		);
	virtual void SetBasicCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual void SetCustomizedCmdNode(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CExtCustomizeCmdTreeNode * OnCustomizeNodeInit(
		CExtCustomizeSite * pSite,
		CExtCustomizeCmdTreeNode * pParentNode // toolbar node
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

}; // class CExtBarContentExpandButton

/////////////////////////////////////////////////////////////////////////////
// CExtBarSliderButton

class __PROF_UIS_API CExtBarSliderButton : public CExtBarButton
{
public:
	enum e_SliderArea_t
	{
		__ESA_ICON_AND_TEXT = 0, // both icon and text
		__ESA_SLIDER = 1, // all slider
		__ESA_SCROLLABLE = 2, // scrollable part of slider
		__ESA_PAGE_UP = 3,
		__ESA_PAGE_DOWN = 4,
		__ESA_LINE_UP = 5,
		__ESA_LINE_DOWN = 6,
		__ESA_THUMB = 7,
		__ESA_COUNT = 8,
		__ESA_UNKNOWN_OR_NOWHERE = -1,
		__ESA_PAINT_RANGE_FIRST = __ESA_SLIDER,
		__ESA_PAINT_RANGE_LAST = __ESA_THUMB,
		__ESA_CLICKABLE_RANGE_FIRST = __ESA_PAGE_UP,
		__ESA_CLICKABLE_RANGE_LAST = __ESA_THUMB,
	};
protected:
	ULONG m_nScrollTotalRange, m_nScrollPos, m_nScrollPageSize;
	INT m_nScrollButtonExtentH, m_nScrollButtonExtentV,
		m_nScrollControlExtentH, m_nScrollControlExtentV;
	CRect m_arrAreaRects[__ESA_COUNT];
	e_SliderArea_t m_eAreaTracked, m_eAreaPressed;
public:
	DECLARE_DYNCREATE(CExtBarSliderButton);
	CExtBarSliderButton(
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0,
		ULONG nScrollTotalRange = 0L,
		ULONG nScrollPos = 0L,
		ULONG nScrollPageSize = 0L,
		INT nScrollButtonExtentH = 0,
		INT nScrollButtonExtentV = 0,
		INT nScrollControlExtentH = 50,
		INT nScrollControlExtentV = 50
		)
		: CExtBarButton( pBar, nCmdID, nStyle )
		, m_nScrollTotalRange( nScrollTotalRange )
		, m_nScrollPos( nScrollPos )
		, m_nScrollPageSize( nScrollPageSize )
		, m_nScrollButtonExtentH( nScrollButtonExtentH )
		, m_nScrollButtonExtentV( nScrollButtonExtentV )
		, m_nScrollControlExtentH( nScrollControlExtentH )
		, m_nScrollControlExtentV( nScrollControlExtentV )
		, m_eAreaTracked( __ESA_UNKNOWN_OR_NOWHERE )
		, m_eAreaPressed( __ESA_UNKNOWN_OR_NOWHERE )
	{
		for(	int nRectNo = 0;
				nRectNo < __ESA_COUNT;
				nRectNo++
				)
				m_arrAreaRects[ nRectNo ].
					SetRect( 0, 0, 0, 0 );
		ASSERT( m_nScrollPos <= m_nScrollTotalRange );
		ASSERT( m_nScrollButtonExtentH >= 0 );
		ASSERT( m_nScrollButtonExtentV >= 0 );
		ASSERT( m_nScrollControlExtentH >= 0 );
		ASSERT( m_nScrollControlExtentV >= 0 );
	}
	ULONG ScrollLimitGet() const;
	virtual ULONG ScrollTotalRangeGet() const;
	virtual ULONG ScrollTotalRangeSet( ULONG nScrollTotalRange );
	virtual ULONG ScrollPosGet() const;
	virtual ULONG ScrollPosSet( ULONG nScrollPos );
	virtual ULONG ScrollPageSizeGet() const;
	virtual ULONG ScrollPageSizeSet( ULONG nScrollPageSize );
	virtual INT ScrollButtonExtentHorzGet() const;
	virtual INT ScrollButtonExtentHorzSet( INT nScrollButtonExtent );
	virtual INT ScrollButtonExtentVertGet() const;
	virtual INT ScrollButtonExtentVertSet( INT nScrollButtonExtent );
	virtual INT ScrollControlExtentHorzGet() const;
	virtual INT ScrollControlExtentHorzSet( INT nScrollControlExtent );
	virtual INT ScrollControlExtentVertGet() const;
	virtual INT ScrollControlExtentVertSet( INT nScrollControlExtent );
	virtual void RecalcSliderLayout();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CExtBarSliderButton::e_SliderArea_t GetTrackedArea() const;
	CExtBarSliderButton::e_SliderArea_t GetPressedArea() const;
	CRect GetAreaRect( CExtBarSliderButton::e_SliderArea_t eSA ) const;

	// slider button overrides
	virtual void OnPaintScrollArea(
		CDC & dc,
		CExtBarSliderButton::e_SliderArea_t eSA,
		bool bHorz
		);

	// CExtBarButton overrides
	virtual bool GetSeparatedDropDown() const
	{
		ASSERT_VALID( this );
		return false;
	}
	virtual void SetSeparatedDropDown( bool bSeparatedDropDown = true )
	{
		ASSERT_VALID( this );
		ASSERT( FALSE );
		bSeparatedDropDown;
		return;
	}
	virtual bool GetAutoChangeID() const
	{
		ASSERT_VALID( this );
		return false;
	}
	virtual void SetAutoChangeID( bool bAutoChangeID = true )
	{
		ASSERT_VALID( this );
		ASSERT( FALSE );
		bAutoChangeID;
		return;
	}
	virtual void CtrlSet(
		CWnd * pCtrl,
		bool bCtrlAutoDestroyed
		)
	{
		ASSERT_VALID( this );
		ASSERT( FALSE );
		pCtrl;
		bCtrlAutoDestroyed;
		return;
	}
//	virtual bool GetNoRotateVerticalLayout() const
//	{
//		return true;
//	}
//	virtual void SetNoRotateVerticalLayout( bool bNoRotateVerticalLayout = true )
//	{
//		ASSERT( FALSE );
//		bNoRotateVerticalLayout;
//		return;
//	}
	virtual void SetMenu(
		HMENU hMenu,
		bool bPopupMenu,
		bool bAutoDestroyMenu
		);
	virtual HMENU GetMenu();
//	virtual bool IsLargeIcon() const;
//	virtual bool IsDisplayScreenTip() const;
//	virtual bool IsDisplayShortcutKeysOnScreenTip() const;
	virtual void SetRect( const RECT & rectButton );
	virtual bool IsAbleToTrackMenu(
		bool bCustomizeMode = false
		);
	virtual bool IsContainerOfPopupLikeMenu();
//	virtual CExtCmdIcon * GetIconPtr();
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	virtual void Paint(
		CDC & dc,
		bool bHorz
		);
	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny
		);
	virtual void OnHover(
		CPoint point,
		bool bOn
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);
	virtual void OnDeliverCmd();
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		);
	virtual LRESULT OnHelpHitTest(
		CPoint point
		);
	virtual void OnUpdateCmdUI(
		CWnd * pTarget,
		BOOL bDisableIfNoHndler,
		int nIndex
		);
	virtual bool OnSetCursor( CPoint point );
}; // class CExtBarSliderButton

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar

class __PROF_UIS_API CExtToolControlBar : public CExtControlBar
{
public:
	DECLARE_DYNCREATE(CExtToolControlBar);

private:
	friend class CExtBarButton;
	friend class CExtBarContentExpandButton;
	friend class CExtBarMdiDocButton;
	class CExtToolControlBarCmdUI;
	friend class CExtPopupBaseWnd;
	friend class CExtPopupMenuWnd;
	friend class CExtPopupMenuSite;
	friend class CExtMiniDockFrameWnd;

public:
	bool IsRightExpandButton(int nBtnIdx);
protected:
	CExtBarContentExpandButton * m_pRightBtn;

	_tb_buttons_container_t m_buttons;
	
	void _RemoveAllButtonsImpl();
	int _GetButtonsCountImpl() const;

	virtual void _SetCursor(
		const CPoint & point
		);

// Construction
public:
	CExtToolControlBar();

	BOOL LoadToolBar(
		__EXT_MFC_SAFE_LPCTSTR lpszResourceName
		);
	BOOL LoadToolBar(
		UINT nIDResource
		)
	{
		return
			LoadToolBar(
				__EXT_MFC_SAFE_LPCTSTR( MAKEINTRESOURCE(nIDResource) )
				);
	}
	
	BOOL InsertButton(
		int nPos = -1, // append
		UINT nCmdID = ID_SEPARATOR,
		BOOL bDoRecalcLayout = TRUE
		);
	BOOL InsertSpecButton(
		int nPos, // -1 - append
		CExtBarButton * pButton,
		BOOL bDoRecalcLayout = TRUE
		);
	BOOL RemoveButton(
		int nPos,
		BOOL bDoRecalcLayout = TRUE
		);

	BOOL SetButtons(
		const UINT * lpIDArray = NULL,
		int nIDCount = 0
		); // lpIDArray can be NULL to allocate empty buttons
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	BOOL SetButtons(
		CExtCustomizeCmdTreeNode * pNode
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

// Attributes
public:

	static const UINT g_nMsgTrackButtonMenu;
	static const UINT g_nMsgContinueTrackMenu;
	bool m_bRightButtonDisplayBarsList:1;
	bool m_bPaletteMode:1;

	int CommandToIndex( UINT nIDFind ) const;
	UINT GetButtonID( int nIndex ) const;
	virtual void GetButtonRect( int nIndex, LPRECT lpRect ) const;
	UINT GetButtonStyle( int nIndex ) const;
	void SetButtonStyle( int nIndex, UINT nStyle );

	CExtBarButton::e_wrap_t GetBtnWrapCode() const;

protected:
	virtual bool _IsShowContentWhenDragging() const;
	
public:

	virtual bool IsLargeIcons() const;
	virtual bool IsDisplayScreenTips() const;
	virtual bool IsDisplayShortcutKeysOnScreenTips() const;
	virtual LRESULT DoHelpHitTest( CPoint ptHelpHitTest );

	virtual CExtBarButton * OnCreateBarCommandBtn(
		UINT nCmdID,
		UINT nStyle = 0
		);
	virtual CExtBarContentExpandButton * OnCreateBarRightBtn();
	BOOL InitContentExpandButton();

	// for changing button info
	void GetButtonInfo(
		int nIndex,
		UINT & nID,
		UINT & nStyle
		) const;
	void SetButtonInfo(
		int nIndex,
		UINT nID,
		UINT nStyle
		);
	CExtBarButton * GetButton( int nIndex );
	CExtBarContentExpandButton * GetRightButton();

	CWnd * GetButtonCtrl(
		int nIndex
		);
	bool SetButtonCtrl(
		int nIndex,
		CWnd * pCtrl = NULL,
		bool bCtrlAutoDestroyed = TRUE
		);

	void SetButtonCtrlVisibleVertically(
		int nIndex,
		bool bVisible = true
		);
	bool GetButtonCtrlVisibleVertically(
		int nIndex
		) const;

	int GetButtonByAccessKey( __EXT_MFC_SAFE_TCHAR vkTCHAR );

	HMENU GetButtonMenu(
		int nIndex
		);
	bool SetButtonMenu(
		int nIndex,
		HMENU hMenu = NULL,
		bool bPopupMenu = TRUE,
		bool bAutoDestroyMenu = TRUE,
		bool bDoRecalcLayout = TRUE
		);
	bool MarkButtonAsMdiWindowsMenu(
		int nIndex,
		bool bAppendMdiWindowsMenu = TRUE
		);
	virtual BOOL TrackButtonMenu(
		int nIndex,
		BOOL bSelectAny = FALSE,
		BOOL bDelayedTracking = FALSE
		);
	int GetVisibleButton(
		int nBtnIdx,
		BOOL bNext,
		BOOL bPassDisabled = TRUE
		);

	int GetButtonsCount() const
	{
		return _GetButtonsCountImpl();
	}
	int GetMenuTrackingButton() const
	{
		return m_nBtnIdxMenuTracking;
	}

// Implementation
public:
	virtual ~CExtToolControlBar();

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz
		);
    virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD dwMode
		);

	virtual void ToggleDocking();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CExtBarButton* _GetButtonPtr( int nIndex ) const;
	void _InvalidateButton( int nIndex );
	void _UpdateButton( int nIndex );

protected:
	bool _UpdateHoverButton(
		CPoint point = CPoint(-1,-1) // default is use ::GetCursorPos()
		);

	virtual DWORD RecalcDelayShow(
		AFX_SIZEPARENTPARAMS * lpLayout
		);

	virtual void _RecalcPositionsImpl();
	virtual void _RecalcLayoutImpl();

public:
	// rect should have client area size
	virtual void _CalcInsideRect(
		CRect & rect,
		BOOL bHorz
		) const;
	
	virtual CSize _CalcLayout(
		DWORD dwMode,
		int nLength = -1
		);
	virtual CSize _CalcSize( BOOL bVerticallyDocked );
	virtual void _SizeToolBar(
		int nLength,
		BOOL bVert = FALSE
		);
	virtual int _WrapToolBarV(
		int nHeight
		);
	virtual int _WrapToolBarH(
		int nWidth,
		int nHeight = 32767
		);
protected:
    virtual void _DraggingUpdateState(
		const CPoint & point,
		bool bForceFloatMode
		);
	virtual CSize _CalcDesiredMinOuterSize( BOOL bHorz );
	CExtControlBar::InternalAffixmentData m_AffixmentData;
	virtual CExtControlBar::InternalAffixmentData *
		_AffixmentGetOuter();
	virtual void _AffixmentSetOuter(
		CExtControlBar::InternalAffixmentData * pAffixmentData
		);
protected:
	bool m_bHelperTempFullRowMode:1, m_bInsideCalcLayout:1;
public:
	virtual bool _AffixmentIsAlignedHW() const;
	virtual bool _GetFullRowMode() const;
	static LRESULT _ContinueHelpHitTestMode(
		CExtToolControlBar * pBar
		);

public:
	virtual CSize _GetDefButtonSize()
	{
		return g_sizeDefTBB;
	}
	int _GetIndexOf( CExtBarButton * pTBB )
	{
		if( pTBB == NULL )
			return -1;
		int nCountOfButtons = (int)m_buttons.GetSize();
		for( int nBtnIdx=0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		{
			ASSERT( m_buttons[nBtnIdx] != NULL );
			if( m_buttons[nBtnIdx] == pTBB )
				return nBtnIdx;
		}
		return -1;
	}

public:
	void _SwitchMenuTrackingIndex(
		int iNewMenuTrackingIndex = -1
		);
	static CExtToolControlBar * _GetMenuTrackingBar();
	static void _CloseTrackingMenus();
	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);

public:
	static bool g_bMenuTracking;
	static bool g_bMenuTrackingExpanded;
	static bool g_bToolbarLargeIcons;
	static bool g_bToolbarScreenTips;
	static bool g_bToolbarShortcutKeysOnScreenTips;

protected:
	int m_nBtnIdxMenuTracking;
	static const CSize g_sizeDefTBB; // default size of button
	int m_nBtnIdxCapture;    // index of button with capture (-1 => none)
	int m_nBtnIdxHover;

public:
	int _GetTrackingButtonIndex() const
	{
		return m_nBtnIdxMenuTracking;
	}

	virtual void OnUpdateCmdUI(
		CFrameWnd* pTarget,
		BOOL bDisableIfNoHndler
		);
	virtual bool OnQueryMultiRowLayout() const;
	virtual	bool _OnMouseMoveMsg(UINT nFlags, CPoint point);

protected:
	virtual bool _CanDockToInnerCircles() const;
	virtual bool _CanDockToTabbedContainers(
		CExtControlBar * pDestBar
		) const;

public:
	virtual CFont * OnGetToolbarFont( bool bVert );
	virtual void DoPaint( CDC* pDC );
protected:
	int _HitTestImpl(
		CPoint point,
		UINT nButtonStyleInclude = 0, // button must have style
		UINT nButtonStyleExclude = 0,  // button must have not style
		bool bAllowFindNearest = false // if did not hit - find nearest
		) const;
	virtual int HitTest( CPoint point ) const;
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		) const;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
protected:
	CExtCustomizeSite * m_pCustomizeSite;
	CExtBarButton * m_pDragSourceTBB, * m_pDragTargetTBB;
	int m_nDragTargetShift;
	bool m_bHelperSeparatorAction:1;
public:
	bool m_bCustomizationAllowed:1;
	virtual CExtCustomizeSite * GetCustomizeSite();
	CExtCustomizeSite * GetCustomizeSite() const;
	virtual bool OnCustomizeRegisterBar(
		CExtCustomizeSite * pCustomizeSite
		);
	virtual void OnCustomizeGetBarInfo(
		CExtCustomizeSite::CToolBarCustomizeInfo * pInfo
		);
	virtual void OnCustomizeUpdateInitialBarState(
		CExtCustomizeCmdTreeNode * pParentNode,
		bool bRecalcLayout
		);
	virtual void OnCustomizeModeEnter();
	virtual void OnCustomizeModeLeave();
	virtual void OnCustomizeListBoxUpdate( CExtCustomizeToolbarsListBox * pWndList );
	virtual bool _IsDockSiteCustomizeMode() const;
	// CExtCustomizeSite::ICustomizeDropTarget
	virtual DROPEFFECT OnCustomizeTargetOver(
		CExtCustomizeSite::CCmdDragInfo & _dragInfo,
		CPoint point,
		DWORD dwKeyState
		);
	virtual void OnCustomizeTargetLeave();
	virtual bool OnCustomizeTargetDrop(
		CExtCustomizeSite::CCmdDragInfo & _dragInfo,
		CPoint point,
		DROPEFFECT de
		);
	virtual INT OnCustomizeCalcCmdNodePos(
		INT nButtonPos,
		bool * p_bReinitGroupStart = NULL
		);
	// CExtCustomizeSite::ICustomizeDropSource
	virtual void OnCustomizeSourceDragComplete(
		DROPEFFECT de,
		bool bCanceled,
		bool * p_bNoResetActiveItem
		);

	virtual bool OnCustomizeCanShowHideButtons();
	virtual void OnCustomizeInitShowHideButtonMenu(
		CExtPopupMenuWnd * pFolder
		);
	static bool stat_CbXtraMarkState(
		CExtPopupMenuWnd * pPopup,
		LPVOID pItemData
		);
	static bool stat_CbCmdDeliver(
		CExtPopupMenuWnd * pPopup,
		LPVOID pItemData
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

protected:

	//{{AFX_MSG(CExtToolControlBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnTrackButtonMenu(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
}; // class CExtToolControlBar

/////////////////////////////////////////////////////////////////////////////
// CExtPanelControlBar

class __PROF_UIS_API CExtPanelControlBar : public CExtToolControlBar
{
public:
	DECLARE_DYNCREATE(CExtPanelControlBar);

// Construction
public:
	CExtPanelControlBar();

// Attributes
public:
	CRect m_rcMargins;

// Operations
public:

	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz
		);
    virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD dwMode
		);

	enum e_fixed_bar_size_query_t
	{
		__FSQ_FLOATING	= 0,
		__FSQ_DOCKED_H	= 1,
		__FSQ_DOCKED_V	= 2,
	};

	// called when need to know new bar size after re-docking
	virtual CSize OnQueryFixedBarNewSize(
		CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
		) const;

	// should return size of margins for single child window
	virtual CRect OnQueryFixedBarChildWindowMargins() const;

protected:
	virtual CSize _QueryFixedBarNewSize(
		CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
		) const;
	CSize _CalcLayout(
		DWORD dwMode,
		int nLength = -1
		);
	virtual CSize _CalcDesiredMinOuterSize( BOOL bHorz );
	virtual INT _CalcDesiredMinHW() const;
	virtual INT _CalcDesiredMinVH() const;
	virtual CSize _CalcDesiredMinFloatedSize() const;
	virtual void _RepositionChild(
		bool bRedraw = true
		);
	virtual CSize _GetChildSize() const;
public:
	virtual bool _AffixmentIsAlignedHW() const;
	virtual bool OnQueryMultiRowLayout() const;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
public:
	virtual CExtCustomizeSite * GetCustomizeSite();
	virtual bool OnCustomizeRegisterBar(
		CExtCustomizeSite * pCustomizeSite
		);
	virtual void OnCustomizeModeEnter();
	virtual void OnCustomizeModeLeave();
	virtual void OnCustomizeListBoxUpdate( CExtCustomizeToolbarsListBox * pWndList );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

// Implementation
public:
	virtual ~CExtPanelControlBar();

	virtual LRESULT DoHelpHitTest( CPoint ptHelpHitTest );
	
	//{{AFX_MSG(CExtPanelControlBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtPanelControlBar

#endif // __EXT_TOOLCONTROLBAR_H

