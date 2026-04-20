// TDLTaskTreeCtrl.cpp: implementation of the CTDLTaskTreeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDlTaskTreeCtrl.h"
#include "todoctrldata.h"
#include "tdcstatic.h"
#include "tdcmsg.h"
#include "tdcmapping.h"
#include "tdcimagelist.h"

#include "..\shared\treectrlhelper.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\timehelper.h"
#include "..\shared\misc.h"
#include "..\shared\TreeDragDropHelper.h"
#include "..\shared\themed.h"
#include "..\shared\ScopedTimer.h"

#include "..\3rdParty\OSVersion.h"
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

const COLORREF COMMENTSCOLOR	= RGB(98, 98, 98);
const COLORREF ALTCOMMENTSCOLOR = RGB(164, 164, 164);

const int TITLE_BORDER_OFFSET	= 2;
const int ICON_SIZE				= GraphicsMisc::ScaleByDPIFactor(16);

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
								   const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
								   const CContentMgr& mgrContent)
	: 
	CTDLTaskCtrlBase(ilIcons, 
					 data, 
					 styles, 
					 tld, 
					 mapVisibleCols, 
					 aCustAttribDefs, 
					 mgrContent),

	CTreeDragDropRenderer(m_tsh, m_tcTasks),
	m_tsh(m_tcTasks, m_lcColumns),
	m_tch(m_tcTasks),
	m_bMovingItem(FALSE),
	m_reminders(*this),
	m_find(m_tch, data, aCustAttribDefs, m_reminders, mgrContent)
{
	// We handle multiple selection
	m_dwFlags &= ~TLSF_SYNCSELECTION;
}

CTDLTaskTreeCtrl::~CTDLTaskTreeCtrl()
{
}

///////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTDLTaskTreeCtrl, CTDLTaskCtrlBase)
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
							 TVS_EDITLABELS |
							 TVS_NONEVENHEIGHT),
							 rect,
							 pParentWnd,
							 IDC_TASKTREE);
}

// external version
BOOL CTDLTaskTreeCtrl::SelectItem(HTREEITEM hti) 
{ 
	return SelectItem(hti, TRUE, SC_UNKNOWN);
}

// internal version) 
BOOL CTDLTaskTreeCtrl::SelectItem(HTREEITEM hti, BOOL bSyncAndNotify, SELCHANGE_ACTION nBy)
{ 
	BOOL bSelChange = FALSE;
	BOOL bSelected = TSH().SelectSingleItem(hti, bSelChange);

	if (bSelChange && !hti)
		m_lcColumns.Invalidate();

	if (bSyncAndNotify)
		ProcessSelectionChange(bSelChange, nBy);
	
	return bSelected;
}

void CTDLTaskTreeCtrl::DeselectAll()
{
	if (!TSH().IsEmpty())
	{
		CHoldRedraw hr(*this);
		TSH().DeselectAll();
	}
}

