// TDLTransEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TDLTransEdit.h"
#include "TDLTransEditDlg.h"

#include "..\transtext\transtextutils.h"

#include "..\shared\enfiledialog.h"
#include "..\shared\holdredraw.h"
#include "..\shared\filemisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\mousewheelmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditDlg dialog

CTDLTransEditDlg::CTDLTransEditDlg(LPCTSTR szAppVer, CWnd* pParent /*=NULL*/)
	: 
	CDialog(CTDLTransEditDlg::IDD, pParent), 
	m_sAppVer(szAppVer),
	m_bEdited(FALSE), 
	m_bShowAlternatives(TRUE),
	m_bShowTooltips(TRUE),
	m_bSortUntranslatedAtTop(TRUE)
{
	//{{AFX_DATA_INIT(CTDLTransEditDlg)
	m_sFilter = _T("");
	//}}AFX_DATA_INIT

	CTransDictionary::SetAppVersion(szAppVer);
}

void CTDLTransEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLTransEditDlg)
	DDX_Control(pDX, IDC_DICTIONARY, m_lcDictItems);
	DDX_Text(pDX, IDC_FILTER, m_sFilter);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_ENGLISH, m_sEnglish);
	DDX_Text(pDX, IDC_TRANSLATION, m_sTranslation);
}

BEGIN_MESSAGE_MAP(CTDLTransEditDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLTransEditDlg)
	ON_COMMAND(ID_OPTIONS_SHOWTOOLTIPS, OnOptionsShowTooltips)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TOOLS_CLEANUP, OnToolsCleanUp)
	ON_COMMAND(ID_FILE_OPEN_TRANSLATION, OnFileOpenTranslation)
	ON_COMMAND(ID_FILE_SAVE_TRANSLATION, OnFileSaveTranslation)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_DICTIONARY, OnEndlabeleditDictionary)
	ON_COMMAND(ID_FILE_CLOSE, OnFileExit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPTIONS_SHOWOPTIONALS, OnShowAlternatives)
	ON_BN_CLICKED(IDC_UPDATEFILTER, OnUpdateFilter)
	ON_BN_CLICKED(IDC_CLEARFILTER, OnClearFilter)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_FILE_NEW_TRANSLATION, OnFileNewTranslation)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DICTIONARY, OnListItemChanged)
	ON_EN_CHANGE(IDC_TRANSLATION, OnChangeTranslation)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_OPTIONS_SORTUNTRANSATTOP, OnOptionsSortUntranslatedAtTop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditDlg message handlers

BOOL CTDLTransEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMouseWheelMgr::Initialize();
	DICTITEM::SetTranslationOption(ITTTO_ADD2DICTIONARY);

	GetWindowText(m_sBaseTitle);
	m_icons.Initialise(*this, IDR_MAINFRAME);

	CMenu menu;

	if (menu.LoadMenu(IDR_MAINFRAME))
		SetMenu(&menu);

	m_lcDictItems.Initialise();

	if (m_mgrShortcuts.Initialize(this))
	{
		m_mgrShortcuts.AddShortcut(ID_FILE_NEW_TRANSLATION, 'N', HOTKEYF_CONTROL);
		m_mgrShortcuts.AddShortcut(ID_FILE_OPEN_TRANSLATION, 'O', HOTKEYF_CONTROL);
		m_mgrShortcuts.AddShortcut(ID_FILE_SAVE_TRANSLATION, 'S', HOTKEYF_CONTROL);
		m_mgrShortcuts.AddShortcut(ID_TOOLS_CLEANUP, 'U', HOTKEYF_CONTROL);
		m_mgrShortcuts.AddShortcut(ID_FILE_CLOSE, VK_F4, HOTKEYF_ALT);
	}
	
	LoadState();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTDLTransEditDlg::OnFileOpenTranslation() 
{
	CFileOpenDialog dialog(_T("Select Translation"), _T(".csv"), m_sLastBrowsePath, EOFN_DEFAULTOPEN, _T("Translations (*.csv)|*.csv||"));

	if (dialog.DoModal() == IDOK)
	{
		m_sFilter.Empty();
		UpdateData(FALSE);

		VERIFY(LoadDictionary(dialog.GetPathName()));
	}
}

