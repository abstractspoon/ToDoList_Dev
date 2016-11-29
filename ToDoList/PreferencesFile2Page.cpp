// PreferencesFilePage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesFile2Page.h"

#include "..\shared\enstring.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\filemisc.h"
#include "..\shared\importexportmgr.h"
#include "..\shared\misc.h"

#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesFile2Page property page

// backup folder type
enum 
{
	PFP_RELATIVETOTASKLIST,
	PFP_RELATIVETOAPP,
	PFP_FULLPATH,
};

IMPLEMENT_DYNCREATE(CPreferencesFile2Page, CPreferencesPageBase)

CPreferencesFile2Page::CPreferencesFile2Page(const CImportExportMgr* pExportMgr) : 
		CPreferencesPageBase(CPreferencesFile2Page::IDD),
		m_eExportFolderPath(FES_FOLDERS | FES_COMBOSTYLEBTN | FES_RELATIVEPATHS),
		m_eSaveExportStylesheet(FES_COMBOSTYLEBTN | FES_RELATIVEPATHS, CEnString(IDS_XSLFILEFILTER)),
		m_pExportMgr(pExportMgr),
		m_eBackupLocation(FES_FOLDERS | FES_COMBOSTYLEBTN)
{
//	m_psp.dwFlags &= ~PSP_HASHELP;

	//{{AFX_DATA_INIT(CPreferencesFile2Page)
	//}}AFX_DATA_INIT
	m_bAutoSaveOnRunTools = TRUE;
	m_nBackupFolderType = -1;
	m_bBackupOnSave = FALSE;
	m_nKeepBackups = 0;
	m_bExportFilteredOnly = FALSE;
	m_bExportToFolder = FALSE;
	m_nAutoSaveFrequency = 5;
	m_bAutoSave = TRUE;
	m_bAutoExport = FALSE;
	m_bAutoSaveOnSwitchTasklist = FALSE;
	m_bAutoSaveOnSwitchApp = FALSE;
	m_bOtherExport = FALSE;
	m_nOtherExporter = -1;
	m_bUseStylesheetForSaveExport = FALSE;
}

CPreferencesFile2Page::~CPreferencesFile2Page()
{
}

void CPreferencesFile2Page::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesFile2Page)
	DDX_Control(pDX, IDC_NUMBACKUPSTOKEEP, m_cbKeepBackups);
	DDX_Control(pDX, IDC_BACKUPLOCATION, m_eBackupLocation);
	DDX_Check(pDX, IDC_BACKUPONSAVE, m_bBackupOnSave);
	DDX_Text(pDX, IDC_BACKUPLOCATION, m_sBackupLocation);
	DDX_CBIndex(pDX, IDC_NUMBACKUPSTOKEEP, m_nKeepBackups);
	DDX_Check(pDX, IDC_EXPORTFILTERED, m_bExportFilteredOnly);
	DDX_Check(pDX, IDC_AUTOSAVEONRUNNINGTOOLS, m_bAutoSaveOnRunTools);
	DDX_Radio(pDX, IDC_RELTOTASKLIST, m_nBackupFolderType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_OTHEREXPORTERS, m_cbOtherExporters);
	DDX_Control(pDX, IDC_SAVEEXPORTSTYLESHEET, m_eSaveExportStylesheet);
	DDX_Control(pDX, IDC_EXPORTFOLDER, m_eExportFolderPath);
	DDX_Check(pDX, IDC_EXPORTTOFOLDER, m_bExportToFolder);
	DDX_Text(pDX, IDC_EXPORTFOLDER, m_sExportFolderPath);
	DDX_Text(pDX, IDC_SAVEEXPORTSTYLESHEET, m_sSaveExportStylesheet);
	DDX_Check(pDX, IDC_USESTYLESHEETFORSAVE, m_bUseStylesheetForSaveExport);
	DDX_Check(pDX, IDC_AUTOSAVEONSWITCHTASKLIST, m_bAutoSaveOnSwitchTasklist);
	DDX_Check(pDX, IDC_AUTOSAVEONSWITCHAPP, m_bAutoSaveOnSwitchApp);
	DDX_Radio(pDX, IDC_HTMLEXPORT, m_bOtherExport);
	DDX_CBIndex(pDX, IDC_OTHEREXPORTERS, m_nOtherExporter);
	DDX_Check(pDX, IDC_AUTOEXPORT, m_bAutoExport);
	DDX_Check(pDX, IDC_AUTOSAVE, m_bAutoSave);
	DDX_Text(pDX, IDC_AUTOSAVEFREQUENCY, m_nAutoSaveFrequency);

	// custom
	if (pDX->m_bSaveAndValidate)
	{
		m_nKeepBackups = CDialogHelper::GetSelectedItemAsValue(m_cbKeepBackups);
	}
	else if (!CDialogHelper::SelectItemByValue(m_cbKeepBackups, m_nKeepBackups))
	{
		if (m_nKeepBackups == 0) // all
		{
			// select last string in listbox
			m_cbKeepBackups.SetCurSel(m_cbKeepBackups.GetCount() - 1);
		}
		else
		{
			m_nKeepBackups = 10;
			m_cbKeepBackups.SelectString(-1, _T("10"));
		}
	}
}