BOOL CTDLTaskTreeCtrl::SelectAll(BOOL bVisibleOnly)
{ 
	// All selected tasks must be visible
	if (!bVisibleOnly)
		ExpandAll();

	if (TSH().AddAll())
	{
		ProcessSelectionChange(TRUE, SC_UNKNOWN);
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

	TSH().SetReadOnly(HasStyle(TDCS_READONLY));

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
	
	if (hti)
	{
		if (TCH().IsItemExpanded(hti) <= 0)
			return 0; // nothing added

		// else expanded
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

void CTDLTaskTreeCtrl::ExpandTasks(const CDWordArray& aExpand)
{
	CHoldRedraw hr(*this);
	CTLSHoldResync hr2(*this);

	int nNumExpanded = aExpand.GetSize();
	
	for (int nItem = 0; nItem < nNumExpanded; nItem++)
	{
		HTREEITEM hti = GetItem(aExpand[nItem]);
		
		if (hti)
		{
			// Don't expand children or fixup the list
			ExpandItemRaw(hti, TRUE, FALSE, FALSE);
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
}

BOOL CTDLTaskTreeCtrl::EnsureSelectionVisible(BOOL bHorzPartialOK)
{
	return TSH().EnsureVisible(bHorzPartialOK);
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

void CTDLTaskTreeCtrl::ExpandSelection(BOOL bExpand, BOOL bFully)
{
	if (!bExpand || bFully)
		TSH().RemoveChildDuplicates();

	// Note: We don;t use CTreeSelectionHelper::ExpandItems here
	// because we need to expand/collapse the list as we go
	POSITION pos = TSH().GetFirstItemPos();

	while (pos)
		ExpandItem(TSH().GetNextItem(pos), bExpand, bFully);
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
		
		ExpandItemRaw(hti, bExpand, bAndChildren);
	}

	if (!bExpand)
		TSH().RemoveHiddenItems();

	if (htiSel)
		m_tcTasks.EnsureVisible(htiSel);
}

void CTDLTaskTreeCtrl::ExpandItemRaw(HTREEITEM hti, BOOL bExpand, BOOL bAndChildren, BOOL bUpdateList)
{
	TCH().ExpandItem(hti, bExpand, bAndChildren, FALSE); // don't expand parents

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
	CHTIList lstParents;
	TSH().GetUniqueParents(lstParents);
		
	TDSORTPARAMS ss(*this);
	VERIFY(PrepareSort(ss));

	POSITION pos = lstParents.GetHeadPosition();
		
	while (pos)
	{
		HTREEITEM htiParent = lstParents.GetNext(pos);

		CTreeListSyncer::Sort(SortFunc, (LPARAM)&ss, htiParent);
	}
		
	ResyncSelection(m_lcColumns, Tasks(), FALSE);
	EnsureSelectionVisible(TRUE);
}

LRESULT CTDLTaskTreeCtrl::OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD) 
{
	ASSERT(pTVCD->nmcd.hdr.hwndFrom == m_tcTasks);
	ASSERT(IsResyncEnabled());
	ASSERT(!m_bTreeExpanding);

	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
	DWORD dwTaskID = pTVCD->nmcd.lItemlParam;

	DWORD dwRes = CDRF_DODEFAULT;
	
	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		dwRes = CDRF_NOTIFYITEMDRAW;
		break;
		
	case CDDS_ITEMPREPAINT:
		dwRes = OnPrePaintTaskTitle(pTVCD->nmcd, pTVCD->clrText, pTVCD->clrTextBk);
		break;
		
	case CDDS_ITEMPOSTPAINT:
		{
			// On Linux filling the background overwrites the tree insertion marker
			BOOL bFillRow = !OsIsLinux();
			dwRes = OnPostPaintTaskTitle(pTVCD->nmcd, pTVCD->nmcd.rc, bFillRow);
		}
		break;
	}
	
	return dwRes;
}

void CTDLTaskTreeCtrl::OnGetDragItemRect(CDC& dc, HTREEITEM hti, CRect& rItem)
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	VERIFY(m_data.GetTask(GetTaskID(hti), pTDI, pTDS));

	HFONT hOldFont = PrepareDCFont(&dc, pTDI, pTDS, TRUE);

	CTreeDragDropRenderer::OnGetDragItemRect(dc, hti, rItem);

	dc.SelectObject(hOldFont);
	rItem.left -= ICON_SIZE;
}

void CTDLTaskTreeCtrl::OnDrawDragItem(CDC& dc, HTREEITEM hti, const CRect& rItem)
{
	DWORD dwTaskID = m_dragTree.GetItemData(hti);

	GraphicsMisc::DrawCentred(&dc, 
							  m_ilTaskIcons,
							  GetTaskIconIndex(dwTaskID),
							  rItem,
							  FALSE, 
							  TRUE);
	CRect rText(rItem);
	rText.OffsetRect(ICON_SIZE, 0);

	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	VERIFY(m_data.GetTask(GetTaskID(hti), pTDI, pTDS));

	HFONT hOldFont = PrepareDCFont(&dc, pTDI, pTDS, TRUE);

	CTreeDragDropRenderer::OnDrawDragItem(dc, hti, rText);

	dc.SelectObject(hOldFont);
}

BOOL CTDLTaskTreeCtrl::GetSelectionBoundingRect(CRect& rSelection) const
{
	if (!TSH().GetBoundingRect(rSelection))
		return FALSE;

	m_tcTasks.MapWindowPoints((CWnd*)this, rSelection);
	return TRUE;
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
			return GMIS_DROPHILITED;

		// else
		if (IsItemSelected(hti))
		{
			DWORD dwTaskID = GetTaskID(hti);
		
			if (HasFocus() || (dwTaskID == m_dwEditTitleTaskID))
				return GMIS_SELECTED;
		
			// else 
			return GMIS_SELECTEDNOTFOCUSED;
		}
	}
	
	// all else
	return GMIS_NONE;
}

GM_ITEMSTATE CTDLTaskTreeCtrl::GetColumnItemState(int nItem) const
{
	return GetTreeItemState((HTREEITEM)m_lcColumns.GetItemData(nItem));
}

void CTDLTaskTreeCtrl::OnListSelectionChange(NMLISTVIEW* pNMLV)
{
	BOOL bSelChange = FALSE;
	TSH().OnListNotifyParentSelChange(pNMLV, bSelChange);

	if (bSelChange)
	{
		UpdateAll();

		// notify parent of selection change
		CPoint pt(GetMessagePos());
		m_lcColumns.ScreenToClient(&pt);

		int nHit = m_lcColumns.HitTest(pt);

		if (Misc::IsCursorKeyPressed(MKC_UPDOWN))
		{
			// vertical scrolling
			// SHOULD NO LONGER GET HERE
			ASSERT(0);
			return;
		}

		if (IsBoundSelecting())
		{
			if ((nHit == -1) || (m_lcColumns.GetSelectedCount() > 2))
			{
				// bulk selecting
				return;
			}
		}
		else
		{
			BOOL bLBtnDown = Misc::IsKeyPressed(VK_LBUTTON);
			BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);

			if ((nHit != -1) && bLBtnDown && !bCtrl && TSH().IsEmpty())
			{
				// In the middle of a simple click
				// SHOULD NO LONGER GET HERE
				ASSERT(0);
				return;
			}
		}

		NotifyParentSelChange();
	}
}

