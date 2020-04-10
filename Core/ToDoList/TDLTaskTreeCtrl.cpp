// TDCTreeListCtrl.cpp: implementation of the CTDCTreeListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDlTaskTreeCtrl.h"
#include "todoctrldata.h"
#include "tdcstatic.h"
#include "tdcmsg.h"
#include "tdcimagelist.h"

#include "..\shared\treectrlhelper.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\timehelper.h"
#include "..\shared\misc.h"
#include "..\shared\TreeDragDropHelper.h"
#include "..\shared\themed.h"
#include "..\shared\osversion.h"
#include "..\shared\ScopedTimer.h"

#include "..\3rdparty\shellicons.h"
#include "..\3rdparty\colordef.h"

#include "..\Interfaces\Preferences.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const UINT TIMER_EDITLABEL		= 101;

const COLORREF COMMENTSCOLOR	= RGB(98, 98, 98);
const COLORREF ALTCOMMENTSCOLOR = RGB(164, 164, 164);

const int TITLE_BORDER_OFFSET	= 2;

//////////////////////////////////////////////////////////////////////

enum
{
	IDC_TASKTREE = 100,		
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLTaskTreeCtrl, CTDLTaskCtrlBase)

//////////////////////////////////////////////////////////////////////

CTDLTaskTreeCtrl::CTDLTaskTreeCtrl(const CTDCImageList& ilIcons,
								   const CToDoCtrlData& data, 
								   const CTDCStyleMap& styles,
								   const TDCAUTOLISTDATA& tld,
								   const CTDCColumnIDMap& mapVisibleCols,
								   const CTDCCustomAttribDefinitionArray& aCustAttribDefs) 
	: 
	CTDLTaskCtrlBase(FALSE, ilIcons, data, m_find, styles, tld, mapVisibleCols, aCustAttribDefs),
	m_tsh(m_tcTasks),
	m_tch(m_tcTasks),
	m_htiLastHandledLBtnDown(NULL),
	m_wKeyPress(0),
	m_bMovingItem(FALSE),
	m_bEditLabelTimerStarted(FALSE),
#pragma warning (disable: 4355)
	m_reminders(*this),
	m_find(m_tch, data, m_reminders)
#pragma warning (default: 4355)
{
}

CTDLTaskTreeCtrl::~CTDLTaskTreeCtrl()
{
}

///////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTDLTaskTreeCtrl, CTDLTaskCtrlBase)
//{{AFX_MSG_MAP(CTDCTreeListCtrl)
//}}AFX_MSG_MAP
ON_WM_SETCURSOR()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////

DWORD CTDLTaskTreeCtrl::GetHelpID() const
{
	static LPCTSTR IDC_TREEVIEW_CTRL = _T("IDC_TREEVIEW_CTRL");
	return (DWORD)IDC_TREEVIEW_CTRL;
}

BOOL CTDLTaskTreeCtrl::CreateTasksWnd(CWnd* pParentWnd, const CRect& rect, BOOL bVisible)
{
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0));
	
	return m_tcTasks.Create((dwStyle | 
							WS_TABSTOP |
							TVS_SHOWSELALWAYS | 
							TVS_HASLINES | 
							TVS_LINESATROOT | 
							TVS_HASBUTTONS | 
							TVS_NONEVENHEIGHT),
							rect, 
							pParentWnd, 
							IDC_TASKTREE);
}

BOOL CTDLTaskTreeCtrl::IsTreeItemSelected(HWND hwnd, HTREEITEM hti) const
{
	ASSERT(hwnd == m_tcTasks);
	UNREFERENCED_PARAMETER(hwnd);
	
	return TSH().HasItem(hti);
}

// external version
BOOL CTDLTaskTreeCtrl::SelectItem(HTREEITEM hti) 
{ 
	return SelectItem(hti, TRUE, SC_UNKNOWN);
}

// internal version) 
BOOL CTDLTaskTreeCtrl::SelectItem(HTREEITEM hti, BOOL bSyncAndNotify, SELCHANGE_ACTION nBy)
{ 
	// Avoid unnecessary selections
	if (GetSelectedCount() == 1)
	{
		HTREEITEM htiSel = GetSelectedItem();

		if ((hti == htiSel) && (htiSel == m_tcTasks.GetSelectedItem()))
		{
			if (bSyncAndNotify)
				SyncColumnSelectionToTasks();

			return TRUE;
		}
	}

	if (TSH().RemoveAll() && !hti)
		m_lcColumns.Invalidate();
	
	BOOL bSelected = FALSE;
	
	if (hti)
	{
		TSH().AddItem(hti);
		TSH().SetAnchor(hti);
		
		bSelected = TCH().SelectItem(hti);
		
		CHoldHScroll hhs(m_tcTasks);
		TCH().EnsureItemVisible(hti, FALSE);
	}

	if (bSyncAndNotify)
	{
		SyncColumnSelectionToTasks();
		NotifyParentSelChange(nBy);
	}
	
	return bSelected;
}

void CTDLTaskTreeCtrl::DeselectAll()
{
	// prevent resyncing
	CTLSHoldResync hr(*this);

	TSH().RemoveAll();
	TCH().SelectItem(NULL);

	m_lcColumns.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CTDLTaskTreeCtrl::SelectAll() 
{ 
	if (TSH().AddAll())
	{
		SyncColumnSelectionToTasks();
		UpdateSelectedTaskPath();
		NotifyParentSelChange();

		return TRUE;
	}
	
	return FALSE;
}

void CTDLTaskTreeCtrl::OnStylesUpdated(const CTDCStyleMap& styles, BOOL bAllowResort)
{
	CTDLTaskCtrlBase::OnStylesUpdated(styles, bAllowResort);

	// Our extra handling
	if (styles.HasStyle(TDCS_SHOWINFOTIPS))
		SetTasksWndStyle(TVS_NOTOOLTIPS, styles.IsStyleEnabled(TDCS_SHOWINFOTIPS), FALSE);

	SyncColumnSelectionToTasks();
}

BOOL CTDLTaskTreeCtrl::BuildColumns()
{
	// nothing more to do
	return CTDLTaskCtrlBase::BuildColumns();
}

DWORD CTDLTaskTreeCtrl::HitTestTasksTask(const CPoint& ptScreen) const
{
	// see if we hit a task in the tree
	CPoint ptClient(ptScreen);
	m_tcTasks.ScreenToClient(&ptClient);
	
	HTREEITEM hti = m_tcTasks.HitTest(ptClient);

	if (hti)
		return GetTaskID(hti);

	// all else
	return 0;
}

void CTDLTaskTreeCtrl::Release() 
{ 
	if (::IsWindow(m_hdrTasks))
		m_hdrTasks.UnsubclassWindow();

	CTDLTaskCtrlBase::Release();
}

int CTDLTaskTreeCtrl::GetExpandedTasks(CDWordArray& aExpanded) const
{
	return GetExpandedTasks(aExpanded, NULL);
}

int CTDLTaskTreeCtrl::GetExpandedTasks(CDWordArray& aExpanded, HTREEITEM hti) const
{
	int nStart = 0;
	
	if (hti == NULL)
	{
		// guesstimate initial size
		aExpanded.SetSize(0, m_tcTasks.GetCount() / 4);
	}
	else if (TCH().IsItemExpanded(hti) <= 0)
	{
		return 0; // nothing added
	}
	else // expanded
	{
		nStart = aExpanded.GetSize();
		aExpanded.Add(GetTaskID(hti));
	}
	
	// process children
	HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);
	
	while (htiChild)
	{
		GetExpandedTasks(aExpanded, htiChild);
		htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
	}
	
	return (aExpanded.GetSize() - nStart);
}

void CTDLTaskTreeCtrl::SetExpandedTasks(const CDWordArray& aExpanded)
{
	CHoldRedraw hr(*this);
	CTLSHoldResync hr2(*this);

	int nNumExpanded = aExpanded.GetSize();
	
	for (int nItem = 0; nItem < nNumExpanded; nItem++)
	{
		HTREEITEM hti = GetItem(aExpanded[nItem]);
		
		if (hti)
		{
			// Don't expand parents because they should also be in the list
			ExpandItemRaw(hti, TRUE, FALSE, FALSE, FALSE);
		}
	}
	
	if (nNumExpanded)
	{
		ExpandList();
		RecalcUntrackedColumnWidths();
	}
}

HTREEITEM CTDLTaskTreeCtrl::GetItem(DWORD dwTaskID) const
{
	return m_mapTaskIDToHTI.GetItem(dwTaskID);
}

void CTDLTaskTreeCtrl::OnBeginRebuild()
{
	EnableResync(FALSE);

	if (CWnd::IsWindowVisible())
		CWnd::SendMessage(WM_SETREDRAW, FALSE);

	m_mapTaskIDToHTI.RemoveAll();
}

void CTDLTaskTreeCtrl::OnEndRebuild()
{
	if (CWnd::IsWindowVisible())
		CWnd::SendMessage(WM_SETREDRAW, TRUE);

	EnableResync(TRUE, Tasks());

	ExpandList();
	RecalcUntrackedColumnWidths();

	// Resync horizontal scrollbars because 
	// RecalcUntrackedColumnWidths doesn't always do it
	PostResize();
}

BOOL CTDLTaskTreeCtrl::EnsureSelectionVisible()
{
	if (!GetSelectedCount())
		return FALSE;

	OSVERSION nOSVer = COSVersion();
	
	if ((nOSVer == OSV_LINUX) || (nOSVer < OSV_VISTA))
	{
		m_tcTasks.PostMessage(TVM_ENSUREVISIBLE, 0, (LPARAM)GetTreeSelectedItem());
	}
	else
	{
		// Check there's something to do because holding 
		// the redraw/scroll has a cost
		BOOL bAllVisible = TRUE;
		POSITION pos = TSH().GetFirstItemPos();
		
		while (pos && bAllVisible)
		{
			HTREEITEM htiSel = TSH().GetNextItem(pos);
			bAllVisible = TCH().IsParentItemExpanded(htiSel, TRUE);
		}
		
		if (bAllVisible)
			bAllVisible = TCH().IsItemVisible(GetTreeSelectedItem(), FALSE);
		
		if (!bAllVisible)
		{
			CHoldRedraw hr(*this);
			CHoldHScroll hhs(m_tcTasks, 0);
			
			// Expand the parents of all selected tasks
			POSITION pos = TSH().GetFirstItemPos();
			
			while (pos)
			{
				HTREEITEM htiSel = TSH().GetNextItem(pos);
				HTREEITEM htiParent = m_tcTasks.GetParentItem(htiSel);
				
				if (htiParent)
					TCH().ExpandItem(htiParent);
			}
			
			TCH().EnsureItemVisible(GetTreeSelectedItem(), FALSE);
		}
	}

	return TRUE;
}

