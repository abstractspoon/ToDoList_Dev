// ToDoCtrlViewTabControl.cpp : implementation file
//

#include "stdafx.h"
#include "TDlViewTabControl.h"
#include "tdcmsg.h"

#include "..\shared\deferwndmove.h"
#include "..\shared\misc.h"
#include "..\shared\holdredraw.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDCViewTabControl

CTDLViewTabControl::CTDLViewTabControl() 
: 
	CTabCtrlEx(TCE_MBUTTONCLOSE | TCE_CLOSEBUTTON | TCE_BOLDSELTEXT | TCE_DRAGDROP, e_tabBottom), 
	m_nSelTab(-1),
	m_bShowingTabs(TRUE),
	m_rOverall(0, 0, 0, 0)
{
}

CTDLViewTabControl::~CTDLViewTabControl()
{
	// cleanup view data
	int nIndex = m_aViews.GetSize();

	while (nIndex--)
	{
		delete m_aViews[nIndex].pVData;
		::DestroyIcon(m_aViews[nIndex].hIcon);
	}
}


BEGIN_MESSAGE_MAP(CTDLViewTabControl, CTabCtrlEx)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
	ON_NOTIFY_REFLECT(TCN_CLOSETAB, OnCloseTab)
	ON_NOTIFY_REFLECT(TCN_ENDDRAG, OnEndDrag)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CTDLViewTabControl::AttachView(HWND hWnd, FTC_VIEW nView, LPCTSTR szLabel, HICON hIcon, IVIEWTABDATA* pData, int nVertOffset)
{
	ASSERT (hWnd == NULL || ::IsWindow(hWnd));

	// window and enum must be unique
	if ((hWnd && GetViewIndex(hWnd) != -1) || (GetViewIndex(nView) != -1))
		return FALSE;

	// prepare tab bar
	if (!hIcon)
		hIcon = AfxGetApp()->LoadStandardIcon(IDI_WARNING);

	ASSERT(hIcon);

	TDCVIEW view(hWnd, nView, szLabel, hIcon, pData, nVertOffset);
	int nIndex = m_aViews.Add(view);

	if (GetSafeHwnd())
	{
		VERIFY(InsertItem(TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE, nIndex, view.sViewLabel, nIndex, (LPARAM)nView) >= 0);
		UpdateTabItemWidths(); 
	}

	return TRUE;
}

BOOL CTDLViewTabControl::DetachView(HWND hWnd)
{
	int nFind = GetViewIndex(hWnd);

	if (nFind != -1)
	{
		m_aViews.RemoveAt(nFind);
		return TRUE;
	}

	return FALSE;
}

BOOL CTDLViewTabControl::DetachView(FTC_VIEW nView)
{
	int nFind = GetViewIndex(nView);

	if (nFind != -1)
	{
		m_aViews.RemoveAt(nFind);
		return TRUE;
	}

	return FALSE;
}

void CTDLViewTabControl::GetViewRect(const TDCVIEW& view, CRect& rView) const
{
	CRect rUnused;
	CalcTabViewRects(m_rOverall, rUnused, rView);

	rView.top += view.nVertOffset;
}

BOOL CTDLViewTabControl::SwitchToTab(int nTab)
{
	ASSERT (GetSafeHwnd());

	if (GetSafeHwnd() == NULL)
		return FALSE;

	if ((nTab == m_nSelTab) || (nTab < 0))
		return FALSE;

	int nIndex = TabToViewIndex(nTab);
	
	if (nIndex == -1)
		return FALSE;

	// make sure we have a valid HWND to switch to
	const TDCVIEW& viewNew = m_aViews[nIndex];
	HWND hwndNew = viewNew.hwndView, hwndOld = NULL;

	if (!hwndNew || !::IsWindow(hwndNew))
	{
		ASSERT(0);
		return FALSE;
	}

	if (m_nSelTab == -1)
	{
		// first time so hide all other views
		int nOther = m_aViews.GetSize();

		while (nOther--)
		{
			if (nOther != nIndex)
				::ShowWindow(m_aViews[nOther].hwndView, SW_HIDE);
		}
	}
	else // just hide the currently visible view
	{
		int nOldIndex = TabToViewIndex(m_nSelTab);
		hwndOld = m_aViews[nOldIndex].hwndView;
	}

	CRect rView;
	GetViewRect(viewNew, rView);

	if (!rView.IsRectEmpty())
		::MoveWindow(hwndNew, rView.left, rView.top, rView.Width(), rView.Height(), FALSE);

	{
		CHoldRedraw hr(hwndNew, NCR_ALL);

		if (hwndOld)
			::ShowWindow(hwndOld, SW_HIDE);
		
		::ShowWindow(hwndNew, SW_SHOW);
	}
		
	m_nSelTab = nTab;
	SetCurSel(nTab);
	UpdateTabItemWidths();

	return TRUE;
}