BOOL CTDLTransEditDlg::LoadDictionary(LPCTSTR szDictPath, BOOL bAllowPrompt)
{
	if (PromptAndSave())
	{
		CWaitCursor cursor;
		CHoldRedraw hr(*this);

		// First clean it
		if (RecheckYourLanguagePath(szDictPath, bAllowPrompt))
			TransText::CleanupDictionary(m_sYourLanguagePath, szDictPath);
		
		// Then load it
		if (m_dictionary.LoadDictionary(szDictPath, FALSE))
		{
			m_lcDictItems.RebuildList(m_dictionary, m_bShowAlternatives, m_sFilter);
			m_lcDictItems.SetReadOnly(IsReadOnly());
			
			m_bEdited = FALSE;
			m_sLastBrowsePath = FileMisc::GetFolderFromFilePath(szDictPath);

			UpdateCaption();
			return TRUE;
		}
	}

	// else
	return FALSE;
}

void CTDLTransEditDlg::OnFileSaveTranslation() 
{
	ASSERT(!IsReadOnly());

	if (!IsReadOnly())
	{
		m_dictionary.SaveDictionary();
		m_bEdited = FALSE;

		UpdateCaption();
	}
}

void CTDLTransEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

void CTDLTransEditDlg::Resize(int cx, int cy)
{
	if (m_lcDictItems.GetSafeHwnd())
	{
		CRect rClient(0, 0, cx, cy);

		if (rClient.IsRectNull())
			GetClientRect(rClient);

		rClient.DeflateRect(4, 0, 4, 4);

		CRect rEnglish(GetCtrlRect(this, IDC_ENGLISH));
		CRect rTrans(GetCtrlRect(this, IDC_TRANSLATION));

		int nYOffset = (rClient.bottom - rTrans.bottom);
		int nEditWidth = ((rClient.Width() - 6) / 2);
		
		rEnglish.left = rClient.left;
		rEnglish.right = (rEnglish.left + nEditWidth);
		rTrans.left = (rEnglish.right + 6);
		rTrans.right = (rTrans.left + nEditWidth);

		rEnglish.OffsetRect(0, nYOffset);
		rTrans.OffsetRect(0, nYOffset);

		GetDlgItem(IDC_ENGLISH)->MoveWindow(rEnglish);
		GetDlgItem(IDC_TRANSLATION)->MoveWindow(rTrans);

		rEnglish = OffsetCtrl(this, IDC_ENGLISHLABEL, 0, nYOffset);
		MoveCtrl(this, IDC_TRANSLABEL, rTrans.left, rEnglish.top);

		CRect rList = GetChildRect(&m_lcDictItems);
		rList.bottom = (rEnglish.top - 6);
		rList.left = rClient.left;
		rList.right = rClient.right;

		m_lcDictItems.MoveWindow(rList);

		Invalidate(FALSE);
	}
}

void CTDLTransEditDlg::OnEndlabeleditDictionary(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(!IsReadOnly());

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	ASSERT(pDispInfo->item.iSubItem == TRANS_COL);

	if (ModifyDictionaryItem(pDispInfo->item.iItem, pDispInfo->item.pszText))
	{
		// Move selection down one row and edit
		if (pDispInfo->item.iItem < (m_lcDictItems.GetItemCount() - 1))
		{
			m_lcDictItems.SetCurSel(pDispInfo->item.iItem + 1);
			//m_lcDictItems.PostMessage(WM_KEYDOWN, VK_F2, 0);
		}
	}

	*pResult = 0;
}

BOOL CTDLTransEditDlg::ModifyDictionaryItem(int nItem, const CString& sTrans)
{
	ASSERT(!IsReadOnly());

	// Modify the dictionary item that this refers to
	CString sEnglish = m_lcDictItems.GetEnglishText(nItem);
	CString sClassID = m_lcDictItems.GetItemText(nItem, HINT_COL);

	if (m_dictionary.ModifyItem(sEnglish, sClassID, sTrans))
	{
		if (!m_bEdited)
		{
			m_bEdited = TRUE;
			UpdateCaption();
		}

		return TRUE;
	}

	return FALSE;
}

