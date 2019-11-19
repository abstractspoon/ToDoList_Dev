// TDLImportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcenum.h"
#include "TDLImportDialog.h"
#include "TDCImportExportMgr.h"

#include "../shared/enstring.h"
#include "../shared/misc.h"
#include "../shared/filemisc.h"
#include "../shared/graphicsmisc.h"
#include "../shared/dialoghelper.h"
#include "../shared/clipboard.h"

#include "../Interfaces/preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR CRLF = _T("\r\n");

/////////////////////////////////////////////////////////////////////////////

enum IMPORTTO 
{
	// Match order of radio buttons
	NEWTASKLIST,
	TOPOFTASKLIST,
	SELECTEDTASK,
	MERGE,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog dialog

CTDLImportDialog::CTDLImportDialog(const CTDCImportExportMgr& mgr, BOOL bReadonlyTasklist, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLImportDialog::IDD, _T("Importing"), pParent),
	m_mgrImportExport(mgr),
	m_cbFormat(mgr, TRUE),
	m_bFileOnly(FALSE),
	m_bReadonlyTasklist(bReadonlyTasklist),
	m_bTextIsClipboard(TRUE)
{
	//{{AFX_DATA_INIT(CTDLImportDialog)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bFromText = prefs.GetProfileInt(m_sPrefsKey, _T("ImportOption"), FALSE);
	m_sFromFilePath = prefs.GetProfileString(m_sPrefsKey, _T("ImportFilePath"));
	m_bMatchByTaskID = FALSE; // always

	m_sFormatTypeID = prefs.GetProfileString(m_sPrefsKey, _T("ImporterTypeID"));

	// backwards compat
	if (m_sFormatTypeID.IsEmpty())
	{
		int nFormat = prefs.GetProfileInt(m_sPrefsKey, _T("ImportFormat"), -1);

		if (nFormat != -1)
			m_sFormatTypeID = mgr.GetImporterTypeID(nFormat);

		if (m_sFormatTypeID.IsEmpty())
			m_sFormatTypeID = mgr.GetTypeID(TDCET_CSV);
	}

	if (m_bReadonlyTasklist)
		m_nImportTo = NEWTASKLIST;
	else
		m_nImportTo = prefs.GetProfileInt(m_sPrefsKey, _T("ImportToWhere"), SELECTEDTASK);
}


void CTDLImportDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLImportDialog)
	DDX_Control(pDX, IDC_INPUTFILE, m_eFilePath);
	DDX_Control(pDX, IDC_FORMATOPTIONS, m_cbFormat);
	DDX_Radio(pDX, IDC_FROMFILE, m_bFromText);
	DDX_Text(pDX, IDC_INPUTFILE, m_sFromFilePath);
	DDX_Radio(pDX, IDC_CREATETASK, m_nImportTo);
	DDX_Radio(pDX, IDC_MERGEBYTITLE, m_bMatchByTaskID);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
		m_sFormatTypeID = m_cbFormat.GetSelectedTypeID();
	else
		m_cbFormat.SetSelectedTypeID(m_sFormatTypeID);
}


