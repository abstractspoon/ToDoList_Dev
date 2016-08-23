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

#if (!defined __EXT_MENUCONTROLBAR_H)
#define __EXT_MENUCONTROLBAR_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_HOOK_H)
	#include "../Src/ExtHook.h"
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiDocButton

class __PROF_UIS_API CExtBarMdiDocButton : public CExtBarButton
{
public:
	DECLARE_DYNCREATE(CExtBarMdiDocButton)

protected:
	CExtCmdIcon m_iconCache;

	HWND _GetActiveMdiChildWnd(
		BOOL & bMaximized
		);

public:
	CExtBarMdiDocButton(
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		) :
		CExtBarButton(
			pBar,
			nCmdID,
			nStyle
			)
	{
	}
	~CExtBarMdiDocButton()
	{
		CExtBarButton::~CExtBarButton();
	}

	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);
	virtual UINT GetStyle() const
	{
		return
			CExtBarButton::GetStyle()
			&
			(~TBBS_DISABLED)
			;
	}
	virtual void SetStyle( UINT nStyle )
	{
		nStyle &= ~TBBS_DISABLED;
		CExtBarButton::SetStyle( nStyle );
	}
	virtual void ModifyStyle(
		UINT nAdd,
		UINT nRemove = 0
		)
	{
		nAdd &= ~TBBS_DISABLED;
		nRemove |= TBBS_DISABLED;
		CExtBarButton::ModifyStyle(
			nAdd,
			nRemove
			);
	}

	virtual CExtCmdIcon * GetIconPtr();

	virtual CWnd * GetCmdTargetWnd();

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

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny
		);
}; // class CExtBarMdiDocButton

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiRightButton

#define __GAP_BETWEEN_WND_RIGHT_BUTTONS 2

class CExtMenuControlBar;

class __PROF_UIS_API CExtBarMdiRightButton : public CExtBarMdiDocButton
{
protected:
	struct __PROF_UIS_API MdiMenuBarRightButtonsInfo_t
	{
		CWnd * m_pWndMdiChildFrame;
		CExtMenuControlBar * m_pBar;
		int m_cxIcon,m_cyIcon;
		CSize m_calcSize;
		bool
			m_bInitialized:1,
			m_bBtnClose:1,
			m_bBtnMaximize:1,
			m_bBtnMinimize:1,
			m_bBtnHelp:1;
		CRect
			m_rcBtnClose,
			m_rcBtnMaximize,
			m_rcBtnRestore,
			m_rcBtnMinimize,
			m_rcBtnHelp;
		MdiMenuBarRightButtonsInfo_t();
		void EmptyLocations();
		void InitLocations(
			const CRect & rcBtn,
			BOOL bHorz
			);
	};

	friend class CExtMenuControlBar;
	
public:
	DECLARE_DYNCREATE(CExtBarMdiRightButton)

public:
	CExtBarMdiRightButton(
		CExtToolControlBar * pBar = NULL,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		) :
		CExtBarMdiDocButton(
			pBar,
			nCmdID,
			nStyle
			)
	{
	};
	~CExtBarMdiRightButton()
	{
		CExtBarMdiDocButton::~CExtBarMdiDocButton();
	};

	virtual CExtCmdIcon * GetIconPtr();

	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);

	virtual void Paint(
		CDC & dc,
		bool bHorz
		);

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual UINT OnTrackPopup(
		CPoint point,
		bool bSelectAny
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

}; // class CExtBarMdiRightButton

/////////////////////////////////////////////////////////////////////////////
// CExtMenuControlBar

class __PROF_UIS_API CExtMenuControlBar
	: public CExtToolControlBar
	, public CExtHookSink
{
protected:
	friend class CExtBarMdiDocButton;
	friend class CExtBarMdiRightButton;
	friend struct CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t;

	virtual void _GetMdiMenuBarRightButtonsInfo(
		CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t & _info,
		CExtBarMdiRightButton * pTBB,
		BOOL bHorz
		);

	UINT m_nMdiDocButtonCmdID;
	HWND m_hWndHelper;

	CExtSafeString m_sMdiWindowPopupName;
	
	bool m_bFlatTracking:1,	m_bSysMenuTracking:1;
	int m_nFlatTrackingIndex,m_nOldTrackingIndex;
	CMenu m_menuFrame, m_menuDoc;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	CExtCustomizeSite::CCmdMenuInfo * m_pMenuInfo;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	virtual void _UpdateFlatTracking(
		BOOL bRepaint = TRUE
		);
	CRect _GetMainFrameSysIconRect();
	CRect _GetChildFrameSysIconRect();

	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		);

	HWND _GetHwndPlacement();
	HWND _GetHwndMainFrame();
	HWND _GetHwndChildFrame();
	HWND _GetHwndMdiArea();

	bool m_bMdiApp:1;
	virtual BOOL _UpdateMenuBar(
		BOOL bDoRecalcLayout = TRUE
		);
	virtual CMenu * _GetMenu();
	virtual HWND _GetActiveMdiChildWnd(
		BOOL & bMaximized
		);
	virtual BOOL _InstallMdiDocButtons(
		BOOL bDoRecalcLayout = TRUE
		);
	virtual BOOL _SyncActiveMdiChild();
	virtual BOOL _TrackFrameSystemMenu(
		CFrameWnd * pFrame,
		CPoint * pPoint = NULL, // NULL means calc meny track area automatically
		BOOL bSelectAny = FALSE,
		LPCRECT rcExcludeArea = NULL,
		UINT nTrackFlags = (UINT)(-1),
		BOOL bCombinedMode = FALSE
		);

