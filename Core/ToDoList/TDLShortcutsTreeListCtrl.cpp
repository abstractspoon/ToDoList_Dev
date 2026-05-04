// PrefererencesShortcutsPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLShortcutsTreeListCtrl.h"
#include "todoctrl.h"
#include "tdcstatic.h"
#include "TDCMainMenu.h"
#include "TDCFilter.h"

#include "..\shared\holdredraw.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\clipboard.h"
#include "..\shared\AcceleratorString.h"
#include "..\shared\CtrlTextHighlighter.h"
#include "..\shared\ShortcutManager.h"
#include "..\shared\MenuIconMgr.h"
#include "..\shared\WndPrompt.h"

#include "..\3rdParty\XNamedColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{
	COL_SHORTCUT = 0,
	COL_CMDID,
};

/////////////////////////////////////////////////////////////////////////////

#define ID_MISC_SUBMENU ((UINT)-1)

/////////////////////////////////////////////////////////////////////////////

const int TEXT_PADDING			= 3;
const int SHORTCUTCOL_MINWIDTH	= 75;
const int ICON_OFFSET			= GraphicsMisc::ScaleByDPIFactor(20);

/////////////////////////////////////////////////////////////////////////////
// CTDLShortcutsTreeListCtrl control

CTDLShortcutsTreeListCtrl::CTDLShortcutsTreeListCtrl(const CCtrlTextHighlighter& ctrlHighlighter,
													 const CMenuIconMgr& mgrIcons,
													 CWndPromptManager& mgrPrompts,
													 CShortcutManager* pMgrShortcuts)
	:
	m_ctrlHighlighter(ctrlHighlighter),
	m_mgrMenuIcons(mgrIcons),
	m_mgrPrompts(mgrPrompts),
	m_pMgrShortcuts(pMgrShortcuts),
	m_bShowCommandIDs(FALSE)
{
	EnableMultiSelection(FALSE);
}

BEGIN_MESSAGE_MAP(CTDLShortcutsTreeListCtrl, CTreeListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

int CTDLShortcutsTreeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeListCtrl::OnCreate(lpCreateStruct) != 0)
		return -1;

	m_tree.ModifyStyle(TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT, TVS_FULLROWSELECT);

	// Add columns
	m_treeHeader.InsertItem(0, 1000, CEnString(IDS_PSP_MENUITEM), (HDF_LEFT | HDF_STRING), 0, 1);
	m_treeHeader.ModifyStyle(HDS_DRAGDROP | HDS_BUTTONS, 0);
	m_treeHeader.EnableItemDragging(0, FALSE);
	m_treeHeader.EnableTracking(FALSE);

	m_list.InsertColumn(COL_SHORTCUT, CEnString(IDS_PSP_SHORTCUT), LVCFMT_LEFT, 100);
	m_list.InsertColumn(COL_CMDID, CEnString(IDS_TDC_COLUMN_ID), LVCFMT_LEFT, 100);

	m_listHeader.ShowItem(COL_CMDID, m_bShowCommandIDs);
	m_listHeader.ModifyStyle(HDS_DRAGDROP | HDS_BUTTONS, 0);
	m_listHeader.EnableItemDragging(COL_SHORTCUT, FALSE);
	m_listHeader.EnableItemDragging(COL_CMDID, FALSE);
	m_listHeader.EnableTracking(FALSE);

	m_mgrPrompts.SetPrompt(m_tree, CEnString(IDS_PSP_NOMATCHES), TVM_GETCOUNT);

	SetGridLineColor(colorSilver);
	SetSplitBarWidth(1);
	SwapSides();

#ifndef _DEBUG
	EnableSplitting(FALSE);
#endif

	return 0;
}

void CTDLShortcutsTreeListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTreeListCtrl::OnSize(nType, cx, cy);

	m_treeHeader.SetItemWidth(0, (cx - m_listHeader.CalcTotalItemWidth()));
}

