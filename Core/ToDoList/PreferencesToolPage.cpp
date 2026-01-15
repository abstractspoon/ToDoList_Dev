// PreferencesToolPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesToolPage.h"
#include "tdcenum.h"
#include "TDLTaskIconDlg.h"

#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\enmenu.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\themed.h"
#include "..\shared\fileicons.h"
#include "..\shared\enlistctrl.h"

#include "..\3rdparty\ini.h"
#include "..\3rdparty\XNamedColors.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{
	COL_NAME,
	COL_PATH,
	COL_ARGS,
	COL_ICON,
};

/////////////////////////////////////////////////////////////////////////////

const int BTN_SELECTICON = 101;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesToolPage property page

CPreferencesToolPage::CPreferencesToolPage(int nMaxNumTools)
	: 
	CPreferencesPageBase(IDD_PREFTOOLS_PAGE),
	m_eToolPath(FES_ALLOWURL),
	m_eIconPath(FES_DISPLAYIMAGETHUMBNAILS),
	m_nMaxNumTools(nMaxNumTools),
	m_btnArgMenu(IDR_MISC, MM_TOOLARGS, MBS_DOWN),
	m_bRunMinimized(FALSE),
	m_nNumDefaultIcons(-1)
{
	m_eIconPath.InsertButton(0, BTN_SELECTICON, CIcon(IDI_ICON_SELECT), CEnString(IDS_CAD_BROWSEIMAGES));
}

CPreferencesToolPage::~CPreferencesToolPage()
{
}

void CPreferencesToolPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesToolPage)
	DDX_Control(pDX, IDC_ARGMENUBTN, m_btnArgMenu);
	DDX_Control(pDX, IDC_TOOLPATH, m_eToolPath);
	DDX_Control(pDX, IDC_TOOLLIST, m_lcTools);
	DDX_Text(pDX, IDC_TOOLPATH, m_sToolPath);
	DDX_Text(pDX, IDC_CMDLINE, m_sCommandLine);
	DDX_Check(pDX, IDC_RUNMINIMIZED, m_bRunMinimized);
	DDX_Text(pDX, IDC_ICONPATH, m_sIconPath);
	DDX_Control(pDX, IDC_ICONPATH, m_eIconPath);
	DDX_Control(pDX, IDC_CMDLINE, m_eCmdLine);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_DISPLAYUDTSINTOOLBAR, m_bDisplayUDTsInToolbar);
}

BEGIN_MESSAGE_MAP(CPreferencesToolPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesToolPage)
	ON_WM_SIZE()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_UDTPREFS_NEW, OnNewExternalTool)
	ON_COMMAND(ID_UDTPREFS_NEWTDL, OnNewTDLTool)
	ON_COMMAND(ID_UDTPREFS_DELETE, OnDeleteTool)
	ON_COMMAND(ID_UDTPREFS_COPY, OnCopyTool)
	ON_COMMAND(ID_UDTPREFS_EDIT, OnEditToolName)
	ON_COMMAND(ID_UDTPREFS_MOVEDOWN, OnMoveToolDown)
	ON_COMMAND(ID_UDTPREFS_MOVEUP, OnMoveToolUp)
	ON_UPDATE_COMMAND_UI(ID_UDTPREFS_NEW, OnUpdateCmdUINewExternalTool)
	ON_UPDATE_COMMAND_UI(ID_UDTPREFS_NEWTDL, OnUpdateCmdUINewTDLTool)
	ON_UPDATE_COMMAND_UI(ID_UDTPREFS_DELETE, OnUpdateCmdUIDeleteTool)
	ON_UPDATE_COMMAND_UI(ID_UDTPREFS_COPY, OnUpdateCmdUICopyTool)
	ON_UPDATE_COMMAND_UI(ID_UDTPREFS_EDIT, OnUpdateCmdUIEditToolName)
	ON_UPDATE_COMMAND_UI(ID_UDTPREFS_MOVEDOWN, OnUpdateCmdUIMoveToolDown)
	ON_UPDATE_COMMAND_UI(ID_UDTPREFS_MOVEUP, OnUpdateCmdUIMoveToolUp)

	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_TOOLLIST, OnEndlabeleditToollist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TOOLLIST, OnItemchangedToollist)
	ON_EN_CHANGE(IDC_TOOLPATH, OnChangeToolpath)
	ON_NOTIFY(LVN_KEYDOWN, IDC_TOOLLIST, OnKeydownToollist)
	ON_EN_CHANGE(IDC_CMDLINE, OnChangeCmdline)
	ON_BN_CLICKED(IDC_RUNMINIMIZED, OnRunminimized)
	ON_BN_CLICKED(IDC_TESTTOOL, OnTestTool)
	ON_EN_CHANGE(IDC_ICONPATH, OnChangeIconPath)
	ON_BN_CLICKED(IDC_IMPORT, OnImportTools)
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnGetFileIcon)
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnSelectIcon)

	ON_COMMAND_RANGE(ID_TOOLARG_CUSTOMATTRIB1, ID_TOOLARG_CUSTOMATTRIB16, OnInsertCustomAttribute)
	ON_COMMAND_RANGE(ID_TOOLARG_USERVAR1, ID_TOOLARG_USERVAR10, OnInsertUserVariable)
	ON_COMMAND_RANGE(ID_TOOLARG_PATHNAME, ID_TOOLARG_SELTASKPATH, OnInsertPlaceholder)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesToolPage message handlers

