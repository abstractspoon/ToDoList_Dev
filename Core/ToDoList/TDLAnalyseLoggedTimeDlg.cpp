// TDLAnalyseLoggedTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLAnalyseLoggedTimeDlg.h"
#include "TDCStruct.h"
#include "TDCTaskTimeLog.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\enstring.h"
#include "..\Shared\misc.h"
#include "..\Shared\filemisc.h"
#include "..\Shared\DateHelper.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

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


CTDLAnalyseLoggedTimeDlg::CTDLAnalyseLoggedTimeDlg(const CString& sTaskFile, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLAnalyseLoggedTimeDlg::IDD, _T("AnalyseLog"), pParent), 
	m_aCustomAttribDefs(aCustomAttribDefs),
	m_dtFrom(COleDateTime::GetCurrentTime()),
	m_dtTo(COleDateTime::GetCurrentTime()),
	m_eOutputFile(FES_COMBOSTYLEBTN | FES_SAVEAS)
{
	//{{AFX_DATA_INIT(CTDLAnalyseLoggedTimeDlg)
	//}}AFX_DATA_INIT

	// restore previous state
	CPreferences prefs;

	m_nTimePeriod = prefs.GetProfileEnum(m_sPrefsKey, _T("AnalysisTimePeriod"), TTLP_THISMONTH);
	m_nBreakdown = prefs.GetProfileEnum(m_sPrefsKey, _T("AnalysisBreakdown"), TTLB_BYDAY);
	m_nOutputFormat = prefs.GetProfileEnum(m_sPrefsKey, _T("AnalysisOutputFormat"), TTLF_CSV);
	m_bAutoDisplayReport = prefs.GetProfileInt(m_sPrefsKey, _T("AutoDisplayReport"), FALSE);

	m_bGroupBy = prefs.GetProfileInt(m_sPrefsKey, _T("GroupBy"), FALSE);
	m_nGroupByAttrib = prefs.GetProfileEnum(m_sPrefsKey, _T("GroupByAttrib"), TDCA_STATUS);

	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(m_nGroupByAttrib))
	{
		int nCust = m_aCustomAttribDefs.Find(prefs.GetProfileString(m_sPrefsKey, _T("GroupByCustomAttrib")));

		if (nCust == -1)
		{
			// Fallback
			m_nGroupByAttrib = TDCA_STATUS;
		}
		else
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nCust];

			if (attribDef.IsList() && !attribDef.IsMultiList())
				m_nGroupByAttrib = m_aCustomAttribDefs[nCust].GetAttributeID();
			else
				m_nGroupByAttrib = TDCA_STATUS;
		}
	}

	m_dtFrom = prefs.GetProfileDouble(m_sPrefsKey, _T("AnalysisFromDate"), COleDateTime::GetCurrentTime());
	m_dtTo = prefs.GetProfileDouble(m_sPrefsKey, _T("AnalysisToDate"), COleDateTime::GetCurrentTime());

	CString sFolder = prefs.GetProfileString(m_sPrefsKey, _T("AnalysisFolder"));

	if (!sFolder.IsEmpty() && FileMisc::FolderExists(sFolder))
		FileMisc::MakePath(m_sOutputFilePath, NULL, sFolder, FileMisc::GetFileNameFromPath(m_sOutputFilePath));

	m_sOutputFilePath = CTDCTaskTimeLog(sTaskFile).GetLogPath();

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
	DDX_Control(pDX, IDC_OUTPUTFILEPATH, m_eOutputFile);
	DDX_Control(pDX, IDC_TIMEPERIODS, m_cbTimePeriod);
	DDX_Control(pDX, IDC_BREAKDOWNS, m_cbBreakdown);
	DDX_Control(pDX, IDC_GROUPBYATTRIB, m_cbGroupByAttrib);
	DDX_Check(pDX, IDC_GROUPBY, m_bGroupBy);
	DDX_Check(pDX, IDC_AUTODISPLAYREPORT, m_bAutoDisplayReport);

	CDialogHelper::DDX_CBData(pDX, m_cbTimePeriod, m_nTimePeriod, TTLP_THISMONTH);
	CDialogHelper::DDX_CBData(pDX, m_cbBreakdown, m_nBreakdown, TTLB_BYDAY);
	CDialogHelper::DDX_CBData(pDX, m_cbOutputFormat, m_nOutputFormat, TTLF_CSV);
	CDialogHelper::DDX_CBData(pDX, m_cbGroupByAttrib, m_nGroupByAttrib, TDCA_STATUS);
}

BEGIN_MESSAGE_MAP(CTDLAnalyseLoggedTimeDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLAnalyseLoggedTimeDlg)
	ON_CBN_SELCHANGE(IDC_OUTPUTFORMAT, OnSelchangeOutputFormat)
	ON_CBN_SELCHANGE(IDC_TIMEPERIODS, OnSelChangeTimePeriod)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GROUPBY, OnGroupBy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLAnalyseLoggedTimeDlg message handlers

BOOL CTDLAnalyseLoggedTimeDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	BuildTimePeriodCombo();
	BuildBreakdownCombo(m_nTimePeriod);
	BuildGroupByCombo();
	BuildOutputFormatCombo();

	m_cbGroupByAttrib.EnableWindow(m_bGroupBy);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLAnalyseLoggedTimeDlg::BuildTimePeriodCombo()
{
	CDialogHelper::AddString(m_cbTimePeriod, _T("Last year"),			TTLP_LASTYEAR);
	CDialogHelper::AddString(m_cbTimePeriod, _T("Last month"),			TTLP_LASTMONTH);
	CDialogHelper::AddString(m_cbTimePeriod, _T("Last week"),			TTLP_LASTWEEK);
	CDialogHelper::AddString(m_cbTimePeriod, _T("Yesterday"),			TTLP_YESTERDAY);
	CDialogHelper::AddString(m_cbTimePeriod, _T("Today"),				TTLP_TODAY);
	CDialogHelper::AddString(m_cbTimePeriod, _T("This week"),			TTLP_THISWEEK);
	CDialogHelper::AddString(m_cbTimePeriod, _T("This month"),			TTLP_THISMONTH);
	CDialogHelper::AddString(m_cbTimePeriod, _T("This year"),			TTLP_THISYEAR);
	CDialogHelper::AddString(m_cbTimePeriod, _T("<specific dates>"),	TTLP_FROMTO);

	CDialogHelper::SelectItemByData(m_cbTimePeriod, m_nTimePeriod);
	UpdateDateFields();
}

void CTDLAnalyseLoggedTimeDlg::BuildBreakdownCombo(TDCTTL_PERIOD nPeriod)
{
	TDCTTL_BREAKDOWN nPrevBreakdown = m_nBreakdown;

	m_cbBreakdown.ResetContent();
		
	CDialogHelper::AddString(m_cbBreakdown, _T("By task"),	TTLB_BYTASK); // Always
	CDialogHelper::AddString(m_cbBreakdown, _T("By day"),	TTLB_BYDAY); // Always

	if (nPeriod == TTLP_LASTYEAR || 
		nPeriod == TTLP_THISYEAR ||
		nPeriod == TTLP_LASTMONTH ||
		nPeriod == TTLP_THISMONTH)
	{
		CDialogHelper::AddString(m_cbBreakdown, _T("By week"), TTLB_BYWEEK);
		CDialogHelper::AddString(m_cbBreakdown, _T("By month"), TTLB_BYMONTH);
	}
	else if (nPeriod == TTLP_LASTWEEK || nPeriod == TTLP_THISWEEK)
	{
		CDialogHelper::AddString(m_cbBreakdown, _T("By week"), TTLB_BYWEEK);
	}

	if (CDialogHelper::SelectItemByData(m_cbBreakdown, nPrevBreakdown) == -1)
	{
		m_nBreakdown = TTLB_BYDAY;

		VERIFY(CDialogHelper::SelectItemByData(m_cbBreakdown, m_nBreakdown) != -1);
	}
}

void CTDLAnalyseLoggedTimeDlg::BuildGroupByCombo()
{
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_ALLOCBY,		TDCA_ALLOCBY);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_ALLOCTO,		TDCA_ALLOCTO);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_CATEGORY,		TDCA_CATEGORY);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_CREATEDBY,	TDCA_CREATEDBY);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_EXTERNALID,	TDCA_EXTERNALID);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_LASTMODBY,	TDCA_LASTMODBY);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_PRIORITY,		TDCA_PRIORITY);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_RISK,			TDCA_RISK);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_STATUS,		TDCA_STATUS);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_TAGS,			TDCA_TAGS);
	CDialogHelper::AddString(m_cbGroupByAttrib, IDS_TDLBC_VERSION,		TDCA_VERSION);

	// Add custom attributes
	int nCust = m_aCustomAttribDefs.GetSize();

	while (nCust--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs[nCust];

		if (!attribDef.IsList() || !attribDef.IsMultiList())
		{
			CEnString sItem(IDS_CUSTOMCOLUMN, attribDef.sLabel);
			CDialogHelper::AddString(m_cbGroupByAttrib, sItem, attribDef.GetAttributeID());
		}
	}

	CDialogHelper::SelectItemByData(m_cbGroupByAttrib, m_nGroupByAttrib);
}

