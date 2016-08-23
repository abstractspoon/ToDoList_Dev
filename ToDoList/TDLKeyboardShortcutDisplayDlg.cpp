// KeyboardShortcutDisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdlKeyboardShortcutDisplayDlg.h"

#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\clipboard.h"
#include "..\shared\themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyboardShortcutDisplayDlg dialog

CTDLKeyboardShortcutDisplayDlg::CTDLKeyboardShortcutDisplayDlg(const CStringArray& aMapping, TCHAR cDelim, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLKeyboardShortcutDisplayDlg::IDD, pParent), 
	m_aMapping(aMapping), 
	m_cDelim(cDelim)
{
	//{{AFX_DATA_INIT(CKeyboardShortcutDisplayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTDLKeyboardShortcutDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyboardShortcutDisplayDlg)
	DDX_Control(pDX, IDC_SHORTCUTS, m_lcShortcuts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLKeyboardShortcutDisplayDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CKeyboardShortcutDisplayDlg)
	ON_BN_CLICKED(IDC_COPYSHORTCUTS, OnCopyshortcuts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyboardShortcutDisplayDlg message handlers

BOOL CTDLKeyboardShortcutDisplayDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	if (m_aMapping.GetSize() == 0)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_lcShortcuts.InsertColumn(0, _T("Shortcut"));
	m_lcShortcuts.InsertColumn(1, _T("Menu Option"));

	for (int nItem = 0; nItem < m_aMapping.GetSize(); nItem++)
	{
		CString sShortcut = m_aMapping[nItem], sMenu;

		if (Misc::Split(sShortcut, sMenu, m_cDelim))
		{
			int nIndex = m_lcShortcuts.InsertItem(nItem, sShortcut);
			m_lcShortcuts.SetItemText(nIndex, 1, sMenu);
		}
		else
			m_lcShortcuts.InsertItem(nItem, _T(""));
	}

	m_lcShortcuts.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_lcShortcuts.SetColumnWidth(1, LVSCW_AUTOSIZE);

	if (m_il.Create(1, 16, ILC_COLOR, 1, 1))
		m_lcShortcuts.SetImageList(&m_il, LVSIL_SMALL);

//	ListView_SetExtendedListViewStyleEx(m_lcShortcuts, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);
	ListView_SetExtendedListViewStyleEx(m_lcShortcuts, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_lcShortcuts, LVS_EX_LABELTIP, LVS_EX_LABELTIP);
	
	CThemed::SetWindowTheme(&m_lcShortcuts, _T("Explorer"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLKeyboardShortcutDisplayDlg::OnCopyshortcuts() 
{
	CString sText, sLine;

	for (int nItem = 0; nItem < m_aMapping.GetSize(); nItem++)
	{
		CString sShortcut = m_aMapping[nItem], sMenu;

		if (Misc::Split(sShortcut, sMenu, m_cDelim))
		{
			sLine.Format(_T("%s\t\t%s\n"), sShortcut, sMenu);
			sText += sLine;
		}
		else
			sText += '\n';
	}

	VERIFY(CClipboard(*this).SetText(sText));
}