BOOL CPreferencesToolPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesToolPage::SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	m_aCustomAttribDefs.Copy(aAttribDefs);
}

void CPreferencesToolPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();

	VERIFY(InitializeToolbar());

	m_nNumDefaultIcons = m_ilTools.LoadDefaultImages(TRUE);

	m_lcTools.SetImageList(&m_ilTools, LVSIL_SMALL);
	m_lcTools.AllowOffItemClickDeselection(FALSE);

	m_eToolPath.SetCurrentFolder(FileMisc::GetAppFolder());
	m_eIconPath.SetCurrentFolder(FileMisc::GetAppFolder());

	m_btnArgMenu.SetTooltip(CEnString(IDS_PTP_PLACEHOLDERS));

	CRect rList;
	m_lcTools.GetClientRect(rList);

	m_lcTools.InsertColumn(COL_NAME, CEnString(IDS_PTP_TOOLNAME), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(150));
	m_lcTools.InsertColumn(COL_PATH, CEnString(IDS_PTP_TOOLPATH), LVCFMT_LEFT, GraphicsMisc::ScaleByDPIFactor(250));
	m_lcTools.InsertColumn(COL_ARGS, CEnString(IDS_PTP_ARGUMENTS), LVCFMT_LEFT, rList.Width() - GraphicsMisc::ScaleByDPIFactor(400));
	m_lcTools.InsertColumn(COL_ICON, CEnString(IDS_PTP_ICONPATH), LVCFMT_LEFT, 0);

	ListView_SetExtendedListViewStyleEx(m_lcTools, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_lcTools, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	// add tools we loaded from the registry
	for (int nTool = 0; nTool < m_aTools.GetSize(); nTool++)
	{
		const TDCUSERTOOL& tool = m_aTools[nTool];
		VERIFY(AddToolToList(tool) != -1);
	}

	RebuildListImages();
	UpdateRemainingCount();

	m_lcTools.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	OnItemchangedToollist(NULL, NULL);

	CThemed::SetWindowTheme(&m_lcTools, _T("Explorer"));

	EnableControls();
	m_toolbar.RefreshButtonStates(FALSE);
}

void CPreferencesToolPage::OnOK()
{
	CPreferencesPageBase::OnOK();

#ifdef _DEBUG
	// Check we kept things properly synchronised
	CTDCUserToolArray aTools;
	aTools.Copy(m_aTools);
#endif

	RebuildToolsFromList();
	ASSERT(Misc::MatchAllT(aTools, m_aTools, TRUE));
}

int CPreferencesToolPage::AddToolToList(const TDCUSERTOOL& tool, int nPos, BOOL bRebuildImages, BOOL bUpdateRemaining)
{
	// special case
	if (nPos == -1)
		nPos = m_lcTools.GetItemCount();

	// Sanity check
	if ((nPos < 0) || (nPos > m_lcTools.GetItemCount()))
	{
		ASSERT(0);
		return -1;
	}

	int nIndex = m_lcTools.InsertItem(nPos, tool.sToolName, -1);

	if (nIndex == -1)
		return -1;
	
	m_lcTools.SetItemText(nIndex, COL_PATH, tool.sToolPath);
	m_lcTools.SetItemText(nIndex, COL_ARGS, tool.sCmdline);
	m_lcTools.SetItemText(nIndex, COL_ICON, tool.sIconPath);
	m_lcTools.SetItemData(nIndex, tool.bRunMinimized);

	if (bRebuildImages)
		RebuildListImages();

	if (bUpdateRemaining)
		UpdateRemainingCount();
	
	return nIndex;
}

void CPreferencesToolPage::OnNewExternalTool() 
{
	if (AddNewTool(FALSE) == -1)
		return;

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesToolPage::OnNewTDLTool()
{
	if (AddNewTool(TRUE) == -1)
		ASSERT(0);

	RebuildListImages();

	CPreferencesPageBase::OnControlChange();
}

int CPreferencesToolPage::AddNewTool(BOOL bTDLTool)
{
	if (m_lcTools.GetItemCount() >= m_nMaxNumTools)
	{
		ASSERT(0);
		return -1;
	}

	TDCUSERTOOL tool;
	tool.sToolName = CEnString(IDS_PTP_NEWTOOL);

	if (bTDLTool)
	{
		tool.sToolPath = _T("todolist.exe");
		tool.sCmdline = MapCmdIDToPlaceholder(ID_TOOLARG_PATHNAME);
	}

	int nIndex = m_aTools.Add(tool);
	VERIFY(AddToolToList(tool, -1, FALSE, TRUE) == nIndex);

	m_lcTools.SetFocus();
	m_lcTools.EditLabel(nIndex);

	m_toolbar.RefreshButtonStates();

	return nIndex;
}

void CPreferencesToolPage::UpdateRemainingCount()
{
	CEnString sCount;
	sCount.Format(IDS_TOOLS_REMAININGCOUNT, (m_nMaxNumTools - m_lcTools.GetItemCount()), m_nMaxNumTools);

	SetDlgItemText(IDC_REMAININGCOUNT, sCount);
}

void CPreferencesToolPage::OnUpdateCmdUINewExternalTool(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_lcTools.GetItemCount() < m_nMaxNumTools);
}

