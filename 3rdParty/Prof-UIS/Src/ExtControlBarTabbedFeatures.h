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

#if (!defined __EXT_CONTROLBAR_TABBED_FEATURES_H)
#define __EXT_CONTROLBAR_TABBED_FEATURES_H

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXTDOCKBAR_H)
	#include <../Src/ExtDockBar.h>
#endif // __EXTDOCKBAR_H

/////////////////////////////////////////////////////////////////////////
// CExtDockDynTabBar window

class __PROF_UIS_API CExtDockDynTabBar : public CExtDockDynBar
{
public:

	CExtDockDynTabBar( UINT nCircleNo = 0 );
	
	DECLARE_DYNCREATE(CExtDockDynTabBar)

	virtual void OnDynamicLayoutUpdate();
	virtual void OnDynamicLayoutOptimize();

	// this is the one and only method of interest
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

	virtual void CalcOrderedVector(
		ExtControlBarVector_t & vBars
		);

	CSize _RepositionTabbedChilds(
		bool bLockChilds
		);
	void _SyncTabbedChilds(
		bool bSyncSizes,
		bool bSyncSwitcher
		);

public:

#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CExtDockDynBar::AssertValid();
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtDockDynBar::Dump( dc );
	}

#endif

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDockDynTabBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDockDynTabBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtDockDynTabBar

/////////////////////////////////////////////////////////////////////////
// CExtDynTabWnd window

class __PROF_UIS_API CExtDynTabWnd : public CExtTabWnd
{
protected:
	class __PROF_UIS_API CDynDropTarget : public COleDropTarget
	{
		CExtDynTabWnd * m_pWndDynTab;
	public:
		CDynDropTarget()
			: m_pWndDynTab( NULL )
		{
		}
		virtual DROPEFFECT OnDragEnter(
			CWnd * pWnd,
			COleDataObject * pDataObject,
			DWORD dwKeyState,
			CPoint point
			)
		{
			ASSERT_VALID( this );
			return OnDragOver( pWnd, pDataObject, dwKeyState, point );
		}
		virtual DROPEFFECT OnDragOver(
			CWnd * pWnd,
			COleDataObject * pDataObject,
			DWORD dwKeyState,
			CPoint point
			);
		friend class CExtDynTabWnd;
	}; // class CDynDropTarget

	CDynDropTarget m_oleDropTarget;

public:

	CExtDynTabWnd();
	
	DECLARE_DYNCREATE(CExtDynTabWnd)

#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CExtTabWnd::AssertValid();
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtTabWnd::Dump( dc );
	}

#endif

protected:

	virtual CExtCmdIcon * OnTabWndQueryItemIcon(
		const TAB_ITEM_INFO * pTii
		) const;
	virtual __EXT_MFC_SAFE_LPCTSTR OnTabWndQueryItemText(
		const TAB_ITEM_INFO * pTii
		) const;

	virtual bool OnTabWndStartDrag( LONG nIndex );

	virtual bool OnTabWndClickedItem(
		LONG nItemIndex,
		bool bButtonPressed,
		INT nMouseButton, // MK_... values
		UINT nMouseEventFlags
		);
	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		);

	virtual void OnTabWndDrawItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon
		);

	LONG m_nSavedIndex;