void CTDLTransEditDlg::UpdateCaption()
{
	CString sCaption, sDictPath(m_dictionary.GetDictionaryPath());

	if (sDictPath.IsEmpty())
	{
		sCaption = m_sBaseTitle;
	}
	else
	{
		ASSERT(!(IsReadOnly() && m_bEdited));

		sCaption = FileMisc::GetFileNameFromPath(sDictPath, FALSE);
		
		if (m_bEdited)
		{
			sCaption += '*';
		}
		else if (IsReadOnly())
		{
			sCaption += _T(" [read-only]");
		}
				
		sCaption += _T(" - ");
		sCaption += m_sBaseTitle;
	}

	SetWindowText(sCaption);
}

void CTDLTransEditDlg::OnOK()
{
	// do nothing
}

void CTDLTransEditDlg::OnCancel()
{
	// do nothing
}

BOOL CTDLTransEditDlg::PromptAndSave()
{
	if (!IsReadOnly() && m_bEdited)
	{
		CEnString sText(IDS_SAVECHANGES, FileMisc::GetFileNameFromPath(m_dictionary.GetDictionaryPath()));

		int nRet = CDialogHelper::ShowMessageBox(*this, m_sBaseTitle, _T("Save Changes?"), sText, MB_YESNOCANCEL);

		switch (nRet)
		{
		case IDYES:
			m_dictionary.SaveDictionary();
			return TRUE;

		case IDNO:
			return TRUE;
		}
		
		// all else cancel
		return FALSE;
	}

	return TRUE;
}

void CTDLTransEditDlg::OnFileExit() 
{
	EndDialog(IDCANCEL);
}

void CTDLTransEditDlg::OnClose() 
{
	if (PromptAndSave())
	{
		SaveState();
		EndDialog(IDCANCEL);
	}
}

void CTDLTransEditDlg::LoadState()
{
	BOOL bFirstTime = !FileMisc::FileExists(AfxGetApp()->m_pszProfileName);

	m_bShowAlternatives = AfxGetApp()->GetProfileInt(_T("State"), _T("ShowAlternatives"), FALSE);
	m_sYourLanguagePath = AfxGetApp()->GetProfileString(_T("State"), _T("YourLanguage"));
	m_sFilter = AfxGetApp()->GetProfileString(_T("State"), _T("LastFilter"));
	m_bShowTooltips = AfxGetApp()->GetProfileInt(_T("State"), _T("ShowTooltips"), TRUE);
	m_bSortUntranslatedAtTop = AfxGetApp()->GetProfileInt(_T("State"), _T("SortUntranslatedAtTop"), TRUE);

	m_lcDictItems.EnableToolTips(m_bShowTooltips);
	m_lcDictItems.SetSortUntranslatedAtTop(m_bSortUntranslatedAtTop);

	CString sDefBrowsePath;
	FileMisc::MakePath(sDefBrowsePath, NULL, FileMisc::GetModuleFolder(), _T("Resources\\Translations"));
	m_sLastBrowsePath = AfxGetApp()->GetProfileString(_T("State"), _T("LastBrowsePath"), sDefBrowsePath);

	CRect rect;
	rect.left = AfxGetApp()->GetProfileInt(_T("State"), _T("Left"), -1);
	rect.top = AfxGetApp()->GetProfileInt(_T("State"), _T("Top"), -1);
	rect.right = AfxGetApp()->GetProfileInt(_T("State"), _T("Right"), -1);
	rect.bottom = AfxGetApp()->GetProfileInt(_T("State"), _T("Bottom"), -1);

	if (rect.Width() > 0 && rect.Height() > 0)
	{
		// ensure this intersects with the closest screen by a decent amount
		int BORDER = 200;
		rect.DeflateRect(BORDER, BORDER);
		
		CRect rUnused;
		
		if (GraphicsMisc::GetAvailableScreenSpace(rect, rUnused))
		{
			rect.InflateRect(BORDER, BORDER);
			
			// because the position was saved using the results of 
			// GetWindowPlacement we must use SetWindowPlacement
			// to restore the window
			WINDOWPLACEMENT wp = { 0 };
			
			wp.rcNormalPosition = rect;
			wp.ptMaxPosition.x = -1;
			wp.ptMaxPosition.y = -1;
			wp.ptMinPosition.x = -1;
			wp.ptMinPosition.y = -1;
			
			if (AfxGetApp()->GetProfileInt(_T("State"), _T("Maximised"), FALSE))
				wp.showCmd = SW_MAXIMIZE;
			
			SetWindowPlacement(&wp);
		}
		else
		{
			ASSERT(0);
			Resize();
		}
	}
	else
	{
		Resize();
	}
	
	// Right at the end so columns do not get auto-resized again
	int nColWidths[NUM_COLS];
	nColWidths[TRANS_COL] = AfxGetApp()->GetProfileInt(_T("State"), _T("TransColWidth"), 300);
	nColWidths[ENG_COL] = AfxGetApp()->GetProfileInt(_T("State"), _T("EnglishColWidth"), 300);
	nColWidths[HINT_COL] = AfxGetApp()->GetProfileInt(_T("State"), _T("HintColWidth"), 100);
	VERIFY(m_lcDictItems.SetColumnWidths(nColWidths));
	
	if (bFirstTime)
		m_lcDictItems.RecalcColumnWidths();
	
	CString sDictPath = AfxGetApp()->GetProfileString(_T("State"), _T("LastDictionary"));
	
	if (!sDictPath.IsEmpty())
		LoadDictionary(sDictPath, FALSE);
}