public:
    DECLARE_DYNCREATE(CExtMenuControlBar);
// Construction
public:
    CExtMenuControlBar();

// Attributes
public:
	// hide expand button on menu bar if possible ?
	static bool g_bHideMenuBarExpandContentButton;
	
	static bool g_bCancelNearestAltUp;

	// multiply rows in docked state
	bool m_bMultiRowLayout:1;

	virtual CSize _GetDefButtonSize()
	{
		return g_sizeDefTBB;
	}

protected:
	static const CSize g_sizeDefTBB; // default size of button
	bool m_bDelayedSyncActiveMdiChild:1;
	bool m_bDelayedUpdateMenuBar:1;
	virtual void _DelaySyncActiveMdiChild();
	virtual void _DelayUpdateMenuBar();

// Operations
public:

	CMenu * GetMenu(); // get current menu
	BOOL UpdateMenuBar( // update after menu changed
		BOOL bDoRecalcLayout = TRUE
		);

	virtual void SetMdiWindowPopupName(
		__EXT_MFC_SAFE_LPCTSTR sMdiWindowPopupName = NULL
		);

	// LoadMenuBar() required only for CFrameWnd
	// based windows principally without menu
	virtual BOOL LoadMenuBar(
		UINT nResourceID
		);

	virtual BOOL TranslateMainFrameMessage(MSG* pMsg);
	virtual BOOL TrackMainFrameSystemMenu(
		CPoint * pPoint = NULL, // NULL means calc meny track area automatically
		BOOL bSelectAny = FALSE
		);
	virtual BOOL TrackChildFrameSystemMenu(
		CPoint * pPoint = NULL, // NULL means calc meny track area automatically
		BOOL bSelectAny = FALSE
		);

// Overridables

public:
	virtual void OnUpdateCmdUI(
		CFrameWnd * pTarget,
		BOOL bDisableIfNoHndler
		);
	virtual LRESULT DoHelpHitTest( CPoint ptHelpHitTest );

	virtual bool _GetFullRowMode() const;

	virtual bool OnQueryMultiRowLayout() const;
	virtual CExtBarContentExpandButton * OnCreateBarRightBtn();

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual CExtCustomizeSite::CCmdMenuInfo * MenuInfoGet();
	virtual CExtCustomizeSite::CCmdMenuInfo * MenuInfoDetach();
	virtual CExtCustomizeSite::CCmdMenuInfo * MenuInfoAttach(
		CExtCustomizeSite::CCmdMenuInfo * pMenuInfo
		);
	virtual void MenuInfoUpdate();
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

protected:
	virtual void _Dragging_OnStart();
	virtual void _Dragging_OnStop(
		InternalDraggingState_t & _dsOld,
		InternalDraggingState_t & _dsNew,
		bool bCancel
		);
	virtual BOOL TrackButtonMenu(
		int nIndex,
		BOOL bSelectAny = FALSE,
		BOOL bDelayedTracking = FALSE
		);
	virtual void _RecalcPositionsImpl();
	virtual void _RecalcLayoutImpl();
	virtual void _KillFrameMenu();

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtMenuControlBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
public:
    virtual ~CExtMenuControlBar();

	BOOL IsMenuBarTracking()
	{
		if(	m_bFlatTracking
			|| m_bSysMenuTracking
			)
			return TRUE;
		return FALSE;
	}

	virtual bool IsDisplayMdiDocumentButtons() const;
	virtual	bool _OnMouseMoveMsg(UINT nFlags, CPoint point);

// Generated message map functions
protected:
    //{{AFX_MSG(CExtMenuControlBar)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnTrackButtonMenu(WPARAM wParam,LPARAM lParam);
    DECLARE_MESSAGE_MAP()
}; // class CExtMenuControlBar

#endif // __EXT_MENUCONTROLBAR_H

