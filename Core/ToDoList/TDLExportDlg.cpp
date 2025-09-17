// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLExportDlg.h"

#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\misc.h"
#include "..\Shared\DateHelper.h"

#include "..\Interfaces\Preferences.h"

#include <shlwapi.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog

CTDLExportDlg::CTDLExportDlg(LPCTSTR szSingleFileTitle,
							 LPCTSTR szMultiFileTitle,
							 const CTDCImportExportMgr& mgr,
							 BOOL bSingleTaskList,
							 BOOL bEnableSubtaskSelection,
							 BOOL bVisibleColumnsOnly,
							 LPCTSTR szFilePath,
							 LPCTSTR szFolderPath,
							 const CTDCCustomAttribDefinitionArray& aAttribDefs,
							 CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_EXPORT_DIALOG, _T("Exporting"), pParent), 
	m_pageTo(mgr, bSingleTaskList, szFilePath, szFolderPath, _T("Exporting")),
	m_pageTaskSel(aAttribDefs, _T("Exporting"), bEnableSubtaskSelection, bVisibleColumnsOnly),
	m_mgrImportExport(mgr),
	m_sSingleFileTitle(szSingleFileTitle),
	m_sMultiFileTitle(szMultiFileTitle),
	m_nPrevActiveTab(0)
{
	//{{AFX_DATA_INIT(CExportDlg)
	//}}AFX_DATA_INIT

	// retrieve previous user input
	CPreferences prefs;

	m_bExportDate = prefs.GetProfileInt(m_sPrefsKey, _T("WantDate"), TRUE);
	m_nPrevActiveTab = prefs.GetProfileInt(m_sPrefsKey, _T("PrevActiveTab"), 0);

	// share same title history as print dialog
	m_cbTitle.Load(prefs, _T("Print"));

	m_ppHost.AddPage(&m_pageTo, CEnString(IDS_EXPORTDLG_EXPORT));
	m_ppHost.AddPage(&m_pageTaskSel, CEnString(IDS_EXPORTDLG_TASKSEL));

	m_pageTo.ForwardMessage(WM_FE_GETFILEICON);
	m_ppHost.ForwardMessage(WM_FE_GETFILEICON);
}


void CTDLExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CExportDlg)
	DDX_Control(pDX, IDC_EXPORTTITLE, m_cbTitle);
	DDX_CBString(pDX, IDC_EXPORTTITLE, m_sExportTitle);
	DDX_Check(pDX, IDC_EXPORTDATE, m_bExportDate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTDLExportDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CExportDlg)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_TASKLISTOPTIONS, OnSelchangeTasklistoptions)
	ON_BN_CLICKED(IDC_EXPORTONEFILE, OnExportonefile)
	ON_EN_CHANGE(IDC_EXPORTPATH, OnChangeExportpath)
	ON_CBN_EDITCHANGE(IDC_EXPORTTITLE, OnChangeExportTitle)
	ON_BN_CLICKED(IDC_TOPATH, OnExportToClipboardOrPath)
	ON_BN_CLICKED(IDC_TOCLIPBOARD, OnExportToClipboardOrPath)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDlg message handlers

BOOL CTDLExportDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	m_ppHost.Create(IDC_PAGEHOST, this);
	m_ppHost.SetActivePage(m_nPrevActiveTab);

	UpdateTitle();
	EnableOK();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLExportDlg::OnChangeExportTitle()
{
	UpdateData();

	// Update our cached values as we go so that we know
	// with certainty that they are always up to date
	if (GetExportAllTasklists())
	{
		if (GetExportOneFile())
			m_sMultiFileTitle = m_sExportTitle;
	}
	else
	{
		m_sSingleFileTitle = m_sExportTitle;
	}
}

void CTDLExportDlg::OnChangeExportpath()
{
	EnableOK();
}

void CTDLExportDlg::OnSelchangeTasklistoptions()
{
	UpdateTitle();
}

void CTDLExportDlg::OnExportonefile()
{
	UpdateTitle();
}

