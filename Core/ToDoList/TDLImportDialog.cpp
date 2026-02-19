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
const LPCTSTR LF = _T("\n");

const LPCTSTR PREFSKEY = _T("Importing");

/////////////////////////////////////////////////////////////////////////////

enum IMPORTTO
{
	// Match order of radio buttons
	NEWTASKLIST,
	ACTIVETASKLIST,
	MERGETASKLIST,
};

enum TASKLISTPOS
{
	// Order not relevant
	TOPOFTASKLIST = 1,
	SELECTEDTASK,
	BELOWSELECTEDTASK,
	BOTTOMOFTASKLIST,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog dialog

CTDLImportDialog::CTDLImportDialog(const CTDCImportExportMgr& mgr, BOOL bReadonlyTasklist, BOOL bTasklistHasSelection, CWnd* pParent /*=NULL*/)
	:
	CTDLDialog(IDD_IMPORT_DIALOG, PREFSKEY, pParent),
	m_mgrImportExport(mgr),
	m_cbFormat(mgr, TRUE, FALSE),
	m_pageFrom(mgr),
	m_pageTo(bReadonlyTasklist, bTasklistHasSelection),
	m_ppHost(TCE_BOLDSELTEXT)
{
	CPreferences prefs;

	m_sFormatTypeID = prefs.GetProfileString(m_sPrefsKey, _T("ImporterTypeID"));

	// backwards compat
	if (m_sFormatTypeID.IsEmpty())
	{
		int nFormat = prefs.GetProfileInt(m_sPrefsKey, _T("ImportFormat"), -1);

		if (nFormat != -1)
			m_sFormatTypeID = mgr.GetImporterTypeID(nFormat);

		if (m_sFormatTypeID.IsEmpty())
			m_sFormatTypeID = mgr.GetTypeID(TDCIT_CSV);
	}

	m_ppHost.AddPage(&m_pageFrom, CEnString(IDS_IMPORTDLGFROMPAGE_TITLE));
	m_ppHost.AddPage(&m_pageTo, CEnString(IDS_IMPORTDLGTOPAGE_TITLE));
}

void CTDLImportDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IMPORTERLIST, m_cbFormat);

	m_cbFormat.DDX(pDX, m_sFormatTypeID);
}

BEGIN_MESSAGE_MAP(CTDLImportDialog, CTDLDialog)
	ON_CBN_SELCHANGE(IDC_IMPORTERLIST, OnSelchangeImporter)
	ON_EN_CHANGE(IDC_INPUTTEXT, EnableOK)
	ON_EN_CHANGE(IDC_INPUTFILE, EnableOK)
	ON_BN_CLICKED(IDC_REFRESHCLIPBOARD, EnableOK)
	ON_BN_CLICKED(IDC_FROMFILE, EnableOK)
	ON_BN_CLICKED(IDC_MERGETOACTIVETASKLIST, EnableOK)
	ON_BN_CLICKED(IDC_ADDTOACTIVETASKLIST, EnableOK)
	ON_BN_CLICKED(IDC_ADDTOSELECTEDTASK, EnableOK)
	ON_BN_CLICKED(IDC_CREATENEWTASKLIST, EnableOK)
END_MESSAGE_MAP()

//--------------------------------------------------------------

BOOL CTDLImportDialog::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	VERIFY(m_ppHost.Create(IDC_PLACEHOLDER, this));

	ASSERT(m_cbFormat.GetCount());
	OnSelchangeImporter();

	return TRUE;
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

	prefs.WriteProfileInt(m_sPrefsKey, _T("ImportOption"), m_pageFrom.GetImportFromText());
	prefs.WriteProfileString(m_sPrefsKey, _T("ImportFilePath"), m_pageFrom.GetImportFilePath());
	prefs.WriteProfileInt(m_sPrefsKey, _T("ImportToWhere"), m_pageTo.GetImportTo());
	prefs.WriteProfileString(m_sPrefsKey, _T("ImporterTypeID"), m_sFormatTypeID);
}

CString CTDLImportDialog::GetFormatTypeID() const
{
	return m_sFormatTypeID;
}

void CTDLImportDialog::OnSelchangeImporter()
{
	UpdateData();

	m_pageFrom.SetImporterFormatID(m_sFormatTypeID);

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
		GetDlgItem(IDOK)->EnableWindow(!m_pageFrom.GetImportText().IsEmpty());
	}
	else // import from file
	{
		GetDlgItem(IDOK)->EnableWindow(FileMisc::FileExists(m_pageFrom.GetImportFilePath()));
	}
}