BOOL CTDLTaskTreeCtrl::IsColumnShowing(TDC_COLUMN nColID) const
{
	// never show path
	if (nColID == TDCC_PATH)
		return FALSE;

	// else
	return CTDLTaskCtrlBase::IsColumnShowing(nColID);
}

BOOL CTDLTaskTreeCtrl::IsAlternateTreeItemLine(HTREEITEM hti) const
{
	return IsAlternateColumnLine(GetListItem(hti));
}

int CTDLTaskTreeCtrl::GetListItem(HTREEITEM hti) const
{
	return FindListItem(m_lcColumns, (DWORD)hti);
}

void CTDLTaskTreeCtrl::DeleteAll()
{
	TSH().RemoveAll();

	if (!IsResyncEnabled())
		m_lcColumns.DeleteAllItems();
		
	m_tcTasks.DeleteAllItems();
	m_mapTaskIDToHTI.RemoveAll();

	ASSERT(m_lcColumns.GetItemCount() == 0);
}

void CTDLTaskTreeCtrl::ExpandAll(BOOL bExpand)
{
	// if we're collapsing the tasklist find the 
	// currently selected item's top-level parent and
	// move the selection there else the branch
	// containing the selected item never closes
	if (!bExpand)
	{
		HTREEITEM htiSel = GetSelectedItem();
		HTREEITEM htiParent = TCH().GetTopLevelItem(htiSel);

		if (htiParent != htiSel)
			SelectItem(htiParent, FALSE, CTDLTaskCtrlBase::SC_UNKNOWN);
	}

	ExpandItem(NULL, bExpand, TRUE);
}

void CTDLTaskTreeCtrl::ExpandItem(HTREEITEM hti, BOOL bExpand, BOOL bAndChildren)
{
	// avoid unnecessary processing
	if (hti && !CanExpandItem(hti, bExpand))
		return;

	HTREEITEM htiSel = GetSelectedItem();
	
	// scope redraw holding else EnsureVisible doesn't work
	{
		CHoldRedraw hr(*this);
		CTLSHoldResync hr3(*this);
		
		ExpandItemRaw(hti, bExpand, bAndChildren, TRUE);
	}

	if (htiSel)
	{
		CHoldHScroll hhs(m_tcTasks);
		m_tcTasks.EnsureVisible(htiSel);
	}

	RecalcUntrackedColumnWidths();
}

void CTDLTaskTreeCtrl::ExpandItemRaw(HTREEITEM hti, BOOL bExpand, BOOL bAndChildren, BOOL bAndParents, BOOL bUpdateList)
{
	TCH().ExpandItem(hti, bExpand, bAndChildren, bAndParents);

	if (bUpdateList)
	{
		if (bExpand)
		{
			if (hti)
			{
				int nNextIndex = (GetListItem(hti) + 1);
				ExpandList(hti, nNextIndex);
			}
			else
			{
				ExpandList(); // all
			}
		}
		else
		{
			CollapseList(hti);
		}
	}
}

BOOL CTDLTaskTreeCtrl::CanExpandItem(HTREEITEM hti, BOOL bExpand) const
{
	int nFullyExpanded = TCH().IsItemExpanded(hti, TRUE);
	
	if (nFullyExpanded == -1)	// item has no children
	{
		return FALSE; // can neither expand nor collapse
	}
	else if (bExpand)
	{
		return !nFullyExpanded;
	}
	
	// else
	return TCH().IsItemExpanded(hti, FALSE);
}

void CTDLTaskTreeCtrl::ResortSelectedTaskParents()
{
	{
		CHoldListVScroll hold(m_lcColumns);
	
		CHTIList lstParents;
		TSH().GetUniqueParents(lstParents);
		
		TDSORTPARAMS ss(*this);
		PFNTLSCOMPARE pfnSort = PrepareSort(ss);

		POSITION pos = lstParents.GetHeadPosition();
		
		while (pos)
		{
			HTREEITEM htiParent = lstParents.GetNext(pos);

			CTreeListSyncer::Sort(pfnSort, (LPARAM)&ss, htiParent);
		}
		
		ResyncSelection(m_lcColumns, Tasks(), FALSE);
	}

	ResyncScrollPos(Tasks(), m_lcColumns);
	EnsureSelectionVisible();
}

LRESULT CTDLTaskTreeCtrl::OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD) 
{
	ASSERT(pTVCD->nmcd.hdr.hwndFrom == m_tcTasks);
	ASSERT(IsResyncEnabled());
	ASSERT(!m_bTreeExpanding);

	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
	DWORD dwTaskID = pTVCD->nmcd.lItemlParam;
	
	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
		
	case CDDS_ITEMPREPAINT:
		return OnPrePaintTaskTitle(pTVCD->nmcd, TRUE, pTVCD->clrText, pTVCD->clrTextBk);
		
	case CDDS_ITEMPOSTPAINT:
		return OnPostPaintTaskTitle(pTVCD->nmcd);
	}
	
	return CDRF_DODEFAULT;
}

BOOL CTDLTaskTreeCtrl::GetSelectionBoundingRect(CRect& rSelection) const
{
	if (TSH().GetBoundingRect(rSelection))
	{
		m_tcTasks.ClientToScreen(rSelection);
		ScreenToClient(rSelection);
		
		return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::IsAlternateTitleLine(const NMCUSTOMDRAW& nmcd) const
{
	return IsAlternateTreeItemLine((HTREEITEM)nmcd.dwItemSpec);
}

GM_ITEMSTATE CTDLTaskTreeCtrl::GetItemTitleState(const NMCUSTOMDRAW& nmcd) const
{
	return GetTreeItemState((HTREEITEM)nmcd.dwItemSpec);
}

GM_ITEMSTATE CTDLTaskTreeCtrl::GetTreeItemState(HTREEITEM hti) const
{
	if (!m_bSavingToImage)
	{
		if (m_tcTasks.GetItemState(hti, TVIS_DROPHILITED) & TVIS_DROPHILITED)
		{
			return GMIS_DROPHILITED;
		}
		else if (IsItemSelected(hti))
		{
			DWORD dwTaskID = GetTaskID(hti);
		
			if (HasFocus() || (dwTaskID == m_dwEditTitleTaskID))
				return GMIS_SELECTED;
		
			// else 
			return GMIS_SELECTEDNOTFOCUSED;
		}
	}
	
	return GMIS_NONE;
}

GM_ITEMSTATE CTDLTaskTreeCtrl::GetColumnItemState(int nItem) const
{
	return GetTreeItemState((HTREEITEM)m_lcColumns.GetItemData(nItem));
}

BOOL CTDLTaskTreeCtrl::OnListSelectionChange(NMLISTVIEW* pNMLV)
{
	// only called when the focus is actually on the columns
	// ie. not when Syncing Column Selection)
	
	// sync only the item that has changed 
	HTREEITEM hti = (HTREEITEM)m_lcColumns.GetItemData(pNMLV->iItem);
	
	BOOL bWasSel = (pNMLV->uOldState & LVIS_SELECTED);
	BOOL bSel = (pNMLV->uNewState & LVIS_SELECTED);
	
	if (bSel != bWasSel)
		TSH().SetItem(hti, (bSel ? TSHS_SELECT : TSHS_DESELECT), FALSE);
	
	// then sync focused item
	BOOL bWasFocused = (pNMLV->uOldState & LVIS_FOCUSED);
	BOOL bFocused = (pNMLV->uNewState & LVIS_FOCUSED);
	
	BOOL bLBtnDown = Misc::IsKeyPressed(VK_LBUTTON);
	BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);

	if (bFocused != bWasFocused)
	{
		TSH().FixupTreeSelection();
		
		if (bFocused && !Misc::IsKeyPressed(VK_SHIFT))
			TSH().SetAnchor(hti);
	}
	
	InvalidateItem(hti, TRUE);
	
	// notify parent of selection change
	CPoint pt(GetMessagePos());
	m_lcColumns.ScreenToClient(&pt);

	int nHit = m_lcColumns.HitTest(pt);

	if (Misc::IsCursorKeyPressed(MKC_UPDOWN))
	{
		// vertical scrolling
		return FALSE;
	}

	if (bLBtnDown && !bCtrl && TSH().IsEmpty() && (nHit != -1))
	{
		// In the middle of a simple click
		return FALSE;
	}

	if (IsBoundSelecting() && ((nHit == -1) || (m_lcColumns.GetSelectedCount() > 2)))
	{
		// bulk selecting
		return FALSE;
	}

	NotifyParentSelChange();
	return TRUE;
}

void CTDLTaskTreeCtrl::SyncColumnSelectionToTasks()
{
	ASSERT(CanResync());

	CHTIList selection;
	TSH().CopySelection(selection);

	if (ResyncListToTreeSelection(m_tcTasks, selection, TSH().GetAnchor()))
 		m_lcColumns.UpdateWindow();
}

void CTDLTaskTreeCtrl::NotifyParentSelChange(SELCHANGE_ACTION nAction)
{
	if (m_bMovingItem)
		return;

	NMTREEVIEW nmtv = { 0 };

	nmtv.hdr.code = TVN_SELCHANGED;

	switch (nAction)
	{
	case SC_UNKNOWN:
		nmtv.action = TVC_UNKNOWN;
		break;

	case SC_BYKEYBOARD:
		nmtv.action = TVC_BYKEYBOARD;
		break;

	default:
		nmtv.action = TVC_BYMOUSE;
		nmtv.ptDrag = CPoint(::GetMessagePos()); // screen coords
		break;
	}

	RepackageAndSendToParent(WM_NOTIFY, 0, (LPARAM)&nmtv);
}

BOOL CTDLTaskTreeCtrl::OnTreeSelectionChange(NMTREEVIEW* pNMTV)
{
	// we don't support nothing having being selected unless there
	// are no items in the tree
	BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL) && (s_nExtendedSelection & HOTKEYF_CONTROL);
	BOOL bShift = Misc::IsKeyPressed(VK_SHIFT) && (s_nExtendedSelection & HOTKEYF_SHIFT);
	
	// cursor handled here
	// <shift>+cursor handled here
	if (m_wKeyPress == 0)
		return FALSE;

	HTREEITEM hti = pNMTV->itemNew.hItem;

	// snapshot current selection to test for changes
	CHTIList lstPrevSel;
	TSH().CopySelection(lstPrevSel);

	switch (m_wKeyPress)
	{
	case VK_NEXT:
	case VK_DOWN:
	case VK_UP:
	case VK_PRIOR:
	case VK_RIGHT:
	case VK_LEFT:
	case VK_HOME:
	case VK_END:
		if (!bCtrl)
		{
			TSH().RemoveAll();

			if (bShift)
			{
				TSH().AddItems(TSH().GetAnchor(), hti);
			}
			else
			{
				TSH().SetAnchor(hti);
				TSH().AddItem(hti);
			}
		}
		break;

	default:
		// else handle alphanum method of changing selection
		TSH().RemoveAll();
		TSH().SetAnchor(hti);
		TSH().AddItem(hti);
		break;
	}
	m_wKeyPress = 0; // always

	// Only propagate this if we're not already in the 
	// middle of a resync
	if (CanResync())
		SyncColumnSelectionToTasks();

	if (hti)
	{
		CHoldHScroll hhs(m_tcTasks);
		TCH().EnsureItemVisible(hti, FALSE);
	}

	// notify parent of selection change
	// unless up/down cursor key still pressed
	if (TSH().Matches(lstPrevSel) || Misc::IsCursorKeyPressed(MKC_UPDOWN))
		return FALSE;

	NotifyParentSelChange(SC_BYKEYBOARD);
	return TRUE;
}