void CTDLExportDlg::OnExportToClipboardOrPath()
{
	UpdateTitle();
}

void CTDLExportDlg::UpdateTitle()
{
	static CEnString MULTI_FILE_TITLE(IDS_EXPORTTITLE_MULTIPLEFILES);

	if (GetExportAllTasklists())
	{
		if (GetExportOneFile())
			m_sExportTitle = m_sMultiFileTitle;
		else
			m_sExportTitle = MULTI_FILE_TITLE;
	}
	else
	{
		m_sExportTitle = m_sSingleFileTitle;
	}

	SetDlgItemText(IDC_EXPORTTITLE, m_sExportTitle);
	GetDlgItem(IDC_EXPORTTITLE)->EnableWindow(m_sExportTitle != MULTI_FILE_TITLE);
}

void CTDLExportDlg::OnOK()
{
	m_ppHost.OnOK();
	
	CString sFormatID = GetFormatTypeID();
	CString sExportPath = GetExportPath();
	BOOL bExportAllTasklists = GetExportAllTasklists();
	BOOL bExportOneFile = GetExportOneFile(); 
	BOOL bClipboard = GetExportToClipboard();

	int nFormat = m_mgrImportExport.FindExporterByType(sFormatID);
	BOOL bExporterHasFileExt = m_mgrImportExport.ExporterHasFileExtension(nFormat);

	if (bExporterHasFileExt)
	{
		Misc::Trim(sExportPath);

		// if the export path is relative we build a path based
		// on the exe path and check with the user
		if (::PathIsRelative(sExportPath))
		{
			CString sPath = FileMisc::GetAppFilePath(), sDrive, sFolder;

			FileMisc::SplitPath(sPath, &sDrive, &sFolder);
			FileMisc::MakePath(sPath, sDrive, sFolder, sExportPath);

			CString sMessage;
			
			if (bExportAllTasklists)
				sMessage.Format(IDS_ED_CONFIRMEXPORTPATHMULTI, sPath);
			else
				sMessage.Format(IDS_ED_CONFIRMEXPORTPATH, sPath);
							
			UINT nRet = MessageBox(sMessage, CEnString(IDS_ED_CONFIRMEXPORTPATH_TITLE), MB_YESNO);

			if (nRet == IDNO)
			{
				m_ppHost.SetActivePage(0);
				return;
			}
			
			// else
			sExportPath = sPath;
		}

		// make sure the output folder is valid
		BOOL bBadFolder = (bExportAllTasklists && !bExportOneFile) ? 
							!FileMisc::CreateFolder(sExportPath) : 
							!FileMisc::CreateFolderFromFilePath(sExportPath);

		if (bBadFolder)
		{
			CEnString sMessage(IDS_ED_NOMAKEEXPORTPATH, sExportPath);
			
			UINT nRet = MessageBox(sMessage, CEnString(IDS_ED_NOMAKEEXPORTPATH_TITLE), MB_OKCANCEL);

			if (nRet == IDOK)
				m_ppHost.SetActivePage(0); // don't close dialog
			else
				EndDialog(IDCANCEL);

			return;
		}
	}

	CTDLDialog::OnOK();

	// save settings
	CPreferences prefs;

	prefs.WriteProfileInt(m_sPrefsKey, _T("WantDate"), m_bExportDate);
	prefs.WriteProfileInt(m_sPrefsKey, _T("PrevActiveTab"), m_ppHost.GetActiveIndex());

	// share same title history as print dialog
	m_cbTitle.Save(prefs, _T("Print"));
}

void CTDLExportDlg::EnableOK()
{
	CString sFormatID = GetFormatTypeID();
	BOOL bEnable = FALSE;

	if (!sFormatID.IsEmpty())
	{
		int nFormat = m_mgrImportExport.FindExporterByType(sFormatID);
		bEnable = (!GetExportPath().IsEmpty() || !m_mgrImportExport.ExporterHasFileExtension(nFormat));
	}

	GetDlgItem(IDOK)->EnableWindow(bEnable);
}