void CTDLTransEditDlg::SaveState()
{
	AfxGetApp()->WriteProfileString(_T("State"), _T("LastDictionary"), m_dictionary.GetDictionaryPath());
	AfxGetApp()->WriteProfileString(_T("State"), _T("LastBrowsePath"), m_sLastBrowsePath);
	AfxGetApp()->WriteProfileString(_T("State"), _T("YourLanguage"), m_sYourLanguagePath);
	AfxGetApp()->WriteProfileInt(_T("State"), _T("ShowAlternatives"), m_bShowAlternatives);
	AfxGetApp()->WriteProfileInt(_T("State"), _T("ShowTooltips"), m_bShowTooltips);
	AfxGetApp()->WriteProfileInt(_T("State"), _T("SortUntranslatedAtTop"), m_bSortUntranslatedAtTop);
	AfxGetApp()->WriteProfileString(_T("State"), _T("LastFilter"), m_sFilter);

	int nColWidths[NUM_COLS];
	VERIFY(m_lcDictItems.GetColumnWidths(nColWidths));

	AfxGetApp()->WriteProfileInt(_T("State"), _T("EnglishColWidth"), nColWidths[ENG_COL]);
	AfxGetApp()->WriteProfileInt(_T("State"), _T("TransColWidth"), nColWidths[TRANS_COL]);
	AfxGetApp()->WriteProfileInt(_T("State"), _T("HintColWidth"), nColWidths[HINT_COL]);

	WINDOWPLACEMENT wp = { 0 };
	
	if (GetWindowPlacement(&wp))
	{
		AfxGetApp()->WriteProfileInt(_T("State"), _T("Left"), wp.rcNormalPosition.left);
		AfxGetApp()->WriteProfileInt(_T("State"), _T("Top"), wp.rcNormalPosition.top);
		AfxGetApp()->WriteProfileInt(_T("State"), _T("Right"), wp.rcNormalPosition.right);
		AfxGetApp()->WriteProfileInt(_T("State"), _T("Bottom"), wp.rcNormalPosition.bottom);

		AfxGetApp()->WriteProfileInt(_T("State"), _T("Maximised"), IsZoomed());
	}
}

void CTDLTransEditDlg::OnListItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// Only interested in the newly selected item
	if (!(pNMLV->uOldState & LVIS_SELECTED) && (pNMLV->uNewState & LVIS_SELECTED))
	{
		CString sHint(m_lcDictItems.GetItemText(pNMLV->iItem, HINT_COL));

		if (sHint.IsEmpty())
			m_sEnglish = m_lcDictItems.GetItemText(pNMLV->iItem, ENG_COL);
		else
			m_sEnglish.Format(_T("%s [%s]"), m_lcDictItems.GetItemText(pNMLV->iItem, ENG_COL), sHint);

		m_sTranslation = m_lcDictItems.GetItemText(pNMLV->iItem, TRANS_COL);

		UpdateData(FALSE);
	}

	GetDlgItem(IDC_TRANSLATION)->EnableWindow(m_lcDictItems.GetCurSel() != -1);

	*pResult = 0;
}