void CPreferencesToolPage::OnUpdateCmdUINewTDLTool(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_lcTools.GetItemCount() < m_nMaxNumTools);
}

void CPreferencesToolPage::OnDeleteTool() 
{
	int nSel = GetCurSel();

	if (nSel != -1)
	{
		m_aTools.RemoveAt(nSel);
		m_lcTools.DeleteItem(nSel);

		// Select next previous item
		if (nSel == m_lcTools.GetItemCount())
			nSel--;

		SetCurSel(nSel);
		
		if (nSel == -1)
		{
			m_sToolPath.Empty();
			m_sIconPath.Empty();
			
			UpdateRemainingCount();
			EnableControls();
			UpdateData(FALSE);
		}
	}

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesToolPage::OnUpdateCmdUIDeleteTool(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCurSel() != -1);
}

void CPreferencesToolPage::OnEditToolName() 
{
	int nSel = GetCurSel();
	
	if (nSel != -1)
	{
		m_lcTools.SetFocus();
		m_lcTools.EditLabel(nSel);
	}
}

void CPreferencesToolPage::OnUpdateCmdUIEditToolName(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCurSel() != -1);
}

void CPreferencesToolPage::OnCopyTool() 
{
	ASSERT(m_lcTools.GetItemCount() < m_nMaxNumTools);
	int nSel = GetCurSel();
	
	if (nSel != -1)
	{
		TDCUSERTOOL tool = m_aTools[nSel];
		m_aTools.InsertAt(nSel + 1, tool);

		int nCopy = AddToolToList(tool, (nSel + 1), TRUE, TRUE);
		SetCurSel(nCopy);
		
		m_lcTools.SetFocus();
		m_lcTools.EditLabel(nCopy);
		
		m_toolbar.RefreshButtonStates();
		
		CPreferencesPageBase::OnControlChange();
	}
}

void CPreferencesToolPage::OnUpdateCmdUICopyTool(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((GetCurSel() != -1) && (m_lcTools.GetItemCount() < m_nMaxNumTools));
}

void CPreferencesToolPage::OnMoveToolUp() 
{
	int nSel = GetCurSel();
	
	if (nSel > 0)
	{
		TDCUSERTOOL tool = m_aTools[nSel]; // copy

		m_aTools.RemoveAt(nSel);
		m_aTools.InsertAt(nSel - 1, tool);

		m_lcTools.DeleteItem(nSel);
		nSel = AddToolToList(tool, (nSel - 1), TRUE);

		SetCurSel(nSel);
		m_lcTools.SetFocus();

		RebuildToolsFromList();

		CPreferencesPageBase::OnControlChange();
	}
}

void CPreferencesToolPage::OnUpdateCmdUIMoveToolUp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCurSel() > 0);
}

void CPreferencesToolPage::OnMoveToolDown() 
{
	int nSel = GetCurSel();
	
	if ((nSel >= 0) && (nSel < (m_lcTools.GetItemCount() - 1)))
	{
		TDCUSERTOOL tool = m_aTools[nSel]; // copy

		m_aTools.RemoveAt(nSel);
		m_aTools.InsertAt(nSel + 1, tool);

		m_lcTools.DeleteItem(nSel);
		nSel = AddToolToList(tool, (nSel + 1), TRUE);
		SetCurSel(nSel);

		m_lcTools.SetFocus();

		CPreferencesPageBase::OnControlChange();
	}
}

void CPreferencesToolPage::OnUpdateCmdUIMoveToolDown(CCmdUI* pCmdUI)
{
	int nSel = GetCurSel();

	pCmdUI->Enable((nSel >= 0) && (nSel < (m_lcTools.GetItemCount() - 1)));
}

