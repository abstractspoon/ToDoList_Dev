// FilterBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLFilterBar.h"
#include "tdcmsg.h"
#include "filteredtodoctrl.h"
#include "tdccustomattributeUIHelper.h"

#include "..\shared\deferwndmove.h"
#include "..\shared\dlgunits.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\themed.h"
#include "..\shared\holdredraw.h"

#include "..\3rdparty\colordef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// In DLU
const int CTRLHSPACING	= 6;
const int CTRLVSPACING	= 4;
const int CTRLLEN		= 75;
const int LABELHEIGHT	= 9;

static CTRLITEM FILTERCTRLS[] = 
{
	CTRLITEM(IDC_FILTERCOMBO,			IDC_FILTERLABEL,			TDCA_NONE),
	CTRLITEM(IDC_TITLEFILTERTEXT,		IDC_TITLEFILTERLABEL,		TDCA_TASKNAME),
	CTRLITEM(IDC_STARTFILTERCOMBO,		IDC_STARTFILTERLABEL,		TDCA_STARTDATE),
	CTRLITEM(IDC_USERSTARTDATE,			0,							TDCA_STARTDATE),
	CTRLITEM(IDC_STARTNEXTNDAYS,		0,							TDCA_STARTDATE),
	CTRLITEM(IDC_DUEFILTERCOMBO,		IDC_DUEFILTERLABEL,			TDCA_DUEDATE),
	CTRLITEM(IDC_USERDUEDATE,			0,							TDCA_DUEDATE),
	CTRLITEM(IDC_DUENEXTNDAYS,			0,							TDCA_DUEDATE),
	CTRLITEM(IDC_PRIORITYFILTERCOMBO,	IDC_PRIORITYFILTERLABEL,	TDCA_PRIORITY),
	CTRLITEM(IDC_RISKFILTERCOMBO,		IDC_RISKFILTERLABEL,		TDCA_RISK),
	CTRLITEM(IDC_ALLOCTOFILTERCOMBO,	IDC_ALLOCTOFILTERLABEL,		TDCA_ALLOCTO),
	CTRLITEM(IDC_ALLOCBYFILTERCOMBO,	IDC_ALLOCBYFILTERLABEL,		TDCA_ALLOCBY),
	CTRLITEM(IDC_STATUSFILTERCOMBO,		IDC_STATUSFILTERLABEL,		TDCA_STATUS),
	CTRLITEM(IDC_CATEGORYFILTERCOMBO,	IDC_CATEGORYFILTERLABEL,	TDCA_CATEGORY),
	CTRLITEM(IDC_TAGFILTERCOMBO,		IDC_TAGFILTERLABEL,			TDCA_TAGS),
	CTRLITEM(IDC_VERSIONFILTERCOMBO,	IDC_VERSIONFILTERLABEL,		TDCA_VERSION),
	CTRLITEM(IDC_RECURFILTERCOMBO,		IDC_RECURFILTERLABEL,		TDCA_RECURRENCE),
	CTRLITEM(IDC_OPTIONFILTERCOMBO,		IDC_OPTIONFILTERLABEL,		TDCA_NONE),
};

const int NUMFILTERCTRLS = sizeof(FILTERCTRLS) / sizeof(CTRLITEM);

#define WM_WANTCOMBOPROMPT (WM_APP+1)

/////////////////////////////////////////////////////////////////////////////
// CFilterBar dialog