void CTDLShortcutsTreeListCtrl::ShowCommandIDs(BOOL bShow)
{
	if (Misc::StatesDiffer(bShow, m_bShowCommandIDs))
	{
		m_bShowCommandIDs = bShow;

		if (m_listHeader.GetSafeHwnd())
		{
			m_listHeader.ShowItem(COL_CMDID, bShow);

			RecalcColumnsToFit();
		}
	}
}

void CTDLShortcutsTreeListCtrl::BuildMenuTree()
{
	ASSERT(m_pMgrShortcuts);

	CWaitCursor cursor;

	{
		CTLSHoldResync hr(*this);

		m_tree.DeleteAllItems();
		m_tree.SetIndent(ICON_OFFSET);

		CTDCMainMenu menu;

		if (menu.LoadMenu())
		{
			RemoveUnusedDefaultFilterItems(menu);

			// CTreeListCtrl wants all tree item data to be unique 
			// so submenus cannot all be -1
			UINT nNextSubMenuCmdID = (ID_MISC_SUBMENU - 1);

			for (int nPos = 0; nPos < (int)menu.GetMenuItemCount(); nPos++)
				AddMenuItem(TVI_ROOT, &menu, nPos, !m_ctrlHighlighter.HasSearch(), nNextSubMenuCmdID);
		}

		AddMiscShortcuts(); // un-editable shortcuts
		ExpandAll();
	}

	SelectItem(m_tree.GetChildItem(NULL));
	m_fonts.Initialise(m_tree);

	RecalcColumnsToFit();
}

HTREEITEM CTDLShortcutsTreeListCtrl::AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, 
												 int nPos, BOOL bForceAdd, UINT& nNextSubMenuCmdID)
{
	UINT nCmdID = pMenu->GetMenuItemID(nPos);
	BOOL bSubMenu = IsSubMenu(nCmdID);

	// Exclude the debug menu
	if (bSubMenu)
	{
		if (pMenu->GetSubMenu(nPos)->GetMenuItemID(0) == ID_DEBUG_ENDSESSION)
			return NULL;

		// CTreeListCtrl wants all tree item data to be unique 
		// so submenus cannot all be -1
		nCmdID = nNextSubMenuCmdID--;
	}

	CString sItem;
	pMenu->GetMenuString(nPos, sItem, MF_BYPOSITION);

	// remove '&'
	CAcceleratorString::RemoveAccelerator(sItem);

	// remove everything after '\t'
	int nTab = sItem.Find('\t');

	if (nTab >= 0)
		sItem = sItem.Left(nTab);

	if (sItem.IsEmpty())
		return NULL;

	if (!bForceAdd && (htiParent != TVI_ROOT))
		bForceAdd = WantItem(m_tree.GetItemText(htiParent));

	HTREEITEM hti = InsertItem(sItem, nCmdID, htiParent, bForceAdd);

	if (!hti)
	{
		ASSERT(!bSubMenu && m_ctrlHighlighter.HasSearch());
		return NULL;
	}

	if (bSubMenu)
	{
		CMenu* pSubMenu = pMenu->GetSubMenu(nPos);

		if (pSubMenu)
		{
			for (int nSubPos = 0; nSubPos < (int)pSubMenu->GetMenuItemCount(); nSubPos++)
			{
				AddMenuItem(hti, pSubMenu, nSubPos, bForceAdd, nNextSubMenuCmdID); // RECURSIVE CALL
			}
		}

		// remove the submenu if it contains no items
		if (!WantKeepSubmenu(hti))
		{
			m_tree.DeleteItem(hti);
		}
	}
	else if (!IsMiscCommandID(nCmdID)) // fixes a bug where misc ids were being saved
	{
		DWORD dwShortcut = m_pMgrShortcuts->GetShortcut(nCmdID);

		if (dwShortcut)
		{
			m_mapID2Shortcut[nCmdID] = dwShortcut;
			m_mapShortcut2ID[dwShortcut] = nCmdID;
		}
	}

	return hti;
}

BOOL CTDLShortcutsTreeListCtrl::WantKeepSubmenu(HTREEITEM hti) const
{
	if (m_tree.ItemHasChildren(hti))
		return TRUE;

	return WantItem(m_tree.GetItemText(hti));
}

