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

#if (!defined __EXT_POPUP_MENU_WND_H)
#define __EXT_POPUP_MENU_WND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtPopupMenuWnd.h : header file
//

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	#if (!defined __EXTCUSTOMIZE_H)
		#include <ExtCustomize.h>
	#endif
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#define __BaseClassOfCExtPopupBaseWnd CWnd 
#define __EXT_MENU_DEF_INPLACE_EDIT_WIDTH 100

/////////////////////////////////////////////////////////////////////////////
// CExtSoundPlayer

class __PROF_UIS_API CExtSoundPlayer : public CObject
{
	struct __PROF_UIS_API InternalSoundEventInitDone_t : public CEvent
	{
		InternalSoundEventInitDone_t();
		void SetInitDone();
		void WaitInitDone();
	};
	struct __PROF_UIS_API InternalSoundThredadParms_t
	{
		InternalSoundEventInitDone_t * m_pEventInitDone;
		TCHAR m_sSoundSpecBuffer[1024];
		HANDLE m_hModule;
		DWORD m_dwPlayerFlags;
		HANDLE m_hThread;
		DWORD m_dwThreadID;
		static CEvent g_EventPlaySnd;
		static CCriticalSection g_CsPlaySnd;
		InternalSoundThredadParms_t(
			__EXT_MFC_SAFE_LPCTSTR sSoundSpecBuffer,
			HANDLE hModule,
			DWORD dwPlayerFlags
			);

		static DWORD WINAPI stat_ParallelPlayerProc(
			InternalSoundThredadParms_t * pParms
			);
		operator LPTHREAD_START_ROUTINE() const
		{
			return (LPTHREAD_START_ROUTINE)stat_ParallelPlayerProc;
		};
	
		void PlaySound();
		void PlaySoundParallel();
	}; // struct __PROF_UIS_API InternalSoundThredadParms_t

public:

	DECLARE_DYNCREATE( CExtSoundPlayer );

	class __PROF_UIS_API CExtSoundPlayerAutoPtr
	{
		CExtSoundPlayer * m_pPlayer;
	public:
		CExtSoundPlayerAutoPtr();
		~CExtSoundPlayerAutoPtr();
		void InstallSoundPlayer(
			CExtSoundPlayer * pPlayer
			);
		CExtSoundPlayer * operator->()
		{
			ASSERT( m_pPlayer != NULL );
			ASSERT_VALID( m_pPlayer );
			return m_pPlayer;
		};
	}; // class CExtSoundPlayerAutoPtr
	
	CExtSoundPlayer();
	~CExtSoundPlayer();

	enum e_ui_sounds_t
	{
	__NO_SOUND					= 0,
	__ON_MENU_EXPAND_CLICKED	= 1,
	__ON_MENU_POPUP_DISPLAYED	= 2,
	__ON_MENU_CMD_CLICKED		= 3,
	}; // enum e_ui_sounds_t

	virtual void PlaySound(
		CExtSoundPlayer::e_ui_sounds_t eSoundID
		);
}; // class CExtSoundPlayer

extern __PROF_UIS_API CExtSoundPlayer::CExtSoundPlayerAutoPtr g_SoundPlayer;

/////////////////////////////////////////////////////////////////////////////
// CExtWndShadow

class __PROF_UIS_API CExtWndShadow
{
	CBitmap m_bmp0, m_bmp1;			// photo of right & bottom shadow parts to save/restore
	COLORREF * m_pHelperDibSurface;
	CRect m_rcWndArea;				// window are not including shadow
	CRect m_rc1stArea;				// basic area
	CRect m_rc2ndArea;				// combined area
	UINT m_nShadowSize;				// shadow size (in pixels)
	UINT m_nBr0,m_nBr1;				// min/max brightness percents [0..100]
	bool m_bEnablePhotos:1;			// use photo bitmaps
	bool m_bEnablePaingMgrColor:1;	// use paing manager's shadow color
	void _FreeWinObjects();
	void _DoPixelOvershadow(
		int nMakeSpec,
		int nPosX,
		int nPosY,
		COLORREF clrShadowAdjust
		);
	bool _MakePhotos( CDC & dc, CDC & dcmm );
	bool _PaintLo( CDC & dc );
	bool _PaintHi( CDC & dc );
public:
	enum eShadowOptions
	{
		DEF_BRIGHTNESS_MIN =  75UL, // default min brightness percent
		DEF_BRIGHTNESS_MAX = 100UL, // default max brightness percent
		DEF_SHADOW_SIZE    =   3UL, // default shadow size (in pixels)
	};

	CExtWndShadow();
	~CExtWndShadow();
	
	bool Restore( CDC & dc );
	bool Paint( CDC & dc );
	bool Paint(
		CDC & dc,
		const CRect & rcWndArea,
		const CRect & rc1stArea = CRect(0,0,0,0),
		const CRect & rc2ndArea = CRect(0,0,0,0),
		UINT nShadowSize = DEF_SHADOW_SIZE,
		UINT nBr0 = DEF_BRIGHTNESS_MIN,
		UINT nBr1 = DEF_BRIGHTNESS_MAX,
		bool bEnablePhotos = true,
		bool bEnablePaingMgrColor = true
		);
	void Destroy();
}; // class CExtWndShadow


/////////////////////////////////////////////////////////////////////////////
// CExtPopupBaseWnd

class __PROF_UIS_API CExtPopupBaseWnd :
	public __BaseClassOfCExtPopupBaseWnd
{
public:
	DECLARE_DYNCREATE(CExtPopupBaseWnd)

	CExtPopupBaseWnd();

	virtual BOOL DestroyWindow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupBaseWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPopupBaseWnd();

	static UINT g_nMsgPrepareMenu; // prepare popup-menu window message
	static UINT g_nMsgPopupNext;
	static UINT g_nMsgPopupPrev;
	static UINT g_nMsgNotifyMenuExpanded;
	static UINT g_nMsgNotifyMenuClosed;
	static UINT g_nMsgPopupDrawItem;
	static UINT g_nMsgPopupDrawLeftArea;
	static UINT g_nMsgCreateInplaceEdit;

	enum e_combine_align_t // relatively to menu window rect
	{
		__CMBA_NONE = 0,
		__CMBA_TOP = 1,
		__CMBA_BOTTOM = 2,
		__CMBA_RIGHT = 3,
		__CMBA_LEFT = 4,
	};

	typedef
		void (*pCbPaintCombinedContent)(
			LPVOID pCookie,
			CDC & dc,
			const CWnd & refWndMenu,
			const CRect & rcExcludeArea, // in screen coords
			int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
			);

	enum e_animation_type_t
	{
		__AT_NONE = 0,
		__AT_RANDOM = 1,
		__AT_ROLL = 2,
		__AT_SLIDE = 3,
		__AT_FADE = 4,
		__AT_ROLL_AND_STRETCH = 5,
		__AT_SLIDE_AND_STRETCH = 6,
		__AT_NOISE = 7,
		__AT_BOXES = 8,
		__AT_CIRCLES = 9,
		__AT_HOLES = 10,
		
		__AT_MAX_VALID_VALUE = 10, // used for random effect selection
		__AT_CONTENT_DISPLAY = -1, // internal helper - delayed display
		__AT_CONTENT_EXPAND = -2 // internal helper - funny expand/stretch rarely used items (office XP like)
	};
	static e_animation_type_t g_DefAnimationType;

	static bool g_bUseDesktopWorkArea; // align to desktop work area (false - to screen area)
	static bool g_bEnableOnIdleCalls; // enable idle time processing

protected:
	
	CRect m_rcExcludeArea;
	CPoint m_ptTrack;
	bool m_bExcludeAreaSpec:1,m_bCombineWithEA:1;

	LPVOID m_pCbPaintCombinedCookie;
	pCbPaintCombinedContent m_pCbPaintCombinedContent;
	bool m_bCookieIsObject;

public:
	virtual CObject * GetCoockieAsObject();
	virtual bool IsToolBtnCombinedArea() const;
	pCbPaintCombinedContent GetCbPaintCombinedContent() const
	{
		return (pCbPaintCombinedContent) m_pCbPaintCombinedContent;
	}
	LPVOID GetCbPaintCombinedCookie() const
	{
		return (LPVOID)m_pCbPaintCombinedCookie;
	}

protected:
	e_animation_type_t m_AnimationType;
	CExtSoundPlayer::e_ui_sounds_t m_ePlaySoundOnAnimationFinished;

	COLORREF * m_clrFadeBitsSrc;
	COLORREF * m_clrFadeBitsDst;
	COLORREF * m_clrFadeBitsTmp;
	int m_nAnimPercent,m_nAnimPercentOld;
	bool m_bAnimFinished:1;

	CSize m_sizeFullItems;

	CExtWndShadow m_ShadowMain,m_ShadowCMBA;
	CBitmap
		m_bmpScreenSrc,m_bmpScreenDst,m_bmpScreenTmp;
		//m_bmpSaveBits;

	virtual bool _FindCustomizeMode() const;
	virtual void _InitAnimation();
	virtual void _StartAnimation();
	virtual void _AdjustAnimation( e_animation_type_t & eAT );

	void _SurfacesDelete()
	{
		ASSERT_VALID( this );

		if( m_bmpScreenDst.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_bmpScreenDst.DeleteObject()
				);
		}
		if( m_bmpScreenSrc.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_bmpScreenSrc.DeleteObject()
				);
		}
		if( m_bmpScreenTmp.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_bmpScreenTmp.DeleteObject()
				);
		}
		m_clrFadeBitsSrc
			= m_clrFadeBitsDst
			= m_clrFadeBitsTmp
			= NULL;
	}
	
	virtual void _EndAnimation()
	{
		ASSERT_VALID( this );

		_SurfacesDelete();
	}

	virtual bool _IsPopupWithShadows() const;
	virtual int _GetPopupShadowSize() const;

	virtual void _DrawAnimatedState( CDC & dc );