CTDLFilterBar::CTDLFilterBar(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_FILTER_BAR, pParent), 
	m_cbCategoryFilter(TRUE, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbAllocToFilter(TRUE, IDS_TDC_NOBODY, IDS_TDC_ANYONE),
	m_cbAllocByFilter(TRUE, IDS_TDC_NOBODY, IDS_TDC_ANYONE),
	m_cbStatusFilter(TRUE, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbVersionFilter(TRUE, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbTagFilter(TRUE, IDS_TDC_NONE, IDS_TDC_ANY),
	m_cbPriorityFilter(TRUE),
	m_cbRiskFilter(TRUE),
	m_cbRecurrence(TRUE),
	m_crUIBack(CLR_NONE),
	m_eStartNextNDays(TRUE, _T("-0123456789")),
	m_eDueNextNDays(TRUE, _T("-0123456789")),
	m_bMultiSelection(TRUE),
	m_nTitleFilter(FT_FILTERONTITLEONLY)
{
	//{{AFX_DATA_INIT(CFilterBar)
	//}}AFX_DATA_INIT
	m_iconUpdateBtn.Load(IDI_UPDATE_FILTER, 16, FALSE);

	// add update button to 'title text' and 'next 'n' days'
	m_eTitleFilter.AddButton(1, m_iconUpdateBtn, CEnString(IDS_TDC_UPDATEFILTER_TIP));
	m_eStartNextNDays.AddButton(1, m_iconUpdateBtn, CEnString(IDS_TDC_UPDATEFILTER_TIP));
	m_eDueNextNDays.AddButton(1, m_iconUpdateBtn, CEnString(IDS_TDC_UPDATEFILTER_TIP));
}

CTDLFilterBar::~CTDLFilterBar()
{
}

void CTDLFilterBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterBar)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAGFILTERCOMBO, m_cbTagFilter);
	DDX_Control(pDX, IDC_VERSIONFILTERCOMBO, m_cbVersionFilter);
	DDX_Control(pDX, IDC_OPTIONFILTERCOMBO, m_cbOptions);
	DDX_Control(pDX, IDC_FILTERCOMBO, m_cbTaskFilter);
	DDX_Control(pDX, IDC_STARTFILTERCOMBO, m_cbStartFilter);
	DDX_Control(pDX, IDC_DUEFILTERCOMBO, m_cbDueFilter);
	DDX_Control(pDX, IDC_ALLOCTOFILTERCOMBO, m_cbAllocToFilter);
	DDX_Control(pDX, IDC_ALLOCBYFILTERCOMBO, m_cbAllocByFilter);
	DDX_Control(pDX, IDC_CATEGORYFILTERCOMBO, m_cbCategoryFilter);
	DDX_Control(pDX, IDC_STATUSFILTERCOMBO, m_cbStatusFilter);
	DDX_Control(pDX, IDC_PRIORITYFILTERCOMBO, m_cbPriorityFilter);
	DDX_Control(pDX, IDC_RISKFILTERCOMBO, m_cbRiskFilter);
	DDX_Control(pDX, IDC_RECURFILTERCOMBO, m_cbRecurrence);
	DDX_Control(pDX, IDC_TITLEFILTERTEXT, m_eTitleFilter);
	DDX_Control(pDX, IDC_USERSTARTDATE, m_dtcUserStart);
	DDX_Control(pDX, IDC_USERDUEDATE, m_dtcUserDue);
	DDX_Control(pDX, IDC_STARTNEXTNDAYS, m_eStartNextNDays);
	DDX_Control(pDX, IDC_DUENEXTNDAYS, m_eDueNextNDays);

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
		// Note: we only modify 'm_filter.nShow' in 'OnSelchangeFilter'
		// Note: we only modify 'm_filter.dwFlags' in 'OnCloseUpOptions'

		m_filter.nStartBy = m_cbStartFilter.GetSelectedFilter();
		m_filter.nDueBy = m_cbDueFilter.GetSelectedFilter();
		m_filter.nTitleOption = m_nTitleFilter;

		m_cbCategoryFilter.GetChecked(m_filter.aCategories);
		m_cbAllocToFilter.GetChecked(m_filter.aAllocTo);
		m_cbStatusFilter.GetChecked(m_filter.aStatus);
		m_cbAllocByFilter.GetChecked(m_filter.aAllocBy);
		m_cbVersionFilter.GetChecked(m_filter.aVersions);
		m_cbTagFilter.GetChecked(m_filter.aTags);
	}
	else
	{
		// filter
		if (m_filter.IsAdvanced())
			m_cbTaskFilter.SelectAdvancedFilter(m_sAdvancedFilter);
		else
			m_cbTaskFilter.SelectFilter(m_filter.nShow);
		
		m_cbStartFilter.SelectFilter(m_filter.nStartBy);
		m_cbStartFilter.SetNextNDays(m_filter.nStartNextNDays);
		m_cbDueFilter.SelectFilter(m_filter.nDueBy);
		m_cbDueFilter.SetNextNDays(m_filter.nDueNextNDays);

		// check combos
		m_cbCategoryFilter.SetChecked(m_filter.aCategories);
		m_cbAllocToFilter.SetChecked(m_filter.aAllocTo);
		m_cbStatusFilter.SetChecked(m_filter.aStatus);
		m_cbAllocByFilter.SetChecked(m_filter.aAllocBy);
		m_cbVersionFilter.SetChecked(m_filter.aVersions);
		m_cbTagFilter.SetChecked(m_filter.aTags);

		// options
		RebuildOptionsCombo();
	}
}

BEGIN_MESSAGE_MAP(CTDLFilterBar, CDialog)
	//{{AFX_MSG_MAP(CFilterBar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_ALLOCTOFILTERCOMBO, OnSelchangeFilterAttribute)
	ON_CBN_SELCHANGE(IDC_TAGFILTERCOMBO, OnSelchangeFilterAttribute)
	ON_CBN_SELCHANGE(IDC_VERSIONFILTERCOMBO, OnSelchangeFilterAttribute)
	ON_CBN_SELCHANGE(IDC_ALLOCBYFILTERCOMBO, OnSelchangeFilterAttribute)
	ON_CBN_SELCHANGE(IDC_STATUSFILTERCOMBO, OnSelchangeFilterAttribute)
	ON_CBN_SELCHANGE(IDC_CATEGORYFILTERCOMBO, OnSelchangeFilterAttribute)

	// Cancelling checkbox filters needs extra care
	ON_CBN_SELENDCANCEL(IDC_ALLOCTOFILTERCOMBO, OnSelcancelAllocToFilter)
	ON_CBN_SELENDCANCEL(IDC_TAGFILTERCOMBO, OnSelcancelTagFilter)
	ON_CBN_SELENDCANCEL(IDC_VERSIONFILTERCOMBO, OnSelcancelVersionFilter)
	ON_CBN_SELENDCANCEL(IDC_ALLOCBYFILTERCOMBO, OnSelcancelAllocByFilter)
	ON_CBN_SELENDCANCEL(IDC_STATUSFILTERCOMBO, OnSelcancelStatusFilter)
	ON_CBN_SELENDCANCEL(IDC_CATEGORYFILTERCOMBO, OnSelcancelCategoryFilter)

	ON_CBN_SELENDOK(IDC_STARTFILTERCOMBO, OnSelchangeStartDateFilter)
	ON_CBN_SELENDOK(IDC_DUEFILTERCOMBO, OnSelchangeDueDateFilter)
	ON_CBN_SELENDOK(IDC_PRIORITYFILTERCOMBO, OnSelchangeFilterAttribute)
	ON_CBN_SELENDOK(IDC_RISKFILTERCOMBO, OnSelchangeFilterAttribute)
	ON_CBN_SELENDOK(IDC_RECURFILTERCOMBO, OnSelchangeFilterAttribute)

	ON_CBN_SELENDOK(IDC_FILTERCOMBO, OnSelchangeFilter) // separate handler
	ON_CBN_CLOSEUP(IDC_OPTIONFILTERCOMBO, OnCloseUpOptions)

	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_USERDUEDATE, OnChangeDateFilter)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_USERSTARTDATE, OnChangeDateFilter)

	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_FIRST_CUSTOMFILTERFIELD, IDC_LAST_CUSTOMFILTERFIELD, OnCustomAttributeSelchangeFilter)
	ON_CONTROL_RANGE(CBN_SELENDCANCEL, IDC_FIRST_CUSTOMFILTERFIELD, IDC_LAST_CUSTOMFILTERFIELD, OnCustomAttributeSelcancelFilter)
	ON_NOTIFY_RANGE(DTN_DATETIMECHANGE, IDC_FIRST_CUSTOMFILTERFIELD, IDC_LAST_CUSTOMFILTERFIELD, OnCustomAttributeChangeDateFilter)

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)

	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_HELPINFO()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterBar message handlers