BOOL CTDLShortcutsTreeListCtrl::WantItem(const CString& sItem) const
{
	if (!m_ctrlHighlighter.HasSearch())
		return TRUE;
	
	return m_ctrlHighlighter.TextContainsOneOf(sItem);
}

BOOL CTDLShortcutsTreeListCtrl::MatchesSearch(const CString& sItem) const
{
	if (!m_ctrlHighlighter.HasSearch())
		return FALSE;
	
	return m_ctrlHighlighter.TextContainsOneOf(sItem);
}

HTREEITEM CTDLShortcutsTreeListCtrl::InsertItem(const CString& sItem, UINT nCmdID, HTREEITEM htiParent, BOOL bForceAdd)
{
	// Exclude leaf tasks not matching the search terms
	// unless their parent matches the search
	if (!IsSubMenu(nCmdID) && !bForceAdd && !WantItem(sItem))
		return NULL;
	
	HTREEITEM hti = m_tree.InsertItem(sItem, -1, -1, nCmdID, htiParent, TVI_LAST);
	ASSERT(hti);

	return hti;
}

void CTDLShortcutsTreeListCtrl::AddMiscShortcuts()
{
	if (!NUM_MISCSHORTCUTS)
		return;

	// Add parent placeholder
	CEnString sSubMenuText(IDS_MISCSHORTCUTS);

	BOOL bForceAdd = !m_ctrlHighlighter.HasSearch();
	HTREEITEM htiParent = InsertItem(sSubMenuText, ID_MISC_SUBMENU, TVI_ROOT, bForceAdd);

	// add children
	for (int nItem = 0; nItem < NUM_MISCSHORTCUTS; nItem++)
	{
		DWORD dwShortcut = MISC_SHORTCUTS[nItem].dwShortcut;
		
		if (dwShortcut)
		{
			// make fake command IDs so it does not intersect with normal IDs
			UINT nCmdID = MAKELONG(0, nItem + 1);
			CEnString sMisc(MISC_SHORTCUTS[nItem].nIDShortcut);
			
			HTREEITEM hti = InsertItem(sMisc, nCmdID, htiParent, bForceAdd);

			if (hti && dwShortcut)
			{
				m_mapID2Shortcut[nCmdID] = dwShortcut;
				m_mapShortcut2ID[dwShortcut] = nCmdID;
			}
		}
	}

	// Delete parent item if it has no items
	if (!WantKeepSubmenu(htiParent))
	{
		m_tree.DeleteItem(htiParent);
	}
}

void CTDLShortcutsTreeListCtrl::RemoveUnusedDefaultFilterItems(CMenu& menu) const
{
	HMENU hFilterMenu = CEnMenu::GetSubMenu(menu, ID_VIEW_ACTIVATEFILTER1);
	ASSERT(hFilterMenu);

	int nNumFilters = CTDCFilter::GetDefaultFilterNames().GetSize();

	for (int nFilter = nNumFilters; nFilter < 24; nFilter++)
		::DeleteMenu(hFilterMenu, (ID_VIEW_ACTIVATEFILTER1 + nFilter), MF_BYCOMMAND);
}

BOOL CTDLShortcutsTreeListCtrl::IsMiscCommandID(UINT nCmdID)
{
	return (LOWORD(nCmdID) == 0) && (HIWORD(nCmdID) != 0);
}

BOOL CTDLShortcutsTreeListCtrl::SaveToShortcutMgr()
{
	if (!m_pMgrShortcuts)
	{
		ASSERT(0);
		return FALSE;
	}

	// copy all the changes to m_pShortcutMgr
	// except for reserved shortcuts
	POSITION pos = m_mapID2Shortcut.GetStartPosition();

	while (pos)
	{
		UINT nCmdID = 0;
		DWORD dwShortcut = 0;

		m_mapID2Shortcut.GetNextAssoc(pos, nCmdID, dwShortcut);
		ASSERT (nCmdID);

		if (!IsMiscCommandID(nCmdID))
			m_pMgrShortcuts->SetShortcut(nCmdID, dwShortcut);
	}

	return TRUE;
}