CWnd* CTDLViewTabControl::GetActiveWnd() const
{
	ASSERT (GetSafeHwnd());

	if (GetSafeHwnd() == NULL)
		return NULL;

	int nIndex = TabToViewIndex(m_nSelTab);

	return (nIndex == -1) ? NULL : CWnd::FromHandle(m_aViews[nIndex].hwndView);
}

FTC_VIEW CTDLViewTabControl::GetActiveView() const
{
	return ((m_nSelTab == -1) ? FTCV_UNSET : GetTabView(m_nSelTab));
}

HWND CTDLViewTabControl::GetViewHwnd(FTC_VIEW nView) const
{
	int nIndex = GetViewIndex(nView);

	if (nIndex < 0 || nIndex > m_aViews.GetSize())
		return NULL;

	return m_aViews[nIndex].hwndView;
}

CString CTDLViewTabControl::GetViewName(FTC_VIEW nView) const
{
	int nIndex = GetViewIndex(nView);

	if (nIndex < 0 || nIndex > m_aViews.GetSize())
		return _T("");

	CString sViewName(m_aViews[nIndex].sViewLabel);

	return Misc::Trim(sViewName);
}

BOOL CTDLViewTabControl::SetViewHwnd(FTC_VIEW nView, HWND hWnd)
{
	int nIndex = GetViewIndex(nView);

	if (nIndex < 0 || nIndex > m_aViews.GetSize())
		return NULL;

	ASSERT (m_aViews[nIndex].hwndView == NULL);

	if (m_aViews[nIndex].hwndView)
		return FALSE;

	m_aViews[nIndex].hwndView = hWnd;
	return TRUE;
}

IVIEWTABDATA* CTDLViewTabControl::GetViewData(FTC_VIEW nView) const
{
	int nIndex = GetViewIndex(nView);

	return (nIndex == -1) ? NULL : m_aViews[nIndex].pVData;
}

int CTDLViewTabControl::TabToViewIndex(int nTab) const
{
	FTC_VIEW nView = GetTabView(nTab);
	ASSERT(nView != FTCV_UNSET);

	int nIndex = GetViewIndex(nView);
	ASSERT(nIndex >= 0 && nIndex < m_aViews.GetSize());

	return nIndex;
}

FTC_VIEW CTDLViewTabControl::GetTabView(int nTab) const
{
	return (FTC_VIEW)GetItemData(nTab);
}

BOOL CTDLViewTabControl::SetActiveView(CWnd* pWnd, BOOL bNotify)
{
	int nNewIndex = GetViewIndex(pWnd->GetSafeHwnd());
	ASSERT(nNewIndex != -1);

	int nNewTab = GetTabIndex(m_aViews[nNewIndex].nView);
	int nOldTab = m_nSelTab;

	return DoTabChange(nOldTab, nNewTab, bNotify);
}

BOOL CTDLViewTabControl::SetActiveView(FTC_VIEW nView, BOOL bNotify)
{
	int nNewTab = GetTabIndex(nView);
	int nOldTab = m_nSelTab;

	return DoTabChange(nOldTab, nNewTab, bNotify);
}

void CTDLViewTabControl::Resize(const CRect& rect, CDeferWndMove* pDWM)
{
	m_rOverall = rect;

	CRect rTabs, rView;

	if (CalcTabViewRects(rect, rTabs, rView))
	{
		int nActive = TabToViewIndex(m_nSelTab);

		CWnd* pView = ((nActive == -1) ? NULL : CWnd::FromHandle(m_aViews[nActive].hwndView));
		ASSERT(pView);

		if (pView)
		{
			rView.top += m_aViews[nActive].nVertOffset;

			if (pDWM)
			{
				pDWM->MoveWindow(this, rTabs);
				pDWM->MoveWindow(pView, rView/*, FALSE*/);
			}
			else
			{
				MoveWindow(rTabs);
				pView->MoveWindow(rView/*, FALSE*/);
			}
		}
	}
}

int CTDLViewTabControl::GetViewIndex(HWND hWnd) const
{
	int nIndex = m_aViews.GetSize();

	while (nIndex--)
	{
		if (m_aViews[nIndex].hwndView == hWnd)
			return nIndex;
	}

	// else
	return -1;
}

int CTDLViewTabControl::GetViewIndex(FTC_VIEW nView) const
{
	int nIndex = m_aViews.GetSize();

	while (nIndex--)
	{
		if (m_aViews[nIndex].nView == nView)
			return nIndex;
	}

	// else
	return -1;
}

int CTDLViewTabControl::GetTabIndex(FTC_VIEW nView) const
{
	// find tab with view as its item data
	return FindItemByData(nView);
}