BOOL CTDLTaskTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (((pMsg->hwnd == m_tcTasks) && CToolTipCtrlEx::IsMouseDown(pMsg->message)))
	{
		CToolTipCtrl* pTT = m_tcTasks.GetToolTips();

		if (pTT)
			pTT->PostMessage(TTM_POP);
	}
	
	return CTDLTaskCtrlBase::PreTranslateMessage(pMsg);
}

BOOL CTDLTaskTreeCtrl::HandleClientColumnClick(const CPoint& pt, BOOL bDblClk)
{
	if (!IsReadOnly() && Misc::ModKeysArePressed(0))
	{
		UINT nHitFlags = 0;
		HTREEITEM htiHit = HitTestItem(pt, &nHitFlags);

		if (htiHit && !(nHitFlags & TVHT_ONITEMBUTTON))
		{
			DWORD dwTaskID = GetTaskID(htiHit);
			TDC_COLUMN nClickCol = TDCC_NONE;

			if (!bDblClk)
			{
				if (nHitFlags & TVHT_ONITEMSTATEICON)
				{
					nClickCol = TDCC_DONE;
				}
				else if (nHitFlags & TVHT_ONITEMICON)
				{
					nClickCol = TDCC_ICON;
				}
			}
			else // double click
			{
				if (m_tcTasks.ItemHasChildren(htiHit))
				{
					ExpandItem(htiHit, !TCH().IsItemExpanded(htiHit), TRUE);

					// save item handle so we don't re-handle in LButtonUp handler
					m_htiLastHandledLBtnDown = htiHit;

					return TRUE;
				}
				else if (nHitFlags & (TVHT_ONITEMLABEL | TVHT_ONITEMRIGHT))
				{
					nClickCol = TDCC_CLIENT;
				}
			}

			if ((nClickCol != TDCC_NONE) && !SelectionHasLocked(FALSE))
			{
				// make sure attribute pane is synced
				SyncColumnSelectionToTasks();

				// forward the click
				NotifyParentOfColumnEditClick(nClickCol, dwTaskID);

				return TRUE;
			}
		}
	}

	// not handled
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ((pWnd == &m_tcTasks) && !IsReadOnly())
	{
		UINT nHitFlags = 0;

		CPoint ptClient(::GetMessagePos());
		m_tcTasks.ScreenToClient(&ptClient);

		HTREEITEM htiHit = m_tcTasks.HitTest(ptClient, &nHitFlags);
		
		if (m_calculator.IsTaskLocked(GetTaskID(htiHit)))
		{
			return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));
		}
		else if (nHitFlags & TVHT_ONITEMICON)
		{
			return GraphicsMisc::SetHandCursor();
		}
	}
	
	// else
	return CTDLTaskCtrlBase::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CTDLTaskTreeCtrl::OnTreeGetDispInfo(NMTVDISPINFO* pTVDI)
{
	DWORD dwTaskID = pTVDI->item.lParam;

	// text
	if ((pTVDI->item.mask & TVIF_TEXT) && (m_dwEditTitleTaskID != dwTaskID))
	{
		pTVDI->item.pszText = (LPTSTR)(LPCTSTR)m_data.GetTaskTitle(dwTaskID);
	}

	// image
	if (pTVDI->item.mask & (TVIF_SELECTEDIMAGE | TVIF_IMAGE))
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		if (m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		{
			if (IsColumnShowing(TDCC_ICON))
			{
				pTVDI->item.iImage = pTVDI->item.iSelectedImage = -1;
			}
			else
			{
				pTVDI->item.iImage = -1;
				
				if (!pTDI->sIcon.IsEmpty())
				{
					pTVDI->item.iImage = m_ilTaskIcons.GetImageIndex(pTDI->sIcon);
				}
				else if (HasStyle(TDCS_SHOWPARENTSASFOLDERS) && pTDS->HasSubTasks())
				{
					pTVDI->item.iImage = 0;
				}
				
				pTVDI->item.iSelectedImage = pTVDI->item.iImage;
			}

			// checkbox image
			pTVDI->item.mask |= TVIF_STATE;
			pTVDI->item.stateMask = TVIS_STATEIMAGEMASK;

			if (pTDI->IsDone())
			{
				pTVDI->item.state = TCHC_CHECKED;
			}
			else if (m_data.TaskHasCompletedSubtasks(pTDS))
			{
				pTVDI->item.state = TCHC_MIXED;
			}
			else 
			{
				pTVDI->item.state = TCHC_UNCHECKED;
			}
		}
	}

	return 0L;
}

#ifdef _DEBUG ////////////////////////////////////////////////////////////////////

void CTDLTaskTreeCtrl::Trace(LPCTSTR szComment)
{
	Trace(szComment, NULL);
}

void CTDLTaskTreeCtrl::Trace(LPCTSTR szComment, HTREEITEM hti)
{
	if (m_tcTasks.GetCount() == 0)
		return;

	// header
	CString sComment;

	if (hti)
		sComment.Format(_T("\n%s(%08x) - Begin\n\n"), szComment, (DWORD)hti);
	else
		sComment.Format(_T("\n%s - Begin\n\n"), szComment);
	
	TRACE(sComment);

	// contents
	Trace(NULL, -1);

	// footer
	if (hti)
		sComment.Format(_T("\n%s(%08x) - End\n\n"), szComment, (DWORD)hti);
	else
		sComment.Format(_T("\n%s - End\n\n"), szComment);
	
	TRACE(sComment);
}

void CTDLTaskTreeCtrl::Trace(HTREEITEM hti, int nLevel)
{
	if (hti && (hti != TVI_ROOT))
	{
		CString sIndent('\t', nLevel);
		TRACE(_T("%s%s (%08x), %d \n"), sIndent, m_tcTasks.GetItemText(hti), (DWORD)hti, GetListItem(hti));
	}

	// children
	HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);

	while (htiChild)
	{
		Trace(htiChild, (nLevel + 1)); // RECURSIVE
		htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
	}
}

#endif // _DEBUG ///////////////////////////////////////////////////////////////////

// handles messages to 'Us'
LRESULT CTDLTaskTreeCtrl::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTDLTaskCtrlBase::WindowProc(hRealWnd, msg, wp, lp);
	
	ASSERT(hRealWnd == GetHwnd());
	
	switch (msg)
	{
	case WM_TIMER:
		if (wp == TIMER_EDITLABEL)
		{
			EndLabelEditTimer();
			
			if (!IsReadOnly())
				NotifyParentOfColumnEditClick(TDCC_CLIENT, GetSelectedTaskID());
			
			return 0L; // eat
		}
		break;
		
	case WM_NOTIFY:
		if (wp == m_scLeft.GetDlgCtrlID() || 
			wp == m_scRight.GetDlgCtrlID() ||
			wp == (UINT)m_hdrTasks.GetDlgCtrlID() ||
			wp == (UINT)m_hdrColumns.GetDlgCtrlID())
		{
			LPNMHDR pNMHDR = (LPNMHDR)lp;
			
			// our extra handling
			switch (pNMHDR->code)
			{
			case NM_DBLCLK:
				return 0L; // always eat

			case TVN_SELCHANGED:
				{
					CHoldHScroll hhs(m_tcTasks);
					return CTDLTaskCtrlBase::WindowProc(hRealWnd, msg, wp, lp);
				}
				break;

 			case TVN_BEGINLABELEDIT:
				return 0L; // we handle this ourselves
				
			case TVN_GETDISPINFO:
				OnTreeGetDispInfo((NMTVDISPINFO*)pNMHDR);
				break;

			case TVN_KEYDOWN:
				{
					// all we do is get a copy of the key pressed for reference
					NMTVKEYDOWN* pTVKD = (NMTVKEYDOWN*)pNMHDR;
					
					m_wKeyPress = 0;
					
					switch (pTVKD->wVKey)
					{
					case VK_NEXT:  
					case VK_DOWN:
					case VK_UP:
					case VK_PRIOR: 
					case VK_RIGHT:
					case VK_LEFT:
					case VK_HOME:
					case VK_END:
						m_wKeyPress = pTVKD->wVKey;
						break;
						
					default:
						// handle alphanum method of changing selection
						{
							// convert to char because its easier to work out what
							// are valid chars
							UINT nChar = MapVirtualKey(pTVKD->wVKey, 2);
							
							if (nChar >= 0x20 && nChar <= 0xFF)
								m_wKeyPress = pTVKD->wVKey;
						}
						break;
					}
				}
				break;
				
			case TVN_ITEMEXPANDED:
				{
					NMTREEVIEW* pNMTV = (NMTREEVIEW*)lp;

					// make sure at least one selected item is visible 
					// if collapsing the item
					if (pNMTV->action == TVE_COLLAPSE)
					{
						TSH().RemoveHiddenItems();

						if (TSH().GetCount() == 0)
						{
							// move selection to parent
							SelectItem(pNMTV->itemNew.hItem, TRUE, SC_BYMOUSE);
						}
					}
				}

				UpdateAll();
				RecalcUntrackedColumnWidths();
				break;
			}
		}
	}
	
	return CTDLTaskCtrlBase::WindowProc(hRealWnd, msg, wp, lp);
}

void CTDLTaskTreeCtrl::BeginLabelEditTimer()
{
	if (!m_bEditLabelTimerStarted)
	{
		m_bEditLabelTimerStarted = TRUE;
		SetTimer(TIMER_EDITLABEL, 500, NULL);
	}
}

void CTDLTaskTreeCtrl::EndLabelEditTimer()
{
	if (m_bEditLabelTimerStarted)
	{
		m_bEditLabelTimerStarted = FALSE;
		KillTimer(TIMER_EDITLABEL);
	}
}

