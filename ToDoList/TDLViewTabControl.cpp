// ToDoCtrlViewTabControl.cpp : implementation file
//

#include "stdafx.h"
#include "TDlViewTabControl.h"
#include "tdcmsg.h"

#include "..\shared\deferwndmove.h"
#include "..\shared\misc.h"
#include "..\shared\holdredraw.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDCViewTabControl

CTDLViewTabControl::CTDLViewTabControl(DWORD dwStyles) 
: 
	CTabCtrlEx(TCE_MBUTTONCLOSE | TCE_CLOSEBUTTON | TCE_BOLDSELTEXT, e_tabBottom), 
	m_nSelTab(-1),
	m_dwStyles(dwStyles),
	m_bShowingTabs(TRUE)
{
}

CTDLViewTabControl::~CTDLViewTabControl()
{
	// cleanup view data
	int nIndex = m_aViews.GetSize();

	while (nIndex--)
	{
		delete m_aViews[nIndex].pData;
		m_aViews[nIndex].pData = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTDLViewTabControl, CTabCtrlEx)
	//{{AFX_MSG_MAP(CTDCViewTabControl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
	ON_NOTIFY_REFLECT(TCN_CLOSETAB, OnCloseTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDCViewTabControl message handlers

BOOL CTDLViewTabControl::AttachView(HWND hWnd, FTC_VIEW nView, LPCTSTR szLabel, HICON hIcon, void* pData)
{
	ASSERT (hWnd == NULL || ::IsWindow(hWnd));

	// window and enum must be unique
	if ((hWnd && FindView(hWnd) != -1) || (FindView(nView) != -1))
		return FALSE;

	// prepare tab bar
	int nImage = -1;

	if (hIcon)
	{
		if (!m_ilTabs.GetSafeHandle())
		{
			if (m_ilTabs.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 1))
				SetImageList(&m_ilTabs);
		}

		if (m_ilTabs.GetSafeHandle())
			nImage = m_ilTabs.Add(hIcon);
		else
			hIcon = NULL;
	}

	TDCVIEW view(hWnd, nView, szLabel, nImage, pData);
	int nIndex = m_aViews.Add(view);

	if (GetSafeHwnd())
	{
		int nTab = InsertItem(TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE, nIndex, view.sViewLabel, nImage, (LPARAM)nView);
		ASSERT(nTab >= 0);
		
		UpdateTabItemWidths(); 
	}

	return TRUE;
}

BOOL CTDLViewTabControl::DetachView(HWND hWnd)
{
	int nFind = FindView(hWnd);

	if (nFind != -1)
	{
		m_aViews.RemoveAt(nFind);
		return TRUE;
	}

	return FALSE;
}

BOOL CTDLViewTabControl::DetachView(FTC_VIEW nView)
{
	int nFind = FindView(nView);

	if (nFind != -1)
	{
		m_aViews.RemoveAt(nFind);
		return TRUE;
	}

	return FALSE;
}

void CTDLViewTabControl::GetViewRect(TDCVIEW& view, CRect& rPos) const
{
	CWnd* pWndView = GetViewWnd(view);

	pWndView->GetWindowRect(rPos);
	pWndView->GetParent()->ScreenToClient(rPos);
}

CWnd* CTDLViewTabControl::GetViewWnd(TDCVIEW& view) const
{
	return CWnd::FromHandle(view.hwndView);
}

BOOL CTDLViewTabControl::SwitchToTab(int nTab)
{
	ASSERT (GetSafeHwnd());

	if (GetSafeHwnd() == NULL)
		return FALSE;

	if ((nTab == m_nSelTab) || (nTab < 0))
		return FALSE;

	int nIndex = TabToIndex(nTab);
	
	if (nIndex == -1)
		return FALSE;

	// make sure we have a valid HWND to switch to
	const TDCVIEW& newView = m_aViews[nIndex];
	ASSERT(newView.hwndView);

	if (newView.hwndView == NULL)
		return FALSE;

	CRect rView(0, 0, 0, 0);

	if (m_nSelTab == -1)
	{
		// first time so hide all other views
		int nOther = m_aViews.GetSize();

		while (nOther--)
		{
			if (nOther != nIndex)
				::ShowWindow(m_aViews[nOther].hwndView, SW_HIDE);
		}

		// and if the index > 0, grab the [0] view rect as the best hint
		if (nIndex > 0)
			GetViewRect(m_aViews[0], rView);
	}
	else // just hide the currently visible view
	{
		int nOldIndex = TabToIndex(m_nSelTab);
		TDCVIEW& oldView = m_aViews[nOldIndex];

		GetViewRect(oldView, rView);
	}

	if (!rView.IsRectEmpty())
		::MoveWindow(newView.hwndView, rView.left, rView.top, rView.Width(), rView.Height(), FALSE);

	::ShowWindow(newView.hwndView, SW_SHOW);

	if (m_nSelTab != -1)
	{
		int nOldIndex = TabToIndex(m_nSelTab);
		::ShowWindow(m_aViews[nOldIndex].hwndView, SW_HIDE);
	}

	m_nSelTab = nTab;
	SetCurSel(nTab);
	UpdateTabItemWidths(FALSE);

	return TRUE;
}

CWnd* CTDLViewTabControl::GetActiveWnd() const
{
	ASSERT (GetSafeHwnd());

	if (GetSafeHwnd() == NULL)
		return NULL;

	int nIndex = TabToIndex(m_nSelTab);

	return (nIndex == -1) ? NULL : CWnd::FromHandle(m_aViews[nIndex].hwndView);
}

FTC_VIEW CTDLViewTabControl::GetActiveView() const
{
	return ((m_nSelTab == -1) ? FTCV_UNSET : GetTabView(m_nSelTab));
}

void* CTDLViewTabControl::GetActiveViewData() const
{
	int nIndex = TabToIndex(m_nSelTab);

	return ((nIndex == -1) ? NULL : m_aViews[nIndex].pData);
}

HWND CTDLViewTabControl::GetViewHwnd(FTC_VIEW nView) const
{
	int nIndex = FindView(nView);

	if (nIndex < 0 || nIndex > m_aViews.GetSize())
		return NULL;

	return m_aViews[nIndex].hwndView;
}

CString CTDLViewTabControl::GetViewName(FTC_VIEW nView) const
{
	int nIndex = FindView(nView);

	if (nIndex < 0 || nIndex > m_aViews.GetSize())
		return _T("");

	return m_aViews[nIndex].sViewLabel;
}

BOOL CTDLViewTabControl::SetViewHwnd(FTC_VIEW nView, HWND hWnd)
{
	int nIndex = FindView(nView);

	if (nIndex < 0 || nIndex > m_aViews.GetSize())
		return NULL;

	ASSERT (m_aViews[nIndex].hwndView == NULL);

	if (m_aViews[nIndex].hwndView)
		return FALSE;

	m_aViews[nIndex].hwndView = hWnd;
	return TRUE;
}

void* CTDLViewTabControl::GetViewData(FTC_VIEW nView) const
{
	int nIndex = FindView(nView);

	return (nIndex == -1) ? NULL : m_aViews[nIndex].pData;
}

FTC_VIEW CTDLViewTabControl::GetView(int nIndex) const
{
	if (nIndex < 0 || nIndex > m_aViews.GetSize())
		return FTCV_UNSET;

	// else
	return m_aViews[nIndex].nView;
}

int CTDLViewTabControl::TabToIndex(int nTab) const
{
	FTC_VIEW nView = GetTabView(nTab);
	ASSERT(nView != FTCV_UNSET);

	int nIndex = FindView(nView);
	ASSERT(nIndex >= 0 && nIndex < m_aViews.GetSize());

	return nIndex;
}

int CTDLViewTabControl::IndexToTab(int nIndex) const
{
	FTC_VIEW nView = m_aViews[nIndex].nView;

	int nTab = FindTab(nView);
	ASSERT(nTab >= 0);

	return nTab;
}

FTC_VIEW CTDLViewTabControl::GetTabView(int nTab) const
{
	return (FTC_VIEW)GetItemData(nTab);
}

BOOL CTDLViewTabControl::SetActiveView(CWnd* pWnd, BOOL bNotify)
{
	int nNewIndex = FindView(pWnd->GetSafeHwnd());
	ASSERT(nNewIndex != -1);

	int nNewTab = IndexToTab(nNewIndex);
	int nOldTab = m_nSelTab;

	return DoTabChange(nOldTab, nNewTab, bNotify);
}

BOOL CTDLViewTabControl::SetActiveView(FTC_VIEW nView, BOOL bNotify)
{
	int nNewTab = FindTab(nView);
	int nOldTab = m_nSelTab;

	return DoTabChange(nOldTab, nNewTab, bNotify);
}

void CTDLViewTabControl::Resize(const CRect& rect, CDeferWndMove* pDWM, LPRECT prcView)
{
	CRect rTabs, rView;

	if (CalcTabViewRects(rect, rTabs, rView))
	{
		CWnd* pView = GetActiveWnd();
		ASSERT(pView);

		if (pView)
		{
			if (pDWM)
			{
				pDWM->MoveWindow(this, rTabs);
				pDWM->MoveWindow(pView, rView, FALSE);
			}
			else
			{
				MoveWindow(rTabs);
				pView->MoveWindow(rView, FALSE);
			}
		}

		if (prcView)
			*prcView = rView;
	}
}

int CTDLViewTabControl::FindView(HWND hWnd) const
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

int CTDLViewTabControl::FindView(FTC_VIEW nView) const
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

int CTDLViewTabControl::FindTab(FTC_VIEW nView) const
{
	// find tab with view as its item data
	return FindItemByData(nView);
}

void CTDLViewTabControl::PreSubclassWindow() 
{
	CXPTabCtrl::PreSubclassWindow();

	ShowTabControl(m_bShowingTabs);

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
	}
}