COleDateTime CTDLExportDlg::GetExportDate() const
{
	if (m_bExportDate)
		return CDateHelper::GetDate(DHD_TODAY);

	// else
	return CDateHelper::NullDate();
}

/////////////////////////////////////////////////////////////////////////////
// CTDLExportTaskSelectionPage dialog

CTDLExportTaskSelectionPage::CTDLExportTaskSelectionPage(const CTDCCustomAttribDefinitionArray& aAttribDefs,
														 LPCTSTR szRegKey, 
														 BOOL bEnableSubtaskSelection,
														 BOOL bVisibleColumnsOnly)
	:
	CCmdNotifyPropertyPage(CTDLExportTaskSelectionPage::IDD),
	m_dlgTaskSel(aAttribDefs, szRegKey, bEnableSubtaskSelection, bVisibleColumnsOnly)
{
	//{{AFX_DATA_INIT(CTDLExportTaskSelectionPage)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTDLExportTaskSelectionPage::~CTDLExportTaskSelectionPage()
{
}

void CTDLExportTaskSelectionPage::DoDataExchange(CDataExchange* pDX)
{
	CCmdNotifyPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLExportTaskSelectionPage)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLExportTaskSelectionPage, CCmdNotifyPropertyPage)
	//{{AFX_MSG_MAP(CTDLExportTaskSelectionPage)
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLExportTaskSelectionPage message handlers