BEGIN_MESSAGE_MAP(CPreferencesFile2Page, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesFile2Page)
	ON_BN_CLICKED(IDC_BACKUPONSAVE, OnBackuponsave)
	ON_BN_CLICKED(IDC_RELTOAPP, OnChangeBackupFolderType)
	ON_BN_CLICKED(IDC_RELTOTASKLIST, OnChangeBackupFolderType)
	ON_BN_CLICKED(IDC_FULLPATH, OnChangeBackupFolderType)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_EXPORTTOFOLDER, OnExporttofolder)
	ON_BN_CLICKED(IDC_AUTOEXPORT, OnAutoexport)
	ON_BN_CLICKED(IDC_USESTYLESHEETFORSAVE, OnUsestylesheetforsave)
	ON_BN_CLICKED(IDC_HTMLEXPORT, OnHtmlexport)
	ON_BN_CLICKED(IDC_OTHEREXPORT, OnOtherexport)
	ON_BN_CLICKED(IDC_AUTOSAVE, OnAutosave)
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesFile2Page message handlers

BOOL CPreferencesFile2Page::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	m_mgrGroupLines.AddGroupLine(IDC_BACKUPGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_SAVEGROUP, *this);
	
	GetDlgItem(IDC_BACKUPLOCATION)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_BACKUPLOCATIONLABEL)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_FULLPATH)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_RELTOAPP)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_RELTOTASKLIST)->EnableWindow(m_bBackupOnSave);

	GetDlgItem(IDC_BACKUPCOUNTLABEL)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_NUMBACKUPSTOKEEP)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_BACKUPCOUNTTRAIL)->EnableWindow(m_bBackupOnSave);

	GetDlgItem(IDC_AUTOSAVEFREQUENCY)->EnableWindow(m_bAutoSave);
	GetDlgItem(IDC_EXPORTTOFOLDER)->EnableWindow(m_bAutoExport);
	GetDlgItem(IDC_EXPORTFOLDER)->EnableWindow(m_bAutoExport && m_bExportToFolder);
	GetDlgItem(IDC_HTMLEXPORT)->EnableWindow(m_bAutoExport);
	GetDlgItem(IDC_USESTYLESHEETFORSAVE)->EnableWindow(m_bAutoExport && !m_bOtherExport);
	GetDlgItem(IDC_SAVEEXPORTSTYLESHEET)->EnableWindow(m_bAutoExport && !m_bOtherExport && m_bUseStylesheetForSaveExport);
	GetDlgItem(IDC_OTHEREXPORT)->EnableWindow(m_bAutoExport);
	GetDlgItem(IDC_OTHEREXPORTERS)->EnableWindow(m_bAutoExport && m_bOtherExport);
	GetDlgItem(IDC_EXPORTFILTERED)->EnableWindow(m_bAutoExport);

	// build the exporter format comboxbox
	ASSERT(m_pExportMgr);

	for (int nExp = 0; nExp < m_pExportMgr->GetNumExporters(); nExp++)
	{
		CString sExt = m_pExportMgr->GetExporterFileExtension(nExp);
		
		if (!sExt.IsEmpty())
		{
			CString sItem;
			sItem.Format(_T("%s (.%s)"), m_pExportMgr->GetExporterMenuText(nExp), sExt);

			m_cbOtherExporters.AddString(sItem);
		}
	}

	m_cbOtherExporters.SetCurSel(m_nOtherExporter);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPreferencesFile2Page::OnEEBtnClick(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_BACKUPLOCATION:
		if (lParam == FEBTN_BROWSE)
		{
			UpdateData();

			switch (m_nBackupFolderType)
			{
			case PFP_FULLPATH:
				// nothing to do
				break;

			case PFP_RELATIVETOAPP:
				FileMisc::MakeRelativePath(m_sBackupLocation, FileMisc::GetAppFolder(), TRUE);
 				break;

			case PFP_RELATIVETOTASKLIST:
				FileMisc::SplitPath(m_sBackupLocation, NULL, NULL, &m_sBackupLocation);
				break;
			}

			UpdateData(FALSE);
		}
		break;
	}
	
	return 0L;
}