void CTDLViewTabControl::PreSubclassWindow() 
{
	CXPTabCtrl::PreSubclassWindow();

	ShowTabControl(m_bShowingTabs);

	int nImageSize = GraphicsMisc::ScaleByDPIFactor(16);

	if (m_ilTabs.Create(nImageSize, nImageSize, ILC_COLOR32 | ILC_MASK, 0, 1))
		SetImageList(&m_ilTabs); // Populated on demand

	// add tabs
	int nNumView = m_aViews.GetSize();

	for (int nView = 0; nView < nNumView; nView++)
	{
		const TDCVIEW& view = m_aViews[nView];
		InsertItem(TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE, nView, view.sViewLabel, nView, (LPARAM)view.nView);
	}

	if (nNumView)
	{
		SwitchToTab(0);
		UpdateTabItemWidths();
	}
}

void CTDLViewTabControl::ShowTabControl(BOOL bShow) 
{ 
	m_bShowingTabs = bShow; 

	if (GetSafeHwnd())
	{
		ShowWindow(bShow ? SW_SHOW : SW_HIDE);
		EnableWindow(bShow);

		if (bShow)
			EnsureSelVisible();
	}
}

BOOL CTDLViewTabControl::CalcTabViewRects(const CRect& rPos, CRect& rTabs, CRect& rView) const
{
	if (!GetSafeHwnd())
		return FALSE;

	// Always recalculate the tab rect so that we maintain
	// its position for when we switch between invisible and visible.
	// We just don't adjust the view rect if we are invisible.
	rTabs = rPos;
	rTabs.bottom = rTabs.top;

	const_cast<CTDLViewTabControl*>(this)->AdjustRect(TRUE, rTabs);

	int nTabHeight = rTabs.Height();
	rTabs = rView = rPos;

	switch (GetOrientation())
	{
	case e_tabTop:
		rTabs.bottom = rTabs.top + nTabHeight - 7;

		if (m_bShowingTabs)
			rView.top = rTabs.bottom;
		break;

	case e_tabBottom:
		rTabs.top = rTabs.bottom - nTabHeight + 7;

		if (m_bShowingTabs)
			rView.bottom = rTabs.top;
		break;

	case e_tabLeft:
		rTabs.right = rTabs.left + nTabHeight;

		if (m_bShowingTabs)
			rView.left = rTabs.right;
		break;

	case e_tabRight:
		rTabs.left = rTabs.right - nTabHeight;

		if (m_bShowingTabs)
			rView.right = rTabs.left;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

void CTDLViewTabControl::OnSelChange(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	int nOldTab = max(m_nSelTab, 0);
	int nNewTab = GetCurSel();

	if (DoTabChange(nOldTab, nNewTab, TRUE) == FALSE)
		SetCurSel(nOldTab);

	*pResult = 0;
}

void CTDLViewTabControl::ActivateNextView(BOOL bNext)
{
	int nCurTab = GetTabIndex(GetActiveView());
	int nOldTab = nCurTab, nNewTab = nCurTab;

	// keep iterating until we find a valid view or until
	// we return to the start
	do
	{
		if (bNext)
		{
			nNewTab++;

			if (nNewTab == GetItemCount())
				nNewTab = 0;
		}
		else
		{
			nNewTab--;

			if (nNewTab == -1)
				nNewTab = (GetItemCount() - 1);
		}
		ASSERT (nNewTab >= 0 && nNewTab < GetItemCount());
	}
	while (nNewTab != nOldTab && DoTabChange(nCurTab, nNewTab, TRUE) == FALSE);
}

BOOL CTDLViewTabControl::DoTabChange(int nOldTab, int nNewTab, BOOL bNotify)
{
	ASSERT (nOldTab != nNewTab);

	if (nOldTab == nNewTab)
		return FALSE;

	// Special case: Old tab is unset and new tab is tree
	if ((nOldTab == -1) && (GetTabView(nNewTab) == FTCV_TASKTREE))
	{
		m_nSelTab = nNewTab;
		SetCurSel(nNewTab);

		return TRUE;
	}

	// check if the previous view had the focus
	int nOldIndex = TabToViewIndex(nOldTab);

	HWND hOldView = (nOldIndex != -1) ? m_aViews[nOldIndex].hwndView : NULL;
	BOOL bHadFocus = TRUE;

	if (hOldView)
	{
		HWND hFocus = ::GetFocus();
		bHadFocus = (hFocus == NULL) || CDialogHelper::IsChildOrSame(hOldView, hFocus);
	}

	// notify parent before the change
	if (bNotify)
	{
		BOOL bCancel = GetParent()->SendMessage(WM_TDCN_VIEWPRECHANGE, GetTabView(nOldTab), GetTabView(nNewTab));

		if (bCancel)
		{
			SetCurSel(nOldTab);
			return FALSE;
		}
	}

	if (SwitchToTab(nNewTab))
	{
		ASSERT (nOldTab != m_nSelTab);

		// notify parent after the change
		if (bNotify)
			GetParent()->SendMessage(WM_TDCN_VIEWPOSTCHANGE, GetTabView(nOldTab), GetTabView(nNewTab));

		// restore focus		
		int nNewIndex = TabToViewIndex(nNewTab);
		HWND hNewView = m_aViews[nNewIndex].hwndView;

		if (bHadFocus)
			::SetFocus(hNewView);
	}

	return TRUE;
}

void CTDLViewTabControl::OnCloseTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	
	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;
	
	// check valid tab
	if (pNMTCE->iTab >= 0)
	{
		FTC_VIEW nView = GetTabView(pNMTCE->iTab);

		if (nView != FTCV_TASKTREE)
			ShowViewTab(nView, FALSE);
	}
}

void CTDLViewTabControl::OnEndDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	
	NMTABCTRLEX* pNMTCE = (NMTABCTRLEX*)pNMHDR;

	int nFrom = pNMTCE->iTab, nTo = (int)pNMTCE->dwExtra;
	ASSERT((nFrom >= 0) && (nTo >= 0));

	MoveTab(nFrom, nTo);
	m_nSelTab = GetCurSel();
}