LRESULT CTDLTaskTreeCtrl::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTDLTaskCtrlBase::ScWindowProc(hRealWnd, msg, wp, lp);
	
	if (hRealWnd == m_tcTasks)
	{
		BOOL bSelChange = FALSE, bEatMsg = FALSE;
		BOOL bColClick = FALSE, bColDblClk = FALSE;

		SELCHANGE_ACTION nAction = SC_UNKNOWN;
		CHTIList lstPrevSel;
		
		switch (msg)
		{
#ifdef _DEBUG
		case WM_PAINT:
			break;
#endif
		case WM_ERASEBKGND:
			if (m_bMovingItem)
				return TRUE;
			break;

		case WM_TIMER:
			// eat the tree's internal timer for editing labels
			// because we implement our own
			if (wp == 42)
				return 0L;
			break;

		case TVM_SELECTITEM:
			if (wp == TVGN_DROPHILITE)
			{
				HTREEITEM hti = (HTREEITEM)lp;
				HTREEITEM htiDrop = m_tcTasks.GetDropHilightItem();
				
				if (htiDrop && (htiDrop != hti))
					InvalidateItem(htiDrop);
				
				if (hti)
					InvalidateItem(hti);
			}
			else if (wp == TVGN_FIRSTVISIBLE)
			{
				CHoldHScroll hhs(m_tcTasks);
				
				return CTDLTaskCtrlBase::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;
			
		case WM_KEYUP: // --------------------------------------------------------------------------
			switch (wp)
			{
			case VK_NEXT:  
			case VK_DOWN:
			case VK_UP:
			case VK_PRIOR: 
				// force a parent notification
				lstPrevSel.RemoveAll();
				bSelChange = TRUE;
				nAction = SC_BYKEYBOARD;
				break;
			}
			break;

		case WM_KEYDOWN: // --------------------------------------------------------------------------
			{
				EndLabelEditTimer();

				BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
				BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
				BOOL bAlt = Misc::IsKeyPressed(VK_MENU);
				
				// <ctrl>+cursor handled here
				// <ctrl>+<shift>+cursor here
				if (!bAlt)
				{
					// snapshot existing selection before we might change it
					TSH().CopySelection(lstPrevSel, TRUE);
					
					// get the real currently selected item
					HTREEITEM hti = m_tcTasks.GetSelectedItem();
					
					bCtrl &= (s_nExtendedSelection & HOTKEYF_CONTROL);
					bShift &= (s_nExtendedSelection & HOTKEYF_SHIFT);
					
					switch (wp)
					{
					case VK_NEXT:  
					case VK_DOWN:
						if (bCtrl)
						{
							HTREEITEM htiNext = NULL;
							
							if (wp == VK_NEXT)
								htiNext = TCH().GetNextPageVisibleItem(hti);
							else
								htiNext = TCH().GetNextVisibleItem(hti);
							
							if (htiNext)
							{
								TCH().SelectItem(htiNext);
								
								// toggle items if shift is also down, but not the one 
								// we're just moving on to
								if (bShift)
								{
									HTREEITEM htiPrev = TCH().GetPrevVisibleItem(htiNext, FALSE);
									TSH().SetItems(htiPrev, hti, TSHS_TOGGLE);
								}
								
								bSelChange = TRUE;
							}
						}
						break;
						
					case VK_UP:
					case VK_PRIOR: 
						if (bCtrl)
						{
							HTREEITEM htiPrev = NULL;
							
							if (wp == VK_PRIOR)
								TCH().GetPrevPageVisibleItem(hti);
							else
								TCH().GetPrevVisibleItem(hti);
							
							if (htiPrev)
							{
								TCH().SelectItem(htiPrev);
								
								// toggle items if shift is also down, but not the one 
								// we're just moving on to
								if (bShift)
								{
									HTREEITEM htiNext = TCH().GetNextVisibleItem(htiPrev, FALSE);
									TSH().SetItems(htiNext, hti, TSHS_TOGGLE);
								}
								
								bSelChange = TRUE;
							}
						}
						break;
						
					case VK_SPACE:
						if (bCtrl && !bShift)
						{
							// toggle real selected item state
							TSH().SetItem(hti, TSHS_TOGGLE);
							
							bSelChange = TRUE;
						}
						break;

					case VK_MULTIPLY:
						// Eat this if the selected item(s) is already fully expanded
						if (!bCtrl && !bShift)
						{
							if (TSH().IsSelectionExpanded(TRUE))
								return 0L;
						}
						break;

					case VK_SUBTRACT:
						// Eat this if the selected item(s) is already collapsed
						if (!bCtrl && !bShift)
						{
							if (!TSH().IsSelectionExpanded())
								return 0L;
						}
						break;
					}
				}
				
				nAction = SC_BYKEYBOARD;
			}
			break;
			
		case WM_RBUTTONDOWN: // --------------------------------------------------------------------------
			{
				EndLabelEditTimer();

				// allow parent to handle any focus changes
				// before we change our selection
				m_tcTasks.SetFocus();

				HTREEITEM hti = m_tcTasks.HitTest(lp);
				
				if (hti)
				{
					// snapshot existing selection before we might change it
					TSH().CopySelection(lstPrevSel, TRUE);
					
					if (!TSH().HasItem(hti))
					{
						TSH().RemoveAll();
						TSH().AddItem(hti);
						TSH().SetAnchor(hti);
						
						bSelChange = TRUE;
					}
					
					if (hti != m_tcTasks.GetSelectedItem())
					{
						m_tcTasks.SelectItem(hti);
						
						bSelChange = TRUE;
					}
				}
				
				nAction = SC_BYMOUSE;
			}
			break;
			
		case WM_LBUTTONDOWN: // --------------------------------------------------------------------------
			{
				BOOL bHadFocus = HasFocus();

				// allow parent to handle any focus changes
				// before we change our selection
				if (!bHadFocus)
					m_tcTasks.SetFocus();

				// don't change selection if user is expanding an item
				UINT nHitFlags = 0;
				CPoint pt(lp);
				HTREEITEM htiHit = m_tcTasks.HitTest(pt, &nHitFlags);

				if (nHitFlags & TVHT_ONITEMBUTTON)
					break;
				
				// snapshot existing selection before we might change it
				TSH().CopySelection(lstPrevSel, TRUE);
				
				BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
				BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
				
				HTREEITEM htiAnchor = TSH().GetAnchor();
				
				if (!htiAnchor && bShift)
					htiAnchor = htiHit;

				if (bCtrl)
				{
					if (bShift)
					{
						TSH().SetItems(htiAnchor, htiHit, TSHS_SELECT);
						
						bSelChange = TRUE;
					}
					else if (m_bReadOnly || !::DragDetect(hRealWnd, pt))
					{
						// if this is not the beginning of a drag then toggle selection
						TSH().SetItem(htiHit, TSHS_TOGGLE);
							
						bSelChange = TRUE;
					}
					
					// save item handle so we don't rehandle in LButtonUp handler
					m_htiLastHandledLBtnDown = htiHit;
				}
				else if (bShift) 
				{
					TSH().RemoveAll();
					TSH().SetItems(htiAnchor, htiHit, TSHS_SELECT);
					
					// save item handle so we don't rehandle in LButtonUp handler
					m_htiLastHandledLBtnDown = htiHit;
					
					bSelChange = TRUE;
				}
				else if (htiHit) // !bCtrl && !bShift
				{
					// select item if not already
					if (!TSH().HasItem(htiHit))
					{
						SelectItem(htiHit, FALSE, SC_BYMOUSE);
						bSelChange = TRUE;
					}

					if (nHitFlags & TVHT_ONITEMLABEL)
					{
						// if we didn't previously have the focus then we save
						// off the clicked item so the button up handler will
						// not initiate a label edit
						if (bSelChange || !bHadFocus)
							m_htiLastHandledLBtnDown = htiHit;
					}
					else if (nHitFlags & TVHT_ONITEMICON)
					{
						if (!m_bReadOnly && !SelectionHasLocked(FALSE, TRUE))
						{
							// save item handle so we don't re-handle in LButtonUp handler
							m_htiLastHandledLBtnDown = htiHit;
							bColClick = TRUE;
						}
					}
					else
					{
						// save item handle so we don't re-handle in LButtonUp handler
						m_htiLastHandledLBtnDown = htiHit;
						bColClick = TRUE;
					}
				}
				
				// update anchor
				if (htiHit && !bShift)
					TSH().SetAnchor(htiHit);

				nAction = SC_BYMOUSE;
			}
			break;
			
		case WM_LBUTTONUP: // --------------------------------------------------------------------------
			{
				// always clear the last handled item
				HTREEITEM htiLastHandledLBtnDown = m_htiLastHandledLBtnDown;
				m_htiLastHandledLBtnDown = NULL;			
				
				BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
				BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
				
				if (!bCtrl && !bShift)
				{
					UINT nHitFlags = 0;
					CPoint ptCursor(lp);
					HTREEITEM htiHit = HitTestItem(ptCursor, &nHitFlags);
					
					// don't reprocess items already handled in the button down handler
					if ((nHitFlags & (TVHT_ONITEMBUTTON | TVHT_ONITEMICON | TVHT_ONITEMSTATEICON)) || 
						!htiHit || (htiHit == htiLastHandledLBtnDown))
					{
						break;
					}

					if (TSH().HasItem(htiHit))
					{
						int nSelCount = TSH().GetCount();
						ASSERT (nSelCount);
						
						if (!m_bReadOnly &&
							!SelectionHasLocked(FALSE, FALSE) && 
							(nHitFlags & TVHT_ONITEMLABEL) && 
							(nSelCount == 1) && 
							(htiLastHandledLBtnDown == NULL))
						{
							BeginLabelEditTimer();
						}
						else if (nSelCount > 1)
						{
							SelectItem(htiHit, TRUE, SC_BYMOUSE);
						}
						
						// update anchor
						TSH().SetAnchor(htiHit);
					}
				}
				
				nAction = SC_BYMOUSE;
			}
			break;
			
		case WM_LBUTTONDBLCLK: // --------------------------------------------------------------------------
			EndLabelEditTimer();
			bColDblClk = TRUE;
			break;

		case WM_VSCROLL:
			{
 				CHoldHScroll hhs(m_tcTasks);
 				CDisableTreeTips dtt(m_tcTasks, FALSE);
				
				return CTDLTaskCtrlBase::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;
		} 
		// -----------------------------------------------------------------------------------------------

		// Handle selection change before column click/dblclk
		if (bSelChange)
		{
			SyncColumnSelectionToTasks();
			
			if (!TSH().Matches(lstPrevSel))
			{
				ASSERT(nAction != SC_UNKNOWN);
				NotifyParentSelChange(nAction);
			}
		}

		if (bColClick)
		{
			if (HandleClientColumnClick(lp, FALSE))
				return 0L; // eat

			// else
			m_htiLastHandledLBtnDown = NULL;
		}
		else if (bColDblClk)
		{
			if (HandleClientColumnClick(lp, TRUE))
				return 0L; // eat
		}
	}
	else if (hRealWnd == m_lcColumns)
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			// Selecting or de-selecting a lot of items can be slow
			// because OnListSelectionChange is called once for each.
			// Base class handles simple click de-selection so we
			// handle bulk selection here
			if (Misc::IsKeyPressed(VK_SHIFT)) // bulk-selection
			{
				CTLSHoldResync hr(*this);
				
				int nAnchor = m_lcColumns.GetSelectionMark();

				if (!Misc::IsKeyPressed(VK_CONTROL))
					DeselectAll();

				// Add new items to tree and list
				TDC_COLUMN nColID = TDCC_NONE;
				int nHit = HitTestColumnsItem(lp, TRUE, nColID);

				int nFrom = (nAnchor < nHit) ? nAnchor : nHit;
				int nTo = (nAnchor < nHit) ? nHit : nAnchor;

				for (int nItem = nFrom; nItem <= nTo; nItem++)
				{
					HTREEITEM hti = GetTreeItem(m_tcTasks, m_lcColumns, nItem);
					ASSERT(hti);

					TSH().AddItem(hti, FALSE);
					m_lcColumns.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);				
				}

				return 0; // eat it
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				// let parent handle any focus changes first
				m_lcColumns.SetFocus();

				// don't let the selection to be set to -1
				// when clicking below the last item
				int nHit = m_lcColumns.HitTest(lp);
				HTREEITEM hti = CTreeListSyncer::GetTreeItem(m_tcTasks, m_lcColumns, nHit);

				if (hti == NULL)
					return 0L; // eat it

				// Allow double-clicking to expand tree item 
				// if not on an 'active' column
				ASSERT(hti == GetTreeSelItem(m_tcTasks));

				if (TCH().TreeCtrl().ItemHasChildren(hti))
				{
					ExpandItem(hti, !TCH().IsItemExpanded(hti));
					return 0L; // eat it
				}
			}
			break;

		case WM_KEYDOWN:
			switch (wp)
			{
				case VK_MULTIPLY:
				case VK_SUBTRACT:
					// Forward on to tree for handling
					m_tcTasks.PostMessage(msg, wp, lp);
					return 0L; // eat it
			}
			break;
		}
	}
	
	return CTDLTaskCtrlBase::ScWindowProc(hRealWnd, msg, wp, lp);
}

