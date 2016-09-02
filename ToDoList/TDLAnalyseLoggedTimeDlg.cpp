// TDLAnalyseLoggedTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLAnalyseLoggedTimeDlg.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\enstring.h"
#include "..\Shared\misc.h"
#include "..\Shared\filemisc.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct LOGFORMATTYPE
{
	UINT nIDText;
	TDCTTL_FORMAT nFormat; 
	LPCTSTR szExt;
};

LOGFORMATTYPE FORMAT_TYPES[] = 
{
	{ IDS_ANALYSECSV_FORMAT, TTLF_CSV, _T("csv") },
	{ IDS_ANALYSEXML_FORMAT, TTLF_XML, _T("xml") }
// 	{ IDS_ANALYSETXT_FORMAT, TTLF_TXT, _T("txt") },
// 	{ IDS_ANALYSEHTM_FORMAT, TTLF_HTM, _T("htm") },
};
int NUM_FORMATS = (sizeof(FORMAT_TYPES) / sizeof(LOGFORMATTYPE));

static int FindFormat(TDCTTL_FORMAT nFormat)
{
	int nFmt = NUM_FORMATS;

	while (nFmt--)
	{
		if (FORMAT_TYPES[nFmt].nFormat == nFormat)
			return nFmt;
	}

	ASSERT(0);
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// CTDLAnalyseLoggedTimeDlg dialog


CTDLAnalyseLoggedTimeDlg::CTDLAnalyseLoggedTimeDlg(const CString& sLogPath, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLAnalyseLoggedTimeDlg::IDD, pParent), 
	m_sOutputFilePath(sLogPath),
	m_dtFrom(COleDateTime::GetCurrentTime()),
	m_dtTo(COleDateTime::GetCurrentTime()),
	m_eOutputFile(FES_COMBOSTYLEBTN | FES_SAVEAS)
{
	//{{AFX_DATA_INIT(CTDLAnalyseLoggedTimeDlg)
	//}}AFX_DATA_INIT

	// restore previous state
	CPreferences prefs;

	m_nTimePeriod = (TDCTTL_PERIOD)prefs.GetProfileInt(_T("Preferences"), _T("AnalysisTimePeriod"), TTLP_THISMONTH);
	m_nBreakdown = (TDCTTL_BREAKDOWN)prefs.GetProfileInt(_T("Preferences"), _T("AnalysisBreakdown"), TTLB_BYDAY);
	m_nOutputFormat = (TDCTTL_FORMAT)prefs.GetProfileInt(_T("Preferences"), _T("preAnalysisOutputFormat"), TTLF_CSV);
	m_dtFrom = prefs.GetProfileDouble(_T("Preferences"), _T("AnalysisFromDate"), COleDateTime::GetCurrentTime());
	m_dtTo = prefs.GetProfileDouble(_T("Preferences"), _T("AnalysisToDate"), COleDateTime::GetCurrentTime());

	CString sFolder = prefs.GetProfileString(_T("Preferences"), _T("AnalysisFolder"));

	if (!sFolder.IsEmpty() && FileMisc::FolderExists(sFolder))
		FileMisc::MakePath(m_sOutputFilePath, NULL, sFolder, FileMisc::GetFileNameFromPath(m_sOutputFilePath));

	FileMisc::RemoveExtension(m_sOutputFilePath);
	m_sOutputFilePath += _T("_Report");
}


void CTDLAnalyseLoggedTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLAnalyseLoggedTimeDlg)
	DDX_Control(pDX, IDC_OUTPUTFORMAT, m_cbOutputFormat);
	DDX_DateTimeCtrl(pDX, IDC_DATEFROM, m_dtFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATETO, m_dtTo);
	DDX_Text(pDX, IDC_OUTPUTFILEPATH, m_sOutputFilePath);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_TODAY, (int&)m_nTimePeriod);
	DDX_Radio(pDX, IDC_BYTASK, (int&)m_nBreakdown);
	DDX_Control(pDX, IDC_OUTPUTFILEPATH, m_eOutputFile);

	if (pDX->m_bSaveAndValidate)
		m_nOutputFormat = (TDCTTL_FORMAT)CDialogHelper::GetSelectedItemData(m_cbOutputFormat);
	else
		CDialogHelper::SelectItemByData(m_cbOutputFormat, m_nOutputFormat);
}