public:

	LONG FindControlBar(
		const CExtControlBar * pBar
		) const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( pBar );
		LPARAM lParam =
			reinterpret_cast < LPARAM > ( pBar );
		LONG nIndex = ItemFindByLParam( lParam, -1, true, true );
		return nIndex;
	}

	void SaveIndex( LONG nIndex = -1 )
	{
		ASSERT_VALID( this );
		m_nSavedIndex = nIndex;
	}
	void SaveIndex( CExtControlBar * pBar )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( pBar );
		m_nSavedIndex = FindControlBar( pBar );
		ASSERT( m_nSavedIndex >= 0 && m_nSavedIndex < ItemGetCount() );
	}
	void RestoreIndex( CExtControlBar * pBar )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( pBar );
		ASSERT( pBar->GetParent()->GetParent() == GetParent() );

		ASSERT( m_nSavedIndex >= 0 && m_nSavedIndex <= ItemGetCount() );
		LONG nExistingIndex = FindControlBar( pBar );
		if( nExistingIndex != m_nSavedIndex )
		{
			if( nExistingIndex >= 0 )
				ItemRemove( nExistingIndex, 1, false );
			ASSERT( m_nSavedIndex >= 0 && m_nSavedIndex <= ItemGetCount() );
			InsertControlBar( pBar, m_nSavedIndex );
		}
		m_nSavedIndex = -1L;
		UpdateTabWnd( true );
	}

	void SelectControlBar( CExtControlBar * pBar )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( pBar );
		ASSERT( pBar->GetParent()->GetParent() == GetParent() );
		LONG nExistingIndex = FindControlBar( pBar );
		ASSERT( nExistingIndex >= 0 && nExistingIndex < ItemGetCount() );
		SelectionSet( nExistingIndex, true, true );
		pBar->SetFocus();
	}
	
	void InsertControlBar(
		CExtControlBar * pBar,
		INT nPos = -1, // default - append
		bool bTempOnDrag = false, // is tab item temporary (when dragging)
		bool bUpdateTabWnd = false
		);
	void RemoveTemporaryItems(
		bool bUpdateTabWnd = false
		);

	CExtControlBar * GetBarAt(
		LONG nIndex,
		bool bExcludeTemporary
		);

	LONG ReindexPosCalc( CPoint ptCursor );

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDynTabWnd)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDynTabWnd)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CExtDynTabControlBar;
}; // class CExtDynTabWnd

/////////////////////////////////////////////////////////////////////////
// CExtDynTabControlBar window

class __PROF_UIS_API CExtDynTabControlBar : public CExtDynControlBar
{
	CExtDynTabWnd m_wndTabSwitcher;

public:

	CExtDynTabControlBar();
	
	DECLARE_DYNCREATE(CExtDynTabControlBar)


#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CExtDynControlBar::AssertValid();
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtDynControlBar::Dump( dc );
	}

#endif

public:
	virtual bool IsBarWithGripper(
		bool * pbGripperAtTop = NULL,
		bool * pbTextOnGripper = NULL
		) const;

	void InsertTemporaryItemsFrom(
		CExtControlBar * pBar,
		LONG nIndex,
		bool bUpdateTabWnd
		);

	void InsertBar( 
		CExtControlBar * pBar,
		LONG nIndex,
		bool bUpdateTabWnd
		);
	
	void UpdateBarVisibility(
		CExtControlBar * pBar
		);


	LONG FindControlBar(
		const CExtControlBar * pBar
		) const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		return m_wndTabSwitcher.FindControlBar( pBar );
	}

	void SaveIndex( LONG nIndex = -1 )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		m_wndTabSwitcher.SaveIndex( nIndex );
	}

	void SaveIndex( CExtControlBar * pBar )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		m_wndTabSwitcher.SaveIndex( pBar );
	}

	void RestoreIndex( CExtControlBar * pBar )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		m_wndTabSwitcher.RestoreIndex( pBar );
	}

	void SelectControlBar( CExtControlBar * pBar )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		m_wndTabSwitcher.SelectControlBar( pBar );
	}

	void InsertControlBar(
		CExtControlBar * pBar,
		INT nPos = -1, // default - append
		bool bTempOnDrag = false, // is tab item temporary (when dragging)
		bool bUpdateTabWnd = false
		)
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		m_wndTabSwitcher.InsertControlBar(
			pBar,
			nPos,
			bTempOnDrag,
			bUpdateTabWnd
			);
	}

	void RemoveTemporaryItems(
		bool bUpdateTabWnd = false
		)
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		m_wndTabSwitcher.RemoveTemporaryItems( bUpdateTabWnd );
	}

	CExtControlBar * GetBarAt(
		LONG nIndex,
		bool bExcludeTemporary
		)
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		return
			m_wndTabSwitcher.GetBarAt(
				nIndex,
				bExcludeTemporary
				);
	}

	LONG ReindexPosCalc( CPoint ptCursor )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		return m_wndTabSwitcher.ReindexPosCalc( ptCursor );
	}

	void InvalidateSwitcher()
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		if( m_wndTabSwitcher.GetSafeHwnd() == NULL
			|| ( ! ::IsWindow( m_wndTabSwitcher.GetSafeHwnd() ) )
			|| ( ! m_wndTabSwitcher.IsWindowVisible() )
			)
			return;
		m_wndTabSwitcher.Invalidate();
		m_wndTabSwitcher.UpdateWindow();
	}

	void SyncSwitcher();
	void RemoveSelFromSwitcher();

	bool IsSwitcherVisible()
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		if( m_wndTabSwitcher.GetSafeHwnd() == NULL
			|| ( ! ::IsWindow( m_wndTabSwitcher.GetSafeHwnd() ) )
			)
			return false;
		bool bVisible = (m_wndTabSwitcher.GetStyle() & WS_VISIBLE) ? true : false;
		return bVisible;
	}

	void GetSwitcherWindowRect( RECT * pRcSwitcherWnd ) const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		ASSERT( pRcSwitcherWnd != NULL );
		m_wndTabSwitcher.GetWindowRect( pRcSwitcherWnd );
	}

	LONG GetSwitcherItemCount() const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		return m_wndTabSwitcher.ItemGetCount();
	}

	LONG SetSwitcherSelection(
		LONG nSelIndex,
		bool bEnsureVisible = false,
		bool bUpdateTabWnd = false
		)
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		return
			m_wndTabSwitcher.SelectionSet(
				nSelIndex,
				bEnsureVisible,
				bUpdateTabWnd
				);
	}
	LONG GetSwitcherSelection() const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&m_wndTabSwitcher) );
		return
			m_wndTabSwitcher.SelectionGet();
	}


