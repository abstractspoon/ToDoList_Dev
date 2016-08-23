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

#if (!defined __EXTDOCKBAR_H)
#define __EXTDOCKBAR_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
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

#if (!defined __EXT_MFC_NO_TAB_CTRL)
	#if (!defined __EXT_TABWND_H)
		#include <ExtTabWnd.h>
	#endif
#endif // (!defined __EXT_MFC_NO_TAB_CTRL)

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef _DEBUG
	#define ASSERT_BAR_LOCATION_IN_ROW( __DOCK_BAR, __BAR__ ) \
		CExtDockBar::_AssertValid_FixedBarNotInNonfixedRow( \
			__DOCK_BAR, \
			__BAR__ \
			)
#endif

#if (!defined ASSERT_BAR_LOCATION_IN_ROW)
	#define ASSERT_BAR_LOCATION_IN_ROW( __DOCK_BAR, __BAR__ )
#endif

#ifdef _DEBUG
	#define ASSERT_DOCKBAR_DLGCTRLID( __ID__ ) \
		ASSERT( \
				__ID__ == AFX_IDW_DOCKBAR_FLOAT \
			||	__ID__ == AFX_IDW_DOCKBAR_LEFT \
			||	__ID__ == AFX_IDW_DOCKBAR_TOP \
			||	__ID__ == AFX_IDW_DOCKBAR_RIGHT \
			||	__ID__ == AFX_IDW_DOCKBAR_BOTTOM \
			)
	#define ASSERT_DOCKBAR_DLGCTRLID_DOCKED( __ID__ ) \
		ASSERT( \
				__ID__ == AFX_IDW_DOCKBAR_LEFT \
			||	__ID__ == AFX_IDW_DOCKBAR_TOP \
			||	__ID__ == AFX_IDW_DOCKBAR_RIGHT \
			||	__ID__ == AFX_IDW_DOCKBAR_BOTTOM \
			)
	#define ASSERT_HWND_IS_DOCKBAR( __hWndDockBar__, __nDockBarID__, __nCircleNo__ ) \
		CExtDockBar::_Assert_HwndIsDockbar( \
			__hWndDockBar__, \
			__nDockBarID__, \
			__nCircleNo__ \
			)
	#define ASSERT_HWND_IS_AUTOHIDER( __hWndAutoHider__, __nAutoHiderID__ ) \
		CExtDockBar::_Assert_HwndIsAutoHider( \
			__hWndAutoHider__, \
			__nAutoHiderID__ \
			)

#else // _DEBUG

	#define ASSERT_DOCKBAR_DLGCTRLID( __ID__ )
	#define ASSERT_DOCKBAR_DLGCTRLID_DOCKED( __ID__ )

	#define ASSERT_HWND_IS_DOCKBAR( __hWndDockBar__, __nDockBarID__, __nCircleNo__ )
	#define ASSERT_HWND_IS_AUTOHIDER( __hWndAutoHider__, __nAutoHiderID__ )

#endif // not _DEBUG

/// #define __DEBUG_PAINTING_AREAS_DOCKBAR__
/// #define __DEBUG_PAINTING_AREAS_EXTRESIZABLEBAR__

/// #define __DEBUG_LOCKING_OPTIMIZATION_ASSERTS__

class CExtDockBar;
class CExtDockDynBar;
class CExtDynControlBar;

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	class CExtDynTabWnd;
	class CExtDynTabControlBar;
	class CExtDynAutoHideArea;
	class CExtDynAutoHideSlider;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

/////////////////////////////////////////////////////////////////////////
// CExtDockBar window

