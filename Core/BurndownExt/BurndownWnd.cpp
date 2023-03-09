// BurndownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownWnd.h"
#include "BurndownStatic.h"

#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\WorkingWeek.h"
#include "..\shared\AutoFlag.h"

#include "..\3rdparty\GdiPlus.h"
#include "..\3rdparty\XNamedColors.h"

#include "..\Interfaces\ipreferences.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT WM_REBUILDGRAPH = (WM_APP+1);

/////////////////////////////////////////////////////////////////////////////

enum // m_dwUpdateGraphOnShow
{
	REBUILD_GRAPH	= 0x01,
	RESORT_DATA		= 0x02,
	UPDATE_EXTENTS	= 0x04
};

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd dialog

CBurndownWnd::CBurndownWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_STATISTICS_DLG, pParent),
	m_nGraph(BCT_TIMESERIES_REMAININGDAYS),
	m_dwUpdateGraphOnShow(0),
	m_dtDataRange(DHD_BEGINTHISMONTH, DHD_ENDTHISMONTH),
	m_chart(m_data),
	m_dlgPrefs(m_chart, this),
	m_bUpdatingSlider(FALSE),
	m_sliderDateRange(TBS_BOTTOM),
	m_bVisible(FALSE)
{
	//{{AFX_DATA_INIT(CBurndownWnd)
	//}}AFX_DATA_INIT
	m_icon.Load(IDR_STATISTICS);
	m_sliderDateRange.SetMinMaxRangeWidths(1.0); // min range = 1 month
}

CBurndownWnd::~CBurndownWnd()
{

}

void CBurndownWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurndownWnd)
	DDX_Control(pDX, IDC_FRAME, m_stFrame);
	DDX_Control(pDX, IDC_DISPLAY, m_cbGraphs);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_OPTIONS, m_cbOptions);
	DDX_Control(pDX, IDC_ACTIVEDATERANGE, m_sliderDateRange);

	m_cbGraphs.DDX(pDX, m_nGraph);
}


BEGIN_MESSAGE_MAP(CBurndownWnd, CDialog)
	//{{AFX_MSG_MAP(CBurndownWnd)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_BURNDOWN_PREF, OnPreferences)
	ON_WM_HELPINFO()
	ON_CBN_SELENDOK(IDC_DISPLAY, OnSelchangeDisplay)
	ON_CBN_SELENDOK(IDC_OPTIONS, OnOptionChanged)
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()

	ON_MESSAGE(WM_REBUILDGRAPH, OnRebuildGraph)
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnActiveDateRangeChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd message handlers

void CBurndownWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	
	delete this;
}

void CBurndownWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CBurndownWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

void CBurndownWnd::OnPreferences()
{
	if (m_dlgPrefs.DoModal() == IDOK)
	{
		m_chart.SetGraphColors(m_dlgPrefs.GetGraphColors());
		m_chart.SetShowEmptyFrequencyValues(m_dlgPrefs.GetShowEmptyFrequencyValues());
	}
}