UINT CTDLShortcutsTreeListCtrl::GetSelectedCmdID() const
{
	return GetSelectedItemData();
}

DWORD CTDLShortcutsTreeListCtrl::GetSelectedShortcut() const
{
	return GetShortcut(GetSelectedCmdID());
}

DWORD CTDLShortcutsTreeListCtrl::GetShortcut(UINT nCmdID) const
{
	DWORD dwShortcut = 0;
	m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

	return ((dwShortcut == NO_SHORTCUT) ? 0 : dwShortcut);
}

UINT CTDLShortcutsTreeListCtrl::GetCmdID(DWORD dwShortcut) const
{
	if (!dwShortcut || (dwShortcut == NO_SHORTCUT))
		return 0;

	UINT nCmdID = 0;
	m_mapShortcut2ID.Lookup(dwShortcut, nCmdID);

	return nCmdID;
}

BOOL CTDLShortcutsTreeListCtrl::HasShorcut(DWORD dwShortcut) const
{
	return (GetCmdID(dwShortcut) != 0);
}

CString CTDLShortcutsTreeListCtrl::GetMenuText(UINT nCmdID) const
{
	HTREEITEM hti = GetTreeItem((DWORD)nCmdID);

	return m_tree.GetItemText(hti);
}

BOOL CTDLShortcutsTreeListCtrl::AssignShortcut(UINT nCmdID, DWORD dwNewShortcut)
{
	// Sanity checks
	if (!nCmdID)
		return FALSE;

	DWORD dwCurShortcut = GetShortcut(nCmdID);

	if (dwNewShortcut == dwCurShortcut)
		return FALSE;

	// remove any shortcut currently assigned to nCmdID
	m_mapShortcut2ID.RemoveKey(dwCurShortcut);
	
	if (!dwNewShortcut)
	{
		// Special case: User is explicitly deleting a shortcut
		dwNewShortcut = NO_SHORTCUT;
	}
	else
	{
		// else if anyone has this shortcut we must remove their mapping first
		UINT nOtherCmdID = GetCmdID(dwNewShortcut);

		if (nOtherCmdID)
		{
			m_mapID2Shortcut.RemoveKey(nOtherCmdID);
			InvalidateListItem(GetTreeItem((DWORD)nOtherCmdID));
		}
	}

	// Set new shortcut
	m_mapID2Shortcut[nCmdID] = dwNewShortcut;

	if (dwNewShortcut != NO_SHORTCUT)
		m_mapShortcut2ID[dwNewShortcut] = nCmdID;

	InvalidateListItem(GetTreeItem((DWORD)nCmdID));
	return TRUE;
}

BOOL CTDLShortcutsTreeListCtrl::DeleteShortcut(UINT nCmdID)
{
	if (!nCmdID)
		return FALSE;

	// remove any shortcut currently assigned to nCmdID
	DWORD dwShortcut = GetShortcut(nCmdID);

	if (!dwShortcut)
		return FALSE;

	// else
	m_mapShortcut2ID.RemoveKey(dwShortcut);
	m_mapID2Shortcut[nCmdID] = 0;

	InvalidateListItem(GetTreeItem((DWORD)nCmdID));
	return TRUE;
}

void CTDLShortcutsTreeListCtrl::RecalcColumnsToFit()
{
	CClientDC dc(&m_tree);

	// Shortcut Column
	int nMaxShortcutWidth = GetLongestShortcutText(&dc);

	if (nMaxShortcutWidth)
		nMaxShortcutWidth += (2 * TEXT_PADDING);

	nMaxShortcutWidth = max(SHORTCUTCOL_MINWIDTH, nMaxShortcutWidth);

	m_listHeader.SetItemWidth(0, nMaxShortcutWidth);

	// ID column
	int nMaxIDWidth = 0;
	
	if (m_bShowCommandIDs)
		nMaxIDWidth = (dc.GetTextExtent(_T("12345")).cx + (2 * TEXT_PADDING));

	m_listHeader.SetItemWidth(1, nMaxIDWidth);

// 	// Tree
	CRect rClient;
	GetClientRect(rClient);

	CTreeListSyncer::Resize(rClient, nMaxShortcutWidth + nMaxIDWidth);
}