class __PROF_UIS_API CExtDockBar : public CDockBar
{
	friend class CExtBarButton;
	friend class CExtBarContentExpandButton;
	friend class CExtControlBar;
	friend class CExtControlBar::InternalDockStateBar;
	friend class CExtControlBar::InternalDockStateSite;
	friend class CExtToolControlBar;
	friend class CExtMiniDockFrameWnd;
	friend class CExtDynControlBar;
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	friend class CExtDynTabControlBar;
	friend class CExtDynAutoHideArea;
	friend class CExtDynAutoHideSlider;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	DECLARE_DYNCREATE(CExtDockBar)

protected:
	static bool g_bControlBarFixSizePixel;
	bool m_bLockedOptimize:1;
	UINT m_nCircleNo;
	CExtDockBar
		* m_pDockBarOuter,
		* m_pDockBarInner;
	CRect m_rcLastInvisiblePreCalc;
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	CExtDynAutoHideArea * m_pWndAutoHideArea;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

public:

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual void OnCustomizeModeEnter();
	virtual void OnCustomizeModeLeave();
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	bool _IsLayoutQuery()
	{
		return CDockBar::m_bLayoutQuery ? true : false;
	}
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	virtual CExtDynAutoHideArea * _GetAutoHideArea();
	const CExtDynAutoHideArea * _GetAutoHideArea() const
	{
		return ( ((CExtDockBar *)this)->_GetAutoHideArea() );
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	CExtDockBar(
		UINT nCircleNo = 0
		);

	UINT _GetCircleNo() const
	{
		ASSERT( this != NULL );
		return m_nCircleNo;
	}

	CExtDockBar * _GetDockBarInner()
	{
		ASSERT( this != NULL );
		return m_pDockBarInner;
	}
	const CExtDockBar * _GetDockBarInner() const
	{
		ASSERT( this != NULL );
		return m_pDockBarInner;
	}
	CExtDockBar * _GetDockBarOuter()
	{
		ASSERT( this != NULL );
		return m_pDockBarOuter;
	}
	const CExtDockBar * _GetDockBarOuter() const
	{
		ASSERT( this != NULL );
		return m_pDockBarOuter;
	}

	CExtDockBar * _GetDockBarInnerTop()
	{
		ASSERT( this != NULL );
		if( m_pDockBarInner == NULL )
			return this;
		CExtDockBar * pBar = m_pDockBarInner;
		while( pBar->m_pDockBarInner != NULL )
			pBar = pBar->m_pDockBarInner;
		return pBar;
	}
	const CExtDockBar * _GetDockBarInnerTop() const
	{
		ASSERT( this != NULL );
		if( m_pDockBarInner == NULL )
			return this;
		const CExtDockBar * pBar = m_pDockBarInner;
		while( pBar->m_pDockBarInner != NULL )
			pBar = pBar->m_pDockBarInner;
		return pBar;
	}
	CExtDockBar * _GetDockBarOuterTop()
	{
		ASSERT( this != NULL );
		if( m_pDockBarOuter == NULL )
			return this;
		CExtDockBar * pBar = m_pDockBarOuter;
		while( pBar->m_pDockBarOuter != NULL )
			pBar = pBar->m_pDockBarOuter;
		return pBar;
	}
	const CExtDockBar * _GetDockBarOuterTop() const
	{
		ASSERT( this != NULL );
		if( m_pDockBarOuter == NULL )
			return this;
		const CExtDockBar * pBar = m_pDockBarOuter;
		while( pBar->m_pDockBarOuter != NULL )
			pBar = pBar->m_pDockBarOuter;
		return pBar;
	}

	CExtDockBar * _GetBarByCircleNo( UINT nCircleNo );

	const CRect & _GetLastInvisiblePreCalcRect() const
	{
		ASSERT_VALID( this );
		return m_rcLastInvisiblePreCalc;
	}
	void _GetPreCalcWindowRect( CRect & rcPreCalc )
	{
		ASSERT_VALID( this );
//		bool bDockBarIsVisible =
//			(	(GetStyle() & WS_VISIBLE) != 0
//				&&
//				GetDockedVisibleCount() > 0
//			) ? true : false;
//		if( bDockBarIsVisible )
//		{
//			ASSERT( GetSafeHwnd() != NULL );
//			ASSERT( ::IsWindow(GetSafeHwnd()) );
//			GetWindowRect( &rcPreCalc );
//		}
//		else
			rcPreCalc = _GetLastInvisiblePreCalcRect();
	}

	CExtDockBar * _GetInCircle(
		UINT nDockBarID,
		bool bEnableCreateNew = true
		);
	static CExtDockBar * _GetInCircle(
		CFrameWnd * pFrame,
		UINT nCircleNo,
		UINT nDockBarID,
		bool bEnableCreateNew = true
		);
	UINT _CreateInnerCircle();
	static UINT _CreateInnerCircle( CFrameWnd * pFrame );
	void _InjectInnerCircle();
	void _InjectCircle( UINT nCircleNo );
	static void _InjectCircle( CFrameWnd * pFrame, UINT nCircleNo );
//	HWND _GetHwndForInjectingAfter();

	static bool g_bExtendedRepositioning;

	CControlBar * _GetDockedControlBar( int nPos ) const
	{
		ASSERT_VALID( this );
		return CDockBar::GetDockedControlBar( nPos );
	}

	void RemoveAllPlaceHolders(
		bool bSearchPlaceHolder = true
		);
	void RemovePlaceHolder(
		CControlBar * pBar,
		bool bSearchPlaceHolder = true,
		bool * p_bRemoved = NULL
		);
	static void _RemovePlaceHolder(
		CFrameWnd * pFrame,
		CControlBar * pBar,
		bool * p_bRemoved = NULL
		);

	void _SlideDockControlBar(
		CControlBar* pBar,
		LPCRECT lpRect,
		BOOL bMovingEnabled,
		const POINT * ptDesiredMid = NULL
		);
	void _NewRowDockControlBar(
		CControlBar * pExtBarDocked,
		CControlBar * pExtBarNew,
		bool bLessIndex
		);
	void _InnerOuterDockControlBar(
		CControlBar * pBar,
		bool bInner
		);

	virtual void _LockSequenceOptimization( bool bLock );

	void DockControlBar(
		CControlBar * pBar,
		LPCRECT lpRect = NULL,
		bool bSmoothMode = false
		);

	void _OptimizeCircles();
	static void _OptimizeCircles( CFrameWnd * pFrame );
	bool _CanBeSafeOptimized();
	void _SafeOptimizeInnerOuterChain();

    CFrameWnd * _GetDockingFrameImpl();

	BOOL RemoveControlBar(
		CControlBar * pBar,
		int nPosExclude = -1,
		int nAddPlaceHolder = 0,
		bool bEnableFrameDelayRecalcLayout = true
		);
	
	static bool _ConstructDockSiteControlBarPopupMenu(
		CFrameWnd * pFrame,
		CExtPopupMenuWnd * pPopup,
		SHORT nHelperNotificationType,
		CControlBar * pBarEventSrc,
		UINT nMsgID = WM_NULL,
		CObject * pObjectSrc = NULL
		);
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	static bool _AppendAddRemoveButtonsCustomizeMenu(
		CExtPopupMenuWnd * pPopup,
		CControlBar * pBarEventSrc
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

protected:
	int _InsertByPointImpl(
		CControlBar * pBar,
		CRect rect,
		CPoint ptMid
		);
	int Insert(
		CControlBar * pBar,
		CRect rect,
		CPoint ptMid
		);

public:

#ifdef _DEBUG
	
	void _AssertValid_CheckBarRows() const;
	static void _AssertValid_FixedBarNotInNonfixedRow(
		CDockBar * pDockBar,
		CControlBar * pBar
		);

	static void _Assert_HwndIsDockbar(
		HWND hWndDockBar,
		UINT nDockBarID,
		UINT nCircleNo
		);
	static void _Assert_HwndIsDockbar(
		CWnd * pWnd,
		UINT nDockBarID,
		UINT nCircleNo
		)
	{
		HWND hWndDockBar = pWnd->GetSafeHwnd();
		_Assert_HwndIsDockbar(
			hWndDockBar,
			nDockBarID,
			nCircleNo
			);
	}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	static void _Assert_HwndIsAutoHider(
		HWND hWndAutoHider,
		UINT nAutoHiderID
		);
	static void _Assert_HwndIsAutoHider(
		CWnd * pWndAutoHider,
		UINT nAutoHiderID
		)
	{
		HWND hWndAutoHider = pWndAutoHider->GetSafeHwnd();
		_Assert_HwndIsAutoHider(
			hWndAutoHider,
			nAutoHiderID
			);
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	
	virtual void AssertValid() const
	{
		ASSERT( this != NULL );
		_AssertValid_CheckBarRows();

		CDockBar::AssertValid();

		if( m_pDockBarInner != NULL )
		{
			ASSERT_KINDOF( CExtDockBar, m_pDockBarInner );
			ASSERT( m_pDockBarInner->m_pDockBarOuter == this );
			ASSERT( m_pDockBarInner->m_nCircleNo == (m_nCircleNo+1) );
		}
		if( m_pDockBarOuter != NULL )
		{
			ASSERT_KINDOF( CExtDockBar, m_pDockBarOuter );
			ASSERT( m_pDockBarOuter->m_pDockBarInner == this );
			ASSERT( m_pDockBarOuter->m_nCircleNo == (m_nCircleNo-1) );
		}
	
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CDockBar::Dump( dc );
	}

#endif

protected:
	bool m_bInDynamicLayoutUpdate:1;
	static bool g_bDockBarClassRegistered;
	static bool RegisterDockBarClass();

	void _HandleDestruction();

public:

	virtual BOOL DestroyWindow();

	virtual void OnDynamicLayoutUpdate();
	virtual void OnDynamicLayoutOptimize();

		// this is the one and only method of interest
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual void DoPaint(CDC* pDC);

	void DrawBorders(CDC* pDC, CRect& rect);
	void DrawGripper(CDC* pDC, const CRect& rect);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDockBar)
	//}}AFX_VIRTUAL

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	//{{AFX_MSG(CExtDockBar)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtDockBar

#define __EXT_DOCKBAR_CLASS_NAME _T("ProfUIS-DockBar")

/////////////////////////////////////////////////////////////////////////
// CExtDockDynBar window

class __PROF_UIS_API CExtDockDynBar : public CExtDockBar
{
public:
	struct __PROF_UIS_API VisibleLayoutItem_t
	{
		CArray < CExtControlBar *, CExtControlBar * & > m_vRow;
		LONG m_nRowMetric;
		LONG m_nRowMinMetric;
		LONG m_nRowExtent;
		LONG m_nRowMinExtent;

		void _AssignFromOther(
			const VisibleLayoutItem_t & other
			);

		VisibleLayoutItem_t();
		
		VisibleLayoutItem_t(
			const VisibleLayoutItem_t & other
			);
		
		~VisibleLayoutItem_t();
		
		VisibleLayoutItem_t & operator=(
			const VisibleLayoutItem_t & other
			);

		bool IsEmpty() const;
		bool IsRowMinSized() const;
	}; // struct VisibleLayoutItem_t

	struct __PROF_UIS_API VisibleLayout_t
	{
		CArray < VisibleLayoutItem_t *, VisibleLayoutItem_t * & > m_vRows;
		LONG m_nTotalMetric;
		LONG m_nTotalMinMetric;
		LONG m_nTotalMinExtent;

		LONG m_nTotalBarsCount;
		
		void _Clean();

		VisibleLayout_t();
		~VisibleLayout_t();

		void AddBarPointer( CExtControlBar * pBar );
		void MakeNewRow();

		bool IsEmpty() const;
	}; // struct VisibleLayout_t
	
	void _VisibleLayoutBuild(
		VisibleLayout_t & _vl
		);
	void _VisibleLayoutAlign(
		VisibleLayout_t & _vl,
		CSize _size
		);

public:

	CExtDockDynBar( UINT nCircleNo = 0 );
	
	DECLARE_DYNCREATE(CExtDockDynBar)

	virtual void OnDynamicLayoutUpdate();
	virtual void OnDynamicLayoutOptimize();

	// this is the one and only method of interest
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

	virtual void CalcOrderedVector(
		ExtControlBarVector_t & vBars
		);

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	virtual CExtDynAutoHideArea * _GetAutoHideArea();
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

private:
	bool m_bHelperDockSiteModified:1;


public:

	virtual void _LockSequenceOptimization( bool bLock );

#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CExtDockBar::AssertValid();

		ASSERT( m_bAutoDelete );
		
		if( m_bHelperDockSiteModified
			&& m_pDockSite != NULL
			)
		{
			// must be unregistered
			POSITION pos = m_pDockSite->m_listControlBars.Find( (void *)this );
			ASSERT( pos == NULL );
		}
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtDockBar::Dump( dc );
	}

#endif

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDockDynBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDockDynBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CExtControlBar::InternalDockStateBar;
}; // class CExtDockDynBar

/////////////////////////////////////////////////////////////////////////
// CExtDynControlBar window

class __PROF_UIS_API CExtDynControlBar : public CExtControlBar
{
public:

	CExtDockDynBar * m_pWndDynDocker;

	CExtDynControlBar();
	
	DECLARE_DYNCREATE(CExtDynControlBar)

#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CExtControlBar::AssertValid();

		ASSERT( m_bAutoDelete );

		if( m_pWndDynDocker != NULL
			&& m_pDockSite != NULL
			)
		{
			ASSERT_VALID( m_pWndDynDocker );
		}
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtControlBar::Dump( dc );
	}

#endif

public:
	virtual bool IsBarWithGripper(
		bool * pbGripperAtTop = NULL,
		bool * pbTextOnGripper = NULL
		) const;

protected:
	virtual bool _CanDockToTabbedContainers(
		CExtControlBar * pDestBar
		) const;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	virtual CExtDynTabControlBar * _GetNearestTabbedContainer();
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

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

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDynControlBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDynControlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtDynControlBar

/////////////////////////////////////////////////////////////////////////
// CExtDockOuterBar window

class __PROF_UIS_API CExtDockOuterBar : public CExtDockBar
{
public:
	struct __PROF_UIS_API OuterLayoutItemData
	{
		void _AssignFromOther(
			const OuterLayoutItemData & other
			);
	
	public:
		
		CControlBar * m_pBar;
		bool m_bVisible:1;
		CSize m_sizeCalcDynamic, m_sizeBarMin;
		CRect m_rcPreCalc, m_rcReal;
		
		OuterLayoutItemData();
		OuterLayoutItemData(
			CControlBar * pBar,
			BOOL bHorz,
			const SIZE & sizeMax
			);
		OuterLayoutItemData(
			const OuterLayoutItemData & other
			)
		{
			_AssignFromOther( other );
		}
		OuterLayoutItemData & operator=(
			const OuterLayoutItemData & other
			)
		{
			_AssignFromOther( other );
			return *this;
		}
	}; // struct OuterLayoutItemData

	typedef
		CArray < OuterLayoutItemData, OuterLayoutItemData >
		dockbar_outer_layout_t;

	void _RedockInRow(
		CExtControlBar * pBarSlide,
		int nShiftDesired,
		bool * p_bInplaceResizing = NULL
		);
	bool _ReDockToNewRow(
		CControlBar * pBarSlide,
		MfcControlBarVector_t & vRow,
		int nShiftDesired
		);

private:
	void _SwapInRowWithAffixment(
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		int nIdxLess,
		int nIdxGreater
		);
	bool _CompressSubRow(
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		int nSubRowStartPos,
		int nSubRowEndPos,
		int nExtentMax,
		bool bHorz,
		bool & bAdjustWithAffixment,
		bool * p_bFullyOccupiedLayout = NULL
		);
	void _CalcFixedRowLayout(
		CSize & sizeFixed,
		CSize & sizeMax,
		CPoint & pt,
		BOOL bHorz,
		AFX_SIZEPARENTPARAMS & layout,
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		int * p_nSubRowCount = NULL,
		bool * p_bFullyOccupiedLayout = NULL
		);
	void _AdjustByTotalRowExtent(
		int & nTotalRowExtent,
		CSize & sizeFixed,
		CPoint & pt,
		BOOL bHorz
		);
	void _PreAdjustWithAffixment(
		bool bHorz,
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		CExtDockOuterBar::OuterLayoutItemData & olidAdjustedSrc,
		int nBarAdjustedSrc,
		CExtControlBar::InternalAffixmentData * pAffixmentDataAdjustedSrc,
		int nExtentMax
		);
	void _PostAdjustWithAffixment(
		bool bHorz,
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		CExtDockOuterBar::OuterLayoutItemData & olidAdjustedSrc,
		int nBarAdjustedSrc,
		CExtControlBar::InternalAffixmentData * pAffixmentDataAdjustedSrc,
		int nExtentMax
		);
	int _ShiftSubRowWithAffixment(
		bool bHorz,
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		int nReviewStart,
		int nReviewEnd,
		int nAdjustShift,
		int nExtentMax
		);
	void _AlignSubRowAffixmentHW(
		bool bHorz,
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		int nSubRowStartPos,
		int nSubRowEndPos
		);

	typedef
		CArray < int, int >
		AffixmentOrderVector_t;
	void _CalcAffixmentOrderVector(
		bool bHorz,
		AffixmentOrderVector_t & arrAffixmentOrder,
		CExtDockOuterBar::dockbar_outer_layout_t & dbol,
		bool bResetAffixmentDataIfNeeded = false,
		int nSubRowStartPos = 0,
		int nSubRowEndPos = -1
		);
public:
	CExtDockOuterBar();
	
	DECLARE_DYNCREATE(CExtDockOuterBar)

	virtual void OnDynamicLayoutUpdate();
	virtual void OnDynamicLayoutOptimize();

	// this is the one and only method of interest
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

	void _AffixmentBringToTop( CExtControlBar * pBar );

	void _DelayDockSiteRecalc();
protected:
	bool m_bInDockSiteDelayedRecalc:1;
	bool m_bDockSiteRecalcIsDelayed:1;
	void _DelayDockSiteRecalc_Start();
	bool _DelayDockSiteRecalc_Update();

public:
#ifdef _DEBUG
	
	virtual void AssertValid() const
	{
		CExtDockBar::AssertValid();
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CExtDockBar::Dump( dc );
	}

#endif

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDockOuterBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExtDockOuterBar)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	friend class CExtControlBar::InternalDockStateBar;
	friend class CExtControlBar::InternalDraggingState_t;
}; // class CExtDockOuterBar


#define __PLACEHODLER_BAR_PTR( __CB__ ) \
	( (HIWORD(__CB__) ) == 0 )

#endif // __EXTDOCKBAR_H