BOOL CBurndownWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	UINT nSize = sizeof(CPlex);

	if (CDialog::Create(IDD_STATISTICS_DLG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

BOOL CBurndownWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// create toolbar
	if (m_toolbar.CreateEx(this))
	{
		VERIFY(m_toolbar.LoadToolBar(IDR_TOOLBAR, IDB_TOOLBAR_STD, colorMagenta));
		VERIFY(m_tbHelper.Initialize(&m_toolbar, this));

		CRect rToolbar = CDialogHelper::GetCtrlRect(this, IDC_TB_PLACEHOLDER);
		m_toolbar.Resize(rToolbar.Width(), rToolbar.TopLeft());
		m_toolbar.RefreshButtonStates(TRUE);
	}

	// Initialise graph
	CRect rFrame;
	m_stFrame.GetWindowRect(rFrame);
	ScreenToClient(rFrame);
	rFrame.DeflateRect(1, 1);

	VERIFY(m_chart.SubclassDlgItem(IDC_GRAPH, this));
	VERIFY(m_cbGraphs.Initialise(m_chart));

	RebuildGraph(FALSE, FALSE, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBurndownWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//CString sKey(szKey);

	pPrefs->WriteProfileInt(szKey, _T("ActiveGraph"), m_nGraph);

	// Active date range
	pPrefs->DeleteProfileSection(_T("ActiveRange"));

	COleDateTimeRange dtActiveRange;

	if (m_sliderDateRange.HasSelectedRange())
	{
		VERIFY(GetSliderDateRange(dtActiveRange));

		pPrefs->WriteProfileDouble(_T("ActiveRange"), _T("Start"), dtActiveRange.GetStart().m_dt);
		pPrefs->WriteProfileDouble(_T("ActiveRange"), _T("End"), dtActiveRange.GetEnd().m_dt);
	}
	else
	{
		pPrefs->DeleteProfileSection(_T("ActiveRange"));
	}

	m_chart.SavePreferences(pPrefs, szKey);
	m_dlgPrefs.SavePreferences(pPrefs, szKey);
}

void CBurndownWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// burn down specific options
	if (!bAppOnly)
	{
		m_nGraph = (BURNDOWN_GRAPH)pPrefs->GetProfileInt(szKey, _T("ActiveGraph"), BCT_TIMESERIES_INCOMPLETETASKS);
		
		if (!IsValidGraph(m_nGraph))
			m_nGraph = BCT_TIMESERIES_INCOMPLETETASKS;

		m_chart.LoadPreferences(pPrefs, szKey);
		m_dlgPrefs.LoadPreferences(pPrefs, szKey);

		m_chart.SetActiveGraph(m_nGraph);
		m_chart.SetShowEmptyFrequencyValues(m_dlgPrefs.GetShowEmptyFrequencyValues());
		
		m_cbOptions.SetActiveGraph(m_nGraph);
		m_cbOptions.SetSelectedOption(m_chart.GetActiveGraphOption());

		// Active range
		m_dtPrevActiveRange.Reset();

		double dStart = pPrefs->GetProfileDouble(_T("ActiveRange"), _T("Start"), 0);
		double dEnd = pPrefs->GetProfileDouble(_T("ActiveRange"), _T("End"), -1);

		if (dEnd > dStart)
			VERIFY(m_dtPrevActiveRange.Set(dStart, dEnd));

		UpdateData(FALSE);
	}

	// application preferences
	DWORD dwWeekends = pPrefs->GetProfileInt(_T("Preferences"), _T("Weekends"), (DHW_SATURDAY | DHW_SUNDAY));
	double dHoursInDay = _ttof(pPrefs->GetProfileString(_T("Preferences"), _T("HoursInDay"), _T("8")));

	CWorkingWeek::Initialise(dwWeekends, dHoursInDay);
}

void CBurndownWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);
	
	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(pTheme->crAppBackLight);
		
		// intentionally set background colours to be same as ours
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, FALSE, FALSE);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);

		m_sliderDateRange.SetParentBackgroundColor(m_theme.crAppBackLight);
		m_chart.SetTodayColour(m_theme.crToday);
	}
}

void CBurndownWnd::SetTaskFont(HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sFontName;
	int nFontSize = GraphicsMisc::GetFontNameAndPointSize(hFont, sFontName);

	m_chart.SetFont(sFontName, nFontSize);
}

bool CBurndownWnd::ProcessMessage(MSG* /*pMsg*/) 
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
//	if (!IsWindowEnabled())
//		return false;

	// process editing shortcuts
	// TODO

	return false;
}

void CBurndownWnd::FilterToolTipMessage(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_chart.FilterToolTipMessage(pMsg);
}

bool CBurndownWnd::GetLabelEditRect(LPRECT /*pEdit*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false;
}

bool CBurndownWnd::PrepareNewTask(ITaskList* /*pTask*/) const 
{ 
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; 
}

IUI_HITTEST CBurndownWnd::HitTest(POINT /*ptScreen*/) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return IUI_NOWHERE;
}

DWORD CBurndownWnd::HitTestTask(POINT /*ptScreen*/, bool /*bTitleColumnOnly*/) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return 0L;
}