void CTDLTransEditDlg::OnChangeTranslation()
{
	UpdateData();

	int nSel = m_lcDictItems.GetCurSel();

	if (nSel != -1)
	{
		if (ModifyDictionaryItem(nSel, m_sTranslation))
			m_lcDictItems.SetItemText(nSel, TRANS_COL, m_sTranslation);
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CTDLTransEditDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialogHelper::ExcludeCtrl(this, IDC_DICTIONARY, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_ENGLISH, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_TRANSLATION, pDC);

	CDialog::OnEraseBkgnd(pDC);

	// Draw a divider under the menu
	CRect rDivider;

	GetClientRect(rDivider);
	rDivider.bottom = (rDivider.top + 1);

	pDC->FillSolidRect(rDivider, GetSysColor(COLOR_3DSHADOW));

	return TRUE;
}

void CTDLTransEditDlg::OnShowAlternatives() 
{
	m_bShowAlternatives = !m_bShowAlternatives;

	m_lcDictItems.RebuildList(m_dictionary, m_bShowAlternatives, m_sFilter);
}

void CTDLTransEditDlg::OnUpdateFilter() 
{
	// Only refilter on change
	CString sPrevFilter(m_sFilter);
	UpdateData();

	if (m_sFilter.CompareNoCase(sPrevFilter) != 0)
		m_lcDictItems.RebuildList(m_dictionary, m_bShowAlternatives, m_sFilter);
}

void CTDLTransEditDlg::OnClearFilter() 
{
	if (!m_sFilter.IsEmpty())
	{
		m_sFilter.Empty();
		UpdateData(FALSE);
		
		m_lcDictItems.RebuildList(m_dictionary, m_bShowAlternatives, m_sFilter);
	}
}

void CTDLTransEditDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	switch (nIndex)
	{
	case 0: // File
		pPopupMenu->EnableMenuItem(ID_FILE_SAVE_TRANSLATION, ((!IsReadOnly() && m_bEdited) ? MF_ENABLED : MF_DISABLED));
		break;

	case 1: // Options
		pPopupMenu->CheckMenuItem(ID_OPTIONS_SHOWOPTIONALS, (m_bShowAlternatives ? MF_CHECKED : MF_UNCHECKED));
		pPopupMenu->CheckMenuItem(ID_OPTIONS_SHOWTOOLTIPS, (m_bShowTooltips ? MF_CHECKED : MF_UNCHECKED));
		pPopupMenu->CheckMenuItem(ID_OPTIONS_SORTUNTRANSATTOP, (m_bSortUntranslatedAtTop ? MF_CHECKED : MF_UNCHECKED));
		break;

	case 2: // Tools
		pPopupMenu->EnableMenuItem(ID_TOOLS_CLEANUP, ((!IsReadOnly() && !m_dictionary.IsEmpty()) ? MF_ENABLED : MF_DISABLED));
		break;
	}
}

BOOL CTDLTransEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	// Handle 'Enter' in the 'Filter' field
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		if (pMsg->hwnd == *GetDlgItem(IDC_FILTER))
		{
			OnUpdateFilter();
			return TRUE;
		}
		else if (pMsg->hwnd == m_lcDictItems)
		{
			m_lcDictItems.EditSelectedCell();
			return TRUE;
		}
	}
	else if (m_mgrShortcuts.ProcessMessage(pMsg))
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CTDLTransEditDlg::OnToolsCleanUp() 
{
	if (!RecheckYourLanguagePath(m_dictionary.GetDictionaryPath()))
		return;

	// Save any changes first
	OnFileSaveTranslation();
	
	if (TransText::CleanupDictionary(m_sYourLanguagePath, m_dictionary.GetDictionaryPath()))
	{
		LoadDictionary(m_dictionary.GetDictionaryPath());
	}
}

