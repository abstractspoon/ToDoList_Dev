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
// #include "..\shared\holdredraw.h"
// #include "..\shared\treectrlhelper.h"
// #include "..\shared\misc.h"
// #include "..\shared\graphicsmisc.h"
// #include "..\shared\clipboard.h"
// #include "..\shared\themed.h"
// #include "..\shared\AcceleratorString.h"
#include "..\shared\ShortcutManager.h"

#include "..\3rdParty\XNamedColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// enum 
// {
// 	COL_SHORTCUT = 0,
// 	COL_CMDID,
// };
// 
// #define PSP_SHORTCUTCOLUMNID	(OTC_POSCOLUMNID - 1)
// #define PSP_COMMANDIDCOLUMNID	(OTC_POSCOLUMNID - 2)
// 
// #define ID_SUBMENU ((UINT)-1)
// 
// const int TEXT_PADDING			= 3;
// const int SHORTCUTCOL_MINWIDTH	= 75;
// const int ICON_OFFSET			= GraphicsMisc::ScaleByDPIFactor(20);

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

UINT CPreferencesShortcutsPage::GetShortcutCmdID(DWORD dwShortcut) const 
{ 
	return m_pMgrShortcuts->GetCommandID(dwShortcut); 
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