int CTDLShortcutsTreeListCtrl::GetLongestShortcutText(CDC* pDC, HTREEITEM hti)
{
	int nLongest = 0;

	if (hti != NULL)
	{
		int nCmdID = m_tree.GetItemData(hti);

		if (nCmdID)
		{
			DWORD dwShortcut = 0;
			m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

			if (dwShortcut)
			{
				CString sShortcut = m_pMgrShortcuts->GetShortcutText(dwShortcut);
				nLongest = sShortcut.IsEmpty() ? 0 : pDC->GetTextExtent(sShortcut).cx;
			}
		}
	}

	// children
	HTREEITEM htiChild = m_tree.GetChildItem(hti);

	while (htiChild)
	{
		int nWidth = GetLongestShortcutText(pDC, htiChild);
		nLongest = max(nLongest, nWidth);

		htiChild = m_tree.GetNextItem(htiChild, TVGN_NEXT);
	}

	return nLongest;
}

LRESULT CTDLShortcutsTreeListCtrl::OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD, const CIntArray& aColOrder, const CIntArray& aColWidths)
{
	HWND hwndList = pLVCD->nmcd.hdr.hwndFrom;
	int nItem = (int)pLVCD->nmcd.dwItemSpec;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;

	case CDDS_ITEMPREPAINT:
		{
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			HTREEITEM hti = (HTREEITEM)pLVCD->nmcd.lItemlParam;

			UINT nCmdID = m_tree.GetItemData(hti);
			BOOL bSubMenu = IsSubMenu(nCmdID);

			DWORD dwShortcut = 0;
			m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

			// draw item bkgnd and gridlines full width of list
			COLORREF crBack = GetSysColor(bSubMenu ? COLOR_3DFACE : COLOR_WINDOW);
			pLVCD->clrTextBk = pLVCD->clrText = crBack;

			GM_ITEMSTATE nState = CTreeListCtrl::GetItemState(nItem);
			BOOL bSelected = (nState != GMIS_NONE);

			// Fill bkgnd before drawing gridline
			if (bSubMenu && !bSelected)
				pDC->FillSolidRect(&pLVCD->nmcd.rc, crBack);

			// draw horz gridline before selection
			CRect rFullWidth(pLVCD->nmcd.rc);
			rFullWidth.right += 1000;
			DrawHorzItemDivider(pDC, rFullWidth);

			// Draw selection before text
			DWORD dwDrawFlags = (GMIB_THEMECLASSIC | GMIB_CLIPLEFT | GMIB_PREDRAW | GMIB_POSTDRAW);

			if (bSelected)
				GraphicsMisc::DrawExplorerItemSelection(pDC, m_list, nState, pLVCD->nmcd.rc, dwDrawFlags);

			// Draw columns
			CRect rItem(pLVCD->nmcd.rc);

			for (int nCol = COL_SHORTCUT; nCol <= COL_CMDID; nCol++)
			{
				rItem.right = (rItem.left + aColWidths[nCol]);
				DrawVertItemDivider(pDC, rItem, bSelected);

				if (!bSubMenu)
				{
					COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);
					CString sText;

					switch (nCol)
					{
					case COL_SHORTCUT:
						if (dwShortcut)
						{
							sText = m_pMgrShortcuts->GetShortcutText(dwShortcut);

							// text for reserved shortcut and mark in red
							if (CToDoCtrl::IsReservedShortcut(dwShortcut) && !IsMiscCommandID(nCmdID))
							{
								if (bSelected)
									crText = GraphicsMisc::GetExplorerItemSelectionTextColor(colorRed, nState, dwDrawFlags);
								else
									crText = colorRed;
							}
						}
						break;

					case COL_CMDID:
						if (m_bShowCommandIDs && nCmdID && !IsMiscCommandID(nCmdID))
							sText = Misc::Format((DWORD)nCmdID);
						break;
					}

					if (!sText.IsEmpty())
					{
						rItem.left += TEXT_PADDING;

						pDC->SetTextColor(crText);
						pDC->DrawText(sText, rItem, (DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX));
					}
				}

				// next column
				rItem.left = rItem.right; 
			}
		}
		return CDRF_SKIPDEFAULT;
	}

	return CDRF_DODEFAULT;
}

