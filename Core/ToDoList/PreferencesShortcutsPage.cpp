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
#include "..\shared\enstring.h"
#include "..\shared\holdredraw.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\misc.h"
#include "..\shared\clipboard.h"
#include "..\shared\themed.h"
#include "..\shared\AcceleratorString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define PSP_SHORTCUTCOLUMNID	(OTC_POSCOLUMNID - 1)
#define PSP_COMMANDIDCOLUMNID	(OTC_POSCOLUMNID - 2)

#define ID_SUBMENU ((UINT)-1)

const COLORREF SUBMENU_COLOR	= RGB(192, 192, 192);
const int TEXT_PADDING			= 3;
const int SHORTCUTCOL_MINWIDTH	= 75;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesShortcutsPage property page

CPreferencesShortcutsPage::CPreferencesShortcutsPage(CShortcutManager* pMgr) 
	: CPreferencesPageBase(CPreferencesShortcutsPage::IDD), 
	m_pShortcutMgr(pMgr), 
	m_tcCommands(NCGS_SHOWHEADER)
{
	//{{AFX_DATA_INIT(CPreferencesShortcutsPage)
	m_sOtherCmdID = _T("");
	m_bShowCommandIDs = FALSE;
	//}}AFX_DATA_INIT

	m_tcCommands.AddGutterColumn(PSP_SHORTCUTCOLUMNID, CEnString(IDS_PSP_SHORTCUT));
	m_tcCommands.AddGutterColumn(PSP_COMMANDIDCOLUMNID, CEnString(_T("ID")), 0, DT_CENTER);
	m_tcCommands.SetGutterColumnHeaderTitle(NCG_CLIENTCOLUMNID, CEnString(IDS_PSP_MENUITEM));
	m_tcCommands.ShowGutterPosColumn(FALSE);
	m_tcCommands.SetGridlineColor(OTC_GRIDCOLOR);
	m_tcCommands.EnableGutterColumnHeaderClicking(PSP_SHORTCUTCOLUMNID, FALSE);
	m_tcCommands.EnableGutterColumnHeaderClicking(PSP_COMMANDIDCOLUMNID, FALSE);
	m_tcCommands.EnableGutterColumnHeaderClicking(NCG_CLIENTCOLUMNID, FALSE);
	m_tcCommands.SetParentHandlesCustomDraw(TRUE);
}

CPreferencesShortcutsPage::~CPreferencesShortcutsPage()
{
}

void CPreferencesShortcutsPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesShortcutsPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CURHOTKEY, m_hkCur);
	DDX_Control(pDX, IDC_COMMANDS, m_tcCommands);
	DDX_Control(pDX, IDC_NEWHOTKEY, m_hkNew);
	DDX_Text(pDX, IDC_INUSE, m_sOtherCmdID);
	DDX_Check(pDX, IDC_SHOWCMDIDS, m_bShowCommandIDs);
}

BEGIN_MESSAGE_MAP(CPreferencesShortcutsPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesShortcutsPage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ASSIGNSHORTCUT, OnAssignshortcut)
	ON_BN_CLICKED(IDC_SHOWCMDIDS, OnShowCmdIDs)
	ON_BN_CLICKED(IDC_COPYALL, OnCopyall)
	ON_NOTIFY(TVN_SELCHANGED, IDC_COMMANDS, OnSelchangedShortcuts)
	ON_EN_CHANGE(IDC_NEWHOTKEY, OnChangeShortcut)
	ON_REGISTERED_MESSAGE(WM_NCG_DRAWITEMCOLUMN, OnGutterDrawItem)
	ON_REGISTERED_MESSAGE(WM_NCG_POSTDRAWITEM, OnGutterPostDrawItem)
	ON_REGISTERED_MESSAGE(WM_NCG_RECALCCOLWIDTH, OnGutterRecalcColWidth)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COMMANDS, OnTreeCustomDraw)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesShortcutsPage message handlers