BEGIN_MESSAGE_MAP(CTDLImportDialog, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLImportDialog)
	ON_BN_CLICKED(IDC_FROMTEXT, OnChangeImportFrom)
	ON_CBN_SELCHANGE(IDC_FORMATOPTIONS, OnSelchangeFormatoptions)
	ON_EN_CHANGE(IDC_INPUTTEXT, OnChangeClipboardtext)
	ON_EN_CHANGE(IDC_INPUTFILE, OnChangeFilepath)
	ON_BN_CLICKED(IDC_REFRESHCLIPBOARD, OnRefreshclipboard)
	ON_BN_CLICKED(IDC_FROMFILE, OnChangeImportFrom)
	ON_BN_CLICKED(IDC_MERGETOACTIVETASKLIST, OnChangeMergeTo)
	ON_BN_CLICKED(IDC_ADDTOACTIVETASKLIST, OnChangeMergeTo)
	ON_BN_CLICKED(IDC_ADDTOSELECTEDTASK, OnChangeMergeTo)
	ON_BN_CLICKED(IDC_CREATENEWTASKLIST, OnChangeMergeTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog message handlers

BOOL CTDLImportDialog::SetImportTo(TDLID_IMPORTTO nImportTo)
{
	switch (nImportTo)
	{
		case TDIT_CREATENEWTASKLIST:
			m_nImportTo = NEWTASKLIST;
			break;

		case TDIT_ADDTOTOPOFTASKLIST:
			m_nImportTo = TOPOFTASKLIST;
			break;

		case TDIT_ADDTOSELECTEDTASK:
			m_nImportTo = SELECTEDTASK;
			break;

		case TDIT_MERGETOTASKLISTBYID:
			m_nImportTo = MERGE;
			m_bMatchByTaskID = TRUE;
			break;

		case TDIT_MERGETOTASKLISTBYTITLE:
			m_nImportTo = MERGE;
			m_bMatchByTaskID = FALSE;
			break;

		default:
			ASSERT(0);
			return FALSE;
	}

	return TRUE;
}


BOOL CTDLImportDialog::SetFilePath(LPCTSTR szFilePath)
{
	if (Misc::IsEmpty(szFilePath))
		return FALSE;

	m_sFromFilePath = szFilePath;
	m_bFileOnly = TRUE;

	int nFormat = m_mgrImportExport.FindImporterByPath(szFilePath);

	if (nFormat == -1)
		return FALSE;

	// else
	m_sFormatTypeID = m_mgrImportExport.GetImporterTypeID(nFormat);

	m_bFromText = FALSE;
	m_sFromText.Empty();
	m_cbFormat.SetFileBasedOnly(TRUE, FileMisc::GetExtension(szFilePath));

	return TRUE;
}

void CTDLImportDialog::SetUseClipboard()
{
	m_bFileOnly = FALSE;
	m_sFromText = CClipboard().GetText();
	m_bTextIsClipboard = TRUE;
	m_bFromText = TRUE;
	m_sFromFilePath.Empty();
}

BOOL CTDLImportDialog::SetUseText(LPCTSTR szText)
{
	if (Misc::IsEmpty(szText))
		return FALSE;

	m_bFileOnly = FALSE;
	m_sFromText = szText;
	m_bTextIsClipboard = FALSE;
	m_bFromText = TRUE;
	m_sFromFilePath.Empty();

	return TRUE;
}

void CTDLImportDialog::OnChangeImportFrom() 
{
	ASSERT(!m_bFileOnly);

	UpdateData();

	BOOL bHasFilter = IsCurrentImporterFileBased();
	
	GetDlgItem(IDC_INPUTFILE)->EnableWindow(!m_bFromText && bHasFilter);
	GetDlgItem(IDC_INPUTTEXT)->EnableWindow(m_bFromText && bHasFilter);
	GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromText && bHasFilter);

	EnableOK();
}

BOOL CTDLImportDialog::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	ASSERT(!m_bFileOnly || !m_bFromText);

	BOOL bHasFilter = IsCurrentImporterFileBased();
	ASSERT(!m_bFileOnly || bHasFilter);

	m_eFilePath.SetFilter(GetCurrentImporterFilter());

	GetDlgItem(IDC_FORMATOPTIONS)->EnableWindow(TRUE);
	GetDlgItem(IDC_FORMATOPTIONS)->SetFocus();

	if (m_bFileOnly)
	{
		GetDlgItem(IDC_FROMFILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_INPUTFILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_FROMTEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_INPUTTEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(FALSE);

		GetDlgItem(IDC_CREATENEWTASKLIST)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_FROMFILE)->EnableWindow(bHasFilter);
		GetDlgItem(IDC_INPUTFILE)->EnableWindow(!m_bFromText && bHasFilter);
		GetDlgItem(IDC_FROMTEXT)->EnableWindow(bHasFilter);
		GetDlgItem(IDC_INPUTTEXT)->EnableWindow(m_bFromText && bHasFilter);

		if (m_bTextIsClipboard)
		{
			GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromText && bHasFilter);

			OnRefreshclipboard();
		}
		else
		{
			GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(FALSE);
			GetDlgItem(IDC_REFRESHCLIPBOARD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_FROMTEXT)->SetWindowText(_T("&Text:"));
		}

		// Set text font to be mono-spaced
		if (GraphicsMisc::CreateFont(m_fontMonospace, _T("Lucida Console")))
			GetDlgItem(IDC_INPUTTEXT)->SetFont(&m_fontMonospace, FALSE);
	}

	if (m_bReadonlyTasklist)
	{
		GetDlgItem(IDC_TOACTIVETASLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_TOSELECTEDTASK)->EnableWindow(FALSE);
	}
	
	GetDlgItem(IDC_MERGEBYTITLE)->EnableWindow(m_nImportTo == MERGE);
	GetDlgItem(IDC_MERGEBYTASKID)->EnableWindow(m_nImportTo == MERGE);
	GetDlgItem(IDC_MERGEBYTASKIDWARNING)->EnableWindow(m_nImportTo == MERGE);

	EnableOK();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLImportDialog::IsCurrentImporterFileBased() const
{
	int nFormat = m_mgrImportExport.FindImporterByType(m_sFormatTypeID);

	return m_mgrImportExport.ImporterHasFileExtension(nFormat);
}

