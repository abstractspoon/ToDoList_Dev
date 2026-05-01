// PrefererencesShortcutsPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesShortcutsPage.h"
#include "todoctrl.h"
#include "tdcstatic.h"
#include "TDCMainMenu.h"
#include "TDCFilter.h"

#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\holdredraw.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\clipboard.h"
#include "..\shared\themed.h"
#include "..\shared\AcceleratorString.h"

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

#define PSP_SHORTCUTCOLUMNID	(OTC_POSCOLUMNID - 1)
#define PSP_COMMANDIDCOLUMNID	(OTC_POSCOLUMNID - 2)

#define ID_SUBMENU ((UINT)-1)

const int TEXT_PADDING			= 3;
const int SHORTCUTCOL_MINWIDTH	= 75;
const int ICON_OFFSET			= GraphicsMisc::ScaleByDPIFactor(20);

/////////////////////////////////////////////////////////////////////////////
// CPreferencesShortcutsPage property page

CPreferencesShortcutsPage::CPreferencesShortcutsPage(const CMenuIconMgr& mgrIcons, CShortcutManager* pMgrShortcuts)
	: 
	CPreferencesPageBase(IDD_PREFSHORTCUTS_PAGE),
	m_ctrlCommands(m_ctrlHighlighter, mgrIcons, m_mgrPrompts, pMgrShortcuts),
	m_pMgrShortcuts(pMgrShortcuts), 
	m_bShowCommandIDs(FALSE)
{
}

CPreferencesShortcutsPage::~CPreferencesShortcutsPage()
{
}

void CPreferencesShortcutsPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CURHOTKEY, m_hkCur);
	DDX_Control(pDX, IDC_NEWHOTKEY, m_hkNew);
	DDX_Text(pDX, IDC_INUSE, m_sOtherCmdID);
	DDX_Check(pDX, IDC_SHOWCMDIDS, m_bShowCommandIDs);
}

BEGIN_MESSAGE_MAP(CPreferencesShortcutsPage, CPreferencesPageBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ASSIGNSHORTCUT, OnAssignshortcut)
	ON_BN_CLICKED(IDC_SHOWCMDIDS, OnShowCmdIDs)
	ON_BN_CLICKED(IDC_COPYALL, OnCopyall)
	ON_NOTIFY(TVN_SELCHANGED, IDC_COMMANDS, OnSelchangedShortcuts)
	ON_EN_CHANGE(IDC_NEWHOTKEY, OnChangeShortcut)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CTDLShortcutsTreeListCtrl, CTreeListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesShortcutsPage message handlers

BOOL CPreferencesShortcutsPage::OnInitDialog()  
{
	CPreferencesPageBase::OnInitDialog();
	
	CRect rCtrl = CDialogHelper::GetCtrlRect(this, IDC_COMMANDS_FRAME);
	VERIFY(m_ctrlCommands.Create(this, rCtrl, IDC_COMMANDS));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesShortcutsPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();

	// show the 'copy all' button
	GetDlgItem(IDC_COPYALL)->ShowWindow(m_bShowCommandIDs ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_COPYALL)->EnableWindow(m_bShowCommandIDs);

	if (m_pMgrShortcuts)
	{
		m_ctrlCommands.BuildMenuTree();

		// init hotkey controls
		// note: we no longer pass in m_pShortcutMgr->GetInvalidComb() because
		// the hotkey control does a less than perfect job of respecting these
		m_hkNew.SetRules(0, 0);
	}
}

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
}

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

	SetGridLineColor(GetSysColor(COLOR_3DSHADOW));
	SetSplitBarWidth(1);

	return 0;
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

	CTLSHoldResync hr(*this);

	m_tree.SetRedraw(FALSE);
	m_tree.DeleteAllItems();
	m_tree.SetIndent(ICON_OFFSET);

	CWaitCursor cursor;
	HTREEITEM htiFirst = NULL;

	CTDCMainMenu menu;

	if (menu.LoadMenu())
	{
		RemoveUnusedDefaultFilterItems(menu);

		for (int nPos = 0; nPos < (int)menu.GetMenuItemCount(); nPos++)
		{
			HTREEITEM hti = AddMenuItem(TVI_ROOT, &menu, nPos, !m_ctrlHighlighter.HasSearch());

			if (!htiFirst)
				htiFirst = hti;
		}
	}

	// add miscellaneous un-editable shortcuts
	AddMiscShortcuts();

	ExpandAll();
	m_tree.SetRedraw(TRUE);

	if (htiFirst)
		m_tree.EnsureVisible(htiFirst);

	if (m_tree.GetCount() == 0)
		m_mgrPrompts.SetPrompt(m_tree, CEnString(IDS_PSP_NOMATCHES), TVM_GETCOUNT);

	m_fonts.Initialise(m_tree);

 	SwapSides();
	RecalcColumnsToFit();
}