public:
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );
protected:
	virtual void _GetClientRect(RECT * pRectClient)
	{
		ASSERT_VALID( this );

		ASSERT( GetSafeHwnd() != NULL );
		ASSERT( pRectClient != NULL );
		GetClientRect(pRectClient);
	}
	void _GetClientRect(RECT & rectClient)
	{
		ASSERT_VALID( this );

		_GetClientRect( &rectClient );
	}

public:
	virtual bool _FindHelpMode() const;

protected:

	// Generated message map functions
	//{{AFX_MSG(CExtPopupBaseWnd)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPopupBaseWnd

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuTipWnd

class __PROF_UIS_API CExtPopupMenuTipWnd :
	public CExtPopupBaseWnd
{
	CSize m_IconSize;
	HICON m_hIcon;
	CExtSafeString m_sText;
	HRGN m_hRgn;
	bool m_bFlipHorz:1,m_bFlipVert:1;

public:
	DECLARE_DYNCREATE(CExtPopupMenuTipWnd)


	CExtPopupMenuTipWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupMenuTipWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPopupMenuTipWnd();

	void SetIcon(HICON hIcon);
	void SetText(__EXT_MFC_SAFE_LPCTSTR lpszText);

	bool Show(
		CWnd * pWndParent,
		const RECT & rcExcludeArea
		);
	void Hide();

protected:
	bool GetWindowRegion(
		CDC* pDC,
		HRGN* hRegion,
		CSize *Size = NULL
		);
public:
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtPopupMenuTipWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPopupMenuTipWnd

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuSite

class CExtPopupMenuWnd;
class CExtPopupColorMenuWnd;
class CExtPopupInplaceListBox;

class __PROF_UIS_API CExtPopupMenuSite
{
	bool m_bShutdownMode:1;
	CExtPopupMenuWnd
		* m_pTopMenu,
		* m_pWndCapture,
		* m_pWndAnimation;
	HHOOK m_hMouseHook;
	HHOOK m_hKeyboardHook;
	UINT * m_lpnResultCmdID;

	void _Done();

	// hooks
	static LRESULT CALLBACK _HookMouseProc(
		int nCode,      // hook code
		WPARAM wParam,  // message identifier
		LPARAM lParam   // mouse coordinates
		);
	static LRESULT CALLBACK _HookKeyboardProc(
		int nCode,      // hook code
		WPARAM wParam,  // virtual-key code
		LPARAM lParam   // keystroke-message information
		);
	void _Hook( bool bHook = true );

public:
	
	CExtPopupMenuSite();
	~CExtPopupMenuSite();

	bool IsEmpty() const
	{
		return (m_pTopMenu == NULL) ? true : false;
	}
	bool IsShutdownMode() const
	{
		return m_bShutdownMode;
	}

	void SetTargetCmdIdPtr( UINT * lpnResultCmdID = NULL );
	UINT * GetTargetCmdIdPtr();

	CExtPopupMenuWnd * GetCapture();
	void SetCapture( CExtPopupMenuWnd * pNewPopup = NULL );

	CExtPopupMenuWnd * GetAnimated();
	void SetAnimated( CExtPopupMenuWnd * pNewPopup = NULL );

	void SetInstance( CExtPopupMenuWnd * pTopMenu = NULL );
	CExtPopupMenuWnd * GetInstance();
	void DoneInstance();
	
	void operator=( CExtPopupMenuWnd * pTopMenu )
	{
		SetInstance(pTopMenu);
	}

	CExtPopupMenuWnd * operator->()
	{
		ASSERT( !IsEmpty() );
		ASSERT( !IsShutdownMode() );
		ASSERT( m_pTopMenu != NULL );
		return m_pTopMenu;
	}

	bool IsTopPupup(
		CExtPopupMenuWnd * pTopMenu
		) const;

	static CExtPopupMenuSite g_DefPopupMenuSite;

	friend class CExtPopupBaseWnd;
	friend class CExtPopupMenuWnd;
}; // class CExtPopupMenuSite

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd

// track flags (menu area align)
#define TPMX_LEFTALIGN				0x00000001L
#define TPMX_CENTERALIGN			0x00000002L
#define TPMX_RIGHTALIGN				0x00000003L
#define TPMX_TOPALIGN				0x00000004L
#define TPMX_VCENTERALIGN			0x00000005L
#define TPMX_BOTTOMALIGN			0x00000006L
#define TPMX_ALIGN_MASK				0x0000000FL
// track flags (menu area combine type)
#define TPMX_COMBINE_NONE			0x00000000L // do not combine menu area with lpRect
#define TPMX_COMBINE_ANY_SUITABLE	0x00000010L // try combine menu area with lpRect
#define TPMX_COMBINE_DEFAULT		0x00000020L // do like PaintManager said
#define TPMX_COMBINE_MASK			0x00000030L
// select first enabled menu item
#define TPMX_SELECT_ANY				0x00000040L
// perform message loop
#define TPMX_DO_MESSAGE_LOOP		0x00000080L
// initially expand rarely used items
#define TPMX_NO_HIDE_RARELY			0x00000100L
// do not post WM_COMMAND message
// (result could be stored in lpnResultCmdID)
#define TPMX_NO_WM_COMMAND			0x00000200L
// do not update cmdui
#define TPMX_NO_CMD_UI				0x00000400L
// ownerdraw items
// (variable sizes are not supported)
#define TPMX_OWNERDRAW_FIXED		0x00000800L
// force no shadows
#define TPMX_NO_SHADOWS				0x00001000L
// help ctx mode
#define TPMX_HELP_CTX_MODE			0x00002000L
// customize mode / no menu site control
#define TPMX_CUSTOMIZE_MODE			0x00004000L

class __PROF_UIS_API CExtPopupMenuWnd
	: public CExtPopupBaseWnd
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	, public CExtCustomizeSite::ICustomizeDropTarget
	, public CExtCustomizeSite::ICustomizeDropSource
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
{
	friend class CExtControlBar;
	friend class CExtToolControlBar;
	friend class CExtMenuControlBar;

	class __PROF_UIS_API InternalFriendlyWinApp : CWinApp
	{
	public:
		inline static InternalFriendlyWinApp * _GetFriendlyApp()
		{
			CWinApp * pApp = ::AfxGetApp();
			ASSERT( pApp != NULL );
			return (InternalFriendlyWinApp *)pApp;
		}
		CRecentFileList * _GetRecentFileList()
		{
			//ASSERT( m_pRecentFileList != NULL );
			return m_pRecentFileList;
		}
	}; // class InternalFriendlyWinApp

public:

	// is allowed menu item positioning without
	// using only & - marked text
	static bool g_bAllowNonAccelPositioning;

	friend class CExtPopupMenuSite;

public:
	DECLARE_DYNCREATE(CExtPopupMenuWnd)

	CExtPopupMenuWnd();

// Attributes
public:

	virtual int _GetCurIndex() const;
	void _DoShowChain( int nCmdShow = SW_SHOWNA );
	void _DoResetChainPositions();
protected:
	void _DoResetChainPositionsImpl(
		CExtPopupMenuWnd * pPrev
		);

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
public:
	int m_nHelperDropIndexBefore;
	virtual void _SetDropIndexBefore(
		int nIndex = -1 // reset
		);
protected:
	bool m_bHelperNoHideChild:1, m_bHelperNoRemoveSrc:1, m_bHelperDragOverPassed:1;
	CPoint m_ptHelperDragStart;
	int m_nDragSrcIdx;
	CExtCustomizeCmdTreeNode * m_pNode, * m_pDragSrcNode;
	CExtCustomizeSite * m_pCustomizeSite;
public:
	CExtCustomizeCmdTreeNode * _CmdNodeGet()
	{
		ASSERT_VALID( this );
#ifdef _DEBUG
		if( m_pNode != NULL )
		{
			ASSERT_VALID( m_pNode );
		}
#endif // _DEBUG
		return m_pNode;
	}
	const CExtCustomizeCmdTreeNode * _CmdNodeGet() const
	{
		ASSERT_VALID( this );
#ifdef _DEBUG
		if( m_pNode != NULL )
		{
			ASSERT_VALID( m_pNode );
		}
#endif // _DEBUG
		return m_pNode;
	}
	void _CmdNodeSet( CExtCustomizeCmdTreeNode * pNode )
	{
		ASSERT_VALID( this );
#ifdef _DEBUG
		if( pNode != NULL )
		{
			ASSERT_VALID( pNode );
		}
#endif // _DEBUG
		m_pNode = pNode;
	}
	virtual CExtCustomizeSite * _FindCustomizeSite() const;
protected:
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
	// CExtCustomizeSite::ICustomizeDropSource
	virtual void OnCustomizeSourceDragComplete(
		DROPEFFECT de,
		bool bCanceled,
		bool * p_bNoResetActiveItem
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual bool _FindCustomizeMode() const;

	CExtPopupMenuTipWnd m_wndToolTip;

	int m_nCurIndex,m_nDelayedFocusItemIndex;
	bool
		m_bTopLevel:1,
		m_bScrollingAvailable:1,
		m_bExpandAvailable:1,
		m_bExpandWasPressed:1
		;
	UINT m_nWaitingExpandTickCount;
	DWORD m_dwTrackFlags;

	HWND m_hWndCmdReciever;

	//CPoint m_ptTrack;
	int m_nDyScrollOffset;
	CExtPopupMenuWnd * m_pWndParentMenu;

	CRect
		m_rcScrollTop,
		m_rcScrollBottom,
		m_rcExpandBtn;
	int m_nScrollingDirection;

	UINT m_nLeftAreaWidth;

public:
	CExtPopupMenuWnd * GetParentMenuWnd()
	{
		return m_pWndParentMenu;
	}
	const CExtPopupMenuWnd * GetParentMenuWnd() const
	{
		return m_pWndParentMenu;
	}
	virtual HWND GetCmdRecieverHWND()
	{
		ASSERT( m_hWndCmdReciever != NULL );
		ASSERT( ::IsWindow( m_hWndCmdReciever ) );
		return m_hWndCmdReciever;
	}
protected:
	enum spec_buttons_index_t
	{
		IDX_NOTHING = -1,
		IDX_SCROLL_TOP = -2,
		IDX_SCROLL_BOTTOM = -3,
		IDX_EXPAND = -4
	};
// Operations
public:
	virtual BOOL LoadMenu( 
		HWND hWndCmdRecv,
		UINT nIDResource,
		bool bPopupMenu = true
		);
	virtual HMENU ExportToMenu(
		BOOL bDeep = TRUE
		) const;
	virtual BOOL UpdateFromMenu( 
		HWND hWndCmdRecv,
		CMenu *pBuildMenu,
		bool bPopupMenu = true,
		bool bTopLevel = true
		);
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	bool UpdateFromCmdTree(
		HWND hWndCmdRecv,
		CExtCustomizeCmdTreeNode * pNode,
		bool bTopLevel = true
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual BOOL UpdateMdiWindowsMenu(
		CWnd * pWndStartSearchMdiFrameWnd = NULL
		);
	virtual void SetCmdTargetToAllItems(
		HWND hWndSpecCmdReciever = NULL,
		bool bOnlyThisLevel = false
		);

	enum menu_item_type_t
	{
		TYPE_POPUP		= 0xFFFFFFFF,
		TYPE_SEPARATOR	= 0x00000000 // same as ID_SEPARATOR
	};

	virtual INT ItemGetCount() const;
	virtual UINT ItemGetCmdID( // menu_item_type_t values can be returned
		INT nPos
		) const;
	virtual HICON ItemGetIcon( INT nPos ) const;
	virtual CExtSafeString ItemGetText( INT nPos ) const;
	virtual CExtSafeString ItemGetAccelText( INT nPos ) const;
	virtual INT ItemFindPosForCmdID(
		UINT nCmdID,
		INT nPosStart = -1
		) const;
	virtual bool ItemSetPopupIcon(
		INT nPos,
		HICON hIcon = NULL // no icon by default
		);
	virtual bool ItemSetPopupText(
		INT nPos,
		__EXT_MFC_SAFE_LPCTSTR sText // NULL if empty
		);
	virtual bool ItemSetPopupAccelText(
		INT nPos,
		__EXT_MFC_SAFE_LPCTSTR sText // NULL if empty
		);
	virtual CExtPopupMenuWnd * ItemGetPopup(
		INT nPos
		);
	virtual const CExtPopupMenuWnd * ItemGetPopup(
		INT nPos
		) const;
	virtual bool ItemIsDisplayed(
		INT nPos
		) const;
	virtual void ItemSetDisplayed(
		INT nPos,
		bool bForceDisplayed = true
		);
	virtual BOOL ItemRemove(
		INT nPos = -1 // remove all
		);
	virtual BOOL ItemInsert(
		UINT nCmdID = TYPE_SEPARATOR,
		INT nPos = -1, // append
		__EXT_MFC_SAFE_LPCTSTR sText = NULL, // for TYPE_POPUP only
		HICON hIcon = NULL, // for TYPE_POPUP only
		HWND hWndSpecCmdReciever = NULL // specific command reciever
		);
	virtual BOOL ItemInsertCommand( // no references to command manager
		UINT nCmdID,
		INT nPos, // -1 append
		__EXT_MFC_SAFE_LPCTSTR sItemText, // may be NULL
		__EXT_MFC_SAFE_LPCTSTR sAccelText, // may be NULL
		const CExtCmdIcon & icon,
		int nCheck = 0, // 0 - no check, 1 - checked, 2 - indeterminate, 3 - radio
		HWND hWndSpecCmdReciever = NULL
		);
	BOOL ItemInsertCommand( // no references to command manager
		UINT nCmdID = TYPE_SEPARATOR,
		INT nPos = -1, // append
		__EXT_MFC_SAFE_LPCTSTR sItemText = NULL,
		__EXT_MFC_SAFE_LPCTSTR sAccelText = NULL,
		HICON hIcon = NULL,
		bool bCopyIcon = true,
		int nCheck = 0, // 0 - no check, 1 - checked, 2 - indeterminate, 3 - radio
		HWND hWndSpecCmdReciever = NULL
		);
	virtual BOOL ItemInsertSpecPopup( // insert specific popup implementation
		CExtPopupMenuWnd * pSpecPopup,
		INT nPos = -1, // append
		__EXT_MFC_SAFE_LPCTSTR sText = NULL,
		HICON hIcon = NULL
		);
	virtual INT ItemFindByAccessChar(
		__EXT_MFC_SAFE_TCHAR chrAccess,
		INT nStartIdx = -1,
		BOOL bRestartAt0 = TRUE
		) const;
	virtual INT ItemFindByText(
		__EXT_MFC_SAFE_LPCTSTR sText,
		INT nStartIdx = -1,
		BOOL bRestartAt0 = TRUE
		) const;

	virtual bool IsAllItemsRarelyUsed() const;
	virtual BOOL CreatePopupMenu( HWND hWndCmdRecv ); // alternative to LoadMenu()

	void SetLeftAreaWidth( UINT nWidth = 0 );
	UINT GetLeftAreaWidth() const;

	static CExtPopupMenuWnd * GetTrackingMenu();
	static void CancelMenuTracking();
	inline static BOOL IsKeyPressed(
		int nVirtKey
		)
	{
		SHORT nKeyState =
			::GetKeyState( nVirtKey );
		if( (nKeyState & (1 << (sizeof(SHORT)*8-1))) != 0 )
			return TRUE;
		return FALSE;
	}
	static BOOL IsCmdKeyActivation(
		LPARAM lParam = 0
		);
	static BOOL IsMenuTracking();

	virtual BOOL TrackPopupMenu(
		DWORD dwTrackFlags,
		int x,
		int y,
		LPCRECT lpRect = NULL,
		LPVOID pCbPaintCombinedCookie = NULL,
		pCbPaintCombinedContent pCbPaintCombinedContent = NULL,
		UINT * lpnResultCmdID = NULL,
		bool bCookieIsObject = false
		);

	struct __PROF_UIS_API MsgPrepareMenuData_t
	{
		bool m_bMenuChanged:1;
		bool m_bMenuCanceled:1;
		CExtPopupMenuWnd * m_pPopup;

		MsgPrepareMenuData_t(
			CExtPopupMenuWnd * pPopup
			)
			: m_bMenuChanged( false )
			, m_bMenuCanceled( false )
			, m_pPopup( pPopup )
		{
			ASSERT( m_pPopup != NULL );
		}
		~MsgPrepareMenuData_t()
		{
		}

		operator WPARAM() const
		{
			return WPARAM( this );
		}

		LRESULT SendMessage( HWND hWnd )
		{
			ASSERT( m_pPopup != NULL );

			ASSERT( hWnd != NULL );
			ASSERT( ::IsWindow(hWnd) );
			LRESULT lResult =
				::SendMessage(
					hWnd,
					CExtPopupMenuWnd::g_nMsgPrepareMenu,
					WPARAM( *this ),
					LPARAM( m_pPopup )
					);
			
			return lResult;
		}
		LRESULT SendMessage( CWnd * pWnd )
		{
			return SendMessage( pWnd->GetSafeHwnd() );
		}
	}; // struct MsgPrepareMenuData_t

	static bool g_bMenuWithShadows; // allow shadows
	static bool g_bMenuExpanding; // allow hide rarely used
	static bool g_bMenuDelayExpanding; // show full menu after short delay
	static bool g_bMenuHighlightRarely; // display rarely used in different style
	static bool g_bMenuShowCoolTips; // allow display cool tooltips
	static bool g_bMenuExpandAnimation; // allow animation when show rarely used
	static bool g_bUseStretchOnExpandAnimation; // use StretchBlt() instead of BitBlt() for painting menu expand amimation
	static bool g_bMenuLargeIcons; // use large icons

public:
	static void stat_paint_combined_menu_toolbtn(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);
protected:
	virtual bool _CreateHelper(
		CWnd * pWndCmdReciever
		);
	virtual bool _BuildItems(
		CMenu * pBuildMenu,
		bool bTopLevel
		);
	virtual void _UpdateCmdUI();
	virtual void _Init();
	virtual void _RecalcLayoutImpl();
	static int _GetSpecBtnHeight();
public:
	virtual int _HitTest(
		const CPoint & point,
		bool * p_bInplaceControlArea = NULL,
		bool * p_bInplaceDropDownArea = NULL
		);
protected:
	virtual void _DrawAnimatedState( CDC & dc );
public:
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );
protected:
	virtual void _GetClientRect(RECT * pRectClient);
	virtual void _EndSequence(
		UINT nCmdID = 0,
		HWND hWndDeliver = NULL
		);
public:
	static void PassMsgLoop(
		bool bEnableOnIdleCalls
		);
protected:

	struct __PROF_UIS_API ExpandEffectRects_t
	{
		CRect m_rcSrc,m_rcDst;
		ExpandEffectRects_t()
		{
			m_rcSrc.SetRectEmpty();
			m_rcDst.SetRectEmpty();
		};
		ExpandEffectRects_t(
			const RECT & rcSrc,
			const RECT & rcDst
			)
		{
			m_rcSrc = rcSrc;
			m_rcDst = rcDst;
		}
		ExpandEffectRects_t(
			const ExpandEffectRects_t & other
			)
		{
			m_rcSrc = other.m_rcSrc;
			m_rcDst = other.m_rcDst;
		}
		ExpandEffectRects_t &
			operator= (
				const ExpandEffectRects_t & other
				)
		{
			m_rcSrc = other.m_rcSrc;
			m_rcDst = other.m_rcDst;
			return * this;
		}
	}; // struct ExpandEffectRects_t

	typedef
		CArray < ExpandEffectRects_t, ExpandEffectRects_t & >
		expand_effect_rects_container_t;

	struct __PROF_UIS_API VisibleItemDefinition_t
	{
		INT m_nIndex;
		bool m_bRarelyUsed:1;
		bool m_bRarelyUsedPrev:1;
		bool m_bRarelyUsedNext:1;
		bool m_bHelperIsSeparator:1;
		bool m_bHelperIsPopup:1;
		UINT m_nHelperCmdID;
		CRect m_rcItem;
		VisibleItemDefinition_t()
		{
			m_nIndex = -1;
			m_bRarelyUsed
				= m_bRarelyUsedPrev
				= m_bRarelyUsedNext
				= m_bHelperIsSeparator
				= m_bHelperIsPopup
				= false;
			m_nHelperCmdID = ID_SEPARATOR;
			m_rcItem.SetRectEmpty();
		}
		VisibleItemDefinition_t(
			const VisibleItemDefinition_t & other
			)
		{
			AssignFromOther(other);
		}
		~VisibleItemDefinition_t()
		{
		}
		VisibleItemDefinition_t & operator=(
			const VisibleItemDefinition_t & other
			)
		{
			AssignFromOther(other);
			return * this;
		}
		void AssignFromOther(
			const VisibleItemDefinition_t & other
			)
		{
			m_nIndex = other.m_nIndex;
			m_bRarelyUsed = other.m_bRarelyUsed;
			m_bRarelyUsedPrev = other.m_bRarelyUsedPrev;
			m_bRarelyUsedNext = other.m_bRarelyUsedNext;
			m_bHelperIsSeparator = other.m_bHelperIsSeparator;
			m_bHelperIsPopup = other.m_bHelperIsPopup;
			m_nHelperCmdID = other.m_nHelperCmdID;
			m_rcItem = other.m_rcItem;
		}
		operator int() const
		{
			return m_nIndex;
		}
	};

	typedef
		CArray < VisibleItemDefinition_t, VisibleItemDefinition_t & >
		visible_items_t;
	virtual void _GetVisibleItems(
		HDC hDcTest, // may be NULL
		visible_items_t & v
		);

	virtual BOOL _TrackPopupMenu(
		DWORD dwTrackFlags,
		int x,
		int y,
		LPCRECT lpRect,
		LPVOID pCbPaintCombinedCookie = NULL,
		pCbPaintCombinedContent pCbPaintCombinedContent = NULL,
		bool bCookieIsObject = false
		);
public:
	virtual void _ItemFocusCancel( BOOL bRepaint );
	virtual void _ItemFocusDelay( int nItemIndex = IDX_NOTHING );
	virtual void _ItemFocusSet(
		int nCurIndex,
		BOOL bEnableDropChild,
		BOOL bRepaint,
		BOOL bSelectAnyInChild = TRUE
		);
	virtual void _GetItemRect(
		int nIndex,
		RECT & rectItem
		);
public:
	virtual bool _OnMouseWheel(
		WPARAM wParam,
		LPARAM lParam,
		bool & bNoEat
		);
	virtual bool _OnMouseMove(
		UINT nFlags,
		CPoint point,
		bool & bNoEat
		);
	virtual bool _OnMouseClick(
		UINT nFlags,
		CPoint point,
		bool & bNoEat
		);
	virtual bool _OnKeyDown(
		UINT nChar,
		UINT nRepCnt,
		UINT nFlags,
		bool & bNoEat
		);

	virtual bool _CanStartLevelTracking();

protected:
	virtual void _FreeWinObjects();
	virtual void _OnCancelMode(
		bool bNcDestroy = false
		);
	virtual bool _PtInWndArea( CPoint ptClient );


public:
	virtual void _SetCapture();
	virtual CExtPopupMenuWnd * _GetCapture();
	virtual void _ReleaseCapture();
protected:

	virtual int _GetMaxScrollPos();
	virtual bool _StartScrolling(int nButtonIndex);

public:
	virtual void _SyncItems();
protected:
	virtual void _DoExpand();
	virtual bool _CoolTipIsVisible();
	virtual void _CoolTipHide(
		bool bAdvOperation = true
		);

	e_combine_align_t m_eCombineAlign;
	CRgn m_rgnWnd;
	CRect m_rcClient;

	virtual CRect _CalcTrackRect();
	virtual CSize _CalcTrackSize();
	virtual bool _IsPopupWithShadows() const;
	virtual int _GetPopupShadowSize() const;

	enum next_item_t
	{
		__NI_NOTHING,
		__NI_ANY,
		__NI_NEXT,
		__NI_PREV,
		__NI_PAGE_UP,
		__NI_PAGE_DOWN
	};
	
	virtual int _GetNextItem(
		next_item_t nit
		);

public:
	
	/////////////////////////////////////////////////////////////////////////////
	// CExtPopupMenuWnd::CInPlaceEditWnd

	class CInPlaceEditWnd;

	typedef
		bool (*pCbInplaceEditWndProc)(
			LRESULT & lResult,
			UINT message,
			WPARAM wParam,
			LPARAM lParam,
			CInPlaceEditWnd & wndEdit,
			LPVOID pCoockie
			);
	typedef
		bool (*pCbVerifyTextInput)(
			CInPlaceEditWnd & wndEdit,
			LPVOID pCoockie,
			__EXT_MFC_SAFE_LPCTSTR sTextOld,
			__EXT_MFC_SAFE_LPCTSTR sTextNew
			);
	typedef
		void (*pCbPutTextInputResult)(
			CInPlaceEditWnd & wndEdit,
			LPVOID pCoockie,
			__EXT_MFC_SAFE_LPCTSTR sTextNew
			);

	class CInPlaceEditWnd : public CEdit
	{
		pCbVerifyTextInput m_pCbVerifyTextInput;
		pCbPutTextInputResult m_pCbPutTextInputResult;
		pCbInplaceEditWndProc m_pCbWndProc;
		LPVOID m_pCbCoockie;
		CExtSafeString * m_pStr;
		CExtSafeString m_sTextSaved;
		bool m_bNoCancelAtKillFocus:1;
	public:
		static UINT g_nMsgDeleteSelection, g_nMsgSelectAll;
		CInPlaceEditWnd(
			CExtSafeString * pStr,
			pCbVerifyTextInput pCbVerify = NULL,
			pCbPutTextInputResult pCbResult = NULL,
			pCbInplaceEditWndProc pCbWndProc = NULL,
			LPVOID pCbCoockie = NULL
			);
		~CInPlaceEditWnd();
#ifdef _DEBUG
		virtual void AssertValid() const;
#endif // _DEBUG
		virtual bool Create(
			CExtPopupMenuWnd * pPopup,
			CRect rc,
			UINT nDlgCtrlID
			);
		virtual LRESULT WindowProc(
			UINT message,
			WPARAM wParam,
			LPARAM lParam
			);
	protected:
		virtual void PostNcDestroy();
		CExtPopupMenuWnd * GetPopupMenu();
	}; // class CInPlaceEditWnd


	/////////////////////////////////////////////////////////////////////////////
	// CExtPopupMenuWnd::MENUITEMDATA
	
	class MENUITEMDATA;
	friend class MENUITEMDATA;
	class CExtPopupMenuCmdUI;

	struct __PROF_UIS_API INPLACEEDITCREATEINFO
	{
		CWnd ** m_ppInplaceEdit;
		CExtPopupMenuWnd * m_pPopup;
		MENUITEMDATA * m_pMenuItemData;
		CExtSafeString * m_pInplaceEditStr;
		pCbVerifyTextInput m_pCbVerifyTextInput;
		pCbPutTextInputResult m_pCbPutTextInputResult;
		pCbInplaceEditWndProc m_pInplaceEditCbWndProc;
		LPVOID m_pInplaceEditCbCoockie;
		CRect m_rc;
		INPLACEEDITCREATEINFO(
			CWnd ** ppInplaceEdit,
			CExtPopupMenuWnd * pPopup,
			MENUITEMDATA * pMenuItemData,
			CExtSafeString * pInplaceEditStr,
			pCbVerifyTextInput pCbVerify,
			pCbPutTextInputResult pCbResult,
			pCbInplaceEditWndProc pInplaceEditCbWndProc,
			LPVOID pInplaceEditCbCoockie,
			const RECT & rc
			)
			: m_ppInplaceEdit( ppInplaceEdit )
			, m_pPopup( pPopup )
			, m_pMenuItemData( pMenuItemData )
			, m_pInplaceEditStr( pInplaceEditStr )
			, m_pCbVerifyTextInput( pCbVerify )
			, m_pCbPutTextInputResult( pCbResult )
			, m_pInplaceEditCbWndProc( pInplaceEditCbWndProc )
			, m_pInplaceEditCbCoockie( pInplaceEditCbCoockie )
			, m_rc( rc )
		{
		}
	}; // struct INPLACEEDITCREATEINFO

	class __PROF_UIS_API MENUITEMDATA
	{
	public:
		typedef bool (*pCbItemClick)(
			CExtPopupMenuWnd * pPopup,
			MENUITEMDATA * pItemData
			);
	protected:
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeCmdTreeNode * m_pCmdNode;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtPopupMenuWnd * m_pWndChild;
		int m_nItemIndex;
		
		UINT m_nCmdID; // can be ID_SEPARATOR
		CExtSafeString m_sItemText, m_sAccelText;
		CExtCmdIcon m_iconPopup;

		CSize m_sizeItem;
		INT m_nIconAreaWidth;
		bool m_bSelected:1, m_bDisplayed:1, m_bForceDisplayed:1,
			m_bChildCombine:1, m_bToolButton:1, m_bForceEnabled:1,
			m_bForceNoLeftGradient:1,
			m_bCheck:1,m_bRadio:1,m_bEnabled:1,m_bIndeterminate:1,
			m_bXtraChecked:1,m_bNoCmdUI:1;
		pCbItemClick m_pCbCmdDeliver;
		pCbItemClick m_pCbXtraMarkState;
		__EXT_MFC_SAFE_TCHAR m_cAccelChar;

		HWND m_hWndSpecCmdReciever;
		
		CWnd * m_pHelperInplaceEditWnd;
		CExtSafeString * m_pInplaceEditStr;
		pCbVerifyTextInput m_pCbVerifyTextInput;
		pCbPutTextInputResult m_pCbPutTextInputResult;
		pCbInplaceEditWndProc m_pInplaceEditCbWndProc;
		LPVOID m_pInplaceEditCbCoockie;
		int m_nInplaceEditWidth;
		bool m_bAllowInplaceEditActivation:1;

		bool m_bTempSelTextValid:1;
		CExtSafeString m_sTempSelText;

		LPARAM m_nLParam;

	public:
		MENUITEMDATA();
		MENUITEMDATA( const MENUITEMDATA & other );
		~MENUITEMDATA();
		MENUITEMDATA & operator=( const MENUITEMDATA & other );
	private:
		void AssignFromOther( const MENUITEMDATA & other );
	public:
		bool UpdateFromMenu(
			HWND hWndCmdRecv,
			CMenu * pTrackMenu,
			int nItemIndex
			);
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		bool UpdateFromCmdTree(
			HWND hWndCmdRecv,
			CExtCustomizeCmdTreeNode * pNode,
			int nItemIndex
			);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	public:
		void AccelCharInit();
		__EXT_MFC_SAFE_TCHAR AccelCharGet() const;
		bool AccelCharIsSet() const;

	protected:
		bool UpdateCmdManagerCommand(
			CExtCmdItem * pCmdItem,
			int nItemIndex
			);

	public:
		void MeasureItem();

		CExtCmdItem * GetCmd() const;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeCmdTreeNode * GetCmdNode();
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

		HWND GetCmdReciever() const
		{
			ASSERT( m_hWndSpecCmdReciever != NULL );
			ASSERT( ::IsWindow(m_hWndSpecCmdReciever) );
			return m_hWndSpecCmdReciever;
		}
		void SetCmdReciever( HWND hWndSpecCmdReciever )
		{
			ASSERT( hWndSpecCmdReciever != NULL );
			ASSERT( ::IsWindow(hWndSpecCmdReciever) );
			m_hWndSpecCmdReciever = hWndSpecCmdReciever;
		}

	protected:
		void SetDisplayed( bool bDisplayed )
		{
			m_bDisplayed = bDisplayed;
		}
		void SetForceDisplayed( bool bForceDisplayed )
		{
			m_bForceDisplayed = bForceDisplayed;
		}
	public:
		bool IsDisplayed() const
		{
			return m_bDisplayed || m_bForceDisplayed;
		}
		bool IsForceDisplayed()
		{
			return m_bForceDisplayed;
		}
		bool IsAllItemsRarelyUsed() const
		{
			ASSERT( IsPopup() );
			ASSERT( m_pWndChild != NULL );
			if( IsNoCmdUI() )
				return false;
			return m_pWndChild->IsAllItemsRarelyUsed();
		}

		bool IsEnabled() const
		{
			//if( IsPopup() && (! IsInplaceEdit()) )
			if( IsPopup() && GetCmd() == NULL )
				return true;
			if( IsSeparator() )
				return false;
			if( IsForceEnabled() )
				return true;
			return m_bEnabled;
		}
		bool Enable( bool bOn )
		{
//			if( IsPopup() )
//				return true;
			if( IsSeparator() )
				return false;
			bool bRetVal = m_bEnabled;
			m_bEnabled = bOn;
			return bRetVal;
		}
		bool IsIndeterminated() const
		{
			if( IsPopup() || IsSeparator() )
				return false;
			return m_bIndeterminate;
		}
		bool Indeterminate( bool bOn )
		{
			bool bRetVal = m_bIndeterminate;
			m_bIndeterminate = bOn;
			return bRetVal;
		}

		bool IsSelected() const
		{
			if( /*IsPopup() ||*/ IsSeparator() )
				return false;
			return m_bSelected;
		}
	protected:
		void SetSelected( bool bOn = true )
		{
			m_bSelected = bOn;
		}
	public:
		bool GetRadio() const
		{
			if( IsPopup() || IsSeparator() )
				return false;
			return m_bRadio;
		}
		bool Radio( bool bOn )
		{
			bool bRetVal = m_bRadio;
			m_bRadio = bOn;
			return bRetVal;
		}

		bool GetCheck() const
		{
			if( IsPopup() || IsSeparator() )
				return false;
			return m_bCheck;
		}
		bool Check( bool bOn )
		{
			bool bRetVal = m_bCheck;
			m_bCheck = bOn;
			return bRetVal;
		}

		pCbItemClick GetCmdDeliverCb() const
		{
			if( IsSeparator() )
				return NULL;
			return (pCbItemClick)m_pCbCmdDeliver;
		}
		void SetCmdDeliverCb( pCbItemClick pCb )
		{
			if( IsSeparator() )
				return;
			m_pCbCmdDeliver = pCb;
		}

		bool IsExtraMark() const
		{
			if( IsSeparator() )
				return false;
			return (m_pCbXtraMarkState != NULL) ? true : false;
		}
		pCbItemClick GetExtraMarkCallback() const
		{
			return (pCbItemClick)m_pCbXtraMarkState;
		}
		void SetExtraMark( pCbItemClick pCb )
		{
			if( IsSeparator() )
				return;
			m_pCbXtraMarkState = pCb;
		}
		bool IsExtraChecked() const
		{
			if( !IsExtraMark() )
				return false;
			return m_bXtraChecked;
		}
		void SetExtraChecked( bool bOn )
		{
			if( !IsExtraMark() )
				return;
			m_bXtraChecked = bOn;
		}
		
		bool IsNoCmdUI() const
		{
			return m_bNoCmdUI;
		}
		void SetNoCmdUI( bool bOn )
		{
			m_bNoCmdUI = bOn;
		}

		LPARAM LParamGet() const
		{
			return m_nLParam;
		}
		void LParamSet( LPARAM nLParam )
		{
			m_nLParam = nLParam;
		}

		__EXT_MFC_SAFE_LPCTSTR GetText() const;
		void SetText( __EXT_MFC_SAFE_LPCTSTR strText );
		__EXT_MFC_SAFE_LPCTSTR GetAccelText() const;
		void SetAccelText( __EXT_MFC_SAFE_LPCTSTR strText );
		
		CExtCmdIcon * GetIconPtr()
		{
			if( IsSeparator() )
				return NULL;
			if( IsPopup() || IsNoCmdUI() )
				return (m_iconPopup.IsEmpty()) ? NULL : (&m_iconPopup);
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
			CExtCustomizeCmdTreeNode * pNode = GetCmdNode();
			if( pNode != NULL )
				return
					pNode->GetIconPtrInMenu(
						g_CmdManager->CmdGetPtr(
							g_CmdManager->ProfileNameFromWnd( GetCmdReciever() ),
							m_nCmdID
							)
						);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
			return
				g_CmdManager->CmdGetIconPtr(
					g_CmdManager->ProfileNameFromWnd( GetCmdReciever() ),
					m_nCmdID
					);
		}

		__EXT_MFC_SAFE_LPCTSTR GetTip() const
		{
			if( IsPopup() || IsSeparator() || GetCmd() == NULL )
				return _T("");
			return GetCmd()->m_sTipStatus;
		}

		bool IsSeparator() const
		{
			bool bSeparator =
				(m_nCmdID==TYPE_SEPARATOR) ? true : false;
			return bSeparator;
		}

		bool IsChildCombine() const
		{
			return m_bChildCombine;
		}
		void SetChildCombine( bool bChildCombine = true )
		{
			m_bChildCombine = bChildCombine;
		}

		bool IsToolButton() const
		{
			return m_bToolButton;
		}
		void SetToolButton( bool bToolButton = true )
		{
			if(		(m_bToolButton && bToolButton)
				||	((!m_bToolButton) && (!bToolButton))
				)
				return;
			m_bToolButton = bToolButton;
			MeasureItem();
		}
		
		bool IsForceEnabled() const
		{
			return m_bForceEnabled;
		}
		void SetForceEnabled( bool bForceEnabled = true )
		{
			if(		(m_bForceEnabled && bForceEnabled)
				||	((!m_bForceEnabled) && (!bForceEnabled))
				)
				return;
			m_bForceEnabled = bForceEnabled;
		}

		bool IsForceNoLeftGradient() const
		{
			return m_bForceNoLeftGradient;
		}
		void SetForceNoLeftGradient( bool bForceNoLeftGradient = true )
		{
			if(		(m_bForceNoLeftGradient && bForceNoLeftGradient)
				||	((!m_bForceNoLeftGradient) && (!bForceNoLeftGradient))
				)
				return;
			m_bForceNoLeftGradient = bForceNoLeftGradient;
			MeasureItem();
		}

		bool IsInplaceEdit() const
		{
			return (m_pInplaceEditStr != NULL) ? true : false;
		}
		void SetInplaceEdit(
			CExtSafeString * pInplaceEditStr,
			pCbVerifyTextInput pCbVerify = NULL,
			pCbPutTextInputResult pCbResult = NULL,
			pCbInplaceEditWndProc pInplaceEditCbWndProc = NULL,
			LPVOID pInplaceEditCbCoockie = NULL,
			int nInplaceEditWidth = __EXT_MENU_DEF_INPLACE_EDIT_WIDTH
			)
		{
			ASSERT( pInplaceEditStr != NULL );
			ASSERT( nInplaceEditWidth > 0 );
			m_pInplaceEditStr = pInplaceEditStr;
			m_pCbVerifyTextInput = pCbVerify;
			m_pCbPutTextInputResult = pCbResult;
			m_pInplaceEditCbWndProc = pInplaceEditCbWndProc;
			m_pInplaceEditCbCoockie = pInplaceEditCbCoockie;
			m_nInplaceEditWidth = nInplaceEditWidth;
			MeasureItem();
		}
		void ResetActiveInplaceEditPtr()
		{
			m_pHelperInplaceEditWnd = NULL;
		}
		CWnd * GetInplaceEditPtr()
		{
			if( !IsInplaceEdit() )
				return NULL;
			return m_pHelperInplaceEditWnd;
		}
		int GetInplaceEditWidth() const
		{
			if( !IsInplaceEdit() )
				return 0;
			ASSERT( m_nInplaceEditWidth > 0 );
			return m_nInplaceEditWidth;
		}
		bool IsAllowInplaceEditActivation() const
		{
			if( !IsInplaceEdit() )
				return false;
			return m_bAllowInplaceEditActivation;
		}
		void AllowInplaceEditActivation(
			bool bAllowInplaceEditActivation
			)
		{
			m_bAllowInplaceEditActivation = bAllowInplaceEditActivation;
		}
		CExtSafeString GetInplaceEditText() const
		{
			CExtSafeString s( _T("") );
			if(		IsInplaceEdit()
				&&	m_pInplaceEditStr != NULL
				)
			{
				if( m_bTempSelTextValid )
					s = m_sTempSelText;
				else
					s = *m_pInplaceEditStr;
			}
			return s;
		}
		void SetTempSelectedInplaceEditText(
			__EXT_MFC_SAFE_LPCTSTR sTempSelText = NULL // NULL - reset
			)
		{
			m_bTempSelTextValid = false;
			m_sTempSelText.Empty();
			if(		(!IsInplaceEdit())
				||	sTempSelText == NULL
				)
				return;
			m_bTempSelTextValid = true;
			m_sTempSelText = sTempSelText;
		}
		bool CreateInplaceEdit(
			CExtPopupMenuWnd * pPopup,
			CRect rc
			)
		{
			if( m_pInplaceEditStr == NULL )
			{
				ASSERT( FALSE );
				return false;
			}
			m_pHelperInplaceEditWnd = NULL;
			HWND hWndCmdReciever = GetCmdReciever();
			if(		hWndCmdReciever != NULL
				&&	::IsWindow( hWndCmdReciever )
				)
			{
				CExtPopupMenuWnd::INPLACEEDITCREATEINFO _ieci(
					&m_pHelperInplaceEditWnd,
					pPopup,
					this,
					m_pInplaceEditStr,
					m_pCbVerifyTextInput,
					m_pCbPutTextInputResult,
					m_pInplaceEditCbWndProc,
					m_pInplaceEditCbCoockie,
					rc
					);
				::SendMessage(
					hWndCmdReciever,
					CExtPopupBaseWnd::g_nMsgCreateInplaceEdit,
					(WPARAM)(&_ieci),
					(LPARAM)0L
					);
			}
			
			if( m_pHelperInplaceEditWnd == NULL )
			{
				m_pHelperInplaceEditWnd =
					new CInPlaceEditWnd(
						m_pInplaceEditStr,
						m_pCbVerifyTextInput,
						m_pCbPutTextInputResult,
						m_pInplaceEditCbWndProc,
						m_pInplaceEditCbCoockie
						);
				if( !((CInPlaceEditWnd*)m_pHelperInplaceEditWnd)->
						Create( pPopup, rc, GetCmdID() ) )
				{
					ASSERT( FALSE );
					delete m_pHelperInplaceEditWnd;
					m_pHelperInplaceEditWnd = NULL;
					return false;
				}
			} // if( m_pHelperInplaceEditWnd == NULL )
#ifdef _DEBUG
			else
			{
				ASSERT(
						m_pHelperInplaceEditWnd->GetSafeHwnd() != NULL
						&&	::IsWindow( m_pHelperInplaceEditWnd->GetSafeHwnd() )
					);
			} // else from if( m_pHelperInplaceEditWnd == NULL )
#endif // _DEBUG

			return true;
		}

		CRect AdjustInplaceEditRect( const RECT & rcItem ) const;
		
	protected:
		void SetSeparator(
			int nItemIndex
			)
		{
			if( !IsSeparator() )
			{
				if( m_pWndChild != NULL )
					DestroyPopup();
				ASSERT( m_pWndChild == NULL );
				m_sItemText.Empty();
				m_nCmdID = TYPE_SEPARATOR;
				m_nItemIndex = nItemIndex;
			}
			MeasureItem();
		}

		void SetPopup(
			int nItemIndex,
			__EXT_MFC_SAFE_LPCTSTR sText = NULL,
			HICON hIcon = NULL,
			CExtPopupMenuWnd * pSpecPopup = NULL
			)
		{
			ASSERT( m_hWndSpecCmdReciever != NULL );
			ASSERT( ::IsWindow(m_hWndSpecCmdReciever) );

			if( m_pWndChild == NULL )
			{
				if( pSpecPopup == NULL )
				{
					ConstructPopup();
				} // if( pSpecPopup == NULL )
				else
				{
					m_pWndChild = pSpecPopup;
					ASSERT( m_pWndChild != NULL );
					m_pWndChild->m_hWndCmdReciever = m_hWndSpecCmdReciever;
				} // else from if( pSpecPopup == NULL )
			}
			
			m_nCmdID = (UINT)TYPE_POPUP;
			m_sItemText = _T("");
			m_sAccelText = _T("");
			m_nItemIndex = nItemIndex;
			ASSERT( m_iconPopup.IsEmpty() );
			VERIFY( SetPopupIcon(hIcon) );
			VERIFY( SetPopupText(sText) );
			MeasureItem();
		}
		bool SetPopupIcon( HICON hIcon, bool bCopyIcon = true )
		{
			if( ! ( IsPopup() || IsNoCmdUI() ) )
			{
				ASSERT( FALSE );
				return false;
			}
			if( hIcon == NULL )
			{
				m_iconPopup.Empty();
				return true;
			}
			m_iconPopup.AssignFromHICON( hIcon, bCopyIcon );
			return true;
		}
		bool SetPopupIcon( const CExtCmdIcon & icon )
		{
			if( ! ( IsPopup() || IsNoCmdUI() ) )
			{
				ASSERT( FALSE );
				return false;
			}
			if( icon.IsEmpty() )
			{
				m_iconPopup.Empty();
				return true;
			}
			m_iconPopup = icon;
			return true;
		}
		CExtCmdIcon & GetPopupIcon()
		{
			return m_iconPopup;
		}
		const CExtCmdIcon & GetPopupIcon() const
		{
			return m_iconPopup;
		}
		bool SetPopupText( __EXT_MFC_SAFE_LPCTSTR strText );
		bool SetPopupAccelText( __EXT_MFC_SAFE_LPCTSTR strText ); // NULL if empty
		bool ConstructPopup();
		void DestroyPopup();
	public:
		CExtPopupMenuWnd * GetPopup()
		{
			ASSERT( m_pWndChild != NULL );
			return m_pWndChild;
		}
		const CExtPopupMenuWnd * GetPopup() const
		{
			ASSERT( m_pWndChild != NULL );
			return m_pWndChild;
		}
		bool IsPopup() const
		{
			return ( m_nCmdID == TYPE_POPUP ) ? true : false;
		}

		UINT GetCmdID() const;
		UINT SetCmdID( UINT nCmdID );

		bool IsExecutableCmdID() const
		{
			if( IsSeparator() || IsPopup() )
				return false;
			return true;
		}

		int GetMeasuredHeight() const
		{
			return m_sizeItem.cy;
		}
		int GetMeasuredWidth() const
		{
			return m_sizeItem.cx;
		}
		int GetIconAreaWidth() const
		{
			return m_nIconAreaWidth;
		}
		void UpdateIconAreaWidth(
			INT nIconAreaWidthLager
			)
		{
			if( IsToolButton() )
				return;
			if( nIconAreaWidthLager <= m_nIconAreaWidth )
				return;
			if( !IsSeparator() )
				m_sizeItem.cx +=
					nIconAreaWidthLager - m_nIconAreaWidth;
			m_nIconAreaWidth = nIconAreaWidthLager;
		}

		CRect GetBaseRect() const
		{
			CRect rcItem(
				CPoint(0,0),
				m_sizeItem
				);
			return rcItem;
		}
		
		int GetIndex() const
		{
			return m_nItemIndex;
		}
	protected:
		void SetIndex( int nItemIndex )
		{
			m_nItemIndex = nItemIndex;
		}

		friend class CExtPopupMenuWnd;
		friend class CExtPopupMenuCmdUI;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
		static bool stat_CbVerifyTextInput(
			CInPlaceEditWnd & wndEdit,
			CExtCustomizeCmdTreeNode * pNode,
			__EXT_MFC_SAFE_LPCTSTR sTextOld,
			__EXT_MFC_SAFE_LPCTSTR sTextNew
			);
		static void stat_CbPutTextInputResult(
			CInPlaceEditWnd & wndEdit,
			CExtCustomizeCmdTreeNode * pNode,
			__EXT_MFC_SAFE_LPCTSTR sTextNew
			);
		static bool stat_CbInplaceEditWndProc(
			LRESULT & lResult,
			UINT message,
			WPARAM wParam,
			LPARAM lParam,
			CEdit & wndEdit,
			CExtCustomizeCmdTreeNode * pNode
			);
		static bool stat_CbInitListBoxContent(
			CExtPopupInplaceListBox & wndListBox,
			CExtCustomizeCmdTreeNode * pNode
			);
		static bool stat_CbListBoxSelection(
			CExtPopupInplaceListBox & wndListBox,
			CExtCustomizeCmdTreeNode * pNode,
			int eSAT // CExtPopupInplaceListBox::e_sel_action_t
			);
		static bool stat_CbListBoxItemDraw(
			CExtPopupInplaceListBox & wndListBox,
			CExtCustomizeCmdTreeNode * pNode,
			LPDRAWITEMSTRUCT pDIS
			);
		static bool stat_CbListBoxItemMeasure(
			CExtPopupInplaceListBox & wndListBox,
			CExtCustomizeCmdTreeNode * pNode,
			LPMEASUREITEMSTRUCT pMIS
			);
	#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	}; // class MENUITEMDATA

	
	CExtPopupMenuWnd::MENUITEMDATA & ItemGetInfo( INT nPos );
	const CExtPopupMenuWnd::MENUITEMDATA & ItemGetInfo( INT nPos ) const;

	/////////////////////////////////////////////////////////////////////////////
	// CExtPopupMenuWnd::DRAWITEMDATA
	
	struct DRAWITEMDATA;
	friend struct DRAWITEMDATA;
	
	struct __PROF_UIS_API DRAWITEMDATA
	{
		CDC * m_pDC;
		LPCRECT m_pRectItem;
		const MENUITEMDATA * m_pItemData;
		CExtPopupMenuWnd * m_pPopup;
		const bool m_bMenuOwnerDrawFixed:1;
		const bool m_bPopupItem:1;
		const bool m_bRarelyUsed:1;
		const bool m_bRarelyUsedPrevious:1;
		const bool m_bRarelyUsedNext:1;
	protected:
		DRAWITEMDATA(
			CDC * pDC,
			LPCRECT pRectItem,
			const MENUITEMDATA * pItemData,
			CExtPopupMenuWnd * pPopup,
			bool bMenuOwnerDrawFixed,
			bool bPopupItem,
			bool bRarelyUsed,
			bool bRarelyUsedPrevious,
			bool bRarelyUsedNext
			)
			: m_pDC( pDC )
			, m_pRectItem( pRectItem )
			, m_pItemData( pItemData )
			, m_pPopup( pPopup )
			, m_bMenuOwnerDrawFixed( bMenuOwnerDrawFixed )
			, m_bPopupItem( bPopupItem )
			, m_bRarelyUsed( bRarelyUsed )
			, m_bRarelyUsedPrevious( bRarelyUsedPrevious )
			, m_bRarelyUsedNext( bRarelyUsedNext )
		{
			ASSERT( m_pDC != NULL );
			ASSERT( m_pDC->GetSafeHdc() != NULL );
			ASSERT( m_pRectItem != NULL );
			ASSERT( m_pItemData != NULL );
			ASSERT( m_pPopup != NULL );
		}
	public:
		operator CDC *()
		{
			ASSERT( this != NULL );
			ASSERT( m_pDC != NULL );
			ASSERT( m_pDC->GetSafeHdc() != NULL );
			return m_pDC;
		}
		operator LPCRECT()
		{
			ASSERT( this != NULL );
			ASSERT( m_pRectItem != NULL );
			return m_pRectItem;
		}
		operator const MENUITEMDATA *()
		{
			ASSERT( this != NULL );
			ASSERT( m_pItemData != NULL );
			return m_pItemData;
		}
		operator __EXT_MFC_SAFE_LPCTSTR()
		{
			ASSERT( this != NULL );
			ASSERT( m_pItemData != NULL );
			return m_pItemData->GetText();
		}
		operator CSize()
		{
			ASSERT( this != NULL );
			ASSERT( m_pRectItem != NULL );
			return
				CSize(
					m_pRectItem->right - m_pRectItem->left,
					m_pRectItem->bottom - m_pRectItem->top
					);
		}
		operator CExtPopupMenuWnd *()
		{
			ASSERT( this != NULL );
			ASSERT( m_pPopup != NULL );
			return m_pPopup;
		}
		CExtPopupMenuWnd * operator ->()
		{
			ASSERT( this != NULL );
			ASSERT( m_pPopup != NULL );
			return m_pPopup;
		}
		operator LPARAM()
		{
			return reinterpret_cast < LPARAM > ( this );
		}
		UINT GetCmdID()
		{
			ASSERT( this != NULL );
			ASSERT( m_pItemData != NULL );
			return m_pItemData->GetCmdID();
		}
		void PaintDefault(
			bool bForceNoIcon = false,
			bool bForceNoText = false,
			bool bForceNoCheck = false,
			bool bForceEnabled = false,
			bool bForceUnselected = false
			);
		void PaintDefaultBk()
		{
			PaintDefault(
				true,
				true,
				true,
				true,
				false
				);
		}
	protected:
		bool DoOwnerDrawPainting();
		friend class CExtPopupMenuWnd;
	}; // struct DRAWITEMDATA

	/////////////////////////////////////////////////////////////////////////////
	// CExtPopupMenuWnd::DRAWLEFTAREADATA
	
	struct DRAWLEFTAREADATA;
	friend struct DRAWLEFTAREADATA;
	
	struct __PROF_UIS_API DRAWLEFTAREADATA
	{
		CDC * m_pDC;
		LPCRECT m_pRectLeftArea;
		CExtPopupMenuWnd * m_pPopup;
	protected:
		DRAWLEFTAREADATA(
			CDC * pDC,
			LPCRECT pRectLeftArea,
			CExtPopupMenuWnd * pPopup
			)
			: m_pDC( pDC )
			, m_pRectLeftArea( pRectLeftArea )
			, m_pPopup( pPopup )
		{
			ASSERT( m_pDC != NULL );
			ASSERT( m_pDC->GetSafeHdc() != NULL );
			ASSERT( m_pRectLeftArea != NULL );
			ASSERT( m_pPopup != NULL );
		}
	public:
		operator CDC *()
		{
			ASSERT( this != NULL );
			ASSERT( m_pDC != NULL );
			ASSERT( m_pDC->GetSafeHdc() != NULL );
			return m_pDC;
		}
		operator LPCRECT()
		{
			ASSERT( this != NULL );
			ASSERT( m_pRectLeftArea != NULL );
			return m_pRectLeftArea;
		}
		operator CSize()
		{
			ASSERT( this != NULL );
			ASSERT( m_pRectLeftArea != NULL );
			return
				CSize(
					m_pRectLeftArea->right - m_pRectLeftArea->left,
					m_pRectLeftArea->bottom - m_pRectLeftArea->top
					);
		}
		operator CExtPopupMenuWnd *()
		{
			ASSERT( this != NULL );
			ASSERT( m_pPopup != NULL );
			return m_pPopup;
		}
		CExtPopupMenuWnd * operator ->()
		{
			ASSERT( this != NULL );
			ASSERT( m_pPopup != NULL );
			return m_pPopup;
		}
		operator LPARAM()
		{
			return reinterpret_cast < LPARAM > ( this );
		}
	protected:
		bool DoOwnerDrawPainting();
		friend class CExtPopupMenuWnd;
		friend class CExtPopupColorMenuWnd;
	}; // struct DRAWLEFTAREADATA

protected:

	typedef
		CArray < MENUITEMDATA, MENUITEMDATA & >
		items_container_t;
	typedef
		const items_container_t
		const_items_container_t;
	items_container_t m_items_all;
//	items_container_t & _GetItemsContaner();
//	const_items_container_t & _GetItemsContaner() const;
	MENUITEMDATA & _GetItemRef(int nIndex);
	const MENUITEMDATA & _GetItemRef(int nIndex) const;
	virtual void _InsertItem(
		int nInsertBefore,
		MENUITEMDATA & mi
		);

public:

	static bool TestHoverEnabledFromActiveHWND(
		HWND hWndTestFrom,
		HWND hWndActive = ::GetActiveWindow(),
		bool bCheckEnabled = true,
		bool bCheckVisible = true,
		bool bCheckExtPopupMenuTracking = true
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupMenuWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPopupMenuWnd();
	virtual CExtPopupMenuSite & GetSite() const;

protected:
	virtual void _StartAnimation();
	virtual void _EndAnimation()
	{
		ASSERT_VALID( this );

		CExtPopupBaseWnd::_EndAnimation();
		Invalidate( FALSE );
		_SetCapture();
	}
	

public:
	virtual bool _FindHelpMode() const;

	virtual BOOL DestroyWindow();

protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CExtPopupMenuWnd)
	afx_msg void OnCancelMode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	friend class CExtPopupMenuCmdUI;
}; // class CExtPopupMenuWnd

class __PROF_UIS_API CExtPopupColorMenuWnd :
	public CExtPopupMenuWnd
{
public:
	DECLARE_DYNCREATE(CExtPopupColorMenuWnd)

	CExtPopupColorMenuWnd();

// Attributes
public:
	static UINT g_nMsgNotifyColorChanged;
	static UINT g_nMsgNotifyColorChangedFinally;
	static UINT g_nMsgNotifyCustColor;

	HWND m_hWndNotifyColorChanged; // def is NULL - use command targed
	LPARAM m_lParamCookie;

	COLORREF m_clrDefault,m_clrInitial;
	bool m_bEnableBtnColorDefault:1;
	bool m_bEnableBtnColorCustom:1;

	CExtSafeString m_sBtnTextColorDefault;
	CExtSafeString m_sBtnTextColorCustom;

protected:
	CRect m_rcDefColorText,m_rcCustColorText;

	enum
	{
		IDX_DEFAULT_COLOR_BTN = -4,
		IDX_CUSTOM_COLOR_BTN = -5
	};

// Implementation
protected:
	int m_nColorIdxCurr, m_nInitialColorIdx;

	HWND _GetWndNotifyColorChanged();

	int _FindCellByColorRef(COLORREF clr);

	virtual bool _CreateHelper(
		CWnd * pWndCmdReciever
		);

	struct __PROF_UIS_API COLORREF_TABLE_ENTRY
	{
		COLORREF m_clr;
		__EXT_MFC_SAFE_LPCTSTR m_sName;
		COLORREF_TABLE_ENTRY(
			COLORREF clr = RGB(0,0,0),
			__EXT_MFC_SAFE_LPCTSTR sName = NULL
			)
			: m_clr( clr )
			, m_sName( sName )
		{
		}
	}; // struct COLORREF_TABLE_ENTRY

	static COLORREF_TABLE_ENTRY g_colors[];

public:
	virtual int _HitTest(
		const CPoint & point,
		bool * p_bInplaceControlArea = NULL,
		bool * p_bInplaceDropDownArea = NULL
		)
	{
		ASSERT_VALID( this );
		if( p_bInplaceControlArea != NULL )
			*p_bInplaceControlArea = false;
		if( p_bInplaceDropDownArea != NULL )
			*p_bInplaceDropDownArea = false;
		point;
		return IDX_NOTHING;
	}
protected:

	void _NotifyColorChanged(
		bool bFinal = false
		);

	CPoint _GetColorItemCoord(int nIdx);
	CRect _GetColorItemRect(int nIdx);
	int _ColorItemHitTest(const CPoint & point);

	virtual CRect _CalcTrackRect();
	virtual CSize _CalcTrackSize();
public:
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );
protected:

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
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
	// CExtCustomizeSite::ICustomizeDropSource
	virtual void OnCustomizeSourceDragComplete(
		DROPEFFECT de,
		bool bCanceled,
		bool * p_bNoResetActiveItem
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

public:
	virtual bool _OnMouseWheel(
		WPARAM wParam,
		LPARAM lParam,
		bool & bNoEat
		);
	virtual bool _OnMouseMove(
		UINT nFlags,
		CPoint point,
		bool & bNoEat
		);
	virtual bool _OnMouseClick(
		UINT nFlags,
		CPoint point,
		bool & bNoEat
		);
	virtual bool _OnKeyDown(
		UINT nChar,
		UINT nRepCnt,
		UINT nFlags,
		bool & bNoEat
		);

	virtual bool _CanStartLevelTracking();

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupColorMenuWnd)
	//}}AFX_MSG
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupColorMenuWnd

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_POPUP_MENU_WND_H