void CPreferencesToolPage::OnEndlabeleditToollist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	if (pDispInfo->item.pszText)
	{
		int nSel = GetCurSel();

		if (nSel >= 0)
		{
			m_lcTools.SetItemText(nSel, COL_NAME, pDispInfo->item.pszText);
			m_aTools[nSel].sToolName = pDispInfo->item.pszText;

			m_eToolPath.SetFocus();
		}
	}
	
	*pResult = 0;

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesToolPage::OnItemchangedToollist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (pNMHDR && !CEnListCtrl::IsSelectionChange((NMLISTVIEW*)pNMHDR))
		return;

	int nSel = GetCurSel();

	if (nSel >= 0)
	{
		m_sToolPath = m_lcTools.GetItemText(nSel, COL_PATH);
		m_sCommandLine = m_lcTools.GetItemText(nSel, COL_ARGS);
		m_sIconPath = m_lcTools.GetItemText(nSel, COL_ICON);
		m_bRunMinimized = m_lcTools.GetItemData(nSel);
	}
	else
	{
		m_sToolPath.Empty();
		m_sCommandLine.Empty();
		m_bRunMinimized = FALSE;
		m_sIconPath.Empty();
	}

	UpdateData(FALSE);
	EnableControls();

	m_toolbar.RefreshButtonStates(FALSE);
}

void CPreferencesToolPage::EnableControls()
{
	int nSel = GetCurSel();

	GetDlgItem(IDC_TOOLPATH)->EnableWindow(nSel >= 0);
	GetDlgItem(IDC_ICONPATH)->EnableWindow(nSel >= 0);
	GetDlgItem(IDC_CMDLINE)->EnableWindow(nSel >= 0);
	GetDlgItem(IDC_RUNMINIMIZED)->EnableWindow(nSel >= 0);

	m_btnArgMenu.EnableWindow(nSel >= 0);
}

int CPreferencesToolPage::GetCurSel() const
{
	int nSel = -1;
	POSITION pos = m_lcTools.GetFirstSelectedItemPosition();

	if (pos)
		nSel = m_lcTools.GetNextSelectedItem(pos);

	return nSel;
}

