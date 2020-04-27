// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLExportDlg.h"

#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\misc.h"

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

CTDLExportDlg::CTDLExportDlg(LPCTSTR szTitle,
							 const CTDCImportExportMgr& mgr,
							 BOOL bSingleTaskList,
							 FTC_VIEW nView,
							 BOOL bVisibleColumnsOnly,
							 LPCTSTR szFilePath,
							 LPCTSTR szFolderPath,
							 const CTDCCustomAttribDefinitionArray& aAttribDefs,
							 CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_EXPORT_DIALOG, _T("Exporting"), pParent), 
	m_pageTo(mgr, bSingleTaskList, szFilePath, szFolderPath, _T("Exporting")),
	m_pageTaskSel(aAttribDefs, _T("Exporting"), nView, bVisibleColumnsOnly),
	m_mgrImportExport(mgr),
	m_sExportTitle(szTitle),
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
	BOOL bWantMultiFileTitle = (m_pageTo.GetExportAllTasklists() &&
								!m_pageTo.GetExportOneFile());

	static CEnString MULTI_FILE_TITLE(IDS_EXPORTTITLE_MULTIPLEFILES);

	if (bWantMultiFileTitle && (m_sExportTitle != MULTI_FILE_TITLE))
	{
		m_sSingleFileTitle = m_sExportTitle;
		m_sExportTitle = MULTI_FILE_TITLE;
	}
	else if (!bWantMultiFileTitle && !m_sSingleFileTitle.IsEmpty())
	{
		m_sExportTitle = m_sSingleFileTitle;
	}

	GetDlgItemText(IDC_EXPORTTITLE, m_sExportTitle);
	GetDlgItem(IDC_EXPORTTITLE)->EnableWindow(GetExportOneFile());
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
			else
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

			// re-display dialog
			if (nRet == IDOK)
			{
				m_ppHost.SetActivePage(0);
				return;
			}
			else
			{
				EndDialog(IDCANCEL);
				return;
			}
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
													   LPCTSTR szRegKey, FTC_VIEW nView, BOOL bVisibleColumnsOnly)
	:
	CCmdNotifyPropertyPage(CTDLExportTaskSelectionPage::IDD),
	m_dlgTaskSel(aAttribDefs, szRegKey, nView, bVisibleColumnsOnly)
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
	m_sFilePath(szFilePath), m_sOrgFilePath(szFilePath),
	m_sFolderPath(szFolderPath), m_sOrgFolderPath(szFolderPath),
	m_sPrefsKey(szPrefsKey),
	m_eExportPath(FES_COMBOSTYLEBTN | FES_SAVEAS | FES_NOPROMPTOVERWRITE), // parent handles prompting
	m_cbFormat(mgr, FALSE)
{
	//{{AFX_DATA_INIT(CExportDlg)
	//}}AFX_DATA_INIT

	// retrieve previous user input
	CPreferences prefs;

	m_bExportOneFile = prefs.GetProfileInt(m_sPrefsKey, _T("ExportOneFile"), FALSE);
	m_bExportToClipboard = prefs.GetProfileInt(m_sPrefsKey, _T("ExportToClipboard"), FALSE);

	m_sFormatTypeID = prefs.GetProfileString(m_sPrefsKey, _T("ExporterTypeID"));
	m_sMultiFilePath = prefs.GetProfileString(m_sPrefsKey, _T("LastMultiFilePath"));

	if (m_bSingleTaskList)
		m_bExportAllTasklists = FALSE;
	else
		m_bExportAllTasklists = prefs.GetProfileInt(m_sPrefsKey, _T("ExportOption"), FALSE);

	// backwards compat
	if (m_sFormatTypeID.IsEmpty())
	{
		int nFormat = prefs.GetProfileInt(m_sPrefsKey, _T("FormatOption"), -1);

		if (nFormat != -1)
			m_sFormatTypeID = mgr.GetExporterTypeID(nFormat);
		else
			m_sFormatTypeID = mgr.GetTypeID(TDCET_CSV);
	}

	if (m_sFolderPath.IsEmpty())
	{
		m_sFolderPath = prefs.GetProfileString(m_sPrefsKey, _T("LastFolder"));

		if (m_sFolderPath.IsEmpty())
			m_sFolderPath = FileMisc::GetFolderFromFilePath(szFilePath);
	}

	// handle empty filepaths
	if (m_sMultiFilePath.IsEmpty())
	{
		CString sDrive, sFolder;
		FileMisc::SplitPath(m_sFilePath, &sDrive, &sFolder);

		FileMisc::MakePath(m_sMultiFilePath, sDrive, sFolder, CEnString(IDS_TDC_MULTIFILE));
	}
	ReplaceExtension(m_sMultiFilePath, m_sFormatTypeID);

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
	ReplaceExtension(m_sFilePath, m_sFormatTypeID);

	// prepare initial export path
	if (m_bSingleTaskList || !m_bExportAllTasklists) 
	{
		m_sExportPath = m_sFilePath; // default
		m_sPathLabel.LoadString(IDS_ED_FILEPATH);
	}
	else if (m_bExportAllTasklists && m_bExportOneFile)
	{
		m_sExportPath = m_sMultiFilePath; // default
		m_sPathLabel.LoadString(IDS_ED_FILEPATH);
	}
	else // multiple files
	{
		m_sExportPath = m_sFolderPath;
		m_sPathLabel.LoadString(IDS_ED_FOLDER);
	}
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

	if (pDX->m_bSaveAndValidate)
	{
		m_sFormatTypeID = m_cbFormat.GetSelectedTypeID();
	}
	else
	{
		m_cbFormat.SetSelectedTypeID(m_sFormatTypeID);
	}
}