BEGIN_MESSAGE_MAP(CTDLAnalyseLoggedTimeDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLAnalyseLoggedTimeDlg)
	ON_BN_CLICKED(IDC_TODAY, OnChangePeriod)
	ON_CBN_SELCHANGE(IDC_OUTPUTFORMAT, OnSelchangeOutputFormat)
	ON_BN_CLICKED(IDC_THISYEAR, OnChangePeriod)
	ON_BN_CLICKED(IDC_THISWEEK, OnChangePeriod)
	ON_BN_CLICKED(IDC_THISMONTH, OnChangePeriod)
	ON_BN_CLICKED(IDC_DATERANGE, OnChangePeriod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLAnalyseLoggedTimeDlg message handlers

BOOL CTDLAnalyseLoggedTimeDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	// we build the output format combo ourselves
	int nFmt = NUM_FORMATS;

	while (nFmt--)
	{
		CEnString sFormat(FORMAT_TYPES[nFmt].nIDText), sFilter;
		VERIFY(Misc::Split(sFormat, sFilter, '|'));

		int nIndex = m_cbOutputFormat.AddString(sFormat);
		m_cbOutputFormat.SetItemData(nIndex, FORMAT_TYPES[nFmt].nFormat);

		// handle selected format
		if (FORMAT_TYPES[nFmt].nFormat == m_nOutputFormat)
		{
			m_cbOutputFormat.SetCurSel(nIndex);
			OnSelchangeOutputFormat();
		}
	}

	// enable/disable breakdowns
	OnChangePeriod();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLAnalyseLoggedTimeDlg::OnOK()
{
	CTDLDialog::OnOK();

	// save state
	CPreferences prefs;

	prefs.WriteProfileInt(_T("Preferences"), _T("AnalysisTimePeriod"), m_nTimePeriod);
	prefs.WriteProfileInt(_T("Preferences"), _T("AnalysisBreakdown"), m_nBreakdown);
	prefs.WriteProfileInt(_T("Preferences"), _T("AnalysisOutputFormat"), m_nOutputFormat);
	prefs.WriteProfileDouble(_T("Preferences"), _T("AnalysisFromDate"), m_dtFrom);
	prefs.WriteProfileDouble(_T("Preferences"), _T("AnalysisToDate"), m_dtTo);
	prefs.WriteProfileString(_T("Preferences"), _T("AnalysisFolder"), FileMisc::GetFolderFromFilePath(m_sOutputFilePath));
}

void CTDLAnalyseLoggedTimeDlg::OnChangePeriod() 
{
	UpdateData();

	GetDlgItem(IDC_DATEFROM)->EnableWindow(m_nTimePeriod == TTLP_FROMTO);
	GetDlgItem(IDC_DATETO)->EnableWindow(m_nTimePeriod == TTLP_FROMTO);

	// enable/disable breakdowns
	TDCTTL_BREAKDOWN nBreakdown = m_nBreakdown;

	switch (m_nTimePeriod)
	{
	case TTLP_FROMTO:
		break;

	case TTLP_TODAY:
		nBreakdown = min(nBreakdown, TTLB_BYDAY);
		break;

	case TTLP_THISWEEK:
		nBreakdown = min(nBreakdown, TTLB_BYWEEK);
		break;

	case TTLP_THISMONTH:
	case TTLP_THISYEAR:
		nBreakdown = min(nBreakdown, TTLB_BYMONTH);
		break;
	}

	if (nBreakdown < m_nBreakdown)
	{
		m_nBreakdown = nBreakdown;
		UpdateData(FALSE);
	}

	// Note: BY_TASK always enabled
	GetDlgItem(IDC_BYDAY)->EnableWindow(m_nTimePeriod >= TTLP_TODAY);
	GetDlgItem(IDC_BYWEEK)->EnableWindow(m_nTimePeriod >= TTLP_THISWEEK);
	GetDlgItem(IDC_BYMONTH)->EnableWindow(m_nTimePeriod >= TTLP_THISMONTH);
}

void CTDLAnalyseLoggedTimeDlg::OnSelchangeOutputFormat() 
{
	UpdateData();

	// update filter
	int nFmt = FindFormat(m_nOutputFormat);

	CEnString sFormat(FORMAT_TYPES[nFmt].nIDText), sFilter;
	VERIFY(Misc::Split(sFormat, sFilter, '|'));

	m_eOutputFile.SetFilter(sFilter);

	// change extension of output file
	FileMisc::ReplaceExtension(m_sOutputFilePath, FORMAT_TYPES[nFmt].szExt);
	m_eOutputFile.SetWindowText(m_sOutputFilePath);
}

BOOL CTDLAnalyseLoggedTimeDlg::GetDateRange(COleDateTime& dtFrom, COleDateTime& dtTo) const
{
	COleDateTime dtNow = CDateHelper::GetDate(DHD_TODAY);

	CDateHelper::ClearDate(dtFrom);
	CDateHelper::ClearDate(dtTo);

	switch (m_nTimePeriod)
	{
	case TTLP_FROMTO:
		dtFrom = CDateHelper::GetDateOnly(m_dtFrom);
		dtTo = (CDateHelper::GetDateOnly(m_dtTo).m_dt + 1.0);
		break;

	case TTLP_TODAY:
		dtFrom = dtNow;
		dtTo = (dtNow.m_dt + 1.0);
		break;

	case TTLP_THISWEEK:
		dtFrom = CDateHelper::GetDate(DHD_BEGINTHISWEEK);
		dtTo = (dtNow.m_dt + 1.0);
		break;

	case TTLP_THISMONTH:
		dtFrom.SetDate(dtNow.GetYear(), dtNow.GetMonth(), 1);
		dtTo = (dtNow.m_dt + 1.0);
		break;

	case TTLP_THISYEAR:
		dtFrom.SetDate(dtNow.GetYear(), 1, 1);
		dtTo = (dtNow.m_dt + 1.0);
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	return (dtTo > dtFrom);
}