BOOL CTDLViewTabControl::CalcTabViewRects(const CRect& rPos, CRect& rTabs, CRect& rView)
{
	if (!GetSafeHwnd())
		return FALSE;

	if (m_bShowingTabs)
	{
		rTabs = rPos;
		rTabs.bottom = rTabs.top;
		AdjustRect(TRUE, rTabs);

		int nTabHeight = rTabs.Height();
		rTabs = rView = rPos;

		switch (GetOrientation())
		{
			case e_tabTop:	  
				rTabs.bottom = rTabs.top + nTabHeight - 7;
				rView.top = rTabs.bottom;
				break;

			case e_tabBottom: 
				rTabs.top = rTabs.bottom - nTabHeight + 7;
				rView.bottom = rTabs.top;
				break;

			case e_tabLeft:	  
				rTabs.right = rTabs.left + nTabHeight; 
				rView.left = rTabs.right;
				break;

			case e_tabRight:  
				rTabs.left = rTabs.right - nTabHeight;
				rView.right = rTabs.left;
				break;

			default:
				return FALSE;
		}
	}
	else
	{
		rTabs.SetRectEmpty();
		rView = rPos;
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
	int nCurTab = FindTab(GetActiveView());
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
	int nOldIndex = TabToIndex(nOldTab);

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
		int nNewIndex = TabToIndex(nNewTab);
		HWND hNewView = m_aViews[nNewIndex].hwndView;

		if (bHadFocus)
			::SetFocus(hNewView);
	}

	return TRUE;
}