protected:
	virtual bool _CanDockToTabbedContainers(
		CExtControlBar * pDestBar
		) const;
	virtual CExtDynTabControlBar * _GetNearestTabbedContainer();

	void _InsertTemporaryItemsFromImpl(
		CExtControlBar * pBar,
		LONG & nIndex
		);
	void _InsertBarImpl( 
		CExtControlBar * pBar,
		LONG & nIndex
		);

	INT _CalcTabHeight() const;

	virtual CSize _CalcLayoutMinSize() const;

	virtual INT _CalcDesiredMinHW() const;
	virtual INT _CalcDesiredMinVH() const;
	virtual CSize _CalcDesiredMinFloatedSize() const;

	virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD nMode
		);

	virtual CExtDockDynBar * OnCreateDynamicDockBarObject();

public:
	virtual void OnRepositionSingleChild(
		int cx = -1, // if negative - get from client area
		int cy = -1,
		bool bRedraw = true
		);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDynTabControlBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDynTabControlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CExtDynTabWnd;

}; // class CExtDynTabControlBar

/////////////////////////////////////////////////////////////////////////
// CExtDynAutoHideArea window

class __PROF_UIS_API CExtDynAutoHideArea : public CExtTabWnd
{
protected:
	class __PROF_UIS_API CDynDropTarget : public COleDropTarget
	{
		CExtDynAutoHideArea * m_pWndAutoHideArea;
		CExtDynAutoHideSlider * m_pWndSlider;
	public:
		CDynDropTarget()
			: m_pWndAutoHideArea( NULL )
			, m_pWndSlider( NULL )
		{
		}
		virtual DROPEFFECT OnDragEnter(
			CWnd * pWnd,
			COleDataObject * pDataObject,
			DWORD dwKeyState,
			CPoint point
			)
		{
			ASSERT_VALID( this );
			return OnDragOver( pWnd, pDataObject, dwKeyState, point );
		}
		virtual DROPEFFECT OnDragOver(
			CWnd * pWnd,
			COleDataObject * pDataObject,
			DWORD dwKeyState,
			CPoint point
			);
		friend class CExtDynAutoHideArea;
	}; // class CDynDropTarget

	CDynDropTarget m_oleDropTarget;

	CExtDynAutoHideSlider * m_pWndSlider;
	bool m_bActivationLocked:1, m_bInsertMode:1;
	CRect m_rcLastPos;

public:

	CExtDynAutoHideArea();
	
	DECLARE_DYNCREATE(CExtDynAutoHideArea)

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual void OnCustomizeModeEnter();
	virtual void OnCustomizeModeLeave();
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CExtTabWnd::AssertValid();
		ASSERT( m_bReflectParentSizing );
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtTabWnd::Dump( dc );
	}

#endif

	virtual BOOL Create(
		CFrameWnd * pParentWnd,
		CExtDynAutoHideSlider * pWndSlider,
		UINT nDockedDockBarID
		);
	void AdjustOrder();

	void AppendControlBar(
		CExtControlBar * pBar,
		bool bForceNoShow
		);
	void RemoveControlBar(
		CExtControlBar * pBar,
		bool bForceNoShow
		);

	LONG FindControlBar(
		const CExtControlBar * pBar
		) const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( pBar );
		LPARAM lParam =
			reinterpret_cast < LPARAM > ( pBar );
		LONG nIndex = ItemFindByLParam( lParam, -1, true, true );
		return nIndex;
	}