bool CBurndownWnd::SelectTask(DWORD /*dwTaskID*/, bool /*bTaskLink*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// because we can't change the selection
	// in this plugin we don't care what it's set to
	return false;
}

bool CBurndownWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// because we can't change the selection
	// in this plugin we don't care what it's set to
	return false;
}

bool CBurndownWnd::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nAttribute)
	{
	case TDCA_DONEDATE:
	case TDCA_STARTDATE:
	case TDCA_CREATIONDATE:
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
	case TDCA_COST:
	case TDCA_ALLOCBY:
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_PRIORITY:
	case TDCA_RISK:
	case TDCA_TAGS:
	case TDCA_VERSION:
		return true;
	}

	// all else 
	return false;
}

void CBurndownWnd::BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings, BOOL bCheckExist)
{
	if (hTask == NULL)
		return;

	// Not interested in references
	if (!pTasks->IsTaskReference(hTask))
	{
		// only interested in leaf tasks
		if (!pTasks->IsTaskParent(hTask))
		{
			STATSITEM* pSI = m_data.AddItem(pTasks->GetTaskID(hTask));

			if (pSI) // means it's new
				pSI->Set(pTasks, hTask);
		}
		else // Process children
		{
			BuildData(pTasks, pTasks->GetFirstTask(hTask), TRUE, bCheckExist);
		}
	}

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildData(pTasks, hSibling, FALSE, bCheckExist);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CBurndownWnd::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	// we must have been initialized already
	ASSERT(m_chart.GetSafeHwnd());

	switch (nUpdate)
	{
	case IUI_ALL:
		m_data.RemoveAll();

		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, FALSE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_NEW:
		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, TRUE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_EDIT:
		UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, TRUE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_DELETE:
		if (RemoveDeletedTasks(pTasks))
			RebuildGraph(FALSE, TRUE, TRUE);
		break;
		
	default:
		ASSERT(0);
	}
}

void CBurndownWnd::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings)
{
	// handle task if not NULL (== root)
	if (hTask == NULL)
		return;

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return;
	
	ASSERT(nUpdate == IUI_EDIT);

	// only interested in leaf tasks
	if (!pTasks->IsTaskParent(hTask))
	{
		DWORD dwTaskID = pTasks->GetTaskID(hTask);

		// if the task we've been given is actually a parent task
		// then we'll have no record of it, so the following
		// lookup has a legitimate reason to fail
		STATSITEM* pSI = m_data.GetItem(dwTaskID);
		ASSERT(pSI);
		
		if (pSI)
		{
			pSI->Update(pTasks, hTask);
		}
		else
		{
			int breakpoint = 0;
		}
	}
	
	// children
	UpdateTask(pTasks, pTasks->GetFirstTask(hTask), nUpdate, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			UpdateTask(pTasks, hSibling, nUpdate, FALSE);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

BOOL CBurndownWnd::RemoveDeletedTasks(const ITASKLISTBASE* pTasks)
{
	// iterating sorted data is quickest
	int nOrgCount = m_data.GetSize();
	int nItem = nOrgCount;

	while (nItem--)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (!pTasks->FindTask(pSI->dwTaskID))
			m_data.RemoveAt(nItem);
	}

	return (m_data.GetSize() != nOrgCount);
}

bool CBurndownWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_SAVETOIMAGE:
		if (pData && !m_data.IsEmpty())
		{
			CBitmap bmImage;

			if (m_chart.SaveToImage(bmImage))
				return (CGdiPlusBitmap(bmImage).SaveAsFile(pData->szFilePath) != FALSE);
		}
		break;

	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_SORT:
	case IUI_SETFOCUS:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;
	}

	return false;
}

bool CBurndownWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* /*pData*/) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_SAVETOIMAGE:
		return !m_data.IsEmpty();

	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_SORT:
	case IUI_SETFOCUS:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;
	}

	return false;
}

BOOL CBurndownWnd::OnEraseBkgnd(CDC* pDC) 
{
	CDialogHelper::ExcludeChild(&m_chart, pDC);
	CDialogHelper::ExcludeChild(&m_cbGraphs, pDC);
	CDialogHelper::ExcludeChild(&m_cbOptions, pDC);
	CDialogHelper::ExcludeChild(&m_sliderDateRange, pDC);
	CDialogHelper::ExcludeChild(&m_toolbar, pDC);

	// then our background
	if (m_brBack.GetSafeHandle())
	{
		CRect rClient;
		GetClientRect(rClient);

		pDC->FillSolidRect(rClient, m_theme.crAppBackLight);
		return TRUE;
	}
	
	// else
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CBurndownWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC && m_brBack.GetSafeHandle())
	{
		pDC->SetTextColor(m_theme.crAppText);
		pDC->SetBkMode(TRANSPARENT);

		hbr = m_brBack;
	}

	return hbr;
}

void CBurndownWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_stFrame.GetSafeHwnd())
	{
		CRect rFrame;
		m_stFrame.GetWindowRect(rFrame);
		
		ScreenToClient(rFrame);
		rFrame.right = cx;
		rFrame.bottom = cy;

		m_stFrame.MoveWindow(rFrame);

		rFrame.DeflateRect(1, 1);
		m_chart.MoveWindow(rFrame);

		// selected task dates takes available space
		CRect rSlider = CDialogHelper::GetChildRect(&m_sliderDateRange);

		CDialogHelper::ResizeChild(&m_sliderDateRange, (cx - 10 - rSlider.right), 0);
		CDialogHelper::ResizeCtrl(this, IDC_ACTIVEDATERANGE_LABEL, (cx - 10 - rSlider.right), 0);

		CAutoFlag af(m_bUpdatingSlider, TRUE);
		UpdateRangeSliderStep();
	}
}

void CBurndownWnd::RebuildGraph(BOOL bSortData, BOOL bUpdateExtents, BOOL bCheckVisibility)
{
	if (bCheckVisibility && !m_bVisible)
	{
		m_dwUpdateGraphOnShow = REBUILD_GRAPH;
		m_dwUpdateGraphOnShow |= (bSortData ? RESORT_DATA : 0);
		m_dwUpdateGraphOnShow |= (bUpdateExtents ? UPDATE_EXTENTS : 0);
		
		return;
	}

	if (bSortData)
		m_data.Sort();
	
	if (bUpdateExtents)
		m_data.GetDataExtents(m_dtDataRange);

	// Only restore previous range if it's wholly contained within the data
	COleDateTime dtDataStart = CDateHelper::GetStartOfMonth(m_dtDataRange.GetStart());
	COleDateTime dtDataEnd = CDateHelper::GetEndOfMonth(m_dtDataRange.GetEnd());
		
	COleDateTimeRange dtActiveRange(dtDataStart, dtDataEnd);

	if (m_dtPrevActiveRange.IsValid() && m_dtDataRange.IsValid())
	{
		COleDateTimeRange dtDataRange;

		if (dtDataRange.Set(dtDataStart, dtDataEnd) && dtDataRange.Contains(m_dtPrevActiveRange))
			dtActiveRange = m_dtPrevActiveRange;

		m_dtPrevActiveRange.Reset(); // always
	}

	m_chart.RebuildGraph(dtActiveRange);

	UpdateRangeSlider(dtActiveRange);
	UpdateActiveRangeLabel(dtActiveRange);
}

void CBurndownWnd::OnSelchangeDisplay()
{
	UpdateData();

	m_chart.SetActiveGraph(m_nGraph);

	m_cbOptions.SetActiveGraph(m_nGraph);
	m_cbOptions.SetSelectedOption(m_chart.GetActiveGraphOption());
}

void CBurndownWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	m_bVisible = bShow;

	if (bShow && m_dwUpdateGraphOnShow)
	{
		ASSERT(m_dwUpdateGraphOnShow & REBUILD_GRAPH);
		SendMessage(WM_REBUILDGRAPH);
	}
}

LRESULT CBurndownWnd::OnRebuildGraph(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(m_dwUpdateGraphOnShow & REBUILD_GRAPH);

	BOOL bSortData = (m_dwUpdateGraphOnShow & RESORT_DATA);
	BOOL bUpdateExtents = (m_dwUpdateGraphOnShow & UPDATE_EXTENTS);

	m_dwUpdateGraphOnShow = 0;

	RebuildGraph(bSortData, bUpdateExtents, FALSE);

	return 0L;
}