void CTDLAnalyseLoggedTimeDlg::BuildOutputFormatCombo()
{
	int nFmt = NUM_FORMATS;

	while (nFmt--)
	{
		CEnString sFormat(FORMAT_TYPES[nFmt].nIDText), sFilter;
		VERIFY(Misc::Split(sFormat, sFilter, '|'));

		int nIndex = m_cbOutputFormat.AddString(sFormat);
		m_cbOutputFormat.SetItemData(nIndex, FORMAT_TYPES[nFmt].nFormat);
	}

	if (CDialogHelper::SelectItemByData(m_cbOutputFormat, m_nOutputFormat) != CB_ERR)
		OnSelchangeOutputFormat();
}

void CTDLAnalyseLoggedTimeDlg::OnOK()
{
	CTDLDialog::OnOK();

	// save state
	CPreferences prefs;

	prefs.WriteProfileDouble(m_sPrefsKey, _T("AnalysisFromDate"), m_dtFrom);
	prefs.WriteProfileDouble(m_sPrefsKey, _T("AnalysisToDate"), m_dtTo);
	prefs.WriteProfileString(m_sPrefsKey, _T("AnalysisFolder"), FileMisc::GetFolderFromFilePath(m_sOutputFilePath));

	prefs.WriteProfileInt(m_sPrefsKey, _T("AnalysisTimePeriod"), m_nTimePeriod);
	prefs.WriteProfileInt(m_sPrefsKey, _T("AnalysisBreakdown"), m_nBreakdown);
	prefs.WriteProfileInt(m_sPrefsKey, _T("AnalysisOutputFormat"), m_nOutputFormat);
	prefs.WriteProfileInt(m_sPrefsKey, _T("AutoDisplayReport"), m_bAutoDisplayReport);
	prefs.WriteProfileInt(m_sPrefsKey, _T("GroupBy"), m_bGroupBy);
	prefs.WriteProfileInt(m_sPrefsKey, _T("GroupByAttrib"), m_nGroupByAttrib);

	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(m_nGroupByAttrib))
	{
		int nCust = (m_nGroupByAttrib - TDCA_CUSTOMATTRIB);
		prefs.WriteProfileString(m_sPrefsKey, _T("GroupByCustomAttrib"), m_aCustomAttribDefs[nCust].sUniqueID);
	}
}

void CTDLAnalyseLoggedTimeDlg::OnSelChangeTimePeriod() 
{
	UpdateData();

	BuildBreakdownCombo(m_nTimePeriod);
	UpdateDateFields();
}

void CTDLAnalyseLoggedTimeDlg::UpdateDateFields()
{
	GetDlgItem(IDC_DATEFROM)->EnableWindow(m_nTimePeriod == TTLP_FROMTO);
	GetDlgItem(IDC_DATETO)->EnableWindow(m_nTimePeriod == TTLP_FROMTO);

	if (m_nTimePeriod != TTLP_FROMTO)
	{
		GetDateRange(m_dtFrom, m_dtTo);

		// 'To' date is always start of the next day
		m_dtTo.m_dt--;

		UpdateData(FALSE);
	}
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

	case TTLP_YESTERDAY:
		dtFrom = (dtNow.m_dt - 1.0);
		dtTo = dtNow;
		break;

	case TTLP_TODAY:
		dtFrom = dtNow;
		dtTo = (dtNow.m_dt + 1.0);
		break;

	case TTLP_LASTWEEK:
		dtTo = CDateHelper::GetDate(DHD_BEGINTHISWEEK);
		dtFrom = (dtTo.m_dt - 7.0);
		break;

	case TTLP_THISWEEK:
		dtFrom = CDateHelper::GetDate(DHD_BEGINTHISWEEK);
		dtTo = (dtFrom.m_dt + 7.0);
		break;

	case TTLP_LASTMONTH:
		CDateHelper::IncrementMonth(dtNow, -1);
		dtFrom.SetDate(dtNow.GetYear(), dtNow.GetMonth(), 1);
		dtTo = (CDateHelper::GetEndOfMonth(dtNow).m_dt + 1.0);
		break;

	case TTLP_THISMONTH:
		dtFrom.SetDate(dtNow.GetYear(), dtNow.GetMonth(), 1);
		dtTo = (CDateHelper::GetEndOfMonth(dtNow).m_dt + 1.0);
		break;

	case TTLP_LASTYEAR:
		dtFrom.SetDate(dtNow.GetYear() - 1, 1, 1);
		dtTo = CDateHelper::GetStartOfYear(dtNow);
		break;

	case TTLP_THISYEAR:
		dtFrom.SetDate(dtNow.GetYear(), 1, 1);
		dtTo = (CDateHelper::GetEndOfYear(dtNow).m_dt + 1.0);
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	return (dtTo > dtFrom);
}

void CTDLAnalyseLoggedTimeDlg::OnGroupBy()
{
	UpdateData();

	m_cbGroupByAttrib.EnableWindow(m_bGroupBy);
}