COLORREF CTDLShortcutsTreeListCtrl::GetTreeItemBackColor(HTREEITEM hti, DWORD dwItemData, BOOL bSelected) const
{
	if (IsSubMenu(dwItemData))
		return GetSysColor(COLOR_3DFACE);

	return CTreeListCtrl::GetTreeItemBackColor(hti, dwItemData, bSelected);
}

void CTDLShortcutsTreeListCtrl::DrawTreeSubItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, int nCol, const CRect& rSubItem, BOOL bSelected)
{
	CString sItem = m_tree.GetItemText(hti);
	CRect rText(rSubItem);
	COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);

	if (MatchesSearch(sItem))
	{
		// don't draw over gridline
		rText.bottom--;

		if (bSelected)
			rText.top++;

		pDC->FillSolidRect(rText, m_ctrlHighlighter.GetBkColor());
		crText = m_ctrlHighlighter.GetTextColor();
	}

	UINT nCmdID = dwItemData;
	DWORD dwShortcut = 0;

	m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

	if (dwShortcut && CToDoCtrl::IsReservedShortcut(dwShortcut) && !IsMiscCommandID(nCmdID))
	{
		crText = colorRed;
	}

	BOOL bBold = IsSubMenu(dwItemData);
	HGDIOBJ hOldFont = pDC->SelectObject(m_fonts.GetHFont(bBold, FALSE, FALSE, FALSE));

	pDC->SetTextColor(crText);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(sItem, rText, (DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_NOCLIP));
	pDC->SelectObject(hOldFont);
}

void CTDLShortcutsTreeListCtrl::DrawTreeItemIcon(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel)
{
	CRect rIcon(rLabel);
 	rIcon.OffsetRect(-ICON_OFFSET, 0);

	m_mgrMenuIcons.DrawImage(*pDC, dwItemData, rIcon.TopLeft());
}

void CTDLShortcutsTreeListCtrl::CopyAllToClipboard() const
{
	CString sOutput;
	
	if (CopyItem(TVI_ROOT, sOutput))
		VERIFY(CClipboard(*this).SetText(sOutput));
}

BOOL CTDLShortcutsTreeListCtrl::CopyItem(HTREEITEM hti, CString& sOutput) const
{
	if (!hti)
		return FALSE;

	// copy self
	if (hti != TVI_ROOT)
	{
		if (!m_tree.ItemHasChildren(hti))
		{
			// ignore Reserved menu commands
			DWORD dwCmdID = m_tree.GetItemData(hti);
			
			if (IsMiscCommandID(dwCmdID))
				return FALSE;

			CString sItem = m_tree.GetItemText(hti);
			ASSERT(!sItem.IsEmpty());

			if (!sItem.IsEmpty())
			{
				// build path by recursing up the parent chain
				HTREEITEM htiParent = m_tree.GetParentItem(hti);

				while (htiParent)
				{
					CString sParent = m_tree.GetItemText(htiParent);
					ASSERT(!sParent.IsEmpty());
					
					if (!sParent.IsEmpty())
						sItem = sParent + _T(" > ") + sItem;

					// parent's parent
					htiParent = m_tree.GetParentItem(htiParent);
				}

				sOutput += Misc::Format(dwCmdID);
				sOutput += _T(" : ");
				sOutput += sItem;
				sOutput += _T("\r\n");
			}
		}
	}

	// then children
	CopyItem(m_tree.GetChildItem(hti), sOutput);

	// then siblings
	if (hti != TVI_ROOT)
	{
		// add a spacer between top-level items
		if (m_tree.GetParentItem(hti) == NULL)
			sOutput += _T("\r\n");

		CopyItem(m_tree.GetNextItem(hti, TVGN_NEXT), sOutput);
	}

	return (!sOutput.IsEmpty());
}
