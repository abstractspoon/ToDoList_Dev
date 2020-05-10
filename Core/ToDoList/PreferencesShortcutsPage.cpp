// PrefererencesShortcutsPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesShortcutsPage.h"
#include "todoctrl.h"
#include "tdcstatic.h"
#include "TDCMainMenu.h"

#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\enstring.h"
#include "..\shared\holdredraw.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\misc.h"
#include "..\shared\clipboard.h"
#include "..\shared\themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define PSP_SHORTCUTCOLUMNID	(OTC_POSCOLUMNID - 1)
#define PSP_COMMANDIDCOLUMNID	(OTC_POSCOLUMNID - 2)

const COLORREF SUBMENU_COLOR = RGB(192, 192, 192);

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
	m_tcCommands.AddGutterColumn(PSP_COMMANDIDCOLUMNID, _T("ID"), 0, DT_RIGHT);
	m_tcCommands.SetGutterColumnHeaderTitle(NCG_CLIENTCOLUMNID, CEnString(IDS_PSP_MENUITEM));
	m_tcCommands.ShowGutterPosColumn(FALSE);
	m_tcCommands.SetGridlineColor(OTC_GRIDCOLOR);
	m_tcCommands.EnableGutterColumnHeaderClicking(PSP_SHORTCUTCOLUMNID, FALSE);
	m_tcCommands.EnableGutterColumnHeaderClicking(PSP_COMMANDIDCOLUMNID, FALSE);
	m_tcCommands.EnableGutterColumnHeaderClicking(NCG_CLIENTCOLUMNID, FALSE);
	
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
		m_tcCommands.SendMessage(WM_NULL);
		m_tcCommands.SetRedraw(FALSE);

		CWaitCursor cursor;
		HTREEITEM htiFirst = NULL;

		CTDCMainMenu menu;

		if (menu.LoadMenu())
		{
			for (int nPos = 0; nPos < (int)menu.GetMenuItemCount(); nPos++)
			{
				HTREEITEM hti = AddMenuItem(NULL, &menu, nPos);

				if (!htiFirst)
					htiFirst = hti;
			}
		}
		
		m_tcCommands.ExpandAll();
		m_tcCommands.SetRedraw(TRUE);

		if (m_bShowCommandIDs)
			m_tcCommands.RecalcGutterColumn(PSP_COMMANDIDCOLUMNID);

		// add miscellaneous un-editable shortcuts
		AddMiscShortcuts();

		if (htiFirst)
			m_tcCommands.EnsureVisible(htiFirst);

		// init hotkey controls
		// note: we no longer pass in m_pShortcutMgr->GetInvalidComb() because
		// the hotkey control does a less than perfect job of respecting these
		m_hkNew.SetRules(0, 0);
	}
}