BOOL CPreferencesShortcutsPage::OnInitDialog()  
{
	CPreferencesPageBase::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesShortcutsPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();

	// show the 'copy all' button
	GetDlgItem(IDC_COPYALL)->ShowWindow(m_bShowCommandIDs ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_COPYALL)->EnableWindow(m_bShowCommandIDs);

	if (m_pShortcutMgr)
	{
		m_fonts.Initialise(*this);

		BuildMenuTree();

		// init hotkey controls
		// note: we no longer pass in m_pShortcutMgr->GetInvalidComb() because
		// the hotkey control does a less than perfect job of respecting these
		m_hkNew.SetRules(0, 0);
	}
}

void CPreferencesShortcutsPage::BuildMenuTree()
{
	ASSERT(m_pShortcutMgr);

	CHoldRedraw ht(*this);

	m_tcCommands.SendMessage(WM_NULL);
	m_tcCommands.SetRedraw(FALSE);
	m_tcCommands.DeleteAllItems();

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

	if (m_bShowCommandIDs)
		m_tcCommands.RecalcGutterColumn(PSP_COMMANDIDCOLUMNID);

	// add miscellaneous un-editable shortcuts
	AddMiscShortcuts();

	m_tcCommands.ExpandAll();
	m_tcCommands.SetRedraw(TRUE);

	if (htiFirst)
		m_tcCommands.EnsureVisible(htiFirst);

	if (m_tcCommands.GetCount() == 0)
		m_mgrPrompts.SetPrompt(m_tcCommands, CEnString(IDS_PSP_NOMATCHES), TVM_GETCOUNT);
}

HTREEITEM CPreferencesShortcutsPage::AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, int nPos, BOOL bForceAdd)
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
		bForceAdd = WantItem(m_tcCommands.GetItemText(htiParent));

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
			m_tcCommands.DeleteItem(hti);
		}
	}
	else if (!IsMiscCommandID(nCmdID)) // fixes a bug where misc ids were being saved
	{
		DWORD dwShortcut = m_pShortcutMgr->GetShortcut(nCmdID);

		if (dwShortcut)
		{
			m_mapID2Shortcut[nCmdID] = dwShortcut;
			m_mapShortcut2HTI[dwShortcut] = hti;
		}
	}

	return hti;
}

BOOL CPreferencesShortcutsPage::WantKeepSubmenu(HTREEITEM hti) const
{
	if (m_tcCommands.ItemHasChildren(hti))
		return TRUE;

	return WantItem(m_tcCommands.GetItemText(hti));
}

BOOL CPreferencesShortcutsPage::WantItem(const CString& sItem) const
{
	if (!m_ctrlHighlighter.HasSearch())
		return TRUE;
	
	return m_ctrlHighlighter.TextContainsOneOf(sItem);
}

BOOL CPreferencesShortcutsPage::MatchesSearch(const CString& sItem) const
{
	if (!m_ctrlHighlighter.HasSearch())
		return FALSE;
	
	return m_ctrlHighlighter.TextContainsOneOf(sItem);
}

HTREEITEM CPreferencesShortcutsPage::InsertItem(const CString& sItem, UINT nCmdID, HTREEITEM htiParent, BOOL bForceAdd)
{
	// Exclude leaf tasks not matching the search terms
	// unless their parent matches the search
	BOOL bSubMenu = (nCmdID == ID_SUBMENU);

	if (!bSubMenu && !bForceAdd && !WantItem(sItem))
		return NULL;
	
	HTREEITEM hti = m_tcCommands.InsertItem(sItem, htiParent);
	ASSERT(hti);

	m_tcCommands.SetItemData(hti, nCmdID);

	if (htiParent == TVI_ROOT)
	{
		ASSERT(bSubMenu);
		m_tcCommands.SetItemState(hti, TVIS_BOLD, TVIS_BOLD);
	}

	return hti;
}

void CPreferencesShortcutsPage::AddMiscShortcuts()
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
		m_tcCommands.DeleteItem(htiParent);
	}
}

void CPreferencesShortcutsPage::RemoveUnusedDefaultFilterItems(CMenu& menu) const
{
	HMENU hFilterMenu = CEnMenu::GetSubMenu(menu, ID_VIEW_ACTIVATEFILTER1);
	ASSERT(hFilterMenu);

	int nNumFilters = CTDCFilter::GetDefaultFilterNames().GetSize();

	for (int nFilter = nNumFilters; nFilter < 24; nFilter++)
		::DeleteMenu(hFilterMenu, (ID_VIEW_ACTIVATEFILTER1 + nFilter), MF_BYCOMMAND);
}