void CTDLTransEditDlg::OnFileNewTranslation() 
{
	CString sYourLangPath(m_sYourLanguagePath);

	if (!RecheckYourLanguagePath(sYourLangPath))
	{
		CFileOpenDialog dialog(_T("Select Base Language File"), m_sLastBrowsePath, 
								_T("YourLanguage.csv"), EOFN_DEFAULTOPEN, 
								_T("YourLanguage.csv|YourLanguage.csv||"));

		if (dialog.DoModal() != IDOK)
			return;

		// else
		sYourLangPath = dialog.GetPathName();
	}

	CFileSaveDialog dialog(_T("Save New Translation"), _T(".csv"), 
							m_sLastBrowsePath, EOFN_DEFAULTSAVE, 
							_T("Translations (*.csv)|*.csv||"));

	while (dialog.DoModal() == IDOK)
	{
		CString sTransPath = dialog.GetPathName();

		// Disallow 'YourLanguage.csv"
		if (FileMisc::GetFileNameFromPath(sTransPath).CompareNoCase(_T("YourLanguage.csv")) != 0)
		{
			m_sLastBrowsePath = FileMisc::GetFolderFromFilePath(sYourLangPath);

			FileMisc::CopyFile(sYourLangPath, sTransPath, TRUE, TRUE);
			VERIFY(LoadDictionary(sTransPath));

			break;
		}

		// else
		AfxMessageBox(_T("'YourLanguage.csv' is a reserved filename.\n\nPlease choose another."), MB_OK | MB_ICONEXCLAMATION);
	}
}

BOOL CTDLTransEditDlg::RecheckYourLanguagePath(LPCTSTR szDictPath, BOOL bAllowPrompt)
{
	if (!szDictPath || !szDictPath[0])
		return FALSE;

	// Construct the 'YourLanguage.csv' path from the dictionary path
	CString sDictFolder(FileMisc::GetFolderFromFilePath(szDictPath));

	CString sYourLangPath;
	FileMisc::MakePath(sYourLangPath, NULL, sDictFolder, _T("YourLanguage"), _T("csv"));

	if (!FileMisc::FileExists(sYourLangPath))
	{
		sYourLangPath.Empty();
	}
	else
	{
		// Compare version info
		CString sTransVer = GetTranslationVersion(szDictPath);
		CString sYourLangVer = GetTranslationVersion(sYourLangPath);

		if ((FileMisc::CompareVersions(sYourLangVer, sTransVer) < 0) ||
			(FileMisc::CompareVersions(m_sAppVer, sYourLangVer) < 0))
		{
			sYourLangPath.Empty();
		}
		else
		{
			CTransDictionary::SetAppVersion(sYourLangVer);
		}
	}

	if (sYourLangPath.IsEmpty() && bAllowPrompt)
	{
		CString sMessage;
		sMessage.Format(_T("Before you can edit '%s' there needs to be an up-to-date copy of 'YourLanguage.csv' in the same folder.\n\nPlease update to the latest 'YourLanguage.csv' and re-open this file to edit."),
						FileMisc::GetFileNameFromPath(szDictPath, FALSE));

		MessageBox(sMessage, _T("Missing File"), MB_OK | MB_ICONEXCLAMATION);
	}
	
	m_sYourLanguagePath = sYourLangPath;
	
	return !m_sYourLanguagePath.IsEmpty();
}

BOOL CTDLTransEditDlg::IsReadOnly() const 
{ 
	return (m_sYourLanguagePath.IsEmpty() || m_dictionary.IsReadOnly()); 
}

void CTDLTransEditDlg::OnOptionsShowTooltips() 
{
	m_bShowTooltips = !m_bShowTooltips;

	m_lcDictItems.EnableToolTips(m_bShowTooltips);
}

CString CTDLTransEditDlg::GetTranslationVersion(const CString& sTransPath) const
{
	CTransDictionary dict;

	if (dict.LoadDictionary(sTransPath))
		return dict.GetDictionaryVersion();

	// else
	return _T("");
}

void CTDLTransEditDlg::OnOptionsSortUntranslatedAtTop()
{
	m_bSortUntranslatedAtTop = !m_bSortUntranslatedAtTop;

	m_lcDictItems.SetSortUntranslatedAtTop(m_bSortUntranslatedAtTop);
}