HTREEITEM CPreferencesShortcutsPage::AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, int nPos)
{
	// Exclude the debug menu
	if (pMenu->GetMenuItemID(nPos) == (UINT)-1)
	{
		if (pMenu->GetSubMenu(nPos)->GetMenuItemID(0) == ID_DEBUGENDSESSION)
			return NULL;
	}

	CString sItem;
	pMenu->GetMenuString(nPos, sItem, MF_BYPOSITION);

	// remove '&'
	sItem.Replace(_T("&&"), _T("~~~"));
	sItem.Replace(_T("&"), _T(""));
	sItem.Replace(_T("~~~"), _T("&"));
	
	// remove everything after '\t'
	int nTab = sItem.Find('\t');
	
	if (nTab >= 0)
		sItem = sItem.Left(nTab);
	
	if (!sItem.IsEmpty())
	{
		HTREEITEM hti = m_tcCommands.InsertItem(sItem, htiParent ? htiParent : TVI_ROOT);

		if (hti)
		{
			UINT nCmdID = pMenu->GetMenuItemID(nPos);
			
			if (nCmdID == (UINT)-1) // submenu
			{
				// make top level items bold
				if (!htiParent)
					m_tcCommands.SetItemState(hti, TVIS_BOLD, TVIS_BOLD);

				CMenu* pSubMenu = pMenu->GetSubMenu(nPos);

				if (pSubMenu)
				{
					for (int nSubPos = 0; nSubPos < (int)pSubMenu->GetMenuItemCount(); nSubPos++)
						AddMenuItem(hti, pSubMenu, nSubPos); // RECURSIVE CALL
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

				m_tcCommands.SetItemData(hti, nCmdID);
			}

			return hti;
		}
	}

	return NULL;
}

void CPreferencesShortcutsPage::AddMiscShortcuts()
{
	if (!NUM_MISCSHORTCUTS)
		return;

	// Add parent placeholder
	HTREEITEM htiParent = m_tcCommands.InsertItem(CEnString(IDS_MISCSHORTCUTS), TVI_ROOT);
	m_tcCommands.SetItemState(htiParent, TVIS_BOLD, TVIS_BOLD);

	// add children
	for (int nItem = 0; nItem < NUM_MISCSHORTCUTS; nItem++)
	{
		DWORD dwShortcut = MISC_SHORTCUTS[nItem].dwShortcut;
		
		if (dwShortcut)
		{
			CEnString sMisc(MISC_SHORTCUTS[nItem].nIDShortcut);
			HTREEITEM hti = m_tcCommands.InsertItem(sMisc, htiParent);

			// make fake command IDs so it does not intersect with normal IDs
			UINT nCmdID = MAKELONG(0, nItem + 1);
			
			if (dwShortcut)
			{
				m_mapID2Shortcut[nCmdID] = dwShortcut;
				m_mapShortcut2HTI[dwShortcut] = hti;
			}
			
			m_tcCommands.SetItemData(hti, nCmdID);
		}		
	}
	
	// expand parent
	m_tcCommands.Expand(htiParent, TVE_EXPAND);
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
	BOOL bMisc = IsMiscCommandID(nCmdID);
	BOOL bCanHaveShortcut = !bMisc && !m_tcCommands.ItemHasChildren(pNMTreeView->itemNew.hItem);

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
		m_sOtherCmdID.Empty();

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
		BOOL bParentItem = m_tcCommands.ItemHasChildren(hti);

		if (bThemedSel)
		{
			BOOL bFocused = (GetFocus() == &m_tcCommands);
			DWORD dwFlags = GMIB_CLIPRIGHT;

			if (pNCGDI->nColID == PSP_COMMANDIDCOLUMNID)
				dwFlags |= GMIB_CLIPLEFT;

			GraphicsMisc::DrawExplorerItemSelection(pNCGDI->pDC, m_tcCommands, (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED), rItem, dwFlags, &rItem);
		}
		else if (bParentItem)
		{
			pNCGDI->pDC->FillSolidRect(rItem, SUBMENU_COLOR);
		}

		// draw text
		if (!bParentItem)
		{
			UINT nCmdID = m_tcCommands.GetItemData(hti);
			pNCGDI->pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

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

							rItem.left += 3;
							pNCGDI->pDC->DrawText(sText, rItem, (DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX));
							rItem.left -= 3;
						}
					}
				}
				break;

			case PSP_COMMANDIDCOLUMNID:
				if (nCmdID != 0)
				{
					CString sText = Misc::Format((DWORD)nCmdID);

					rItem.right -= 3;
					pNCGDI->pDC->DrawText(sText, rItem, (DT_SINGLELINE | DT_VCENTER | DT_RIGHT | DT_NOPREFIX));
					rItem.right += 3;
				}
				break;
			}
			
			// vertical divider
			if (!bThemedSel)
				pNCGDI->pDC->FillSolidRect(rItem.right - 1, rItem.top, 1, rItem.Height(), m_tcCommands.GetGridlineColor());
		}

		// horz divider
		if (!bThemedSel)
		{
			rItem.top = rItem.bottom - 1;
			pNCGDI->pDC->FillSolidRect(rItem, m_tcCommands.GetGridlineColor());
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
					nLongest += 6; // some padding

				pNCRC->nWidth = max(40, nLongest);
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
	NMTVCUSTOMDRAW* pTVCD = (NMTVCUSTOMDRAW*)pNMHDR;

	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult |= CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;	
		break;
		
	case CDDS_ITEMPREPAINT:
		{
			// set colors
			HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
			
			if (m_tcCommands.ItemHasChildren(hti)) // popup menu
			{
				CRect rItem(pTVCD->nmcd.rc), rText;
				m_tcCommands.GetItemRect(hti, rText, TRUE);
				rItem.left = (rText.left - 2);
				
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
				pDC->FillSolidRect(rItem, SUBMENU_COLOR);
				
				pTVCD->clrTextBk = SUBMENU_COLOR;
				pTVCD->clrText = 0;
				
				*pResult |= CDRF_NEWFONT;
			}
			else if (pTVCD->nmcd.uItemState & CDIS_SELECTED)
			{
				if (!CThemed::AreControlsThemed())
				{
					CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
					BOOL bFocused = (GetFocus() == &m_tcCommands);

					GraphicsMisc::DrawExplorerItemSelection(pDC, m_tcCommands, (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED), pTVCD->nmcd.rc, GMIB_CLIPLEFT, &pTVCD->nmcd.rc);
				}
				else
				{
					pTVCD->clrTextBk = GetSysColor(COLOR_HIGHLIGHT);
					pTVCD->clrText = GetSysColor(COLOR_HIGHLIGHTTEXT);
				}
				
				*pResult |= CDRF_NEWFONT;
			}
			else // test for reserved shortcut
			{
				DWORD dwShortcut = 0;
				UINT nCmdID = pTVCD->nmcd.lItemlParam;
				m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);
				
				if (CToDoCtrl::IsReservedShortcut(dwShortcut) && !IsMiscCommandID(nCmdID))
				{
					pTVCD->clrText = 255;
					*pResult |= CDRF_NEWFONT;
				}
			}
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
	
	// Take a copy of the current mapping because we 
	// are going to be making in-situ changes
	CMap<UINT, UINT, DWORD, DWORD&> mapOldShortcuts;

	if (!m_pShortcutMgr->CopyShortcuts(mapOldShortcuts))
	{
		ASSERT(m_pShortcutMgr->IsEmpty());
		return FALSE;
	}
	
	// Work through the old mappings updating them to their new positions
	POSITION pos = mapCmdIDs.GetStartPosition();
	BOOL bRemapped = FALSE;

	while (pos)
	{
		UINT nOldCmdID, nNewCmdID;
		mapCmdIDs.GetNextAssoc(pos, nOldCmdID, nNewCmdID);

		if (nNewCmdID != nOldCmdID)
		{
			// Always remove the old menu ID
			m_pShortcutMgr->DeleteShortcut(nOldCmdID);

			// Point the shortcut to the new menu ID if it hasn't been deleted
			DWORD dwShortcut;

			if (mapOldShortcuts.Lookup(nOldCmdID, dwShortcut) && dwShortcut)
				m_pShortcutMgr->SetShortcut(nNewCmdID, dwShortcut);

			bRemapped = TRUE;
		}
	}
		
	return bRemapped;
}