BOOL CTDLExportTaskSelectionPage::OnInitDialog()
{
	CCmdNotifyPropertyPage::OnInitDialog();

	VERIFY(m_dlgTaskSel.Create(IDC_FRAME, this));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLExportTaskSelectionPage::OnOK()
{
	CCmdNotifyPropertyPage::OnOK();

	m_dlgTaskSel.OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CTDLExportToPage dialog

CTDLExportToPage::CTDLExportToPage(const CTDCImportExportMgr& mgr,
								   BOOL bSingleTaskList,
								   LPCTSTR szFilePath,
								   LPCTSTR szFolderPath,
								   LPCTSTR szPrefsKey)
	: 
	CCmdNotifyPropertyPage(CTDLExportToPage::IDD), 
	m_mgrImportExport(mgr),
	m_bSingleTaskList(bSingleTaskList), 
	m_sFilePath(szFilePath),
	m_sFolderPath(szFolderPath),
	m_sPrefsKey(szPrefsKey),
	m_eExportPath(FES_SAVEAS | FES_NOPROMPTOVERWRITE), // parent handles prompting
	m_cbFormat(mgr, FALSE, FALSE),
	m_nHtmlStyle(TDLPDS_WRAP)
{
	//{{AFX_DATA_INIT(CExportDlg)
	//}}AFX_DATA_INIT

	// retrieve previous user input
	CPreferences prefs;

	m_bExportOneFile = prefs.GetProfileInt(m_sPrefsKey, _T("ExportOneFile"), FALSE);
	m_bExportToClipboard = prefs.GetProfileInt(m_sPrefsKey, _T("ExportToClipboard"), FALSE);
	m_bExportAllTasklists = (!m_bSingleTaskList && prefs.GetProfileInt(m_sPrefsKey, _T("ExportOption"), FALSE));

	m_sFormatTypeID = prefs.GetProfileString(m_sPrefsKey, _T("ExporterTypeID"));
	m_sMultiFilePath = prefs.GetProfileString(m_sPrefsKey, _T("LastMultiFilePath"));

	///////////////////////////////////////////////////////////////////////////
	// Backwards compatibility - we no longer persist the path extensions
	// to fix issues with double-barrelled extensions. Remove in 9.2+
	if (FileMisc::HasExtension(m_sMultiFilePath))
		m_sMultiFilePath.Empty();
	///////////////////////////////////////////////////////////////////////////

	// handle empty file/folder paths
	if (m_sFolderPath.IsEmpty())
	{
		m_sFolderPath = prefs.GetProfileString(m_sPrefsKey, _T("LastFolder"));

		if (m_sFolderPath.IsEmpty())
			m_sFolderPath = FileMisc::GetFolderFromFilePath(szFilePath);
	}

	if (m_sMultiFilePath.IsEmpty())
	{
		CString sDrive, sFolder;
		FileMisc::SplitPath(m_sFilePath, &sDrive, &sFolder);

		// See above - we no longer add a extension
		FileMisc::MakePath(m_sMultiFilePath, sDrive, sFolder, CEnString(IDS_TDC_MULTIFILE));
	}

	if ((m_sFilePath.IsEmpty() || PathIsRelative(m_sFilePath)) && !m_sFolderPath.IsEmpty())
	{
		CEnString sFName;
		FileMisc::SplitPath(m_sFilePath, NULL, NULL, &sFName);
		
		// handle empty filename
		if (sFName.IsEmpty())
		{
			sFName.LoadString(IDS_TDC_UNTITLEDFILE);
			Misc::Trim(sFName);
		}
		
		FileMisc::MakePath(m_sFilePath, NULL, m_sFolderPath, sFName);
	}
	FileMisc::RemoveExtension(m_sFilePath);

	// prepare initial export path
	if (WantSaveToFolder())
	{
		m_sExportPath = m_sFolderPath;
		m_sPathLabel.LoadString(IDS_ED_FOLDER);
	}
	else // single or multiple files
	{
		if (m_bSingleTaskList || !m_bExportAllTasklists)
			m_sExportPath = m_sFilePath;
		else
			m_sExportPath = m_sMultiFilePath;

		ASSERT(!FileMisc::HasExtension(m_sExportPath));

		EnsureExporterFileExtension(m_sExportPath, m_sFormatTypeID, FALSE);
		m_sPathLabel.LoadString(IDS_ED_FILEPATH);
	}

	m_nHtmlStyle = prefs.GetProfileEnum(m_sPrefsKey, _T("HtmlStyle"), TDLPDS_WRAP);
}

void CTDLExportToPage::DoDataExchange(CDataExchange* pDX)
{
	CCmdNotifyPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CExportDlg)
	DDX_Control(pDX, IDC_FORMATOPTIONS, m_cbFormat);
	DDX_Control(pDX, IDC_EXPORTPATH, m_eExportPath);
	DDX_CBIndex(pDX, IDC_TASKLISTOPTIONS, m_bExportAllTasklists);
	DDX_Check(pDX, IDC_EXPORTONEFILE, m_bExportOneFile);
	DDX_Radio(pDX, IDC_TOPATH, m_bExportToClipboard);
	DDX_Text(pDX, IDC_EXPORTPATH, m_sExportPath);
	DDX_Text(pDX, IDC_TOPATH, m_sPathLabel);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_HTMLOPTIONS_ICON, m_stHtmlOptionIcon);
	DDX_Control(pDX, IDC_HTMLOPTIONS, m_cbHtmlOptions);

	m_cbFormat.DDX(pDX, m_sFormatTypeID);
	m_cbHtmlOptions.DDX(pDX, m_nHtmlStyle);

	m_stHtmlOptionIcon.SetStyle(m_nHtmlStyle);
}


BEGIN_MESSAGE_MAP(CTDLExportToPage, CCmdNotifyPropertyPage)
	//{{AFX_MSG_MAP(CExportDlg)
	ON_CBN_SELENDOK(IDC_FORMATOPTIONS, OnSelchangeExporterFormat)
	ON_CBN_SELENDOK(IDC_TASKLISTOPTIONS, OnSelchangeTasklistoptions)
	ON_BN_CLICKED(IDC_EXPORTONEFILE, OnExportonefile)
	ON_EN_CHANGE(IDC_EXPORTPATH, OnChangeExportpath)
	ON_BN_CLICKED(IDC_TOPATH, OnExportToClipboardOrPath)
	ON_BN_CLICKED(IDC_TOCLIPBOARD, OnExportToClipboardOrPath)
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(IDC_HTMLOPTIONS, OnSelChangeHtmlOption)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDlg message handlers