CString CTDLImportDialog::GetCurrentImporterFilter() const
{
	int nFormat = m_mgrImportExport.FindImporterByType(m_sFormatTypeID);

	return m_mgrImportExport.GetImporterFileFilter(nFormat);
}

void CTDLImportDialog::OnOK()
{
	CTDLDialog::OnOK();
	
	CPreferences prefs;
	
	prefs.WriteProfileInt(m_sPrefsKey, _T("ImportOption"), m_bFromText);
	prefs.WriteProfileString(m_sPrefsKey, _T("ImportFilePath"), m_sFromFilePath);
	prefs.WriteProfileInt(m_sPrefsKey, _T("ImportToWhere"), m_nImportTo);
	prefs.WriteProfileString(m_sPrefsKey, _T("ImporterTypeID"), m_sFormatTypeID);

	// retrieve clipboard text
	if (IsCurrentImporterFileBased())
		GetDlgItem(IDC_INPUTTEXT)->GetWindowText(m_sFromText);
}

CString CTDLImportDialog::GetFormatTypeID() const
{
	return m_sFormatTypeID;
}

TDLID_IMPORTTO CTDLImportDialog::GetImportTo() const
{
	switch (m_nImportTo)
	{
	case NEWTASKLIST:	return TDIT_CREATENEWTASKLIST;
	case TOPOFTASKLIST: return TDIT_ADDTOTOPOFTASKLIST;
	case SELECTEDTASK:	return TDIT_ADDTOSELECTEDTASK;
	case MERGE:			return (m_bMatchByTaskID ? TDIT_MERGETOTASKLISTBYID : TDIT_MERGETOTASKLISTBYTITLE);
	}

	return (TDLID_IMPORTTO)-1;
}

BOOL CTDLImportDialog::GetImportFromText() const
{
	return (m_bFromText);
}

CString CTDLImportDialog::GetImportFilePath() const
{
	return (m_bFromText || !IsCurrentImporterFileBased()) ? _T("") : m_sFromFilePath;
}

CString CTDLImportDialog::GetImportText() const
{
	return (m_bFromText && IsCurrentImporterFileBased()) ? m_sFromText : _T("");
}

void CTDLImportDialog::OnSelchangeFormatoptions() 
{
	int nFormat = m_mgrImportExport.FindImporterByType(m_sFormatTypeID);
	BOOL bHadFilter = m_mgrImportExport.ImporterHasFileExtension(nFormat);

	UpdateData(TRUE);
	
	// change the filter on the CFileEdit and clear the filepath
	// and clear/restore clipboard text depending
	BOOL bHasFilter = IsCurrentImporterFileBased();

	m_eFilePath.SetFilter(GetCurrentImporterFilter());
	m_eFilePath.EnableWindow(bHasFilter);
	
	GetDlgItem(IDC_FROMFILE)->EnableWindow(bHasFilter);
	GetDlgItem(IDC_INPUTFILE)->EnableWindow(!m_bFromText && bHasFilter);
	GetDlgItem(IDC_FROMTEXT)->EnableWindow(bHasFilter);
	GetDlgItem(IDC_INPUTTEXT)->EnableWindow(m_bFromText && bHasFilter);
	GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromText && bHasFilter);

	if (bHadFilter && !bHasFilter)
	{
		GetDlgItem(IDC_INPUTTEXT)->GetWindowText(m_sFromText); // update
		GetDlgItem(IDC_INPUTTEXT)->SetWindowText(_T("")); // clear field
	}
	else if (!bHadFilter && bHasFilter)
	{
		GetDlgItem(IDC_INPUTTEXT)->SetWindowText(m_sFromText); // restore field
	}
	
	m_sFromFilePath.Empty();
	UpdateData(FALSE);

	EnableOK();
}