void CTDLTaskTreeCtrl::ExpandList(HTREEITEM hti)
{
	int nNextIndex = 0;
	
	if (hti && (hti != TVI_ROOT))
	{
		nNextIndex = FindListItem(m_lcColumns, (DWORD)hti);
		ASSERT(nNextIndex != -1);

		nNextIndex++;
	}

	ExpandList(hti, nNextIndex);
}

void CTDLTaskTreeCtrl::ExpandList(HTREEITEM hti, int& nNextIndex)
{
	CTDLTaskCtrlBase::ExpandList(m_lcColumns, m_tcTasks, hti, nNextIndex);
}

void CTDLTaskTreeCtrl::CollapseList(HTREEITEM hti)
{
	CTDLTaskCtrlBase::CollapseList(m_lcColumns, m_tcTasks, hti);
}

void CTDLTaskTreeCtrl::GetWindowRect(CRect& rWindow, BOOL bWithHeader) const
{
	// always
	GetWindowRect(rWindow);

	if (!bWithHeader)
	{
		CRect rTree;
		m_tcTasks.GetWindowRect(rTree);

		rWindow.top = rTree.top;
	}
}

DWORD CTDLTaskTreeCtrl::GetColumnItemTaskID(int nItem) const
{
	return GetTaskID((HTREEITEM)m_lcColumns.GetItemData(nItem));
}

BOOL CTDLTaskTreeCtrl::GetItemTitleRect(const NMCUSTOMDRAW& nmcd, TDC_LABELRECT nArea, CRect& rect, CDC* pDC, LPCTSTR szTitle) const
{
	return GetItemTitleRect((HTREEITEM)nmcd.dwItemSpec, nArea, rect, pDC, szTitle);
}

BOOL CTDLTaskTreeCtrl::GetItemTitleRect(HTREEITEM hti, TDC_LABELRECT nArea, CRect& rect, CDC* pDC, LPCTSTR szTitle) const
{
	ASSERT(hti);

	// basic title rect
	m_tcTasks.GetItemRect(hti, rect, TRUE);
	int nHdrWidth = m_hdrTasks.GetItemWidth(0);

	switch (nArea)
	{
	case TDCTR_TEXT:
		if (pDC && szTitle)
		{
			rect.right = (rect.left + pDC->GetTextExtent(szTitle).cx);
			rect.right = min(rect.right, nHdrWidth);
		}
		else
		{
			ASSERT(!pDC && !szTitle);
			rect.right = nHdrWidth;
		}
		return TRUE;

	case TDCTR_BKGND:
		if (GetItemTitleRect(hti, TDCTR_TEXT, rect)) // recursive call
		{
			rect.left -= TITLE_BORDER_OFFSET;
			return TRUE;
		}
		break;

	case TDCTR_EDIT:
		if (GetItemTitleRect(hti, TDCTR_BKGND, rect)) // recursive call
		{
			rect.top--;
			
			// return in screen coords
			m_tcTasks.ClientToScreen(rect);
			return TRUE;
		}
		break;
	}
	
	ASSERT(0);
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::GetLabelEditRect(CRect& rLabel) const
{
	return GetItemTitleRect(GetSelectedItem(), TDCTR_EDIT, rLabel);
}

BOOL CTDLTaskTreeCtrl::MoveSelection(TDC_MOVETASK nDirection)
{
	if (!CanMoveSelection(nDirection))
		return FALSE;

	HTREEITEM htiDestParent = NULL, htiDestAfter = NULL;
	VERIFY(GetInsertLocation(nDirection, htiDestParent, htiDestAfter));

	if (!MoveSelection(htiDestParent, htiDestAfter, FALSE))
		return FALSE;

	// if moving up or down make sure we scroll ahead a bit
	switch (nDirection)
	{
	case TDCM_UP:
		TCH().SetMinDistanceToEdge(TSH().GetFirstItem(), TCHE_TOP, 2);
		break;

	case TDCM_DOWN:
		TCH().SetMinDistanceToEdge(TSH().GetLastItem(), TCHE_BOTTOM, 2);
		break;
	}

	return TRUE;
}

// External version - DON'T CALL INTERNALLY - Except from above
BOOL CTDLTaskTreeCtrl::MoveSelection(HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, BOOL bEnsureVisible)
{
	CAutoFlag af(m_bMovingItem, TRUE);
	// No 'hold redraw' here

	CHTIList moved;
	HTREEITEM htiFirst = MoveSelectionRaw(htiDestParent, htiDestPrevSibling, moved);

	if (htiFirst == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	TSH().SetItems(moved, TSHS_SELECT, FALSE);
	TSH().SetAnchor(htiFirst);
	TCH().SelectItem(htiFirst);

	ResyncListToTreeSelection(m_tcTasks, moved, htiFirst);

	// make sure first moved item is visible
	if (bEnsureVisible)
	{
		CHoldHScroll hhs(m_tcTasks);
		TCH().EnsureItemVisible(htiFirst, FALSE, FALSE);
	}

	// No need to notify parent of selection change
	// because logically the selection hasn't changed
	return TRUE;
}

// Internal version
HTREEITEM CTDLTaskTreeCtrl::MoveSelectionRaw(HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, CHTIList& moved)
{
	ASSERT(m_bMovingItem);
	ASSERT(HasSelection());

	HTREEITEM htiFirst = NULL;

	// expand the destination ahead of the move
	m_tcTasks.Expand(htiDestParent, TVE_EXPAND);
	ExpandList(htiDestParent);

	// Get selected tasks ordered, and without duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE, TRUE);

	TSH().RemoveAll(FALSE, FALSE); // no redraw
	TCH().SelectItem(NULL);

	// move the tree items
	POSITION pos = selection.GetHeadPosition();
	HTREEITEM htiAfter = htiDestPrevSibling;

	if (htiAfter == NULL)
		htiAfter = TVI_FIRST;

	moved.RemoveAll();

	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		hti = MoveItemRaw(hti, htiDestParent, htiAfter);

		moved.AddTail(hti);
		htiAfter = hti;
	}
	ASSERT(moved.GetCount());

	return (moved.GetCount() ? moved.GetHead() : NULL);
}

// External version - DON'T CALL INTERNALLY
HTREEITEM CTDLTaskTreeCtrl::MoveItem(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling)
{
	CAutoFlag af(m_bMovingItem, TRUE);

	return MoveItemRaw(hti, htiDestParent, htiDestPrevSibling);
}

// Internal version
HTREEITEM CTDLTaskTreeCtrl::MoveItemRaw(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling)
{
	ASSERT(hti);
	ASSERT(m_bMovingItem);

	// make sure the destination parent is expanded so that the new items 
	// get automatically handled by CTreeListSyncer
	if (htiDestParent)
		m_tcTasks.SetItemState(htiDestParent, TVIS_EXPANDED, TVIS_EXPANDED);
	
	// Remove the existing task and its children from the master map
	// Note: Reference task list remains unaffected because it just
	// stores task IDs and these won't have changed
	m_mapTaskIDToHTI.RemoveItem(m_tcTasks, hti);

	// do the move
	hti = TCH().MoveTree(hti, htiDestParent, htiDestPrevSibling, TRUE, TRUE);

	// Add the 'new' task and its children to the map
	m_mapTaskIDToHTI.AddItem(m_tcTasks, hti);

	// return the new tree item
	return hti;
}

BOOL CTDLTaskTreeCtrl::CanMoveSelection(TDC_MOVETASK nDirection) const
{
	if (!IsSelectedTaskMoveEnabled(TDCM_NONDRAG))
		return FALSE;
	
	// Get selected tasks without duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE);
	
	if (!selection.GetCount() || !TSH().ItemsAreAllSiblings(selection))
		return FALSE;
	
	// must be able to move all items
	POSITION pos = selection.GetHeadPosition();
	
	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		
		if (!CanMoveItem(hti, nDirection))
			return FALSE;
	}
	
	return TRUE;
}