BOOL CTDLExportToPage::OnInitDialog() 
{
	CCmdNotifyPropertyPage::OnInitDialog();

	RefreshCtrlStates();

	return TRUE;
}

void CTDLExportToPage::OnSelChangeHtmlOption()
{
	UpdateData();
}

void CTDLExportToPage::OnSelchangeTasklistoptions() 
{
	BOOL bPrevExportAll = m_bExportAllTasklists;

	// Must preserve export path before calling UpdateData()
	PreserveExportPath();
	UpdateData();

	// Check for actual change
	if (!Misc::StatesDiffer(bPrevExportAll, m_bExportAllTasklists))
		return;

	RestoreExportPath();
	RefreshCtrlStates();
}

void CTDLExportToPage::OnSelchangeExporterFormat() 
{
	CString sPrevTypeID = m_sFormatTypeID;

	// Must preserve export path before calling UpdateData()
	PreserveExportPath();
	UpdateData();

	// Check for actual change
	if (m_sFormatTypeID == sPrevTypeID)
		return;

	RestoreExportPath();
	RefreshCtrlStates();
}

void CTDLExportToPage::OnExportonefile()
{
	// Must preserve export path before calling UpdateData()
	PreserveExportPath();
	UpdateData();

	RestoreExportPath();
	RefreshCtrlStates();
}

void CTDLExportToPage::RefreshCtrlStates()
{
	int nFormat = m_mgrImportExport.FindExporterByType(m_sFormatTypeID);
	BOOL bHasExt = m_mgrImportExport.ExporterHasFileExtension(nFormat);

	m_eExportPath.SetDefaultSaveAsFileExtension(m_mgrImportExport.GetExporterFileExtension(nFormat, FALSE));
	m_eExportPath.SetFilter(m_mgrImportExport.GetExporterFileFilter(nFormat));
	m_eExportPath.EnableWindow(bHasExt && !m_bExportToClipboard);

	BOOL bFolder = WantSaveToFolder();

	m_eExportPath.EnableStyle(FES_FOLDERS, bFolder);
	m_sPathLabel.LoadString(bFolder ? IDS_ED_FOLDER : IDS_ED_FILEPATH);

	// set initial control states
	GetDlgItem(IDC_TASKLISTOPTIONS)->EnableWindow(!m_bSingleTaskList);
	GetDlgItem(IDC_EXPORTONEFILE)->EnableWindow(bHasExt && !m_bSingleTaskList && m_bExportAllTasklists && !m_bExportToClipboard);

	BOOL bShowHtmlFormat = m_mgrImportExport.IsFormat(m_sFormatTypeID, TDCET_HTML);

	m_cbHtmlOptions.ShowWindow(bShowHtmlFormat ? SW_SHOW : SW_HIDE);
	m_stHtmlOptionIcon.ShowWindow(bShowHtmlFormat ? SW_SHOW : SW_HIDE);
}

void CTDLExportToPage::PreserveExportPath()
{
	// Don't save empty paths that are simply the result
	// of the current exporter not being file-based
	if (!m_sFormatTypeID.IsEmpty() && GetExporterFileExtension(m_sFormatTypeID).IsEmpty())
	{
		ASSERT(m_sExportPath.IsEmpty());
		return;
	}

	if (m_bExportAllTasklists)
	{
		ASSERT(!m_bSingleTaskList);

		if (m_bExportOneFile)
		{
			m_sMultiFilePath = m_sExportPath;
			RemoveExporterFileExtension(m_sMultiFilePath, m_sFormatTypeID);
		}
		else
		{
			m_sFolderPath = m_sExportPath;
		}
	}
	else // active tasklist only
	{
		m_sFilePath = m_sExportPath;
		RemoveExporterFileExtension(m_sFilePath, m_sFormatTypeID);
	}
}

