// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLFilterDlg.h"
#include "tdcstatic.h"
#include "TDCCustomAttributeUIHelper.h"

#include "..\shared\enstring.h"
#include "..\shared\localizer.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDlg dialog


CTDLFilterDlg::CTDLFilterDlg(FILTER_TITLE nTitleFilter, 
							BOOL bMultiSelFilters, 
							const CStringArray& aAdvFilterNames,
							const CFilteredToDoCtrl& tdc, 
							const CDWordArray& aPriorityColors,
							CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLFilterDlg::IDD, _T("Filtering"), pParent), 
	m_cbCategoryFilter(bMultiSelFilters, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbAllocToFilter(bMultiSelFilters, IDS_TDC_NOBODY, IDS_TDC_ANYONE),
	m_cbAllocByFilter(bMultiSelFilters, IDS_TDC_NOBODY, IDS_TDC_ANYONE),
	m_cbStatusFilter(bMultiSelFilters, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbVersionFilter(bMultiSelFilters, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbTagFilter(bMultiSelFilters, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbPriorityFilter(TRUE),
	m_cbRiskFilter(TRUE),
	m_cbRecurrence(TRUE),
	m_eStartNextNDays(TRUE, _T("-0123456789")),
	m_eDueNextNDays(TRUE, _T("-0123456789")),
	m_nTitleFilter(nTitleFilter)
{
	// main filter
	tdc.GetFilter(m_filter);

	m_filter.nTitleOption = m_nTitleFilter;

	// get custom filter
	m_aAdvancedFilterNames.Copy(aAdvFilterNames);

	if (tdc.HasAdvancedFilter())
	{
		m_sAdvancedFilter = tdc.GetAdvancedFilterName();
		m_dwCustomFlags = tdc.GetAdvancedFilterFlags();

		m_filter.Reset(FS_ADVANCED);
	}
	else
	{
		m_sAdvancedFilter.Empty();
		m_dwCustomFlags = 0;
	}

	// auto-droplists
	tdc.GetAutoListData(m_tldListData, TDCA_ALL);

	m_cbPriorityFilter.SetColors(aPriorityColors);
}


void CTDLFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLFilterDlg)
	DDX_Control(pDX, IDC_OPTIONFILTERCOMBO, m_cbOptions);
	DDX_Control(pDX, IDC_CATEGORYFILTERCOMBO, m_cbCategoryFilter);
	DDX_Control(pDX, IDC_STATUSFILTERCOMBO, m_cbStatusFilter);
	DDX_Control(pDX, IDC_PRIORITYFILTERCOMBO, m_cbPriorityFilter);
	DDX_Control(pDX, IDC_RISKFILTERCOMBO, m_cbRiskFilter);
	DDX_Control(pDX, IDC_ALLOCTOFILTERCOMBO, m_cbAllocToFilter);
	DDX_Control(pDX, IDC_ALLOCBYFILTERCOMBO, m_cbAllocByFilter);
	DDX_Control(pDX, IDC_VERSIONFILTERCOMBO, m_cbVersionFilter);
	DDX_Control(pDX, IDC_TAGFILTERCOMBO, m_cbTagFilter);
	DDX_Control(pDX, IDC_FILTERCOMBO, m_cbTaskFilter);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STARTNEXTNDAYS, m_eStartNextNDays);
	DDX_Control(pDX, IDC_DUENEXTNDAYS, m_eDueNextNDays);
	DDX_Control(pDX, IDC_STARTFILTERCOMBO, m_cbStartFilter);
	DDX_Control(pDX, IDC_DUEFILTERCOMBO, m_cbDueFilter);
	DDX_Control(pDX, IDC_USERSTARTDATE, m_dtcUserStart);
	DDX_Control(pDX, IDC_USERDUEDATE, m_dtcUserDue);
	DDX_Control(pDX, IDC_RECURFILTERCOMBO, m_cbRecurrence);

	DDX_Text(pDX, IDC_TITLEFILTERTEXT, m_filter.sTitle);
	DDX_Text(pDX, IDC_STARTNEXTNDAYS, m_filter.nStartNextNDays);
	DDX_Text(pDX, IDC_DUENEXTNDAYS, m_filter.nDueNextNDays);
	DDX_DateTimeCtrl(pDX, IDC_USERSTARTDATE, m_filter.dtUserStart);
	DDX_DateTimeCtrl(pDX, IDC_USERDUEDATE, m_filter.dtUserDue);

	CDialogHelper::DDX_CBData(pDX, m_cbRecurrence, m_filter.nRecurrence, TDIR_NONE);

	m_cbPriorityFilter.DDX(pDX, m_filter.nPriority);
	m_cbRiskFilter.DDX(pDX, m_filter.nRisk);

	// special handling
	if (pDX->m_bSaveAndValidate)
	{
		// filter
		m_filter.nShow = m_cbTaskFilter.GetSelectedFilter(m_sAdvancedFilter);
		m_filter.nStartBy = m_cbStartFilter.GetSelectedFilter();
		m_filter.nDueBy = m_cbDueFilter.GetSelectedFilter();

		// auto droplists
		m_cbCategoryFilter.GetChecked(m_filter.aCategories);
		m_cbAllocToFilter.GetChecked(m_filter.aAllocTo);
		m_cbStatusFilter.GetChecked(m_filter.aStatus);
		m_cbAllocByFilter.GetChecked(m_filter.aAllocBy);
		m_cbVersionFilter.GetChecked(m_filter.aVersions);
		m_cbTagFilter.GetChecked(m_filter.aTags);

		// build the filter options from the listbox
		if (m_sAdvancedFilter.IsEmpty())
			m_filter.dwFlags = m_cbOptions.GetSelectedOptions();
		else
			m_dwCustomFlags = m_cbOptions.GetSelectedOptions();
	}
	else
	{
		// filter
		if (m_filter.IsAdvanced())
			m_cbTaskFilter.SelectAdvancedFilter(m_sAdvancedFilter);
		else
			m_cbTaskFilter.SelectFilter(m_filter.nShow);

		m_cbStartFilter.SelectFilter(m_filter.nStartBy);
		m_cbDueFilter.SelectFilter(m_filter.nDueBy);

		// auto droplists
		m_cbCategoryFilter.SetChecked(m_filter.aCategories);
		m_cbAllocToFilter.SetChecked(m_filter.aAllocTo);
		m_cbStatusFilter.SetChecked(m_filter.aStatus);
		m_cbAllocByFilter.SetChecked(m_filter.aAllocBy);
		m_cbVersionFilter.SetChecked(m_filter.aVersions);
		m_cbTagFilter.SetChecked(m_filter.aTags);

		// options states set in OnInitDialog
		if (m_sAdvancedFilter.IsEmpty())
		{
			m_cbOptions.Initialize(m_filter, TDCA_ALL, TRUE);
		}
		else
		{
			DWORD dwOptions = (FO_HIDEOVERDUE | FO_HIDEDONE | FO_SHOWALLSUB);
			m_cbOptions.Initialize(dwOptions, m_dwCustomFlags);
		}
	}
}


BEGIN_MESSAGE_MAP(CTDLFilterDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLFilterDlg)
	ON_BN_CLICKED(IDC_CLEARFILTER, OnClearfilter)
	ON_CBN_SELCHANGE(IDC_FILTERCOMBO, OnSelchangeFiltercombo)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_DUEFILTERCOMBO, OnSelchangeDatecombo)
	ON_CBN_SELCHANGE(IDC_STARTFILTERCOMBO, OnSelchangeDatecombo)
	ON_CBN_SELCHANGE(IDC_ALLOCTOFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_TAGFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_VERSIONFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_ALLOCBYFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_STATUSFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_CATEGORYFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_PRIORITYFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_RISKFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELCHANGE(IDC_RECURFILTERCOMBO, OnSelchangeFilter)
	ON_CBN_SELENDCANCEL(IDC_ALLOCTOFILTERCOMBO, OnSelcancelFilter)
	ON_CBN_SELENDCANCEL(IDC_TAGFILTERCOMBO, OnSelcancelFilter)
	ON_CBN_SELENDCANCEL(IDC_VERSIONFILTERCOMBO, OnSelcancelFilter)
	ON_CBN_SELENDCANCEL(IDC_ALLOCBYFILTERCOMBO, OnSelcancelFilter)
	ON_CBN_SELENDCANCEL(IDC_STATUSFILTERCOMBO, OnSelcancelFilter)
	ON_CBN_SELENDCANCEL(IDC_CATEGORYFILTERCOMBO, OnSelcancelFilter)
	ON_CBN_SELENDCANCEL(IDC_PRIORITYFILTERCOMBO, OnSelcancelFilter)
	ON_CBN_SELENDCANCEL(IDC_RISKFILTERCOMBO, OnSelcancelFilter)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_EN_CHANGE(IDC_STARTNEXTNDAYS, OnSelchangeStartNextNDays)
	ON_EN_CHANGE(IDC_DUENEXTNDAYS, OnSelchangeDueNextNDays)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDlg message handlers

FILTER_SHOW CTDLFilterDlg::GetFilter(TDCFILTER& filter, CString& sCustom, DWORD& dwCustomFlags) const
{
	filter = m_filter;
	sCustom = m_sAdvancedFilter;
	dwCustomFlags = m_dwCustomFlags;

	return filter.nShow;
}

BOOL CTDLFilterDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	// disable translation of user-data combos
	CLocalizer::EnableTranslation(m_cbAllocByFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbAllocToFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbCategoryFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbStatusFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbVersionFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbTagFilter, FALSE);

	// auto droplist filters
	m_cbAllocToFilter.SetStrings(m_tldListData.aAllocTo);
	m_cbAllocByFilter.SetStrings(m_tldListData.aAllocBy);
	m_cbCategoryFilter.SetStrings(m_tldListData.aCategory);
	m_cbStatusFilter.SetStrings(m_tldListData.aStatus);
	m_cbVersionFilter.SetStrings(m_tldListData.aVersion);
	m_cbTagFilter.SetStrings(m_tldListData.aTags);

	// title
	m_mgrPrompts.SetComboPrompt(m_cbOptions, CEnString(IDS_TDC_NONE));
	m_mgrPrompts.SetEditPrompt(IDC_TITLEFILTERTEXT, *this, CEnString(IDS_TDC_ANY));
	SetDlgItemText(IDC_TITLEFILTERLABEL, m_filter.GetTitleFilterLabel());

	// custom filters
	m_cbTaskFilter.AddAdvancedFilters(m_aAdvancedFilterNames, m_sAdvancedFilter);

	m_cbStartFilter.SetNextNDays(m_filter.nStartNextNDays);
	m_cbDueFilter.SetNextNDays(m_filter.nDueNextNDays);

	// update UI
	UpdateData(FALSE);
	EnableDisableControls();

	EnableToolTips();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLFilterDlg::EnableDisableControls()
{
	BOOL bEnable = ((m_filter.nShow != FS_SELECTED) && !m_filter.IsAdvanced());

	GetDlgItem(IDC_TITLEFILTERTEXT)->EnableWindow(bEnable);

	m_cbStartFilter.EnableWindow(bEnable);
	m_cbDueFilter.EnableWindow(bEnable);
	m_cbCategoryFilter.EnableWindow(bEnable);
	m_cbStatusFilter.EnableWindow(bEnable);
	m_cbPriorityFilter.EnableWindow(bEnable);
	m_cbRiskFilter.EnableWindow(bEnable);
	m_cbAllocToFilter.EnableWindow(bEnable);
	m_cbAllocByFilter.EnableWindow(bEnable);
	m_cbVersionFilter.EnableWindow(bEnable);
	m_cbTagFilter.EnableWindow(bEnable);

	// special cases
	m_cbOptions.EnableWindow(TRUE); // always

	// buddy date controls
	GetDlgItem(IDC_USERSTARTDATE)->EnableWindow(bEnable && (m_filter.nStartBy == FD_USER));
	GetDlgItem(IDC_USERDUEDATE)->EnableWindow(bEnable && (m_filter.nDueBy == FD_USER));
	GetDlgItem(IDC_USERSTARTDATE)->ShowWindow((m_filter.nStartBy != FD_NEXTNDAYS) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_USERDUEDATE)->ShowWindow((m_filter.nDueBy != FD_NEXTNDAYS) ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_STARTNEXTNDAYS)->EnableWindow(bEnable && (m_filter.nStartBy == FD_NEXTNDAYS));
	GetDlgItem(IDC_DUENEXTNDAYS)->EnableWindow(bEnable && (m_filter.nDueBy == FD_NEXTNDAYS));
	GetDlgItem(IDC_STARTNEXTNDAYS)->ShowWindow((m_filter.nStartBy == FD_NEXTNDAYS) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_DUENEXTNDAYS)->ShowWindow((m_filter.nDueBy == FD_NEXTNDAYS) ? SW_SHOW : SW_HIDE);
}

void CTDLFilterDlg::OnClearfilter() 
{
	m_filter.Reset();
	m_sAdvancedFilter.Empty();
		
	UpdateData(FALSE);
}

void CTDLFilterDlg::OnSelchangeFiltercombo() 
{
	// enable/disable controls if necessary
	UpdateData();
	EnableDisableControls();

	m_cbOptions.Initialize(m_filter, TDCA_ALL, m_cbCategoryFilter.IsMultiSelectionEnabled());
}

void CTDLFilterDlg::OnSelchangeFilter() 
{
	UpdateData();
}

void CTDLFilterDlg::OnSelcancelFilter() 
{
	// Restore existing filter
	UpdateData(FALSE);
}

void CTDLFilterDlg::OnSelchangeDatecombo()
{
	// enable/disable controls if necessary
	UpdateData();
	EnableDisableControls();
}

void CTDLFilterDlg::OnSelchangeStartNextNDays()
{
	UpdateData();
	
	m_cbStartFilter.SetNextNDays(m_filter.nStartNextNDays);
}

void CTDLFilterDlg::OnSelchangeDueNextNDays()
{
	UpdateData();
	
	m_cbDueFilter.SetNextNDays(m_filter.nDueNextNDays);
}

BOOL CTDLFilterDlg::OnToolTipNotify(UINT /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT*)pNMHDR;

	UINT nCtrlID = CToolTipCtrlEx::GetCtrlID(pTTT);

	static CString sTooltip;

	switch (nCtrlID)
	{
	case IDC_CATEGORYFILTERCOMBO:
		sTooltip = m_cbCategoryFilter.GetTooltip();
		break;

	case IDC_ALLOCTOFILTERCOMBO:
		sTooltip = m_cbAllocToFilter.GetTooltip();
		break;

	case IDC_STATUSFILTERCOMBO:
		sTooltip = m_cbStatusFilter.GetTooltip();
		break;

	case IDC_ALLOCBYFILTERCOMBO:
		sTooltip = m_cbAllocByFilter.GetTooltip();
		break;

	case IDC_VERSIONFILTERCOMBO:
		sTooltip = m_cbVersionFilter.GetTooltip();
		break;

	case IDC_TAGFILTERCOMBO:
		sTooltip = m_cbTagFilter.GetTooltip();
		break;

	case IDC_OPTIONFILTERCOMBO:
		sTooltip = m_cbOptions.GetTooltip();
		break;

	default:
		if (!CTDCCustomAttributeUIHelper::IsCustomFilterControl(nCtrlID))
			return FALSE;

		sTooltip = CTDCCustomAttributeUIHelper::GetFilterControlTooltip(this, nCtrlID);
		break;
	}

	if (!sTooltip.IsEmpty())
	{
		// disable translation of the tip
		CLocalizer::EnableTranslation(pNMHDR->hwndFrom, FALSE);

		// Set the tooltip text.
		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
		pTTT->lpszText = (LPTSTR)(LPCTSTR)sTooltip;
	}

	return TRUE; // handled
}