void CTDLTaskTreeCtrl::SyncColumnSelectionToTasks()
{
	ASSERT(CanResync());

	if (CanResync())
	{
		CTLSResyncing tr(*this);
		TSH().SyncListSelection();
	}
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

void CTDLTaskTreeCtrl::OnTreeSelectionChange(NMTREEVIEW* pNMTV)
{
	if (m_bMovingItem || IsBoundSelecting())
		return;

	// snapshot current selection to test for changes
	CHTIList lstPrevSel;
	TSH().CopySelection(lstPrevSel);

	// Update selection
	BOOL bSelChange = FALSE;
	TSH().OnTreeNotifySelectionChange(pNMTV, bSelChange);

	if (bSelChange)
	{
		SyncColumnSelectionToTasks();

		HTREEITEM hti = pNMTV->itemNew.hItem;

		if (hti && !TCH().IsItemVisible(hti, FALSE))
		{
			CLockUpdates lr(m_tcTasks);

			TCH().EnsureItemVisible(hti, FALSE);
		}

		// notify parent of selection change
		// unless up/down cursor key still pressed
		if (TSH().Matches(lstPrevSel) || Misc::IsCursorKeyPressed(MKC_UPDOWN))
			return;

		NotifyParentSelChange(SC_BYKEYBOARD);
	}
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
					return TRUE;
				}
				else if (nHitFlags & (TVHT_ONITEMLABEL | TVHT_ONITEMRIGHT))
				{
					nClickCol = TDCC_CLIENT;
				}
			}

			if ((nClickCol != TDCC_NONE) && SelectionHasUnlocked())
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
			pTVDI->item.iImage = pTVDI->item.iSelectedImage = -1;

			if (!IsColumnShowing(TDCC_ICON))
				pTVDI->item.iImage = pTVDI->item.iSelectedImage = GetTaskIconIndex(pTDI, pTDS);

			// checkbox image
			pTVDI->item.mask |= TVIF_STATE;
			pTVDI->item.stateMask = TVIS_STATEIMAGEMASK;

			switch (GetTaskCheckState(pTDI, pTDS))
			{
			case CTDLTaskCtrlBase::TTCBC_UNCHECKED:	pTVDI->item.state = TCHC_UNCHECKED; break;
			case CTDLTaskCtrlBase::TTCBC_MIXED:		pTVDI->item.state = TCHC_MIXED;		break;
			case CTDLTaskCtrlBase::TTCBC_CHECKED:	pTVDI->item.state = TCHC_CHECKED;	break;
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

 			case TVN_BEGINLABELEDIT:
				if (!m_bMovingItem)
				{
					// Notify parent and cancel this default editing
					NotifyParentOfColumnEditClick(TDCC_CLIENT, GetSelectedTaskID());
				}
				return 1L; // always
				
			case TVN_GETDISPINFO:
				OnTreeGetDispInfo((NMTVDISPINFO*)pNMHDR);
				break;

			case TVN_KEYDOWN:
				TSH().OnTreeNotifyKeyDown((NMTVKEYDOWN*)pNMHDR);
				break;
				
			case TVN_ITEMEXPANDING:
				{
					BOOL bSelChange = FALSE;
					TSH().OnTreeNotifyItemExpanding((NMTREEVIEW*)lp, bSelChange);

					ProcessSelectionChange(bSelChange, SC_BYMOUSE);
				}
				break;

			case TVN_ITEMEXPANDED:
				RecalcUntrackedColumnWidths();
				RepackageAndSendToParent(msg, wp, lp);
				break;
			}
		}
	}
	
	return CTDLTaskCtrlBase::WindowProc(hRealWnd, msg, wp, lp);
}