BOOL CBurndownWnd::GetSliderDateRange(COleDateTimeRange& dtActiveRange) const
{
	COleDateTime dtActiveStart(CDateHelper::GetDateFromMonths((int)m_sliderDateRange.GetLeft()));
	COleDateTime dtActiveEnd(CDateHelper::GetDateFromMonths((int)m_sliderDateRange.GetRight() - 1));

	return dtActiveRange.Set(dtActiveStart, dtActiveEnd);
}

LRESULT CBurndownWnd::OnActiveDateRangeChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (!m_bUpdatingSlider)
	{
		COleDateTimeRange dtActiveRange;
		
		if (GetSliderDateRange(dtActiveRange))
		{
			UpdateActiveRangeLabel(dtActiveRange);
			m_chart.RebuildGraph(dtActiveRange);
		}
	}

	return 0L;
}

void CBurndownWnd::UpdateRangeSliderStep()
{
	// Set the tick spacing to match the current scale
	int nStep = 1;

// 	switch (m_graph.GetCurrentScale())
// 	{
// 	case BCS_DAY:
// 	case BCS_WEEK:
// 	case BCS_MONTH:		break;
// 
// 	case BCS_2MONTH:	nStep = 2;	break;
// 	case BCS_QUARTER:	nStep = 3;	break;
// 	case BCS_HALFYEAR:	nStep = 6;	break;
// 	case BCS_YEAR:		nStep = 12;	break;
// 
// 	default:			ASSERT(0);	break;
// 	}

	m_sliderDateRange.SetStep(nStep);
}

void CBurndownWnd::UpdateRangeSlider(const COleDateTimeRange& dtActiveRange)
{
	CAutoFlag af(m_bUpdatingSlider, TRUE);

	if (m_dtDataRange.IsValid())
	{
		int nDataStart = CDateHelper::GetDateInMonths(m_dtDataRange.GetStart());
		int nDataEnd = CDateHelper::GetDateInMonths(m_dtDataRange.GetEnd()) + 1;
		nDataEnd = max(nDataEnd, nDataStart + 1);

		m_sliderDateRange.SetMinMax(nDataStart, nDataEnd);
		
		int nActiveStart = CDateHelper::GetDateInMonths(dtActiveRange.GetStart());
		int nActiveEnd = CDateHelper::GetDateInMonths(dtActiveRange.GetEnd()) + 1;
		nActiveEnd = max(nActiveEnd, nActiveStart + 1);

		nActiveStart = max(nActiveStart, nDataStart);
		nActiveEnd = min(nActiveEnd, nDataEnd);

		m_sliderDateRange.SetRange(nActiveStart, nActiveEnd);
		m_sliderDateRange.EnableWindow(TRUE);
	}
	else
	{
		m_sliderDateRange.SetMinMax(0, 1.0);
		m_sliderDateRange.SetRange(0.0, 1.0);
		m_sliderDateRange.EnableWindow(FALSE);
	}

	UpdateRangeSliderStep();
}

void CBurndownWnd::UpdateActiveRangeLabel(const COleDateTimeRange& dtActiveRange)
{
	CString sRange;

	if (dtActiveRange.IsValid())
	{
		COleDateTime dtStart = dtActiveRange.GetStart();

		if (m_sliderDateRange.GetRangeWidth() == 1.0)
		{
			sRange.Format(_T("%s %d"), CDateHelper::GetMonthName(dtStart.GetMonth(), TRUE), dtStart.GetYear());
		}
		else
		{
			COleDateTime dtEnd = dtActiveRange.GetEnd();
			CString sStart, sEnd;

			sStart.Format(_T("%s %d"), CDateHelper::GetMonthName(dtStart.GetMonth(), TRUE), dtStart.GetYear());
			sEnd.Format(_T("%s %d"), CDateHelper::GetMonthName(dtEnd.GetMonth(), TRUE), dtEnd.GetYear());

			sRange.Format(_T("%s - %s"), sStart, sEnd);
		}
	}
	
	SetDlgItemText(IDC_ACTIVEDATERANGE_LABEL, CEnString(IDS_ACTIVEDATERANGE, sRange));
}

void CBurndownWnd::OnOptionChanged()
{
	m_chart.SetActiveGraphOption(m_cbOptions.GetSelectedOption());
}
