// TDLImportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcenum.h"
#include "TDLImportDialog.h"

#include "../shared/enstring.h"
#include "../shared/misc.h"
#include "../shared/filemisc.h"
#include "../shared/preferences.h"
#include "../shared/dialoghelper.h"
#include "../shared/clipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog dialog


CTDLImportDialog::CTDLImportDialog(const CImportExportMgr& mgr, BOOL bReadonlyTasklist, CWnd* pParent /*=NULL*/)
	: CTDLDialog(CTDLImportDialog::IDD, pParent),
	  m_mgrImportExport(mgr),
	  m_sizeMin(0, 0),
	  m_cbFormat(mgr, TRUE),
	  m_bReadonlyTasklist(bReadonlyTasklist)
{
	//{{AFX_DATA_INIT(CTDLImportDialog)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bFromClipboard = prefs.GetProfileInt(_T("Importing"), _T("ImportOption"), FALSE);
	m_sFromFilePath = prefs.GetProfileString(_T("Importing"), _T("ImportFilePath"));
	m_nFormatOption = prefs.GetProfileInt(_T("Importing"), _T("ImportFormat"), 0);
	m_nFormatOption = min(m_nFormatOption, mgr.GetNumImporters());

	if (m_bReadonlyTasklist)
		m_nImportTo = TDIT_NEWTASKLIST;
	else
		m_nImportTo = prefs.GetProfileInt(_T("Importing"), _T("ImportToWhere"), TDIT_ONSELECTEDTASK);

}


void CTDLImportDialog::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLImportDialog)
	DDX_Control(pDX, IDC_FROMFILEPATH, m_eFilePath);
	DDX_Control(pDX, IDC_FORMATOPTIONS, m_cbFormat);
	DDX_Radio(pDX, IDC_FROMFILE, m_bFromClipboard);
	DDX_Text(pDX, IDC_FROMFILEPATH, m_sFromFilePath);
	DDX_Radio(pDX, IDC_TONEWTASKLIST, m_nImportTo);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nIndex = m_cbFormat.GetCurSel();
		m_nFormatOption = m_cbFormat.GetItemData(nIndex);
	}
	else
		CDialogHelper::SelectItemByData(m_cbFormat, m_nFormatOption);
}


BEGIN_MESSAGE_MAP(CTDLImportDialog, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLImportDialog)
	ON_BN_CLICKED(IDC_FROMCLIPBOARD, OnChangeImportFrom)
	ON_CBN_SELCHANGE(IDC_FORMATOPTIONS, OnSelchangeFormatoptions)
	ON_EN_CHANGE(IDC_FROMCLIPBOARDTEXT, OnChangeClipboardtext)
	ON_EN_CHANGE(IDC_FROMFILEPATH, OnChangeFilepath)
	ON_BN_CLICKED(IDC_REFRESHCLIPBOARD, OnRefreshclipboard)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_FROMFILE, OnChangeImportFrom)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog message handlers

void CTDLImportDialog::OnChangeImportFrom() 
{
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

	// init file edit
	BOOL bHasFilter = CurImporterHasFilter();

	m_eFilePath.SetFilter(GetCurImporterFilter());
	m_eFilePath.EnableWindow(bHasFilter);
	
	GetDlgItem(IDC_FROMFILE)->EnableWindow(bHasFilter);
	GetDlgItem(IDC_FROMFILEPATH)->EnableWindow(!m_bFromClipboard && bHasFilter);
	GetDlgItem(IDC_FROMCLIPBOARD)->EnableWindow(bHasFilter);
	GetDlgItem(IDC_FROMCLIPBOARDTEXT)->EnableWindow(m_bFromClipboard && bHasFilter);
	GetDlgItem(IDC_REFRESHCLIPBOARD)->EnableWindow(m_bFromClipboard && bHasFilter);

	if (m_bReadonlyTasklist)
	{
		GetDlgItem(IDC_TOACTIVETASLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_TOSELECTEDTASK)->EnableWindow(FALSE);
	}
	
	m_sClipboardText = _T("\r\n") + CClipboard().GetText();

	if (bHasFilter)
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->SetWindowText(m_sClipboardText);

	EnableOK();
	
	return TRUE;  // return TRUE unless you set the focus to a control
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
	
	prefs.WriteProfileInt(_T("Importing"), _T("ImportOption"), m_bFromClipboard);
	prefs.WriteProfileString(_T("Importing"), _T("ImportFilePath"), m_sFromFilePath);
	prefs.WriteProfileInt(_T("Importing"), _T("ImportToWhere"), m_nImportTo);
	prefs.WriteProfileInt(_T("Importing"), _T("ImportFormat"), m_nFormatOption);

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
	return (TDLID_IMPORTTO)m_nImportTo;
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
		GetDlgItem(IDOK)->EnableWindow(TRUE);

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
	m_sClipboardText = _T("\r\n") + CClipboard().GetText();

	// set clipboard text
	if (CurImporterHasFilter())
		GetDlgItem(IDC_FROMCLIPBOARDTEXT)->SetWindowText(m_sClipboardText);
}

void CTDLImportDialog::OnSize(UINT nType, int cx, int cy) 
{
	static CSize sizePrev(0, 0);

	// initialize min size
	if (m_sizeMin.cx == 0 || m_sizeMin.cy == 0)
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		m_sizeMin = rWindow.Size();
	}

	CTDLDialog::OnSize(nType, cx, cy);
	
	// move the controls required
	if (m_eFilePath.GetSafeHwnd() && (sizePrev.cx > 0 || sizePrev.cy > 0))
	{
		int nDx = cx - sizePrev.cx;
		int nDy = cy - sizePrev.cy;

		CDialogHelper::ResizeCtrl(this, IDC_FROMBORDER, nDx, nDy);
		CDialogHelper::ResizeCtrl(this, IDC_FROMCLIPBOARDTEXT, nDx, nDy);
		CDialogHelper::OffsetCtrl(this, IDC_REFRESHCLIPBOARD, nDx, nDy);

		CDialogHelper::OffsetCtrl(this, IDC_TOBORDER, 0, nDy);
		CDialogHelper::ResizeCtrl(this, IDC_TOBORDER, nDx, 0);
		CDialogHelper::OffsetCtrl(this, IDC_TOACTIVETASLIST, 0, nDy);
		CDialogHelper::OffsetCtrl(this, IDC_TOSELECTEDTASK, 0, nDy);
		CDialogHelper::OffsetCtrl(this, IDC_TONEWTASKLIST, 0, nDy);

		CDialogHelper::OffsetCtrl(this, IDOK, nDx, nDy);
		CDialogHelper::OffsetCtrl(this, IDCANCEL, nDx, nDy);

		Invalidate(FALSE);
		UpdateWindow();
	}

	// snapshot current size before the change
	sizePrev.cx = cx;
	sizePrev.cy = cy;
}


void CTDLImportDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CTDLDialog::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = m_sizeMin.cx;
	lpMMI->ptMinTrackSize.y = m_sizeMin.cy;
}
