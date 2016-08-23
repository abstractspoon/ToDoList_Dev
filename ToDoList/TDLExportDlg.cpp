// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLExportDlg.h"

#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\preferences.h"
#include "..\shared\misc.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog

enum { ACTIVETASKLIST, ALLTASKLISTS };

CTDLExportDlg::CTDLExportDlg(const CImportExportMgr& mgr, BOOL bSingleTaskList, FTC_VIEW nView, 
					   BOOL bVisibleColumnsOnly, LPCTSTR szFilePath, LPCTSTR szFolderPath, CWnd* pParent /*=NULL*/)
	: CTDLDialog(IDD_EXPORT_DIALOG, pParent), 
	  m_mgrImportExport(mgr),
	  m_bSingleTaskList(bSingleTaskList), 
	  m_sFilePath(szFilePath), m_sOrgFilePath(szFilePath),
	  m_sFolderPath(szFolderPath), m_sOrgFolderPath(szFolderPath),
	  m_taskSel(_T("Exporting"), nView, bVisibleColumnsOnly),
	  m_eExportPath(FES_COMBOSTYLEBTN | FES_SAVEAS),
	  m_nFormatOption(0),
	  m_cbFormat(mgr, FALSE)
{
	//{{AFX_DATA_INIT(CExportDlg)
	//}}AFX_DATA_INIT

	CPreferences prefs;

	// retrieve previous user input
	m_bExportOneFile = prefs.GetProfileInt(_T("Exporting"), _T("ExportOneFile"), FALSE);
	m_sMultiFilePath = prefs.GetProfileString(_T("Exporting"), _T("LastMultiFilePath"));

	m_nExportOption = m_bSingleTaskList ? ACTIVETASKLIST : prefs.GetProfileInt(_T("Exporting"), _T("ExportOption"), ACTIVETASKLIST);
	
	m_nFormatOption = prefs.GetProfileInt(_T("Exporting"), _T("FormatOption"), 0);
	m_nFormatOption = min(m_nFormatOption, mgr.GetNumExporters());

	if (m_sFolderPath.IsEmpty())
	{
		m_sFolderPath = prefs.GetProfileString(_T("Exporting"), _T("LastFolder"));

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
	ReplaceExtension(m_sMultiFilePath, m_nFormatOption);

	if ((m_sFilePath.IsEmpty() || PathIsRelative(m_sFilePath)) && !m_sFolderPath.IsEmpty())
	{
		CEnString sFName;
		FileMisc::SplitPath(m_sFilePath, NULL, NULL, &sFName);
		
		// handle empty filename
		if (sFName.IsEmpty())
			sFName.LoadString(IDS_TDC_UNTITLEDFILE);
		
		FileMisc::MakePath(m_sFilePath, NULL, m_sFolderPath, sFName);
	}
	ReplaceExtension(m_sFilePath, m_nFormatOption);

	// prepare initial export path
	if (m_bSingleTaskList || m_nExportOption == ACTIVETASKLIST) 
	{
		m_sExportPath = m_sFilePath; // default
		m_sPathLabel.LoadString(IDS_ED_FILEPATH);
	}
	else if (m_nExportOption == ALLTASKLISTS && m_bExportOneFile)
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


void CTDLExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDlg)
	DDX_Control(pDX, IDC_FORMATOPTIONS, m_cbFormat);
	DDX_Control(pDX, IDC_EXPORTPATH, m_eExportPath);
	DDX_CBIndex(pDX, IDC_TASKLISTOPTIONS, m_nExportOption);
	DDX_Text(pDX, IDC_EXPORTPATH, m_sExportPath);
	DDX_Check(pDX, IDC_EXPORTONEFILE, m_bExportOneFile);
	DDX_Text(pDX, IDC_PATHLABEL, m_sPathLabel);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nIndex = m_cbFormat.GetCurSel();
		m_nFormatOption = m_cbFormat.GetItemData(nIndex);
	}
	else
		CDialogHelper::SelectItemByData(m_cbFormat, m_nFormatOption);
}