DWORD CTDLViewTabControl::ModifyStyles(DWORD dwStyle, BOOL bAdd)
{
	if (bAdd)
		m_dwStyles |= dwStyle;
	else
		m_dwStyles &= dwStyle;

	return m_dwStyles;
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

BOOL CTDLViewTabControl::WantTabCloseButton(int nTab) const
{
	return (GetTabView(nTab) != FTCV_TASKTREE);
}

BOOL CTDLViewTabControl::IsViewTabShowing(FTC_VIEW nView) const
{
	return (FindTab(nView) != -1);
}

BOOL CTDLViewTabControl::ShowViewTab(FTC_VIEW nView, BOOL bShow)
{
	ASSERT(GetSafeHwnd());
	ASSERT(nView != FTCV_TASKTREE);

	// find index of TDCVIEW item
	int nItem = FindView(nView);

	if (nItem == -1)
		return FALSE; // item must exist

	// find tab with that as its item data
	int nTab = FindTab(nView);

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
		nInsert = FindTab(m_aViews[nPrevItem].nView);

		if (nInsert != -1)
		{
			nInsert++; // insert after
			break;
		}
	}
	ASSERT(nInsert > 0);

	nTab = InsertItem(TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE, nInsert, view.sViewLabel, view.nImage, (LPARAM)nView);
	ASSERT(nTab > 0);

	// fixup selection
	if ((nTab > 0) && (m_nSelTab >= nTab))
	{
		m_nSelTab++;
	}

	return (nTab > 0);
}