void CTDLImportDialog::OnChangeClipboardtext()
{
	EnableOK();
}

void CTDLImportDialog::OnChangeFilepath()
{
	EnableOK();
}

/////////////////////////////////////////////////////////////////////////////
// CTDLImportFromPage dialog

CTDLImportFromPage::CTDLImportFromPage(const CTDCImportExportMgr& mgr, CWnd* pParent /*=NULL*/)
	: 
	CCmdNotifyPropertyPage(IDD_IMPORT_FROM_PAGE),
	m_mgrImportExport(mgr),
	m_nImportMode(TDCIM_ALL),
	m_sFromText(CClipboard().GetText())
{
	CPreferences prefs;

	m_bFromText = prefs.GetProfileInt(PREFSKEY, _T("ImportOption"), FALSE);
	m_sFromFilePath = prefs.GetProfileString(PREFSKEY, _T("ImportFilePath"));
}

void CTDLImportFromPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_INPUTFILE, m_eFilePath);
	DDX_Radio(pDX, IDC_FROMFILE, m_bFromText);
	DDX_Text(pDX, IDC_INPUTFILE, m_sFromFilePath);
	DDX_Text(pDX, IDC_INPUTTEXT, m_sFromText);
}

BEGIN_MESSAGE_MAP(CTDLImportFromPage, CCmdNotifyPropertyPage)
	ON_BN_CLICKED(IDC_FROMTEXT, OnChangeImportFrom)
	ON_EN_CHANGE(IDC_INPUTTEXT, OnChangeClipboardtext)
	ON_EN_CHANGE(IDC_INPUTFILE, OnChangeFilepath)
	ON_BN_CLICKED(IDC_REFRESHCLIPBOARD, OnRefreshclipboard)
	ON_BN_CLICKED(IDC_FROMFILE, OnChangeImportFrom)
END_MESSAGE_MAP()

//----------------------------------------------------------

BOOL CTDLImportFromPage::SetUseFile(LPCTSTR szFilePath)
{
	if (Misc::IsEmpty(szFilePath))
		return FALSE;

	m_sFromFilePath = szFilePath;
	m_nImportMode = TDCIM_FILEONLY;

	int nFormat = m_mgrImportExport.FindImporterByPath(szFilePath);

	if (nFormat == -1)
		return FALSE;

	// else
	m_bFromText = FALSE;
	m_sFromText.Empty();

	return TRUE;
}

void CTDLImportFromPage::SetUseClipboard()
{
	m_nImportMode = TDCIM_CLIPBOARDONLY;
	m_sFromText = CClipboard().GetText();
	m_bFromText = TRUE;
	m_sFromFilePath.Empty();
}

BOOL CTDLImportFromPage::SetUseText(LPCTSTR szText)
{
	if (Misc::IsEmpty(szText))
		return FALSE;

	m_nImportMode = TDCIM_TEXTONLY;
	m_sFromText = szText;
	m_bFromText = TRUE;
	m_sFromFilePath.Empty();

	return TRUE;
}

void CTDLImportFromPage::OnChangeImportFrom()
{
	ASSERT(m_nImportMode == TDCIM_ALL);

	UpdateData();
	EnableDisableControls();
}

BOOL CTDLImportFromPage::OnInitDialog()
{
	CCmdNotifyPropertyPage::OnInitDialog();

	m_eFilePath.SetFilter(GetCurrentImporterFilter());

	BOOL bHasFilter = IsCurrentImporterFileBased();
	ASSERT((m_nImportMode != TDCIM_FILEONLY) || bHasFilter);

	switch (m_nImportMode)
	{
	case TDCIM_ALL:
		break;

	case TDCIM_FILEONLY:
		ASSERT(!m_bFromText);
		SetWindowText(CEnString(IDS_IMPORTDIALOGTITLE_FILE));
		break;

	case TDCIM_CLIPBOARDONLY:
		ASSERT(m_bFromText);
		SetWindowText(CEnString(IDS_IMPORTDIALOGTITLE_CLIPBOARD));
		break;

	case TDCIM_TEXTONLY:
		ASSERT(m_bFromText);
		SetWindowText(CEnString(IDS_IMPORTDIALOGTITLE_TEXT));
		break;
	}
	
	// Set text font to be mono-spaced
	if (GraphicsMisc::CreateFont(m_fontMonospace, _T("Lucida Console")))
		GetDlgItem(IDC_INPUTTEXT)->SetFont(&m_fontMonospace, FALSE);

	if (m_nImportMode != TDCIM_FILEONLY)
		UpdateTextField();

	EnableDisableControls();
	
	return TRUE;
}