void CTDLTaskTreeCtrl::EnableExtendedKeyboardSelection(BOOL bCtrl, BOOL bShift)
{
	CTDLTaskCtrlBase::EnableExtendedKeyboardSelection(bCtrl, bShift);

	CTreeSelectionHelper::EnableExtendedKeyboardSelection(bCtrl, bShift);
}

LRESULT CTDLTaskTreeCtrl::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTDLTaskCtrlBase::ScWindowProc(hRealWnd, msg, wp, lp);
	
	if (hRealWnd == m_tcTasks)
	{
		switch (msg)
		{
#ifdef _DEBUG
		case WM_PAINT:
			break;
#endif
		case WM_ERASEBKGND:
			if (m_bMovingItem)
			{
				return TRUE; // we handled it
			}
			break;

		case WM_CHAR:
			if (wp == VK_ESCAPE)
			{
				return 0L; // prevent beep
			}
			break;

		case TVM_SELECTITEM:
			if (wp == TVGN_DROPHILITE)
			{
				HTREEITEM htiNew = (HTREEITEM)lp;
				HTREEITEM htiOld = m_tcTasks.GetDropHilightItem();
				
				if (htiNew != htiOld)
				{
					LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

					InvalidateColumnItem(htiOld);
					InvalidateColumnItem(htiNew);
					UpdateAll();

					return lr;
				}
			}
			break;
			
		case WM_KEYUP: 
			switch (wp)
			{
			case VK_NEXT:  
			case VK_DOWN:
			case VK_UP:
			case VK_PRIOR: 
				{
					BOOL bSelChange = FALSE;
					TSH().OnTreeKeyUp(wp, lp, bSelChange);
			
					if (ProcessSelectionChange(bSelChange, SC_BYKEYBOARD))
						return 0L; // we handled it
				}
				break;
			}
			break;

		case WM_KEYDOWN:
			{
				// Update the selection
				BOOL bSelChange = FALSE;
				TSH().OnTreeKeyDown(wp, lp, bSelChange);
			
				if (ProcessSelectionChange(bSelChange, SC_BYKEYBOARD))
				{
					return 0L; // we handled it
				}
				else if (Misc::ModKeysArePressed(0))
				{
					// Handle expanding/contracting tasks
					switch (wp)
					{
					case VK_MULTIPLY:
					case VK_RIGHT:
						if (TSH().IsAnyItemCollapsed())
						{
							ExpandSelection(TRUE, (wp == VK_MULTIPLY));
						}
						return 0L; // we handled it

					case VK_SUBTRACT:
					case VK_LEFT:
						if (TSH().IsAnyItemExpanded())
						{
							bSelChange = TSH().RemoveChildDuplicates();
							ExpandSelection(FALSE);
							
							ProcessSelectionChange(bSelChange, SC_BYKEYBOARD);
						}
						return 0L; // we handled it
					}
				}
			}
			break;
			
		case WM_RBUTTONDOWN: // --------------------------------------------------------------------------
			{
				// Update the selection
				BOOL bSelChange = FALSE;
				TSH().OnTreeRButtonDown(wp, lp, bSelChange);
				
				ProcessSelectionChange(bSelChange, SC_BYMOUSE);

				// Allow default handling to produce context menu
			}
			break;
			
		case WM_LBUTTONDOWN: // --------------------------------------------------------------------------
			{
				// Update the selection
				BOOL bSelChange = FALSE;
				TSH().OnTreeLButtonDown(wp, lp, bSelChange);
				
				ProcessSelectionChange(bSelChange, SC_BYMOUSE);

				// Handle icon and checkbox clicking
				if (0 == (wp & (MK_CONTROL | MK_SHIFT)))
				{
					UINT nHitFlags = 0;
					HTREEITEM htiHit = m_tcTasks.HitTest(lp, &nHitFlags);

					BOOL bHitIcon = (nHitFlags & (TVHT_ONITEMICON | TVHT_ONITEMSTATEICON));

					if (bHitIcon && HandleClientColumnClick(lp, FALSE))
					{
						return 0L; // we handled it
					}
				}

				if (bSelChange)
				{
					return 0L; // we handled it
				}
			}
			break;
		
		case WM_LBUTTONDBLCLK:
			if (HandleClientColumnClick(lp, TRUE))
			{
				return 0L; // we handled it
			}
			break;

		case WM_VSCROLL:
			{
 				CDisableTreeTips dtt(m_tcTasks, FALSE);
				
				return CTDLTaskCtrlBase::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;
		} 
	}
	else if (hRealWnd == m_lcColumns)
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			{
				BOOL bSelChange = FALSE;
				TSH().OnListLButtonDown(wp, lp, bSelChange);

				if (ProcessSelectionChange(bSelChange, SC_BYMOUSE))
					return 0L; // we handled it
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
					return 0L; // we handled it

				// Allow double-clicking to expand tree item 
				// if not on an 'active' column
				ASSERT(hti == GetTreeSelItem(m_tcTasks));

				if (TCH().TreeCtrl().ItemHasChildren(hti))
				{
					ExpandItem(hti, !TCH().IsItemExpanded(hti));
					return 0L; // we handled it
				}
			}
			break;

		case WM_KEYDOWN:
		case WM_KEYUP:
			{
				// In the rare event that the list has gained the 
				// focus let the tree process these messages 
				ScWindowProc(m_tcTasks, msg, wp, lp);

				// And move the focus to the tree once we've 
				// received WM_KEYUP
				if (msg == WM_KEYUP)
					m_tcTasks.SetFocus();
			}
			return 0L; // we handled it
		}
	}
	
	return CTDLTaskCtrlBase::ScWindowProc(hRealWnd, msg, wp, lp);
}

