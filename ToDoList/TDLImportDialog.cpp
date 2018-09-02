// TDLImportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcenum.h"
#include "TDLImportDialog.h"

#include "../shared/enstring.h"
#include "../shared/misc.h"
#include "../shared/filemisc.h"
#include "../shared/graphicsmisc.h"
#include "../shared/preferences.h"
#include "../shared/dialoghelper.h"
#include "../shared/clipboard.h"

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

CTDLImportDialog::CTDLImportDialog(const CImportExportMgr& mgr, BOOL bReadonlyTasklist, CWnd* pParent /*=NULL*/)
	: CTDLDialog(CTDLImportDialog::IDD, _T("Importing"), pParent),
	  m_mgrImportExport(mgr),
	  m_cbFormat(mgr, TRUE),
	  m_bFileOnly(FALSE),
	  m_bReadonlyTasklist(bReadonlyTasklist)
{
	//{{AFX_DATA_INIT(CTDLImportDialog)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bFromClipboard = prefs.GetProfileInt(m_sPrefsKey, _T("ImportOption"), FALSE);
	m_sFromFilePath = prefs.GetProfileString(m_sPrefsKey, _T("ImportFilePath"));
	m_bMatchByTaskID = FALSE; // always

	m_nFormatOption = prefs.GetProfileInt(m_sPrefsKey, _T("ImportFormat"), 0);
	m_nFormatOption = min(m_nFormatOption, mgr.GetNumImporters());

	if (m_bReadonlyTasklist)
		m_nImportTo = NEWTASKLIST;
	else
		m_nImportTo = prefs.GetProfileInt(m_sPrefsKey, _T("ImportToWhere"), SELECTEDTASK);
}


void CTDLImportDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLImportDialog)
	DDX_Control(pDX, IDC_FROMFILEPATH, m_eFilePath);
	DDX_Control(pDX, IDC_FORMATOPTIONS, m_cbFormat);
	DDX_Radio(pDX, IDC_FROMFILE, m_bFromClipboard);
	DDX_Text(pDX, IDC_FROMFILEPATH, m_sFromFilePath);
	DDX_Radio(pDX, IDC_CREATETASK, m_nImportTo);
	DDX_Radio(pDX, IDC_MERGEBYTITLE, m_bMatchByTaskID);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nIndex = m_cbFormat.GetCurSel();
		m_nFormatOption = m_cbFormat.GetItemData(nIndex);
	}
	else
	{
		SelectItemByData(m_cbFormat, m_nFormatOption);
	}
}


BEGIN_MESSAGE_MAP(CTDLImportDialog, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLImportDialog)
	ON_BN_CLICKED(IDC_FROMCLIPBOARD, OnChangeImportFrom)
	ON_CBN_SELCHANGE(IDC_FORMATOPTIONS, OnSelchangeFormatoptions)
	ON_EN_CHANGE(IDC_FROMCLIPBOARDTEXT, OnChangeClipboardtext)
	ON_EN_CHANGE(IDC_FROMFILEPATH, OnChangeFilepath)
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

int CTDLImportDialog::DoModal(LPCTSTR szFilePath)
{
	if (!Misc::IsEmpty(szFilePath))
	{
		m_sFromFilePath = szFilePath;
		m_bFileOnly = TRUE;

		m_nFormatOption = m_mgrImportExport.FindImporter(szFilePath);

		if (m_nFormatOption == -1)
			return IDCANCEL;

		m_bFromClipboard = FALSE;
		m_sClipboardText.Empty();
		m_cbFormat.SetFileBasedOnly(TRUE);
	}
	else
	{
		m_bFileOnly = FALSE;
	}

	return CTDLDialog::DoModal();
}

void CTDLImportDialog::OnChangeImportFrom() 
{
	ASSERT(!m_bFileOnly);

	UpdateData();

	BOOL bHasFilter = CurImporterHasFilter();
	
	GetDlgItem(IDC_FROMFILEPATH)->EnableWindow(!m_bFromClipboard && bHasFilter);
	GetDlgItem(IDC_FROMCLIPBOARDTEXT)->EnableWindow(m_bFromClipboard && bHasFilter);
	GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromClipboard && bHasFilter);

	EnableOK();
}

BOOL CTDLImportDialog::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	ASSERT(!m_bFileOnly || !m_bFromClipboard);

	BOOL bHasFilter = CurImporterHasFilter();
	ASSERT(!m_bFileOnly || bHasFilter);

	m_eFilePath.SetFilter(GetCurImporterFilter());

	if (m_bFileOnly)
	{
		GetDlgItem(IDC_FORMATOPTIONS)->EnableWindow(FALSE);
		GetDlgItem(IDC_FROMFILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_FROMFILEPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_FROMCLIPBOARD)->EnableWindow(FALSE);
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(FALSE);

		GetDlgItem(IDC_CREATENEWTASKLIST)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_FORMATOPTIONS)->EnableWindow(TRUE);
		GetDlgItem(IDC_FROMFILE)->EnableWindow(bHasFilter);
		GetDlgItem(IDC_FROMFILEPATH)->EnableWindow(!m_bFromClipboard && bHasFilter);
		GetDlgItem(IDC_FROMCLIPBOARD)->EnableWindow(bHasFilter);
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->EnableWindow(m_bFromClipboard && bHasFilter);
		GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromClipboard && bHasFilter);

		// Set clipboard text font to be mono-spaced
		if (GraphicsMisc::CreateFont(m_fontMonospace, _T("Lucida Console")))
			GetDlgItem(IDC_FROMCLIPBOARDTEXT)->SetFont(&m_fontMonospace, FALSE);
	
		OnRefreshclipboard();

		GetDlgItem(IDC_FORMATOPTIONS)->SetFocus();
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