BOOL CTDLFilterBar::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(IDD_FILTER_BAR);
	return TRUE;
}

void CTDLFilterBar::OnDestroy()
{
	CTDCCustomAttributeUIHelper::CleanupControls(m_aCustomControls, this);

	CDialog::OnDestroy();
}

BOOL CTDLFilterBar::Create(CWnd* pParentWnd, UINT nID, BOOL bVisible)
{
	if (CDialog::Create(IDD_FILTER_BAR, pParentWnd))
	{
		SetDlgCtrlID(nID);
		SetWindowText(_T("FilterBar"));

		ModifyStyle(WS_CLIPCHILDREN, WS_TABSTOP, 0);
		ShowWindow(bVisible ? SW_SHOW : SW_HIDE);

		return TRUE;
	}

	return FALSE;
}

void CTDLFilterBar::ClearCheckboxHistory()
{
	if (m_bMultiSelection)
	{
		ASSERT(FilterMatches(TDCFILTER()));

		m_cbAllocByFilter.CheckAll(CCBC_UNCHECKED);
		m_cbAllocToFilter.CheckAll(CCBC_UNCHECKED);
		m_cbCategoryFilter.CheckAll(CCBC_UNCHECKED);
		m_cbStatusFilter.CheckAll(CCBC_UNCHECKED);
		m_cbTagFilter.CheckAll(CCBC_UNCHECKED);
		m_cbVersionFilter.CheckAll(CCBC_UNCHECKED);

		CTDCCustomAttributeUIHelper::ClearFilterCheckboxHistory(m_aCustomControls, this);
	}
}

void CTDLFilterBar::OnSelchangeFilter() 
{
	CString sAdvanced;
	FILTER_SHOW nShow = m_cbTaskFilter.GetSelectedFilter(sAdvanced);

	// Refresh the labels if switching from custom to not, or vice versa
	if (sAdvanced.IsEmpty() != m_sAdvancedFilter.IsEmpty())
		Invalidate(FALSE);

	// Only notify the parent if something actually changed
	if ((nShow != m_filter.nShow) || ((nShow == FS_ADVANCED) && (sAdvanced != m_sAdvancedFilter)))
	{
		m_filter.nShow = nShow;
		m_sAdvancedFilter = sAdvanced;

		// Update the Options combo with the stored custom flags before notifying the parent
		if (nShow == FS_ADVANCED)
		{
			DWORD dwCustomFlags = 0;
			m_mapCustomFlags.Lookup(sAdvanced, dwCustomFlags);

			m_cbOptions.SetSelectedOptions(dwCustomFlags);
		}
		
		NotifyParentFilterChange();
	}
}

void CTDLFilterBar::NotifyParentFilterChange()
{
	GetParent()->SendMessage(WM_FBN_FILTERCHANGE, GetDlgCtrlID(), (LPARAM)GetSafeHwnd());

	RefreshUIBkgndBrush();
}

void CTDLFilterBar::OnSelchangeFilterAttribute() 
{
	TDCFILTER prevFilter = m_filter;
	UpdateData();
	
	// Only notify the parent if something actually changed
	if (m_filter != prevFilter)
		NotifyParentFilterChange();
}

void CTDLFilterBar::OnSelcancelAllocToFilter()
{
	if (m_bMultiSelection)
	{
		// Restore existing filter
		m_cbAllocToFilter.SetChecked(m_filter.aAllocTo);
		m_cbAllocToFilter.ClearMultiSelectionHistory();
	}
}

void CTDLFilterBar::OnSelcancelAllocByFilter()
{
	if (m_bMultiSelection)
	{
		// Restore existing filter
		m_cbAllocByFilter.SetChecked(m_filter.aAllocBy);
		m_cbAllocByFilter.ClearMultiSelectionHistory();
	}
}

void CTDLFilterBar::OnSelcancelCategoryFilter()
{
	if (m_bMultiSelection)
	{
		// Restore existing filter
		m_cbCategoryFilter.SetChecked(m_filter.aCategories);
		m_cbCategoryFilter.ClearMultiSelectionHistory();
	}
}

void CTDLFilterBar::OnSelcancelStatusFilter()
{
	if (m_bMultiSelection)
	{
		// Restore existing filter
		m_cbStatusFilter.SetChecked(m_filter.aStatus);
		m_cbStatusFilter.ClearMultiSelectionHistory();
	}
}

void CTDLFilterBar::OnSelcancelTagFilter()
{
	if (m_bMultiSelection)
	{
		// Restore existing filter
		m_cbTagFilter.SetChecked(m_filter.aTags);
		m_cbTagFilter.ClearMultiSelectionHistory();
	}
}