void CTDLImportDialog::EnableOK()
{
	if (!IsCurrentImporterFileBased())
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else if (GetImportFromText())
	{
		GetDlgItem(IDOK)->EnableWindow(!m_sFromText.IsEmpty());
	}
	else // import from file
	{
		Misc::Trim(m_sFromFilePath);

		GetDlgItem(IDOK)->EnableWindow(FileMisc::FileExists(m_sFromFilePath));
	}
}

void CTDLImportDialog::OnChangeClipboardtext() 
{
	GetDlgItem(IDC_INPUTTEXT)->GetWindowText(m_sFromText); // update
	EnableOK();
}

void CTDLImportDialog::OnChangeFilepath() 
{
	UpdateData();
	EnableOK();
}

void CTDLImportDialog::OnRefreshclipboard() 
{
	if (!m_bFileOnly)
	{
		m_sFromText = CClipboard().GetText();

		// Edit control wants CRLF not just LF
		if (m_sFromText.Find(CRLF) == -1)
			m_sFromText.Replace(_T("\n"), CRLF);

		// Add blank line at start in case the chosen importer
		// treats the first line as a header line
		m_sFromText = (CRLF + m_sFromText);

		if (IsCurrentImporterFileBased())
			GetDlgItem(IDC_INPUTTEXT)->SetWindowText(m_sFromText);
	}
}

void CTDLImportDialog::OnRepositionControls(int dx, int dy)
{
	CTDLDialog::OnRepositionControls(dx, dx);
	
	CDialogHelper::ResizeCtrl(this, IDC_FROMBORDER, dx, dy);
	CDialogHelper::ResizeCtrl(this, IDC_INPUTTEXT, dx, dy);
	CDialogHelper::ResizeCtrl(this, IDC_INPUTFILE, dx, 0);
	CDialogHelper::OffsetCtrl(this, IDC_REFRESHCLIPBOARD, dx, dy);

	CDialogHelper::OffsetCtrl(this, IDC_TOBORDER, dx, 0);
	CDialogHelper::ResizeCtrl(this, IDC_TOBORDER, 0, dy);
	CDialogHelper::OffsetCtrl(this, IDC_ADDTOACTIVETASKLIST, dx, 0);
	CDialogHelper::OffsetCtrl(this, IDC_ADDTOSELECTEDTASK, dx, 0);
	CDialogHelper::OffsetCtrl(this, IDC_CREATENEWTASKLIST, dx, 0);

	CDialogHelper::OffsetCtrl(this, IDC_TODIVIDER, dx, 0);
	CDialogHelper::OffsetCtrl(this, IDC_MERGETOACTIVETASKLIST, dx, 0);
	CDialogHelper::OffsetCtrl(this, IDC_MERGEBYTITLE, dx, 0);
	CDialogHelper::OffsetCtrl(this, IDC_MERGEBYTASKID, dx, 0);
	CDialogHelper::OffsetCtrl(this, IDC_MERGEBYTASKIDWARNING, dx, 0);

	CDialogHelper::OffsetCtrl(this, IDOK, dx, dy);
	CDialogHelper::OffsetCtrl(this, IDCANCEL, dx, dy);
}

void CTDLImportDialog::OnChangeMergeTo() 
{
	UpdateData();

	GetDlgItem(IDC_MERGEBYTITLE)->EnableWindow(m_nImportTo == MERGE);
	GetDlgItem(IDC_MERGEBYTASKID)->EnableWindow(m_nImportTo == MERGE);
	GetDlgItem(IDC_MERGEBYTASKIDWARNING)->EnableWindow(m_nImportTo == MERGE);
}