BOOL CTDLViewTabControl::WantTabCloseButton(int nTab) const
{
	return (GetTabView(nTab) != FTCV_TASKTREE);
}

BOOL CTDLViewTabControl::IsViewTabShowing(FTC_VIEW nView) const
{
	return (GetTabIndex(nView) != -1);
}

BOOL CTDLViewTabControl::ShowViewTab(FTC_VIEW nView, BOOL bShow)
{
	ASSERT(GetSafeHwnd());
	ASSERT(nView != FTCV_TASKTREE);

	// find index of TDCVIEW item
	int nItem = GetViewIndex(nView);

	if (nItem == -1)
		return FALSE; // item must exist

	// find tab with that as its item data
	int nTab = GetTabIndex(nView);

	if (!bShow)
	{
		if (nTab == -1)
			return TRUE; // already hidden

		// removing currently selected tab need care
		if (m_nSelTab == nTab)
		{
			ASSERT(nTab > 0);

			// always switch to tree because it will never be hidden
			if (!SwitchToTab(0))
				return FALSE;

			// else
			return DeleteItem(nTab);
		}
		else if (DeleteItem(nTab)) // remove tab
		{
			// fixup selection
			if (m_nSelTab > nTab)
				m_nSelTab--;
				
			return TRUE;
		}

		// else
		return FALSE;
	}

	// else show tab
	if (nTab != -1)
		return TRUE; // already showing

	// else add tab
	const TDCVIEW& view = m_aViews[nItem];

	// find the index to insert at by working back thru the 
	// items until we find a visible tab
	int nPrevItem = nItem;
	int nInsert = -1;

	while (nPrevItem--)
	{
		nInsert = GetTabIndex(m_aViews[nPrevItem].nView);

		if (nInsert != -1)
		{
			nInsert++; // insert after
			break;
		}
	}
	ASSERT(nInsert > 0);

	nTab = InsertItem(TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE, nInsert, view.sViewLabel, (view.hIcon ? nItem : -1), (LPARAM)nView);
	ASSERT(nTab > 0);

	// fixup selection
	if ((nTab > 0) && (m_nSelTab >= nTab))
	{
		m_nSelTab++;
	}

	return (nTab > 0);
}

BOOL CTDLViewTabControl::OnEraseBkgnd(CDC* pDC)
{
	// Delayed one-time imagelist initialisation
	int nNumViews = m_aViews.GetSize();

	if (nNumViews && !m_ilTabs.GetImageCount())
	{
		for (int nView = 0; nView < nNumViews; nView++)
		{
			m_ilTabs.Add(m_aViews[nView].hIcon);
		}
	}

	return CTabCtrlEx::OnEraseBkgnd(pDC);
}

int CTDLViewTabControl::GetViewOrder(CTDCViewArray& aViewOrder) const
{
	aViewOrder.RemoveAll();

	int nTab = GetItemCount();

	while (nTab--)
		aViewOrder.InsertAt(0, GetTabView(nTab));

	return aViewOrder.GetSize();
}

void CTDLViewTabControl::SetViewOrder(const CTDCViewArray& aViewOrder)
{
	for (int nNewPos = 0; nNewPos < aViewOrder.GetSize(); nNewPos++)
	{
		FTC_VIEW nView = aViewOrder[nNewPos];
		int nOldPos = GetTabIndex(nView);

		MoveTab(nOldPos, nNewPos);
	}

	m_nSelTab = GetCurSel();
	EnsureSelVisible();
}