void CTDLFilterBar::OnSelcancelVersionFilter()
{
	if (m_bMultiSelection)
	{
		// Restore existing filter
		m_cbVersionFilter.SetChecked(m_filter.aVersions);
		m_cbVersionFilter.ClearMultiSelectionHistory();
	}
}

void CTDLFilterBar::OnCloseUpOptions()
{
	DWORD dwPrevFlags = 0, dwNewFlags = m_cbOptions.GetSelectedOptions();

	if (m_filter.IsAdvanced())
		m_mapCustomFlags.Lookup(m_sAdvancedFilter, dwPrevFlags);
	else
		dwPrevFlags = m_filter.dwFlags;

	// only notify parent if there has been a change
	if (dwNewFlags != dwPrevFlags)
	{
		if (m_filter.IsAdvanced())
			m_mapCustomFlags[m_sAdvancedFilter] = dwNewFlags;
		else
			m_filter.dwFlags = dwNewFlags;

		NotifyParentFilterChange();
	}
}

void CTDLFilterBar::OnSelchangeDueDateFilter()
{
	OnSelchangeDateFilter(m_filter.nDueBy, m_cbDueFilter);
}

void CTDLFilterBar::OnSelchangeStartDateFilter()
{
	OnSelchangeDateFilter(m_filter.nStartBy, m_cbStartFilter);
}

void CTDLFilterBar::OnSelchangeDateFilter(FILTER_DATE nPrevFilter, const CTDLFilterDateComboBox& combo)
{
	FILTER_DATE nFilter = combo.GetSelectedFilter();

	BOOL bWasShowingBuddy = ((nPrevFilter == FD_USER) || (nPrevFilter == FD_NEXTNDAYS));
	BOOL bShowBuddy = ((nFilter == FD_USER) || (nFilter == FD_NEXTNDAYS));

	UpdateData();

	if (Misc::StateChanged(bWasShowingBuddy, bShowBuddy))
		ReposControls();

	NotifyParentFilterChange();
}

void CTDLFilterBar::OnChangeDateFilter(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	UpdateData();

	NotifyParentFilterChange();

	*pResult = 0;
}

LRESULT CTDLFilterBar::OnEEBtnClick(WPARAM wp, LPARAM /*lp*/)
{
	int nCtrl = m_aCustomControls.Find(wp);

	if (nCtrl != -1)
	{
		OnCustomAttributeChangeFilter(m_aCustomControls[nCtrl]);
	}
	else
	{
		OnSelchangeFilterAttribute();

		switch (wp)
		{
		case IDC_STARTNEXTNDAYS:
			m_cbStartFilter.SetNextNDays(m_filter.nStartNextNDays);
			break;

		case IDC_DUENEXTNDAYS:
			m_cbDueFilter.SetNextNDays(m_filter.nDueNextNDays);
			break;
		}
	}
	
	return 0L;
}