void CPreferencesShortcutsPage::OnSelchangedShortcuts(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	UINT nCmdID = (UINT)pNMTreeView->itemNew.lParam;
	DWORD dwShortcut = 0;

	m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

	WORD wVKeyCode = LOWORD(dwShortcut);
	WORD wModifiers = HIWORD(dwShortcut);

	m_hkCur.SetHotKey(wVKeyCode, wModifiers);
	m_hkNew.SetHotKey(wVKeyCode, wModifiers);

	// if it's a misc item then disable keys
	BOOL bCanHaveShortcut = (nCmdID && !IsMiscCommandID(nCmdID));

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
	UpdateData(FALSE);
	
	*pResult = 0;
}

BOOL CPreferencesShortcutsPage::IsMiscCommandID(UINT nCmdID)
{
	return (LOWORD(nCmdID) == 0) && (HIWORD(nCmdID) != 0);
}

void CPreferencesShortcutsPage::OnOK()
{
	CPreferencesPageBase::OnOK();

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
			m_pShortcutMgr->SetShortcut(nCmdID, dwShortcut);
	}
}

BOOL CPreferencesShortcutsPage::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// eat this if the hotkeyctrl has the focus
	if (GetFocus() == &m_hkNew)
		return FALSE;

	// else
	return CPreferencesPageBase::OnHelpInfo(pHelpInfo);
}