void CPreferencesFile2Page::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bBackupOnSave = pPrefs->GetProfileInt(szKey, _T("BackupOnSave"), TRUE);
	m_sBackupLocation = pPrefs->GetProfileString(szKey, _T("BackupLocation"), _T("backup"));
	m_nKeepBackups = pPrefs->GetProfileInt(szKey, _T("KeepBackups"), 10);
	m_nBackupFolderType = pPrefs->GetProfileInt(szKey, _T("BackupFolderType"), PFP_RELATIVETOAPP);
	
	// saving
	m_nAutoSaveFrequency = pPrefs->GetProfileInt(szKey, _T("AutoSaveFrequency"), 1);
	m_bAutoSave = pPrefs->GetProfileInt(szKey, _T("AutoSave"), (m_nAutoSaveFrequency > 0));
	m_bAutoExport = pPrefs->GetProfileInt(szKey, _T("AutoHtmlExport"), FALSE);
	m_sExportFolderPath = pPrefs->GetProfileString(szKey, _T("ExportFolderPath"));
	m_sSaveExportStylesheet = pPrefs->GetProfileString(szKey, _T("SaveExportStylesheet"));
	m_bAutoSaveOnSwitchTasklist = pPrefs->GetProfileInt(szKey, _T("AutoSaveOnSwitchTasklist"), FALSE);
	m_bAutoSaveOnSwitchApp = pPrefs->GetProfileInt(szKey, _T("AutoSaveOnSwitchApp"), FALSE);
	m_bOtherExport = pPrefs->GetProfileInt(szKey, _T("OtherExport"), FALSE);
	m_nOtherExporter = pPrefs->GetProfileInt(szKey, _T("OtherExporter"), 1);
	m_bExportFilteredOnly = pPrefs->GetProfileInt(szKey, _T("ExportFilteredOnly"), FALSE);
	m_bAutoSaveOnRunTools = pPrefs->GetProfileInt(szKey, _T("AutoSaveOnRunTools"), TRUE);

	// these are dependent on the values they control for backward compat
	m_bUseStylesheetForSaveExport = pPrefs->GetProfileInt(szKey, _T("UseStylesheetForSaveExport"), !m_sSaveExportStylesheet.IsEmpty());
	m_bExportToFolder = pPrefs->GetProfileInt(szKey, _T("ExportToFolder"), !m_sExportFolderPath.IsEmpty());

	Misc::Trim(m_sExportFolderPath);

	// set file edit directories and make paths relative
	CString sFolder = FileMisc::GetAppFolder();

	m_eExportFolderPath.SetCurrentFolder(sFolder);
	m_sExportFolderPath = FileMisc::GetRelativePath(m_sExportFolderPath, sFolder, TRUE);

	sFolder = FileMisc::GetAppResourceFolder() + _T("\\Stylesheets");

	m_eSaveExportStylesheet.SetCurrentFolder(sFolder);
	m_sSaveExportStylesheet = FileMisc::GetRelativePath(m_sSaveExportStylesheet, sFolder, FALSE);