void CTDLImportFromPage::EnableDisableControls()
{
	BOOL bHasFilter = IsCurrentImporterFileBased();
	ASSERT((m_nImportMode != TDCIM_FILEONLY) || bHasFilter);

	switch (m_nImportMode)
	{
	case TDCIM_ALL:
		{
			GetDlgItem(IDC_FROMFILE)->EnableWindow(bHasFilter);
			GetDlgItem(IDC_INPUTFILE)->EnableWindow(!m_bFromText && bHasFilter);
			GetDlgItem(IDC_FROMTEXT)->EnableWindow(bHasFilter);
			GetDlgItem(IDC_INPUTTEXT)->EnableWindow(m_bFromText && bHasFilter);
			GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromText && bHasFilter);
		}
		break;

	case TDCIM_FILEONLY:
		{
			ASSERT(!m_bFromText);

			GetDlgItem(IDC_FROMFILE)->EnableWindow(TRUE);
			GetDlgItem(IDC_INPUTFILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_FROMTEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_INPUTTEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(FALSE);
		}
		break;

	case TDCIM_CLIPBOARDONLY:
		{
			ASSERT(m_bFromText);

			GetDlgItem(IDC_FROMFILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_INPUTFILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_FROMTEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_INPUTTEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(TRUE);
		}
		break;

	case TDCIM_TEXTONLY:
		{
			ASSERT(m_bFromText);

			GetDlgItem(IDC_FROMFILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_INPUTFILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_FROMTEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_INPUTTEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(FALSE);
			GetDlgItem(IDC_REFRESHCLIPBOARD)->ShowWindow(SW_HIDE);

			SetDlgItemText(IDC_FROMTEXT, CEnString(IDS_IMPORTFROMTEXT));
		}
		break;
	}
}

BOOL CTDLImportFromPage::IsCurrentImporterFileBased() const
{
	int nFormat = m_mgrImportExport.FindImporterByType(m_sFormatTypeID);

	return m_mgrImportExport.ImporterHasFileExtension(nFormat);
}

CString CTDLImportFromPage::GetCurrentImporterFilter() const
{
	int nFormat = m_mgrImportExport.FindImporterByType(m_sFormatTypeID);

	return m_mgrImportExport.GetImporterFileFilter(nFormat);
}

void CTDLImportFromPage::OnOK()
{
	CCmdNotifyPropertyPage::OnOK();

	CPreferences prefs;
	
	prefs.WriteProfileInt(PREFSKEY, _T("ImportOption"), m_bFromText);
	prefs.WriteProfileString(PREFSKEY, _T("ImportFilePath"), m_sFromFilePath);

	// retrieve input text
	if (IsCurrentImporterFileBased())
		GetDlgItemText(IDC_INPUTTEXT, m_sFromText);
}

BOOL CTDLImportFromPage::GetImportFromText() const
{
	return m_bFromText;
}

CString CTDLImportFromPage::GetImportFilePath() const
{
	return (m_bFromText || !IsCurrentImporterFileBased()) ? _T("") : m_sFromFilePath;
}

CString CTDLImportFromPage::GetImportText() const
{
	if (!m_bFromText || !IsCurrentImporterFileBased())
		return _T("");

	// else un-escape typical delimiters
	CString sText(m_sFromText);

	sText.Replace(_T("\\t"), _T("\t"));
	sText.Replace(_T("\\r"), _T("\r"));
	sText.Replace(_T("\\n"), _T("\r"));

	return sText;
}

void CTDLImportFromPage::SetImporterFormatID(LPCTSTR szFormatID)
{
	int nFormat = m_mgrImportExport.FindImporterByType(m_sFormatTypeID);
	BOOL bHadFilter = m_mgrImportExport.ImporterHasFileExtension(nFormat);

	m_sFormatTypeID = szFormatID;
	
	// change the filter on the CFileEdit and clear the filepath
	// and clear/restore clipboard text depending
	m_eFilePath.SetFilter(GetCurrentImporterFilter());

	BOOL bHasFilter = IsCurrentImporterFileBased();

	if (bHadFilter && !bHasFilter)
	{
		GetDlgItemText(IDC_INPUTTEXT, m_sFromText); // update
		SetDlgItemText(IDC_INPUTTEXT, _T("")); // clear field
	}
	else if (!bHadFilter && bHasFilter)
	{
		SetDlgItemText(IDC_INPUTTEXT, m_sFromText); // restore field
	}
	
	SetDlgItemText(IDC_INPUTFILE, _T("")); // clear field

	EnableDisableControls();
}

void CTDLImportFromPage::OnChangeClipboardtext() 
{
	GetDlgItemText(IDC_INPUTTEXT, m_sFromText); // update
}

void CTDLImportFromPage::OnChangeFilepath() 
{
	UpdateData();
}

void CTDLImportFromPage::OnRefreshclipboard() 
{
	if ((m_nImportMode == TDCIM_ALL )|| (m_nImportMode == TDCIM_CLIPBOARDONLY))
	{
		m_sFromText = CClipboard().GetText();
		UpdateTextField();
	}
}

void CTDLImportFromPage::UpdateTextField()
{
	if ((m_nImportMode == TDCIM_ALL ) || (m_nImportMode != TDCIM_FILEONLY))
	{
		// Edit control wants CRLF not just LF
		if (m_sFromText.Find(CRLF) == -1)
			m_sFromText.Replace(LF, CRLF);

		// Add blank line at start in case the chosen importer
		// treats the first line as a header line
		m_sFromText = (CRLF + m_sFromText);

		if (IsCurrentImporterFileBased())
			SetDlgItemText(IDC_INPUTTEXT, m_sFromText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTDLImportToPage page

CTDLImportToPage::CTDLImportToPage(BOOL bReadonlyTasklist, BOOL bTasklistHasSelection, CWnd* pParent)
	:
	CCmdNotifyPropertyPage(IDD_IMPORT_TO_PAGE),
	m_bReadonlyTasklist(bReadonlyTasklist),
	m_bTasklistHasSelection(bTasklistHasSelection),
	m_bMatchByTaskID(FALSE),
	m_nImportTo(NEWTASKLIST)
{
	CPreferences prefs;

	if (!m_bReadonlyTasklist)
		m_nImportTo = prefs.GetProfileEnum(PREFSKEY, _T("ImportToWhere"), ACTIVETASKLIST);

	m_nActiveTasklistPos = prefs.GetProfileInt(PREFSKEY, _T("TasklistPos"), SELECTEDTASK);

	if (!m_bTasklistHasSelection && ((m_nActiveTasklistPos == SELECTEDTASK) || (m_nActiveTasklistPos == BELOWSELECTEDTASK)))
		m_nActiveTasklistPos = BOTTOMOFTASKLIST;
}

void CTDLImportToPage::DoDataExchange(CDataExchange* pDX)
{
	CCmdNotifyPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ACTIVETASKLISTPOSITION, m_cbTasklistPos);
	DDX_Radio(pDX, IDC_CREATENEWTASKLIST, m_nImportTo);
	DDX_Radio(pDX, IDC_MERGEBYTITLE, m_bMatchByTaskID);
	DDX_CBData(pDX, m_cbTasklistPos, m_nActiveTasklistPos, (int)(m_bTasklistHasSelection ? SELECTEDTASK : BOTTOMOFTASKLIST));
}

BEGIN_MESSAGE_MAP(CTDLImportToPage, CCmdNotifyPropertyPage)
	ON_BN_CLICKED(IDC_MERGETOACTIVETASKLIST, OnChangeImportTo)
	ON_BN_CLICKED(IDC_ADDTOACTIVETASKLIST, OnChangeImportTo)
	ON_BN_CLICKED(IDC_ADDTOSELECTEDTASK, OnChangeImportTo)
	ON_BN_CLICKED(IDC_CREATENEWTASKLIST, OnChangeImportTo)
END_MESSAGE_MAP()

//------------------------------------------------------------------

BOOL CTDLImportToPage::SetImportTo(TDLID_IMPORTTO nImportTo)
{
	switch (nImportTo)
	{
	case TDIT_CREATENEWTASKLIST:
		m_nImportTo = NEWTASKLIST;
		break;

	case TDIT_ADDTOTOPOFTASKLIST:
		m_nImportTo = ACTIVETASKLIST;
		m_nActiveTasklistPos = TOPOFTASKLIST;
		break;

	case TDIT_ADDTOSELECTEDTASK:
		m_nImportTo = ACTIVETASKLIST;
		m_nActiveTasklistPos = SELECTEDTASK;
		break;

	case TDIT_ADDBELOWSELECTEDTASK:
		m_nImportTo = ACTIVETASKLIST;
		m_nActiveTasklistPos = BELOWSELECTEDTASK;
		break;

	case TDIT_ADDTOBOTTOMOFTASKLIST:
		m_nImportTo = ACTIVETASKLIST;
		m_nActiveTasklistPos = BOTTOMOFTASKLIST;
		break;

	case TDIT_MERGETOTASKLISTBYID:
		m_nImportTo = MERGETASKLIST;
		m_bMatchByTaskID = TRUE;
		break;

	case TDIT_MERGETOTASKLISTBYTITLE:
		m_nImportTo = MERGETASKLIST;
		m_bMatchByTaskID = FALSE;
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CTDLImportToPage::OnInitDialog()
{
	CCmdNotifyPropertyPage::OnInitDialog();

	// Build active tasklist pos
	if (m_bTasklistHasSelection)
	{
		AddStringT(m_cbTasklistPos, IDS_IMPORTTOTOPOFTASKLIST, TOPOFTASKLIST);
		AddStringT(m_cbTasklistPos, IDS_IMPORTTOSELTASK, SELECTEDTASK);
		AddStringT(m_cbTasklistPos, IDS_IMPORTTOBELOWSELTASK, BELOWSELECTEDTASK);
		AddStringT(m_cbTasklistPos, IDS_IMPORTTOBOTTOMOFTASKLIST, BOTTOMOFTASKLIST);
	}
	else
	{
		AddStringT(m_cbTasklistPos, IDS_IMPORTTOTOPOFTASKLIST, TOPOFTASKLIST);
		AddStringT(m_cbTasklistPos, IDS_IMPORTTOBOTTOMOFTASKLIST, BOTTOMOFTASKLIST);
	}

	SelectItemByDataT(m_cbTasklistPos, m_nActiveTasklistPos);
	EnableDisableControls();

	return TRUE;
}

void CTDLImportToPage::EnableDisableControls()
{
	GetDlgItem(IDC_MERGETOACTIVETASKLIST)->EnableWindow(!m_bReadonlyTasklist);
	GetDlgItem(IDC_MERGEBYTITLE)->EnableWindow(!m_bReadonlyTasklist && (m_nImportTo == MERGETASKLIST));
	GetDlgItem(IDC_MERGEBYTASKID)->EnableWindow(!m_bReadonlyTasklist && (m_nImportTo == MERGETASKLIST));
	GetDlgItem(IDC_MERGEBYTASKIDWARNING)->EnableWindow(!m_bReadonlyTasklist && (m_nImportTo == MERGETASKLIST));

	GetDlgItem(IDC_TOACTIVETASLIST)->EnableWindow(!m_bReadonlyTasklist);
	GetDlgItem(IDC_ACTIVETASKLISTPOSITION)->EnableWindow(!m_bReadonlyTasklist && (m_nImportTo == ACTIVETASKLIST));
}

void CTDLImportToPage::OnChangeImportTo()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLImportToPage::OnOK()
{
	CCmdNotifyPropertyPage::OnOK();

	CPreferences prefs;

	prefs.WriteProfileInt(PREFSKEY, _T("ImportToWhere"), m_nImportTo);
	prefs.WriteProfileInt(PREFSKEY, _T("TasklistPos"), m_nActiveTasklistPos);
}

TDLID_IMPORTTO CTDLImportToPage::GetImportTo() const
{
	switch (m_nImportTo)
	{
	case NEWTASKLIST:
		return TDIT_CREATENEWTASKLIST;

	case ACTIVETASKLIST:
		switch (m_nActiveTasklistPos)
		{
		case TOPOFTASKLIST:
			return TDIT_ADDTOTOPOFTASKLIST;

		case SELECTEDTASK:
			return TDIT_ADDTOSELECTEDTASK;

		case BELOWSELECTEDTASK:
			return TDIT_ADDBELOWSELECTEDTASK;

		case BOTTOMOFTASKLIST:
			return TDIT_ADDTOBOTTOMOFTASKLIST;
			ASSERT(0);
		}
		break;

	case MERGETASKLIST:
		return (m_bMatchByTaskID ? TDIT_MERGETOTASKLISTBYID : TDIT_MERGETOTASKLISTBYTITLE);
	}

	ASSERT(0);
	return (TDLID_IMPORTTO)-1;
}