void CTDLExportToPage::RestoreExportPath()
{
	// check exporter has a file extension
	int nFormat = m_mgrImportExport.FindExporterByType(m_sFormatTypeID);

	if (m_mgrImportExport.ExporterHasFileExtension(nFormat))
	{
		m_eExportPath.SetFilter(m_mgrImportExport.GetExporterFileFilter(nFormat));

		if (m_bExportAllTasklists && !m_bExportOneFile)
		{
			// Saving to a folder location
			m_sExportPath = m_sFolderPath;
		}
		else 
		{
			// Saving to a file location
			if (m_bExportAllTasklists && m_bExportOneFile)
			{
				ASSERT(!FileMisc::HasExtension(m_sMultiFilePath));
				m_sExportPath = m_sMultiFilePath;
			}
			else
			{
				ASSERT(!FileMisc::HasExtension(m_sFilePath));
				m_sExportPath = m_sFilePath;
			}
			ASSERT(!m_sExportPath.IsEmpty());

			EnsureExporterFileExtension(m_sExportPath, m_sFormatTypeID);
		}
	}
	else // disable path edit and remove file path
	{
		m_sExportPath.Empty();
	}

	UpdateData(FALSE);
}

CString CTDLExportToPage::GetExporterFileExtension(LPCTSTR szFormatTypeID) const
{
	int nFormat = m_mgrImportExport.FindExporterByType(szFormatTypeID);

	return m_mgrImportExport.GetExporterFileExtension(nFormat, TRUE);
}

BOOL CTDLExportToPage::RemoveExporterFileExtension(CString& sPathName, LPCTSTR szFormatTypeID) const
{
	if (sPathName.IsEmpty())
		return FALSE;

	CString sExt = GetExporterFileExtension(szFormatTypeID);
	return Misc::RemoveSuffix(sPathName, sExt, TRUE);
}

void CTDLExportToPage::EnsureExporterFileExtension(CString& sPathName, LPCTSTR szFormatTypeID, BOOL bRemovePrevExt) const
{
	if (!sPathName.IsEmpty())
	{
		CString sExt = GetExporterFileExtension(szFormatTypeID);

		if (bRemovePrevExt)
			FileMisc::ReplaceExtension(sPathName, sExt);
		else
			sPathName += sExt;
	}
}

void CTDLExportToPage::OnOK()
{
	CCmdNotifyPropertyPage::OnOK();

	// make sure extension is right
	if (!m_bExportAllTasklists || m_bExportOneFile)
		EnsureExporterFileExtension(m_sExportPath, m_sFormatTypeID);

	CPreferences prefs;

	prefs.WriteProfileString(m_sPrefsKey, _T("ExporterTypeID"), m_sFormatTypeID);
	prefs.WriteProfileInt(m_sPrefsKey, _T("ExportOneFile"), m_bExportOneFile);
	prefs.WriteProfileInt(m_sPrefsKey, _T("ExportToClipboard"), m_bExportToClipboard);
	prefs.WriteProfileInt(m_sPrefsKey, _T("HtmlStyle"), m_nHtmlStyle);

	if (!m_bSingleTaskList)
	{
		PreserveExportPath(); // Update m_sFolderPath/m_sMultiFilePath

		prefs.WriteProfileInt(m_sPrefsKey, _T("ExportOption"), m_bExportAllTasklists);
		prefs.WriteProfileString(m_sPrefsKey, _T("LastFolder"), m_sFolderPath);

		// We no longer persist the multi-path extension
		ASSERT(!FileMisc::HasExtension(m_sMultiFilePath));
		prefs.WriteProfileString(m_sPrefsKey, _T("LastMultiFilePath"), m_sMultiFilePath);
	}
}

BOOL CTDLExportToPage::WantSaveToFolder() const
{
	return (!m_bSingleTaskList && m_bExportAllTasklists && !m_bExportOneFile && !m_bExportToClipboard);
}

void CTDLExportToPage::OnExportToClipboardOrPath()
{
	UpdateData();
	RefreshCtrlStates();
}

void CTDLExportToPage::OnChangeExportpath() 
{
	UpdateData();
	PreserveExportPath();
}