//	m_b = pPrefs->GetProfileInt(szKey, _T(""), FALSE);
}

void CPreferencesFile2Page::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("BackupOnSave"), m_bBackupOnSave);
	pPrefs->WriteProfileString(szKey, _T("BackupLocation"), m_sBackupLocation);
	pPrefs->WriteProfileInt(szKey, _T("KeepBackups"), m_nKeepBackups);
	pPrefs->WriteProfileInt(szKey, _T("BackupFolderType"), m_nBackupFolderType);

	// saving
	pPrefs->WriteProfileInt(szKey, _T("AutoSave"), m_bAutoSave);
	pPrefs->WriteProfileInt(szKey, _T("AutoSaveFrequency"), m_nAutoSaveFrequency);
	pPrefs->WriteProfileInt(szKey, _T("AutoHtmlExport"), m_bAutoExport);
	pPrefs->WriteProfileInt(szKey, _T("ExportToFolder"), m_bExportToFolder);
	pPrefs->WriteProfileString(szKey, _T("ExportFolderPath"), m_sExportFolderPath);
	pPrefs->WriteProfileInt(szKey, _T("UseStylesheetForSaveExport"), m_bUseStylesheetForSaveExport);
	pPrefs->WriteProfileString(szKey, _T("SaveExportStylesheet"), m_sSaveExportStylesheet);
	pPrefs->WriteProfileInt(szKey, _T("AutoSaveOnSwitchTasklist"), m_bAutoSaveOnSwitchTasklist);
	pPrefs->WriteProfileInt(szKey, _T("AutoSaveOnSwitchApp"), m_bAutoSaveOnSwitchApp);
	pPrefs->WriteProfileInt(szKey, _T("OtherExport"), m_bOtherExport);
	pPrefs->WriteProfileInt(szKey, _T("OtherExporter"), m_nOtherExporter);
	pPrefs->WriteProfileInt(szKey, _T("ExportFilteredOnly"), m_bExportFilteredOnly);
	pPrefs->WriteProfileInt(szKey, _T("AutoSaveOnRunTools"), m_bAutoSaveOnRunTools);

//	pPrefs->WriteProfileInt(szKey, _T(""), m_b);
}