BOOL CTDLFilterBar::PreTranslateMessage(MSG* pMsg)
{
	// handle return key in title field
	if ((pMsg->message == WM_KEYDOWN) && 
		(pMsg->hwnd == m_eTitleFilter) &&
		(pMsg->wParam == VK_RETURN))
	{
		OnSelchangeFilterAttribute();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTDLFilterBar::SelectFilter(int nFilter)
{
	if (nFilter < 0 || nFilter >= m_cbTaskFilter.GetCount())
		return FALSE;

	m_cbTaskFilter.SetCurSel(nFilter);
	OnSelchangeFilter();

	return TRUE;
}

int CTDLFilterBar::GetSelectedFilter() const
{
	return m_cbTaskFilter.GetCurSel();
}

FILTER_SHOW CTDLFilterBar::GetFilter(TDCFILTER& filter, CString& sCustom, DWORD& dwCustomFlags) const
{
	sCustom.Empty();
	dwCustomFlags = 0;

	if (m_filter.IsAdvanced())
	{
		filter.Reset(FS_ADVANCED);

		sCustom = m_sAdvancedFilter;
		m_mapCustomFlags.Lookup(sCustom, dwCustomFlags);
	}
	else
	{
		filter = m_filter;
		filter.nTitleOption = m_nTitleFilter;
	}

	return filter.nShow;
}

BOOL CTDLFilterBar::SetAdvancedFilterIncludesDoneTasks(const CString& sCustom, BOOL bIncDone)
{
	if (!m_cbTaskFilter.HasAdvancedFilter(sCustom))
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD dwFlags = 0;
	m_mapCustomFlags.Lookup(sCustom, dwFlags);

	Misc::SetFlag(dwFlags, FO_HIDEDONE, !bIncDone);

	CString sActive;

	if ((GetFilter(sActive) == FS_ADVANCED) && (sCustom == sActive))
		m_cbOptions.SetSelectedOptions(dwFlags);

	m_mapCustomFlags[sCustom] = dwFlags;

	return TRUE;
}

FILTER_SHOW CTDLFilterBar::GetFilter(CString& sCustom) const
{
	return m_cbTaskFilter.GetSelectedFilter(sCustom);
}

FILTER_SHOW CTDLFilterBar::GetFilter() const
{
	return m_cbTaskFilter.GetSelectedFilter();
}

void CTDLFilterBar::AddAdvancedFilters(const CStringArray& aFilters)
{
	m_cbTaskFilter.AddAdvancedFilters(aFilters); 
}

const CStringArray& CTDLFilterBar::GetAdvancedFilterNames() const
{
	return m_cbTaskFilter.GetAdvancedFilterNames(); 
}

void CTDLFilterBar::RemoveAdvancedFilters()
{
	m_cbTaskFilter.RemoveAdvancedFilters(); 
}

void CTDLFilterBar::ShowDefaultFilters(BOOL bShow)
{
	m_cbTaskFilter.ShowDefaultFilters(bShow); 
}

void CTDLFilterBar::RefreshFilterControls(const CFilteredToDoCtrl& tdc, TDC_ATTRIBUTE nAttribID)
{
	if (tdc.IsDelayLoaded())
		return;

	if (nAttribID == TDCA_ALL)
	{
		CHoldRedraw hr(GetSafeHwnd(), NCR_PAINT | NCR_ERASEBKGND);

		// column visibility
		CTDCAttributeMap mapDiffs;

		if (m_mapVisibility.GetDifferences(tdc.GetVisibleFilterFields(), mapDiffs))
		{
			m_mapVisibility.Copy(tdc.GetVisibleFilterFields());
			RebuildOptionsCombo();
		}

		// get filter
		if (tdc.GetFilter(m_filter) == FS_ADVANCED)
		{
			m_sAdvancedFilter = tdc.GetAdvancedFilterName();

			m_mapCustomFlags[m_sAdvancedFilter] = tdc.GetAdvancedFilterFlags();
		}
		else
		{
			m_filter.nTitleOption = m_nTitleFilter;

			m_sAdvancedFilter.Empty();
		}

		// Custom attributes
		UpdateCustomControls(tdc, nAttribID);
	
		// auto droplist filters
		UpdateDropListData(tdc, nAttribID);
		
		// update UI
		RefreshUIBkgndBrush();

		UpdateData(FALSE);
		UpdateWindow();

		// disable controls if a custom filter.
		// just do a repos because this also handles enabled state
		ReposControls();
	}
	else
	{
		UpdateDropListData(tdc, nAttribID);
		UpdateCustomControls(tdc, nAttribID);
	}
}

void CTDLFilterBar::UpdateDropListData(const CFilteredToDoCtrl& tdc, TDC_ATTRIBUTE nAttribID)
{
	TDCAUTOLISTDATA tld;
	tdc.GetAutoListData(tld, nAttribID);

	BOOL bAllAttrib = (nAttribID == TDCA_ALL);

	if (bAllAttrib || (nAttribID == TDCA_ALLOCTO))
		m_cbAllocToFilter.SetStrings(tld.aAllocTo);

	if (bAllAttrib || (nAttribID == TDCA_ALLOCBY))
		m_cbAllocByFilter.SetStrings(tld.aAllocBy);

	if (bAllAttrib || (nAttribID == TDCA_CATEGORY))
		m_cbCategoryFilter.SetStrings(tld.aCategory);

	if (bAllAttrib || (nAttribID == TDCA_STATUS))
		m_cbStatusFilter.SetStrings(tld.aStatus);

	if (bAllAttrib || (nAttribID == TDCA_VERSION))
		m_cbVersionFilter.SetStrings(tld.aVersion);

	if (bAllAttrib || (nAttribID == TDCA_TAGS))
		m_cbTagFilter.SetStrings(tld.aTags);

	if (bAllAttrib)
	{
		int nCtrl = m_aCustomControls.GetSize();

		while (nCtrl--)
		{
			const CUSTOMATTRIBCTRLITEM& ctrl = m_aCustomControls[nCtrl];
			CTDCCustomAttributeUIHelper::UpdateControlAutoListData(this, ctrl, tdc.GetCustomAttributeDefs());
		}

	}
	else if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
	{
		int nCtrl = m_aCustomControls.Find(nAttribID);
		ASSERT(nCtrl != -1);

		if (nCtrl != -1)
		{
			const CUSTOMATTRIBCTRLITEM& ctrl = m_aCustomControls[nCtrl];
			CTDCCustomAttributeUIHelper::UpdateControlAutoListData(this, ctrl, tdc.GetCustomAttributeDefs());
		}
	}
}

BOOL CTDLFilterBar::SetTitleFilterOption(FILTER_TITLE nOption) 
{ 
	if (m_nTitleFilter != nOption)
	{
		m_nTitleFilter = m_filter.nTitleOption = nOption; 
		SetDlgItemText(IDC_TITLEFILTERLABEL, m_filter.GetTitleFilterLabel());

		return TRUE;
	}

	return FALSE; // no change
}

void CTDLFilterBar::UpdateCustomControls(const CFilteredToDoCtrl& tdc, TDC_ATTRIBUTE nAttribID)
{
	if (nAttribID == TDCA_ALL)
	{
		CTDCCustomAttribDefinitionArray aNewAttribDefs;
		tdc.GetCustomAttributeDefs(aNewAttribDefs);

		if (CTDCCustomAttributeUIHelper::NeedRebuildFilterControls(m_aCustomAttribDefs,
																   aNewAttribDefs,
																   m_aCustomControls))
		{
			CTDCCustomAttributeUIHelper::RebuildFilterControls(this,
															   aNewAttribDefs,
															   m_filter.mapCustomAttrib,
															   tdc.GetTaskIconImageList(),
															   IDC_OPTIONFILTERCOMBO,
															   m_bMultiSelection,
															   m_aCustomControls);
		}

		// Update data
		CTDCCustomAttributeUIHelper::UpdateControls(this, m_aCustomControls, aNewAttribDefs, m_filter.mapCustomAttrib);

		m_aCustomAttribDefs.Copy(aNewAttribDefs);
	}
	else if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
	{
		int nCtrl = m_aCustomControls.Find(nAttribID);

		if (nCtrl != -1)
		{
			CTDCCustomAttributeUIHelper::UpdateControl(this, 
														m_aCustomControls[nCtrl],
														tdc.GetCustomAttributeDefs(), 
														m_filter.mapCustomAttrib);
		}
	}
}

void CTDLFilterBar::SetFilterLabelAlignment(BOOL bLeft)
{
	UINT nAlign = bLeft ? SS_LEFT : SS_RIGHT;
	
	int nLabel = NUMFILTERCTRLS;
	
	while (nLabel--)
	{
		UINT nLabelID = FILTERCTRLS[nLabel].nLabelID;

		if (nLabelID)
		{
			CWnd* pLabel = GetDlgItem(nLabelID);
			pLabel->ModifyStyle(SS_TYPEMASK, nAlign);
			pLabel->Invalidate();
		}
	}
}

void CTDLFilterBar::SetPriorityColors(const CDWordArray& aColors)
{
	m_cbPriorityFilter.SetColors(aColors);
}

void CTDLFilterBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// check we're not in the middle of creation
	if (m_cbStatusFilter.GetSafeHwnd())
		ReposControls(cx, FALSE);
}

int CTDLFilterBar::CalcHeight(int nWidth)
{
	return ReposControls(nWidth, TRUE);
}

void CTDLFilterBar::RebuildOptionsCombo()
{
	if (m_filter.IsAdvanced())
	{
		DWORD dwFlags = TDCFILTER().dwFlags; // default
		m_mapCustomFlags.Lookup(m_sAdvancedFilter, dwFlags);

		DWORD dwOptions = (FO_HIDEOVERDUE | FO_HIDEDONE | FO_SHOWALLSUB);
		m_cbOptions.Initialize(dwOptions, dwFlags);
	}
	else
	{
		m_cbOptions.Initialize(m_filter, m_mapVisibility, m_bMultiSelection);
	}
}

BOOL CTDLFilterBar::WantShowFilter(TDC_ATTRIBUTE nType) const
{
	switch (nType)
	{
	case TDCA_NONE:
		return TRUE;

	case TDCA_TASKNAME:
		return !m_filter.IsAdvanced();

	default:
		if ((nType >= TDCA_CUSTOMATTRIB_FIRST) && (nType <= TDCA_CUSTOMATTRIB_LAST))
			return !m_filter.IsAdvanced();
		break;
	}

	return (!m_filter.IsAdvanced() && m_mapVisibility.Has(nType));
}

void CTDLFilterBar::EnableMultiSelection(BOOL bEnable)
{
	if (bEnable != m_bMultiSelection)
	{
		m_bMultiSelection = bEnable;

		m_cbCategoryFilter.EnableMultiSelection(bEnable);
		m_cbAllocToFilter.EnableMultiSelection(bEnable);
		m_cbStatusFilter.EnableMultiSelection(bEnable);
		m_cbAllocByFilter.EnableMultiSelection(bEnable);
		m_cbVersionFilter.EnableMultiSelection(bEnable);
		m_cbTagFilter.EnableMultiSelection(bEnable);

		RebuildOptionsCombo();

		CTDCCustomAttributeUIHelper::EnableMultiSelectionFilter(m_aCustomControls, this, bEnable);
		
		UpdateData(); // Pick up any changes
	}
}

int CTDLFilterBar::ReposControls(int nWidth, BOOL bCalcOnly)
{
	if (nWidth <= 0)
	{
		CRect rClient;
		GetClientRect(rClient);

		nWidth = rClient.Width();
	}

	// Note: All calculations are performed in DLU until just before the move
	// is performed. This ensures that we minimize the risk of rounding errors.
	CDlgUnits dlu(this);

	// Don't disable visible labels if the background color is not gray
	// because embossed text looks 'wrong' over a colour
	BOOL bNonGrayBkgnd = ((m_crUIBack != CLR_NONE) && !RGBX(m_crUIBack).IsGray());
	
	int nXPosDLU = 0, nYPosDLU = CTRLVSPACING;
	int nWidthDLU = dlu.FromPixelsX(nWidth);
	
	// To handle DPI scaling better simply use the height of the category combo
	int nActualCtrlHeight = GetChildHeight(&m_cbCategoryFilter);
	const int CTRLHEIGHT = dlu.FromPixelsY(nActualCtrlHeight);

	CTDCControlArray aControls;
	int nNumCtrls = GetControls(aControls);
	
	CDeferWndMove dwm(bCalcOnly ? 0 : nNumCtrls);

	for (int nCtrl = 0; nCtrl < nNumCtrls; nCtrl++)
	{
		const CTRLITEM& fc = aControls[nCtrl];
		
		// display this control only if the corresponding column
		// is also showing
		BOOL bWantCtrl = WantShowFilter(fc.nAttrib);
		
		// special case: User Dates
		switch (fc.nCtrlID)
		{
		case IDC_USERSTARTDATE:
			bWantCtrl &= (m_filter.nStartBy == FD_USER);
			break;

		case IDC_USERDUEDATE:
			bWantCtrl &= (m_filter.nDueBy == FD_USER);
			break;

		case IDC_STARTNEXTNDAYS:
			bWantCtrl &= (m_filter.nStartBy == FD_NEXTNDAYS);
			break;

		case IDC_DUENEXTNDAYS:
			bWantCtrl &= (m_filter.nDueBy == FD_NEXTNDAYS);
			break;
		}

		if (bWantCtrl)
		{
			// if we're at the start of the line then just move ctrls
			// else we must check whether there's enough space to fit
			if (nXPosDLU > 0) // we're not the first
			{
				// do we fit?
				if ((nXPosDLU + CTRLLEN) > nWidthDLU) // no
				{
					// move to next line
					nXPosDLU = 0;
					nYPosDLU += (CTRLVSPACING + LABELHEIGHT + CTRLHEIGHT);
				}
			}
			
			// move label
			CRect rCtrlDLU(nXPosDLU, nYPosDLU, nXPosDLU + CTRLLEN, nYPosDLU + LABELHEIGHT);
			CRect rCtrl = rCtrlDLU;

			dlu.ToPixels(rCtrl);
			
			if (fc.nLabelID && !bCalcOnly)
				dwm.MoveWindow(GetDlgItem(fc.nLabelID), rCtrl);
			
			// move ctrl
			rCtrlDLU.OffsetRect(0, LABELHEIGHT);
			rCtrl = rCtrlDLU;
			
			dlu.ToPixels(rCtrl);
			rCtrl.bottom = (rCtrl.top + nActualCtrlHeight);
			
			if (!bCalcOnly)
			{
				// add 200 to combo dropdowns
				CWnd* pCtrl = GetDlgItem(fc.nCtrlID);
				
				if (CWinClasses::IsComboBox(*pCtrl))
					rCtrl.bottom += GraphicsMisc::ScaleByDPIFactor(200);
				
				dwm.MoveWindow(pCtrl, rCtrl);
			}
			
			// update XPos for the control
			nXPosDLU = rCtrlDLU.right + CTRLHSPACING;
		}

		// show/hide and enable as appropriate
		if (!bCalcOnly)
		{
			BOOL bEnable = bWantCtrl;
			
			// special cases
			switch (fc.nCtrlID)
			{
			case IDC_USERSTARTDATE:
				bEnable &= (m_filter.nStartBy == FD_USER);
				break;

			case IDC_USERDUEDATE:
				bEnable &= (m_filter.nDueBy == FD_USER);
				break;

			case IDC_FILTERCOMBO: 
			case IDC_OPTIONFILTERCOMBO:
				// always enabled
				break;

			default: // all the rest
				bEnable &= ((m_filter.nShow != FS_SELECTED) && !m_filter.IsAdvanced());
			}

			if (fc.nLabelID)
			{
				GetDlgItem(fc.nLabelID)->ShowWindow(bWantCtrl ? SW_SHOW : SW_HIDE);
				GetDlgItem(fc.nLabelID)->EnableWindow(bNonGrayBkgnd ? bWantCtrl : bEnable);
			}
			
			GetDlgItem(fc.nCtrlID)->ShowWindow(bWantCtrl ? SW_SHOW : SW_HIDE);
			GetDlgItem(fc.nCtrlID)->EnableWindow(bEnable);
		}
	}

	// update bottom of filter bar
	nYPosDLU += (LABELHEIGHT + CTRLHEIGHT + CTRLVSPACING);

	return dlu.ToPixelsY(nYPosDLU);
}

int CTDLFilterBar::GetControls(CTDCControlArray& aControls) const
{
	aControls.RemoveAll();

	// standard controls, except for options which is always last
	int nCtrl;
	for (nCtrl = 0; nCtrl < NUMFILTERCTRLS - 1; nCtrl++)
	{
		aControls.Add(FILTERCTRLS[nCtrl]);
	}

	// custom attribs
	for (nCtrl = 0; nCtrl < m_aCustomControls.GetSize(); nCtrl++)
	{
		CUSTOMATTRIBCTRLITEM ctrl = m_aCustomControls[nCtrl];
		aControls.Add(ctrl);

		// Buddy Control
		CTRLITEM buddy;

		if (ctrl.GetBuddy(buddy) && ctrl.IsShowingBuddy())
			aControls.Add(buddy);
	}

	// finally options combo
	aControls.Add(FILTERCTRLS[NUMFILTERCTRLS - 1]);

	return aControls.GetSize();
}

BOOL CTDLFilterBar::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// disable translation of user-data combos
	CLocalizer::EnableTranslation(m_cbAllocByFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbAllocToFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbCategoryFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbStatusFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbVersionFilter, FALSE);
	CLocalizer::EnableTranslation(m_cbTagFilter, FALSE);
	
	m_eTitleFilter.ModifyStyle(0, ES_WANTRETURN, 0);
	m_mgrPrompts.SetEditPrompt(m_eTitleFilter, CEnString(IDS_TDC_ANY));
	m_mgrPrompts.SetComboPrompt(m_cbOptions, CEnString(IDS_TDC_NONE));
	
	SetDlgItemText(IDC_TITLEFILTERLABEL, m_filter.GetTitleFilterLabel());
	EnableToolTips();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLFilterBar::OnToolTipNotify(UINT /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
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

void CTDLFilterBar::SetUITheme(const CUIThemeFile& theme)
{
	if ((m_theme.crAppBackDark != theme.crAppBackDark) ||
		(m_theme.crAppBackLight != theme.crAppBackLight) ||
		(m_theme.crAppText != theme.crAppText))
	{
		m_theme = theme;
		RefreshUIBkgndBrush();
	}
}

void CTDLFilterBar::RefreshUIBkgndBrush()
{
	if (CThemed::IsAppThemed())
	{
		COLORREF crPrev(m_crUIBack);
		m_crUIBack = CalcUIBkgndColor();

		if (m_crUIBack != crPrev)
		{
			GraphicsMisc::VerifyDeleteObject(m_brUIBack);

			if (m_crUIBack != CLR_NONE)
				m_brUIBack.CreateSolidBrush(m_crUIBack);
			
			Invalidate();
			return;
		}
	}
}

COLORREF CTDLFilterBar::CalcUIBkgndColor() const
{
	if (CThemed::IsAppThemed())
	{
		if (m_filter.IsSet() || m_filter.IsAdvanced())
		{
			if (m_theme.crAppBackDark != CLR_NONE)
			{
				return GraphicsMisc::Darker(m_theme.crAppBackDark, 0.1, FALSE);
			}
			else if (m_theme.crAppBackLight != CLR_NONE)
			{
				return GraphicsMisc::Darker(m_theme.crAppBackLight, 0.3, FALSE);
			}
		}
		else if (m_theme.crAppBackLight != CLR_NONE)
		{
			return m_theme.crAppBackLight;
		}
	}

	// else
	return CLR_NONE;
}

void CTDLFilterBar::OnPaint()
{
	CPaintDC dc(this);

	// Paint controls and labels
	CDialog::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);

	// Fill remaining background
	if (m_crUIBack != CLR_NONE)
	{
		CRect rClient;
		GetClientRect(rClient);

		GraphicsMisc::DrawHorzLine(&dc, 0, rClient.Width(), rClient.bottom - 2, m_theme.crAppLinesDark);
		GraphicsMisc::DrawHorzLine(&dc, 0, rClient.Width(), (rClient.bottom - 1), m_theme.crAppLinesLight);

		rClient.top = rClient.bottom - 2;
		dc.ExcludeClipRect(rClient);

		int nCtrl = NUMFILTERCTRLS;

		while (nCtrl--)
		{
			ExcludeCtrl(this, FILTERCTRLS[nCtrl].nLabelID, &dc);
			ExcludeCtrl(this, FILTERCTRLS[nCtrl].nCtrlID, &dc);
		}

		// Custom
		nCtrl = m_aCustomControls.GetSize();

		while (nCtrl--)
		{
			ExcludeCtrl(this, m_aCustomControls[nCtrl].nLabelID, &dc);
			ExcludeCtrl(this, m_aCustomControls[nCtrl].nCtrlID, &dc);
		}

		CRect rect;
		dc.GetClipBox(rect);
		dc.FillSolidRect(rect, m_crUIBack);
	}
}

BOOL CTDLFilterBar::OnEraseBkgnd(CDC* pDC) 
{
	if (m_crUIBack != CLR_NONE)
	{
		// Handle background in OnPaint
		return TRUE;
	}

	// else
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CTDLFilterBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Handle label background and text colors when themed
	if (CThemed::IsAppThemed() && (nCtlColor == CTLCOLOR_STATIC) && CWinClasses::IsClass(*pWnd, WC_STATIC))
	{
		pDC->SetTextColor(m_theme.crAppText);

		if (m_brUIBack.GetSafeHandle())
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_brUIBack;
		}
	}
	
	return hbr;
}