BOOL CTDLImportDialog::CurImporterHasFilter() const
{
	return m_mgrImportExport.ImporterHasFileExtension(m_nFormatOption);
}

CString CTDLImportDialog::GetCurImporterFilter() const
{
	return m_mgrImportExport.GetImporterFileFilter(m_nFormatOption);
}

void CTDLImportDialog::OnOK()
{
	CTDLDialog::OnOK();
	
	CPreferences prefs;
	
	prefs.WriteProfileInt(m_sPrefsKey, _T("ImportOption"), m_bFromClipboard);
	prefs.WriteProfileString(m_sPrefsKey, _T("ImportFilePath"), m_sFromFilePath);
	prefs.WriteProfileInt(m_sPrefsKey, _T("ImportToWhere"), m_nImportTo);
	prefs.WriteProfileInt(m_sPrefsKey, _T("ImportFormat"), m_nFormatOption);

	// retrieve clipboard text
	if (CurImporterHasFilter())
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->GetWindowText(m_sClipboardText);
}

int CTDLImportDialog::GetImporterIndex() const
{
	return m_nFormatOption;
}

TDLID_IMPORTTO CTDLImportDialog::GetImportTo() const
{
	switch (m_nImportTo)
	{
	case 0: return TDIT_CREATENEWTASKLIST;
	case 1: return TDIT_ADDTOTOPOFTASKLIST;
	case 2: return TDIT_ADDTOSELECTEDTASK;
	case 3: return (m_bMatchByTaskID ? TDIT_MERGETOTASKLISTBYID : TDIT_MERGETOTASKLISTBYTITLE);
	}

	return (TDLID_IMPORTTO)-1;
}

BOOL CTDLImportDialog::GetImportFromClipboard() const
{
	return (m_bFromClipboard);
}

CString CTDLImportDialog::GetImportFilePath() const
{
	return (m_bFromClipboard || !CurImporterHasFilter()) ? _T("") : m_sFromFilePath;
}

CString CTDLImportDialog::GetImportClipboardText() const
{
	return (m_bFromClipboard && CurImporterHasFilter()) ? m_sClipboardText : _T("");
}

void CTDLImportDialog::OnSelchangeFormatoptions() 
{
	BOOL bHadFilter = m_mgrImportExport.ImporterHasFileExtension(m_nFormatOption);

	UpdateData(TRUE);
	
	// change the filter on the CFileEdit and clear the filepath
	// and clear/restore clipboard text depending
	BOOL bHasFilter = CurImporterHasFilter();

	m_eFilePath.SetFilter(GetCurImporterFilter());
	m_eFilePath.EnableWindow(bHasFilter);
	
	GetDlgItem(IDC_FROMFILE)->EnableWindow(bHasFilter);
	GetDlgItem(IDC_FROMFILEPATH)->EnableWindow(!m_bFromClipboard && bHasFilter);
	GetDlgItem(IDC_FROMCLIPBOARD)->EnableWindow(bHasFilter);
	GetDlgItem(IDC_FROMCLIPBOARDTEXT)->EnableWindow(m_bFromClipboard && bHasFilter);
	GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromClipboard && bHasFilter);

	if (bHadFilter && !bHasFilter)
	{
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->GetWindowText(m_sClipboardText); // update
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->SetWindowText(_T("")); // clear field
	}
	else if (!bHadFilter && bHasFilter)
	{
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->SetWindowText(m_sClipboardText); // restore field
	}
	
	m_sFromFilePath.Empty();
	UpdateData(FALSE);

	EnableOK();
}

void CTDLImportDialog::EnableOK()
{
	if (!CurImporterHasFilter())
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else if (GetImportFromClipboard())
	{
		GetDlgItem(IDOK)->EnableWindow(!m_sClipboardText.IsEmpty());
	}
	else // import from file
	{
		Misc::Trim(m_sFromFilePath);

		GetDlgItem(IDOK)->EnableWindow(FileMisc::FileExists(m_sFromFilePath));
	}
}

void CTDLImportDialog::OnChangeClipboardtext() 
{
	GetDlgItem(IDC_FROMCLIPBOARDTEXT)->GetWindowText(m_sClipboardText); // update
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
		m_sClipboardText = CClipboard().GetText();

		// Edit control wants CRLF not just LF
		if (m_sClipboardText.Find(CRLF) == -1)
			m_sClipboardText.Replace(_T("\n"), CRLF);

		// Add blank line at start in case the chosen importer
		// treats the first line as a header line
		m_sClipboardText = (CRLF + m_sClipboardText);

		if (CurImporterHasFilter())
			GetDlgItem(IDC_FROMCLIPBOARDTEXT)->SetWindowText(m_sClipboardText);
	}
}

void CTDLImportDialog::OnRepositionControls(int dx, int dy)
{
	CTDLDialog::OnRepositionControls(dx, dx);
	
	CDialogHelper::ResizeCtrl(this, IDC_FROMBORDER, dx, dy);
	CDialogHelper::ResizeCtrl(this, IDC_FROMCLIPBOARDTEXT, dx, dy);
	CDialogHelper::ResizeCtrl(this, IDC_FROMFILEPATH, dx, 0);
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