void CPreferencesFile2Page::OnBackuponsave() 
{
	UpdateData();

	GetDlgItem(IDC_BACKUPLOCATION)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_BACKUPLOCATIONLABEL)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_FULLPATH)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_RELTOAPP)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_RELTOTASKLIST)->EnableWindow(m_bBackupOnSave);

	GetDlgItem(IDC_BACKUPCOUNTLABEL)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_NUMBACKUPSTOKEEP)->EnableWindow(m_bBackupOnSave);
	GetDlgItem(IDC_BACKUPCOUNTTRAIL)->EnableWindow(m_bBackupOnSave);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFile2Page::OnAutosave() 
{
	UpdateData();

	GetDlgItem(IDC_AUTOSAVEFREQUENCY)->EnableWindow(m_bAutoSave);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFile2Page::OnExporttofolder() 
{
	UpdateData();

	GetDlgItem(IDC_EXPORTFOLDER)->EnableWindow(m_bAutoExport && m_bExportToFolder);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFile2Page::OnAutoexport() 
{
	UpdateData();	

	GetDlgItem(IDC_EXPORTTOFOLDER)->EnableWindow(m_bAutoExport);
	GetDlgItem(IDC_HTMLEXPORT)->EnableWindow(m_bAutoExport);
	GetDlgItem(IDC_OTHEREXPORT)->EnableWindow(m_bAutoExport);
	GetDlgItem(IDC_EXPORTFOLDER)->EnableWindow(m_bAutoExport && m_bExportToFolder);
	GetDlgItem(IDC_USESTYLESHEETFORSAVE)->EnableWindow(m_bAutoExport && !m_bOtherExport);
	GetDlgItem(IDC_SAVEEXPORTSTYLESHEET)->EnableWindow(m_bAutoExport && !m_bOtherExport && m_bUseStylesheetForSaveExport);
	GetDlgItem(IDC_OTHEREXPORTERS)->EnableWindow(m_bAutoExport && m_bOtherExport);
	GetDlgItem(IDC_EXPORTFILTERED)->EnableWindow(m_bAutoExport);
	
	CPreferencesPageBase::OnControlChange();
}

CString CPreferencesFile2Page::GetAutoExportFolderPath() const 
{ 
	if (m_bAutoExport && m_bExportToFolder && !m_sExportFolderPath.IsEmpty())
		return FileMisc::GetFullPath(m_sExportFolderPath, FileMisc::GetAppFolder());
	
	// else
	return _T("");
}

void CPreferencesFile2Page::OnUsestylesheetforsave() 
{
	UpdateData();
	
	GetDlgItem(IDC_USESTYLESHEETFORSAVE)->EnableWindow(m_bAutoExport);
	GetDlgItem(IDC_SAVEEXPORTSTYLESHEET)->EnableWindow(m_bAutoExport && m_bUseStylesheetForSaveExport);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFile2Page::OnHtmlexport() 
{
	UpdateData();	

	GetDlgItem(IDC_USESTYLESHEETFORSAVE)->EnableWindow(m_bAutoExport && !m_bOtherExport);
	GetDlgItem(IDC_SAVEEXPORTSTYLESHEET)->EnableWindow(m_bAutoExport && !m_bOtherExport && m_bUseStylesheetForSaveExport);
	GetDlgItem(IDC_OTHEREXPORTERS)->EnableWindow(m_bAutoExport && m_bOtherExport);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesFile2Page::OnOtherexport() 
{
	UpdateData();	

	GetDlgItem(IDC_USESTYLESHEETFORSAVE)->EnableWindow(m_bAutoExport && !m_bOtherExport);
	GetDlgItem(IDC_SAVEEXPORTSTYLESHEET)->EnableWindow(m_bAutoExport && !m_bOtherExport && m_bUseStylesheetForSaveExport);
	GetDlgItem(IDC_OTHEREXPORTERS)->EnableWindow(m_bAutoExport && m_bOtherExport);

	CPreferencesPageBase::OnControlChange();
}

CString CPreferencesFile2Page::GetSaveExportStylesheet() const 
{ 
	if (m_bUseStylesheetForSaveExport && !m_sSaveExportStylesheet.IsEmpty())
		return FileMisc::GetFullPath(m_sSaveExportStylesheet, FileMisc::GetAppResourceFolder() + _T("\\Stylesheets")); 

	// else
	return _T("");
}

CString CPreferencesFile2Page::GetBackupLocation(LPCTSTR szFilePath) const 
{ 
	CString sBackup;

	if (m_bBackupOnSave)
	{
		switch (m_nBackupFolderType)
		{
		case PFP_FULLPATH:
			sBackup = m_sBackupLocation;
			break;

		case PFP_RELATIVETOAPP:
			sBackup = FileMisc::GetFullPath(m_sBackupLocation, FileMisc::GetAppFolder());
			break;

		case PFP_RELATIVETOTASKLIST:
 			sBackup = FileMisc::GetFullPath(m_sBackupLocation, FileMisc::GetFolderFromFilePath(szFilePath));
			break;

		default:
			ASSERT(0);
		}
	}

	sBackup.TrimRight();
	return sBackup;
}

void CPreferencesFile2Page::OnChangeBackupFolderType() 
{
	int nPrevType = m_nBackupFolderType;

	UpdateData();

	// nothing to do if both are relative
	if (nPrevType != PFP_FULLPATH && m_nBackupFolderType != PFP_FULLPATH)
	{
		return;
	}
	// else if was full path then make relative
	else if (nPrevType == PFP_FULLPATH)
	{
		if (m_nBackupFolderType == PFP_RELATIVETOTASKLIST)
			FileMisc::SplitPath(m_sBackupLocation, NULL, NULL, &m_sBackupLocation);
		else
			FileMisc::MakeRelativePath(m_sBackupLocation, FileMisc::GetAppFolder(), TRUE);
	}
	// else make full
	else
	{
		FileMisc::MakeFullPath(m_sBackupLocation, FileMisc::GetAppFolder());
	}

	UpdateData(FALSE);
}