void CTDLFilterBar::OnCustomAttributeSelchangeFilter(UINT nCtrlID)
{
	ASSERT(CTDCCustomAttributeUIHelper::IsCustomFilterControl(nCtrlID));

	int nCtrl = m_aCustomControls.Find(nCtrlID);

	if (nCtrl != -1)
	{
		// Common helper
		OnCustomAttributeChangeFilter(m_aCustomControls[nCtrl]);
	}
	else
	{
		ASSERT(0);
	}
}

void CTDLFilterBar::OnCustomAttributeChangeDateFilter(UINT nCtrlID, NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	int nCtrl = m_aCustomControls.Find(nCtrlID);

	if (nCtrl != -1)
	{
		CUSTOMATTRIBCTRLITEM& ctrl = m_aCustomControls[nCtrl];

		ASSERT(nCtrlID == ctrl.nBuddyCtrlID);

		// Common helper
		OnCustomAttributeChangeFilter(ctrl);
	}
	else
	{
		ASSERT(0);
	}
}

void CTDLFilterBar::OnCustomAttributeChangeFilter(CUSTOMATTRIBCTRLITEM& ctrl)
{
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(ctrl.nAttrib));
	ASSERT(CTDCCustomAttributeUIHelper::IsCustomFilterControl(ctrl.nCtrlID));

	TDCCADATA data, dataPrev;
	m_filter.mapCustomAttrib.Lookup(ctrl.sAttribID, dataPrev);

	TDCCAUI_UPDATERESULT nRes = CTDCCustomAttributeUIHelper::GetControlData(this, ctrl, m_aCustomAttribDefs, dataPrev, data);

	if (data.IsEmpty())
		m_filter.mapCustomAttrib.RemoveKey(ctrl.sAttribID);
	else
		m_filter.mapCustomAttrib[ctrl.sAttribID] = data;
	
	if (data != dataPrev)
		NotifyParentFilterChange();

	if (nRes == TDCCAUIRES_REPOSCTRLS)
		ReposControls();
}

void CTDLFilterBar::OnCustomAttributeSelcancelFilter(UINT nCtrlID)
{
	ASSERT(CTDCCustomAttributeUIHelper::IsCustomFilterControl(nCtrlID));

	CUSTOMATTRIBCTRLITEM ctrl;

	if (CTDCCustomAttributeUIHelper::GetControl(nCtrlID, m_aCustomControls, ctrl))
	{
		// Restore previous state
		CTDCCustomAttributeUIHelper::UpdateControl(this, ctrl, m_aCustomAttribDefs, m_filter.mapCustomAttrib);
		CTDCCustomAttributeUIHelper::ClearFilterCheckboxHistory(ctrl, this);
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CTDLFilterBar::CanPasteText() const
{
	return (::GetFocus() == m_eTitleFilter);
}