void CPreferencesShortcutsPage::OnAssignshortcut() 
{
	HTREEITEM htiSel = m_tcCommands.GetSelectedItem();

	if (!htiSel)
		return;
	
	UINT nCmdID = m_tcCommands.GetItemData(htiSel);

	if (nCmdID)
	{
		// remove any shortcut currently assigned to nCmdID
		DWORD dwPrevSC = 0;

		if (m_mapID2Shortcut.Lookup(nCmdID, dwPrevSC))
			m_mapShortcut2HTI.RemoveKey(dwPrevSC);

		WORD wVKeyCode = 0, wModifiers = 0;
		m_hkNew.GetHotKey(wVKeyCode, wModifiers);

		DWORD dwShortcut = MAKELONG(wVKeyCode, wModifiers);
		
		// handle special case where user is explicitly deleting a 
		// shortcut
		if (!dwShortcut)
			dwShortcut = NO_SHORTCUT;
		// else if anyone has this shortcut we must remove their mapping first
		else
		{
			HTREEITEM htiOther = NULL;

			if (m_mapShortcut2HTI.Lookup(dwShortcut, htiOther) && htiOther)
			{
				UINT nOtherCmdID = m_tcCommands.GetItemData(htiOther);

				if (nOtherCmdID)
					m_mapID2Shortcut.RemoveKey(nOtherCmdID);
			}
		}

		// update maps
		m_mapID2Shortcut[nCmdID] = dwShortcut;

		if (dwShortcut != NO_SHORTCUT)
		{
			m_mapShortcut2HTI[dwShortcut] = htiSel;
			m_hkCur.SetHotKey(LOWORD(dwShortcut), HIWORD(dwShortcut));
		}

		m_sOtherCmdID.Empty();

		m_tcCommands.RecalcGutter();
		m_tcCommands.RedrawGutter();

		CTreeCtrlHelper(m_tcCommands).InvalidateItem(htiSel);

		UpdateData(FALSE);
	}

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesShortcutsPage::OnChangeShortcut()
{
	HTREEITEM htiSel = m_tcCommands.GetSelectedItem();

	if (!htiSel)
		return;

	UINT nCmdID = m_tcCommands.GetItemData(htiSel);

	WORD wVKeyCode = 0, wModifiers = 0;
	m_hkNew.GetHotKey(wVKeyCode, wModifiers);

	// validate modifiers but only if a 'main' key has been pressed
	if (wVKeyCode)
	{
		WORD wValidModifiers = m_pShortcutMgr->ValidateModifiers(wModifiers, wVKeyCode);

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

	if (CToDoCtrl::IsReservedShortcut(dwShortcut))
	{
		m_sOtherCmdID.LoadString(IDS_PSP_RESERVED);
		bReserved = TRUE;
	}
	else if (htiOther && m_tcCommands.GetItemData(htiOther) != nCmdID)
	{
		m_sOtherCmdID.Format(IDS_PSP_CURRENTLYASSIGNED, m_tcCommands.GetItemText(htiOther));
	}
	else
		m_sOtherCmdID.Empty();

	GetDlgItem(IDC_ASSIGNSHORTCUT)->EnableWindow(!bReserved);
	UpdateData(FALSE);

	CPreferencesPageBase::OnControlChange();
}

LRESULT CPreferencesShortcutsPage::OnGutterDrawItem(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;

	if (pNCGDI->nColID != OTC_POSCOLUMNID)
	{
		CRect rItem(pNCGDI->rItem);
		HTREEITEM hti = (HTREEITEM)pNCGDI->dwItem;

		// draw background
		BOOL bThemedSel = ((hti == m_tcCommands.GetSelectedItem()) && CThemed::AreControlsThemed());
		BOOL bSubMenu = (m_tcCommands.GetItemData(hti) == ID_SUBMENU);

		if (bThemedSel)
		{
			BOOL bFocused = (GetFocus() == &m_tcCommands);
			DWORD dwFlags = GMIB_CLIPRIGHT;

			if (pNCGDI->nColID == PSP_COMMANDIDCOLUMNID)
				dwFlags |= GMIB_CLIPLEFT;

			GraphicsMisc::DrawExplorerItemSelection(pNCGDI->pDC, m_tcCommands, (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED), rItem, dwFlags, &rItem);
		}
		else
		{
			if (bSubMenu)
				pNCGDI->pDC->FillSolidRect(rItem, SUBMENU_COLOR);
			else
				GraphicsMisc::DrawVertLine(pNCGDI->pDC, rItem.top, rItem.bottom, rItem.right - 1, m_tcCommands.GetGridlineColor());

			GraphicsMisc::DrawHorzLine(pNCGDI->pDC, rItem.left, rItem.right, rItem.bottom - 1, m_tcCommands.GetGridlineColor());
		}

		// draw text
		if (!bSubMenu)
		{
			UINT nCmdID = m_tcCommands.GetItemData(hti);
			pNCGDI->pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

			rItem.left += TEXT_PADDING;

			switch (pNCGDI->nColID)
			{
			case PSP_SHORTCUTCOLUMNID:
				{
					DWORD dwShortcut = 0;
					m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

					if (dwShortcut)
					{
						CString sText = m_pShortcutMgr->GetShortcutText(dwShortcut);

						// test for reserved shortcut and mark in red
						if (!sText.IsEmpty())
						{
							if (CToDoCtrl::IsReservedShortcut(dwShortcut) && !IsMiscCommandID(nCmdID))
								pNCGDI->pDC->SetTextColor(255);

							pNCGDI->pDC->DrawText(sText, rItem, (DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX));
						}
					}
				}
				break;

			case PSP_COMMANDIDCOLUMNID:
				if (nCmdID && !IsMiscCommandID(nCmdID))
				{
					CString sText = Misc::Format((DWORD)nCmdID);

					pNCGDI->pDC->DrawText(sText, rItem, (DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX));
				}
				break;
			}
		}
	
		return TRUE; // we handled it
	}

	return FALSE;
}

LRESULT CPreferencesShortcutsPage::OnGutterPostDrawItem(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// we handle drawing of horz divider
	return TRUE;
}

LRESULT CPreferencesShortcutsPage::OnGutterRecalcColWidth(WPARAM /*wParam*/, LPARAM lParam)
{
	if (!CHoldRedraw::IsHeld(m_tcCommands))
	{
		NCGRECALCCOLUMN* pNCRC = (NCGRECALCCOLUMN*)lParam;

		switch (pNCRC->nColID)
		{
		case PSP_SHORTCUTCOLUMNID:
			{
				int nLongest = 0;
				HTREEITEM hti = m_tcCommands.GetNextItem(TVI_ROOT, TVGN_CHILD);

				while (hti)
				{
					int nWidth = GetLongestShortcutText(hti, pNCRC->pDC);
					nLongest = max(nLongest, nWidth);

					hti = m_tcCommands.GetNextItem(hti, TVGN_NEXT);
				}

				if (nLongest)
					nLongest += (2 * TEXT_PADDING);

				pNCRC->nWidth = max(SHORTCUTCOL_MINWIDTH, nLongest);
				return TRUE; // we handled it
			}
			break;

		case PSP_COMMANDIDCOLUMNID:
			if (m_bShowCommandIDs)
			{
				pNCRC->nWidth = pNCRC->pDC->GetTextExtent(_T("00000")).cx;
				return TRUE; // we handled it
			}
			break;
		}
	}

	return FALSE;
}

int CPreferencesShortcutsPage::GetLongestShortcutText(HTREEITEM hti, CDC* pDC)
{
	int nLongest = 0;
	int nCmdID = m_tcCommands.GetItemData(hti);

	if (nCmdID)
	{
		DWORD dwShortcut = 0;
		m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

		if (dwShortcut)
		{
			CString sShortcut = m_pShortcutMgr->GetShortcutText(dwShortcut);
			nLongest = sShortcut.IsEmpty() ? 0 : pDC->GetTextExtent(sShortcut).cx;
		}
	}

	HTREEITEM htiChild = m_tcCommands.GetChildItem(hti);

	while (htiChild)
	{
		int nWidth = GetLongestShortcutText(htiChild, pDC);
		nLongest = max(nLongest, nWidth);

		htiChild = m_tcCommands.GetNextItem(htiChild, TVGN_NEXT);
	}

	return nLongest;
}

void CPreferencesShortcutsPage::OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	NMTVCUSTOMDRAW* pTVCD = (NMTVCUSTOMDRAW*)pNMCD;

	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult |= CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		break;
		
	case CDDS_ITEMPREPAINT:
		{
			*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT;

			// Set back color
			// Note: we set text color to same as back color 
			// so default text rendering does not show
			BOOL bSubMenu = (pTVCD->nmcd.lItemlParam == ID_SUBMENU);

			if (bSubMenu)
				pTVCD->clrText = pTVCD->clrTextBk = SUBMENU_COLOR;
			else
				pTVCD->clrText = pTVCD->clrTextBk = GetSysColor(COLOR_WINDOW);
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		{
			CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
			HTREEITEM hti = (HTREEITEM)pNMCD->dwItemSpec;
			UINT nCmdID = pNMCD->lItemlParam;
			CString sItem = m_tcCommands.GetItemText(hti);

			// horz gridline
			pDC->FillSolidRect(pNMCD->rc.left, pNMCD->rc.bottom - 1, pNMCD->rc.right - pNMCD->rc.left, 1, m_tcCommands.GetGridlineColor());

			// Draw Text
			COLORREF crText = GetSysColor(COLOR_WINDOWTEXT);

			BOOL bThemedSel = FALSE;

			if (pTVCD->nmcd.uItemState & CDIS_SELECTED)
			{
				if (CThemed::AreControlsThemed())
				{
					GM_ITEMSTATE nState = ((GetFocus() == &m_tcCommands) ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED) ;
					GraphicsMisc::DrawExplorerItemSelection(pDC, m_tcCommands, nState, pTVCD->nmcd.rc, GMIB_CLIPLEFT, &pTVCD->nmcd.rc);

					crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, nState, 0);
					bThemedSel = TRUE;
				}
				else
				{
					crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
				}
			}

			DWORD dwShortcut = 0;
			m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

			if (dwShortcut && CToDoCtrl::IsReservedShortcut(dwShortcut) && !IsMiscCommandID(nCmdID))
			{
				pDC->SetTextColor(255);
			}
			
			CRect rText;
			m_tcCommands.GetItemRect(hti, rText, TRUE);

			if (MatchesSearch(sItem))
			{ 
				// don't draw over gridline
				rText.bottom--;

				if (bThemedSel)
					rText.top++;

				pDC->FillSolidRect(rText, m_ctrlHighlighter.GetColor());
			}

			BOOL bBold = (m_tcCommands.GetItemState(hti, TVIS_BOLD) & TVIS_BOLD);
			HGDIOBJ hOldFont = pDC->SelectObject(m_fonts.GetHFont(bBold, FALSE, FALSE, FALSE));

			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(sItem, rText, (DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX));
			pDC->SelectObject(hOldFont);

			*pResult |= CDRF_SKIPDEFAULT;
		}
		break;
	}
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
}