HTREEITEM CTDLShortcutsTreeListCtrl::AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, int nPos, BOOL bForceAdd)
{
	UINT nCmdID = pMenu->GetMenuItemID(nPos);
	BOOL bSubMenu = (nCmdID == ID_SUBMENU);

	// Exclude the debug menu
	if (bSubMenu)
	{
		if (pMenu->GetSubMenu(nPos)->GetMenuItemID(0) == ID_DEBUG_ENDSESSION)
			return NULL;
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
				AddMenuItem(hti, pSubMenu, nSubPos, bForceAdd); // RECURSIVE CALL
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
			m_mapShortcut2HTI[dwShortcut] = hti;
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
	BOOL bSubMenu = (nCmdID == ID_SUBMENU);

	if (!bSubMenu && !bForceAdd && !WantItem(sItem))
		return NULL;
	
	HTREEITEM hti = m_tree.InsertItem(sItem, -1, -1, nCmdID, htiParent, TVI_LAST);
	ASSERT(hti);

	//m_tree.SetItemData(hti, nCmdID);

	return hti;
}

void CTDLShortcutsTreeListCtrl::AddMiscShortcuts()
{
	if (!NUM_MISCSHORTCUTS)
		return;

	// Add parent placeholder
	CEnString sSubMenuText(IDS_MISCSHORTCUTS);

	BOOL bForceAdd = !m_ctrlHighlighter.HasSearch();
	HTREEITEM htiParent = InsertItem(sSubMenuText, ID_SUBMENU, TVI_ROOT, bForceAdd);

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
				m_mapShortcut2HTI[dwShortcut] = hti;
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

void CPreferencesShortcutsPage::OnSelchangedShortcuts(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	UINT nCmdID = (UINT)pNMTreeView->itemNew.lParam;
	DWORD dwShortcut = 0;

	m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

	WORD wVKeyCode = LOWORD(dwShortcut);
	WORD wModifiers = HIWORD(dwShortcut);

	m_hkCur.SetHotKey(wVKeyCode, wModifiers);
	m_hkNew.SetHotKey(wVKeyCode, wModifiers);

	// if it's a misc item or a sub-menu then disable keys
	BOOL bSubMenu = (pNMTreeView->itemNew.lParam == ID_SUBMENU);
	BOOL bCanHaveShortcut = (!bSubMenu && nCmdID && !IsMiscCommandID(nCmdID));

	m_hkNew.EnableWindow(bCanHaveShortcut);
	GetDlgItem(IDC_CURLABEL)->EnableWindow(bCanHaveShortcut);
	GetDlgItem(IDC_NEWLABEL)->EnableWindow(bCanHaveShortcut);

	// test for reserved shortcut
	// and disable assign button as feedback
	if (bCanHaveShortcut && CToDoCtrl::IsReservedShortcut(dwShortcut))
	{
		bCanHaveShortcut = FALSE;
		m_sOtherCmdID.LoadString(IDS_PSP_RESERVED);
	}
	else
	{
		m_sOtherCmdID.Empty();
	}

	GetDlgItem(IDC_ASSIGNSHORTCUT)->EnableWindow(bCanHaveShortcut);
*/
	UpdateData(FALSE);
	
	*pResult = 0;
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

void CPreferencesShortcutsPage::OnOK()
{
	CPreferencesPageBase::OnOK();

	m_ctrlCommands.SaveToShortcutMgr();
}

BOOL CPreferencesShortcutsPage::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// eat this if the hotkeyctrl has the focus
	if (GetFocus() == &m_hkNew)
		return FALSE;

	// else
	return CPreferencesPageBase::OnHelpInfo(pHelpInfo);
}

UINT CTDLShortcutsTreeListCtrl::GetSelectedCmdID() const
{
	return GetSelectedItemData();
}

UINT CTDLShortcutsTreeListCtrl::GetSelectedShortcut() const
{
	HTREEITEM hti = GetSelectedItem();
	UINT nCmdID = GetItemData(hti);
	DWORD dwShortcut = 0;

	return (m_mapID2Shortcut.Lookup(nCmdID, dwShortcut) ? dwShortcut : 0);
}

BOOL CTDLShortcutsTreeListCtrl::AssignShortcut(UINT nCmdID, DWORD dwShortcut)
{
	if (!nCmdID)
		return FALSE;

	// remove any shortcut currently assigned to nCmdID
	DWORD dwPrevSC = 0;

	if (m_mapID2Shortcut.Lookup(nCmdID, dwPrevSC))
		m_mapShortcut2HTI.RemoveKey(dwPrevSC);

	// handle special case where user is explicitly deleting a shortcut
	if (!dwShortcut)
	{
		dwShortcut = NO_SHORTCUT;
	}
	else
	{
		// else if anyone has this shortcut we must remove their mapping first
		HTREEITEM htiOther = NULL;

		if (m_mapShortcut2HTI.Lookup(dwShortcut, htiOther) && htiOther)
		{
			UINT nOtherCmdID = GetItemData(htiOther);

			if (nOtherCmdID)
				m_mapID2Shortcut.RemoveKey(nOtherCmdID);
		}
	}

	// update maps
	m_mapID2Shortcut[nCmdID] = dwShortcut;

	HTREEITEM htiSel = GetTreeItem((DWORD)nCmdID);

	if (dwShortcut != NO_SHORTCUT)
		m_mapShortcut2HTI[dwShortcut] = htiSel;

	CTreeCtrlHelper(m_tree).InvalidateItem(htiSel);
	return TRUE;
}

void CPreferencesShortcutsPage::OnAssignshortcut() 
{
	UINT nCmdID = m_ctrlCommands.GetSelectedShortcut();

	if (nCmdID)
	{
		WORD wVKeyCode = 0, wModifiers = 0;
		m_hkNew.GetHotKey(wVKeyCode, wModifiers);

		DWORD dwShortcut = MAKELONG(wVKeyCode, wModifiers);

		if (m_ctrlCommands.AssignShortcut(nCmdID, dwShortcut))
		{
			if (dwShortcut)
				m_hkCur.SetHotKey(LOWORD(dwShortcut), HIWORD(dwShortcut));

			m_sOtherCmdID.Empty();
			UpdateData(FALSE);
		}
	}

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesShortcutsPage::OnChangeShortcut()
{
/*
	HTREEITEM htiSel = m_tcCommands.GetSelectedItem();

	if (!htiSel)
		return;

	UINT nCmdID = m_tcCommands.GetItemData(htiSel);

	WORD wVKeyCode = 0, wModifiers = 0;
	m_hkNew.GetHotKey(wVKeyCode, wModifiers);

	// validate modifiers but only if a 'main' key has been pressed
	if (wVKeyCode)
	{
		WORD wValidModifiers = m_pMgrShortcuts->ValidateModifiers(wModifiers, wVKeyCode);

		if (wValidModifiers != wModifiers)
		{
			wModifiers = wValidModifiers;
			m_hkNew.SetHotKey(wVKeyCode, wModifiers);
		}
	}

	DWORD dwShortcut = MAKELONG(wVKeyCode, wModifiers);

	// if anyone has this shortcut we show who it is
	BOOL bReserved = FALSE;
	HTREEITEM htiOther = NULL;

	m_mapShortcut2HTI.Lookup(dwShortcut, htiOther);
	m_sOtherCmdID.Empty();

	if (CToDoCtrl::IsReservedShortcut(dwShortcut))
	{
		m_sOtherCmdID.LoadString(IDS_PSP_RESERVED);
		bReserved = TRUE;
	}
	else if (htiOther && m_tcCommands.GetItemData(htiOther) != nCmdID)
	{
		m_sOtherCmdID.Format(IDS_PSP_CURRENTLYASSIGNED, m_tcCommands.GetItemText(htiOther));
	}

	GetDlgItem(IDC_ASSIGNSHORTCUT)->EnableWindow(!bReserved);
	UpdateData(FALSE);
*/

	CPreferencesPageBase::OnControlChange();
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
	
//	if (m_bShowCommandIDs)
	{
		nMaxIDWidth = (dc.GetTextExtent(_T("12345")).cx + (2 * TEXT_PADDING));

		m_listHeader.SetItemWidth(1, nMaxIDWidth);
	}

	SetSplitPos(nMaxShortcutWidth + (m_bShowCommandIDs ? nMaxIDWidth : 0));

	// Tree
	CRect rClient;
	GetClientRect(rClient);

	m_treeHeader.SetItemWidth(0, rClient.Width() - nMaxIDWidth - nMaxShortcutWidth);
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
			BOOL bSubMenu = (nCmdID == ID_SUBMENU);

			DWORD dwShortcut = 0;
			m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

			// draw item bkgnd and gridlines full width of list
			COLORREF crBack = GetSysColor(bSubMenu ? COLOR_3DFACE : COLOR_WINDOW);
			pLVCD->clrTextBk = pLVCD->clrText = crBack;

			CRect rFullWidth(pLVCD->nmcd.rc);
			rFullWidth.right = 2000;

			GM_ITEMSTATE nState = CTreeListCtrl::GetItemState(nItem);
			BOOL bSelected = (nState != GMIS_NONE);

			// Fill bkgnd before drawing gridline
			if (bSubMenu && !bSelected)
				pDC->FillSolidRect(rFullWidth, crBack);

			// draw horz gridline before selection
			DrawHorzItemDivider(pDC, pLVCD->nmcd.rc);

			// Draw selection before text
			DWORD dwDrawFlags = (GMIB_THEMECLASSIC | GMIB_CLIPLEFT | GMIB_PREDRAW | GMIB_POSTDRAW);

			if (bSelected)
				GraphicsMisc::DrawExplorerItemSelection(pDC, m_list, nState, rFullWidth, dwDrawFlags);

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
	if (dwItemData == ID_SUBMENU)
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

	BOOL bBold = (dwItemData == ID_SUBMENU);
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

BOOL CPreferencesShortcutsPage::PreTranslateMessage(MSG* pMsg) 
{
	// special handling for hotkeys
	if (CWinClasses::IsClass(pMsg->hwnd, WC_HOTKEY))
		return FALSE;

	// handle delete key when tree has the focus
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_DELETE:
			{
/*
				if (GetFocus() == &m_tcCommands)
				{
					HTREEITEM htiSel = m_tcCommands.GetSelectedItem();

					if (htiSel)
					{
						UINT nCmdID = m_tcCommands.GetItemData(htiSel);

						if (nCmdID)
						{
							DWORD dwShortcut = 0;

							if (m_mapID2Shortcut.Lookup(nCmdID, dwShortcut))
								m_mapShortcut2HTI.RemoveKey(dwShortcut);

							m_mapID2Shortcut[nCmdID] = 0;
						}
					}

					m_hkCur.SetHotKey(0, 0);
					m_hkNew.SetHotKey(0, 0);
					m_sOtherCmdID.Empty();

					m_tcCommands.RecalcGutter();
					m_tcCommands.RedrawGutter();
				}
*/
			}
			break;
		}
		break;
	}
	
	return CPreferencesPageBase::PreTranslateMessage(pMsg);
}

void CPreferencesShortcutsPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR /*szKey*/)
{
	m_bShowCommandIDs = pPrefs->GetProfileInt(_T("KeyboardShortcuts"), _T("ShowCommandIDs"), FALSE);
	m_ctrlCommands.ShowCommandIDs(m_bShowCommandIDs);
}

void CPreferencesShortcutsPage::SavePreferences(IPreferences* pPrefs, LPCTSTR /*szKey*/) const
{
	if (m_pMgrShortcuts)
		m_pMgrShortcuts->SaveSettings(pPrefs, _T("KeyboardShortcuts"));

	pPrefs->WriteProfileInt(_T("KeyboardShortcuts"), _T("ShowCommandIDs"), m_bShowCommandIDs);
}

void CPreferencesShortcutsPage::OnShowCmdIDs() 
{
	UpdateData();

	// show the 'copy all' button
	GetDlgItem(IDC_COPYALL)->ShowWindow(m_bShowCommandIDs ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_COPYALL)->EnableWindow(m_bShowCommandIDs);

	m_ctrlCommands.ShowCommandIDs(m_bShowCommandIDs);
}

void CPreferencesShortcutsPage::OnCopyall() 
{
	m_ctrlCommands.CopyAllToClipboard();
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

void CPreferencesShortcutsPage::OnSize(UINT nType, int cx, int cy) 
{
	CPreferencesPageBase::OnSize(nType, cx, cy);
	
	if (m_ctrlCommands.GetSafeHwnd())
	{
		// calculate border
		CRect rCmds = CDialogHelper::GetChildRect(&m_ctrlCommands);
		CPoint ptBorders = rCmds.TopLeft();
		
		// calc offsets
		int nXOffset = cx - (CDialogHelper::GetCtrlRect(this, IDC_NEWHOTKEY).right + ptBorders.x);
		int nYOffset = cy - (rCmds.bottom + ptBorders.y);

		// move controls
		CDialogHelper::OffsetCtrl(this, IDC_CURLABEL, nXOffset, 0);
		CDialogHelper::OffsetCtrl(this, IDC_CURHOTKEY, nXOffset, 0);
		CDialogHelper::OffsetCtrl(this, IDC_NEWLABEL, nXOffset, 0);
		CDialogHelper::OffsetCtrl(this, IDC_NEWHOTKEY, nXOffset, 0);
		CDialogHelper::OffsetCtrl(this, IDC_ASSIGNSHORTCUT, nXOffset, 0);
		CDialogHelper::OffsetCtrl(this, IDC_INUSE, nXOffset, 0);
		
		CDialogHelper::OffsetCtrl(this, IDC_COPYALL, nXOffset, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_SHOWCMDIDS, nXOffset, nYOffset);

		CDialogHelper::ResizeChild(&m_ctrlCommands, nXOffset, nYOffset);
	}
}

BOOL CPreferencesShortcutsPage::RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs)
{
	// CANNOT call this whilst preferences are showing because
	// we want to avoid having also to fix-up the temporary mappings
	if (GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}
			
	return m_pMgrShortcuts->RemapMenuItemIDs(mapCmdIDs);
}

int CPreferencesShortcutsPage::HighlightUIText(const CStringArray& aSearch, COLORREF crHighlight)
{
	int nNumHighlights = CPreferencesPageBase::HighlightUIText(aSearch, crHighlight);

	if (!m_bFirstShow)
		m_ctrlCommands.BuildMenuTree();

	return (nNumHighlights + 1); // always report tree as highlighted
}

void CPreferencesShortcutsPage::ClearHighlights()
{
	CPreferencesPageBase::ClearHighlights();

	if (!m_bFirstShow)
		m_ctrlCommands.BuildMenuTree();
}