BOOL CTDLTaskTreeCtrl::IsSelectedTaskMoveEnabled(TDC_MOVEMETHOD nMethod) const
{
	if (IsReadOnly())
		return FALSE;

	switch (nMethod)
	{
	case TDCM_LEFTDRAG:
		{
			// Copying
			if (Misc::ModKeysArePressed(MKS_CTRL))
				return TRUE;

			// References
			if (Misc::ModKeysArePressed(MKS_CTRL | MKS_SHIFT) || Misc::ModKeysArePressed(MKS_ALT))
				return TRUE;
		}
		// fall through

	case TDCM_NONDRAG:
		{
			// Prevent moving locked tasks unless references
			if (SelectionHasLocked(FALSE, TRUE))
				return FALSE;

			// Prevent moving subtasks of locked parent unless parent is reference
			if (SelectionHasLockedParent(TRUE))
				return FALSE;
		}
		break;

	case TDCM_RIGHTDRAG:
		// Resolved at time of drop
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CTDLTaskTreeCtrl::IsValidSelectedTaskMoveTarget(DWORD dwTargetID, DD_DROPEFFECT nDrop) const
{
	// Root is always valid
	if (dwTargetID == 0)
		return TRUE;
	
	if (m_data.IsTaskReference(dwTargetID))
	{
		// Dropping anything on references 'as references' is okay
		if (nDrop == DD_DROPEFFECT_LINK)
			return TRUE;

		// Dropping only references on references is okay
		if (!SelectionHasNonReferences())
			return TRUE;

		// else trying to drop non-references on references
		return FALSE;
	}
	
	// else target must be a real unlocked task
	return !m_calculator.IsTaskLocked(dwTargetID);
}

DD_DROPEFFECT CTDLTaskTreeCtrl::GetSelectedTaskDropEffect(DWORD dwTargetID, BOOL bLeftDrag) const
{
	ASSERT(!IsReadOnly());
	
	if (!IsSelectedTaskMoveEnabled(bLeftDrag ? TDCM_LEFTDRAG : TDCM_RIGHTDRAG))
		return DD_DROPEFFECT_NONE;
	
	DD_DROPEFFECT nEffect = DD_DROPEFFECT_MOVE; // default
	
	if (bLeftDrag)
	{
		// References can only accept references
		if (m_data.IsTaskReference(dwTargetID) && SelectionHasNonReferences())
		{
			nEffect = DD_DROPEFFECT_LINK;
		}
		else if (Misc::ModKeysArePressed(MKS_CTRL))
		{
			nEffect = DD_DROPEFFECT_COPY;
		}
		else if (Misc::ModKeysArePressed(MKS_CTRL | MKS_SHIFT) || Misc::ModKeysArePressed(MKS_ALT))
		{
			nEffect = DD_DROPEFFECT_LINK;
		}
	}
	
	if (!IsValidSelectedTaskMoveTarget(dwTargetID, nEffect))
		nEffect = DD_DROPEFFECT_NONE;
	
	return nEffect;
}

BOOL CTDLTaskTreeCtrl::SelectionHasLocked(BOOL bCheckChildren, BOOL bTreatRefsAsUnlocked) const
{
	BOOL bLocked = CTDLTaskCtrlBase::SelectionHasLocked(bTreatRefsAsUnlocked);

	if (bLocked || !bCheckChildren)
		return bLocked;

	// Check children of selection
	POSITION pos = GetFirstSelectedTaskPos();

	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		if (TaskHasLockedSubtasks(dwTaskID, bTreatRefsAsUnlocked))
			return TRUE;
	}

	return FALSE; // All subtasks were unlocked
}

BOOL CTDLTaskTreeCtrl::TaskHasLockedSubtasks(DWORD dwTaskID, BOOL bTreatRefsAsUnlocked) const
{
	const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);

	for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
	{
		DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubtask);

		if (bTreatRefsAsUnlocked && m_data.IsTaskReference(dwTaskID))
		{
			// References can only contain other references
			// so no need to check children
			continue;
		}
		
		// Check this task and its children
		if (m_data.IsTaskLocked(dwSubtaskID) || TaskHasLockedSubtasks(dwSubtaskID, bTreatRefsAsUnlocked))
			return TRUE;
	}

	return FALSE; // All subtasks were unlocked
}

BOOL CTDLTaskTreeCtrl::CanSelectTasksInHistory(BOOL bForward) const 
{ 
	return (bForward ? TSH().HasNextSelection() : TSH().HasPrevSelection()); 
}

BOOL CTDLTaskTreeCtrl::SelectTasksInHistory(BOOL bForward) 
{ 
	BOOL bSelected = (bForward ? TSH().NextSelection() : TSH().PrevSelection());

	if (bSelected)
		SyncColumnSelectionToTasks();

	return bSelected;
}

BOOL CTDLTaskTreeCtrl::CanMoveItem(HTREEITEM hti, TDC_MOVETASK nDirection) const
{
	if (!hti)
		return FALSE;

	// Can't move a real locked task
	DWORD dwTaskID = GetTaskID(hti);
	BOOL bTaskIsRef = m_data.IsTaskReference(dwTaskID);

	if (!bTaskIsRef && m_calculator.IsTaskLocked(dwTaskID))
		return FALSE;

	// Can't move a subtask of a real locked task
	HTREEITEM htiParent = m_tcTasks.GetParentItem(hti);
	BOOL bParentIsRoot = (!htiParent || (htiParent == TVI_ROOT));

	if (!bParentIsRoot)
	{
		DWORD dwParentID = GetTaskID(htiParent);

		if (!m_data.IsTaskReference(dwParentID) && m_calculator.IsTaskLocked(dwParentID))
			return FALSE;
	}
	
	switch (nDirection)
	{
	case TDCM_DOWN:
		return (m_tcTasks.GetNextItem(hti, TVGN_NEXT) != NULL);
		
	case TDCM_UP:
		return (m_tcTasks.GetNextItem(hti, TVGN_PREVIOUS) != NULL);
		
	case TDCM_LEFT:
		{
			// If our current parent is root then we've no place left to go
			if (bParentIsRoot)
				return FALSE;

			// Our parent's parent becomes our parent
			HTREEITEM htiNewParent = m_tcTasks.GetParentItem(htiParent);

			// Root is always unlocked
			if (!htiNewParent || (htiNewParent == TVI_ROOT))
				return TRUE;

			// Cannot be locked or must be a reference if we are a reference
			DWORD dwNewParentID = GetTaskID(htiNewParent);

			if (m_data.IsTaskReference(dwNewParentID))
				return bTaskIsRef;

			return !m_data.IsTaskLocked(dwNewParentID);
		}
		break;
		
	case TDCM_RIGHT:
		{
			// must have a prior sibling (which will become the parent)
			// which is not locked unless both the task and sibling are references
			HTREEITEM htiNewParent = m_tcTasks.GetNextItem(hti, TVGN_PREVIOUS);

			if (!htiNewParent)
				return FALSE;

			DWORD dwNewParentID = GetTaskID(htiNewParent);

			if (m_data.IsTaskReference(dwNewParentID))
				return bTaskIsRef;

			return !m_data.IsTaskLocked(dwNewParentID);
		}
		break;
	}
	
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::InvalidateSelection(BOOL bUpdate)
{
	POSITION pos = TSH().GetFirstItemPos();
	BOOL bInvalidated = FALSE;
	
	while (pos)
	{
		HTREEITEM htiSel = TSH().GetNextItem(pos);
		DWORD dwTaskID = GetTaskID(htiSel);

		// if the selected item is a reference or a task with references
		// then we invalidate the whole tree
		if (m_data.IsTaskReference(dwTaskID) || TaskHasReferences(dwTaskID))
		{
			m_tcTasks.Invalidate();
			bInvalidated = TRUE;
		}
		else if (InvalidateItem(htiSel, FALSE))
		{
			bInvalidated = TRUE;
		}
	}
	
	if (bInvalidated && bUpdate)
		m_tcTasks.UpdateWindow();

	return bInvalidated;
}

BOOL CTDLTaskTreeCtrl::InvalidateTask(DWORD dwTaskID, BOOL bUpdate)
{
	if (dwTaskID == 0)
		return TRUE; // nothing to do
	
	HTREEITEM hti = m_mapTaskIDToHTI.GetItem(dwTaskID);

	if (hti)
		return InvalidateItem(hti, bUpdate);

	return FALSE;
}

BOOL CTDLTaskTreeCtrl::InvalidateItem(HTREEITEM hti, BOOL bUpdate)
{
	// draw tree first
	if (TCH().InvalidateItem(hti))
	{
		if (bUpdate)
			m_tcTasks.UpdateWindow();

		// redraw columns
		int nItem = FindListItem(m_lcColumns, (DWORD)hti);
		
		return CTDLTaskCtrlBase::InvalidateColumnItem(nItem, bUpdate);
	}

	//else
	return FALSE;
}

int CTDLTaskTreeCtrl::CacheSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs) const
{
	if (GetSelectedTaskIDs(cache.aSelTaskIDs, cache.dwFocusedTaskID, FALSE) > 0)
	{
		cache.dwFirstVisibleTaskID = GetTaskID(m_tcTasks.GetFirstVisibleItem());
 
		// we prefer the 'down' breadcrumbs because this way the 
		// focused item appears to stay still, just like when we delete items
		// but in case the item in question is last, we also get the 'up' breadcrumbs
		cache.aBreadcrumbs.RemoveAll();

		if (bIncBreadcrumbs)
		{
			HTREEITEM htiFocus = m_tcTasks.GetSelectedItem();
		
			if (htiFocus == NULL)
				htiFocus = GetItem(cache.dwFocusedTaskID);
		
			m_find.GetTaskBreadcrumbs(htiFocus, cache.aBreadcrumbs, TCFBC_VISIBLEONLY);
			m_find.GetTaskBreadcrumbs(htiFocus, cache.aBreadcrumbs, TCFBC_VISIBLEONLY | TCFBC_APPEND | TCFBC_UP);
		}
	}

	return !cache.IsEmpty();
}