BOOL CPreferencesToolPage::SetCurSel(int nSel)
{
	if (nSel != -1)
		m_lcTools.SetItemState(nSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	// else
	return FALSE;
}

void CPreferencesToolPage::OnChangeToolpath() 
{
	int nSel = GetCurSel();

	if (nSel == CB_ERR)
	{
		ASSERT (0);
		return;
	}

	UpdateData();

	m_lcTools.SetItemText(nSel, COL_PATH, m_sToolPath);
	m_aTools[nSel].sToolPath = m_sToolPath;

	RebuildListImages();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesToolPage::RebuildListImages()
{
	int nTool = m_lcTools.GetItemCount();

	while (nTool--)
	{
		int nImage = CTDCToolsHelper::AddToolToImageList(m_aTools[nTool], m_ilTools);
		m_lcTools.SetItemImage(nTool, nImage);
	}
}

void CPreferencesToolPage::OnChangeIconPath() 
{
	int nSel = GetCurSel();

	if (nSel == CB_ERR)
	{
		ASSERT (0);
		return;
	}

	UpdateData();

	m_lcTools.SetItemText(nSel, 3, m_sIconPath);
	m_aTools[nSel].sIconPath = m_sIconPath;

	RebuildListImages();

	CPreferencesPageBase::OnControlChange();
}

int CPreferencesToolPage::GetUserTools(CTDCUserToolArray& aTools) const
{
	aTools.Copy(m_aTools);

	return aTools.GetSize();
}

BOOL CPreferencesToolPage::GetUserTool(int nTool, TDCUSERTOOL& tool) const
{
	if ((nTool < 0) || (nTool >= m_aTools.GetSize()))
		return FALSE;

	tool = m_aTools[nTool];
	return TRUE;
}

void CPreferencesToolPage::RebuildToolsFromList()
{
	m_aTools.RemoveAll();
	int nToolCount = m_lcTools.GetItemCount();

	for (int nTool = 0; nTool < nToolCount; nTool++)
	{
		TDCUSERTOOL tool;
		VERIFY(GetToolFromList(nTool, tool));

		m_aTools.Add(tool);
	}
}

BOOL CPreferencesToolPage::GetToolFromList(int nTool, TDCUSERTOOL& tool) const
{
	if ((nTool < 0) || (nTool >= m_lcTools.GetItemCount()))
	{
		ASSERT(0);
		return FALSE;
	}

	tool.sToolName = m_lcTools.GetItemText(nTool, COL_NAME);
	tool.sToolPath = m_lcTools.GetItemText(nTool, COL_PATH);
	tool.sCmdline = m_lcTools.GetItemText(nTool, COL_ARGS);
	tool.sIconPath = m_lcTools.GetItemText(nTool, COL_ICON);
	tool.bRunMinimized = m_lcTools.GetItemData(nTool);

	return TRUE;
}

void CPreferencesToolPage::OnKeydownToollist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;
	
	switch (pLVKeyDown->wVKey)
	{
	case VK_DELETE:
		OnDeleteTool();
		break;

	case VK_F2:
		{
			int nSel = GetCurSel();

			if (nSel >= 0)
				m_lcTools.EditLabel(nSel);
		}
		break;
	}
	
	*pResult = 0;
}

void CPreferencesToolPage::OnChangeCmdline() 
{
	int nSel = GetCurSel();

	if (nSel >= 0)
	{
		UpdateData();

		m_lcTools.SetItemText(nSel, COL_ARGS, m_sCommandLine);
		m_aTools[nSel].sCmdline = m_sCommandLine;

		CPreferencesPageBase::OnControlChange();
	}
}

void CPreferencesToolPage::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	// Replace user-variable placeholder text with actual variable names
	// in the order they appear so we can reverse-lookup their names
	if (CEnMenu::FindMenuItem(*pPopupMenu, ID_TOOLARG_USERVAR1) == 0)
	{
		ASSERT(pPopupMenu->GetMenuItemCount() == 16);
	
		m_aMenuUserVariableIDs.RemoveAll();

		CTDCToolsCmdlineParser tcp(m_sCommandLine);
	
		CCLArgArray aArgs;
		int nNumArgs = tcp.GetUserArguments(aArgs), nAdded = 0;
		
		for (int nArg = 0; ((nArg < nNumArgs) && (nAdded < 16)); nArg++)
		{
			switch (aArgs[nArg].nType)
			{
			case CLAT_USERDATE:
			case CLAT_USERFILE:
			case CLAT_USERFOLDER:
			case CLAT_USERTEXT:
				CEnMenu::SetMenuString(*pPopupMenu, nAdded, aArgs[nArg].sName, MF_BYPOSITION);

				m_aMenuUserVariableIDs.Add(aArgs[nArg].sName);
				nAdded++;
				break;
			}
		}

		// Delete any outstanding placeholders
		while (pPopupMenu->DeleteMenu(nAdded, MF_BYPOSITION));

		// And sort the rest
		CEnMenu::SortMenuStrings(*pPopupMenu, ID_TOOLARG_USERVAR1, ID_TOOLARG_USERVAR16);
	}
	// Likewise for custom attributes
	else if (CEnMenu::FindMenuItem(*pPopupMenu, ID_TOOLARG_CUSTOMATTRIB1) == 0)
	{
		ASSERT(pPopupMenu->GetMenuItemCount() == 16);
	
		m_aMenuCustomAttribIDs.RemoveAll();

		int nNumAttrib = m_aCustomAttribDefs.GetSize(), nAdded = 0;
		
		for (int nAtt = 0; ((nAtt < nNumAttrib) && (nAdded < 16)); nAtt++)
		{
			if (m_aCustomAttribDefs[nAtt].bEnabled)
			{
				CEnMenu::SetMenuString(*pPopupMenu, nAdded, m_aCustomAttribDefs[nAtt].sLabel, MF_BYPOSITION);

				m_aMenuCustomAttribIDs.Add(Misc::ToLower(m_aCustomAttribDefs[nAtt].sUniqueID));
				nAdded++;
			}
		}

		// Delete any outstanding placeholders
		while (pPopupMenu->DeleteMenu(nAdded, MF_BYPOSITION));

		// And sort the rest
		CEnMenu::SortMenuStrings(*pPopupMenu, ID_TOOLARG_CUSTOMATTRIB1, ID_TOOLARG_CUSTOMATTRIB16);
	}
	
	CPreferencesPageBase::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

void CPreferencesToolPage::OnInsertUserVariable(UINT nCmdID)
{
	int nVar = (nCmdID - ID_TOOLARG_USERVAR1);

	if (nVar >= m_aMenuUserVariableIDs.GetSize())
	{
		ASSERT(0);
		return;
	}

	CString sPlaceholder;
	sPlaceholder.Format(_T("$(%s)"), m_aMenuUserVariableIDs[nVar]);

	m_eCmdLine.ReplaceSel(sPlaceholder, TRUE);
}

void CPreferencesToolPage::OnInsertCustomAttribute(UINT nCmdID)
{
	int nAtt = (nCmdID - ID_TOOLARG_CUSTOMATTRIB1);

	if (nAtt >= m_aMenuCustomAttribIDs.GetSize())
	{
		ASSERT(0);
		return;
	}

	CString sPlaceholder;
	sPlaceholder.Format(_T("$(seltcustom, %s)"), m_aMenuCustomAttribIDs[nAtt]);

	m_eCmdLine.ReplaceSel(sPlaceholder, TRUE);
}

void CPreferencesToolPage::OnInsertPlaceholder(UINT nCmdID) 
{
	UpdateData();

	CString sPlaceholder(MapCmdIDToPlaceholder(nCmdID));
	ASSERT(!sPlaceholder.IsEmpty());

	int nSelStart = LOWORD(m_eCmdLine.GetSel());

	m_eCmdLine.ReplaceSel(sPlaceholder, TRUE);

	// Auto-select the most probable text for the user to edit
	CEnString sSelText;

	switch (MapCmdIDToType(nCmdID))
	{
	case CLAT_USERDATE:		sSelText.LoadString(IDS_USERTOOL_DATEPROMPT);	break;
	case CLAT_USERFILE:		sSelText.LoadString(IDS_USERTOOL_FILEPROMPT);	break;
	case CLAT_USERFOLDER:	sSelText.LoadString(IDS_USERTOOL_FOLDERPROMPT);	break;
	case CLAT_USERTEXT:		sSelText.LoadString(IDS_USERTOOL_TEXTPROMPT);	break;
	}

	if (!sSelText.IsEmpty())
	{
		// Search for the text forward of the insertion point
		// and before the end of the inserted text
		int nSelTextLen = sSelText.GetLength();
		int nPlaceholderLen = sPlaceholder.GetLength();

		VERIFY(SelectText(&m_eCmdLine, sSelText, nSelStart, (nPlaceholderLen - nSelTextLen)));
	}

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesToolPage::OnRunminimized() 
{
	int nSel = GetCurSel();

	if (nSel >= 0)
	{
		UpdateData();

		m_lcTools.SetItemData(nSel, m_bRunMinimized);
		m_aTools[nSel].bRunMinimized = m_bRunMinimized;

		CPreferencesPageBase::OnControlChange();
	}
}

LRESULT CPreferencesToolPage::OnSelectIcon(WPARAM wParam, LPARAM lParam)
{
	int nSel = GetCurSel();

	if ((nSel >= 0) && (lParam == BTN_SELECTICON))
	{
		// We pass in the number of default icons so that any others
		// added due to the tool or icon path are not shown to the user
		CTDLTaskIconDlg dialog(m_ilTools, m_aTools[nSel].sIconPath, FALSE, m_nNumDefaultIcons);
		
		if (dialog.DoModal(IDS_TOOLSICONDLG_TITLE) == IDOK)
		{
			CString sNewIcon = dialog.GetIconName();

			if (sNewIcon != m_sIconPath)
			{
				m_lcTools.SetItemText(nSel, COL_ICON, sNewIcon);
				m_aTools[nSel].sIconPath = sNewIcon;

				RebuildListImages();

				m_sIconPath = sNewIcon;
				UpdateData(FALSE);
			}
		}
	}

	return 0L;
}

LRESULT CPreferencesToolPage::OnGetFileIcon(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_TOOLPATH:
		{
			CString sFilePath((LPCTSTR)lParam);
			int nIcon = m_ilTools.GetImageIndex(sFilePath);

			if (nIcon != -1)
			{
				static CIcon icon;

				icon.SetIcon(m_ilTools.ExtractIcon(nIcon));
				return (LRESULT)(HICON)icon;
			}
		}
		break;

	case IDC_ICONPATH:
		{
			CString sFilePath((LPCTSTR)lParam);
			int nIcon = m_ilTools.GetImageIndex(sFilePath);

			static CIcon icon;

			if (nIcon != -1)
				icon.SetIcon(m_ilTools.ExtractIcon(nIcon));
			else
				icon.Load(IDI_NULL); // else CFileEdit will show a folder icon

			return (LRESULT)(HICON)icon;
		}
		break;
	}

	return 0L;
}

void CPreferencesToolPage::OnTestTool() 
{
	int nTool = GetCurSel();
	
	if (nTool != -1)
		GetParent()->SendMessage(WM_PTP_TESTTOOL, 0, (LPARAM)&(m_aTools[nTool]));
}

CString CPreferencesToolPage::MapCmdIDToPlaceholder(UINT nCmdID) const
{
	CLA_TYPE nType = MapCmdIDToType(nCmdID);
	CString sPlaceHolder = CTDCToolsCmdlineParser::GetPlaceHolder(nType), sText;

	switch (nCmdID)
	{
	case ID_TOOLARG_FILETITLE:			
	case ID_TOOLARG_FOLDER:				
	case ID_TOOLARG_FILENAME:			
	case ID_TOOLARG_SELTASKID:			
	case ID_TOOLARG_TODAYSDATE:	
		sText.Format(_T("$(%s)"), sPlaceHolder);
		break;
		
	case ID_TOOLARG_PATHNAME:			
	case ID_TOOLARG_SELTASKTITLE:		
	case ID_TOOLARG_TODOLIST:			
	case ID_TOOLARG_SELTASKEXTID:		
	case ID_TOOLARG_SELTASKCOMMENTS:	
	case ID_TOOLARG_SELTASKFILELINK:	
	case ID_TOOLARG_SELTASKALLOCBY:		
	case ID_TOOLARG_SELTASKALLOCTO:		
	case ID_TOOLARG_SELTASKPATH:
		sText.Format(_T("\"$(%s)\""), sPlaceHolder);
		break;
		
	case ID_TOOLARG_USERDATE:		
		sText = FormatUserPlaceHolder(sPlaceHolder, _T("date"), IDS_USERTOOL_DATEPROMPT);
		break;
		
	case ID_TOOLARG_USERFILEPATH:	
		sText = FormatUserPlaceHolder(sPlaceHolder, _T("file"), IDS_USERTOOL_FILEPROMPT);
		break;
		
	case ID_TOOLARG_USERFOLDER:		
		sText = FormatUserPlaceHolder(sPlaceHolder, _T("folder"), IDS_USERTOOL_FOLDERPROMPT);
		break;
		
	case ID_TOOLARG_USERTEXT:		
		sText = FormatUserPlaceHolder(sPlaceHolder, _T("text"), IDS_USERTOOL_TEXTPROMPT);
		break;
	}
	
	return sText;
}

CString CPreferencesToolPage::FormatUserPlaceHolder(LPCTSTR szPlaceHolder, LPCTSTR szVarType, UINT nIDTextPrompt) const
{
	return Misc::Format(_T("$(%s, %s, \"%s\", default_%s)"), szPlaceHolder, GetNewUserVariableName(szVarType), CEnString(nIDTextPrompt), szVarType);
}

CString CPreferencesToolPage::GetNewUserVariableName(LPCTSTR szVarType) const
{
	int nIndex = 1;
	CString sVarName;

	do 
	{
		sVarName.Format(_T("var_%s%d"), szVarType, nIndex++);
	} 
	while (m_sCommandLine.Find(sVarName) != -1);

	return sVarName;
}

CLA_TYPE CPreferencesToolPage::MapCmdIDToType(UINT nCmdID)
{
	switch (nCmdID)
	{
	case ID_TOOLARG_PATHNAME:			return CLAT_PATHNAME;		
	case ID_TOOLARG_FILETITLE:			return CLAT_FILETITLE;
	case ID_TOOLARG_FOLDER:				return CLAT_FOLDER; 
	case ID_TOOLARG_FILENAME:			return CLAT_FILENAME;
	case ID_TOOLARG_SELTASKID:			return CLAT_SELTASKID;
	case ID_TOOLARG_SELTASKTITLE:		return CLAT_SELTASKTITLE;
	case ID_TOOLARG_TODAYSDATE:			return CLAT_TODAYSDATE;
	case ID_TOOLARG_TODOLIST:			return CLAT_TODOLIST;
	case ID_TOOLARG_SELTASKEXTID:		return CLAT_SELTASKEXTID;
	case ID_TOOLARG_SELTASKCOMMENTS:	return CLAT_SELTASKCOMMENTS;
	case ID_TOOLARG_SELTASKFILELINK:	return CLAT_SELTASKFILELINK;
	case ID_TOOLARG_SELTASKALLOCBY:		return CLAT_SELTASKALLOCBY;
	case ID_TOOLARG_SELTASKALLOCTO:		return CLAT_SELTASKALLOCTO;	
	case ID_TOOLARG_SELTASKPATH:		return CLAT_SELTASKPATH;
	case ID_TOOLARG_USERDATE:			return CLAT_USERDATE;
	case ID_TOOLARG_USERFILEPATH:		return CLAT_USERFILE;
	case ID_TOOLARG_USERFOLDER:			return CLAT_USERFOLDER;
	case ID_TOOLARG_USERTEXT:			return CLAT_USERTEXT;
	}

	// all else
	ASSERT(0);
	return CLAT_NONE;
}

void CPreferencesToolPage::OnImportTools() 
{
	BOOL bContinue = TRUE;
	CPreferences prefs;

	while (bContinue)
	{
		CFileOpenDialog dialog(IDS_IMPORTPREFS_TITLE, 
								_T("ini"), 
								NULL, 
								OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
								CEnString(IDS_INIFILEFILTER));
		
		if (dialog.DoModal(prefs) == IDOK)
		{
			CIni ini(dialog.GetPathName());

			int nNumTools = ini.GetInt(_T("Tools"), _T("ToolCount"), 0);
			nNumTools = min(nNumTools, m_nMaxNumTools);

			if (!nNumTools)
			{
				bContinue = (AfxMessageBox(IDS_INIHASNOTOOLS, MB_YESNO) == IDYES);
			}
			else
			{
				for (int nTool = 0; nTool < nNumTools; nTool++)
				{
					CString sKey = Misc::MakeKey(_T("Tools\\Tool%d"), nTool + 1);
					TDCUSERTOOL tool;

					tool.sToolName = ini.GetString(sKey, _T("Name"));
					tool.sToolPath = ini.GetString(sKey, _T("Path"));
					tool.sIconPath = ini.GetString(sKey, _T("IconPath"));
					tool.bRunMinimized = ini.GetBool(sKey, _T("RunMinimized"), FALSE);
					tool.sCmdline = INIENTRY::UnSafeQuote(ini.GetString(sKey, _T("Cmdline")));

					// add tool to list
					VERIFY(AddToolToList(tool) != -1);
				}
				bContinue = FALSE;

				CPreferencesPageBase::OnControlChange();
			}
		}
		else
		{
			bContinue = FALSE; // cancelled
		}
	}

	UpdateRemainingCount();
}

void CPreferencesToolPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	int nToolCount = pPrefs->GetProfileInt(_T("Tools"), _T("ToolCount"), 0);
	nToolCount = min(nToolCount, m_nMaxNumTools);

	for (int nTool = 1; nTool <= nToolCount; nTool++)
	{
		CString sKey = Misc::MakeKey(_T("Tools\\Tool%d"), nTool);
		TDCUSERTOOL tool;

		tool.sToolName = pPrefs->GetProfileString(sKey, _T("Name"));
		tool.sToolPath = pPrefs->GetProfileString(sKey, _T("Path"));
		tool.bRunMinimized = pPrefs->GetProfileInt(sKey, _T("RunMinimized"), FALSE);
		tool.sIconPath = pPrefs->GetProfileString(sKey, _T("IconPath"));
		tool.sCmdline = INIENTRY::UnSafeQuote(pPrefs->GetProfileString(sKey, _T("CmdLine")));

		m_aTools.Add(tool);
	}

	m_bDisplayUDTsInToolbar = pPrefs->GetProfileInt(szKey, _T("DisplayUDTsInToolbar"), TRUE);
}

void CPreferencesToolPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	int nToolCount = m_aTools.GetSize();

	for (int nTool = 0; nTool < nToolCount; nTool++)
	{
        CString sKey = Misc::MakeKey(_T("Tools\\Tool%d"), nTool + 1);
		const TDCUSERTOOL& tool = m_aTools[nTool];

		pPrefs->WriteProfileString(sKey, _T("Name"), tool.sToolName);
		pPrefs->WriteProfileString(sKey, _T("Path"), tool.sToolPath);
		pPrefs->WriteProfileString(sKey, _T("IconPath"), tool.sIconPath);
		pPrefs->WriteProfileInt(sKey, _T("RunMinimized"), tool.bRunMinimized);
		pPrefs->WriteProfileString(sKey, _T("Cmdline"), INIENTRY::SafeQuote(tool.sCmdline));
	}

	pPrefs->WriteProfileInt(_T("Tools"), _T("ToolCount"), nToolCount);
	pPrefs->WriteProfileInt(szKey, _T("DisplayUDTsInToolbar"), m_bDisplayUDTsInToolbar);
}

void CPreferencesToolPage::OnSize(UINT nType, int cx, int cy) 
{
	CPreferencesPageBase::OnSize(nType, cx, cy);
	
	if (m_lcTools.GetSafeHwnd())
	{
		// calculate border
		CPoint ptBorders = CDialogHelper::GetCtrlRect(this, IDC_TOOLBAR).TopLeft();
		
		// calc offsets
		CPoint ptDetails = CDialogHelper::GetCtrlRect(this, IDC_TOOLDETAILS).BottomRight();
		int nXOffset = cx - (ptDetails.x + ptBorders.x);
		int nYOffset = cy - (ptDetails.y + ptBorders.y);

		// Update positions and sizes
		CDialogHelper::OffsetCtrl(this, IDC_IMPORT, nXOffset, 0);
		CDialogHelper::OffsetCtrl(this, IDC_TOOLDETAILS, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_REMAININGCOUNT, nXOffset, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_PATHLABEL, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_ARGLABEL, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_ICONLABEL, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_TOOLPATH, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_CMDLINE, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_ICONPATH, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_RUNMINIMIZED, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_ARGMENUBTN, nXOffset, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_TESTTOOL, nXOffset, nYOffset);

		CDialogHelper::ResizeChild(&m_eToolPath, nXOffset, 0);
		CDialogHelper::ResizeChild(&m_eIconPath, nXOffset, 0);
		CDialogHelper::ResizeChild(&m_eCmdLine, nXOffset, 0);
		CDialogHelper::ResizeChild(&m_lcTools, nXOffset, nYOffset);
		CDialogHelper::ResizeCtrl(this, IDC_TOOLDETAILS, nXOffset, 0);

		m_lcTools.SetColumnWidth(2, m_lcTools.GetColumnWidth(2) + nXOffset);
	}
}

BOOL CPreferencesToolPage::InitializeToolbar()
{
	if (m_toolbar.GetSafeHwnd())
		return TRUE;
	
	if (!m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP))
		return FALSE;

	m_toolbar.SetBackgroundColor(m_crBack);
	
	VERIFY(m_toolbar.LoadToolBar(IDR_UDTPREFS_TOOLBAR, IDB_UDTPREFS_TOOLBAR_STD, colorMagenta));
	VERIFY(m_tbHelper.Initialize(&m_toolbar));
	
	// very important - turn OFF all the auto positioning and sizing
	// by default have no borders
	UINT nStyle = m_toolbar.GetBarStyle();
	nStyle &= ~(CCS_NORESIZE | CCS_NOPARENTALIGN | CBRS_BORDER_ANY);
	nStyle |= (CBRS_SIZE_FIXED | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_toolbar.SetBarStyle(nStyle);
	
	CRect rToolbar;
	GetDlgItem(IDC_TOOLBAR)->GetWindowRect(rToolbar);
	ScreenToClient(rToolbar);
	m_toolbar.MoveWindow(rToolbar);
	
	return TRUE;
}