BEGIN_MESSAGE_MAP(CTDLExportToPage, CCmdNotifyPropertyPage)
	//{{AFX_MSG_MAP(CExportDlg)
	ON_CBN_SELCHANGE(IDC_FORMATOPTIONS, OnSelchangeFormatoptions)
	ON_CBN_SELCHANGE(IDC_TASKLISTOPTIONS, OnSelchangeTasklistoptions)
	ON_BN_CLICKED(IDC_EXPORTONEFILE, OnExportonefile)
	ON_EN_CHANGE(IDC_EXPORTPATH, OnChangeExportpath)
	ON_BN_CLICKED(IDC_TOPATH, OnExportToClipboardOrPath)
	ON_BN_CLICKED(IDC_TOCLIPBOARD, OnExportToClipboardOrPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDlg message handlers

BOOL CTDLExportToPage::OnInitDialog() 
{
	CCmdNotifyPropertyPage::OnInitDialog();

	int nFormat = m_mgrImportExport.FindExporterByType(m_sFormatTypeID);

	m_eExportPath.SetDefaultSaveAsFileExtension(m_mgrImportExport.GetExporterFileExtension(nFormat, FALSE));
	m_eExportPath.SetFilter(m_mgrImportExport.GetExporterFileFilter(nFormat));
	m_eExportPath.EnableStyle(FES_FOLDERS, (m_bExportAllTasklists && !m_bExportOneFile));

	// set initial control states
	GetDlgItem(IDC_TASKLISTOPTIONS)->EnableWindow(!m_bSingleTaskList);
	GetDlgItem(IDC_EXPORTONEFILE)->EnableWindow(!m_bSingleTaskList && m_bExportAllTasklists && !m_bExportToClipboard);
	GetDlgItem(IDC_EXPORTPATH)->EnableWindow(m_mgrImportExport.ExporterHasFileExtension(nFormat) && !m_bExportToClipboard);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLExportToPage::OnSelchangeTasklistoptions() 
{
	// save previous export option
	BOOL bPrevExportAll = m_bExportAllTasklists;

	UpdateData();

	// save off current export path depending on our previous option
	if (bPrevExportAll)
	{
		if (m_bExportOneFile)
			m_sMultiFilePath = m_sExportPath;
		else
			m_sFolderPath = m_sExportPath;
	}
	else // single tasklist
	{
		m_sFilePath = m_sExportPath;
	}

	BOOL bFolder = (m_bExportAllTasklists && !m_bExportOneFile && !m_bExportToClipboard);
	
	m_eExportPath.EnableStyle(FES_FOLDERS, bFolder);
	m_sPathLabel.LoadString(bFolder ? IDS_ED_FOLDER : IDS_ED_FILEPATH);
	
	if (m_bExportAllTasklists)
	{
		if (m_bExportOneFile)
		{
			m_sExportPath = m_sMultiFilePath;
			ReplaceExtension(m_sExportPath, m_sFormatTypeID);
		}
		else
			m_sExportPath = m_sFolderPath;
	}
	else
	{
		m_sExportPath = m_sFilePath;
		ReplaceExtension(m_sExportPath, m_sFormatTypeID);
	}

	GetDlgItem(IDC_EXPORTONEFILE)->EnableWindow(!m_bSingleTaskList && m_bExportAllTasklists && !m_bExportToClipboard);
	UpdateData(FALSE);
}

void CTDLExportToPage::OnSelchangeFormatoptions() 
{
	UpdateData();

	// check exporter has a file extension
	int nFormat = m_mgrImportExport.FindExporterByType(m_sFormatTypeID);

	if (m_mgrImportExport.ExporterHasFileExtension(nFormat))
	{
		// enable path edit
		m_eExportPath.EnableWindow(TRUE);

		// check file extension is correct
		m_eExportPath.SetFilter(m_mgrImportExport.GetExporterFileFilter(nFormat));

		if (!m_bExportAllTasklists || m_bExportOneFile)
		{
			if (m_sExportPath.IsEmpty())
			{
				if (m_bExportOneFile)
					m_sExportPath = m_sMultiFilePath;
				else
					m_sExportPath = m_sOrgFilePath;
			}

			ReplaceExtension(m_sExportPath, m_sFormatTypeID);
			UpdateData(FALSE);
		}
		else if (m_sExportPath.IsEmpty())
		{
			m_sExportPath = m_sOrgFolderPath;
		}
	}
	else // disable path edit and remove file path
	{
		m_eExportPath.SetWindowText(_T(""));
		m_eExportPath.EnableWindow(FALSE);
	}
}

void CTDLExportToPage::ReplaceExtension(CString& sPathName, LPCTSTR szFormatTypeID)
{
	if (!sPathName.IsEmpty())
	{
		int nFormat = m_mgrImportExport.FindExporterByType(szFormatTypeID);

		if (!m_mgrImportExport.ExporterHasFileExtension(nFormat))
			return;
	
		CString sExt = m_mgrImportExport.GetExporterFileExtension(nFormat, TRUE);

		FileMisc::ReplaceExtension(sPathName, sExt);
	}
}

void CTDLExportToPage::OnOK()
{
	CCmdNotifyPropertyPage::OnOK();

	// make sure extension is right
	if (!m_bExportAllTasklists || m_bExportOneFile)
		ReplaceExtension(m_sExportPath, m_sFormatTypeID);

	CPreferences prefs;

	prefs.WriteProfileString(m_sPrefsKey, _T("ExporterTypeID"), m_sFormatTypeID);
	prefs.WriteProfileInt(m_sPrefsKey, _T("ExportOneFile"), m_bExportOneFile);
	prefs.WriteProfileInt(m_sPrefsKey, _T("ExportToClipboard"), m_bExportToClipboard);

	if (!m_bSingleTaskList)
	{
		prefs.WriteProfileInt(m_sPrefsKey, _T("ExportOption"), m_bExportAllTasklists);
		prefs.WriteProfileString(m_sPrefsKey, _T("LastMultiFilePath"), m_sMultiFilePath);
		prefs.WriteProfileString(m_sPrefsKey, _T("LastFolder"), m_sFolderPath);
	}
}

void CTDLExportToPage::OnExportonefile()
{
	// save previous flag state
	BOOL bPrevExportOneFile = m_bExportOneFile;

	UpdateData();

	// save off current export path depending
	// on our previous state
	if (bPrevExportOneFile)
	{
		m_sMultiFilePath = m_sExportPath;
	}
	else if (m_bExportAllTasklists)
	{
		m_sFolderPath = m_sExportPath;
	}
	else
	{
		m_sFilePath = m_sExportPath;
	}

	// set export path
	BOOL bFolder = (m_bExportAllTasklists && !m_bExportOneFile && !m_bExportToClipboard);

	m_eExportPath.EnableStyle(FES_FOLDERS, bFolder);
	m_sPathLabel.LoadString(bFolder ? IDS_ED_FOLDER : IDS_ED_FILEPATH);

	if (m_bExportAllTasklists)
		m_sExportPath = (m_bExportOneFile ? m_sMultiFilePath : m_sFolderPath);
	else
		m_sExportPath = m_sFilePath;

	if (!m_bExportAllTasklists || m_bExportOneFile)
		ReplaceExtension(m_sExportPath, m_sFormatTypeID);

	UpdateData(FALSE);
}

void CTDLExportToPage::OnExportToClipboardOrPath()
{
	UpdateData();

	int nFormat = m_mgrImportExport.FindExporterByType(m_sFormatTypeID);

	GetDlgItem(IDC_EXPORTPATH)->EnableWindow(m_mgrImportExport.ExporterHasFileExtension(nFormat) && !m_bExportToClipboard);
	GetDlgItem(IDC_EXPORTONEFILE)->EnableWindow(!m_bSingleTaskList && m_bExportAllTasklists && !m_bExportToClipboard);
}

void CTDLExportToPage::OnChangeExportpath() 
{
	UpdateData();

	if (!m_bExportAllTasklists || m_bExportOneFile)
		m_sFilePath = m_sExportPath;
	else
		m_sFolderPath = m_sExportPath;
}

CString CTDLExportToPage::GetExportPath() const
{
	int nFormat = m_mgrImportExport.FindExporterByType(m_sFormatTypeID);

	if (m_mgrImportExport.ExporterHasFileExtension(nFormat))
		return m_sExportPath;

	// else
	return _T("");
}