BEGIN_MESSAGE_MAP(CTDLExportDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CExportDlg)
	ON_CBN_SELCHANGE(IDC_TASKLISTOPTIONS, OnSelchangeTasklistoptions)
	ON_CBN_SELCHANGE(IDC_FORMATOPTIONS, OnSelchangeFormatoptions)
	ON_BN_CLICKED(IDC_EXPORTONEFILE, OnExportonefile)
	ON_EN_CHANGE(IDC_EXPORTPATH, OnChangeExportpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDlg message handlers

BOOL CTDLExportDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

    VERIFY(m_taskSel.Create(IDC_FRAME, this));
	
	// set initial control states
	GetDlgItem(IDC_TASKLISTOPTIONS)->EnableWindow(!m_bSingleTaskList);
	GetDlgItem(IDC_EXPORTONEFILE)->EnableWindow(!m_bSingleTaskList && m_nExportOption == ALLTASKLISTS);

	m_eExportPath.SetFilter(m_mgrImportExport.GetExporterFileFilter(m_nFormatOption));
	m_eExportPath.EnableStyle(FES_FOLDERS, (m_nExportOption == ALLTASKLISTS && !m_bExportOneFile));
	m_eExportPath.EnableWindow(m_mgrImportExport.ExporterHasFileExtension(m_nFormatOption));

	EnableOK();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLExportDlg::OnSelchangeTasklistoptions() 
{
	// save previous export option
	int nExportOption = m_nExportOption;

	UpdateData();

	// save off current export path depending
	// on our previous option
	if (nExportOption == ALLTASKLISTS)
	{
		if (m_bExportOneFile)
			m_sMultiFilePath = m_sExportPath;
		else
			m_sFolderPath = m_sExportPath;
	}
	else // single tasklist
		m_sFilePath = m_sExportPath;

	BOOL bFolder = (m_nExportOption == ALLTASKLISTS && !m_bExportOneFile);
	
	m_eExportPath.EnableStyle(FES_FOLDERS, bFolder);
	m_sPathLabel.LoadString(bFolder ? IDS_ED_FOLDER : IDS_ED_FILEPATH);
	
	if (m_nExportOption == ALLTASKLISTS)
	{
		if (m_bExportOneFile)
		{
			m_sExportPath = m_sMultiFilePath;
			ReplaceExtension(m_sExportPath, m_nFormatOption);
		}
		else
			m_sExportPath = m_sFolderPath;
	}
	else
	{
		m_sExportPath = m_sFilePath;
		ReplaceExtension(m_sExportPath, m_nFormatOption);
	}

	GetDlgItem(IDC_EXPORTONEFILE)->EnableWindow(!m_bSingleTaskList && m_nExportOption == ALLTASKLISTS);

	UpdateData(FALSE);
}

void CTDLExportDlg::OnSelchangeFormatoptions() 
{
	UpdateData();

	// check exporter has a file extension
	if (m_mgrImportExport.ExporterHasFileExtension(m_nFormatOption))
	{
		// enable path edit
		m_eExportPath.EnableWindow(TRUE);

		// check file extension is correct
		m_eExportPath.SetFilter(m_mgrImportExport.GetExporterFileFilter(m_nFormatOption));

		if (m_nExportOption == ACTIVETASKLIST || m_bExportOneFile)
		{
			if (m_sExportPath.IsEmpty())
			{
				if (m_bExportOneFile)
					m_sExportPath = m_sMultiFilePath;
				else
					m_sExportPath = m_sOrgFilePath;
			}

			ReplaceExtension(m_sExportPath, m_nFormatOption);
			UpdateData(FALSE);
		}
		else if (m_sExportPath.IsEmpty())
			m_sExportPath = m_sOrgFolderPath;
	}
	else // disable path edit and remove file path
	{
		m_eExportPath.SetWindowText(_T(""));
		m_eExportPath.EnableWindow(FALSE);
	}
}

void CTDLExportDlg::ReplaceExtension(CString& sPathName, int nFormat)
{
	if (!m_mgrImportExport.ExporterHasFileExtension(nFormat))
		return;
	
	CString sDrive, sFolder, sFName, sExt = m_mgrImportExport.GetExporterFileExtension(nFormat);

		FileMisc::SplitPath(sPathName, &sDrive, &sFolder, &sFName);
		FileMisc::MakePath(sPathName, sDrive, sFolder, sFName, sExt);
}

void CTDLExportDlg::OnOK()
{
	BOOL bExporterHasFileExt = m_mgrImportExport.ExporterHasFileExtension(m_nFormatOption);

	if (bExporterHasFileExt)
	{
		Misc::Trim(m_sExportPath);

		// if the export path is relative we build a path based
		// on the exe path and check with the user
		if (::PathIsRelative(m_sExportPath))
		{
			CString sPath = FileMisc::GetAppFilePath(), sDrive, sFolder;

			FileMisc::SplitPath(sPath, &sDrive, &sFolder);
			FileMisc::MakePath(sPath, sDrive, sFolder, m_sExportPath);

			CString sMessage;
			
			if (m_nExportOption == ALLTASKLISTS)
				sMessage.Format(IDS_ED_CONFIRMEXPORTPATHMULTI, sPath);
			else
				sMessage.Format(IDS_ED_CONFIRMEXPORTPATH, sPath);
							
			UINT nRet = MessageBox(sMessage, CEnString(IDS_ED_CONFIRMEXPORTPATH_TITLE), MB_YESNO);

			if (nRet == IDNO)
			{
				// re-display dialog
				m_eExportPath.SetSel(0, -1);
				m_eExportPath.SetFocus();
				return;
			}
			else
				m_sExportPath = sPath;
		}

		// make sure the output folder is valid
		BOOL bBadFolder = (m_nExportOption == ALLTASKLISTS && !m_bExportOneFile) ? 
							!FileMisc::CreateFolder(m_sExportPath) : 
							!FileMisc::CreateFolderFromFilePath(m_sExportPath);

		if (bBadFolder)
		{
			CEnString sMessage(IDS_ED_NOMAKEEXPORTPATH, m_sExportPath);
			
			UINT nRet = MessageBox(sMessage, CEnString(IDS_ED_NOMAKEEXPORTPATH_TITLE), MB_OKCANCEL);

			// re-display dialog
			if (nRet == IDOK)
			{
				m_eExportPath.SetSel(0, -1);
				m_eExportPath.SetFocus();
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

	// make sure extension is right
	if (bExporterHasFileExt)
	{
		if (m_nExportOption == ACTIVETASKLIST || m_bExportOneFile)
			ReplaceExtension(m_sExportPath, m_nFormatOption);
	}

	CPreferences prefs;

	prefs.WriteProfileInt(_T("Exporting"), _T("FormatOption"), m_nFormatOption);
	prefs.WriteProfileInt(_T("Exporting"), _T("ExportOneFile"), m_bExportOneFile);

	if (!m_bSingleTaskList)
	{
		prefs.WriteProfileInt(_T("Exporting"), _T("ExportOption"), m_nExportOption);
		prefs.WriteProfileString(_T("Exporting"), _T("LastMultiFilePath"), m_sMultiFilePath);
		prefs.WriteProfileString(_T("Exporting"), _T("LastFolder"), m_sFolderPath);
	}
}

BOOL CTDLExportDlg::GetExportAllTasklists()
{
	return (!m_bSingleTaskList && m_nExportOption == ALLTASKLISTS);
}

void CTDLExportDlg::OnExportonefile() 
{
	// save previous flag state
	BOOL bExportOneFile = m_bExportOneFile;

	UpdateData();

	// save off current export path depending
	// on our previous state
	if (bExportOneFile)
		m_sMultiFilePath = m_sExportPath;
	
	else if (m_nExportOption == ALLTASKLISTS)
		m_sFolderPath = m_sExportPath;
	else
		m_sFilePath = m_sExportPath;

	// set export path
	BOOL bFolder = (m_nExportOption == ALLTASKLISTS && !m_bExportOneFile);

	m_eExportPath.EnableStyle(FES_FOLDERS, bFolder);
	m_sPathLabel.LoadString(bFolder ? IDS_ED_FOLDER : IDS_ED_FILEPATH);

	if (m_nExportOption == ALLTASKLISTS)
	{
		if (m_bExportOneFile)
			m_sExportPath = m_sMultiFilePath;
		else
			m_sExportPath = m_sFolderPath;
	}
	else
		m_sExportPath = m_sFilePath;

	if (m_nExportOption == ACTIVETASKLIST || m_bExportOneFile)
		ReplaceExtension(m_sExportPath, m_nFormatOption);

	UpdateData(FALSE);
}

void CTDLExportDlg::EnableOK()
{
	Misc::Trim(m_sExportPath);
	
	GetDlgItem(IDOK)->EnableWindow(!m_sExportPath.IsEmpty());
}

void CTDLExportDlg::OnChangeExportpath() 
{
	UpdateData();
	EnableOK();

	if (m_nExportOption == ACTIVETASKLIST || m_bExportOneFile)
		m_sFilePath = m_sExportPath;
	else
		m_sFolderPath = m_sExportPath;
}

CString CTDLExportDlg::GetExportPath()
{
	if (m_mgrImportExport.ExporterHasFileExtension(m_nFormatOption))
		return m_sExportPath;

	// else
	return _T("");
}