void CPreferencesShortcutsPage::SavePreferences(IPreferences* pPrefs, LPCTSTR /*szKey*/) const
{
	pPrefs->WriteProfileInt(_T("KeyboardShortcuts"), _T("ShowCommandIDs"), m_bShowCommandIDs);

	if (m_pShortcutMgr)
		m_pShortcutMgr->SaveSettings(pPrefs, _T("KeyboardShortcuts"));
}

void CPreferencesShortcutsPage::OnShowCmdIDs() 
{
	UpdateData();

	// show the 'copy all' button
	GetDlgItem(IDC_COPYALL)->ShowWindow(m_bShowCommandIDs ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_COPYALL)->EnableWindow(m_bShowCommandIDs);

	m_tcCommands.RecalcGutter();
}

void CPreferencesShortcutsPage::OnCopyall() 
{
	CString sOutput;
	
	if (CopyItem(TVI_ROOT, sOutput))
	{
		VERIFY(CClipboard(*this).SetText(sOutput));
	}
}

BOOL CPreferencesShortcutsPage::CopyItem(HTREEITEM hti, CString& sOutput)
{
	if (!hti)
		return FALSE;

	// copy self
	if (hti != TVI_ROOT)
	{
		if (!m_tcCommands.ItemHasChildren(hti))
		{
			// ignore Reserved menu commands
			DWORD dwCmdID = m_tcCommands.GetItemData(hti);
			
			if (IsMiscCommandID(dwCmdID))
				return FALSE;

			CString sItem = m_tcCommands.GetItemText(hti);
			ASSERT(!sItem.IsEmpty());

			if (!sItem.IsEmpty())
			{
				// build path by recursing up the parent chain
				HTREEITEM htiParent = m_tcCommands.GetParentItem(hti);

				while (htiParent)
				{
					CString sParent = m_tcCommands.GetItemText(htiParent);
					ASSERT(!sParent.IsEmpty());
					
					if (!sParent.IsEmpty())
						sItem = sParent + _T(" > ") + sItem;

					// parent's parent
					htiParent = m_tcCommands.GetParentItem(htiParent);
				}

				sOutput += Misc::Format(dwCmdID);
				sOutput += _T(" : ");
				sOutput += sItem;
				sOutput += _T("\r\n");
			}
		}
	}

	// then children
	CopyItem(m_tcCommands.GetChildItem(hti), sOutput);

	// then siblings
	if (hti != TVI_ROOT)
	{
		// add a spacer between top-level items
		if (m_tcCommands.GetParentItem(hti) == NULL)
			sOutput += _T("\r\n");

		CopyItem(m_tcCommands.GetNextItem(hti, TVGN_NEXT), sOutput);
	}

	return (!sOutput.IsEmpty());
}

void CPreferencesShortcutsPage::OnSize(UINT nType, int cx, int cy) 
{
	CPreferencesPageBase::OnSize(nType, cx, cy);
	
	if (m_tcCommands.GetSafeHwnd())
	{
		// calculate border
		CRect rCmds = CDialogHelper::GetChildRect(&m_tcCommands);
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

		CDialogHelper::ResizeChild(&m_tcCommands, nXOffset, nYOffset);
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
			
	return m_pShortcutMgr->RemapMenuItemIDs(mapCmdIDs);
}

int CPreferencesShortcutsPage::HighlightUIText(const CStringArray& aSearch, COLORREF crHighlight)
{
	int nNumHighlights = CPreferencesPageBase::HighlightUIText(aSearch, crHighlight);

	if (!m_bFirstShow)
		BuildMenuTree();

	return (nNumHighlights + 1); // always report tree as highlighted
}

void CPreferencesShortcutsPage::ClearHighlights()
{
	CPreferencesPageBase::ClearHighlights();

	if (!m_bFirstShow)
		BuildMenuTree();
}