BOOL CTDLTaskTreeCtrl::RestoreSelection(const TDCSELECTIONCACHE& cache)
{
	if (!cache.IsEmpty())
	{
		HTREEITEM htiFocus = GetItem(cache.dwFocusedTaskID);

		if (htiFocus == NULL)
		{
			int nID = cache.aBreadcrumbs.GetSize();
			
			while (nID-- && !htiFocus)
				htiFocus = GetItem(cache.aBreadcrumbs[nID]);
		}

		if (SelectTasks(cache.aSelTaskIDs) || htiFocus)
		{
			// restore focused item
			ASSERT(htiFocus);

			MultiSelectItem(htiFocus, TSHS_SELECT);
			TSH().SetAnchor(htiFocus);
			TCH().SelectItem(htiFocus);

			// restore scroll pos
			HTREEITEM htiFirstVis = NULL;

			if (cache.dwFirstVisibleTaskID)
				htiFirstVis = GetItem(cache.dwFirstVisibleTaskID);

			if (htiFirstVis == NULL)
				htiFirstVis = m_tcTasks.GetChildItem(NULL);

			m_tcTasks.SelectSetFirstVisible(htiFirstVis);
			EnsureSelectionVisible();

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::IsItemSelected(HTREEITEM hti, BOOL bSingly) const 
{ 
	if (bSingly && !TSH().HasSingleSelection())
		return FALSE;

	return TSH().HasItem(hti);
}

BOOL CTDLTaskTreeCtrl::IsTaskSelected(DWORD dwTaskID, BOOL bSingly) const 
{ 
	if (bSingly && !TSH().HasSingleSelection())
		return FALSE;

	return TSH().HasItem(dwTaskID);
}

int CTDLTaskTreeCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue) const
{
	DWORD dwFocusID;
	int nNumIDs = GetSelectedTaskIDs(aTaskIDs, dwFocusID, FALSE);

	// extra processing
	if (nNumIDs && bTrue)
		m_data.GetTrueTaskIDs(aTaskIDs);

	return nNumIDs;
}

int CTDLTaskTreeCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID, BOOL bRemoveChildDupes) const
{
	aTaskIDs.RemoveAll();
	dwFocusedTaskID = 0;

	if (GetSelectedCount())
	{
		// get selected tasks ordered with/out duplicate subtasks
		CHTIList selection;
		TSH().CopySelection(selection, bRemoveChildDupes, TRUE);

		TDCGETTASKS filter(TDCGT_ALL, 0);
		POSITION pos = selection.GetHeadPosition();
	
		while (pos)
			aTaskIDs.Add(GetTaskID(selection.GetNext(pos)));

		// focused item
		HTREEITEM htiFocus = m_tcTasks.GetSelectedItem();

		if (htiFocus)
		{
			dwFocusedTaskID = GetTaskID(htiFocus);
		}
		else if (aTaskIDs.GetSize())
		{
			dwFocusedTaskID = aTaskIDs[0];
		}
	}
	
	return (aTaskIDs.GetSize());
}

BOOL CTDLTaskTreeCtrl::MultiSelectTask(DWORD dwTaskID, BOOL bSelected)
{
	if (dwTaskID == 0)
		return FALSE;

	return MultiSelectItem(GetItem(dwTaskID), (bSelected ? TSHS_SELECT : TSHS_DESELECT), TRUE);
}

BOOL CTDLTaskTreeCtrl::SelectTasks(const CDWordArray& aTaskIDs, BOOL bTrue)
{
	// sanity check
	ASSERT(aTaskIDs.GetSize());
	
	if (!aTaskIDs.GetSize())
		return FALSE;
	
	// convert references to true tasks
	CDWordArray aTrueTaskIDs;
	aTrueTaskIDs.Copy(aTaskIDs);
	
	if (bTrue)
		m_data.GetTrueTaskIDs(aTrueTaskIDs);

	// clear existing selection and add to history
	TSH().RemoveAll();

	// select tasks in one hit
	int nTask = aTrueTaskIDs.GetSize();
	BOOL bSel = FALSE;
	
	while (nTask--)
	{
		HTREEITEM hti = GetItem(aTrueTaskIDs[nTask]);

		if (hti)
			bSel |= TSH().SetItem(hti, TSHS_SELECT, TRUE);
	}
	
	if (bSel || !aTrueTaskIDs.GetSize())
	{
		TSH().FixupTreeSelection();

		SyncColumnSelectionToTasks();
		UpdateSelectedTaskPath();
		NotifyParentSelChange();
		
		EnsureSelectionVisible();
		ExpandList();
	}
	
	return bSel;
}

BOOL CTDLTaskTreeCtrl::MultiSelectItem(HTREEITEM hti, TSH_SELECT nState, BOOL bRedraw) 
{ 
	if (TSH().SetItem(hti, nState, bRedraw))
	{
		TSH().FixupTreeSelection();

		SyncColumnSelectionToTasks();
		UpdateSelectedTaskPath();
		NotifyParentSelChange();
		
		if (bRedraw)
			RedrawColumns();
		
		return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::MultiSelectItems(HTREEITEM htiFrom, HTREEITEM htiTo, TSH_SELECT nState, BOOL bRedraw)
{
	BOOL bTreeSelChanged = FALSE;
	
	if (TSH().SetItems(htiFrom, htiTo, nState, bRedraw))
	{
		// avoid calling UpdateControls again if it was called 
		// because the tree selection changed inside FixupTreeSelection
		bTreeSelChanged = TSH().FixupTreeSelection();
		
		if (bRedraw)
			RedrawColumns();
		
		SyncColumnSelectionToTasks();
		UpdateSelectedTaskPath();
		NotifyParentSelChange();
	}
	
	return bTreeSelChanged;
}

DWORD CTDLTaskTreeCtrl::GetTaskParentID(HTREEITEM hti) const
{
	if (!hti || hti == TVI_ROOT || hti == TVI_FIRST || hti == TVI_LAST)
		return 0;
	
	return GetTaskID(m_tcTasks.GetParentItem(hti));
}

HTREEITEM CTDLTaskTreeCtrl::InsertItem(DWORD dwTaskID, HTREEITEM htiParent, HTREEITEM htiAfter)
{
	HTREEITEM htiNew = TCH().InsertItem(LPSTR_TEXTCALLBACK, 
										I_IMAGECALLBACK, 
										I_IMAGECALLBACK, 
										dwTaskID, // lParam
										htiParent, 
										(htiAfter ? htiAfter : TVI_FIRST),
										(htiParent == NULL), // bold top-level items
										FALSE);

	m_mapTaskIDToHTI[dwTaskID] = htiNew;

	// Keep track of references
	DWORD dwRefID = m_data.GetTaskReferenceID(dwTaskID);

	if (dwRefID)
		m_mapReferenceTaskIDs.Add(dwTaskID);

	return htiNew;
}

BOOL CTDLTaskTreeCtrl::DeleteItem(HTREEITEM hti, BOOL bDeleteReferencesToItem)
{ 
	// Remove task from reference list 
	DWORD dwTaskID = GetTaskID(hti);
	BOOL bRefTask = m_mapReferenceTaskIDs.Remove(dwTaskID);

	// Remove item and its children from the master list,
	// including any contained references
	m_mapTaskIDToHTI.RemoveItem(m_tcTasks, hti);
	
	// Remove any references no longer in the master list
	POSITION pos = m_mapReferenceTaskIDs.GetStartPosition();

	while (pos)
	{
		DWORD dwRefID = m_mapReferenceTaskIDs.GetNext(pos);
		
		if (!m_mapTaskIDToHTI.HasItem(dwRefID))
			m_mapReferenceTaskIDs.Remove(dwRefID);
	}

	// Now delete outstanding references to hti
	if (!bRefTask && bDeleteReferencesToItem)
	{
		CHTIList listRefs;
		GetReferencesToTask(dwTaskID, listRefs);

		POSITION pos = listRefs.GetHeadPosition();

		while (pos)
			DeleteItem(listRefs.GetNext(pos), TRUE); // RECURSIVE CALL
	}

	// Finally delete the tree item
	return m_tcTasks.DeleteItem(hti);
}

int CTDLTaskTreeCtrl::GetReferencesToTask(DWORD dwTaskID, CHTIList& listRefs, BOOL bAppend) const
{
	if (!bAppend)
		listRefs.RemoveAll();

	POSITION pos = m_mapReferenceTaskIDs.GetStartPosition();

	while (pos)
	{
		DWORD dwRefID = m_mapReferenceTaskIDs.GetNext(pos);
		
		if (m_data.IsReferenceToTask(dwRefID, dwTaskID))
		{
			// Can't have references to references
			ASSERT(!m_data.IsTaskReference(dwTaskID));

			HTREEITEM htiRef = m_mapTaskIDToHTI.GetItem(dwRefID);

			if (htiRef)
				listRefs.AddTail(htiRef);
			else
				ASSERT(0); // orphan reference
		}
	}

	return listRefs.GetCount();
}

BOOL CTDLTaskTreeCtrl::TaskHasReferences(DWORD dwTaskID) const
{
	POSITION pos = m_mapReferenceTaskIDs.GetStartPosition();

	while (pos)
	{
		DWORD dwRefID = m_mapReferenceTaskIDs.GetNext(pos);
		
		if (m_data.IsReferenceToTask(dwRefID, dwTaskID))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskTreeCtrl::GetInsertLocation(TDC_MOVETASK nDirection, DWORD& dwDest, DWORD& dwDestAfter) const
{
	HTREEITEM htiDest = NULL, htiDestAfter = NULL;

	if (GetInsertLocation(nDirection, htiDest, htiDestAfter))
	{
		dwDest = GetTaskID(htiDest);
		dwDestAfter = GetTaskID(htiDestAfter);

		return TRUE;
	}

	// else
	dwDest = dwDestAfter = 0;
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::GetInsertLocation(TDC_MOVETASK nDirection, HTREEITEM& htiDest, HTREEITEM& htiDestAfter) const
{
	htiDest = htiDestAfter = NULL;
	
	// Get selected tasks ordered, and without duplicate subtasks
	// to ensure we get the correct upper/lowermost items
	CHTIList selection;
	TSH().CopySelection(selection, TRUE, TRUE);

	HTREEITEM htiTopSel = selection.GetHead();
	HTREEITEM htiBotSel = selection.GetTail();

#ifdef _DEBUG
	//DWORD dwSelID = GetTaskID(htiSel);
#endif
	
	switch (nDirection)
	{
	case TDCM_DOWN:	
		htiDestAfter = m_tcTasks.GetNextItem(htiBotSel, TVGN_NEXT);
		htiDest = m_tcTasks.GetParentItem(htiDestAfter);
		break;
		
	case TDCM_LEFT:
		htiDestAfter = m_tcTasks.GetParentItem(htiTopSel);
		htiDest = m_tcTasks.GetParentItem(htiDestAfter);
		break;
		
	case TDCM_UP:
		htiDest = m_tcTasks.GetParentItem(htiTopSel);
		
		// we need item before previous item to copy *after*
		htiDestAfter = m_tcTasks.GetNextItem(htiTopSel, TVGN_PREVIOUS);
		htiDestAfter = m_tcTasks.GetNextItem(htiDestAfter, TVGN_PREVIOUS);
		
		if (htiDestAfter == NULL)
			htiDestAfter = TVI_FIRST;
		break;
		
	case TDCM_RIGHT:
		htiDest = m_tcTasks.GetNextItem(htiTopSel, TVGN_PREVIOUS);
		htiDestAfter = TCH().GetLastChildItem(htiDest);
		
		if (htiDestAfter == NULL)
			htiDestAfter = TVI_FIRST;
		break;
	}
	
	return (htiDestAfter != NULL);
}

BOOL CTDLTaskTreeCtrl::GetInsertLocation(TDC_INSERTWHERE nWhere, HTREEITEM& htiDest, HTREEITEM& htiDestAfter) const
{
	HTREEITEM htiSel = GetSelectedItem();
	htiDest = htiDestAfter = NULL;
	
	// handle no selected item
	if (!htiSel)
	{
		switch (nWhere)
		{
		case TDC_INSERTATTOP:
		case TDC_INSERTATTOPOFSELTASKPARENT:
		case TDC_INSERTATTOPOFSELTASK:
		case TDC_INSERTBEFORESELTASK:
			htiDest = TVI_ROOT;
			htiDestAfter = TVI_FIRST;
			break;
			
		case TDC_INSERTATBOTTOM:
		case TDC_INSERTATBOTTOMOFSELTASKPARENT:
		case TDC_INSERTATBOTTOMOFSELTASK:
		case TDC_INSERTAFTERSELTASK:
			htiDest = TVI_ROOT;
			htiDestAfter = TVI_LAST; 
			break;
		}
	}
	else // determine the actual pos to insert
	{
		switch (nWhere)
		{
		case TDC_INSERTATTOP:
			htiDest = TVI_ROOT;
			htiDestAfter = TVI_FIRST;
			break;
			
		case TDC_INSERTATBOTTOM:
			htiDest = TVI_ROOT;
			htiDestAfter = TVI_LAST; 
			break;
			
		case TDC_INSERTATTOPOFSELTASKPARENT:
			htiDest = m_tcTasks.GetParentItem(htiSel);
			htiDestAfter = TVI_FIRST;
			break;
			
		case TDC_INSERTATBOTTOMOFSELTASKPARENT:
			htiDest = m_tcTasks.GetParentItem(htiSel);
			htiDestAfter = TVI_LAST;
			break;
			
		case TDC_INSERTAFTERSELTASK:
			htiDest = m_tcTasks.GetParentItem(htiSel);
			htiDestAfter = htiSel;
			break;
			
		case TDC_INSERTBEFORESELTASK:
			htiDest = m_tcTasks.GetParentItem(htiSel);
			htiDestAfter = m_tcTasks.GetNextItem(htiSel, TVGN_PREVIOUS);
			
			if (!htiDestAfter)
				htiDestAfter = TVI_FIRST;
			break;
			
		case TDC_INSERTATTOPOFSELTASK: // subtask
			htiDest = htiSel;
			htiDestAfter = TVI_FIRST;
			break;
			
		case TDC_INSERTATBOTTOMOFSELTASK: // subtask
			htiDest = htiSel;
			htiDestAfter = TVI_LAST; 
			break;
		}
	}
	
	return (htiDestAfter != NULL);
}

void CTDLTaskTreeCtrl::SaveState(CPreferences& prefs, const CString& sKey, BOOL bExpandedOnly) const
{
	if (!bExpandedOnly)
		CTDLTaskCtrlBase::SaveState(prefs, sKey);
	
	// ignore this if we have no tasks
	if (GetTaskCount() == 0)
		return;
	
	if (!sKey.IsEmpty())
	{
		CString sExpKey(sKey + _T("\\ExpandedItems"));

		// remove previous expanded state
		prefs.DeleteProfileSection(sExpKey);
		
		// save items themselves
		int nCount = SaveTreeExpandedState(prefs, sExpKey, NULL, 0);
		
		// save expanded count
		prefs.WriteProfileInt(sExpKey, _T("Count"), nCount);
		
		// and selected item
		prefs.WriteProfileInt(sKey, _T("SelItem"), GetSelectedTaskID());
	}
}

HTREEITEM CTDLTaskTreeCtrl::LoadState(const CPreferences& prefs, const CString& sKey, BOOL bExpandedOnly) 
{
	if (!bExpandedOnly)
		CTDLTaskCtrlBase::LoadState(prefs, sKey);

	CString sExpKey(sKey + _T("\\ExpandedItems"));
	int nCount = prefs.GetProfileInt(sExpKey, _T("Count"), 0);
	
	CDWordArray aExpanded;
	
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		CString sItem = Misc::MakeKey(_T("Item%d"), nItem);
		DWORD dwTaskID = (DWORD)prefs.GetProfileInt(sExpKey, sItem, 0);
		
		aExpanded.Add(dwTaskID);
	}
	
	SetExpandedTasks(aExpanded);
	
	// restore prev selected item
	DWORD dwSel = prefs.GetProfileInt(sKey, _T("SelItem"), 0);
	return GetItem(dwSel);
}

int CTDLTaskTreeCtrl::SaveTreeExpandedState(CPreferences& prefs, const CString& sKey, 
									 HTREEITEM hti, int nStart) const
{
	HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);
	int nCount = nStart;
	
	while (htiChild)
	{
		if (TCH().IsItemExpanded(htiChild) > 0)
		{
			CString sItem = Misc::MakeKey(_T("Item%d"), nCount);
			
			prefs.WriteProfileInt(sKey, sItem, (int)GetTaskID(htiChild));
			nCount++;
			
			// now its children
			nCount += SaveTreeExpandedState(prefs, sKey, htiChild, nCount);
		}
		
		htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
	}	
	
	return (nCount - nStart);
}

void CTDLTaskTreeCtrl::SetTasksImageList(HIMAGELIST hil, BOOL bState, BOOL bOn)
{
	TreeView_SetImageList(m_tcTasks, (bOn ? hil : NULL), (bState ? TVSIL_STATE : TVSIL_NORMAL));

	if (!bState && !bOn)
		RecalcItemHeights();
}

double CTDLTaskTreeCtrl::CalcSelectedTaskTimeEstimate(TDC_UNITS nUnits) const
{
	double dTime = 0.0;
	
	// get selected tasks without duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE);
	
	POSITION pos = selection.GetHeadPosition();
	
	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		DWORD dwTaskID = GetTaskID(hti);
		
		dTime += m_calculator.GetTaskTimeEstimate(dwTaskID, nUnits);
	}
	
	return dTime;
}

double CTDLTaskTreeCtrl::CalcSelectedTaskTimeSpent(TDC_UNITS nUnits) const
{
	double dTime = 0.0;
	
	// get selected tasks without duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE);
	
	POSITION pos = selection.GetHeadPosition();
	
	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		DWORD dwTaskID = GetTaskID(hti);
		
		dTime += m_calculator.GetTaskTimeSpent(dwTaskID, nUnits);
	}
	
	return dTime;
}

double CTDLTaskTreeCtrl::CalcSelectedTaskCost() const
{
	double dCost = 0.0;
	
	// get selected tasks without duplicate subtasks
	CHTIList selection;
	TSH().CopySelection(selection, TRUE);
	
	POSITION pos = selection.GetHeadPosition();
	
	while (pos)
	{
		HTREEITEM hti = selection.GetNext(pos);
		DWORD dwTaskID = GetTaskID(hti);
		
		dCost += m_calculator.GetTaskCost(dwTaskID);
	}
	
	return dCost;
}

void CTDLTaskTreeCtrl::RefreshItemBoldState(HTREEITEM hti, BOOL bAndChildren)
{
	if (hti && (hti != TVI_ROOT))
		TCH().SetItemBold(hti, m_tcTasks.GetParentItem(hti) == NULL);
	
	// children
	if (bAndChildren)
	{
		HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);
		
		while (htiChild)
		{
			RefreshItemBoldState(htiChild); // RECURSIVE CALL
			htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
}

void CTDLTaskTreeCtrl::SetModified(const CTDCAttributeMap& mapAttribIDs, BOOL bAllowResort)
{
	ASSERT (!mapAttribIDs.Has(TDCA_POSITION));

	if (mapAttribIDs.Has(TDCA_NEWTASK))
	{
		// Already handled in InsertTreeItem
		ASSERT(m_mapTaskIDToHTI.GetCount() == (int)m_tcTasks.GetCount());
	}

	if (mapAttribIDs.Has(TDCA_UNDO) ||
		mapAttribIDs.Has(TDCA_PASTE) ||
		mapAttribIDs.Has(TDCA_POSITION_DIFFERENTPARENT))
	{
		ASSERT(m_mapTaskIDToHTI.GetCount() == (int)m_tcTasks.GetCount());
		RefreshItemBoldState();
	}

	if (mapAttribIDs.Has(TDCA_DELETE) ||
		mapAttribIDs.Has(TDCA_ARCHIVE))
	{
		ASSERT(m_mapTaskIDToHTI.GetCount() >= (int)m_tcTasks.GetCount());
	}
	
	if (bAllowResort && ModsNeedResort(mapAttribIDs))
	{
		// if parent state is not calculated from its subtask values
		// can do an optimised sort of just the affected branches
		if (!ModsRequireFullResort(mapAttribIDs))
		{
			ResortSelectedTaskParents();
			bAllowResort = FALSE;
		}
	}

	CTDLTaskCtrlBase::SetModified(mapAttribIDs, bAllowResort);
}

BOOL CTDLTaskTreeCtrl::ModsRequireFullResort(const CTDCAttributeMap& mapAttribIDs) const
{
	if (mapAttribIDs.Has(TDCA_ALL) ||
		mapAttribIDs.Has(TDCA_UNDO) ||
		mapAttribIDs.Has(TDCA_PASTE) ||
		mapAttribIDs.Has(TDCA_POSITION_DIFFERENTPARENT))
	{
		return TRUE;
	}
	
	return m_calculator.HasCalculatedAttribute(mapAttribIDs, m_aCustomAttribDefs);
}

BOOL CTDLTaskTreeCtrl::DoSaveToImage(CBitmap& bmImage, COLORREF crDivider)
{
	// Position the splitter to match the widest item
	int nPrevSplitPos = -1;
	
	if (!HasHScrollBar(m_tcTasks))
	{
		nPrevSplitPos = GetSplitPos();

		int nSplitPos = CalcMaxVisibleTreeItemWidth(m_tcTasks);
		SetSplitPos(nSplitPos);
		RefreshSize();
	}
	
	BOOL bRes = CTDLTaskCtrlBase::DoSaveToImage(bmImage, crDivider);

	if (nPrevSplitPos != -1)
	{
		SetSplitPos(nPrevSplitPos);
		RefreshSize();
	}

	return bRes;
}