protected:

	virtual void OnTabWndItemInsert(
		LONG nItemIndex,
		TAB_ITEM_INFO * pTii
		);

	virtual CExtCmdIcon * OnTabWndQueryItemIcon(
		const TAB_ITEM_INFO * pTii
		) const;
	virtual __EXT_MFC_SAFE_LPCTSTR OnTabWndQueryItemText(
		const TAB_ITEM_INFO * pTii
		) const;

	virtual bool OnTabWndSelectionChange(
		LONG nOldItemIndex,
		LONG nNewItemIndex,
		bool bPreSelectionTest
		);

	virtual void OnTabWndSyncVisibility();

	virtual void OnTabWndMeasureItemAreaMargins(
		CDC & dcMeasure,
		LONG & nSpaceBefore,
		LONG & nSpaceAfter,
		LONG & nSpaceOver
		);

public:
	bool _CanActivate();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDynAutoHideArea)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDynAutoHideArea)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	friend class CExtDynAutoHideSlider;
	friend class CExtControlBar;
	friend class CExtBarNcAreaButton;
}; // class CExtDynAutoHideArea

/////////////////////////////////////////////////////////////////////////
// CExtDynAutoHideSlider window

class __PROF_UIS_API CExtDynAutoHideSlider : public CWnd
{
	CExtDynAutoHideArea * m_pWndAnimateSrc;
	CExtControlBar * m_pBar;
	CSize m_sizeControlBar;
	bool m_bActiveState:1;
	bool m_bWndActive:1;
	bool m_bDeactivateTesting:1;
	bool m_bAnimation:1;
	UINT m_nResizingMode;
	INT m_nResizingStart, m_nResizingMetric;
	CExtBarNcAreaButton
		* m_pLastHitTestM,
		* m_pLastHitTestT,
		* m_pLastHitTestB;
	CToolTipCtrl m_wndToolTip;
	CRect m_rcMax;
	
	void _AnalyzeButtonsHover();
	
	static HHOOK g_hMouseHook;
	static CExtDynAutoHideSlider * g_pHookTarget;

	static LRESULT CALLBACK _HookMouseProc(
		int nCode,      // hook code
		WPARAM wParam,  // message identifier
		LPARAM lParam   // mouse coordinates
		);
	bool _HookMouseClick(
		UINT nMkCode,
		bool bDown,
		bool bNcArea,
		CPoint ptScreen
		);
	bool _HookMouseMove(
		bool bNcArea,
		CPoint ptScreen
		);

public:

	CExtDynAutoHideSlider();
	
	DECLARE_DYNCREATE(CExtDynAutoHideSlider)

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual void OnCustomizeModeEnter(
		CExtDynAutoHideArea * pWndAutoHideArea
		);
	virtual void OnCustomizeModeLeave(
		CExtDynAutoHideArea * pWndAutoHideArea
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CWnd::AssertValid();
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CWnd::Dump( dc );
	}

#endif

	static bool RegisterAutohideSliderWndClass();

	virtual BOOL Create(
		CFrameWnd * pParentWnd
		);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void Activate(
		CExtControlBar * pBar,
		bool bActivate,
		bool bAnimate,
		bool bWait
		);

	void CalcInnerNcRects(
		RECT * pRcInnerArea,
		RECT * pRcDrawResizer,
		RECT * pRcDrawGripper
		);
	void CalcNcOffsets(
		RECT * pRcNcSizes
		);
	// for command hit testing (used for automatic tooltips)
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDynAutoHideSlider)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDynAutoHideSlider)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	friend class CExtDynAutoHideArea;
	friend class CExtDynAutoHideArea::CDynDropTarget;
	friend class CExtControlBar;
	friend class CExtBarNcAreaButton;

}; // class CExtDynAutoHideSlider

#define __AUTOHIDE_SLIDER_WNDCLASS _T("ProfUIS-AutohideSlider")

#define __DYNTAB_ITEM_STYLE_TEMPORARY __ETWI_USER_STYLE_00


#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

#endif // __EXT_CONTROLBAR_TABBED_FEATURES_H