BOOL CTDLTaskTreeCtrl::ProcessSelectionChange(BOOL bSelChange, SELCHANGE_ACTION nBy)
{
	if (bSelChange)
	{
		SyncColumnSelectionToTasks();
		NotifyParentSelChange(nBy);
		UpdateSelectedTaskPath();
	}

	return bSelChange;
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

	static BOOL bOsIsLinux = OsIsLinux();

	switch (nArea)
	{
	case TDCTR_TEXT:
		{
			// Basic title rect
			VERIFY(m_tcTasks.GetItemRect(hti, rect, TRUE));

			// Available width
			CRect rAvail;
			m_tcTasks.GetClientRect(rAvail);

			if (pDC && szTitle)
			{
				rect.right = (rect.left + pDC->GetTextExtent(szTitle).cx);
				rect.right = min(rect.right, rAvail.right);
			}
			else
			{
				ASSERT(!pDC && !szTitle);
				rect.right = rAvail.right;
			}
		}
		return TRUE;

	case TDCTR_BKGND:
		if (GetItemTitleRect(hti, TDCTR_TEXT, rect)) // RECURSIVE CALL
		{
			rect.left -= TITLE_BORDER_OFFSET;

			if (bOsIsLinux)
				rect.top--;
			
			return TRUE;
		}
		break;

	case TDCTR_EDIT:
		if (GetItemTitleRect(hti, TDCTR_BKGND, rect)) // RECURSIVE CALL
		{
			if (!bOsIsLinux)
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
	
	SyncColumnSelectionToTasks();

	// make sure first moved item is visible
	if (bEnsureVisible && !TCH().IsItemVisible(htiFirst))
	{
		// post the message to ensure that whatever called this 
		// has already finished
		m_tcTasks.PostMessage(TVM_ENSUREVISIBLE, 0, (LPARAM)htiFirst);
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
	const CHTIList& selection = TSH().Items();
	
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
			if (SelectionHasLocked(TRUE))
				return FALSE;

			// Prevent moving subtasks of locked parent unless parent is reference
			if (SelectionHasLockedParents(TRUE))
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

BOOL CTDLTaskTreeCtrl::CanSelectTasksInHistory(BOOL bForward) const 
{ 
	return (bForward ? TSH().HasNextSelection() : TSH().HasPrevSelection()); 
}

BOOL CTDLTaskTreeCtrl::SelectTasksInHistory(BOOL bForward) 
{ 
	BOOL bSelected = FALSE;

	if (bForward)
		bSelected = TSH().NextSelection(m_mapTaskIDToHTI);
	else
		bSelected = TSH().PrevSelection(m_mapTaskIDToHTI);

	if (bSelected)
		SyncColumnSelectionToTasks();

	return bSelected;
}

int CTDLTaskTreeCtrl::GetSelectedTasksInHistory(BOOL bForward, CDWordArray& aTaskIDs) const
{
	return (bForward ? TSH().GetNextSelectionIDs(aTaskIDs) : TSH().GetPrevSelectionIDs(aTaskIDs));
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
			// which is not a reference unless both the task and sibling are references
			DWORD dwNewParentID = 0, dwUnused = 0;
			VERIFY(GetInsertLocation(nDirection, dwNewParentID, dwUnused));
			
			if (!dwNewParentID)
				return FALSE;

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

		// redraw column row
		InvalidateColumnItem(hti, bUpdate);
	}

	//else
	return FALSE;
}

BOOL CTDLTaskTreeCtrl::InvalidateColumnItem(HTREEITEM hti, BOOL bUpdate)
{
	if (hti)
	{
		int nItem = FindListItem(m_lcColumns, (DWORD)hti);
		return CTDLTaskCtrlBase::InvalidateColumnItem(nItem, bUpdate);
	}

	// else
	return FALSE;
}

int CTDLTaskTreeCtrl::CacheSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs) const
{
	if (GetSelectedTaskIDs(cache.aSelTaskIDs, cache.dwFocusedTaskID, FALSE, TRUE) > 0) // ordered
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

BOOL CTDLTaskTreeCtrl::RestorePreviousSelection(BOOL bRedraw)
{
	return TSH().PrevSelection(m_mapTaskIDToHTI, bRedraw);
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
			EnsureSelectionVisible(TRUE);

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

int CTDLTaskTreeCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue, BOOL bOrdered) const
{
	DWORD dwFocusID;
	int nNumIDs = GetSelectedTaskIDs(aTaskIDs, dwFocusID, FALSE, bOrdered);

	// extra processing
	if (nNumIDs && bTrue)
		m_data.GetTrueTaskIDs(aTaskIDs);

	return nNumIDs;
}

int CTDLTaskTreeCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID, BOOL bRemoveChildDupes, BOOL bOrdered) const
{
	aTaskIDs.RemoveAll();
	dwFocusedTaskID = 0;

	if (GetSelectedCount())
	{
		// get selected tasks ordered with/out duplicate subtasks
		CHTIList selection;

		TSH().CopySelection(selection, bRemoveChildDupes, bOrdered);
		TSH().GetItemData(selection, aTaskIDs);
		
		// focused item
		HTREEITEM htiFocus = m_tcTasks.GetSelectedItem();

		if (htiFocus && selection.HasItem(htiFocus))
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

BOOL CTDLTaskTreeCtrl::SelectTasks(const CDWordArray& aTaskIDs)
{
	// sanity check
	int nTask = aTaskIDs.GetSize();

	if (!nTask)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// clear existing selection and add to history
	TSH().RemoveAll();

	// select tasks in one hit
	BOOL bSel = FALSE;
	
	while (nTask--)
	{
		HTREEITEM hti = GetItem(aTaskIDs[nTask]);

		if (hti)
			bSel |= TSH().SetItem(hti, TSHS_SELECT, TRUE);
	}
	
	if (bSel || !aTaskIDs.GetSize())
	{
		TSH().FixupTreeSelection();

		// Expand all the relevant tree nodes before 
		// updating and syncing the list
		EnsureSelectionVisible(TRUE);
		ExpandList();

		ProcessSelectionChange(TRUE, SC_UNKNOWN);
	}
	
	return bSel;
}

BOOL CTDLTaskTreeCtrl::MultiSelectItem(HTREEITEM hti, TSH_SELECT nState) 
{ 
	if (TSH().SetItem(hti, nState))
	{
		TSH().FixupTreeSelection();
		ProcessSelectionChange(TRUE, SC_UNKNOWN);

		return TRUE;
	}
	
	return FALSE;
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
		GetReferencesToTask(dwTaskID, listRefs, FALSE);

		POSITION pos = listRefs.GetHeadPosition();

		while (pos)
			DeleteItem(listRefs.GetNext(pos), TRUE); // RECURSIVE CALL
	}

	// Finally delete the tree item
	return m_tcTasks.DeleteItem(hti);
}

int CTDLTaskTreeCtrl::GetSelectedTaskLocalDependents(BOOL bImmediateOnly, CHTIList& lstDependents) const
{
	lstDependents.RemoveAll();

	CDWordArray aSelTaskIDs;
	
	if (TSH().GetItemData(aSelTaskIDs))
	{
		CDWordArray aTaskDeps;
		int nDep = m_data.GetTaskLocalDependents(aSelTaskIDs, aTaskDeps, bImmediateOnly);

		while (nDep--)
		{
			HTREEITEM hti = GetItem(aTaskDeps[nDep]);

			if (hti)
				lstDependents.AddTail(hti);
		}
	}

	return lstDependents.GetCount();
}

int CTDLTaskTreeCtrl::GetReferencesToSelectedTask(CHTIList& listRefs) const
{
	listRefs.RemoveAll();

	if (HasReferenceTasks())
	{
		POSITION pos = TSH().GetFirstItemPos();

		while (pos)
		{
			DWORD dwTaskID = TSH().GetNextItemData(pos);
			GetReferencesToTask(dwTaskID, listRefs, TRUE);
		}
	}

	return listRefs.GetCount();
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
	
	if (!TSH().CopySelection(selection, TRUE, TRUE))
	{
		ASSERT(TSH().IsEmpty());
		return FALSE;
	}

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
	
	// Don't overwrite previous state if we have no tasks
	if (GetItemCount() == 0)
		return;
	
	if (!sKey.IsEmpty())
	{
		CDWordArray aExpanded;
		GetExpandedTasks(aExpanded);

		prefs.WriteProfileArray(sKey + _T("\\ExpandedItems"), aExpanded);
		
		// and selected item
		prefs.WriteProfileInt(sKey, _T("SelItem"), GetSelectedTaskID());
	}
}

HTREEITEM CTDLTaskTreeCtrl::LoadState(const CPreferences& prefs, const CString& sKey, BOOL bExpandedOnly) 
{
	if (!bExpandedOnly)
		CTDLTaskCtrlBase::LoadState(prefs, sKey);

	CDWordArray aExpanded;
	
	if (prefs.GetProfileArray(sKey + _T("\\ExpandedItems"), aExpanded))
		ExpandTasks(aExpanded);
	
	// restore prev selected item
	DWORD dwSel = prefs.GetProfileInt(sKey, _T("SelItem"), 0);
	return GetItem(dwSel);
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
		// we can do an optimised sort of just the affected branches
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
	if (!HasStyle(TDCS_RESORTONMODIFY) || !IsSorting())
		return FALSE;

	if (mapAttribIDs.Has(TDCA_ALL) ||
		mapAttribIDs.Has(TDCA_UNDO) ||
		mapAttribIDs.Has(TDCA_PASTE) ||
		mapAttribIDs.Has(TDCA_POSITION_DIFFERENTPARENT))
	{
		return TRUE;
	}

	// Check for calculated attributes which also match current sort
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		TDC_ATTRIBUTE nAttribID = mapAttribIDs.GetNext(pos);
		TDC_COLUMN nColID = TDC::MapAttributeToColumn(nAttribID);

		if (m_mapVisibleCols.Has(nColID) &&
			m_calculator.IsAggregatedAttribute(nAttribID) &&
			m_sort.Matches(nAttribID, m_styles, m_aCustomAttribDefs))
		{
			return TRUE;
		}
	}
	
	return FALSE;
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
